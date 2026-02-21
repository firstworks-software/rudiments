#include <rudiments/ucs2character.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// print information about all ascii characters
	for (int32_t uc=0; uc<128; uc++) {

		ucs2_t	c=(ucs2_t)uc;

		stdoutput.printf("character %c:\n",(char)c);

		stdoutput.printf("	is alphanumeric		: %s\n",
				(ucs2character::isAlphanumeric(c))?"yes":"no");
		stdoutput.printf("	is alphabetical		: %s\n",
				(ucs2character::isAlphabetical(c))?"yes":"no");
		stdoutput.printf("	is lower case		: %s\n",
				(ucs2character::isLowerCase(c))?"yes":"no");
		stdoutput.printf("	is upper case		: %s\n",
				(ucs2character::isUpperCase(c))?"yes":"no");
		stdoutput.printf("	is punctuation		: %s\n",
				(ucs2character::isPunctuation(c))?"yes":"no");
		stdoutput.printf("	is printable		: %s\n",
				(ucs2character::isPrintable(c))?"yes":"no");
		stdoutput.printf("	is printable (non-space): %s\n",
				(ucs2character::isPrintableNonSpace(c))?"yes":"no");
		stdoutput.printf("	is control character	: %s\n",
				(ucs2character::isControlCharacter(c))?"yes":"no");
		stdoutput.printf("	is digit		: %s\n",
				(ucs2character::isDigit(c))?"yes":"no");
		stdoutput.printf("	is hex digit		: %s\n",
				(ucs2character::isHexDigit(c))?"yes":"no");
		stdoutput.printf("	is blank		: %s\n",
				(ucs2character::isBlank(c))?"yes":"no");
		stdoutput.printf("	is whitespace		: %s\n",
				(ucs2character::isWhitespace(c))?"yes":"no");
		stdoutput.printf("	is ascii		: %s\n",
				(ucs2character::isAscii(c))?"yes":"no");
	}
}
