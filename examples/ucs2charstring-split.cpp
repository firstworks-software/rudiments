#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	ucs2literal(str,"All along the untrodden paths of the future...");

	// split...
	ucs2_t		**parts;
	uint64_t	partcount;
	ucs2literal(space," ");
	ucs2charstring::split(str,space,true,&parts,&partcount);

	char	*strstr=charstring::duplicateUcs2(str);
	stdoutput.printf("original string:\n  %s\n",strstr);
	delete[] strstr;

	stdoutput.printf("split on space:\n");
	for (uint64_t i=0; i<partcount; i++) {
		char	*partstr=charstring::duplicateUcs2(parts[i]);
		stdoutput.printf("  %s\n",partstr);
		delete[] partstr;
	}
	stdoutput.write('\n');

	for (uint64_t i=0; i<partcount; i++) {
		delete[] parts[i];
	}
	delete[] parts;



	// substring...
	ucs2_t	*substring1=ucs2charstring::getSubString(str,14);
	ucs2_t	*substring2=ucs2charstring::getSubString(str,14,28);

	char	*ss1str=charstring::duplicateUcs2(substring1);
	char	*ss2str=charstring::duplicateUcs2(substring2);
	stdoutput.printf("string starting at index 14: %s\n",ss1str);
	stdoutput.printf("string from index 14 to 21 : %s\n",ss2str);
	stdoutput.write('\n');

	delete[] ss1str;
	delete[] ss2str;
	delete[] substring1;
	delete[] substring2;



	// insert string...
	ucs2literal(insertstr,", I can see the footprints of an unseen hand");
	ucs2_t	*newstr=ucs2charstring::insertString(str,insertstr,43);

	char	*newstrstr=charstring::duplicateUcs2(newstr);
	stdoutput.printf("string after insert:\n  %s\n",newstrstr);
	stdoutput.write('\n');

	delete[] newstrstr;
	delete[] newstr;
}
