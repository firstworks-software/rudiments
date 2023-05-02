#include <rudiments/character.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// perform case conversions on all ascii characters
	for (byte_t uc=0; uc<128; uc++) {

		char	c=(char)uc;
		
		stdoutput.write("character ");
		stdoutput.safePrint(c);
		stdoutput.write(":\n");

		stdoutput.write("	to upper case	: ");
		stdoutput.safePrint((char)character::upper(c));
		stdoutput.write("\n");

		stdoutput.write("	to lower case	: ");
		stdoutput.safePrint((char)character::lower(c));
		stdoutput.write("\n");

		// place a 1 in bit 8
		c=c|0x80;

		stdoutput.write("	to ascii	: ");
		stdoutput.safePrint((char)character::convertToAscii(c));
		stdoutput.write("\n");
	}
}
