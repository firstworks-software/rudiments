#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	ucs2literal(htsrc,"   hello there!   ");
	ucs2_t	hellothere[19];
	ucs2charstring::copy(hellothere,htsrc);

	char	*str;

	// upper-case...
	ucs2charstring::upper(hellothere);
	str=charstring::duplicateUcs2(hellothere);
	stdoutput.printf("upper cased:   \"%s\"\n",str);
	delete[] str;

	// lower-case...
	ucs2charstring::lower(hellothere);
	str=charstring::duplicateUcs2(hellothere);
	stdoutput.printf("lower cased:   \"%s\"\n",str);
	delete[] str;

	// capitalized...
	ucs2charstring::capitalize(hellothere);
	str=charstring::duplicateUcs2(hellothere);
	stdoutput.printf("capitalized:   \"%s\"\n",str);
	delete[] str;

	// right trimmed...
	ucs2charstring::rightTrim(hellothere);
	str=charstring::duplicateUcs2(hellothere);
	stdoutput.printf("right trimmed: \"%s\"\n",str);
	delete[] str;

	// left trimmed...
	ucs2charstring::leftTrim(hellothere);
	str=charstring::duplicateUcs2(hellothere);
	stdoutput.printf("left trimmed:  \"%s\"\n",str);
	delete[] str;
	stdoutput.write('\n');



	ucs2literal(parasrc,
		"Hello there.\n This is a paragraph\n "
		"with random\n carriage returns\n "
		"scattered throughout.");
	ucs2_t	paragraph[100];
	ucs2charstring::copy(paragraph,parasrc);

	// original...
	str=charstring::duplicateUcs2(paragraph);
	stdoutput.printf("original text:\n%s\n\n",str);
	delete[] str;

	// stripped of carriage returns...
	ucs2charstring::strip(paragraph,(ucs2_t)'\n');
	str=charstring::duplicateUcs2(paragraph);
	stdoutput.printf("text without carriage returns:\n%s\n\n",str);
	delete[] str;

	// stripped of "Hello there."...
	ucs2literal(htstrip,"Hello there. ");
	ucs2charstring::strip(paragraph,htstrip);
	str=charstring::duplicateUcs2(paragraph);
	stdoutput.printf("text without \"Hello There. \":\n%s\n\n",str);
	delete[] str;

	// with replacements...
	ucs2charstring::replace(paragraph,(ucs2_t)' ',(ucs2_t)'_');
	str=charstring::duplicateUcs2(paragraph);
	stdoutput.printf("text with spaces replaced by underscores:\n%s\n\n",
								str);
	delete[] str;



	ucs2literal(ptsrc,"   hello   ");
	ucs2_t	paddedtext[12];
	ucs2charstring::copy(paddedtext,ptsrc);

	// original...
	str=charstring::duplicateUcs2(paddedtext);
	stdoutput.printf("original text:   \"%s\"\n",str);
	delete[] str;

	// left-justified...
	ucs2charstring::leftJustify(paddedtext,
				ucs2charstring::getLength(paddedtext));
	str=charstring::duplicateUcs2(paddedtext);
	stdoutput.printf("left-justified:  \"%s\"\n",str);
	delete[] str;

	// right-justified...
	ucs2charstring::rightJustify(paddedtext,
				ucs2charstring::getLength(paddedtext));
	str=charstring::duplicateUcs2(paddedtext);
	stdoutput.printf("right-justified: \"%s\"\n",str);
	delete[] str;

	// centered...
	ucs2charstring::center(paddedtext,
				ucs2charstring::getLength(paddedtext));
	str=charstring::duplicateUcs2(paddedtext);
	stdoutput.printf("centered:        \"%s\"\n",str);
	delete[] str;
	stdoutput.write('\n');



	ucs2literal(unpaddedtext,"hellothere");

	// original...
	str=charstring::duplicateUcs2(unpaddedtext);
	stdoutput.printf("original text:      \"%s\"\n",str);
	delete[] str;

	// left-padded
	ucs2_t	*leftpadded=ucs2charstring::pad(unpaddedtext,
						(ucs2_t)' ',-1,15);
	str=charstring::duplicateUcs2(leftpadded);
	stdoutput.printf("left padded text:   \"%s\"\n",str);
	delete[] str;
	delete[] leftpadded;

	// right-padded
	ucs2_t	*rightpadded=ucs2charstring::pad(unpaddedtext,
						(ucs2_t)' ',1,15);
	str=charstring::duplicateUcs2(rightpadded);
	stdoutput.printf("right padded text:  \"%s\"\n",str);
	delete[] str;
	delete[] rightpadded;

	// center-padded
	ucs2_t	*centerpadded=ucs2charstring::pad(unpaddedtext,
						(ucs2_t)' ',0,15);
	str=charstring::duplicateUcs2(centerpadded);
	stdoutput.printf("center padded text: \"%s\"\n",str);
	delete[] str;
	delete[] centerpadded;
}
