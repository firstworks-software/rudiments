// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/jsondom.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include "test.cpp"

const char normal[]=
	"{\n"
	"  \"stringval\" : \"string value\",\n"
	"  \"numberval\" : -1234.56,\n"
	"  \"trueval\" : true,\n"
	"  \"falseval\" : false,\n"
	"  \"nullval\" : null,\n"
	"  \"arrayval\" : \n"
	"  [\n"
	"    \"one\",\n"
	"    \"two\",\n"
	"    \"three\",\n"
	"    \"four\"\n"
	"  ],\n"
	"  \"objectval\" : \n"
	"  {\n"
	"    \"stringval\" : \"string value\",\n"
	"    \"numberval\" : -1234.56,\n"
	"    \"trueval\" : true,\n"
	"    \"falseval\" : false,\n"
	"    \"nullval\" : null,\n"
	"    \"arrayval\" : \n"
	"    [\n"
	"      \"one\",\n"
	"      \"two\",\n"
	"      \"three\",\n"
	"      \"four\"\n"
	"    ],\n"
	"    \"objectval\" : \n"
	"    {\n"
	"      \"stringval\" : \"string value\",\n"
	"      \"numberval\" : -1234.56,\n"
	"      \"trueval\" : true,\n"
	"      \"falseval\" : false,\n"
	"      \"nullval\" : null,\n"
	"      \"arrayval\" : \n"
	"      [\n"
	"        \"one\",\n"
	"        \"two\",\n"
	"        \"three\",\n"
	"        \"four\"\n"
	"      ]\n"
	"    }\n"
	"  }\n"
	"}";

const char noindent[]=
	"{"
	  "\"stringval\":\"string value\","
	  "\"numberval\":-1234.56,"
	  "\"trueval\":true,"
	  "\"falseval\":false,"
	  "\"nullval\":null,"
	  "\"arrayval\":"
	  "["
	    "\"one\","
	    "\"two\","
	    "\"three\","
	    "\"four\""
	  "],"
	  "\"objectval\":"
	  "{"
	    "\"stringval\":\"string value\","
	    "\"numberval\":-1234.56,"
	    "\"trueval\":true,"
	    "\"falseval\":false,"
	    "\"nullval\":null,"
	    "\"arrayval\":"
	    "["
	      "\"one\","
	      "\"two\","
	      "\"three\","
	      "\"four\""
	    "],"
	    "\"objectval\":"
	    "{"
	      "\"stringval\":\"string value\","
	      "\"numberval\":-1234.56,"
	      "\"trueval\":true,"
	      "\"falseval\":false,"
	      "\"nullval\":null,"
	      "\"arrayval\":"
	      "["
	        "\"one\","
	        "\"two\","
	        "\"three\","
	        "\"four\""
	      "]"
	    "}"
	  "}"
	"}";

int main() {

	header("jsondom");

	jsondom	j;

	stdoutput.printf("normal...\n");
	j.parseString(normal);
	j.writeFile("normal.json",permissions::evalPermString("rw-r--r--"));
	char	*normaljson=file::getContents("normal.json");
	test("file contents",!charstring::compare(normal,normaljson));
	delete[] normaljson;
	test("parse file",j.parseFile("normal.json"));
	file::remove("normal.json");

	stdoutput.printf("noindent...\n");
	j.parseString(noindent);
	j.writeFile("noindent.json",permissions::evalPermString("rw-r--r--"));
	char	*noindentjson=file::getContents("noindent.json");
	test("file contents",!charstring::compare(normal,noindentjson));
	delete[] noindentjson;
	test("parse file",j.parseFile("noindent.json"));
	file::remove("noindent.json");

	stdoutput.printf("\n");
}
