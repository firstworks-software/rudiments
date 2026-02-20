#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file	f;

	// disable getCurrentProperties() on open for a performance
	// improvement when opening many files
	f.setGetCurrentPropertiesOnOpen(false);

	// open a file
	f.open("testfile",O_RDWR|O_CREAT,
				permissions::parsePermString("rw-rw-rw-"));
	f.write("hello world, this is test data for optimization testing\n");
	stdoutput.write("opened file with properties-on-open disabled\n\n");


	// advise the kernel that we will access the file sequentially
	f.adviseSequentialAccess(0,f.getSize());
	stdoutput.write("advised sequential access\n");


	// advise the kernel that we will need the data soon
	f.adviseWillNeed(0,f.getSize());
	stdoutput.write("advised will-need\n");


	// read the file
	char	buf[1024];
	f.setPositionRelativeToBeginning(0);
	ssize_t	bytesread=f.read(buf,sizeof(buf)-1);
	if (bytesread>0) {
		buf[bytesread]='\0';
		stdoutput.printf("read: %s",buf);
	}


	// advise the kernel that we are done with the data
	f.adviseWontNeed(0,f.getSize());
	stdoutput.write("advised wont-need\n\n");


	// reset to normal access pattern
	f.adviseNormalAccess(0,f.getSize());
	stdoutput.write("advised normal access\n");


	// sync data to disk
	f.sync();
	stdoutput.write("synced to disk\n");
}
