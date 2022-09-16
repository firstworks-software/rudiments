// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/wstringbuffer.h>
#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

wchar_t	alphabet[]=L"abcdefghijklmnopqrstuvwxyz";

int main(int argc, const char **argv) {

	header("wstringbuffer");

	if (!wstringbuffer::supported() ||
			!wcharstring::supportsPrintf()) {
		stdoutput.printf("	not supported\n\n");
		return 0;
	}

	const wchar_t	alphabet[]=L"abcdefghijklmnopqrstuvwxyz";
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
	wchar_t	*str=new wchar_t[strl];
	wchar_t	buf[64];

	// create a new string buffer
	wstringbuffer	*strb=new wstringbuffer();

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,strl*sizeof(wchar_t));

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->append((const wchar_t *)NULL);

			// empty string
			strb->append(L"");

			// strings
			for (uint16_t k=1; k<=5; k++) {
				strb->append(alphabet,k);
				wcharstring::append(str,alphabet,k);
			}

			// integers
			for (uint64_t k=1; k<10000; k*=10) {
				strb->append((uint16_t)k);
				wcharstring::printf(buf,
						sizeof(buf)/sizeof(wchar_t),
						L"%hd",(uint16_t)k);
				wcharstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000; k*=10) {
				strb->append((uint32_t)k);
				wcharstring::printf(buf,
						sizeof(buf)/sizeof(wchar_t),
						L"%ld",(uint32_t)k);
				wcharstring::append(str,buf);
			}
			#ifdef RUDIMENTS_HAVE_LONG_LONG
			for (uint64_t k=1; k<1000000000000000000ULL; k*=10) {
				strb->append((uint64_t)k);
				wcharstring::printf(buf,
						sizeof(buf)/sizeof(wchar_t),
						L"%lld",(uint64_t)k);
				wcharstring::append(str,buf);
			}
			#endif

			// float
			float	mult=10;
			for (uint16_t k=1; k<5; k++) {
				float	l=k+((float)k/mult);
				mult*=10;
				strb->append(l,k+1,(uint16_t)l);
				wcharstring::printf(buf,
						sizeof(buf)/sizeof(wchar_t),
						L"%*.*f",k+1,k,l);
				wcharstring::append(str,buf);
			}

			// double
			mult=10;
			for (uint16_t k=1; k<5; k++) {
				double	l=k+((double)k/mult);
				mult*=10;
				strb->append(l,k+1,k);
				wcharstring::printf(buf,
						sizeof(buf)/sizeof(wchar_t),
						L"%*.*f",k+1,k,l);
				wcharstring::append(str,buf);
			}
		}

		test("length",wcharstring::length(str)==
					strb->getStringLength());
		test("contents",!wcharstring::compare(str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	for (uint16_t i=0; i<iterations; i++) {
		for (uint32_t j=0;
			j<(strl/sizeof(wchar_t))-5-1; j=j+10+i) {
			strb->setPosition(j);
			strb->write(L"66666");
			wcharstring::copy(str+j,L"66666",5);
		}
		test("contents",!wcharstring::compare(str,strb->getString()));
	}
	delete strb;
	stdoutput.printf("\n");


	// initial contents...
	stdoutput.printf("initial contents...\n");
	for (uint16_t i=0; i<iterations; i++) {

		bytestring::zero(str,strl*sizeof(wchar_t));

		wchar_t	*data=new wchar_t[1024*(i+1)+1];
		wcharstring::zero(data,1024*(i+1)+1);
		for (uint16_t j=0; j<1024*(i+1)/16; j++) {
			wcharstring::append(data,L"0123456789abcdef");
			wcharstring::append(str,L"0123456789abcdef");
		}

		strb=new wstringbuffer(data,wcharstring::length(data));
		for (uint16_t j=0; j<=(i+1)*100; j++) {
			strb->append(L"0123456789abcdef");
			wcharstring::append(str,L"0123456789abcdef");
		}

		test("contents",!wcharstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");

	process::exit(0);
}
