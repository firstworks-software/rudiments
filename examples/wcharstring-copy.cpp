#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	wchar_t		buffer[32];


	// zero the buffer
	wcharstring::zero(buffer,32);


	// append strings, integers and floats
	wcharstring::append(buffer,L"Hello ");
	wcharstring::append(buffer,L"there!",6);
	wcharstring::append(buffer,L" ");
	wcharstring::append(buffer,(uint64_t)1);
	wcharstring::append(buffer,(uint64_t)2);
	wcharstring::append(buffer,(uint64_t)3);
	wcharstring::append(buffer,L" ");
	wcharstring::append(buffer,1.234,4,3);
	stdoutput.printf("buffer: %ls\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",wcharstring::getLength(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	wcharstring::zero(buffer,32);


	// copy to the beginning of the buffer
	wcharstring::copy(buffer,L"Hello!");
	stdoutput.printf("buffer: %ls\n",buffer);


	// copy the specified number of characters to the beginning of the buffer
	wcharstring::copy(buffer,L"Hello again!",6);
	stdoutput.printf("buffer: %ls\n",buffer);


	// copy to the specified offset
	wcharstring::copy(buffer,6,L"again!");
	stdoutput.printf("buffer: %ls\n",buffer);


	// copy the specified number of characters to the specified offset
	wcharstring::copy(buffer,12,L" Hi!  blah blah blah",4);
	stdoutput.printf("buffer: %ls\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",wcharstring::getLength(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	wcharstring::zero(buffer,32);


	// safely copy a long string to a smaller buffer
	wcharstring::safeCopy(buffer,32,
				L"This string is longer than the buffer");
	stdoutput.printf("buffer: %ls\n",buffer);


	// safely copy the specified number of characters of a
	// long string to a smaller buffer
	wcharstring::safeCopy(buffer,32,
				L"This string is longer than the buffer",36);
	stdoutput.printf("buffer: %ls\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",wcharstring::getLength(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	wcharstring::zero(buffer,32);


	// print formatted data to the buffer
	wcharstring::printf(buffer,32,L"%ls, %05d, %7.4f",
					L"hello",100,123.4567);
	stdoutput.printf("buffer: %ls\n",buffer);
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",wcharstring::getLength(buffer));
	stdoutput.write('\n');


	// duplicate a string
	wchar_t	*completedup=wcharstring::duplicate(L"Hello there!");
	wchar_t	*partialdup=wcharstring::duplicate(L"Hello there!",5);
	stdoutput.printf("complete duplicate of \"Hello there!\": \"%ls\"\n",
							completedup);
	stdoutput.printf(" partial duplicate of \"Hello there!\": \"%ls\"\n",
							partialdup);
	delete[] completedup;
	delete[] partialdup;
}
