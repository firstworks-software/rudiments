// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/jsondom.h>
#include <rudiments/domnode.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/bytestring.h>
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

// Counts writes and fails call number "failat" and every call after it
// with "failcode".  If "shortwrite" is set, call number "failat" writes
// half of its data instead (none of a single byte) and returns that count,
// and the calls after it succeed.  Counts the calls made after the failure
// and records the size of call number "failat" and the length of the buffer
// after each call.
class failbuffer : public stringbuffer {
	public:
				failbuffer(size_t failat, bool shortwrite,
						ssize_t failcode=RESULT_ERROR);
		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string, size_t length);
		size_t	writes;
		size_t	writesafterfail;
		size_t	failsize;
		size_t	offsets[512];
	private:
		ssize_t	failWrite(const byte_t *data, size_t size);
		size_t	failat;
		bool	shortwrite;
		ssize_t	failcode;
};

failbuffer::failbuffer(size_t failat, bool shortwrite, ssize_t failcode) :
								stringbuffer() {
	writes=0;
	writesafterfail=0;
	failsize=0;
	for (uint16_t i=0; i<512; i++) {
		offsets[i]=0;
	}
	this->failat=failat;
	this->shortwrite=shortwrite;
	this->failcode=failcode;
}

ssize_t failbuffer::write(const byte_t *string, size_t size) {
	return failWrite(string,size);
}

ssize_t failbuffer::write(const char *string, size_t length) {
	return failWrite((const byte_t *)string,length);
}

ssize_t failbuffer::failWrite(const byte_t *data, size_t size) {
	writes++;
	if (failat && writes>failat) {
		writesafterfail++;
	}
	if (failat && writes==failat) {
		failsize=size;
	}
	ssize_t	result;
	if (failat && writes==failat && shortwrite) {
		result=stringbuffer::write(data,size/2);
	} else if (failat && writes>=failat && !shortwrite) {
		result=failcode;
	} else {
		result=stringbuffer::write(data,size);
	}
	if (writes<=512) {
		offsets[writes-1]=getStringLength();
	}
	return result;
}

// writeNode() is protected, so expose it to write a node on its own
class jsondomtest : public jsondom {
	public:
		ssize_t	writeMember(domnode *dn, output *out, bool indent) {
			uint16_t	indentlevel=0;
			return writeNode(dn,out,indent,&indentlevel);
		}
};

// document with nested objects and arrays
const char	nested[]=
"{ \"a\" : 1,\n"
"  \"b\" : [ 1, \"x\\\"y\", { \"c\" : true, \"d\" : null },\n"
"          { \"h\" : [ 2, 3 ] } ],\n"
"  \"e\" : { \"f\" : [], \"g\" : {} } }";

const char	nestedunindented[]=
"{"
"\"a\":1,"
"\"b\":[1,\"x\\\"y\",{\"c\":true,\"d\":null},{\"h\":[2,3]}],"
"\"e\":{\"f\":[],\"g\":{}}"
"}";

const char	nestedindented[]=
"{\n"
"  \"a\" : 1,\n"
"  \"b\" : \n"
"  [\n"
"    1,\n"
"    \"x\\\"y\",\n"
"    {\n"
"      \"c\" : true,\n"
"      \"d\" : null\n"
"    },\n"
"    {\n"
"      \"h\" : \n"
"      [\n"
"        2,\n"
"        3\n"
"      ]\n"
"    }\n"
"  ],\n"
"  \"e\" : \n"
"  {\n"
"    \"f\" : \n"
"    [\n"
"    ],\n"
"    \"g\" : \n"
"    {\n"
"    }\n"
"  }\n"
"}";

// clean and failed runs of writeJson() with and without indent
static void testWriteJsonFailWrite() {

	// writeJson() is protected in jsondom, so go through a dom pointer
	jsondomtest	j;
	j.parseString(nested);
	dom		*d=&j;

	// the "a" member of the top object, and what it writes by itself
	domnode		*member=j.getRootNode()->getFirstTagChild()->
							getFirstTagChild();
	const char	*memberexpected[]={"\"a\":1","\"a\" : 1"};

	const char	*names[]={"writeJson","writeJson indent"};
	const char	*expected[]={nestedunindented,nestedindented};

	// hard error codes, 0 for a short write
	const ssize_t	codes[]={RESULT_ERROR,RESULT_TIMEOUT,RESULT_ABORT,0};

	for (uint16_t i=0; i<2; i++) {

		// clean run
		failbuffer	clean(0,false);
		ssize_t	cleanresult=d->writeJson(&clean,i!=0);
		size_t	total=clean.writes;
		stringbuffer	title;
		test("write count fits the offsets",total<=512);
		title.append(names[i])->append(": clean run");
		test(title.getString(),
			cleanresult==
				(ssize_t)charstring::getLength(expected[i]) &&
			cleanresult==(ssize_t)clean.getStringLength() &&
			!charstring::compare(clean.getString(),expected[i]));

		// fail or shorten each write
		for (uint16_t c=0; c<4; c++) {
			bool	shortwrite=!codes[c];
			for (size_t failat=1; failat<=total; failat++) {
				failbuffer	fb(failat,shortwrite,
						(shortwrite)?RESULT_ERROR:
								codes[c]);
				ssize_t	result=d->writeJson(&fb,i!=0);
				ssize_t	wanted=(shortwrite)?
							RESULT_ERROR:codes[c];
				title.clear();
				title.append(names[i]);
				if (shortwrite) {
					title.append(": short write ");
				} else {
					title.append(": fail write (");
					title.append((int64_t)codes[c]);
					title.append(") ");
				}
				title.append((uint64_t)failat)->append(" of ");
				title.append((uint64_t)total);
				test(title.getString(),
					result==wanted &&
					fb.writesafterfail==0);

				// the text written so far is the clean text up
				// to the failed write, plus half of it for a
				// short write
				size_t	wantlen=((failat>1)?
						clean.offsets[failat-2]:0)+
						((shortwrite)?
						fb.failsize/2:0);
				title.append(", text so far");
				test(title.getString(),
					fb.getStringLength()==wantlen &&
					!bytestring::compare(fb.getString(),
							expected[i],wantlen));

				// a member node written by itself prints its
				// name only if the failed write left no
				// in-array state behind
				failbuffer	mem(0,false);
				ssize_t	memresult=
					j.writeMember(member,&mem,i!=0);
				title.clear();
				title.append(names[i])->append(" after ");
				title.append((shortwrite)?"short write ":
								"fail write ");
				title.append((uint64_t)failat)->append(" of ");
				title.append((uint64_t)total);
				title.append(", member write");
				test(title.getString(),
					memresult==
					(ssize_t)charstring::getLength(
							memberexpected[i]) &&
					!charstring::compare(mem.getString(),
							memberexpected[i]));
			}
		}
	}
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

	stdoutput.printf("writeJson...\n");
	testWriteJsonFailWrite();

	stdoutput.printf("\n");

	return 0;
}
