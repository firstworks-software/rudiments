// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>
#include "test.cpp"

/*#ifdef _WIN32
const char	*dir="C:\\";
#else
const char	*dir="/usr/local";
#endif*/

int main(int argc, const char **argv) {

	header("directory");

	// clean up, just in case
	#ifdef _WIN32
	file::remove("testdir\\testfile1");
	file::remove("testdir\\testfile2");
	#else
	file::remove("testdir/testfile1");
	file::remove("testdir/testfile2");
	#endif
	directory::remove("testdir");


	// create
	stdoutput.printf("create...\n");
	test("create testdir",
			directory::create("testdir",
				permissions::parsePermString("rwxr-xr-x")));
	file	f;
	test("create testdir/testfile1",
			f.create("testdir/testfile1",
				permissions::parsePermString("rw-r--r--")));
	f.close();
	test("create testdir/testfile2",
			f.create("testdir/testfile2",
				permissions::parsePermString("rw-r--r--")));
	f.close();
	stdoutput.printf("\n");

	const char *filenames[]={".","..","testfile1","testfile2",NULL};

	// contents
	stdoutput.printf("contents...\n");
	directory	d;
	test("open",d.open("testdir"));
	d.rewind();
	char	*name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;

	d.rewind();
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;

	d.close();
	stdoutput.printf("\n");


	// contents by index
	stdoutput.printf("contents by index...\n");
	test("open",d.open("testdir"));
	test("count",d.getChildCount()==4);
	uint64_t	i=0;
	name=d.getChildName(i++);
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::isInSet(name,filenames));
	delete[] name;
	d.close();
	stdoutput.printf("\n");


	// current dir/change dir
	stdoutput.printf("current dir/change dir...\n");
	char	*cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	const char	*dir=charstring::findLast(cwd,"\\test");
	test("\\test",!charstring::compare(dir,"\\test"));
	#else
	const char	*dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	#endif
	delete[] cwd;
	directory::changeDirectory("testdir");
	cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	dir=charstring::findLast(cwd,"\\test\\testdir");
	test("\\test\\testdir",!charstring::compare(dir,"\\test\\testdir"));
	#else
	dir=charstring::findLast(cwd,"/test/testdir");
	test("/test/testdir",!charstring::compare(dir,"/test/testdir"));
	#endif
	delete[] cwd;
	directory::changeDirectory("..");
	cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	dir=charstring::findLast(cwd,"\\test");
	test("\\test",!charstring::compare(dir,"\\test"));
	#else
	dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	#endif
	delete[] cwd;
	stdoutput.printf("\n");


	// remove
	stdoutput.printf("remove...\n");
	#ifdef _WIN32
	test("remove testdir\\testfile1",file::remove("testdir\\testfile1"));
	test("remove testdir\\testfile2",file::remove("testdir\\testfile2"));
	#else
	test("remove testdir/testfile1",file::remove("testdir/testfile1"));
	test("remove testdir/testfile2",file::remove("testdir/testfile2"));
	#endif
	test("remove testdir",directory::remove("testdir"));
	stdoutput.printf("\n");


	// path-related
	stdoutput.printf("path-related...\n");
	cwd=directory::getCurrentWorkingDirectory();
	// not always valid with nfs/cifs
	/*test("getMaxFileNameLength valid",
			directory::getMaxFileNameLength(cwd)>0);
	test("getMaxPathLength valid",
			directory::getMaxPathLength(cwd)>0);*/
	d.open(cwd);

	// Syllable and Irix have bugs
	char	*osname=sys::getOperatingSystemName();
	if (charstring::compare(osname,"IRIX")) {
		if (charstring::compare(osname,"syllable")) {
			test("getMaxFileNameLength cross-check",
				directory::getMaxFileNameLength(cwd)==
				d.getMaxFileNameLength());
		}
		test("getMaxPathLength cross-check",
				directory::getMaxPathLength(cwd)==
				d.getMaxPathLength());
		test("getCanExceedMaxFileNameLength cross-check",
				directory::getCanExceedMaxFileNameLength(cwd)==
				d.getCanExceedMaxFileNameLength());
	}
	delete[] osname;
	d.close();
	delete[] cwd;

	// chroot?

	stdoutput.printf("\n");

	return 0;
}
