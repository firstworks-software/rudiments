#include <rudiments/wstringbuffer.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	wstringbuffer	wstrb;

	// append strings, numbers, and characters...
	wstrb.append(L"hello, ")->append(L"goodbye, ");
	wstrb.append((uint64_t)1)->append(L", ");
	wstrb.append((double)1.234,4,3)->append(L'.');

	stdoutput.write("after initial append:\n");
	stdoutput.printf("  contents: \"%ls\"\n",wstrb.getString());
	stdoutput.printf("    length: %d\n",wstrb.getStringLength());
	stdoutput.write('\n');


	// clear the buffer and append more data to it
	wstrb.clear();
	wstrb.append(L"1 2 3 4 5 6 7 8 9 0");

	// overwrite the first 5 numbers...
	wstrb.setPositionRelativeToBeginning(0);
	wstrb.write(L"0 9 8 7 6");

	stdoutput.write("after overwrite:\n");
	stdoutput.printf("  contents: \"%ls\"\n",wstrb.getString());
	stdoutput.printf("    length: %d\n",wstrb.getStringLength());
	stdoutput.write('\n');


	// truncate the string after the first 2 numbers
	wstrb.truncate(3);

	stdoutput.write("after truncate:\n");
	stdoutput.printf("  contents: \"%ls\"\n",wstrb.getString());
	stdoutput.printf("    length: %d\n",wstrb.getStringLength());
	stdoutput.write('\n');


	// detach the string
	wchar_t	*str=wstrb.detachString();

	stdoutput.write("after detach:\n");
	stdoutput.printf("         contents: \"%ls\"\n",wstrb.getString());
	stdoutput.printf("           length: %d\n",wstrb.getStringLength());
	stdoutput.printf("  detached string: \"%ls\"\n",str);
	stdoutput.write('\n');

	delete[] str;
}
