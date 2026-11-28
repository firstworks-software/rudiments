// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>
#include <rudiments/signalclasses.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
	#include <io.h>
	#include <fcntl.h>
#endif
#include "test.cpp"

// These tests run cgi.cgi with crafted environment variables and stdin.
// A file, urlhttprequest.rest, collects whatever cgi.cgi leaves unread
// on stdin.

#define BODY "urlhttprequest.body"
#define OUTFILE "urlhttprequest.out"
#define REST "urlhttprequest.rest"
#define VG "urlhttprequest.vg"

#ifdef _WIN32
	#define CGI "cgi.cgi.exe"
#else
	#define CGI "./cgi.cgi"
#endif

// prefix for each cgi.cgi run, empty if there's no timeout command
static const char	*timeoutcmd="";

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
	f.open(BODY,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY,
			permissions::parsePermString("rw-r--r--"));
	f.write(body);
	f.close();

	// build the command
	stringbuffer	cmd;
#ifdef _WIN32
	// cmd.exe has no env -i or cat, so set (or clear) each variable and
	// let this program copy what cgi.cgi leaves unread on stdin.  The copy
	// runs only if cgi.cgi succeeds, so the exit status is cgi.cgi's.
	cmd.append("set \"SCRIPT_NAME=/cgi.cgi\"&& "
			"set \"DOCUMENT_ROOT=.\"&& "
			"set \"SERVER_NAME=localhost\"&& "
			"set \"PATH_INFO=\"&& "
			"set \"REMOTE_ADDR=\"&& "
			"set \"HTTP_REFERER=\"&& ");
	cmd.append("set \"REQUEST_METHOD=")->append(method)->append("\"&& ");
	cmd.append("set \"CONTENT_TYPE=")->
			append((contenttype)?contenttype:"")->append("\"&& ");
	cmd.append("set \"CONTENT_LENGTH=")->
			append((contentlength)?contentlength:"")->
			append("\"&& ");
	cmd.append("set \"QUERY_STRING=")->
			append((querystring)?querystring:"")->append("\"&& ");
	cmd.append("( " CGI " > " OUTFILE " && urlhttprequest.exe -cat > " REST
						" ) < " BODY " 2> nul");
#else
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
	cmd.append(timeoutcmd)->append(" " CGI " > " OUTFILE "; cat > " REST
						" ) < " BODY " 2> /dev/null");
#endif

	// run it
	time_t	start=time(NULL);
	int	result=system(cmd.getString());
	run->elapsed=time(NULL)-start;
	run->exitedcleanly=(result==0);
	run->output=file::getContents(OUTFILE);
	if (!run->output) {
		run->output=charstring::duplicate("");
	}
	char	*rest=file::getContents(REST);
	run->restsize=charstring::getLength(rest);
	delete[] rest;

	file::remove(BODY);
	file::remove(OUTFILE);
	file::remove(REST);
}

// Runs cgi.cgi with the skin files that "pathinfo" selects from skinvars/.
// The remote address and referer are left unset when NULL.
static void runCgiAccessCheck(const char *pathinfo,
					const char *method,
					const char *remoteaddr,
					const char *referer,
					cgirun *run) {

	// build the command
	stringbuffer	cmd;
#ifdef _WIN32
	cmd.append("set \"SCRIPT_NAME=/skinvars/cgi.cgi\"&& "
			"set \"DOCUMENT_ROOT=.\"&& "
			"set \"SERVER_NAME=localhost\"&& "
			"set \"CONTENT_TYPE=\"&& "
			"set \"CONTENT_LENGTH=\"&& "
			"set \"QUERY_STRING=\"&& ");
	cmd.append("set \"PATH_INFO=")->append(pathinfo)->append("\"&& ");
	cmd.append("set \"REQUEST_METHOD=")->append(method)->append("\"&& ");
	cmd.append("set \"REMOTE_ADDR=")->
			append((remoteaddr)?remoteaddr:"")->append("\"&& ");
	cmd.append("set \"HTTP_REFERER=")->
			append((referer)?referer:"")->append("\"&& ");
	cmd.append(CGI " > " OUTFILE " < nul 2> nul");
#else
	cmd.append("env -i PATH=\"$PATH\" SCRIPT_NAME=/skinvars/cgi.cgi "
			"DOCUMENT_ROOT=. SERVER_NAME=localhost");
	cmd.append(" PATH_INFO='")->append(pathinfo)->append("'");
	cmd.append(" REQUEST_METHOD='")->append(method)->append("'");
	if (remoteaddr) {
		cmd.append(" REMOTE_ADDR='")->append(remoteaddr)->append("'");
	}
	if (referer) {
		cmd.append(" HTTP_REFERER='")->append(referer)->append("'");
	}
	cmd.append(timeoutcmd)->append(" " CGI " > " OUTFILE
						" < /dev/null 2> /dev/null");
#endif

	// run it
	time_t	start=time(NULL);
	int	result=system(cmd.getString());
	run->elapsed=time(NULL)-start;
	run->exitedcleanly=(result==0);
	run->output=file::getContents(OUTFILE);
	if (!run->output) {
		run->output=charstring::duplicate("");
	}
	run->restsize=0;

	file::remove(OUTFILE);
}

static bool hasParameter(cgirun *run, const char *nameandvalue) {
	stringbuffer	line;
	line.append("\n\t")->append(nameandvalue)->append("\n");
	return charstring::contains(run->output,line.getString());
}

static bool hasLine(cgirun *run, const char *text) {
	stringbuffer	line;
	line.append("\n")->append(text)->append("\n");
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
	cmd.append(timeoutcmd)->append(" sh ../libtool --mode=execute valgrind "
			"--leak-check=full --show-leak-kinds=definite "
			"--num-callers=30 --log-file=" VG
			" ./cgi.cgi > " OUTFILE
			" ) < /dev/null 2> /dev/null");

	// run it
	int	result=system(cmd.getString());
	run->exitedcleanly=(result==0);
	char	*output=file::getContents(OUTFILE);
	if (output) {
		delete[] run->output;
		run->output=output;
	}
	char	*log=file::getContents(VG);
	file::remove(OUTFILE);
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

#ifdef _WIN32
static void alarmHandler(int32_t signum) {
	stdoutput.printf("\ttimed out\n");
	process::exit(1);
}
#endif

int main(int argc, const char **argv) {

#ifdef _WIN32
	// copy stdin to stdout byte for byte, for runCgi
	if (argc==2 && !charstring::compare(argv[1],"-cat")) {
		_setmode(0,_O_BINARY);
		_setmode(1,_O_BINARY);
		char	buffer[1024];
		ssize_t	size;
		while ((size=stdinput.read(buffer,sizeof(buffer)))>0) {
			stdoutput.write(buffer,size);
		}
		return 0;
	}
#endif

	header("urlhttprequest");

#ifdef _WIN32
	// The Windows timeout command is a different program, and an alarm
	// with no handler does nothing there.  An alarm with a handler bounds
	// the whole test instead, so a hung cgi.cgi fails it.
	stdoutput.printf("\tno timeout command, using an alarm\n");
	signalhandler	alarmhandler;
	alarmhandler.setHandler(alarmHandler);
	alarmhandler.handleSignal(SIGALRM);
	signalmanager::alarm(600);
#else
	// Some older systems have no timeout command.  There, an alarm
	// bounds the whole test, so a hung cgi.cgi fails it instead.
	if (!system("timeout 5 true > /dev/null 2>&1")) {
		timeoutcmd=" timeout 30";
	} else {
		stdoutput.printf("\tno timeout command, using an alarm\n");
		signalmanager::alarm(600);
	}
#endif

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
	test("mixed-case urlencoded, var1",
				hasParameter(&run,"var1=val1"));
	test("mixed-case urlencoded, var2",
				hasParameter(&run,"var2=val2"));
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

	// allowed ips only, with a match
	runCgiAccessCheck("/allowips/page","GET","10.0.0.1",NULL,&run);
	test("allowed ip, exit status",run.exitedcleanly);
	test("allowed ip, allowed",hasLine(&run,"isIpAllowed=1"));
	delete[] run.output;

	// allowed ips only, with no match
	runCgiAccessCheck("/allowips/page","GET","10.0.0.2",NULL,&run);
	test("unlisted ip, exit status",run.exitedcleanly);
	test("unlisted ip, denied",hasLine(&run,"isIpAllowed=0"));
	delete[] run.output;

	// ip in both the allowed and denied lists
	runCgiAccessCheck("/denyips/page","GET","10.0.0.5",NULL,&run);
	test("denied ip, exit status",run.exitedcleanly);
	test("denied ip, denied",hasLine(&run,"isIpAllowed=0"));
	delete[] run.output;

	// ip in the allowed list only, with a denied list set
	runCgiAccessCheck("/denyips/page","GET","10.0.0.6",NULL,&run);
	test("allowed ip beside denied list, exit status",run.exitedcleanly);
	test("allowed ip beside denied list, allowed",
					hasLine(&run,"isIpAllowed=1"));
	delete[] run.output;

	// denied ips only, with a match
	runCgiAccessCheck("/deniedonlyips/page","GET","10.0.0.5",NULL,&run);
	test("denied-only ip, exit status",run.exitedcleanly);
	test("denied-only ip, denied",hasLine(&run,"isIpAllowed=0"));
	delete[] run.output;

	// denied ips only, with no match: no allowed list denies everything
	runCgiAccessCheck("/deniedonlyips/page","GET","10.0.0.6",NULL,&run);
	test("ip with no allowed list, exit status",run.exitedcleanly);
	test("ip with no allowed list, denied",
					hasLine(&run,"isIpAllowed=0"));
	delete[] run.output;
	stdoutput.printf("\n");

	// allowed ip, denied referer
	runCgiAccessCheck("/referers/page","GET",
				"10.0.0.1","http://bad.example.com/",&run);
	test("denied referer, exit status",run.exitedcleanly);
	test("denied referer, ip allowed",hasLine(&run,"isIpAllowed=1"));
	test("denied referer, referer denied",
					hasLine(&run,"isRefererAllowed=0"));
	delete[] run.output;

	// denied ip, allowed referer
	runCgiAccessCheck("/referers/page","GET",
				"10.0.0.5","http://good.example.com/",&run);
	test("allowed referer, exit status",run.exitedcleanly);
	test("allowed referer, ip denied",hasLine(&run,"isIpAllowed=0"));
	test("allowed referer, referer allowed",
					hasLine(&run,"isRefererAllowed=1"));
	delete[] run.output;

	// unlisted referer
	runCgiAccessCheck("/referers/page","GET",
				"10.0.0.1","http://other.test/",&run);
	test("unlisted referer, exit status",run.exitedcleanly);
	test("unlisted referer, referer denied",
					hasLine(&run,"isRefererAllowed=0"));
	delete[] run.output;
	stdoutput.printf("\n");

	// method in the allowed list only
	runCgiAccessCheck("/methods/page","GET",NULL,NULL,&run);
	test("allowed method, exit status",run.exitedcleanly);
	test("allowed method, allowed",hasLine(&run,"isMethodAllowed=1"));
	delete[] run.output;

	// method in both the allowed and denied lists
	runCgiAccessCheck("/methods/page","POST",NULL,NULL,&run);
	test("denied method, exit status",run.exitedcleanly);
	test("denied method, denied",hasLine(&run,"isMethodAllowed=0"));
	delete[] run.output;

	// unlisted method
	runCgiAccessCheck("/methods/page","PUT",NULL,NULL,&run);
	test("unlisted method, exit status",run.exitedcleanly);
	test("unlisted method, denied",hasLine(&run,"isMethodAllowed=0"));
	delete[] run.output;
	stdoutput.printf("\n");

	// skin variable leaks
#ifdef _WIN32
	stdoutput.printf("\tskin variable leaks: skipped on Windows, "
						"no valgrind\n");
	return 0;
#else
	if (system("valgrind --version > /dev/null 2>&1")) {
		stdoutput.printf("\tskin variable leaks: skipped, "
						"no valgrind\n");
		return 0;
	}
#endif
	uint64_t	leaks=0;
	uint64_t	errors=0;

	// two different skin variables
	if (runCgiUnderValgrind("/unique/page",&run,&leaks,&errors)) {
		test("unique skin variables, exit status",run.exitedcleanly);
		test("unique skin variables, uniquevar1",
			hasLine(&run,"uniquevar1=uniqueval1"));
		test("unique skin variables, uniquevar2",
			hasLine(&run,"uniquevar2=uniqueval2"));
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
			hasLine(&run,"repeatvar=repeatval2") &&
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
