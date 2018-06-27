// Copyright (c) 2018  David Muse
// See the file COPYING for more information

#include <rudiments/jsonsax.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char normal[]=
	"{ "
	"	\"stringval\" : \"string value\","
	"	\"numberval\" : -1234.56,"
	"	\"trueval\" : true,"
	"	\"falseval\" : false,"
	"	\"nullval\" : null,"
	"	\"objectval\" : {"
	"		\"stringval\" : \"string value\","
	"		\"numberval\" : -1234.56,"
	"		\"trueval\" : true,"
	"		\"falseval\" : false,"
	"		\"nullval\" : null,"
	"		\"objectval\" : {"
	"			\"stringval\" : \"string value\","
	"			\"numberval\" : -1234.56,"
	"			\"trueval\" : true,"
	"			\"falseval\" : false,"
	"			\"nullval\" : null"
	"		}"
	"	}"
	"}";

int main() {

	jsonsax	j;

	j.parseString(normal);

	/*header("jsondom");

	jsondom	j;

	stdoutput.printf("normal...\n");
	c.parseString(normal);
	c.writeFile("normal.json",permissions::evalPermString("rw-r--r--"));
	char	*normaljson=file::getContents("normal.json");
	test("file contents",!charstring::compare(normal,normaljson));
	delete[] normaljson;
	test("parse file",c.parseFile("normal.json"));
	file::remove("normal.json");

	stdoutput.printf("\n");*/
}
