#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a fifo
	if (file::createFifo("testfifo",
				permissions::parsePermString("rw-rw-rw-"))) {
		stdoutput.write("created testfifo\n");
	} else {
		stdoutput.write("failed to create testfifo\n");
	}


	// verify that it is a fifo
	file	f;
	f.open("testfifo",O_RDONLY|O_NONBLOCK);
	stdoutput.printf("is fifo: %s\n",(f.isFifo())?"yes":"no");
	f.close();


	// clean up
	file::remove("testfifo");
	stdoutput.write("removed testfifo\n");
}
