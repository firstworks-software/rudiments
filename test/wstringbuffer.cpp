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

// setPosition range checks, return values and truncate units
static void setPositionLimits() {

	signalmanager::alarm(60);
	stdoutput.printf("setPosition and truncate...\n");

	// the largest and smallest offsets whose byte count fits an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	const off64_t	maxchars=max/(off64_t)sizeof(wchar_t);
	const off64_t	minchars=(-max-1)/(off64_t)sizeof(wchar_t);

	// return values are in characters
	wstringbuffer	*sb=new wstringbuffer();
	sb->append(L"abc");
	test("begin return",sb->setPositionRelativeToBeginning(1)==1);
	test("current return",sb->setPositionRelativeToCurrent(1)==2);
	test("end return",sb->setPositionRelativeToEnd(-1)==2);
	test("end 0 return",sb->setPositionRelativeToEnd(0)==3);
	test("current 0 return",sb->setPositionRelativeToCurrent(0)==3);
	test("return matches getPosition",
		sb->setPositionRelativeToBeginning(2)==sb->getPosition());
	delete sb;

	// an offset whose byte count does not fit is refused
	const off64_t	bad[]={maxchars+1,minchars-1,max,-max-1};
	for (uint16_t method=0; method<3; method++) {
		bool	ok=true;
		sb=new wstringbuffer();
		sb->append(L"abcdef");
		for (uint16_t i=0; i<4; i++) {
			sb->setPositionRelativeToBeginning(2);
			off64_t	result=(method==0)?
				sb->setPositionRelativeToBeginning(bad[i]):
				(method==1)?
				sb->setPositionRelativeToCurrent(bad[i]):
				sb->setPositionRelativeToEnd(bad[i]);
			if (result!=-1 || sb->getPosition()!=2) {
				ok=false;
			}
			sb->write(L"X");
			if (wcharstring::compare(sb->getString(),L"abXdef")) {
				ok=false;
			}
			sb->setPositionRelativeToBeginning(2);
			sb->write(L"c");
			sb->write(L"d");
		}
		test((method==0)?"begin out of range":
			(method==1)?"current out of range":
					"end out of range",ok);
		delete sb;
	}

	// the extreme offsets that still fit are accepted
	sb=new wstringbuffer();
	sb->append(L"abcdef");
	test("begin max",sb->setPositionRelativeToBeginning(maxchars)==
								maxchars);
	test("begin max position",sb->getPosition()==maxchars);
	sb->setPositionRelativeToBeginning(0);
	test("current max",sb->setPositionRelativeToCurrent(maxchars)==
								maxchars);
	sb->setPositionRelativeToBeginning(0);
	test("position restored",sb->getPosition()==0);
	delete sb;
	sb=new wstringbuffer();
	test("end max",sb->setPositionRelativeToEnd(maxchars)==maxchars);
	test("end max position",sb->getPosition()==maxchars);
	delete sb;

	// a position before the start is refused, the start is accepted
	const off64_t	before[3][3]={{-1,minchars,0},
					{-3,minchars,-2},
					{-7,minchars,-6}};
	for (uint16_t method=0; method<3; method++) {
		bool	ok=true;
		sb=new wstringbuffer();
		sb->append(L"abcdef");
		for (uint16_t i=0; i<3; i++) {
			sb->setPositionRelativeToBeginning(2);
			off64_t	result=(method==0)?
				sb->setPositionRelativeToBeginning(
							before[method][i]):
				(method==1)?
				sb->setPositionRelativeToCurrent(
							before[method][i]):
				sb->setPositionRelativeToEnd(
							before[method][i]);
			off64_t	expected=(i==2)?0:-1;
			off64_t	position=(i==2)?0:2;
			if (result!=expected ||
					sb->getPosition()!=position) {
				ok=false;
			}
		}
		sb->setPositionRelativeToBeginning(2);
		sb->setPositionRelativeToCurrent(-3);
		sb->write(L"X");
		if (wcharstring::compare(sb->getString(),L"abXdef")) {
			ok=false;
		}
		test((method==0)?"begin before start":
			(method==1)?"current before start":
					"end before start",ok);
		delete sb;
	}

	// truncate counts characters
	sb=new wstringbuffer();
	sb->append(L"abcd");
	sb->truncate(2);
	test("truncate size",sb->getSize()==2*sizeof(wchar_t));
	test("truncate contents",!wcharstring::compare(sb->getString(),L"ab"));
	sb->truncate(100);
	test("truncate past end",sb->getSize()==2*sizeof(wchar_t));
	sb->clear();
	sb->append(L"abcd");
	sb->truncate((size_t)-1/sizeof(wchar_t)+1);
	test("truncate too large",sb->getSize()==4*sizeof(wchar_t));
	sb->truncate((size_t)-1);
	test("truncate max",sb->getSize()==4*sizeof(wchar_t));
	sb->truncate((size_t)-1/sizeof(wchar_t));
	test("truncate limit",sb->getSize()==4*sizeof(wchar_t));
	sb->setPositionRelativeToBeginning(3);
	sb->truncate();
	test("truncate current size",sb->getSize()==3*sizeof(wchar_t));
	test("truncate current contents",
		!wcharstring::compare(sb->getString(),L"abc"));
	sb->setPositionRelativeToBeginning(1);
	sb->truncate();
	test("truncate current 1",sb->getSize()==sizeof(wchar_t));
	delete sb;

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

// check the text of one number write
static void checkWrite(const char *name, wstringbuffer *sb,
				ssize_t result, const wchar_t *expected) {
	test(name,result>0 &&
		!wcharstring::compare(sb->getString(),expected));
}

// number output, including the unsigned values above the signed range
static void numberOutput() {

	signalmanager::alarm(60);
	stdoutput.printf("number output...\n");

	const uint64_t	tento19=(uint64_t)100000*100000*1000000000;
	const uint64_t	max64=(uint64_t)-1;
	const int64_t	maxint64=(int64_t)(((uint64_t)1<<63)-1);
	const int64_t	minint64=-maxint64-1;
	wstringbuffer	sb;

	sb.clear();
	checkWrite("write uint16 0",&sb,sb.write((uint16_t)0),L"0");
	sb.clear();
	checkWrite("write uint16 12345",&sb,
			sb.write((uint16_t)12345),L"12345");
	sb.clear();
	checkWrite("write uint16 32768",&sb,
			sb.write((uint16_t)32768),L"32768");
	sb.clear();
	checkWrite("write uint16 40000",&sb,
			sb.write((uint16_t)40000),L"40000");
	sb.clear();
	checkWrite("write uint16 65535",&sb,
			sb.write((uint16_t)65535),L"65535");

	sb.clear();
	checkWrite("write uint32 0",&sb,sb.write((uint32_t)0),L"0");
	sb.clear();
	checkWrite("write uint32 123456789",&sb,
			sb.write((uint32_t)123456789),L"123456789");
	sb.clear();
	checkWrite("write uint32 2147483648",&sb,
			sb.write((uint32_t)2147483648U),L"2147483648");
	sb.clear();
	checkWrite("write uint32 3000000000",&sb,
			sb.write((uint32_t)3000000000U),L"3000000000");
	sb.clear();
	checkWrite("write uint32 4294967295",&sb,
			sb.write((uint32_t)4294967295U),L"4294967295");

	sb.clear();
	checkWrite("write uint64 0",&sb,sb.write((uint64_t)0),L"0");
	sb.clear();
	checkWrite("write uint64 4294967296",&sb,
			sb.write((uint64_t)4294967296ULL),L"4294967296");
	sb.clear();
	checkWrite("write uint64 9223372036854775807",&sb,
			sb.write((uint64_t)maxint64),L"9223372036854775807");
	sb.clear();
	checkWrite("write uint64 9223372036854775808",&sb,
			sb.write((uint64_t)maxint64+1),L"9223372036854775808");
	sb.clear();
	checkWrite("write uint64 10000000000000000000",&sb,
			sb.write(tento19),L"10000000000000000000");
	sb.clear();
	checkWrite("write uint64 18446744073709551615",&sb,
			sb.write(max64),L"18446744073709551615");

	sb.clear();
	checkWrite("write int16 -32768",&sb,
			sb.write((int16_t)-32768),L"-32768");
	sb.clear();
	checkWrite("write int16 32767",&sb,
			sb.write((int16_t)32767),L"32767");
	sb.clear();
	checkWrite("write int32 -2147483648",&sb,
			sb.write((int32_t)-2147483647-1),L"-2147483648");
	sb.clear();
	checkWrite("write int32 2147483647",&sb,
			sb.write((int32_t)2147483647),L"2147483647");
	sb.clear();
	checkWrite("write int64 -9223372036854775808",&sb,
			sb.write(minint64),L"-9223372036854775808");
	sb.clear();
	checkWrite("write int64 9223372036854775807",&sb,
			sb.write(maxint64),L"9223372036854775807");

	// every unsigned write agrees with the append of the same number
	wstringbuffer	ap;
	bool		ok=true;
	for (uint64_t n=max64; n!=0; n>>=3) {
		sb.clear();
		ap.clear();
		sb.write((uint16_t)n);
		ap.append((uint16_t)n);
		sb.write((uint32_t)n);
		ap.append((uint32_t)n);
		sb.write(n);
		ap.append(n);
		if (wcharstring::compare(sb.getString(),ap.getString())) {
			ok=false;
		}
	}
	test("write and append agree",ok);

	// the number writes go one after another
	sb.clear();
	sb.write((uint16_t)65535);
	sb.write((uint32_t)4294967295U);
	sb.write(max64);
	test("write sequence",!wcharstring::compare(sb.getString(),
			L"65535" L"4294967295" L"18446744073709551615"));

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

int main(int argc, const char **argv) {

	header("wstringbuffer");

	stringBufferBasics();
	setPositionLimits();
	numberOutput();

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
