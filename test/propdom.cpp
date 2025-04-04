// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/propdom.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
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
	false
	},

	{
	"exclamation comment",
	"! hello there\n",
	false
	},

	{
	"exclamation comments",
	"! hello there\n"
	"! hi there\n"
	"! bye  there\n"
	"! goodbye there\n",
	false
	},

	{
	"pound comment",
	"# hello there\n",
	false
	},

	{
	"pound comments",
	"# hello there\n"
	"# hi there\n"
	"# bye  there\n"
	"# goodbye there\n",
	false
	},

	{
	"key",
	"key1=value1\n",
	false
	},

	{
	"keys",
	"key1=value1\n"
	"key2=value2\n",
	false
	},

	{
	"multi-line values",
	"key1=val\\\nue\\\n1\n"
	"key2=val\\\nue\\\n2\n",
	false
	},

	{
	"various escapes",
	"key\\ 1=val\\ ue\\ 1\n"
	"key\\ 2=val\\ ue\\ 2\n",
	false
	},

	{
	"various equals",
	"key1=value1\n"
	"key2 =value2\n"
	"key3 = value3\n"
	"key4:value4\n"
	"key5 :value5\n"
	"key6 : value6\n"
	"key7 value7\n",
	false
	},

	{
	"complex",
	"\n"
	"! first we have a comment\n"
	"# then another comment\n"
	"\n"
	"! then some keys\n"
	"key1=value1\n"
	"key2 = value2\n"
	"key3=value3\n"
	"key4 = value4\n"
	"\n"
	"# and finally some trailing comments\n",
	false
	},

	{
	NULL,
	NULL,
	true
	}
};

int main() {

	header("propdom");

	propdom		i;
	stringbuffer	filename;

	for (const struct test *t=tests; t->name; t++) {

		stdoutput.printf("\n%s...\n",t->name);
		i.parseString(t->testval);
		filename.clear();
		filename.append(t->name)->append(".prop");
		i.writeFile(filename.getString(),
				permissions::parsePermString("rw-r--r--"));
		char	*contents=file::getContents(filename.getString());
		test("file contents",
			!charstring::compare(
				(t->comparetoprevious)?
					(t-1)->testval:t->testval,
				contents));
		delete[] contents;
		test("parse file",i.parseFile(filename.getString()));
		file::remove(filename.getString());
	}

	stdoutput.printf("\n");

	return 0;
}
