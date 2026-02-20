#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// parse a permission string into a mode
	mode_t	mode=permissions::parsePermString("rwxr-xr--");
	stdoutput.printf("rwxr-xr-- = %04o\n",mode);


	// convert a mode back to a permission string
	char	*permstring=permissions::parsePermOctal(mode);
	stdoutput.printf("%04o = %s\n\n",mode,permstring);
	delete[] permstring;


	// build permissions using convenience methods
	mode_t	perms=permissions::getOwnerReadWrite()|
			permissions::getGroupRead()|
			permissions::getOthersRead();
	permstring=permissions::parsePermOctal(perms);
	stdoutput.printf("owner-rw + group-r + others-r = %s\n\n",
							permstring);
	delete[] permstring;


	// some common permission values
	permstring=permissions::parsePermOctal(
				permissions::getEveryoneReadWrite());
	stdoutput.printf("everyone-rw  = %s\n",permstring);
	delete[] permstring;

	permstring=permissions::parsePermOctal(
				permissions::getEveryoneReadWriteExecute());
	stdoutput.printf("everyone-rwx = %s\n",permstring);
	delete[] permstring;

	permstring=permissions::parsePermOctal(
				permissions::getOwnerReadWriteExecute());
	stdoutput.printf("owner-rwx    = %s\n",permstring);
	delete[] permstring;
}
