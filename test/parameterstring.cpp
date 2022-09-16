// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/parameterstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("parameterstring");

	parameterstring	pstr;

	stdoutput.printf("getValue...\n");
	test("parse",pstr.parse("name1=value1;name2='value2';name3='\\'value3\\'';name4=\\\\value4\\\\;name5=;name6="));

	test("name1",!charstring::compare(pstr.getValue("name1"),"value1"));
	test("name2",!charstring::compare(pstr.getValue("name2"),"value2"));
	test("name3",!charstring::compare(pstr.getValue("name3"),"'value3'"));
	test("name4",!charstring::compare(pstr.getValue("name4"),"\\value4\\"));
	test("name5",!charstring::compare(pstr.getValue("name5"),""));
	test("name6",!charstring::compare(pstr.getValue("name6"),""));
	test("name7",!pstr.getValue("name7"));
	stdoutput.printf("\n");

	stdoutput.printf("toDictionary...\n");
	dictionary<char *,char *>	*nvp=pstr.getDictionary();
	test("name1",!charstring::compare(
				nvp->getValue((char *)"name1"),"value1"));
	test("name2",!charstring::compare(
				nvp->getValue((char *)"name2"),"value2"));
	test("name3",!charstring::compare(
				nvp->getValue((char *)"name3"),"'value3'"));
	test("name4",!charstring::compare(
				nvp->getValue((char *)"name4"),"\\value4\\"));
	test("name5",!charstring::compare(
				nvp->getValue((char *)"name5"),""));
	test("name6",!charstring::compare(
				nvp->getValue((char *)"name6"),""));
	test("name7",!nvp->getValue((char *)"name7"));
	stdoutput.printf("\n");


	stdoutput.printf("\n");

	process::exit(0);
}
