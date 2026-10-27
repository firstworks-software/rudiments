// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/wstringbuffer.h>
#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/signalclasses.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// getString, getPosition and attached contents; these need no printf
static void stringBufferBasics() {

	signalmanager::alarm(60);
	stdoutput.printf("getString, getPosition and initialsize...\n");

	// getString removes one character, not one byte
	wstringbuffer	*sb=new wstringbuffer();
	sb->append(L"ab");
	test("getString ab",!wcharstring::compare(sb->getString(),L"ab"));
	test("getSize ab",sb->getSize()==2*sizeof(wchar_t));
	test("getPosition ab",sb->getPosition()==2);
	sb->append(L"c");
	test("getString abc",!wcharstring::compare(sb->getString(),L"abc"));
	test("getSize abc",sb->getSize()==3*sizeof(wchar_t));
	test("getStringLength abc",sb->getStringLength()==3);

	// getString, append and getStringLength repeated
	sb->clear();
	wchar_t	expected[201];
	bool	ok=true;
	for (size_t i=0; i<200; i++) {
		wchar_t	c=(wchar_t)(L'a'+(i%26));
		sb->append(c);
		expected[i]=c;
		expected[i+1]=0;
		if (sb->getStringLength()!=i+1 ||
			wcharstring::compare(sb->getString(),expected) ||
			sb->getSize()!=(i+1)*sizeof(wchar_t) ||
			sb->getPosition()!=(off64_t)(i+1)) {
			ok=false;
		}
	}
	test("repeated getString and append",ok);

	// no append after getString
	sb->clear();
	sb->append(L"hello");
	sb->getString();
	sb->getString();
	test("getString twice",
		!wcharstring::compare(sb->getString(),L"hello"));
	test("getSize twice",sb->getSize()==5*sizeof(wchar_t));
	delete sb;

	// positions count characters
	sb=new wstringbuffer();
	sb->append(L"abc");
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
	sb->write(L"X");
	test("position write",sb->getPosition()==2);
	test("write contents",!wcharstring::compare(sb->getString(),L"aXc"));
	delete sb;

	// an initialsize that would wrap attaches as size 0
	size_t	huge=((size_t)1<<(sizeof(size_t)*8-2))+1;
	wchar_t	*buf=new wchar_t[4];
	wcharstring::copy(buf,L"abc");
	sb=new wstringbuffer(buf,huge);
	test("huge initialsize",sb->getSize()==0);
	sb->append(L"xy");
	test("huge append",!wcharstring::compare(sb->getString(),L"xy"));
	buf=new wchar_t[4];
	wcharstring::copy(buf,L"abc");
	sb->clear(buf,huge);
	test("huge clear",sb->getSize()==0);
	sb->append(L"xyz");
	test("huge clear append",
		!wcharstring::compare(sb->getString(),L"xyz"));
	delete sb;

	// exact-size attached contents still work
	buf=new wchar_t[3];
	wcharstring::copy(buf,L"ab");
	sb=new wstringbuffer(buf,2);
	test("attach size",sb->getSize()==2*sizeof(wchar_t));
	sb->append(L"c");
	test("attach append",!wcharstring::compare(sb->getString(),L"abc"));
	delete sb;

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

int main(int argc, const char **argv) {

	header("wstringbuffer");

	stringBufferBasics();

	if (!wcharstring::supportsPrintf()) {
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

		test("length",wcharstring::getLength(str)==
					strb->getStringLength());
		test("contents",!wcharstring::compare(str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	for (uint16_t i=0; i<iterations; i++) {
		for (uint32_t j=0;
			j<(strl/sizeof(wchar_t))-5-1; j=j+10+i) {
			strb->setPositionRelativeToBeginning(j);
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

		strb=new wstringbuffer(data,wcharstring::getLength(data));
		for (uint16_t j=0; j<=(i+1)*100; j++) {
			strb->append(L"0123456789abcdef");
			wcharstring::append(str,L"0123456789abcdef");
		}

		test("contents",!wcharstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");

	return 0;
}
