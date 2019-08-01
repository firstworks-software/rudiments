// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/jsondom.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include "test.cpp"

struct test {
	const char	*name;
	const char	*test;
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

int main() {

	header("jsondom");

	jsondom		j;
	stringbuffer	filename;

	for (const struct test *t=tests; t->name; t++) {

		stdoutput.printf("%s...\n",t->name);
		j.parseString(t->test);
		filename.clear();
		filename.append(t->name)->append(".json");
		j.writeFile(filename.getString(),
				permissions::evalPermString("rw-r--r--"));
		char	*contents=file::getContents(filename.getString());
		test("file contents",
			!charstring::compare(
				(t->comparetoprevious)?(t-1)->test:t->test,
				contents));
		delete[] contents;
		test("parse file",j.parseFile(filename.getString()));
		file::remove(filename.getString());
	}

	stdoutput.printf("\n");
}
