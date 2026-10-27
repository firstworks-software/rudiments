// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// check the text and the return value of one number write
static void checkWrite(const char *name, stringbuffer *sb,
				ssize_t result, const char *expected) {
	test(name,result==(ssize_t)charstring::getLength(expected) &&
		!charstring::compare(sb->getString(),expected));
}

// number output, including the unsigned values above the signed range
static void numberOutput() {

	stdoutput.printf("number output...\n");

	const uint64_t	tento19=(uint64_t)100000*100000*1000000000;
	const uint64_t	max64=(uint64_t)-1;
	const int64_t	maxint64=(int64_t)(((uint64_t)1<<63)-1);
	const int64_t	minint64=-maxint64-1;
	stringbuffer	sb;

	sb.clear();
	checkWrite("write uint16 0",&sb,sb.write((uint16_t)0),"0");
	sb.clear();
	checkWrite("write uint16 12345",&sb,sb.write((uint16_t)12345),"12345");
	sb.clear();
	checkWrite("write uint16 32768",&sb,sb.write((uint16_t)32768),"32768");
	sb.clear();
	checkWrite("write uint16 40000",&sb,sb.write((uint16_t)40000),"40000");
	sb.clear();
	checkWrite("write uint16 65535",&sb,sb.write((uint16_t)65535),"65535");

	sb.clear();
	checkWrite("write uint32 0",&sb,sb.write((uint32_t)0),"0");
	sb.clear();
	checkWrite("write uint32 123456789",&sb,
			sb.write((uint32_t)123456789),"123456789");
	sb.clear();
	checkWrite("write uint32 2147483648",&sb,
			sb.write((uint32_t)2147483648U),"2147483648");
	sb.clear();
	checkWrite("write uint32 3000000000",&sb,
			sb.write((uint32_t)3000000000U),"3000000000");
	sb.clear();
	checkWrite("write uint32 4294967295",&sb,
			sb.write((uint32_t)4294967295U),"4294967295");

	sb.clear();
	checkWrite("write uint64 0",&sb,sb.write((uint64_t)0),"0");
	sb.clear();
	checkWrite("write uint64 4294967296",&sb,
			sb.write((uint64_t)4294967296ULL),"4294967296");
	sb.clear();
	checkWrite("write uint64 9223372036854775807",&sb,
			sb.write((uint64_t)maxint64),"9223372036854775807");
	sb.clear();
	checkWrite("write uint64 9223372036854775808",&sb,
			sb.write((uint64_t)maxint64+1),"9223372036854775808");
	sb.clear();
	checkWrite("write uint64 10000000000000000000",&sb,
			sb.write(tento19),"10000000000000000000");
	sb.clear();
	checkWrite("write uint64 18446744073709551615",&sb,
			sb.write(max64),"18446744073709551615");

	sb.clear();
	checkWrite("write int16 -32768",&sb,
			sb.write((int16_t)-32768),"-32768");
	sb.clear();
	checkWrite("write int16 32767",&sb,sb.write((int16_t)32767),"32767");
	sb.clear();
	checkWrite("write int32 -2147483648",&sb,
			sb.write((int32_t)-2147483647-1),"-2147483648");
	sb.clear();
	checkWrite("write int32 2147483647",&sb,
			sb.write((int32_t)2147483647),"2147483647");
	sb.clear();
	checkWrite("write int64 -9223372036854775808",&sb,
			sb.write(minint64),"-9223372036854775808");
	sb.clear();
	checkWrite("write int64 9223372036854775807",&sb,
			sb.write(maxint64),"9223372036854775807");

	// every unsigned write agrees with the append of the same number
	stringbuffer	ap;
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
		if (charstring::compare(sb.getString(),ap.getString())) {
			ok=false;
		}
	}
	test("write and append agree",ok);

	// the number writes append one after another
	sb.clear();
	sb.write((uint16_t)65535);
	sb.write((uint32_t)4294967295U);
	sb.write(max64);
	test("write sequence",!charstring::compare(sb.getString(),
				"65535""4294967295""18446744073709551615"));

	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	header("stringbuffer");

	numberOutput();

	const char	alphabet[]="abcdefghijklmnopqrstuvwxyz";
	const uint32_t	iterations=5;
	uint32_t	members;
	size_t		strl;
	if (sys::getPhysicalMemorySize()>1024*1024*1024) {
		// for systems with more than 1G of memory...
		members=1024;
		strl=700000;
	} else {
		// for systems with less than 1G of memory...
		members=64;
		strl=40000;
	}
	char	*str=new char[strl];
	char	buf[64];

	// create a new string buffer
	stringbuffer	*strb=new stringbuffer();

	// append...
	stdoutput.printf("append...\n");
	for (uint16_t i=0; i<iterations; i++) {

		strb->clear();
		bytestring::zero(str,strl);

		for (uint16_t j=0; j<members; j++) {

			// NULL
			strb->append((const char *)NULL);

			// empty string
			strb->append("");

			// strings
			for (uint16_t k=1; k<=25; k++) {
				strb->append(alphabet,k);
				charstring::append(str,alphabet,k);
			}

			// integers
			for (uint64_t k=1; k<10000; k*=10) {
				strb->append((uint16_t)k);
				charstring::printf(buf,sizeof(buf),
							"%hd",(uint16_t)k);
				charstring::append(str,buf);
			}
			for (uint64_t k=1; k<1000000000; k*=10) {
				strb->append((uint32_t)k);
				charstring::printf(buf,sizeof(buf),
							"%ld",(uint32_t)k);
				charstring::append(str,buf);
			}
			#ifdef RUDIMENTS_HAVE_LONG_LONG
			for (uint64_t k=1; k<1000000000000000000ULL; k*=10) {
				strb->append((uint64_t)k);
				charstring::printf(buf,sizeof(buf),
							"%lld",(uint64_t)k);
				charstring::append(str,buf);
			}
			#endif

			// float
			float	mult=10;
			for (uint16_t k=1; k<5; k++) {
				float	l=k+((float)k/mult);
				mult*=10;
				strb->append(l,k+1,(uint16_t)l);
				charstring::printf(buf,sizeof(buf),
							"%*.*f",k+1,k,l);
				charstring::append(str,buf);
			}

			// double
			mult=10;
			for (uint16_t k=1; k<5; k++) {
				double	l=k+((double)k/mult);
				mult*=10;
				strb->append(l,k+1,k);
				charstring::printf(buf,sizeof(buf),
							"%*.*f",k+1,k,l);
				charstring::append(str,buf);
			}
		}

		test("length",charstring::getLength(str)==strb->getStringLength());
		test("contents",!charstring::compare(str,strb->getString()));
	}
	stdoutput.printf("\n");


	// setPosition and write...
	stdoutput.printf("write...\n");
	size_t	strlen=charstring::getLength(str);
	for (uint16_t i=0; i<iterations; i++) {
		for (uint32_t j=0; j<strlen-5-1; j=j+30+i) {
			strb->setPositionRelativeToBeginning(j);
			strb->write("66666");
			charstring::copy(str+j,"66666",5);
		}
		test("contents",!charstring::compare(str,strb->getString()));
	}
	delete strb;
	stdoutput.printf("\n");


	// initial contents...
	stdoutput.printf("initial contents...\n");
	for (uint16_t i=0; i<iterations; i++) {

		bytestring::zero(str,strl);

		char	*data=new char[1024*(i+1)+1];
		bytestring::zero(data,1024*(i+1)+1);
		for (uint16_t j=0; j<1024*(i+1)/16; j++) {
			charstring::append(data,"0123456789abcdef");
			charstring::append(str,"0123456789abcdef");
		}

		strb=new stringbuffer(data,charstring::getLength(data));
		for (uint16_t j=0; j<=(i+1)*100; j++) {
			strb->append("0123456789abcdef");
			charstring::append(str,"0123456789abcdef");
		}

		test("contents",!charstring::compare(strb->getString(),str));
		delete strb;
	}
	stdoutput.printf("\n");

	delete[] str;

	return 0;
}
