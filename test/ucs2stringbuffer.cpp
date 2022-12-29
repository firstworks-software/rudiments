// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("ucs2stringbuffer");

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
	char16_t	*str=new char16_t[strl];
	char16_t	buf[64];

	// create a new string buffer
	ucs2stringbuffer	*strb=new ucs2stringbuffer();

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,strl*sizeof(char16_t));

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->append((const char16_t *)NULL);

			// empty string
			ucs2literal(empty,"");
			strb->append(empty);

			// strings
			for (uint16_t k=1; k<=5; k++) {
				strb->append(alphabet,k);
				ucs2charstring::append(str,alphabet,k);
			}

			// integers
			for (uint64_t k=1; k<10000; k*=10) {
				strb->append((uint16_t)k);
				ucs2literal(format,"%hd");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(char16_t),
						format,(uint16_t)k);
				ucs2charstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000; k*=10) {
				strb->append((uint32_t)k);
				ucs2literal(format,"%ld");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(char16_t),
						format,(uint32_t)k);
				ucs2charstring::append(str,buf);
			}
			#ifdef RUDIMENTS_HAVE_LONG_LONG
			for (uint64_t k=1; k<1000000000000000000ULL; k*=10) {
				strb->append((uint64_t)k);
				ucs2literal(format,"%lld");
				ucs2charstring::printf(buf,
						sizeof(buf)/sizeof(char16_t),
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
						sizeof(buf)/sizeof(char16_t),
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
						sizeof(buf)/sizeof(char16_t),
						format,k+1,k,l);
				ucs2charstring::append(str,buf);
			}
		}

		test("length",ucs2charstring::length(str)==
					strb->getStringLength());
		test("contents",!ucs2charstring::compare(
					str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	for (uint16_t i=0; i<iterations; i++) {
		for (uint32_t j=0;
			j<(strl/sizeof(char16_t))-5-1; j=j+10+i) {
			strb->setPosition(j);
			ucs2literal(sixes,"66666");
			strb->write(sixes);
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

		bytestring::zero(str,strl*sizeof(char16_t));

		char16_t	*data=new char16_t[1024*(i+1)+1];
		ucs2charstring::zero(data,1024*(i+1)+1);
		for (uint16_t j=0; j<1024*(i+1)/16; j++) {
			ucs2charstring::append(data,numbersletters);
			ucs2charstring::append(str,numbersletters);
		}

		strb=new ucs2stringbuffer(data,
					ucs2charstring::length(data));
		for (uint16_t j=0; j<=(i+1)*100; j++) {
			strb->append(numbersletters);
			ucs2charstring::append(str,numbersletters);
		}

		test("contents",
			!ucs2charstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");

	return 0;
}
