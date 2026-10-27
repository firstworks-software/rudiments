// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/signalclasses.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// getString and getPosition; these need no printf
static void stringBufferBasics() {

	signalmanager::alarm(60);
	stdoutput.printf("getString and getPosition...\n");

	ucs2literal(ab,"ab");
	ucs2literal(c,"c");
	ucs2literal(abc,"abc");
	ucs2literal(hello,"hello");
	ucs2literal(x,"X");
	ucs2literal(axc,"aXc");

	// getString removes one character, not one byte
	ucs2stringbuffer	*sb=new ucs2stringbuffer();
	sb->appendUcs2(ab);
	test("getString ab",!ucs2charstring::compare(sb->getString(),ab));
	test("getSize ab",sb->getSize()==2*sizeof(ucs2_t));
	test("getPosition ab",sb->getPosition()==2);
	sb->appendUcs2(c);
	test("getString abc",!ucs2charstring::compare(sb->getString(),abc));
	test("getSize abc",sb->getSize()==3*sizeof(ucs2_t));
	test("getStringLength abc",sb->getStringLength()==3);

	// getString, append and getStringLength repeated
	sb->clear();
	ucs2_t	expected[201];
	bool	ok=true;
	for (size_t i=0; i<200; i++) {
		ucs2_t	ch=(ucs2_t)('a'+(i%26));
		sb->appendUcs2(ch);
		expected[i]=ch;
		expected[i+1]=0;
		if (sb->getStringLength()!=i+1 ||
			ucs2charstring::compare(sb->getString(),expected) ||
			sb->getSize()!=(i+1)*sizeof(ucs2_t) ||
			sb->getPosition()!=(off64_t)(i+1)) {
			ok=false;
		}
	}
	test("repeated getString and append",ok);

	// no append after getString
	sb->clear();
	sb->appendUcs2(hello);
	sb->getString();
	sb->getString();
	test("getString twice",
		!ucs2charstring::compare(sb->getString(),hello));
	test("getSize twice",sb->getSize()==5*sizeof(ucs2_t));
	delete sb;

	// positions count characters
	sb=new ucs2stringbuffer();
	sb->appendUcs2(abc);
	test("position 3",sb->getPosition()==3);
	sb->setPositionRelativeToBeginning(1);
	test("position begin 1",sb->getPosition()==1);
	sb->setPositionRelativeToCurrent(1);
	test("position current 1",sb->getPosition()==2);
	sb->setPositionRelativeToEnd(-1);
	test("position end -1",sb->getPosition()==2);
	sb->setPositionRelativeToEnd(0);
	test("position end 0",sb->getPosition()==3);
	sb->setPositionRelativeToBeginning(1);
	sb->writeUcs2(x);
	test("position write",sb->getPosition()==2);
	test("write contents",!ucs2charstring::compare(sb->getString(),axc));
	delete sb;

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

int main(int argc, const char **argv) {

	header("ucs2stringbuffer");

	stringBufferBasics();

	if (!ucs2charstring::supportsPrintf()) {
		stdoutput.printf("	not supported\n\n");
		return 0;
	}

	ucs2literal(alphabet,"abcdefghijklmnopqrstuvwxyz");
	const uint32_t	iterations=5;
	uint32_t	members;
	size_t		strl;
	if (sys::getPhysicalMemorySize()>1024*1024*1024) {
		// for systems with more than 1G of memory...
		members=1024;
		strl=300000;
	} else {
		// for systems with less than 1G of memory...
		members=64;
		strl=20000;
	}
	ucs2_t	*str=new ucs2_t[strl];
	ucs2_t	buf[64];

	// create a new string buffer
	ucs2stringbuffer	*strb=new ucs2stringbuffer();

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,strl*sizeof(ucs2_t));

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->appendUcs2((const ucs2_t *)NULL);

			// empty string
			ucs2literal(empty,"");
			strb->appendUcs2(empty);

			// strings
			for (uint16_t k=1; k<=5; k++) {
				strb->appendUcs2(alphabet,k);
				ucs2charstring::append(str,alphabet,k);
			}

			// integers
			for (uint64_t k=1; k<10000; k*=10) {
				strb->appendUcs2((uint16_t)k);
				ucs2literal(format,"%hd");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(ucs2_t),
						format,(uint16_t)k);
				ucs2charstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000; k*=10) {
				strb->appendUcs2((uint32_t)k);
				ucs2literal(format,"%ld");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(ucs2_t),
						format,(uint32_t)k);
				ucs2charstring::append(str,buf);
			}
			#ifdef RUDIMENTS_HAVE_LONG_LONG
			for (uint64_t k=1; k<1000000000000000000ULL; k*=10) {
				strb->appendUcs2((uint64_t)k);
				ucs2literal(format,"%lld");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(ucs2_t),
						format,(uint64_t)k);
				ucs2charstring::append(str,buf);
			}
			#endif

			// float
			float	mult=10;
			for (uint16_t k=1; k<5; k++) {
				float	l=k+((float)k/mult);
				mult*=10;
				strb->append(l,k+1,(uint16_t)l);
				ucs2literal(format,"%*.*f");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(ucs2_t),
						format,k+1,k,l);
				ucs2charstring::append(str,buf);
			}

			// double
			mult=10;
			for (uint16_t k=1; k<5; k++) {
				double	l=k+((double)k/mult);
				mult*=10;
				strb->append(l,k+1,k);
				ucs2literal(format,"%*.*f");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(ucs2_t),
						format,k+1,k,l);
				ucs2charstring::append(str,buf);
			}
		}

		test("length",ucs2charstring::getLength(str)==
					strb->getStringLength());
		test("contents",!ucs2charstring::compare(
					str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	for (uint16_t i=0; i<iterations; i++) {
		for (uint32_t j=0;
			j<(strl/sizeof(ucs2_t))-5-1; j=j+10+i) {
			strb->setPositionRelativeToBeginning(j);
			ucs2literal(sixes,"66666");
			strb->writeUcs2(sixes);
			ucs2charstring::copy(str+j,sixes,5);
		}
		test("contents",
			!ucs2charstring::compare(str,strb->getString()));
	}
	delete strb;
	stdoutput.printf("\n");


	// initial contents...
	stdoutput.printf("initial contents...\n");
	ucs2literal(numbersletters,"0123456789abcdef");
	for (uint16_t i=0; i<iterations; i++) {

		bytestring::zero(str,strl*sizeof(ucs2_t));

		ucs2_t	*data=new ucs2_t[1024*(i+1)+1];
		ucs2charstring::zero(data,1024*(i+1)+1);
		for (uint16_t j=0; j<1024*(i+1)/16; j++) {
			ucs2charstring::append(data,numbersletters);
			ucs2charstring::append(str,numbersletters);
		}

		strb=new ucs2stringbuffer(data,
					ucs2charstring::getLength(data));
		for (uint16_t j=0; j<=(i+1)*100; j++) {
			strb->appendUcs2(numbersletters);
			ucs2charstring::append(str,numbersletters);
		}

		test("contents",
			!ucs2charstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");

	return 0;
}
