// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/xmldom.h>
#include <rudiments/datetime.h>
#include "../test/test.cpp"

const char head[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n";

const char body[]=
"	<!-- comment before test1 -->\n"
"	text before test1\n"
"	<test1 name1=\"value1\" name2=\"value2\" name3=\"value3\">\n"
"		text before test11\n"
"		<test11 name111=\"value111\">\n"
"			text inside test11\n"
"		</test11>\n"
"		text after test11\n"
"		text before test12\n"
"		<test12 name121=\"value121\">\n"
"			text inside test12\n"
"		</test12>\n"
"		text after test12\n"
"		text before test13\n"
"		<test13 name131=\"value131\"/>\n"
"		text after test13\n"
"	</test1>\n"
"	text after test1\n"
"	<!-- comment after test1 -->\n"
"	<!-- comment before test2 -->\n"
"	text before test2\n"
"	<test2 name1=\"value1\" name2=\"value2\" name3=\"value3\">\n"
"		text before test21\n"
"		<test21 name211=\"value211\">\n"
"			text inside test21\n"
"		</test21>\n"
"		text after test21\n"
"		text before test22\n"
"		<test22 name221=\"value221\">\n"
"			text inside test22\n"
"		</test22>\n"
"		text after test22\n"
"		text before test23\n"
"		<test23 name231=\"value231\"/>\n"
"		text after test23\n"
"	</test2>\n"
"	text after test2\n"
"	<!-- comment after test2 -->\n";

const char footer[]=
"</test>";

int main(int argc, const char **argv) {


	// determine the file name
	stringbuffer	filename;
	const char	*home=environment::getValue("HOME");
	if (charstring::isNullOrEmpty(home)) {
		filename.append("/home/");
		const char	*user=environment::getValue("USER");
		if (charstring::isNullOrEmpty(user)) {
			filename.append("dmuse");
		} else {
			filename.append(user);
		}
	} else {
		filename.append(home);
	}
	filename.append('/')->append("testfile.xml");
	stdoutput.printf("filename: %s\n",filename.getString());

	// create the file
	file	f;
	if (!f.create(filename.getString(),
			permissions::parsePermString("rw-r--r--"))) {
		stdoutput.printf("create failed\n");
		process::exit(1);
	}

	// write some contents to the file
	stdoutput.printf("creating file...\n");
	if (f.write(head)!=(ssize_t)charstring::getLength(head)) {
		stdoutput.printf("write header failed\n");
		process::exit(1);
	}
	for (uint32_t i=0; i<1024*256; i++) {
		if (f.write(body)!=(ssize_t)charstring::getLength(body)) {
			stdoutput.printf("write body failed\n");
			process::exit(1);
		}
	}
	if (f.write(footer)!=(ssize_t)charstring::getLength(footer)) {
		stdoutput.printf("write footer failed\n");
		process::exit(1);
	}

	// close the file
	f.close();

	// parse the file
	xmlsax		x;
	datetime	start;
	datetime	end;
	stdoutput.printf("parsing file...\n");
	start.initFromSystemDateTime();
	bool		success=x.parseFile(filename.getString());
	if (!success) {
		stdoutput.printf("parse failed\n");
	} else {
		end.initFromSystemDateTime();
		stdoutput.printf("parse success\n");
		displayTime(&start,&end);
	}

	// clean up
	//file::remove(filename.getString());
}
