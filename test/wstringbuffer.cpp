// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/wstringbuffer.h>
#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

#include <wchar.h>

uint32_t	iterations=5;
uint32_t	members=1024;

wchar_t	alphabet[]=L"abcdefghijklmnopqrstuvwxyz";

int main(int argc, const char **argv) {

	header("wstringbuffer");


	// create a new string buffer
	wstringbuffer	*strb=new wstringbuffer();
	wchar_t		str[800000];
	wchar_t		buf[64];

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,sizeof(str));

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->append((const wchar_t *)NULL);

			// empty string
			strb->append(L"");

			// strings
			for (uint16_t k=1; k<=25; k++) {
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
		for (uint16_t j=0; j<10000; j++) {
			strb->setPosition(j*(30+i));
			strb->write(L"66666");
			wcharstring::copy(str+(j*(30+i)),L"66666",5);
		}
		test("contents",!wcharstring::compare(str,strb->getString()));
	}
	delete strb;
	stdoutput.printf("\n");


	// initial contents...
	stdoutput.printf("initial contents...\n");
	for (uint16_t i=0; i<iterations; i++) {

		bytestring::zero(str,sizeof(str));

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
}
