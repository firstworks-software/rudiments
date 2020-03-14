// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/inidom.h>
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
	"semicolon comment",
	"; hello there\n",
	false
	},

	{
	"semicolon comments",
	"; hello there\n"
	"; hi there\n"
	"; bye  there\n"
	"; goodbye there\n",
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
	"hello=hi\n",
	false
	},

	{
	"keys",
	"hello=hi\n"
	"goodbye=bye\n",
	false
	},

	{
	"section",
	"[section1]\n"
	"key1=value1\n"
	"key2=value2\n"
	"key3=value3\n"
	"key4=value4\n",
	false
	},

	{
	"sections",
	"[section1]\n"
	"key1,1=value1,1\n"
	"key1,2=value1,2\n"
	"key1,3=value1,3\n"
	"key1,4=value1,4\n"
	"\n"
	"[section2]\n"
	"key2,1=value2,1\n"
	"key2,2=value2,2\n"
	"key2,3=value2,3\n"
	"key2,4=value2,4\n",
	false
	},

	{
	"complex",
	"\n"
	"; first we have a comment\n"
	"# then another comment\n"
	"\n"
	"; then some keys without sections\n"
	"key1=value1\n"
	"key2=value2\n"
	"key3=value3\n"
	"key4=value4\n"
	"\n"
	"# then one section\n"
	"[section1]\n"
	"key1,1=value1,1\n"
	"key1,2=value1,2\n"
	"key1,3=value1,3\n"
	"key1,4=value1,4\n"
	"\n"
	"; then another section\n"
	"[section2]\n"
	"key2,1=value2,1\n"
	"key2,2=value2,2\n"
	"key2,3=value2,3\n"
	"key2,4=value2,4\n"
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

	header("inidom");

	inidom		i;
	stringbuffer	filename;

	for (const struct test *t=tests; t->name; t++) {

		stdoutput.printf("\n%s...\n",t->name);
		i.parseString(t->testval);
		filename.clear();
		filename.append(t->name)->append(".ini");
		i.writeFile(filename.getString(),
				permissions::evalPermString("rw-r--r--"));
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
}
