#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// first/last instances of a character or string...
	const wchar_t	phrase[]=L"1 and 2 and 3 and 4";
	const wchar_t	numbers[]=L"1234";

	const wchar_t	*firsta=wcharstring::findFirst(phrase,L'a');
	const wchar_t	*firstand=wcharstring::findFirst(phrase,L"and");

	const wchar_t	*lasta=wcharstring::findLast(phrase,L'a');
	const wchar_t	*lastand=wcharstring::findLast(phrase,L"and");

	const wchar_t	*firstnum=wcharstring::findFirstOfSet(phrase,numbers);

	stdoutput.printf("in the phrase: \"%ls\"...\n",phrase);
	stdoutput.printf("  the first 'a' is	: \"%ls\"\n",firsta);
	stdoutput.printf("  the first \"and\" is	: \"%ls\"\n",firstand);
	stdoutput.printf("  the last 'a' is	: \"%ls\"\n",lasta);
	stdoutput.printf("  the last \"and\" is	: \"%ls\"\n",lastand);
	stdoutput.printf("  the first number is	: \"%ls\"\n",firstnum);
	stdoutput.write('\n');
}
