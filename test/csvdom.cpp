// Copyright (c) 2018  David Muse
// See the file COPYING for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char normal[]=
"\"\"\"col1\"\"\",c\"ol\"2,col3,col4,col5\n"
"val11,\"\"\"val12\"\"\",v\"al1\"3,\"val\n14\",val15\n"
"val21,val22,\"\"\"val23\"\"\",v\"al2\"4,\"val\n25\"\n";

int main() {

	header("csvdom");

	csvdom	c;

	stdoutput.printf("normal...\n");
	c.parseString(normal);
	c.writeFile("normal.csv",permissions::evalPermString("rw-r--r--"));
	char	*normalcsv=file::getContents("normal.csv");
	test("file contents",!charstring::compare(normal,normalcsv));
	delete[] normalcsv;
	test("parse file",c.parseFile("normal.csv"));
	file::remove("normal.csv");

	stdoutput.printf("\n");
}
