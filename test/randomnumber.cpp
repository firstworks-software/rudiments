// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const uint16_t	setcount=10;
const uint16_t	numbercount=1000;
const int32_t	lowerrange=-65536;
const int32_t	upperrange=65536;

void testRange(const char *title, int32_t *numbers, uint16_t count,
						int32_t bottom, int32_t top) {
	bool	success=true;
	for (uint16_t i=0; i<count && success; i++) {
		if (numbers[i]<bottom && numbers[i]>top) {
			success=false;
		}
	}
	test(title,success);
}

int main(int argc, const char **argv) {

	header("randomnumber");

	// get the seed
        uint32_t	seed=randomnumber::getSeed();
        stdoutput.printf("	Seed: %d\n\n",seed);

	randomnumber	r;
	int32_t	*numbers=new int32_t[numbercount];

	for (uint16_t i=0; i<setcount; i++) {

		// generate range
		seed=randomnumber::generate(seed);
		int32_t	bottom=randomnumber::scale(seed,lowerrange,upperrange);
		seed=randomnumber::generate(seed);
		int32_t	top=randomnumber::scale(seed,lowerrange,upperrange);
		if (bottom>top) {
			int32_t	temp=top;
			top=bottom;
			bottom=temp;
		}

        	stdoutput.printf("	%d numbers between %d and %d:\n",
							numbercount,bottom,top);

		// static methods
		for (uint16_t j=0; j<numbercount; j++) {
			numbers[j]=randomnumber::generate(seed,bottom,top);
			seed=numbers[j];
		}
		testRange("static methods - in range",
					numbers,numbercount,bottom,top);

		// instance methods
		r.setSeed(seed);
		bool	success=true;
		for (uint16_t j=0; j<numbercount && success; j++) {
			numbers[j]=0;
        		if (!r.generate(&(numbers[j]),bottom,top)) {
				success=false;
			}
		}
		test("instance methods - success",success);
		testRange("instance methods - in range",
					numbers,numbercount,bottom,top);
		stdoutput.printf("\n");
	}

	delete[] numbers;

	stdoutput.printf("	generateBytes...\n\n");

	const size_t	bytesize=32;
	const size_t	buffersize=64;

	// generateBytes(size_t) - allocates and fills a new buffer
	byte_t	*bytes1=r.generateBytes(bytesize);
	test("generateBytes(size) - non-NULL",bytes1!=NULL);
	byte_t	*bytes2=r.generateBytes(bytesize);
	test("generateBytes(size) - non-NULL again",bytes2!=NULL);
	test("generateBytes(size) - differs between calls",
			bytestring::compare(bytes1,bytes2,bytesize));
	delete[] bytes1;
	delete[] bytes2;

	// generateBytes(byte_t *,buffersize,size) with size<=buffersize -
	// only the first "size" bytes should be touched
	byte_t	*buffer=new byte_t[buffersize];
	bytestring::set(buffer,0xFF,buffersize);
	bool	filledok=r.generateBytes(buffer,buffersize,bytesize);
	test("generateBytes(buffer,buffersize,size) - success",filledok);
	bool	untouched=true;
	for (size_t i=bytesize; i<buffersize; i++) {
		if (buffer[i]!=0xFF) {
			untouched=false;
			break;
		}
	}
	test("generateBytes(buffer,buffersize,size) - "
			"bytes past size untouched",untouched);
	delete[] buffer;

	// generateBytes(byte_t *,buffersize,size) with size>buffersize - fail
	byte_t	toosmall[bytesize];
	test("generateBytes(buffer,buffersize,size) - "
			"size>buffersize fails",
			!r.generateBytes(toosmall,bytesize,buffersize));

	// generateBytes(byte_t *,buffersize) - fills the entire buffer
	byte_t	*wholebuffer=new byte_t[buffersize];
	bytestring::set(wholebuffer,0xFF,buffersize);
	bool	wholeok=r.generateBytes(wholebuffer,buffersize);
	test("generateBytes(buffer,buffersize) - success",wholeok);
	bool	changed=false;
	for (size_t i=0; i<buffersize; i++) {
		if (wholebuffer[i]!=0xFF) {
			changed=true;
			break;
		}
	}
	test("generateBytes(buffer,buffersize) - buffer was written",changed);
	delete[] wholebuffer;

	// generateBytes(bytebuffer *,size) - appends "size" bytes
	bytebuffer	bb;
	size_t	sizebefore=bb.getSize();
	bool	appendok=r.generateBytes(&bb,bytesize);
	test("generateBytes(bytebuffer,size) - success",appendok);
	test("generateBytes(bytebuffer,size) - grew by size",
			bb.getSize()==sizebefore+bytesize);

	return 0;
}
