#include <rudiments/directory.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a directory
	directory::create("testdir",
			permissions::parsePermString("rwxr-xr-x"));
	stdoutput.write("created testdir\n\n");


	// print the current working directory
	char	*cwd=directory::getCurrentWorkingDirectory();
	stdoutput.printf("current directory: %s\n\n",cwd);
	delete[] cwd;


	// open the directory and read its contents
	directory	d;
	d.open(".");

	stdoutput.write("directory contents:\n");
	char	*entry=d.read();
	while (entry) {
		stdoutput.printf("  %s\n",entry);
		delete[] entry;
		entry=d.read();
	}
	stdoutput.write('\n');

	// print the child count
	stdoutput.printf("child count: %lld\n\n",d.getChildCount());

	// print the name of the first child
	char	*child=d.getChildName(0);
	stdoutput.printf("first child: %s\n\n",child);
	delete[] child;

	// print max filename length
	stdoutput.printf("max filename length: %lld\n\n",
					d.getMaxFileNameLength());

	d.close();


	// remove the directory
	directory::remove("testdir");
	stdoutput.write("removed testdir\n");
}
