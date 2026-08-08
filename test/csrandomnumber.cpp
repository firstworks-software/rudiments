// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/csrandomnumber.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

const uint16_t	numbercount=1000;
const int32_t	lowerrange=-65536;
const int32_t	upperrange=65536;

void testRange(const char *title, int32_t *numbers, uint16_t count,
						int32_t bottom, int32_t top) {
	bool	success=true;
	for (uint16_t i=0; i<count && success; i++) {
		if (numbers[i]<bottom || numbers[i]>top) {
			success=false;
		}
	}
	test(title,success);
}

int main(int argc, const char **argv) {

	header("csrandomnumber");

	test("isSupported",csrandomnumber::isSupported());

	csrandomnumber	c;

	// generate(uint32_t *) - success and differs between calls
	uint32_t	u1=0;
	uint32_t	u2=0;
	test("generate(uint32_t*) - success",c.generate(&u1) &&
							c.generate(&u2));
	test("generate(uint32_t*) - differs between calls",u1!=u2);

	// generate(int32_t*,lower,upper) - stays in range
	int32_t	*numbers=new int32_t[numbercount];
	stdoutput.printf("	%d numbers between %d and %d:\n",
					numbercount,lowerrange,upperrange);
	bool	success=true;
	for (uint16_t j=0; j<numbercount && success; j++) {
		numbers[j]=0;
		if (!c.generate(&(numbers[j]),lowerrange,upperrange)) {
			success=false;
		}
	}
	test("generate(int32_t*,lower,upper) - success",success);
	testRange("generate(int32_t*,lower,upper) - in range",
				numbers,numbercount,lowerrange,upperrange);
	delete[] numbers;

	// getRandMax() - always the full uint32_t range
	test("getRandMax()",csrandomnumber::getRandMax()==4294967295U);

	// scale() - stays in range, including the range-endpoint overshoot
	// clamp: scale(getRandMax(),lower,upper) should never exceed upper
	stdoutput.printf("\n	%d scaled numbers between %d and %d:\n",
					numbercount,lowerrange,upperrange);
	int32_t	*scaled=new int32_t[numbercount];
	for (uint16_t j=0; j<numbercount; j++) {
		scaled[j]=csrandomnumber::scale(
				(uint32_t)(j*(csrandomnumber::getRandMax()/
							numbercount)),
				lowerrange,upperrange);
	}
	testRange("scale() - in range",scaled,numbercount,
						lowerrange,upperrange);
	delete[] scaled;
	int32_t	overshoot=csrandomnumber::scale(4294967295U,
						lowerrange,upperrange);
	test("scale() - overshoot clamped to upper",overshoot<=upperrange);

	// static generate() - not all identical
	uint32_t	g1=csrandomnumber::generate();
	uint32_t	g2=csrandomnumber::generate();
	uint32_t	g3=csrandomnumber::generate();
	test("static generate() - not all identical",
				!(g1==g2 && g2==g3));

	// static generate(lower,upper) - stays in range
	int32_t	*sgnumbers=new int32_t[numbercount];
	stdoutput.printf("\n	%d static-generated numbers between "
					"%d and %d:\n",
					numbercount,lowerrange,upperrange);
	for (uint16_t j=0; j<numbercount; j++) {
		sgnumbers[j]=csrandomnumber::generate(lowerrange,upperrange);
	}
	testRange("static generate(lower,upper) - in range",
				sgnumbers,numbercount,lowerrange,upperrange);
	delete[] sgnumbers;

	stdoutput.printf("\n	generateBytes...\n\n");

	const size_t	bytesize=32;
	const size_t	buffersize=64;

	// generateBytes(size_t) - allocates and fills a new buffer
	byte_t	*bytes1=c.generateBytes(bytesize);
	test("generateBytes(size) - non-NULL",bytes1!=NULL);
	byte_t	*bytes2=c.generateBytes(bytesize);
	test("generateBytes(size) - non-NULL again",bytes2!=NULL);
	test("generateBytes(size) - differs between calls",
			bytestring::compare(bytes1,bytes2,bytesize));
	delete[] bytes1;
	delete[] bytes2;

	// generateBytes(byte_t *,buffersize,size) with size<=buffersize -
	// only the first "size" bytes should be touched
	byte_t	*buffer=new byte_t[buffersize];
	bytestring::set(buffer,0xFF,buffersize);
	bool	filledok=c.generateBytes(buffer,buffersize,bytesize);
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
			!c.generateBytes(toosmall,bytesize,buffersize));

	// generateBytes(byte_t *,buffersize) - fills the entire buffer
	byte_t	*wholebuffer=new byte_t[buffersize];
	bytestring::set(wholebuffer,0xFF,buffersize);
	bool	wholeok=c.generateBytes(wholebuffer,buffersize);
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
	bool	appendok=c.generateBytes(&bb,bytesize);
	test("generateBytes(bytebuffer,size) - success",appendok);
	test("generateBytes(bytebuffer,size) - grew by size",
			bb.getSize()==sizebefore+bytesize);

	return 0;
}
