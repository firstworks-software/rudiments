#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// comparing strings...
	const wchar_t * const strings[]={
		L"hello",L"HELLO",L"hello there",L"HELLO THERE",NULL
	};

	stdoutput.write("direct comparison...\n");
	for (const wchar_t * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%ls\"  %s?\n",*s,
			(!wcharstring::compare(L"hello",*s))?"yes":"no");

	}
	stdoutput.write('\n');

	stdoutput.write("only first 5 characters...\n");
	for (const wchar_t * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%ls\"?  %s\n",*s,
			(!wcharstring::compare(L"hello",*s,5))?"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const wchar_t * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%ls\"?  %s\n",*s,
			(!wcharstring::compareIgnoringCase(L"hello",*s))?
							"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case, only first 5 characters...\n");
	for (const wchar_t * const *s=strings; *s; s++) {
		stdoutput.printf("  does \"hello\"=\"%ls\"?  %s\n",*s,
			(!wcharstring::compareIgnoringCase(L"hello",*s,5))?
							"yes":"no");
	}
	stdoutput.write('\n');



	// member of a set...
	const wchar_t * const greetings[]={
		L"hello",L"hi",L"good morning",NULL
	};
	const wchar_t * const lowercaseexpressions[]={
		L"hello",L"hi",L"bye",L"goodbye",NULL
	};
	const wchar_t * const uppercaseexpressions[]={
		L"HELLO",L"HI",L"BYE",L"GOODBYE",NULL
	};

	stdoutput.write("considering case...\n");
	for (const wchar_t * const *le=lowercaseexpressions; *le; le++) {
		stdoutput.printf("  is \"%ls\" a greeting?  %s\n",*le,
			(wcharstring::isInSet(*le,greetings))?"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const wchar_t * const *ue=uppercaseexpressions; *ue; ue++) {
		stdoutput.printf("  is \"%ls\" a greeting?  %s\n",*ue,
			(wcharstring::isInSetIgnoringCase(*ue,greetings))?
							"yes":"no");
	}
	stdoutput.write('\n');


	// does one string contain another?
	const wchar_t	phrase[]=L"the quick brown fox jumped over the lazy dog";
	const wchar_t * const lowercasewords[]={
		L"quick",L"brown",L"fox",L"lazy",L"dog",L"hello",L"goodbye",NULL
	};
	const wchar_t * const uppercasewords[]={
		L"QUICK",L"BROWN",L"FOX",L"LAZY",L"DOG",L"HELLO",L"GOODBYE",NULL
	};

	stdoutput.write("considering case...\n");
	for (const wchar_t * const *lw=lowercasewords; *lw; lw++) {
		stdoutput.printf("  does \"%ls\" contain \"%ls\"?  %s\n",
			phrase,*lw,
			(wcharstring::contains(phrase,*lw))?"yes":"no");
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const wchar_t * const *uw=uppercasewords; *uw; uw++) {
		stdoutput.printf("  does \"%ls\" contain \"%ls\"?  %s\n",
			phrase,*uw,
			(wcharstring::containsIgnoringCase(phrase,*uw))?"yes":"no");
	}
	stdoutput.write('\n');
}
