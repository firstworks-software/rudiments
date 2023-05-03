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
                                	permissions::getOwnerReadWrite()|
                                	permissions::getGroupRead()|
                                	permissions::getOthersRead()));
	fd.getCurrentProperties();
        test("get perms",(fd.getPermissions()&0x0fff)==
                                	(permissions::getOwnerReadWrite()|
                                	permissions::getGroupRead()|
                                	permissions::getOthersRead()));
        test("change perms",
        	permissions::setFilePermissions(fd.getFileDescriptor(),
                                	permissions::getOwnerReadWrite()|
                                	permissions::getGroupReadWrite()|
                                	permissions::getOthersRead()));
	fd.getCurrentProperties();
        test("get perms",(fd.getPermissions()&0x0fff)==
                                	(permissions::getOwnerReadWrite()|
                                	permissions::getGroupReadWrite()|
                                	permissions::getOthersRead()));
        fd.close();
        file::remove(permsfile);
	stdoutput.printf("\n");

	return 0;
}
