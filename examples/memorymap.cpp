#include <rudiments/memorymap.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// check if memory mapping is supported
	if (!memorymap::isSupported()) {
		stdoutput.write("memory mapping is not supported\n");
		return 1;
	}


	// create a file to map
	file	f;
	f.open("testfile",O_RDWR|O_CREAT,
				permissions::parsePermString("rw-rw-rw-"));

	// write some data to it
	const char	*data="hello world, this is memory mapped data!";
	f.write(data);


	// map the file into memory
	memorymap	mm;
	if (!mm.attach(f.getFileDescriptor(),0,f.getSize(),
						PROT_READ|PROT_WRITE,
						MAP_SHARED)) {
		stdoutput.write("failed to map file\n");
		return 1;
	}

	// read from the mapped region
	char	*mapped=(char *)mm.getData();
	stdoutput.printf("mapped data: %.*s\n",(int)mm.getSize(),mapped);


	// modify the data through the mapping
	bytestring::copy(mapped,"HELLO WORLD",11);
	stdoutput.printf("modified:    %.*s\n",(int)mm.getSize(),mapped);


	// sync the changes to disk
	mm.sync(true,false);
	stdoutput.write("synced to disk\n");


	// detach the mapping
	mm.detach();
	f.close();


	// clean up
	file::remove("testfile");
}
