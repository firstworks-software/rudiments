#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	ucs2_t		buffer[32];


	// zero the buffer
	ucs2charstring::zero(buffer,32);


	// append strings, integers and floats
	ucs2literal(hello,"Hello ");
	ucs2literal(there,"there!");
	ucs2literal(space," ");
	ucs2charstring::append(buffer,hello);
	ucs2charstring::append(buffer,there,6);
	ucs2charstring::append(buffer,space);
	ucs2charstring::append(buffer,(uint64_t)1);
	ucs2charstring::append(buffer,(uint64_t)2);
	ucs2charstring::append(buffer,(uint64_t)3);
	ucs2charstring::append(buffer,space);
	ucs2charstring::append(buffer,1.234,4,3);

	char	*bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",
			ucs2charstring::getLength(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	ucs2charstring::zero(buffer,32);


	// copy to the beginning of the buffer
	ucs2literal(hello2,"Hello!");
	ucs2charstring::copy(buffer,hello2);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;


	// copy the specified number of characters to the beginning
	ucs2literal(helloagain,"Hello again!");
	ucs2charstring::copy(buffer,helloagain,6);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;


	// copy to the specified offset
	ucs2literal(again,"again!");
	ucs2charstring::copy(buffer,6,again);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;


	// copy the specified number of characters to the specified offset
	ucs2literal(hiblah," Hi!  blah blah blah");
	ucs2charstring::copy(buffer,12,hiblah,4);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",
			ucs2charstring::getLength(buffer));
	stdoutput.write('\n');


	// zero the buffer again
	ucs2charstring::zero(buffer,32);


	// safely copy a long string to a smaller buffer
	ucs2literal(longstr,"This string is longer than the buffer");
	ucs2charstring::safeCopy(buffer,32,longstr);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;


	// safely copy the specified number of characters
	ucs2charstring::safeCopy(buffer,32,longstr,36);
	bufstr=charstring::duplicateUcs2(buffer);
	stdoutput.printf("buffer: %s\n",bufstr);
	delete[] bufstr;
	stdoutput.write('\n');


	// get length
	stdoutput.printf("length(buffer)=%d\n",
			ucs2charstring::getLength(buffer));
	stdoutput.write('\n');


	// duplicate a string
	ucs2literal(hellothere,"Hello there!");
	ucs2_t	*completedup=ucs2charstring::duplicate(hellothere);
	ucs2_t	*partialdup=ucs2charstring::duplicate(hellothere,5);

	char	*cdstr=charstring::duplicateUcs2(completedup);
	char	*pdstr=charstring::duplicateUcs2(partialdup);
	stdoutput.printf("complete duplicate of \"Hello there!\": \"%s\"\n",
							cdstr);
	stdoutput.printf(" partial duplicate of \"Hello there!\": \"%s\"\n",
							pdstr);
	delete[] cdstr;
	delete[] pdstr;
	delete[] completedup;
	delete[] partialdup;
}
