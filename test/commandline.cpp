// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	// if we're called with no arguments then
	// re-call ourself with some arguments
	if (argc==1) {

		process::setWaitForChildren(true);

		const char	*args[]={
			"commandline",
			"-one","1",
			"-two",
			"--three=3",
			"--four",
			"--five=5",
			"-f","five",
			"--six",
			"-s",
			NULL
		};

		process::spawn(
			#ifdef _WIN32
			"commandline.exe",
			#else
			"./commandline",
			#endif
			args,false);
	
		process::exit(0);
	}

	header("commandline");

	commandline     cmdline(argc,argv);

	// found
	stdoutput.printf("found...\n");
	test("isFound(), -one",cmdline.isFound("one"));
	test("isFound(), -two",cmdline.isFound("two"));
	test("isFound(), --three",cmdline.isFound("three"));
	test("isFound(), --four",cmdline.isFound("four"));
	test("isFound(), --five",cmdline.isFound("five"));
	test("isFound(), -f",cmdline.isFound("f"));
	test("isFound(), --five/-f",cmdline.isFound("five","f"));
	test("isFound(), --six",cmdline.isFound("six"));
	test("isFound(), -s",cmdline.isFound("s"));
	test("isFound(), --six/-s",cmdline.isFound("six","s"));
	test("isFound(), --SIX/-s",cmdline.isFound("SIX","s"));
	test("isFound(), --six/-S",cmdline.isFound("six","S"));
	stdoutput.printf("\n");

	// getValue
	stdoutput.printf("getValue...\n");
	test("getValue(), -one",
		!charstring::compare(cmdline.getValue("one"),"1"));
	test("getValue(), -two",
		!charstring::compare(cmdline.getValue("two"),""));
	test("getValue(), --three",
		!charstring::compare(cmdline.getValue("three"),"3"));
	test("getValue(), --four",
		!charstring::compare(cmdline.getValue("four"),""));
	test("getValue(), --five",
		!charstring::compare(cmdline.getValue("five"),"5"));
	test("getValue(), -f",
		!charstring::compare(cmdline.getValue("f"),"five"));
	test("getValue(), --five/-f",
		!charstring::compare(cmdline.getValue("five","f"),"5"));
	stdoutput.printf("\n");

	// toDictionary
	stdoutput.printf("convertToDictionary...\n");
	dictionary<char *,char *>	d;
	cmdline.convertToDictionary(&d);
	test("getValue(), -one",
		!charstring::compare(d.getValue((char *)"one"),"1"));
	test("getValue(), -two",
		!charstring::compare(d.getValue((char *)"two"),""));
	test("getValue(), --three",
		!charstring::compare(d.getValue((char *)"three"),"3"));
	test("getValue(), --four",
		!charstring::compare(d.getValue((char *)"four"),""));
	test("getValue(), --five",
		!charstring::compare(d.getValue((char *)"five"),"5"));
	test("getValue(), -f",
		!charstring::compare(d.getValue((char *)"f"),"five"));
	test("getValue(), --six",
		!charstring::compare(d.getValue((char *)"six"),""));
	test("getValue(), -s",
		!charstring::compare(d.getValue((char *)"s"),""));
	stdoutput.printf("\n");

	process::exit(0);
}
