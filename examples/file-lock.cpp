#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// open/create a file
	file	f;
	f.open("testfile",O_RDWR|O_CREAT,
				permissions::parsePermString("rw-rw-rw-"));
	f.write("hello world, this is a test file\n");


	// lock the entire file for writing
	if (f.lockFile(F_WRLCK)) {
		stdoutput.write("locked entire file for write\n");
	}

	// unlock the entire file
	if (f.unlockFile()) {
		stdoutput.write("unlocked entire file\n\n");
	}


	// lock a region of the file for reading
	if (f.lockRegion(F_RDLCK,0,10)) {
		stdoutput.write("locked bytes 0-10 for read\n");
	}

	// check if the region can be write-locked
	int16_t		conftype;
	int16_t		confwhence;
	off64_t		confstart;
	off64_t		conflen;
	if (!f.checkLockRegion(F_WRLCK,0,10,
				&conftype,&confwhence,
				&confstart,&conflen)) {
		stdoutput.printf("cannot write-lock: "
				"conflicting lock at %lld for %lld bytes\n",
				confstart,conflen);
	}

	// unlock the region
	if (f.unlockRegion(0,10)) {
		stdoutput.write("unlocked bytes 0-10\n\n");
	}


	// try to lock the remainder of the file from position 5
	if (f.lockRemainder(F_WRLCK,5)) {
		stdoutput.write("locked remainder from byte 5\n");
	}

	// unlock the remainder
	if (f.unlockRemainder(5)) {
		stdoutput.write("unlocked remainder from byte 5\n");
	}
}
