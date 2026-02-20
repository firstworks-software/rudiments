#include <rudiments/sensitivevalue.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	sensitivevalue	sv;


	// parse a verbatim string
	sv.parse("Passw0rd");

	stdoutput.printf("verbatim: %s\n",sv.getTextValue());


	// parse a hex-encoded verbatim string
	sv.setVerbatimFormat(FORMAT_HEX);

	sv.parse("5061737377307264");

	stdoutput.printf("from hex: %s\n",sv.getTextValue());


	// reset to defaults
	sv.reset();


	// write a password to a file
	file::createFile("password.txt",0600);
	file	f;
	f.open("password.txt",O_WRONLY);
	f.write("Passw0rd\n");
	f.close();


	// parse a value from a file
	// (the brackets indicate that the value
	// should be read from the specified file)
	sv.parse("[password.txt]");

	stdoutput.printf("from file: %s\n",sv.getTextValue());


	// parse a value from a file without chomping
	sv.setChompTextFile(false);

	sv.parse("[password.txt]");

	stdoutput.printf("from file (unchomped): %s",sv.getTextValue());


	// clean up
	file::remove("password.txt");
}
