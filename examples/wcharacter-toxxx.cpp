#include <rudiments/wcharacter.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// perform case conversions on all ascii characters
	for (int32_t uc=0; uc<128; uc++) {

		wchar_t	c=(wchar_t)uc;

		stdoutput.printf("character %lc:\n",c);

		stdoutput.printf("	to upper case	: %lc\n",
				(wchar_t)wcharacter::upper(c));

		stdoutput.printf("	to lower case	: %lc\n",
				(wchar_t)wcharacter::lower(c));

		// place a 1 in bit 8
		c=c|0x80;

		stdoutput.printf("	to ascii	: %lc\n",
				(wchar_t)wcharacter::convertToAscii(c));
	}
}
