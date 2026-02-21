#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// define the set
	ucs2literal(set,"abcdefg");

	// duplicate the set as a char string for display
	char	*setstr=charstring::duplicate("abcdefg");

	// loop through the alphabet...
	for (ucs2_t c=(ucs2_t)'a'; c<=(ucs2_t)'z'; c++) {
		stdoutput.printf("is %c in the set \"%s\"?  %s\n",
				(char)c,setstr,
				(ucs2character::isInSet(c,set))?"yes":"no");
	}

	delete[] setstr;
}
