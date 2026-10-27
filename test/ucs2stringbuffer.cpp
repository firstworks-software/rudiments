// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
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

// setPosition range checks, return values and truncate units
static void setPositionLimits() {

	signalmanager::alarm(60);
	stdoutput.printf("setPosition and truncate...\n");

	ucs2literal(abc,"abc");
	ucs2literal(abcd,"abcd");
	ucs2literal(abcdef,"abcdef");
	ucs2literal(abxdef,"abXdef");
	ucs2literal(ab,"ab");
	ucs2literal(x,"X");
	ucs2literal(c,"c");
	ucs2literal(d,"d");

	// the largest and smallest offsets whose byte count fits an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	const off64_t	maxchars=max/(off64_t)sizeof(ucs2_t);
	const off64_t	minchars=(-max-1)/(off64_t)sizeof(ucs2_t);

	// return values are in characters
	ucs2stringbuffer	*sb=new ucs2stringbuffer();
	sb->appendUcs2(abc);
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
		sb=new ucs2stringbuffer();
		sb->appendUcs2(abcdef);
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
			sb->writeUcs2(x);
			if (ucs2charstring::compare(sb->getString(),abxdef)) {
				ok=false;
			}
			sb->setPositionRelativeToBeginning(2);
			sb->writeUcs2(c);
			sb->writeUcs2(d);
		}
		test((method==0)?"begin out of range":
			(method==1)?"current out of range":
					"end out of range",ok);
		delete sb;
	}

	// the extreme offsets that still fit are accepted
	sb=new ucs2stringbuffer();
	sb->appendUcs2(abcdef);
	test("begin max",sb->setPositionRelativeToBeginning(maxchars)==
								maxchars);
	test("begin max position",sb->getPosition()==maxchars);
	sb->setPositionRelativeToBeginning(0);
	test("current max",sb->setPositionRelativeToCurrent(maxchars)==
								maxchars);
	sb->setPositionRelativeToBeginning(0);
	test("position restored",sb->getPosition()==0);
	delete sb;
	sb=new ucs2stringbuffer();
	test("end max",sb->setPositionRelativeToEnd(maxchars)==maxchars);
	test("end max position",sb->getPosition()==maxchars);
	delete sb;

	// a position before the start is refused, the start is accepted
	const off64_t	before[3][3]={{-1,minchars,0},
					{-3,minchars,-2},
					{-7,minchars,-6}};
	for (uint16_t method=0; method<3; method++) {
		bool	ok=true;
		sb=new ucs2stringbuffer();
		sb->appendUcs2(abcdef);
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
		sb->writeUcs2(x);
		if (ucs2charstring::compare(sb->getString(),abxdef)) {
			ok=false;
		}
		test((method==0)?"begin before start":
			(method==1)?"current before start":
					"end before start",ok);
		delete sb;
	}

	// truncate counts characters
	sb=new ucs2stringbuffer();
	sb->appendUcs2(abcd);
	sb->truncate(2);
	test("truncate size",sb->getSize()==2*sizeof(ucs2_t));
	test("truncate contents",!ucs2charstring::compare(sb->getString(),ab));
	sb->truncate(100);
	test("truncate past end",sb->getSize()==2*sizeof(ucs2_t));
	sb->clear();
	sb->appendUcs2(abcd);
	sb->truncate((size_t)-1/sizeof(ucs2_t)+1);
	test("truncate too large",sb->getSize()==4*sizeof(ucs2_t));
	sb->truncate((size_t)-1);
	test("truncate max",sb->getSize()==4*sizeof(ucs2_t));
	sb->truncate((size_t)-1/sizeof(ucs2_t));
	test("truncate limit",sb->getSize()==4*sizeof(ucs2_t));
	sb->setPositionRelativeToBeginning(3);
	sb->truncate();
	test("truncate current size",sb->getSize()==3*sizeof(ucs2_t));
	test("truncate current contents",
		!ucs2charstring::compare(sb->getString(),abc));
	sb->setPositionRelativeToBeginning(1);
	sb->truncate();
	test("truncate current 1",sb->getSize()==sizeof(ucs2_t));
	delete sb;

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

// true if the contents of "sb" are exactly "expected", in host-order UCS-2
static bool sameUcs2(ucs2stringbuffer *sb, const char *expected) {
	size_t	len=charstring::getLength(expected);
	if (sb->getSize()!=len*sizeof(ucs2_t)) {
		return false;
	}
	const ucs2_t	*buf=(const ucs2_t *)sb->getBuffer();
	for (size_t i=0; i<len; i++) {
		if (buf[i]!=(ucs2_t)(unsigned char)expected[i]) {
			return false;
		}
	}
	return true;
}

// check the text and the return value (bytes) of one number write
static void checkWrite(const char *name, ucs2stringbuffer *sb,
				ssize_t result, const char *expected) {
	test(name,result==(ssize_t)(charstring::getLength(expected)*
							sizeof(ucs2_t)) &&
		sameUcs2(sb,expected));
}

// check the text of one number append
static void checkAppend(const char *name, ucs2stringbuffer *sb,
						const char *expected) {
	test(name,sameUcs2(sb,expected));
}

// number output; these need no ucs2charstring printf
static void numberOutput() {

	signalmanager::alarm(60);
	stdoutput.printf("number output...\n");

	const uint64_t	tento19=(uint64_t)100000*100000*1000000000;
	const uint64_t	max64=(uint64_t)-1;
	const int64_t	maxint64=(int64_t)(((uint64_t)1<<63)-1);
	const int64_t	minint64=-maxint64-1;
	ucs2stringbuffer	sb;

	// integers
	sb.clear();
	checkWrite("write int16 5",&sb,sb.write((int16_t)5),"5");
	sb.clear();
	checkWrite("write int16 -32768",&sb,
			sb.write((int16_t)-32768),"-32768");
	sb.clear();
	checkWrite("write int32 5",&sb,sb.write((int32_t)5),"5");
	sb.clear();
	checkWrite("write int32 -2147483648",&sb,
			sb.write((int32_t)-2147483647-1),"-2147483648");
	sb.clear();
	checkWrite("write int64 5",&sb,sb.write((int64_t)5),"5");
	sb.clear();
	checkWrite("write int64 -9223372036854775808",&sb,
			sb.write(minint64),"-9223372036854775808");
	sb.clear();
	checkWrite("write int64 9223372036854775807",&sb,
			sb.write(maxint64),"9223372036854775807");
	sb.clear();
	checkWrite("write uint16 0",&sb,sb.write((uint16_t)0),"0");
	sb.clear();
	checkWrite("write uint16 40000",&sb,sb.write((uint16_t)40000),"40000");
	sb.clear();
	checkWrite("write uint16 65535",&sb,sb.write((uint16_t)65535),"65535");
	sb.clear();
	checkWrite("write uint32 0",&sb,sb.write((uint32_t)0),"0");
	sb.clear();
	checkWrite("write uint32 3000000000",&sb,
			sb.write((uint32_t)3000000000U),"3000000000");
	sb.clear();
	checkWrite("write uint32 4294967295",&sb,
			sb.write((uint32_t)4294967295U),"4294967295");
	sb.clear();
	checkWrite("write uint64 0",&sb,sb.write((uint64_t)0),"0");
	sb.clear();
	checkWrite("write uint64 9223372036854775808",&sb,
			sb.write((uint64_t)maxint64+1),"9223372036854775808");
	sb.clear();
	checkWrite("write uint64 10000000000000000000",&sb,
			sb.write(tento19),"10000000000000000000");
	sb.clear();
	checkWrite("write uint64 18446744073709551615",&sb,
			sb.write(max64),"18446744073709551615");

	// floats and doubles, the same text as stringbuffer
	sb.clear();
	checkWrite("write float",&sb,sb.write(1.5f),"1.5000");
	sb.clear();
	checkWrite("write float scale",&sb,sb.write(2.5f,2),"2.50");
	sb.clear();
	checkWrite("write float scale 0",&sb,sb.write(2.5f,0),"2");
	sb.clear();
	checkWrite("write float precision",&sb,
			sb.write(3.25f,(uint16_t)8,(uint16_t)2),"    3.25");
	sb.clear();
	checkWrite("write float negative",&sb,
			sb.write(-0.5f,(uint16_t)0,(uint16_t)3),"-0.500");
	sb.clear();
	checkWrite("write double",&sb,sb.write(1.5),"1.5000");
	sb.clear();
	checkWrite("write double scale",&sb,sb.write(2.5,2),"2.50");
	sb.clear();
	checkWrite("write double precision",&sb,
			sb.write(3.25,(uint16_t)8,(uint16_t)2),"    3.25");
	sb.clear();
	checkWrite("write double negative",&sb,
			sb.write(-0.5,(uint16_t)0,(uint16_t)3),"-0.500");
	sb.clear();
	checkWrite("write double 1e30",&sb,sb.write(1e30),
			"1000000000000000019884624838656.0000");
	sb.clear();
	checkWrite("write double 1e30 precision",&sb,
			sb.write(1e30,(uint16_t)40,(uint16_t)2),
			"      1000000000000000019884624838656.00");

	// append
	sb.clear();
	sb.append(1.5f);
	checkAppend("append float",&sb,"1.5000");
	sb.clear();
	sb.append(2.5f,(uint16_t)2);
	checkAppend("append float scale",&sb,"2.50");
	sb.clear();
	sb.append(3.25f,(uint16_t)8,(uint16_t)2);
	checkAppend("append float precision",&sb,"    3.25");
	sb.clear();
	sb.append(1.5);
	checkAppend("append double",&sb,"1.5000");
	sb.clear();
	sb.append(2.5,(uint16_t)2);
	checkAppend("append double scale",&sb,"2.50");
	sb.clear();
	sb.append(3.25,(uint16_t)8,(uint16_t)2);
	checkAppend("append double precision",&sb,"    3.25");
	sb.clear();
	sb.append(1e30);
	checkAppend("append double 1e30",&sb,
			"1000000000000000019884624838656.0000");
	sb.clear();
	sb.append(max64);
	checkAppend("append uint64",&sb,"18446744073709551615");

	// append and write in sequence
	sb.clear();
	sb.append((int32_t)-7);
	sb.append(1.25f,(uint16_t)2);
	sb.write(max64);
	sb.write(0.5,(uint16_t)1);
	sb.append((uint16_t)9);
	checkAppend("sequence",&sb,"-71.2518446744073709551615""0.59");

	// the same text as stringbuffer for the same input
	bool	ok=true;
	for (uint16_t scale=0; scale<8; scale++) {
		for (uint16_t i=0; i<200; i++) {
			double	d=(((double)i-100.0)*1.37)/(1+(i%7));
			float	f=(float)d;
			uint16_t	precision=i%12;
			ucs2stringbuffer	u;
			stringbuffer		n;
			u.append(f,scale);
			n.append(f,scale);
			u.write(d,scale);
			n.write(d,scale);
			u.append(d,precision,scale);
			n.append(d,precision,scale);
			u.write(f,precision,scale);
			n.write(f,precision,scale);
			u.append(f);
			n.append(f);
			u.write(d);
			n.write(d);
			if (!sameUcs2(&u,n.getString())) {
				ok=false;
			}
		}
	}
	test("floats and doubles as stringbuffer",ok);

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

// initialsize counts bytes
static void initialSizeBytes() {

	signalmanager::alarm(60);
	stdoutput.printf("initialsize...\n");

	ucs2literal(abc,"abc");
	ucs2literal(abcd,"abcd");

	// the attach constructor takes the size in bytes
	ucs2_t	*data=new ucs2_t[3];
	bytestring::copy(data,abc,3*sizeof(ucs2_t));
	ucs2stringbuffer	*sb=new ucs2stringbuffer(data,
						3*sizeof(ucs2_t));
	test("attach getSize",sb->getSize()==3*sizeof(ucs2_t));
	test("attach getPosition",sb->getPosition()==3);
	sb->appendUcs2((ucs2_t)'d');
	test("attach append",!ucs2charstring::compare(sb->getString(),abcd));
	test("attach append size",sb->getSize()==4*sizeof(ucs2_t));
	delete sb;

	// clear takes the size in bytes
	sb=new ucs2stringbuffer();
	data=new ucs2_t[3];
	bytestring::copy(data,abc,3*sizeof(ucs2_t));
	sb->clear(data,3*sizeof(ucs2_t));
	test("clear getSize",sb->getSize()==3*sizeof(ucs2_t));
	sb->appendUcs2((ucs2_t)'d');
	test("clear append",!ucs2charstring::compare(sb->getString(),abcd));
	delete sb;

	stdoutput.printf("\n");
	signalmanager::alarm(0);
}

int main(int argc, const char **argv) {

	header("ucs2stringbuffer");

	stringBufferBasics();
	setPositionLimits();
	numberOutput();
	initialSizeBytes();

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
				ucs2charstring::getLength(data)*sizeof(ucs2_t));
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
