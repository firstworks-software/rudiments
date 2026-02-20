#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file using the static method
	file::createFile("testfile",
			permissions::parsePermString("rw-rw-rw-"));
	stdoutput.write("created testfile\n\n");


	// check if the file exists and its access properties
	stdoutput.printf("exists:      %s\n",
			(file::exists("testfile"))?"yes":"no");
	stdoutput.printf("readable:    %s\n",
			(file::isReadable("testfile"))?"yes":"no");
	stdoutput.printf("writeable:   %s\n",
			(file::isWriteable("testfile"))?"yes":"no");
	stdoutput.printf("executable:  %s\n\n",
			(file::isExecutable("testfile"))?"yes":"no");


	// get parts of a pathname
	char	*dir=file::getDirName("/home/user/testfile.txt");
	char	*base=file::getBaseName("/home/user/testfile.txt");
	char	*ext=file::getExtension("/home/user/testfile.txt");
	stdoutput.printf("dirname:   %s\n",dir);
	stdoutput.printf("basename:  %s\n",base);
	stdoutput.printf("extension: %s\n\n",ext);
	delete[] dir;
	delete[] base;
	delete[] ext;


	// get the contents of a file using the static method
	file	f;
	f.open("testfile",O_WRONLY);
	f.write("hello there");
	f.close();

	char	*contents=file::getContents("testfile");
	stdoutput.printf("contents: \"%s\"\n\n",contents);
	delete[] contents;


	// rename the file
	file::rename("testfile","testfile-renamed");
	stdoutput.write("renamed testfile to testfile-renamed\n");

	// remove the file
	file::remove("testfile-renamed");
	stdoutput.write("removed testfile-renamed\n");
}
