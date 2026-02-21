#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	ucs2stringbuffer	ustrb;

	// append strings, numbers, and characters...
	ucs2literal(hello,"hello, ");
	ucs2literal(goodbye,"goodbye, ");
	ucs2literal(comma,", ");
	ustrb.appendUcs2(hello)->appendUcs2(goodbye);
	ustrb.append((uint64_t)1)->appendUcs2(comma);
	ustrb.append((double)1.234,4,3)->appendUcs2((ucs2_t)'.');

	char	*str=charstring::duplicateUcs2(ustrb.getString());
	stdoutput.write("after initial append:\n");
	stdoutput.printf("  contents: \"%s\"\n",str);
	stdoutput.printf("    length: %d\n",ustrb.getStringLength());
	stdoutput.write('\n');
	delete[] str;


	// clear the buffer and append more data to it
	ustrb.clear();
	ucs2literal(numbers,"1 2 3 4 5 6 7 8 9 0");
	ustrb.appendUcs2(numbers);

	// overwrite the first 5 numbers...
	ustrb.setPositionRelativeToBeginning(0);
	ucs2literal(revnumbers,"0 9 8 7 6");
	ustrb.writeUcs2(revnumbers);

	str=charstring::duplicateUcs2(ustrb.getString());
	stdoutput.write("after overwrite:\n");
	stdoutput.printf("  contents: \"%s\"\n",str);
	stdoutput.printf("    length: %d\n",ustrb.getStringLength());
	stdoutput.write('\n');
	delete[] str;


	// truncate the string after the first 2 numbers
	ustrb.truncate(3);

	str=charstring::duplicateUcs2(ustrb.getString());
	stdoutput.write("after truncate:\n");
	stdoutput.printf("  contents: \"%s\"\n",str);
	stdoutput.printf("    length: %d\n",ustrb.getStringLength());
	stdoutput.write('\n');
	delete[] str;


	// detach the string
	ucs2_t	*detached=ustrb.detachString();

	char	*contentsstr=charstring::duplicateUcs2(ustrb.getString());
	char	*detachedstr=charstring::duplicateUcs2(detached);
	stdoutput.write("after detach:\n");
	stdoutput.printf("         contents: \"%s\"\n",contentsstr);
	stdoutput.printf("           length: %d\n",ustrb.getStringLength());
	stdoutput.printf("  detached string: \"%s\"\n",detachedstr);
	stdoutput.write('\n');

	delete[] contentsstr;
	delete[] detachedstr;
	delete[] detached;
}
