// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/jsondom.h>
#include <rudiments/domnode.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include "test.cpp"

struct test {
	const char	*name;
	const char	*testval;
	bool		comparetoprevious;
};

const struct test tests[]={

	{
	"empty",
	"",
	false
	},

	{
	"whitespace",
	"\n\r\t ",
	true
	},

	{
	"degenerate object",
	"{\n"
	"}",
	false
	},

	{
	"degenerate object, unindented",
	"{}",
	true
	},

	{
	"degenerate array",
	"[\n"
	"]",
	false
	},

	{
	"degenerate array, unindented",
	"[]",
	true
	},

	{
	"array of objects",
	"[\n"
	"  {\n"
	"    \"one\" : 1,\n"
	"    \"two\" : 2,\n"
	"    \"three\" : 3\n"
	"  },\n"
	"  {\n"
	"    \"one\" : 1,\n"
	"    \"two\" : 2,\n"
	"    \"three\" : 3\n"
	"  },\n"
	"  {\n"
	"    \"one\" : 1,\n"
	"    \"two\" : 2,\n"
	"    \"three\" : 3\n"
	"  }\n"
	"]",
	false
	},

	{
	"array of objects, unindented",
	"["
	  "{"
	    "\"one\" : 1,"
	    "\"two\" : 2,"
	    "\"three\" : 3"
	  "},"
	  "{"
	    "\"one\" : 1,"
	    "\"two\" : 2,"
	    "\"three\" : 3"
	  "},"
	  "{"
	    "\"one\" : 1,"
	    "\"two\" : 2,"
	    "\"three\" : 3"
	  "}"
	"]",
	true
	},

	{
	"mixed array",
	"[\n"
	"  {\n"
	"    \"one\" : 1,\n"
	"    \"two\" : 2,\n"
	"    \"three\" : 3\n"
	"  },\n"
	"  \"one\",\n"
	"  2,\n"
	"  true,\n"
	"  false,\n"
	"  null\n"
	"]",
	false
	},

	{
	"mixed array, unindented",
	"["
	  "{"
	    "\"one\" : 1,"
	    "\"two\" : 2,"
	    "\"three\" : 3"
	  "},"
	  "\"one\","
	  "2,"
	  "true,"
	  "false,"
	  "null"
	"]",
	true
	},

	{
	"complex elements",
	"{\n"
	"  \"stringval\" : \"string value\",\n"
	"  \"numberval\" : -1234.56,\n"
	"  \"trueval\" : true,\n"
	"  \"falseval\" : false,\n"
	"  \"nullval\" : null,\n"
	"  \"stringarrayval\" : \n"
	"  [\n"
	"    \"one\",\n"
	"    \"two\",\n"
	"    \"three\",\n"
	"    \"four\"\n"
	"  ],\n"
	"  \"numberarrayval\" : \n"
	"  [\n"
	"    1,\n"
	"    2,\n"
	"    3,\n"
	"    4\n"
	"  ],\n"
	"  \"booleanarrayval\" : \n"
	"  [\n"
	"    true,\n"
	"    false,\n"
	"    true,\n"
	"    false\n"
	"  ],\n"
	"  \"nullarrayval\" : \n"
	"  [\n"
	"    null,\n"
	"    null,\n"
	"    null,\n"
	"    null\n"
	"  ],\n"
	"  \"objectval\" : \n"
	"  {\n"
	"    \"stringval\" : \"string value\",\n"
	"    \"numberval\" : -1234.56,\n"
	"    \"trueval\" : true,\n"
	"    \"falseval\" : false,\n"
	"    \"nullval\" : null,\n"
	"    \"stringarrayval\" : \n"
	"    [\n"
	"      \"one\",\n"
	"      \"two\",\n"
	"      \"three\",\n"
	"      \"four\"\n"
	"    ],\n"
	"    \"numberarrayval\" : \n"
	"    [\n"
	"      1,\n"
	"      2,\n"
	"      3,\n"
	"      4\n"
	"    ],\n"
	"    \"booleanarrayval\" : \n"
	"    [\n"
	"      true,\n"
	"      false,\n"
	"      true,\n"
	"      false\n"
	"    ],\n"
	"    \"nullarrayval\" : \n"
	"    [\n"
	"      null,\n"
	"      null,\n"
	"      null,\n"
	"      null\n"
	"    ],\n"
	"    \"objectval\" : \n"
	"    {\n"
	"      \"stringval\" : \"string value\",\n"
	"      \"numberval\" : -1234.56,\n"
	"      \"trueval\" : true,\n"
	"      \"falseval\" : false,\n"
	"      \"nullval\" : null,\n"
	"      \"stringarrayval\" : \n"
	"      [\n"
	"        \"one\",\n"
	"        \"two\",\n"
	"        \"three\",\n"
	"        \"four\"\n"
	"      ],\n"
	"      \"numberarrayval\" : \n"
	"      [\n"
	"        1,\n"
	"        2,\n"
	"        3,\n"
	"        4\n"
	"      ],\n"
	"      \"booleanarrayval\" : \n"
	"      [\n"
	"        true,\n"
	"        false,\n"
	"        true,\n"
	"        false\n"
	"      ],\n"
	"      \"nullarrayval\" : \n"
	"      [\n"
	"        null,\n"
	"        null,\n"
	"        null,\n"
	"        null\n"
	"      ]\n"
	"    },\n"
	"    \"anotherstringval\" : \"string after object\"\n"
	"  },\n"
	"  \"anotherstringval\" : \"string after object\"\n"
	"}",
	false
	},

	{
	"complex elements, unindented",
	"{"
	  "\"stringval\" : \"string value\","
	  "\"numberval\" : -1234.56,"
	  "\"trueval\" : true,"
	  "\"falseval\" : false,"
	  "\"nullval\" : null,"
	  "\"stringarrayval\" : "
	  "["
	    "\"one\","
	    "\"two\","
	    "\"three\","
	    "\"four\""
	  "],"
	  "\"numberarrayval\" : "
	  "["
	    "1,"
	    "2,"
	    "3,"
	    "4"
	  "],"
	  "\"booleanarrayval\" : "
	  "["
	    "true,"
	    "false,"
	    "true,"
	    "false"
	  "],"
	  "\"nullarrayval\" : "
	  "["
	    "null,"
	    "null,"
	    "null,"
	    "null"
	  "],"
	  "\"objectval\" : "
	  "{"
	    "\"stringval\" : \"string value\","
	    "\"numberval\" : -1234.56,"
	    "\"trueval\" : true,"
	    "\"falseval\" : false,"
	    "\"nullval\" : null,"
	    "\"stringarrayval\" : "
	    "["
	      "\"one\","
	      "\"two\","
	      "\"three\","
	      "\"four\""
	    "],"
	    "\"numberarrayval\" : "
	    "["
	      "1,"
	      "2,"
	      "3,"
	      "4"
	    "],"
	    "\"booleanarrayval\" : "
	    "["
	      "true,"
	      "false,"
	      "true,"
	      "false"
	    "],"
	    "\"nullarrayval\" : "
	    "["
	      "null,"
	      "null,"
	      "null,"
	      "null"
	    "],"
	    "\"objectval\" : "
	    "{"
	      "\"stringval\" : \"string value\","
	      "\"numberval\" : -1234.56,"
	      "\"trueval\" : true,"
	      "\"falseval\" : false,"
	      "\"nullval\" : null,"
	      "\"stringarrayval\" : "
	      "["
	        "\"one\","
	        "\"two\","
	        "\"three\","
	        "\"four\""
	      "],"
	      "\"numberarrayval\" : "
	      "["
	        "1,"
	        "2,"
	        "3,"
	        "4"
	      "],"
	      "\"booleanarrayval\" : "
	      "["
	        "true,"
	        "false,"
	        "true,"
	        "false"
	      "],"
	      "\"nullarrayval\" : "
	      "["
	        "null,"
	        "null,"
	        "null,"
	        "null"
	      "]"
	    "},"
	    "\"anotherstringval\" : \"string after object\""
	  "},"
	  "\"anotherstringval\" : \"string after object\""
	"}",
	true
	},

	{
	NULL,
	NULL,
	true
	}
};

// parses json and returns the first value node of the root array
static bool firstValue(jsondom *j, const char *json, domnode **value) {
	if (!j->parseString(json)) {
		return false;
	}
	*value=j->getRootNode()->getFirstTagChild()->getFirstTagChild();
	return !(*value)->isNullNode();
}

int main() {

	header("jsondom");

	jsondom		j;
	stringbuffer	filename;

	for (const struct test *t=tests; t->name; t++) {

		stdoutput.printf("%s...\n",t->name);
		j.parseString(t->testval);
		filename.clear();
		filename.append(t->name)->append(".json");
		j.writeFile(filename.getString(),
				permissions::parsePermString("rw-r--r--"),
				true);
		char	*contents=file::getContents(filename.getString());
		test("file contents",
			!charstring::compare(
				(t->comparetoprevious)?
					(t-1)->testval:t->testval,
				contents));
		delete[] contents;
		test("parse file",j.parseFile(filename.getString()));
		file::remove(filename.getString());
	}

	// SQL Relay #8872: a jsondom tree built programmatically (not
	// parsed from text) with a field value containing an embedded
	// null must not have that value silently truncated when written.
	{
		stdoutput.printf("embedded null, built programmatically...\n");

		const char	value[]="ab\0cd";
		const size_t	valuelength=5;

		jsondom	built;
		built.createRootNode();
		domnode	*root=built.getRootNode();
		domnode	*obj=root->appendTag("r");
		obj->setAttributeValue("t","o");
		domnode	*field=obj->appendTag("field");
		field->setAttributeValue("t","s");
		field->setAttributeValue("v",value,valuelength);

		filename.clear();
		filename.append("embedded null")->append(".json");
		built.writeFile(filename.getString(),
				permissions::parsePermString("rw-r--r--"),
				true);
		char	*contents=file::getContents(filename.getString());

		test("value not truncated, contains escaped null",
			charstring::contains(contents,"\\u0000"));
		test("value not truncated, contains bytes after null",
			charstring::contains(contents,"cd"));

		delete[] contents;
		file::remove(filename.getString());
	}

	// string escapes and number syntax
	{
		stdoutput.printf("escapes and numbers...\n");

		jsondom	e;
		domnode	*v;
		char	*contents=NULL;

		test("\\u00e9 is utf-8",
			firstValue(&e,"[\"\\u00e9\"]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"\xc3\xa9"));
		test("surrogate pair is utf-8",
			firstValue(&e,"[\"\\ud83d\\ude00\"]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),
				"\xf0\x9f\x98\x80"));
		test("escaped solidus",
			firstValue(&e,"[\"a\\/b\"]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"a/b"));

		test("1e2",
			firstValue(&e,"[1e2]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"1e2"));
		test("-1.5E-3",
			firstValue(&e,"[-1.5E-3]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"-1.5E-3"));
		test("0e+1",
			firstValue(&e,"[0e+1]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"0e+1"));

		test("lone minus fails",!e.parseString("[-]"));
		test("minus dot fails",!e.parseString("[-.]"));
		test("missing exponent digits fails",!e.parseString("[1e]"));
		test("lone high surrogate fails",
			!e.parseString("[\"\\ud83d\"]"));
		test("lone low surrogate fails",
			!e.parseString("[\"\\ude00\"]"));
		test("\\u0000 kept as text",
			firstValue(&e,"[\"a\\u0000b\"]",&v) &&
			!charstring::compare(
				v->getAttributeValue("v"),"a\\u0000b"));
		firstValue(&e,"[\"q\\\"b\\\\c\\u0001\"]",&v);
		e.writeFile("escapes.json",
			permissions::parsePermString("rw-r--r--"),true);
		contents=file::getContents("escapes.json");
		test("quote, backslash and control escaped on write",
			charstring::contains(contents,
				"\"q\\\"b\\\\c\\u0001\""));
		delete[] contents;
		file::remove("escapes.json");
		test("bad hex fails",!e.parseString("[\"\\u12g4\"]"));
	}

	stdoutput.printf("\n");

	return 0;
}
