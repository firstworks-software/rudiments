// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/csprng.h>
#include <rudiments/prng.h>
#include <rudiments/rng.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void testRngInterface(const char *title, rng *r) {
	bool	success=true;
	if (!r->setSeed(1)) {
		success=false;
	}
	uint32_t	u=0;
	if (!r->generate(&u)) {
		success=false;
	}
	int32_t	i=0;
	if (!r->generate(&i,-100,100) || i<-100 || i>100) {
		success=false;
	}
	byte_t	*bytes=r->generateBytes((size_t)16);
	if (!bytes) {
		success=false;
	}
	delete[] bytes;
	byte_t	buffer[16];
	if (!r->generateBytes(buffer,sizeof(buffer),sizeof(buffer))) {
		success=false;
	}
	if (!r->generateBytes(buffer,sizeof(buffer))) {
		success=false;
	}
	bytebuffer	bb;
	if (!r->generateBytes(&bb,16)) {
		success=false;
	}
	test(title,success);
}

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

	header("csprng");

	// exercise both implementations through the rng interface
	prng	p;
	csprng	cs;
	testRngInterface("rng interface - prng",&p);
	testRngInterface("rng interface - csprng",&cs);

	test("isSupported",csprng::isSupported());

	// setSeed()/getSeed() - degenerate, but present for rng interface
	// parity.  setSeed() always succeeds and is ignored: two instances
	// seeded identically still produce different output.
	csprng	c;
	csprng	c2;
	test("setSeed() - returns true",c.setSeed(12345) && c2.setSeed(12345));
	uint32_t	seeded1=0;
	uint32_t	seeded2=0;
	c.generate(&seeded1);
	c2.generate(&seeded2);
	test("setSeed() - ignored, output still differs",seeded1!=seeded2);
	test("getSeed() - returns a placeholder",
				csprng::getSeed()==csprng::getSeed());

	// getNeedsMutex()/setMutex() - degenerate, csprng's backends hold
	// no shared mutable state
	test("getNeedsMutex() - false",!csprng::getNeedsMutex());
	csprng::setMutex(NULL);

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
	test("getRandMax()",csprng::getRandMax()==4294967295U);

	// scale() - stays in range, including the range-endpoint overshoot
	// clamp: scale(getRandMax(),lower,upper) should never exceed upper
	stdoutput.printf("\n	%d scaled numbers between %d and %d:\n",
					numbercount,lowerrange,upperrange);
	int32_t	*scaled=new int32_t[numbercount];
	for (uint16_t j=0; j<numbercount; j++) {
		scaled[j]=csprng::scale(
				(uint32_t)(j*(csprng::getRandMax()/
							numbercount)),
				lowerrange,upperrange);
	}
	testRange("scale() - in range",scaled,numbercount,
						lowerrange,upperrange);
	delete[] scaled;
	int32_t	overshoot=csprng::scale(4294967295U,
						lowerrange,upperrange);
	test("scale() - overshoot clamped to upper",overshoot<=upperrange);

	// static generate() - not all identical
	uint32_t	g1=csprng::generate();
	uint32_t	g2=csprng::generate();
	uint32_t	g3=csprng::generate();
	test("static generate() - not all identical",
				!(g1==g2 && g2==g3));

	// static generate(lower,upper) - stays in range
	int32_t	*sgnumbers=new int32_t[numbercount];
	stdoutput.printf("\n	%d static-generated numbers between "
					"%d and %d:\n",
					numbercount,lowerrange,upperrange);
	for (uint16_t j=0; j<numbercount; j++) {
		sgnumbers[j]=csprng::generate(lowerrange,upperrange);
	}
	testRange("static generate(lower,upper) - in range",
				sgnumbers,numbercount,lowerrange,upperrange);
	delete[] sgnumbers;

	// static generate(seed) / generate(seed,lower,upper) - the seed is
	// ignored, but the calls succeed and stay in range like their
	// unseeded counterparts
	uint32_t	sg1=csprng::generate((uint32_t)42);
	uint32_t	sg2=csprng::generate((uint32_t)42);
	test("static generate(seed) - ignores seed, differs between calls",
				sg1!=sg2);
	int32_t	*seedednumbers=new int32_t[numbercount];
	for (uint16_t j=0; j<numbercount; j++) {
		seedednumbers[j]=csprng::generate((uint32_t)42,
						lowerrange,upperrange);
	}
	testRange("static generate(seed,lower,upper) - in range",
				seedednumbers,numbercount,
				lowerrange,upperrange);
	delete[] seedednumbers;

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

	// static generateBytes(seed,...) overloads - the seed is ignored,
	// but each still draws real cryptographically secure bytes
	byte_t	*sbytes1=csprng::generateBytes((uint32_t)42,bytesize);
	byte_t	*sbytes2=csprng::generateBytes((uint32_t)42,bytesize);
	test("static generateBytes(seed,size) - non-NULL",
				sbytes1!=NULL && sbytes2!=NULL);
	test("static generateBytes(seed,size) - ignores seed, "
			"differs between calls",
			bytestring::compare(sbytes1,sbytes2,bytesize));
	delete[] sbytes1;
	delete[] sbytes2;

	byte_t	*sbuffer=new byte_t[buffersize];
	bool	sfilledok=csprng::generateBytes((uint32_t)42,
					sbuffer,buffersize,bytesize);
	test("static generateBytes(seed,buffer,buffersize,size) - success",
				sfilledok);
	delete[] sbuffer;

	byte_t	*swholebuffer=new byte_t[buffersize];
	bool	swholeok=csprng::generateBytes((uint32_t)42,
					swholebuffer,buffersize);
	test("static generateBytes(seed,buffer,buffersize) - success",
				swholeok);
	delete[] swholebuffer;

	bytebuffer	sbb;
	size_t	sbbsizebefore=sbb.getSize();
	bool	sappendok=csprng::generateBytes((uint32_t)42,&sbb,bytesize);
	test("static generateBytes(seed,bytebuffer,size) - success",
				sappendok);
	test("static generateBytes(seed,bytebuffer,size) - grew by size",
				sbb.getSize()==sbbsizebefore+bytesize);

	return 0;
}
