// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/prng.h>
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
		if (numbers[i]<bottom || numbers[i]>top) {
			success=false;
		}
	}
	test(title,success);
}

int main(int argc, const char **argv) {

	header("prng");

	// get the seed
        uint32_t	seed=prng::getSeed();
        stdoutput.printf("	Seed: %d\n\n",seed);

	prng	r;
	int32_t	*numbers=new int32_t[numbercount];

	for (uint16_t i=0; i<setcount; i++) {

		// generate range
		seed=prng::generate(seed);
		int32_t	bottom=prng::scale(seed,lowerrange,upperrange);
		seed=prng::generate(seed);
		int32_t	top=prng::scale(seed,lowerrange,upperrange);
		if (bottom>top) {
			int32_t	temp=top;
			top=bottom;
			bottom=temp;
		}

        	stdoutput.printf("	%d numbers between %d and %d:\n",
							numbercount,bottom,top);

		// static methods
		for (uint16_t j=0; j<numbercount; j++) {
			numbers[j]=prng::generate(seed,bottom,top);
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

	stdoutput.printf("\n	static generateBytes...\n\n");

	uint32_t	byteseed=prng::getSeed();

	// static generateBytes(seed,size) - allocates and fills a new
	// buffer.  On backends where setSeed() actually seeds the
	// generator, this is reproducible for the same seed.  (On
	// backends like arc4random, setSeed() is a no-op by design - see
	// prng::setSeed() - so reproducibility can't be assumed
	// there.)
	byte_t	*sbytes1=prng::generateBytes(byteseed,bytesize);
	test("static generateBytes(seed,size) - non-NULL",sbytes1!=NULL);
	byte_t	*sbytes2=prng::generateBytes(byteseed,bytesize);
	test("static generateBytes(seed,size) - non-NULL again",
							sbytes2!=NULL);
	#ifndef RUDIMENTS_HAVE_ARC4RANDOM
		test("static generateBytes(seed,size) - "
				"same seed produces same bytes",
				!bytestring::compare(sbytes1,sbytes2,
								bytesize));
	#endif
	delete[] sbytes2;

	// a fresh seed from getSeed() should very likely produce different
	// bytes than the previous seed did
	uint32_t	byteseed2=prng::getSeed();
	byte_t	*sbytes3=prng::generateBytes(byteseed2,bytesize);
	test("static generateBytes(seed,size) - non-NULL for new seed",
							sbytes3!=NULL);
	test("static generateBytes(seed,size) - "
			"different seeds produce different bytes",
			bytestring::compare(sbytes1,sbytes3,bytesize));
	delete[] sbytes1;
	delete[] sbytes3;

	// static generateBytes(seed,byte_t*,buffersize,size) with
	// size<=buffersize - only the first "size" bytes should be touched,
	// and the same seed should reproduce the same bytes
	byte_t	*sbuffer=new byte_t[buffersize];
	bytestring::set(sbuffer,0xFF,buffersize);
	bool	sfilledok=prng::generateBytes(byteseed,sbuffer,
							buffersize,bytesize);
	test("static generateBytes(seed,buffer,buffersize,size) - success",
							sfilledok);
	bool	suntouched=true;
	for (size_t i=bytesize; i<buffersize; i++) {
		if (sbuffer[i]!=0xFF) {
			suntouched=false;
			break;
		}
	}
	test("static generateBytes(seed,buffer,buffersize,size) - "
			"bytes past size untouched",suntouched);
	#ifndef RUDIMENTS_HAVE_ARC4RANDOM
		byte_t	*sbuffer2=new byte_t[buffersize];
		bytestring::set(sbuffer2,0xFF,buffersize);
		prng::generateBytes(byteseed,sbuffer2,
							buffersize,bytesize);
		test("static generateBytes(seed,buffer,buffersize,size) - "
				"same seed produces same bytes",
				!bytestring::compare(sbuffer,sbuffer2,
								bytesize));
		delete[] sbuffer2;
	#endif
	delete[] sbuffer;

	// static generateBytes(seed,byte_t*,buffersize,size) with
	// size>buffersize - fail
	byte_t	stoosmall[bytesize];
	test("static generateBytes(seed,buffer,buffersize,size) - "
			"size>buffersize fails",
			!prng::generateBytes(byteseed,stoosmall,
							bytesize,buffersize));

	// static generateBytes(seed,byte_t*,buffersize) - fills the entire
	// buffer
	byte_t	*swholebuffer=new byte_t[buffersize];
	bytestring::set(swholebuffer,0xFF,buffersize);
	bool	swholeok=prng::generateBytes(byteseed,swholebuffer,
								buffersize);
	test("static generateBytes(seed,buffer,buffersize) - success",
								swholeok);
	bool	schanged=false;
	for (size_t i=0; i<buffersize; i++) {
		if (swholebuffer[i]!=0xFF) {
			schanged=true;
			break;
		}
	}
	test("static generateBytes(seed,buffer,buffersize) - "
			"buffer was written",schanged);
	delete[] swholebuffer;

	// static generateBytes(seed,bytebuffer*,size) - appends "size"
	// bytes
	bytebuffer	sbb;
	size_t	ssizebefore=sbb.getSize();
	bool	sappendok=prng::generateBytes(byteseed,&sbb,bytesize);
	test("static generateBytes(seed,bytebuffer,size) - success",
								sappendok);
	test("static generateBytes(seed,bytebuffer,size) - grew by size",
			sbb.getSize()==ssizebefore+bytesize);

	return 0;
}
