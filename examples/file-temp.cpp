#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a temporary file
	char	templatename1[]="/tmp/testfileXXXXXX";
	int32_t	fd=file::createTemporaryFile(templatename1);
	if (fd>-1) {
		stdoutput.printf("created temp file: %s\n",templatename1);

		// clean up
		file::remove(templatename1);
	} else {
		stdoutput.write("failed to create temp file\n");
	}


	// create a temporary file with specific permissions
	char	templatename2[]="/tmp/testfileXXXXXX";
	fd=file::createTemporaryFile(templatename2,
				permissions::parsePermString("rw-------"));
	if (fd>-1) {
		stdoutput.printf("created temp file: %s\n",templatename2);
		stdoutput.write("  permissions: owner read/write only\n");

		// clean up
		file::remove(templatename2);
	} else {
		stdoutput.write("failed to create temp file\n");
	}
}
