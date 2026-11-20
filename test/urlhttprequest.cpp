// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
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
#define VG "urlhttprequest.vg"

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

static bool hasSkinVariable(cgirun *run, const char *nameandvalue) {
	stringbuffer	line;
	line.append("\n")->append(nameandvalue)->append("\n");
	return charstring::contains(run->output,line.getString());
}

static bool isBlankValgrindLine(const char *line) {

	// skip the ==pid== prefix
	if (line[0]!='=' || line[1]!='=') {
		return false;
	}
	const char	*ptr=charstring::findFirst(line+2,"==");
	if (!ptr) {
		return false;
	}

	// check for anything after it
	for (ptr=ptr+2; *ptr; ptr++) {
		if (!character::isWhitespace(*ptr)) {
			return false;
		}
	}
	return true;
}

// Runs cgi.cgi under valgrind with the skin files that "pathinfo" selects
// from skinvars/, counts the definite leak records of the copies that
// setSkinVariable() makes, and counts the invalid read, write, and free
// errors.  Returns false if valgrind can't run here.
static bool runCgiUnderValgrind(const char *pathinfo,
					cgirun *run, uint64_t *leaks,
					uint64_t *errors) {

	*leaks=0;
	*errors=0;
	run->output=charstring::duplicate("");

	// libtool runs valgrind on the binary rather than on the wrapper
	stringbuffer	cmd;
	cmd.append("( env -i PATH=\"$PATH\" SCRIPT_NAME=/skinvars/cgi.cgi "
			"DOCUMENT_ROOT=. SERVER_NAME=localhost "
			"REQUEST_METHOD=GET");
	cmd.append(" PATH_INFO='")->append(pathinfo)->append("'");
	cmd.append(" timeout 30 sh ../libtool --mode=execute valgrind "
			"--leak-check=full --show-leak-kinds=definite "
			"--num-callers=30 --log-file=" VG " ./cgi.cgi > " OUT
			" ) < /dev/null 2> /dev/null");

	// run it
	int	result=system(cmd.getString());
	run->exitedcleanly=(result==0);
	char	*output=file::getContents(OUT);
	if (output) {
		delete[] run->output;
		run->output=output;
	}
	char	*log=file::getContents(VG);
	file::remove(OUT);
	file::remove(VG);

	if (!charstring::contains(log,"HEAP SUMMARY")) {
		delete[] log;
		return false;
	}

	// count the loss records that come from setSkinVariable's copies
	char		**lines=NULL;
	uint64_t	linecount=0;
	charstring::split(log,"\n",false,&lines,&linecount);
	bool	inrecord=false;
	bool	setskinvariable=false;
	bool	duplicate=false;
	for (uint64_t i=0; i<=linecount; i++) {
		const char	*line=(i<linecount)?lines[i]:"";
		if (charstring::contains(line,"Invalid read") ||
			charstring::contains(line,"Invalid write") ||
			charstring::contains(line,"Invalid free")) {
			(*errors)++;
		}
		if (inrecord && (i==linecount || isBlankValgrindLine(line))) {
			if (setskinvariable && duplicate) {
				(*leaks)++;
			}
			inrecord=false;
		} else if (charstring::contains(line,
				"are definitely lost in loss record")) {
			inrecord=true;
			setskinvariable=false;
			duplicate=false;
		} else if (inrecord) {
			if (charstring::contains(line,
				"urlhttprequest::setSkinVariable")) {
				setskinvariable=true;
			}
			if (charstring::contains(line,
					"charstring::duplicate")) {
				duplicate=true;
			}
		}
	}
	for (uint64_t i=0; i<linecount; i++) {
		delete[] lines[i];
	}
	delete[] lines;
	delete[] log;
	return true;
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

	// skin variable leaks
	if (system("valgrind --version > /dev/null 2>&1")) {
		stdoutput.printf("\tskin variable leaks: skipped, "
						"no valgrind\n");
		return 0;
	}
	uint64_t	leaks=0;
	uint64_t	errors=0;

	// two different skin variables
	if (runCgiUnderValgrind("/unique/page",&run,&leaks,&errors)) {
		test("unique skin variables, exit status",run.exitedcleanly);
		test("unique skin variables, uniquevar1",
			hasSkinVariable(&run,"uniquevar1=uniqueval1"));
		test("unique skin variables, uniquevar2",
			hasSkinVariable(&run,"uniquevar2=uniqueval2"));
		test("unique skin variables, no leak",leaks==0);
		test("unique skin variables, no memory error",errors==0);
	} else {
		stdoutput.printf("\tunique skin variables: skipped, "
					"valgrind did not run\n");
	}
	delete[] run.output;
	stdoutput.printf("\n");

	// one skin variable set twice
	if (runCgiUnderValgrind("/repeat/page",&run,&leaks,&errors)) {
		test("repeated skin variable, exit status",run.exitedcleanly);
		test("repeated skin variable, last value",
			hasSkinVariable(&run,"repeatvar=repeatval2") &&
			!charstring::contains(run.output,"repeatval1"));
		test("repeated skin variable, no leak",leaks==0);
		test("repeated skin variable, no memory error",errors==0);
	} else {
		stdoutput.printf("\trepeated skin variable: skipped, "
					"valgrind did not run\n");
	}
	delete[] run.output;
	stdoutput.printf("\n");

	return 0;
}
