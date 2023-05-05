// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const char normal[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n"
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
"	<!-- comment after test2 -->\n"
"</test>";


const char ns[]=
"<?xml version=\"1.0\"?>\n"
"<testns:test>\n"
"	<!-- comment before test1 -->\n"
"	text before test1\n"
"	<testns:test1 name1=\"value1\" name2=\"value2\" name3=\"value3\">\n"
"		text before test11\n"
"		<testns:test11 name111=\"value111\">\n"
"			text inside test11\n"
"		</testns:test11>\n"
"		text after test11\n"
"		text before test12\n"
"		<testns:test12 name121=\"value121\">\n"
"			text inside test12\n"
"		</testns:test12>\n"
"		text after test12\n"
"		text before test13\n"
"		<testns:test13 name131=\"value131\"/>\n"
"		text after test13\n"
"	</testns:test1>\n"
"	text after test1\n"
"	<!-- comment after test1 -->\n"
"	<!-- comment before test2 -->\n"
"	text before test2\n"
"	<testns:test2 name1=\"value1\" name2=\"value2\" name3=\"value3\">\n"
"		text before test21\n"
"		<testns:test21 name211=\"value211\">\n"
"			text inside test21\n"
"		</testns:test21>\n"
"		text after test21\n"
"		text before test22\n"
"		<testns:test22 name221=\"value221\">\n"
"			text inside test22\n"
"		</testns:test22>\n"
"		text after test22\n"
"		text before test23\n"
"		<testns:test23 name231=\"value231\"/>\n"
"		text after test23\n"
"	</testns:test2>\n"
"	text after test2\n"
"	<!-- comment after test2 -->\n"
"</testns:test>";


const char entities[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n"
"	<test1 name1=\"&amp;&lt;&gt;&apos;&quot;&#200;\" name2=\"&amp&lt&gt&apos&quot\" name3=\"&&&&&\"/>\n"
"	&amp;&lt;&gt;&apos;&quot;&#200;\n"
"	&amp&lt&gt&apos&quot\n"
"	&&&&&\n"
"</test>";

const char entitiesresult[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n"
"	<test1 name1=\"&amp;&lt;&gt;&apos;&quot;&#200;\" name2=\"&amp;amp&amp;lt&amp;gt&amp;apos&amp;quot\" name3=\"&amp;&amp;&amp;&amp;&amp;\"/>\n"
"	&amp;&lt;&gt;&apos;&quot;&#200;\n"
"	&amp;amp&amp;lt&amp;gt&amp;apos&amp;quot\n"
"	&amp;&amp;&amp;&amp;&amp;\n"
"</test>";


const char cdata[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n"
"	<![CDATA[ test [ test [ test [test]]]]]>\n"
"	<![CDATA[ test [ test [ test [test]]]]]>\n"
"	<![CDATA[ test [ test [ test [test]]]]]>\n"
"</test>";


const char target[]=
"<?xml version=\"1.0\"?>\n"
"<test>\n"
"	<?target test1=\"test1\"?>\n"
"	<?target test2=\"test2\"?>\n"
"	<?target test3=\"test3\"?>\n"
"</test>";


const char singlequotes[]=
"<?xml version='1.0'?>\n"
"<test>\n"
"	<!-- comment before test1 -->\n"
"	text before test1\n"
"	<test1 name1='value1' name2='value2' name3='value3'>\n"
"		text before test11\n"
"		<test11 name111='value111'>\n"
"			text inside test11\n"
"		</test11>\n"
"		text after test11\n"
"		text before test12\n"
"		<test12 name121='value121'>\n"
"			text inside test12\n"
"		</test12>\n"
"		text after test12\n"
"		text before test13\n"
"		<test13 name131='value131'/>\n"
"		text after test13\n"
"	</test1>\n"
"	text after test1\n"
"	<!-- comment after test1 -->\n"
"	<!-- comment before test2 -->\n"
"	text before test2\n"
"	<test2 name1='value1' name2='value2' name3='value3'>\n"
"		text before test21\n"
"		<test21 name211='value211'>\n"
"			text inside test21\n"
"		</test21>\n"
"		text after test21\n"
"		text before test22\n"
"		<test22 name221='value221'>\n"
"			text inside test22\n"
"		</test22>\n"
"		text after test22\n"
"		text before test23\n"
"		<test23 name231='value231'/>\n"
"		text after test23\n"
"	</test2>\n"
"	text after test2\n"
"	<!-- comment after test2 -->\n"
"</test>";


const char oddspacing[]=
"<?xml version=\"1.0\" ?>\n"
"<test >\n"
"	<!-- comment before test1 -->\n"
"	text before test1\n"
"	<test1  name1=\"value1\" name2=\"value2\"  name3=\"value3\" >\n"
"		text before test11\n"
"		<test11  name111=\"value111\" >\n"
"			text inside test11\n"
"		< / test11 >\n"
"		text after test11\n"
"		text before test12\n"
"		<test12  name121=\"value121\" >\n"
"			text inside test12\n"
"		< / test12 >\n"
"		text after test12\n"
"		text before test13\n"
"		<test13  name131=\"value131\" / >\n"
"		text after test13\n"
"	< / test1 >\n"
"	text after test1\n"
"	<!-- comment after test1 -->\n"
"	<!-- comment before test2 -->\n"
"	text before test2\n"
"	<test2  name1=\"value1\"  name2=\"value2\"  name3=\"value3\" >\n"
"		text before test21\n"
"		<test21  name211=\"value211\" >\n"
"			text inside test21\n"
"		< / test21 >\n"
"		text after test21\n"
"		text before test22\n"
"		< test22  name221=\"value221\" >\n"
"			text inside test22\n"
"		< / test22 >\n"
"		text after test22\n"
"		text before test23\n"
"		< test23  name231=\"value231\" / >\n"
"		text after test23\n"
"	< / test2 >\n"
"	text after test2\n"
"	<!-- comment after test2 -->\n"
"< / test >";

int main(int argc, const char **argv) {

	header("xmldom");

	xmldom	x(false);

	stdoutput.printf("normal...\n");
	test("parse string",x.parseString(normal));
	file::remove("normal.xml");
	test("write file",x.writeFile("normal.xml",
				permissions::parsePermString("rw-r--r--"),
				false)==(ssize_t)charstring::getLength(normal));
	char	*normaldotxml=file::getContents("normal.xml");
	test("file contents",!charstring::compare(normal,normaldotxml));
	test("parse file",x.parseFile("normal.xml"));


	stdoutput.printf("ns...\n");
	test("parse string",x.parseString(ns));
	file::remove("ns.xml");
	test("write file",x.writeFile("ns.xml",
				permissions::parsePermString("rw-r--r--"),
				false)==(ssize_t)charstring::getLength(ns));
	char	*nsdotxml=file::getContents("ns.xml");
	test("file contents",!charstring::compare(ns,nsdotxml));
	delete[] nsdotxml;
	test("parse file",x.parseFile("ns.xml"));
	file::remove("ns.xml");


	stdoutput.printf("entities...\n");
	test("parse string",x.parseString(entities));
	file::remove("entities.xml");
	test("write file",
		x.writeFile("entities.xml",
			permissions::parsePermString("rw-r--r--"),
			false)==(ssize_t)charstring::getLength(entitiesresult));
	char	*entitiesdotxml=file::getContents("entities.xml");
	test("file contents",!charstring::compare(
				entitiesresult,entitiesdotxml));
	delete[] entitiesdotxml;
	test("parse file",x.parseFile("entities.xml"));
	file::remove("entities.xml");


	stdoutput.printf("cdata...\n");
	test("parse string",x.parseString(cdata));
	file::remove("cdata.xml");
	test("write file",x.writeFile("cdata.xml",
				permissions::parsePermString("rw-r--r--"),
				false)==(ssize_t)charstring::getLength(cdata));
	char	*cdatadotxml=file::getContents("cdata.xml");
	test("file contents",!charstring::compare(cdata,cdatadotxml));
	delete[] cdatadotxml;
	test("parse file",x.parseFile("cdata.xml"));
	file::remove("cdata.xml");


	stdoutput.printf("target...\n");
	test("parse string",x.parseString(target));
	file::remove("target.xml");
	test("write file",x.writeFile("target.xml",
				permissions::parsePermString("rw-r--r--"),
				false)==(ssize_t)charstring::getLength(target));
	char	*targetdotxml=file::getContents("target.xml");
	test("file contents",!charstring::compare(target,targetdotxml));
	delete[] targetdotxml;
	test("parse file",x.parseFile("target.xml"));
	file::remove("target.xml");


	stdoutput.printf("single quotes...\n");
	test("parse string",x.parseString(singlequotes));
	file::remove("singlequotes.xml");
	test("write file",
		x.writeFile("singlequotes.xml",
			permissions::parsePermString("rw-r--r--"),
			false)==(ssize_t)charstring::getLength(singlequotes));
	char	*singlequotesdotxml=file::getContents("singlequotes.xml");
	test("file contents",!charstring::compare(normal,singlequotesdotxml));
	delete[] singlequotesdotxml;
	test("parse file",x.parseFile("singlequotes.xml"));
	file::remove("singlequotes.xml");


	stdoutput.printf("odd spacing...\n");
	test("parse string",x.parseString(oddspacing));
	file::remove("oddspacing.xml");
	test("write file",
		x.writeFile("oddspacing.xml",
			permissions::parsePermString("rw-r--r--"),
			false)==(ssize_t)charstring::getLength(normal));
	char	*oddspacingdotxml=file::getContents("oddspacing.xml");
	test("file contents",!charstring::compare(normal,oddspacingdotxml));
	delete[] oddspacingdotxml;
	test("parse file",x.parseFile("oddspacing.xml"));
	file::remove("oddspacing.xml");

	stdoutput.printf("\n");


	file::remove("normal.xml");
	delete[] normaldotxml;

	return 0;
}
