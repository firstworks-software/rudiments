#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const wchar_t	str[]=L"All along the untrodden paths of the future...";

	// split...
	wchar_t		**parts;
	uint64_t	partcount;
	wcharstring::split(str,L" ",true,&parts,&partcount);

	stdoutput.printf("original string:\n  %ls\n",str);
	stdoutput.printf("split on space:\n");
	for (uint64_t i=0; i<partcount; i++) {
		stdoutput.printf("  %ls\n",parts[i]);
	}
	stdoutput.write('\n');

	for (uint64_t i=0; i<partcount; i++) {
		delete[] parts[i];
	}
	delete[] parts;



	// substring...
	wchar_t	*substring1=wcharstring::getSubString(str,14);
	wchar_t	*substring2=wcharstring::getSubString(str,14,28);

	stdoutput.printf("string starting at index 14: %ls\n",substring1);
	stdoutput.printf("string from index 14 to 21 : %ls\n",substring2);
	stdoutput.write('\n');

	delete[] substring1;
	delete[] substring2;



	// insert string...
	wchar_t	*newstr=wcharstring::insertString(str,
		L", I can see the footprints of an unseen hand",43);

	stdoutput.printf("string after insert:\n  %ls\n",newstr);
	stdoutput.write('\n');

	delete[] newstr;
}
