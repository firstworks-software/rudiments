// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include "test.cpp"

#ifndef _WIN32
const char	*permsfile="permissions.txt";
#else
// local file not reliable on SMB mount
const char	*permsfile="C:\\permissions.txt";
#endif

int main(int argc, const char **argv) {

	header("permissions");

        file::remove(permsfile);


	stdoutput.printf("perm strings...\n");
        file	fd;
	test("create with perms",
			fd.open(permsfile,O_RDWR|O_CREAT,
                                permissions::evalPermString("rw-r--r--")));
        test("change perms",
		permissions::setFilePermissions(fd.getFileDescriptor(),
                                permissions::evalPermString("rw-rw-r--")));
	fd.getCurrentProperties();
	test("get perms",!charstring::compare(
				permissions::evalPermOctal(fd.getPermissions()),
				"rw-rw-r--"));
        fd.close();
        file::remove(permsfile);
	stdoutput.printf("\n");


	stdoutput.printf("perm octals...\n");
        test("create with perms",
			fd.open(permsfile,O_RDWR|O_CREAT,
                                	permissions::ownerReadWrite()|
                                	permissions::groupRead()|
                                	permissions::othersRead()));
	fd.getCurrentProperties();
        test("get perms",(fd.getPermissions()&0x0fff)==
                                	(permissions::ownerReadWrite()|
                                	permissions::groupRead()|
                                	permissions::othersRead()));
        test("change perms",
        	permissions::setFilePermissions(fd.getFileDescriptor(),
                                	permissions::ownerReadWrite()|
                                	permissions::groupReadWrite()|
                                	permissions::othersRead()));
	fd.getCurrentProperties();
        test("get perms",(fd.getPermissions()&0x0fff)==
                                	(permissions::ownerReadWrite()|
                                	permissions::groupReadWrite()|
                                	permissions::othersRead()));
        fd.close();
        file::remove(permsfile);
	stdoutput.printf("\n");
}
