#include <rudiments/wcharacter.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// print information about all ascii characters
	for (int32_t uc=0; uc<128; uc++) {

		wchar_t	c=(wchar_t)uc;

		stdoutput.printf("character %lc:\n",c);

		stdoutput.printf("	is alphanumeric		: %s\n",
				(wcharacter::isAlphanumeric(c))?"yes":"no");
		stdoutput.printf("	is alphabetical		: %s\n",
				(wcharacter::isAlphabetical(c))?"yes":"no");
		stdoutput.printf("	is lower case		: %s\n",
				(wcharacter::isLowerCase(c))?"yes":"no");
		stdoutput.printf("	is upper case		: %s\n",
				(wcharacter::isUpperCase(c))?"yes":"no");
		stdoutput.printf("	is punctuation		: %s\n",
				(wcharacter::isPunctuation(c))?"yes":"no");
		stdoutput.printf("	is printable		: %s\n",
				(wcharacter::isPrintable(c))?"yes":"no");
		stdoutput.printf("	is printable (non-space): %s\n",
				(wcharacter::isPrintableNonSpace(c))?"yes":"no");
		stdoutput.printf("	is control character	: %s\n",
				(wcharacter::isControlCharacter(c))?"yes":"no");
		stdoutput.printf("	is digit		: %s\n",
				(wcharacter::isDigit(c))?"yes":"no");
		stdoutput.printf("	is hex digit		: %s\n",
				(wcharacter::isHexDigit(c))?"yes":"no");
		stdoutput.printf("	is blank		: %s\n",
				(wcharacter::isBlank(c))?"yes":"no");
		stdoutput.printf("	is whitespace		: %s\n",
				(wcharacter::isWhitespace(c))?"yes":"no");
		stdoutput.printf("	is ascii		: %s\n",
				(wcharacter::isAscii(c))?"yes":"no");
	}
}
