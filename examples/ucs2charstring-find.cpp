#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// first/last instances of a character or string...
	ucs2literal(phrase,"1 and 2 and 3 and 4");
	ucs2literal(numbers,"1234");
	ucs2literal(andstr,"and");

	const ucs2_t	*firsta=ucs2charstring::findFirst(phrase,(ucs2_t)'a');
	const ucs2_t	*firstand=ucs2charstring::findFirst(phrase,andstr);

	const ucs2_t	*lasta=ucs2charstring::findLast(phrase,(ucs2_t)'a');
	const ucs2_t	*lastand=ucs2charstring::findLast(phrase,andstr);

	const ucs2_t	*firstnum=ucs2charstring::findFirstOfSet(
							phrase,numbers);

	char	*phrasestr=charstring::duplicateUcs2(phrase);
	char	*firstastr=charstring::duplicateUcs2(firsta);
	char	*firstandstr=charstring::duplicateUcs2(firstand);
	char	*lastastr=charstring::duplicateUcs2(lasta);
	char	*lastandstr=charstring::duplicateUcs2(lastand);
	char	*firstnumstr=charstring::duplicateUcs2(firstnum);

	stdoutput.printf("in the phrase: \"%s\"...\n",phrasestr);
	stdoutput.printf("  the first 'a' is	: \"%s\"\n",firstastr);
	stdoutput.printf("  the first \"and\" is	: \"%s\"\n",firstandstr);
	stdoutput.printf("  the last 'a' is	: \"%s\"\n",lastastr);
	stdoutput.printf("  the last \"and\" is	: \"%s\"\n",lastandstr);
	stdoutput.printf("  the first number is	: \"%s\"\n",firstnumstr);
	stdoutput.write('\n');

	delete[] phrasestr;
	delete[] firstastr;
	delete[] firstandstr;
	delete[] lastastr;
	delete[] lastandstr;
	delete[] firstnumstr;
}
