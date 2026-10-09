// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test.cpp"

// These tests run cgi.cgi with crafted environment variables and stdin.
// A file, urlhttprequest.rest, collects whatever cgi.cgi leaves unread
// on stdin.

#define BODY "urlhttprequest.body"
#define OUT "urlhttprequest.out"
#define REST "urlhttprequest.rest"

struct cgirun {
	bool	exitedcleanly;
	time_t	elapsed;
	char	*output;
	size_t	restsize;
};

static void runCgi(const char *method,
			const char *contenttype,
			const char *contentlength,
			const char *querystring,
			const char *body,
			cgirun *run) {

	// write stdin
	file	f;
	f.open(BODY,O_WRONLY|O_CREAT|O_TRUNC,
			permissions::parsePermString("rw-r--r--"));
	f.write(body);
	f.close();

	// build the command
	stringbuffer	cmd;
	cmd.append("( env -i PATH=\"$PATH\" SCRIPT_NAME=/cgi.cgi "
			"DOCUMENT_ROOT=. SERVER_NAME=localhost");
	cmd.append(" REQUEST_METHOD='")->append(method)->append("'");
	if (contenttype) {
		cmd.append(" CONTENT_TYPE='")->append(contenttype)->append("'");
	}
	if (contentlength) {
		cmd.append(" CONTENT_LENGTH='")->
				append(contentlength)->append("'");
	}
	if (querystring) {
		cmd.append(" QUERY_STRING='")->
				append(querystring)->append("'");
	}
	cmd.append(" timeout 30 ./cgi.cgi > " OUT "; cat > " REST " ) < " BODY
			" 2> /dev/null");

	// run it
	time_t	start=time(NULL);
	int	result=system(cmd.getString());
	run->elapsed=time(NULL)-start;
	run->exitedcleanly=(result==0);
	run->output=file::getContents(OUT);
	if (!run->output) {
		run->output=charstring::duplicate("");
	}
	char	*rest=file::getContents(REST);
	run->restsize=charstring::getLength(rest);
	delete[] rest;

	file::remove(BODY);
	file::remove(OUT);
	file::remove(REST);
}

static bool hasParameter(cgirun *run, const char *nameandvalue) {
	stringbuffer	line;
	line.append("\n\t")->append(nameandvalue)->append("\n");
	return charstring::contains(run->output,line.getString());
}

int main() {

	header("urlhttprequest");

	cgirun	run;

	// sanity check
	runCgi("POST","application/x-www-form-urlencoded","19",NULL,
				"var1=val1&var2=val2",&run);
	test("urlencoded post, exit status",run.exitedcleanly);
	test("urlencoded post, count",
		charstring::contains(run.output,"All Parameters: 2\n"));
	test("urlencoded post, var1",hasParameter(&run,"var1=val1"));
	test("urlencoded post, var2",hasParameter(&run,"var2=val2"));
	test("urlencoded post, nothing left",run.restsize==0);
	delete[] run.output;
	stdoutput.printf("\n");

	// get with a body and no query string
	runCgi("GET",NULL,"5",NULL,"a=b&c",&run);
	test("get with body, exit status",run.exitedcleanly);
	test("get with body, no parameters",
		charstring::contains(run.output,"All Parameters: 0\n"));
	test("get with body, body left for app",run.restsize==5);
	delete[] run.output;
	stdoutput.printf("\n");

	// get with a query string
	runCgi("GET",NULL,NULL,"var1=a%20b&var2=c",NULL,&run);
	test("get, exit status",run.exitedcleanly);
	test("get, var1",hasParameter(&run,"var1=a b"));
	test("get, var2",hasParameter(&run,"var2=c"));
	delete[] run.output;
	stdoutput.printf("\n");

	// multipart with no boundary
	runCgi("POST","multipart/form-data","20",NULL,
				"--b\r\n\r\nvalue\r\n--b--\r\n",&run);
	test("multipart, no boundary, exit status",run.exitedcleanly);
	test("multipart, no boundary, no parameters",
		charstring::contains(run.output,"All Parameters: 0\n"));
	delete[] run.output;
	stdoutput.printf("\n");

	// mixed-case content types
	runCgi("POST","Application/X-WWW-Form-Urlencoded","19",NULL,
				"var1=val1&var2=val2",&run);
	test("mixed-case urlencoded, exit status",run.exitedcleanly);
	test("mixed-case urlencoded, var1",hasParameter(&run,"var1=val1"));
	test("mixed-case urlencoded, var2",hasParameter(&run,"var2=val2"));
	delete[] run.output;

	const char	multipartbody[]=
			"--xyz\r\n"
			"Content-Disposition: form-data; name=\"input1\"\r\n"
			"\r\n"
			"value1\r\n"
			"--xyz\r\n"
			"Content-Disposition: form-data; name=\"input2\"\r\n"
			"\r\n"
			"value2\r\n"
			"--xyz--\r\n";
	char	length[20];
	charstring::printf(length,sizeof(length),"%d",
				(int)charstring::getLength(multipartbody));

	runCgi("POST","Multipart/Form-Data; Boundary=xyz",length,NULL,
				multipartbody,&run);
	test("mixed-case multipart, exit status",run.exitedcleanly);
	test("mixed-case multipart, count",
		charstring::contains(run.output,"All Parameters: 2\n"));
	test("mixed-case multipart, input1",
		charstring::contains(run.output,"\n\tinput1=value1"));
	test("mixed-case multipart, input2",
		charstring::contains(run.output,"\n\tinput2=value2"));
	delete[] run.output;
	stdoutput.printf("\n");

	// multipart with an exact content length: trailing crlf is drained
	runCgi("POST","multipart/form-data; boundary=xyz",length,NULL,
				multipartbody,&run);
	test("multipart, exit status",run.exitedcleanly);
	test("multipart, input1",
		charstring::contains(run.output,"\n\tinput1=value1"));
	test("multipart, nothing left on stdin",run.restsize==0);
	delete[] run.output;
	stdoutput.printf("\n");

	// huge content length with a short body
	runCgi("POST","application/x-www-form-urlencoded","10000000",NULL,
				"var1=val1&var2=val2",&run);
	test("huge content length, exit status",run.exitedcleanly);
	test("huge content length, finishes quickly",run.elapsed<10);
	test("huge content length, var1",hasParameter(&run,"var1=val1"));
	test("huge content length, var2",hasParameter(&run,"var2=val2"));
	delete[] run.output;
	stdoutput.printf("\n");

	return 0;
}
