#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// comparing strings...
	ucs2literal(uhello,"hello");
	ucs2literal(uHELLO,"HELLO");
	ucs2literal(uhellothere,"hello there");
	ucs2literal(uHELLOTHERE,"HELLO THERE");

	const ucs2_t * const strings[]={
		uhello,uHELLO,uhellothere,uHELLOTHERE,NULL
	};

	stdoutput.write("direct comparison...\n");
	for (const ucs2_t * const *s=strings; *s; s++) {
		char	*sstr=charstring::duplicateUcs2(*s);
		stdoutput.printf("  does \"hello\"=\"%s\"  %s?\n",sstr,
			(!ucs2charstring::compare(uhello,*s))?"yes":"no");
		delete[] sstr;
	}
	stdoutput.write('\n');

	stdoutput.write("only first 5 characters...\n");
	for (const ucs2_t * const *s=strings; *s; s++) {
		char	*sstr=charstring::duplicateUcs2(*s);
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",sstr,
			(!ucs2charstring::compare(uhello,*s,5))?"yes":"no");
		delete[] sstr;
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const ucs2_t * const *s=strings; *s; s++) {
		char	*sstr=charstring::duplicateUcs2(*s);
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",sstr,
			(!ucs2charstring::compareIgnoringCase(uhello,*s))?
							"yes":"no");
		delete[] sstr;
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case, only first 5 characters...\n");
	for (const ucs2_t * const *s=strings; *s; s++) {
		char	*sstr=charstring::duplicateUcs2(*s);
		stdoutput.printf("  does \"hello\"=\"%s\"?  %s\n",sstr,
			(!ucs2charstring::compareIgnoringCase(uhello,*s,5))?
							"yes":"no");
		delete[] sstr;
	}
	stdoutput.write('\n');



	// member of a set...
	ucs2literal(uhi,"hi");
	ucs2literal(ugoodmorning,"good morning");
	ucs2literal(ubye,"bye");
	ucs2literal(ugoodbye,"goodbye");
	ucs2literal(uBYE,"BYE");
	ucs2literal(uGOODBYE,"GOODBYE");
	ucs2literal(uHI,"HI");

	const ucs2_t * const greetings[]={
		uhello,uhi,ugoodmorning,NULL
	};
	const ucs2_t * const lowercaseexpressions[]={
		uhello,uhi,ubye,ugoodbye,NULL
	};
	const ucs2_t * const uppercaseexpressions[]={
		uHELLO,uHI,uBYE,uGOODBYE,NULL
	};

	stdoutput.write("considering case...\n");
	for (const ucs2_t * const *le=lowercaseexpressions; *le; le++) {
		char	*lestr=charstring::duplicateUcs2(*le);
		stdoutput.printf("  is \"%s\" a greeting?  %s\n",lestr,
			(ucs2charstring::isInSet(*le,greetings))?"yes":"no");
		delete[] lestr;
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const ucs2_t * const *ue=uppercaseexpressions; *ue; ue++) {
		char	*uestr=charstring::duplicateUcs2(*ue);
		stdoutput.printf("  is \"%s\" a greeting?  %s\n",uestr,
			(ucs2charstring::isInSetIgnoringCase(*ue,greetings))?
							"yes":"no");
		delete[] uestr;
	}
	stdoutput.write('\n');


	// does one string contain another?
	ucs2literal(phrase,"the quick brown fox jumped over the lazy dog");
	ucs2literal(uquick,"quick");
	ucs2literal(ubrown,"brown");
	ucs2literal(ufox,"fox");
	ucs2literal(ulazy,"lazy");
	ucs2literal(udog,"dog");
	ucs2literal(uhello2,"hello");
	ucs2literal(ugoodbye2,"goodbye");
	ucs2literal(uQUICK,"QUICK");
	ucs2literal(uBROWN,"BROWN");
	ucs2literal(uFOX,"FOX");
	ucs2literal(uLAZY,"LAZY");
	ucs2literal(uDOG,"DOG");
	ucs2literal(uHELLO2,"HELLO");
	ucs2literal(uGOODBYE2,"GOODBYE");

	const ucs2_t * const lowercasewords[]={
		uquick,ubrown,ufox,ulazy,udog,uhello2,ugoodbye2,NULL
	};
	const ucs2_t * const uppercasewords[]={
		uQUICK,uBROWN,uFOX,uLAZY,uDOG,uHELLO2,uGOODBYE2,NULL
	};

	char	*phrasestr=charstring::duplicateUcs2(phrase);

	stdoutput.write("considering case...\n");
	for (const ucs2_t * const *lw=lowercasewords; *lw; lw++) {
		char	*lwstr=charstring::duplicateUcs2(*lw);
		stdoutput.printf("  does \"%s\" contain \"%s\"?  %s\n",
			phrasestr,lwstr,
			(ucs2charstring::contains(phrase,*lw))?"yes":"no");
		delete[] lwstr;
	}
	stdoutput.write('\n');

	stdoutput.write("ignoring case...\n");
	for (const ucs2_t * const *uw=uppercasewords; *uw; uw++) {
		char	*uwstr=charstring::duplicateUcs2(*uw);
		stdoutput.printf("  does \"%s\" contain \"%s\"?  %s\n",
			phrasestr,uwstr,
			(ucs2charstring::containsIgnoringCase(phrase,*uw))?
							"yes":"no");
		delete[] uwstr;
	}
	stdoutput.write('\n');

	delete[] phrasestr;
}
