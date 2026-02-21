#include <rudiments/ucs2character.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// perform case conversions on all ascii characters
	for (int32_t uc=0; uc<128; uc++) {

		ucs2_t	c=(ucs2_t)uc;

		stdoutput.printf("character %c:\n",(char)c);

		stdoutput.printf("	to upper case	: %c\n",
				(char)ucs2character::upper(c));

		stdoutput.printf("	to lower case	: %c\n",
				(char)ucs2character::lower(c));

		// place a 1 in bit 8
		c=c|0x80;

		stdoutput.printf("	to ascii	: %c\n",
				(char)ucs2character::convertToAscii(c));
	}
}
