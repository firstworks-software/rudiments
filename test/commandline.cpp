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
	test("getWasFound(), -one",cmdline.getWasFound("one"));
	test("getWasFound(), -two",cmdline.getWasFound("two"));
	test("getWasFound(), --three",cmdline.getWasFound("three"));
	test("getWasFound(), --four",cmdline.getWasFound("four"));
	test("getWasFound(), --five",cmdline.getWasFound("five"));
	test("getWasFound(), -f",cmdline.getWasFound("f"));
	test("getWasFound(), --five/-f",cmdline.getWasFound("five","f"));
	test("getWasFound(), --six",cmdline.getWasFound("six"));
	test("getWasFound(), -s",cmdline.getWasFound("s"));
	test("getWasFound(), --six/-s",cmdline.getWasFound("six","s"));
	test("getWasFound(), --SIX/-s",cmdline.getWasFound("SIX","s"));
	test("getWasFound(), --six/-S",cmdline.getWasFound("six","S"));
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
