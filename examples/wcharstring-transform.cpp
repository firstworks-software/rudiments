#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	wchar_t	hellothere[]=L"   hello there!   ";

	// upper-case...
	wcharstring::upper(hellothere);
	stdoutput.printf("upper cased:   \"%ls\"\n",hellothere);

	// lower-case...
	wcharstring::lower(hellothere);
	stdoutput.printf("lower cased:   \"%ls\"\n",hellothere);

	// capitalized...
	wcharstring::capitalize(hellothere);
	stdoutput.printf("capitalized:   \"%ls\"\n",hellothere);

	// right trimmed...
	wcharstring::rightTrim(hellothere);
	stdoutput.printf("right trimmed: \"%ls\"\n",hellothere);

	// left trimmed...
	wcharstring::leftTrim(hellothere);
	stdoutput.printf("left trimmed:  \"%ls\"\n",hellothere);
	stdoutput.write('\n');



	wchar_t	paragraph[]=L"Hello there.\n This is a paragraph\n "
				L"with random\n carriage returns\n "
				L"scattered throughout.";

	// original...
	stdoutput.printf("original text:\n%ls\n\n",paragraph);

	// stripped of carriage returns...
	wcharstring::strip(paragraph,L'\n');
	stdoutput.printf("text without carriage returns:\n%ls\n\n",paragraph);

	// stripped of "Hello there."...
	wcharstring::strip(paragraph,L"Hello there. ");
	stdoutput.printf("text without \"Hello There. \":\n%ls\n\n",paragraph);

	// with replacements...
	wcharstring::replace(paragraph,L' ',L'_');
	stdoutput.printf("text with spaces replaced by underscores:\n%ls\n\n",
							paragraph);



	wchar_t	paddedtext[]=L"   hello   ";

	// original...
	stdoutput.printf("original text:   \"%ls\"\n",paddedtext);

	// left-justified...
	wcharstring::leftJustify(paddedtext,wcharstring::getLength(paddedtext));
	stdoutput.printf("left-justified:  \"%ls\"\n",paddedtext);

	// right-justified...
	wcharstring::rightJustify(paddedtext,wcharstring::getLength(paddedtext));
	stdoutput.printf("right-justified: \"%ls\"\n",paddedtext);

	// centered...
	wcharstring::center(paddedtext,wcharstring::getLength(paddedtext));
	stdoutput.printf("centered:        \"%ls\"\n",paddedtext);
	stdoutput.write('\n');



	const wchar_t	unpaddedtext[]=L"hellothere";

	// original...
	stdoutput.printf("original text:      \"%ls\"\n",unpaddedtext);

	// left-padded
	wchar_t	*leftpadded=wcharstring::pad(unpaddedtext,L' ',-1,15);
	stdoutput.printf("left padded text:   \"%ls\"\n",leftpadded);
	delete[] leftpadded;

	// right-padded
	wchar_t	*rightpadded=wcharstring::pad(unpaddedtext,L' ',1,15);
	stdoutput.printf("right padded text:  \"%ls\"\n",rightpadded);
	delete[] rightpadded;

	// center-padded
	wchar_t	*centerpadded=wcharstring::pad(unpaddedtext,L' ',0,15);
	stdoutput.printf("center padded text: \"%ls\"\n",centerpadded);
	delete[] centerpadded;
}
