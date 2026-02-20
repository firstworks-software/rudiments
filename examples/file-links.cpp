#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file to link to
	file	f;
	f.create("testfile",permissions::parsePermString("rw-rw-rw-"));
	f.write("hello from testfile\n");
	f.close();


	// create a hard link
	if (file::createHardLink("testfile","testfile-hardlink")) {
		stdoutput.write("created hard link: testfile-hardlink\n");
	} else {
		stdoutput.write("failed to create hard link\n");
	}


	// create a symbolic link
	if (file::createSymbolicLink("testfile","testfile-symlink")) {
		stdoutput.write("created symbolic link: testfile-symlink\n");
	} else {
		stdoutput.write("failed to create symbolic link\n");
	}


	// resolve the symbolic link
	char	*target=file::resolveSymbolicLink("testfile-symlink");
	if (target) {
		stdoutput.printf("testfile-symlink -> %s\n\n",target);
		delete[] target;
	}


	// check the number of hard links
	f.open("testfile",O_RDONLY);
	stdoutput.printf("hard link count: %d\n",
					f.getNumberOfHardLinks());
	f.close();


	// clean up
	file::remove("testfile-hardlink");
	file::remove("testfile-symlink");
	file::remove("testfile");
}
