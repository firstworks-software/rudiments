// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/sys.h>
#include <rudiments/charstring.h>
#include <rudiments/memorymap.h>
#include <rudiments/error.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("memorymap");

	if (!memorymap::isSupported()) {
		stdoutput.printf("	not supported\n\n");
		return 0;
	}

	const char	*filename=
	#ifdef _WIN32
		"C:\\memorymap.txt";
	#else
		"memorymap.txt";
	#endif

	size_t	allocgran=sys::getAllocationGranularity();

	// create a file to test with
	file::remove(filename);
	file	fd;
	test("create file",
		fd.create(filename,permissions::parsePermString("rw-r--r--")));
	
	char	*buffer=new char[allocgran];
	for (uint16_t i=0; i<10; i++) {
		for (size_t j=0; j<allocgran; j++) {
			buffer[j]='0'+i;
		}
		fd.write(buffer,allocgran);
	}
	delete[] buffer;
	
	// loop, mapping blocks of the file
	memorymap	mm;
	for (uint16_t i=0; i<10; i++) {

		// attach to the next block
		test("attach",mm.attach(fd.getFileDescriptor(),
						i*allocgran,allocgran,
						PROT_READ,MAP_PRIVATE));

		// get a pointer to the block
		const char	*ptr=(const char *)mm.getData();
		test("valid ptr",(ptr!=NULL));

		// see if it contains the correct data
		bool	success=true;
		for (size_t j=0; j<allocgran; j++) {
			if (*(ptr+j)!='0'+i) {
				success=false;
				break;
			}
		}
		stringbuffer	blockname;
		blockname.append("block ")->append(i);
		test(blockname.getString(),success);

		// detach from the block
		test("detach",mm.detach());
	}
	stdoutput.printf("\n");

	// detach an unattached map
	memorymap	unattached;
	error::clearError();
	test("detach unattached",unattached.detach());
	test("detach unattached error",(error::getErrorNumber()==0));

	// detach twice
	error::clearError();
	test("detach twice",mm.detach());
	test("detach twice error",(error::getErrorNumber()==0));

	// failed attach, bad descriptor
	memorymap	badfd;
	error::clearError();
	test("attach bad descriptor",
		!badfd.attach(-1,0,allocgran,PROT_READ,MAP_PRIVATE));
	test("bad descriptor data",(badfd.getData()==NULL));
	test("bad descriptor size",(badfd.getSize()==0));
	error::clearError();
	test("detach after bad descriptor",badfd.detach());
	test("detach after bad descriptor error",
					(error::getErrorNumber()==0));

	// failed attach, writable map of a read-only descriptor
	file	rofd;
	test("open read-only",rofd.open(filename,O_RDONLY));
	memorymap	romm;
	bool	roattached=romm.attach(rofd.getFileDescriptor(),
					0,allocgran,
					PROT_READ|PROT_WRITE,MAP_SHARED);
	if (!roattached) {
		test("read-only data",(romm.getData()==NULL));
		test("read-only size",(romm.getSize()==0));
	}
	error::clearError();
	test("detach after read-only",romm.detach());
	test("detach after read-only error",(error::getErrorNumber()==0));
	rofd.close();

	// destructor after failed attach
	memorymap	*dmm=new memorymap;
	dmm->attach(-1,0,allocgran,PROT_READ,MAP_PRIVATE);
	error::clearError();
	delete dmm;
	test("destructor error",(error::getErrorNumber()==0));

	// view beyond the end of the file
	memorymap	beyond;
	beyond.attach(fd.getFileDescriptor(),
			20*allocgran,allocgran,PROT_READ,MAP_PRIVATE);
	error::clearError();
	test("detach after view beyond end",beyond.detach());
	test("detach after view beyond end error",
					(error::getErrorNumber()==0));
	stdoutput.printf("\n");

	// close the file
	fd.close();

	// clean up
	file::remove(filename);

	return 0;
}
