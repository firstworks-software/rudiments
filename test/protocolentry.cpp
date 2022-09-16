// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/protocolentry.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	// uninitialized
	protocolentry	pe;
	stdoutput.printf("uninitialized:\n");
	test("name",!pe.getName());
	test("alias list",!pe.getAliasList());
	test("number",pe.getNumber()==-1);
	stdoutput.printf("\n");
	

	// get the protocol information for "tcp"
	stdoutput.printf("tcp:\n");
	pe.init("tcp");
	test("name",!charstring::compare(pe.getName(),"tcp"));
	test("alias list",pe.getAliasList() &&
		!charstring::compareIgnoringCase(pe.getAliasList()[0],"tcp"));
	test("number",pe.getNumber()==6);
	stdoutput.printf("\n");

	// get the protocol information for protocol number 6
	pe.init(6);
	stdoutput.printf("6:\n");
	test("name",!charstring::compare(pe.getName(),"tcp"));
	test("alias list",pe.getAliasList() &&
		!charstring::compareIgnoringCase(pe.getAliasList()[0],"tcp"));
	test("number",pe.getNumber()==6);
	stdoutput.printf("\n");

	// null-safety
	stdoutput.printf("NULL:\n");
	pe.init((const char *)NULL);
	test("name",!pe.getName());
	test("alias list",!pe.getAliasList());
	test("number",pe.getNumber()==-1);
	stdoutput.printf("\n");

	process::exit(0);
}
