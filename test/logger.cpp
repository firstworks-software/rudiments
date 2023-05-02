// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stringbuffer.h>
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	#include <syslog.h>
#endif
#include "test.cpp"

int main(int argc, const char **argv) {

	header("logger");

	file::remove("test.log");
	
	logger			lg;

	// initialize the log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	syslogdestination	sd;
	sd.open("logtest",LOG_CONS,LOG_USER,LOG_INFO);
	#endif
	filedestination		fd;
	test("file",fd.open("test.log",
			permissions::evalPermString("rw-------")));
	stdoutdestination	sod;
	stderrdestination	sed;

	// add log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
	#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);


	// some crash tests...

	// remove log destnations all at once
	lg.removeAllLogDestinations();

	// add them back
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
	#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);

	// remove them one by one
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.removeLogDestination(&sd);
	#endif
	lg.removeLogDestination(&fd);
	lg.removeLogDestination(&sod);
	lg.removeLogDestination(&sed);

	// remove them all at once (even though none should currently be added)
	lg.removeAllLogDestinations();


	// get the current date/time so we can verify the header
	// (make sure we're not about to bump over to the next minute)
	datetime	dt;
	dt.initFromSystemDateTime();
	if (dt.getSecond()>=58) {
		snooze::macrosnooze(5);
		dt.initFromSystemDateTime();
	}

	// create and verify the header
	char	*header=logger::getLogHeader("logtest");
	test("header month",charstring::convertToInteger(header)==dt.getMonth());
	test("header day",charstring::convertToInteger(header+3)==dt.getDayOfMonth());
	test("header year",charstring::convertToInteger(header+6)==dt.getYear());
	test("header hour",charstring::convertToInteger(header+11)==dt.getHour());
	test("header minute",charstring::convertToInteger(header+14)==dt.getMinute());
	test("header program",
		// (date string may or may not include the timezone)
		!charstring::compare(header+24,"logtest ",8) ||
		!charstring::compare(header+21,"logtest ",8));
	test("header pid",
		// (date string may or may not include the timezone)
		charstring::convertToInteger(header+33)==process::getProcessId() ||
		charstring::convertToInteger(header+30)==process::getProcessId());


	// write various log messages (even though no destinations exist)
	lg.write(0,header,0,"%d%s%d",1,"test",2);
	lg.write(0,header,0,"test");
	lg.write(0,header,0,"t");
	lg.write(0,header,0,"%d",(int32_t)12345);
	lg.write(0,header,0,"%.2f",123.45);
	lg.write(0,"",0,"%d%s%d",1,"test",2);
	lg.write(0,"",0,"test");
	lg.write(0,"",0,"t");
	lg.write(0,"",0,"%d",(int32_t)12345);
	lg.write(0,"",0,"%.2f",123.45);
	lg.write(0,NULL,0,"%d%s%d",1,"test",2);
	lg.write(0,NULL,0,"test");
	lg.write(0,NULL,0,"t");
	lg.write(0,NULL,0,"%d",(int32_t)12345);
	lg.write(0,NULL,0,"%.2f",123.45);

	// re-add log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	lg.addLogDestination(&sd);
	#endif
	lg.addLogDestination(&fd);
	lg.addLogDestination(&sod);
	lg.addLogDestination(&sed);

	// write various log messages, with and without header
	// (now destinations do exist)
	lg.write(0,header,0,"%d%s%d",1,"test",2);
	lg.write(0,header,0,"test");
	lg.write(0,header,0,"t");
	lg.write(0,header,0,"%d",(int32_t)12345);
	lg.write(0,header,0,"%.2f",123.45);
	lg.write(0,"",0,"%d%s%d",1,"test",2);
	lg.write(0,"",0,"test");
	lg.write(0,"",0,"t");
	lg.write(0,"",0,"%d",(int32_t)12345);
	lg.write(0,"",0,"%.2f",123.45);
	lg.write(0,NULL,0,"%d%s%d",1,"test",2);
	lg.write(0,NULL,0,"test");
	lg.write(0,NULL,0,"t");
	lg.write(0,NULL,0,"%d",(int32_t)12345);
	lg.write(0,NULL,0,"%.2f",123.45);

	// verify log file contents
	stringbuffer	testcontents;
	testcontents.append(header)->append(" : 1test2\n");
	testcontents.append(header)->append(" : test\n");
	testcontents.append(header)->append(" : t\n");
	testcontents.append(header)->append(" : 12345\n");
	testcontents.append(header)->append(" : 123.45\n");
	testcontents.append("1test2\n");
	testcontents.append("test\n");
	testcontents.append("t\n");
	testcontents.append("12345\n");
	testcontents.append("123.45\n");
	testcontents.append("1test2\n");
	testcontents.append("test\n");
	testcontents.append("t\n");
	testcontents.append("12345\n");
	testcontents.append("123.45\n");
	char	*testlog=file::getContents("test.log");
	test("contents",!charstring::compare(testcontents.getString(),testlog));
	delete[] testlog;

	// close the log destinations
	#ifdef RUDIMENTS_HAVE_SYSLOG_H
	sd.close();
	#endif
	fd.close();

	// clean up
	delete[] header;
	file::remove("test.log");

	return 0;
}
