// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/mvcproperties.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include "test.cpp"

const char data[]=
	"\n"
	"! first we have a comment\n"
	"# then another comment\n"
	"\n"
	"! then some keys\n"
	"a.b.key1=value1\n"
	"a.b.key2 =value2\n"
	"a.b.key3 = value3\n"
	"a.b.key4:value4\n"
	"a.b.key5 :value5\n"
	"a.b.key6 : value6\n"
	"a.b.key7 value7\n"
	"\n"
	"# and finally some trailing comments\n";

int main() {

	header("mvcproperties");

	mvcproperties	m;
	test("parse string",m.parseString(data));
	test("a.b.key1",!charstring::compare(m.getValue("a.b.key1"),"value1"));
	test("a.b.key2",!charstring::compare(m.getValue("a.b.key2"),"value2"));
	test("a.b.key3",!charstring::compare(m.getValue("a.b.key3"),"value3"));
	test("a.b.key4",!charstring::compare(m.getValue("a.b.key4"),"value4"));
	test("a.b.key5",!charstring::compare(m.getValue("a.b.key5"),"value5"));
	test("a.b.key6",!charstring::compare(m.getValue("a.b.key6"),"value6"));
	test("a.b.key7",!charstring::compare(m.getValue("a.b.key7"),"value7"));

	file	f;
	test("create file",
		f.create("test.prop",
			permissions::evalPermString("rw-r--r--")) &&
		f.write(data,sizeof(data))==sizeof(data) &&
		f.close());

	test("parse file",m.parseFile("test.prop"));
	test("a.b.key1",!charstring::compare(m.getValue("a.b.key1"),"value1"));
	test("a.b.key2",!charstring::compare(m.getValue("a.b.key2"),"value2"));
	test("a.b.key3",!charstring::compare(m.getValue("a.b.key3"),"value3"));
	test("a.b.key4",!charstring::compare(m.getValue("a.b.key4"),"value4"));
	test("a.b.key5",!charstring::compare(m.getValue("a.b.key5"),"value5"));
	test("a.b.key6",!charstring::compare(m.getValue("a.b.key6"),"value6"));
	test("a.b.key7",!charstring::compare(m.getValue("a.b.key7"),"value7"));

	file::remove("test.prop");

	stdoutput.printf("\n");
}
