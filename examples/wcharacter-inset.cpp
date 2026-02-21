#include <rudiments/wcharacter.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// define the set
	const wchar_t	set[]=L"abcdefg";

	// loop through the alphabet...
	for (wchar_t c=L'a'; c<=L'z'; c++) {
		stdoutput.printf("is %lc in the set \"%ls\"?  %s\n",
				c,set,(wcharacter::isInSet(c,set))?"yes":"no");
	}
}
