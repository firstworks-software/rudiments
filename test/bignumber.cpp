// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/bignumber.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include "test.cpp"

// true if "b" renders as the decimal string "expected"
static bool decis(bignumber *b, const char *expected) {
	const char	*str=b->getString();
	return (str && !charstring::compare(str,expected));
}

// builds a test description around a quoted string value
static const char *desc(const char *prefix,
			const char *value, const char *suffix) {
	static char	buf[256];
	charstring::copy(buf,prefix);
	charstring::append(buf,"\"");
	charstring::append(buf,value);
	charstring::append(buf,"\"");
	charstring::append(buf,suffix);
	return buf;
}

// true if "b" renders as the hexadecimal string "expected"
static bool hexis(bignumber *b, const char *expected) {
	const char	*str=b->getString(16);
	return (str && !charstring::compare(str,expected));
}

int main(int argc, const char **argv) {

	header("bignumber");

	const int64_t	i64max=9223372036854775807LL;
	const int64_t	i64min=-i64max-1LL;
	const int32_t	i32max=2147483647;
	const int32_t	i32min=-i32max-1;
	const uint32_t	u32max=4294967295U;
	const uint64_t	u64max=18446744073709551615ULL;

	// a 30 digit value and a 20 digit value, used throughout
	const char	*bigdec="123456789012345678901234567890";
	const char	*bighex="18EE90FF6C373E0EE4E3F0AD2";
	const char	*bighexout="018EE90FF6C373E0EE4E3F0AD2";
	const char	*bigdec2="98765432109876543210";

	test("isSupported",bignumber().isSupported());


	// default construction
	{
		bignumber	b;
		test("default: is zero",b.isZero());
		test("default: string is 0",decis(&b,"0"));
		test("default: sign is 0",b.getSign()==0);
		test("default: not negative",!b.isNegative());
		test("default: bit count is 0",b.getBitCount()==0);
		test("default: magnitude size is 1",b.getMagnitudeSize()==1);
		test("default: no error",
			b.getError()==BIGNUMBER_ERROR_SUCCESS);
	}


	// decimal strings
	{
		bignumber	b("0");
		test("decimal \"0\": is zero",b.isZero());
		test("decimal \"0\": string",decis(&b,"0"));
	}
	{
		bignumber	b("123");
		test("decimal \"123\": string",decis(&b,"123"));
		test("decimal \"123\": positive",b.getSign()==1);
	}
	{
		bignumber	b("-123");
		test("decimal \"-123\": string",decis(&b,"-123"));
		test("decimal \"-123\": negative",b.isNegative());
		test("decimal \"-123\": sign",b.getSign()==-1);
	}
	{
		bignumber	b("+123");
		test("decimal \"+123\": string",decis(&b,"123"));
	}
	{
		bignumber	b("007");
		test("decimal \"007\": leading zeros stripped",decis(&b,"7"));
	}
	{
		bignumber	b("-0");
		test("decimal \"-0\": is zero",b.isZero());
		test("decimal \"-0\": no negative zero",decis(&b,"0"));
		test("decimal \"-0\": not negative",!b.isNegative());
	}
	{
		bignumber	b(bigdec);
		test("decimal 30 digits: round trip",decis(&b,bigdec));
		test("decimal 30 digits: bit count",b.getBitCount()==97);
		test("decimal 30 digits: magnitude size",
					b.getMagnitudeSize()==13);
	}
	{
		bignumber	b;
		test("setValue decimal 30 digits: succeeds",
					b.setValue(bigdec));
		test("setValue decimal 30 digits: round trip",
					decis(&b,bigdec));
		test("setValue decimal: no error",
			b.getError()==BIGNUMBER_ERROR_SUCCESS);
	}


	// hexadecimal strings
	{
		bignumber	b("1F",16);
		test("hex \"1F\": value",decis(&b,"31"));
		test("hex \"1F\": round trip",hexis(&b,"1F"));
	}
	{
		bignumber	b("1f",16);
		test("hex lower case: value",decis(&b,"31"));
		test("hex lower case: upper case round trip",hexis(&b,"1F"));
	}
	{
		bignumber	b("aBcDeF",16);
		test("hex mixed case: value",decis(&b,"11259375"));
		test("hex mixed case: round trip",hexis(&b,"ABCDEF"));
	}
	{
		// an odd number of digits is accepted on the way in, and
		// comes back with a leading 0, since the output is two
		// digits per magnitude byte, as BN_bn2hex() gives
		bignumber	b("ABC",16);
		test("hex odd digit count: value",decis(&b,"2748"));
		test("hex odd digit count: round trip",hexis(&b,"0ABC"));
	}
	{
		bignumber	b("-FF",16);
		test("hex \"-FF\": value",decis(&b,"-255"));
		test("hex \"-FF\": round trip",hexis(&b,"-FF"));
	}
	{
		bignumber	b("0",16);
		test("hex \"0\": is zero",b.isZero());
		test("hex \"0\": round trip",hexis(&b,"0"));
	}
	{
		bignumber	b("00FF",16);
		test("hex leading zeros: value",decis(&b,"255"));
		test("hex leading zeros: stripped",hexis(&b,"FF"));
	}
	{
		// the 30 digit decimal value, in hex, has 25 hex digits
		bignumber	b(bighex,16);
		test("hex 25 digits: decimal value",decis(&b,bigdec));
		bignumber	d(bigdec);
		test("hex 25 digits: hex of decimal value",
						hexis(&d,bighexout));
	}
	{
		bignumber	b;
		test("setValue base 10 explicitly",b.setValue("-42",10));
		test("setValue base 10 explicitly: value",decis(&b,"-42"));
	}


	// native integer types
	{
		bignumber	b((int32_t)0);
		test("int32_t 0: is zero",b.isZero());
	}
	{
		bignumber	b((int32_t)1);
		test("int32_t 1",decis(&b,"1"));
	}
	{
		bignumber	b((int32_t)-1);
		test("int32_t -1",decis(&b,"-1"));
	}
	{
		bignumber	b(i32max);
		test("int32_t max",decis(&b,"2147483647"));
	}
	{
		bignumber	b(i32min);
		test("int32_t min",decis(&b,"-2147483648"));
	}
	{
		bignumber	b((uint32_t)0);
		test("uint32_t 0: is zero",b.isZero());
	}
	{
		bignumber	b(u32max);
		test("uint32_t max",decis(&b,"4294967295"));
	}
	{
		bignumber	b(i64max);
		test("int64_t max",decis(&b,"9223372036854775807"));
	}
	{
		bignumber	b(i64min);
		test("int64_t min",decis(&b,"-9223372036854775808"));
	}
	{
		bignumber	b(u64max);
		test("uint64_t max",decis(&b,"18446744073709551615"));
		test("uint64_t max: bit count",b.getBitCount()==64);
		test("uint64_t max: magnitude size",b.getMagnitudeSize()==8);
	}
	{
		bignumber	b;
		b.setValue(i64min);
		test("setValue int64_t min",decis(&b,"-9223372036854775808"));
		b.setValue(u64max);
		test("setValue uint64_t max",decis(&b,"18446744073709551615"));
		b.setValue((int32_t)-5);
		test("setValue int32_t -5",decis(&b,"-5"));
		b.setValue(u32max);
		test("setValue uint32_t max",decis(&b,"4294967295"));
	}
	{
		bignumber	b;
		b=(int32_t)-7;
		test("operator= int32_t",decis(&b,"-7"));
		b=(uint32_t)7;
		test("operator= uint32_t",decis(&b,"7"));
		b=i64min;
		test("operator= int64_t",decis(&b,"-9223372036854775808"));
		b=u64max;
		test("operator= uint64_t",decis(&b,"18446744073709551615"));
		b="-31415926535897932384626433832795";
		test("operator= const char *",
			decis(&b,"-31415926535897932384626433832795"));
	}


	// raw big-endian bytes
	{
		const byte_t	mag[]={0x01,0x02,0x03};
		bignumber	b(mag,sizeof(mag));
		test("bytes 010203: value",decis(&b,"66051"));
		test("bytes 010203: positive",!b.isNegative());
		test("bytes 010203: magnitude size",b.getMagnitudeSize()==3);
		byte_t		buf[3];
		test("bytes 010203: getMagnitude",
					b.getMagnitude(buf,sizeof(buf)));
		test("bytes 010203: round trip",
				!bytestring::compare(buf,mag,sizeof(buf)));
	}
	{
		const byte_t	mag[]={0x00,0x00,0x05};
		bignumber	b(mag,sizeof(mag));
		test("bytes leading zeros: value",decis(&b,"5"));
		test("bytes leading zeros: magnitude size",
					b.getMagnitudeSize()==1);
		byte_t		buf[1];
		test("bytes leading zeros: getMagnitude",
					b.getMagnitude(buf,sizeof(buf)));
		test("bytes leading zeros: magnitude byte",buf[0]==0x05);
	}
	{
		const byte_t	mag[]={0xff,0xff,0xff,0xff,
					0xff,0xff,0xff,0xff};
		bignumber	b(mag,sizeof(mag),false);
		test("bytes 8 x ff positive",
					decis(&b,"18446744073709551615"));
		bignumber	n(mag,sizeof(mag),true);
		test("bytes 8 x ff negative",
					decis(&n,"-18446744073709551615"));
		test("bytes 8 x ff negative: isNegative",n.isNegative());
		byte_t		buf[8];
		test("bytes 8 x ff negative: getMagnitude",
					n.getMagnitude(buf,sizeof(buf)));
		test("bytes 8 x ff negative: magnitude round trip",
				!bytestring::compare(buf,mag,sizeof(buf)));
	}
	{
		const byte_t	mag[]={0x00,0x00};
		bignumber	b(mag,sizeof(mag),true);
		test("bytes zero magnitude, negative flag: is zero",
								b.isZero());
		test("bytes zero magnitude, negative flag: not negative",
							!b.isNegative());
	}
	{
		bignumber	b((const byte_t *)NULL,4);
		test("bytes NULL: is zero",b.isZero());
		bignumber	c((int64_t)5);
		test("setValue bytes NULL: fails",
					!c.setValue((const byte_t *)NULL,4));
		const byte_t	mag[]={0x01};
		test("setValue bytes size 0: succeeds",c.setValue(mag,0));
		test("setValue bytes size 0: is zero",c.isZero());
	}
	{
		// zero writes a single zero byte
		bignumber	b;
		byte_t		buf[1];
		buf[0]=0x55;
		test("zero: getMagnitude",b.getMagnitude(buf,sizeof(buf)));
		test("zero: magnitude byte is 0",buf[0]==0x00);
	}
	{
		// buffer too small
		bignumber	b((uint32_t)0x010203);
		byte_t		buf[2];
		test("getMagnitude buffer too small: fails",
					!b.getMagnitude(buf,sizeof(buf)));
		test("getMagnitude buffer too small: error",
			b.getError()==BIGNUMBER_ERROR_BUFFER_TOO_SMALL);
		test("getMagnitude NULL buffer: fails",
					!b.getMagnitude(NULL,10));
	}


	// copy construction and assignment
	{
		bignumber	a(bigdec);
		bignumber	b(a);
		test("copy constructor: value",decis(&b,bigdec));
		test("copy constructor: equal",b==a);
		b.setValue((int64_t)1);
		test("copy constructor: independent of original",
							decis(&a,bigdec));

		bignumber	c;
		c=a;
		test("operator=(bignumber): value",decis(&c,bigdec));
		a.setValue((int64_t)2);
		test("operator=(bignumber): independent of original",
							decis(&c,bigdec));

		bignumber	d((int64_t)-99);
		d.setValue(c);
		test("setValue(bignumber): value",decis(&d,bigdec));

		// self assignment
		d=d;
		test("self assignment",decis(&d,bigdec));
	}


	// getValue()
	{
		bignumber	b((int64_t)-12345);
		int64_t		i=0;
		test("getValue int64_t: succeeds",b.getValue(&i));
		test("getValue int64_t: value",i==-12345LL);
		test("getValue int64_t NULL: fails",
					!b.getValue((int64_t *)NULL));
	}
	{
		bignumber	b(i64max);
		int64_t		i=0;
		test("getValue int64_t max: succeeds",b.getValue(&i));
		test("getValue int64_t max: value",i==i64max);
	}
	{
		bignumber	b(i64min);
		int64_t		i=0;
		test("getValue int64_t min: succeeds",b.getValue(&i));
		test("getValue int64_t min: value",i==i64min);
	}
	{
		bignumber	b("9223372036854775808");
		int64_t		i=99;
		test("getValue int64_t max+1: fails",!b.getValue(&i));
		test("getValue int64_t max+1: value untouched",i==99);
		test("getValue int64_t max+1: error",
			b.getError()==BIGNUMBER_ERROR_OVERFLOW);
	}
	{
		bignumber	b("-9223372036854775809");
		int64_t		i=99;
		test("getValue int64_t min-1: fails",!b.getValue(&i));
		test("getValue int64_t min-1: value untouched",i==99);
	}
	{
		bignumber	b(u64max);
		uint64_t	u=0;
		test("getValue uint64_t max: succeeds",b.getValue(&u));
		test("getValue uint64_t max: value",u==u64max);
		int64_t		i=99;
		test("getValue int64_t of uint64_t max: fails",
							!b.getValue(&i));
		test("getValue int64_t of uint64_t max: value untouched",
								i==99);
	}
	{
		bignumber	b("18446744073709551616");
		uint64_t	u=99;
		test("getValue uint64_t max+1: fails",!b.getValue(&u));
		test("getValue uint64_t max+1: value untouched",u==99);
		test("getValue uint64_t max+1: error",
			b.getError()==BIGNUMBER_ERROR_OVERFLOW);
	}
	{
		bignumber	b((int64_t)-1);
		uint64_t	u=99;
		test("getValue uint64_t of negative: fails",!b.getValue(&u));
		test("getValue uint64_t of negative: value untouched",u==99);
		test("getValue uint64_t NULL: fails",
					!b.getValue((uint64_t *)NULL));
	}
	{
		bignumber	b;
		int64_t		i=99;
		uint64_t	u=99;
		test("getValue int64_t of zero",b.getValue(&i) && i==0);
		test("getValue uint64_t of zero",b.getValue(&u) && u==0);
	}


	// malformed input
	{
		bignumber	b((int64_t)5);
		test("setValue NULL: fails",
					!b.setValue((const char *)NULL));
		test("setValue NULL: instance is zero",b.isZero());
		test("setValue NULL: error",
			b.getError()==BIGNUMBER_ERROR_NULL);
	}
	{
		const char	*bad[]={
			"","abc","12a","1.5","+-5","5 "," 5","-","+",
			"1,000","1e5","--5",NULL
		};
		for (uint16_t i=0; bad[i]; i++) {
			bignumber	b((int64_t)5);
			test(desc("malformed decimal ",bad[i],": fails"),
						!b.setValue(bad[i]));
			test(desc("malformed decimal ",bad[i],": error"),
				b.getError()==
					BIGNUMBER_ERROR_INVALID_FORMAT);
			test(desc("malformed decimal ",bad[i],
					": instance is zero"),b.isZero());
		}
	}
	{
		const char	*bad[]={
			"","xyz","12g","0x1f"," 1f","1f ","-","+","1.f",NULL
		};
		for (uint16_t i=0; bad[i]; i++) {
			bignumber	b((int64_t)5);
			test(desc("malformed hex ",bad[i],": fails"),
						!b.setValue(bad[i],16));
			test(desc("malformed hex ",bad[i],": error"),
				b.getError()==
					BIGNUMBER_ERROR_INVALID_FORMAT);
			test(desc("malformed hex ",bad[i],
					": instance is zero"),b.isZero());
		}
	}
	{
		// a malformed value passed to a constructor gives 0
		bignumber	b("bogus");
		test("constructor with malformed decimal: is zero",
								b.isZero());
		test("constructor with malformed decimal: error",
			b.getError()==BIGNUMBER_ERROR_INVALID_FORMAT);
	}
	{
		bignumber	b((int64_t)5);
		test("setValue base 2: fails",!b.setValue("101",2));
		test("setValue base 2: error",
			b.getError()==BIGNUMBER_ERROR_INVALID_BASE);
		test("setValue base 2: instance is zero",b.isZero());
		b.setValue((int64_t)5);
		test("setValue base 8: fails",!b.setValue("17",8));
		test("setValue base 8: error",
			b.getError()==BIGNUMBER_ERROR_INVALID_BASE);
		b.setValue((int64_t)5);
		test("getString base 2: NULL",b.getString(2)==NULL);
		test("getString base 2: error",
			b.getError()==BIGNUMBER_ERROR_INVALID_BASE);
		test("getString base 10 after: succeeds",decis(&b,"5"));
		test("getString base 10 after: error reset",
			b.getError()==BIGNUMBER_ERROR_SUCCESS);
	}


	// comparison
	{
		bignumber	five((int64_t)5);
		bignumber	three((int64_t)3);
		bignumber	negfive((int64_t)-5);
		bignumber	negthree((int64_t)-3);
		bignumber	zero;
		bignumber	fivetoo((int64_t)5);

		test("compare: 5 vs 3",five.compare(three)==1);
		test("compare: 3 vs 5",three.compare(five)==-1);
		test("compare: 5 vs 5",five.compare(fivetoo)==0);
		test("compare: -5 vs -3",negfive.compare(negthree)==-1);
		test("compare: -3 vs -5",negthree.compare(negfive)==1);
		test("compare: -5 vs 3",negfive.compare(three)==-1);
		test("compare: 3 vs -5",three.compare(negfive)==1);
		test("compare: 0 vs 5",zero.compare(five)==-1);
		test("compare: 0 vs -5",zero.compare(negfive)==1);
		test("compare: 0 vs 0",zero.compare(bignumber())==0);

		test("operator==: 5==5",five==fivetoo);
		test("operator==: !(5==3)",!(five==three));
		test("operator==: !(5==-5)",!(five==negfive));
		test("operator!=: 5!=3",five!=three);
		test("operator!=: !(5!=5)",!(five!=fivetoo));

		test("operator<: 3<5",three<five);
		test("operator<: -5<3",negfive<three);
		test("operator<: -5<-3",negfive<negthree);
		test("operator<: !(5<5)",!(five<five));
		test("operator<: !(5<3)",!(five<three));

		test("operator>: 5>3",five>three);
		test("operator>: 3>-5",three>negfive);
		test("operator>: -3>-5",negthree>negfive);
		test("operator>: !(5>5)",!(five>five));

		test("operator<=: 3<=5",three<=five);
		test("operator<=: 5<=5",five<=fivetoo);
		test("operator<=: !(5<=3)",!(five<=three));

		test("operator>=: 5>=3",five>=three);
		test("operator>=: 5>=5",five>=fivetoo);
		test("operator>=: !(3>=5)",!(three>=five));

		test("operator<: 0<5",zero<five);
		test("operator>: 0>-5",zero>negfive);
	}
	{
		// large values that differ only in the low bytes
		bignumber	a(bigdec);
		bignumber	b(bigdec);
		b.add(bignumber((int64_t)1));
		test("compare: large values differing by 1",a.compare(b)==-1);
		test("compare: large values differing by 1, reversed",
							b.compare(a)==1);
		bignumber	c(bigdec);
		test("compare: equal large values",a.compare(c)==0);
	}


	// addition
	{
		bignumber	b((int64_t)5);
		test("add: 5+3",b.add(bignumber((int64_t)3)) &&
							decis(&b,"8"));
		b.setValue((int64_t)-5);
		test("add: -5+3",b.add(bignumber((int64_t)3)) &&
							decis(&b,"-2"));
		b.setValue((int64_t)5);
		test("add: 5+-3",b.add(bignumber((int64_t)-3)) &&
							decis(&b,"2"));
		b.setValue((int64_t)-5);
		test("add: -5+-3",b.add(bignumber((int64_t)-3)) &&
							decis(&b,"-8"));
		b.setValue((int64_t)-5);
		test("add: -5+5 is zero",b.add(bignumber((int64_t)5)) &&
							b.isZero());
	}
	{
		bignumber	a((int64_t)5);
		bignumber	c=a+bignumber((int64_t)3);
		test("operator+: 5+3",decis(&c,"8"));
		test("operator+: left operand unmodified",decis(&a,"5"));
		a+=bignumber((int64_t)-9);
		test("operator+=: 5+-9",decis(&a,"-4"));
	}
	{
		// carry across a byte boundary
		bignumber	a((uint32_t)0xFFFFFF);
		a+=bignumber((int64_t)1);
		test("operator+=: 0xFFFFFF+1",decis(&a,"16777216"));
		test("operator+=: 0xFFFFFF+1 magnitude size",
						a.getMagnitudeSize()==4);
	}
	{
		bignumber	a(bigdec);
		bignumber	c=a+bignumber(bigdec2);
		test("operator+: large values",
			decis(&c,"123456789111111111011111111100"));
	}
	{
		bignumber	a(u64max);
		bignumber	c=a+bignumber((int64_t)1);
		test("operator+: uint64 max + 1",
					decis(&c,"18446744073709551616"));
		test("operator+: uint64 max + 1 bit count",
					c.getBitCount()==65);
	}


	// subtraction
	{
		bignumber	b((int64_t)5);
		test("subtract: 5-3",b.subtract(bignumber((int64_t)3)) &&
							decis(&b,"2"));
		b.setValue((int64_t)-5);
		test("subtract: -5-3",b.subtract(bignumber((int64_t)3)) &&
							decis(&b,"-8"));
		b.setValue((int64_t)5);
		test("subtract: 5--3",b.subtract(bignumber((int64_t)-3)) &&
							decis(&b,"8"));
		b.setValue((int64_t)-5);
		test("subtract: -5--3",b.subtract(bignumber((int64_t)-3)) &&
							decis(&b,"-2"));
		b.setValue((int64_t)3);
		test("subtract: 3-5",b.subtract(bignumber((int64_t)5)) &&
							decis(&b,"-2"));
		b.setValue((int64_t)5);
		test("subtract: 5-5 is zero",
			b.subtract(bignumber((int64_t)5)) && b.isZero());
	}
	{
		bignumber	a((int64_t)5);
		bignumber	c=a-bignumber((int64_t)8);
		test("operator-: 5-8",decis(&c,"-3"));
		test("operator-: left operand unmodified",decis(&a,"5"));
		a-=bignumber((int64_t)-5);
		test("operator-=: 5--5",decis(&a,"10"));
	}
	{
		// borrow across a byte boundary
		bignumber	a((uint32_t)0x1000000);
		a-=bignumber((int64_t)1);
		test("operator-=: 0x1000000-1",decis(&a,"16777215"));
		test("operator-=: 0x1000000-1 magnitude size",
						a.getMagnitudeSize()==3);
	}
	{
		bignumber	a(bigdec);
		bignumber	c=a-bignumber(bigdec2);
		test("operator-: large values",
			decis(&c,"123456788913580246791358024680"));
		bignumber	d=bignumber(bigdec2)-a;
		test("operator-: large values, negative result",
			decis(&d,"-123456788913580246791358024680"));
	}


	// multiplication
	{
		bignumber	b((int64_t)6);
		test("multiply: 6*7",b.multiply(bignumber((int64_t)7)) &&
							decis(&b,"42"));
		b.setValue((int64_t)-6);
		test("multiply: -6*7",b.multiply(bignumber((int64_t)7)) &&
							decis(&b,"-42"));
		b.setValue((int64_t)6);
		test("multiply: 6*-7",b.multiply(bignumber((int64_t)-7)) &&
							decis(&b,"-42"));
		b.setValue((int64_t)-6);
		test("multiply: -6*-7",b.multiply(bignumber((int64_t)-7)) &&
							decis(&b,"42"));
		b.setValue((int64_t)-6);
		test("multiply: -6*0 is zero",
			b.multiply(bignumber((int64_t)0)) && b.isZero());
		test("multiply: -6*0 not negative",!b.isNegative());
	}
	{
		bignumber	a((int64_t)6);
		bignumber	c=a*bignumber((int64_t)-7);
		test("operator*: 6*-7",decis(&c,"-42"));
		test("operator*: left operand unmodified",decis(&a,"6"));
		a*=bignumber((int64_t)3);
		test("operator*=: 6*3",decis(&a,"18"));
	}
	{
		bignumber	a(bigdec);
		bignumber	c=a*bignumber(bigdec2);
		test("operator*: large values",decis(&c,
	"12193263113702179522496570642237463801111263526900"));
		bignumber	d=a*bignumber("-98765432109876543210");
		test("operator*: large values, negative result",decis(&d,
	"-12193263113702179522496570642237463801111263526900"));
	}


	// division and modulo, including the sign rules
	{
		bignumber	q((int64_t)7);
		bignumber	r;
		test("divide: 7/2 succeeds",
			q.divide(bignumber((int64_t)2),&r));
		test("divide: 7/2 quotient",decis(&q,"3"));
		test("divide: 7/2 remainder",decis(&r,"1"));
	}
	{
		bignumber	q((int64_t)-7);
		bignumber	r;
		test("divide: -7/2 succeeds",
			q.divide(bignumber((int64_t)2),&r));
		test("divide: -7/2 quotient",decis(&q,"-3"));
		test("divide: -7/2 remainder",decis(&r,"-1"));
	}
	{
		bignumber	q((int64_t)7);
		bignumber	r;
		test("divide: 7/-2 succeeds",
			q.divide(bignumber((int64_t)-2),&r));
		test("divide: 7/-2 quotient",decis(&q,"-3"));
		test("divide: 7/-2 remainder",decis(&r,"1"));
	}
	{
		bignumber	q((int64_t)-7);
		bignumber	r;
		test("divide: -7/-2 succeeds",
			q.divide(bignumber((int64_t)-2),&r));
		test("divide: -7/-2 quotient",decis(&q,"3"));
		test("divide: -7/-2 remainder",decis(&r,"-1"));
	}
	{
		// the same four cases through modulo()
		bignumber	m((int64_t)7);
		test("modulo: 7%2",m.modulo(bignumber((int64_t)2)) &&
							decis(&m,"1"));
		m.setValue((int64_t)-7);
		test("modulo: -7%2",m.modulo(bignumber((int64_t)2)) &&
							decis(&m,"-1"));
		m.setValue((int64_t)7);
		test("modulo: 7%-2",m.modulo(bignumber((int64_t)-2)) &&
							decis(&m,"1"));
		m.setValue((int64_t)-7);
		test("modulo: -7%-2",m.modulo(bignumber((int64_t)-2)) &&
							decis(&m,"-1"));
	}
	{
		// and through the operators
		bignumber	a((int64_t)-7);
		bignumber	q=a/bignumber((int64_t)2);
		test("operator/: -7/2",decis(&q,"-3"));
		bignumber	m=a%bignumber((int64_t)2);
		test("operator%: -7%2",decis(&m,"-1"));
		test("operator/: left operand unmodified",decis(&a,"-7"));
		a/=bignumber((int64_t)2);
		test("operator/=: -7/2",decis(&a,"-3"));
		bignumber	b((int64_t)7);
		b%=bignumber((int64_t)-2);
		test("operator%=: 7%-2",decis(&b,"1"));
	}
	{
		// exact division leaves no remainder
		bignumber	q((int64_t)-42);
		bignumber	r((int64_t)99);
		test("divide: -42/7 succeeds",
			q.divide(bignumber((int64_t)7),&r));
		test("divide: -42/7 quotient",decis(&q,"-6"));
		test("divide: -42/7 remainder is zero",r.isZero());
	}
	{
		// divide() without a remainder, and a NULL remainder
		bignumber	q((int64_t)-7);
		test("divide: no remainder argument",
			q.divide(bignumber((int64_t)2)) && decis(&q,"-3"));
		q.setValue((int64_t)-7);
		test("divide: NULL remainder argument",
			q.divide(bignumber((int64_t)2),NULL) &&
							decis(&q,"-3"));
	}
	{
		// a divisor larger than the dividend
		bignumber	q((int64_t)3);
		bignumber	r;
		test("divide: 3/7 succeeds",
			q.divide(bignumber((int64_t)7),&r));
		test("divide: 3/7 quotient is zero",q.isZero());
		test("divide: 3/7 remainder",decis(&r,"3"));
	}
	{
		// large values
		bignumber	q(bigdec);
		bignumber	r;
		test("divide: large values succeeds",
			q.divide(bignumber(bigdec2),&r));
		test("divide: large values quotient",decis(&q,"1249999988"));
		test("divide: large values remainder",
			decis(&r,"60185185207253086410"));
	}
	{
		bignumber	q("-123456789012345678901234567890");
		bignumber	r;
		test("divide: large negative dividend succeeds",
			q.divide(bignumber(bigdec2),&r));
		test("divide: large negative dividend quotient",
			decis(&q,"-1249999988"));
		test("divide: large negative dividend remainder",
			decis(&r,"-60185185207253086410"));
	}
	{
		// (a/b)*b+(a%b)==a, for mixed signs
		bignumber	a(bigdec);
		bignumber	b("-98765432109876543210");
		bignumber	q=a/b;
		bignumber	m=a%b;
		bignumber	back=q*b;
		back+=m;
		test("division round trip: (a/b)*b+(a%b)==a, a>0, b<0",
								back==a);

		bignumber	a2("-123456789012345678901234567890");
		bignumber	b2("-98765432109876543210");
		bignumber	q2=a2/b2;
		bignumber	m2=a2%b2;
		bignumber	back2=q2*b2;
		back2+=m2;
		test("division round trip: (a/b)*b+(a%b)==a, a<0, b<0",
								back2==a2);
	}
	{
		// division by zero
		bignumber	q((int64_t)7);
		bignumber	r((int64_t)99);
		bignumber	zero;
		// note that getString() resets the error, so getError() is
		// checked before the value is rendered
		test("divide by zero: fails",!q.divide(zero));
		test("divide by zero: error",
			q.getError()==BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		test("divide by zero: instance unmodified",decis(&q,"7"));
		test("divide by zero with remainder: fails",
						!q.divide(zero,&r));
		test("divide by zero with remainder: instance unmodified",
							decis(&q,"7"));
		test("divide by zero with remainder: remainder unmodified",
							decis(&r,"99"));
		test("modulo by zero: fails",!q.modulo(zero));
		test("modulo by zero: error",
			q.getError()==BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		test("modulo by zero: instance unmodified",decis(&q,"7"));

		bignumber	d=q/zero;
		test("operator/ by zero: error",
			q.getError()==BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		test("operator/ by zero: result is zero",d.isZero());
		bignumber	m=q%zero;
		test("operator% by zero: error",
			q.getError()==BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		test("operator% by zero: result is zero",m.isZero());

		q/=zero;
		test("operator/= by zero: instance unmodified",
							decis(&q,"7"));
		q%=zero;
		test("operator%= by zero: instance unmodified",
							decis(&q,"7"));
		test("divide after divide by zero: error reset",
			q.divide(bignumber((int64_t)7)) &&
			q.getError()==BIGNUMBER_ERROR_SUCCESS);
	}


	// negation and absolute value
	{
		bignumber	b((int64_t)5);
		test("negate: 5",b.negate() && decis(&b,"-5"));
		test("negate: -5",b.negate() && decis(&b,"5"));
		b.setValue((int64_t)0);
		test("negate: 0 stays 0",b.negate() && b.isZero());
		test("negate: 0 is not negative",!b.isNegative());
	}
	{
		bignumber	b((int64_t)5);
		bignumber	n=-b;
		test("unary operator-: 5",decis(&n,"-5"));
		test("unary operator-: instance unmodified",decis(&b,"5"));
		bignumber	p=-n;
		test("unary operator-: -5",decis(&p,"5"));
		bignumber	z;
		bignumber	nz=-z;
		test("unary operator-: 0 stays 0",nz.isZero());
		bignumber	big(bigdec);
		bignumber	nbig=-big;
		test("unary operator-: large value",
			decis(&nbig,"-123456789012345678901234567890"));
	}
	{
		bignumber	b((int64_t)-5);
		test("absoluteValue: -5",b.absoluteValue() && decis(&b,"5"));
		test("absoluteValue: 5",b.absoluteValue() && decis(&b,"5"));
		b.setValue((int64_t)0);
		test("absoluteValue: 0",b.absoluteValue() && b.isZero());
		b.setValue("-123456789012345678901234567890");
		test("absoluteValue: large negative",
			b.absoluteValue() && decis(&b,bigdec));
	}


	// increment and decrement
	{
		bignumber	b((int64_t)5);
		bignumber	r=++b;
		test("prefix ++: instance incremented",decis(&b,"6"));
		test("prefix ++: returns new value",decis(&r,"6"));
	}
	{
		bignumber	b((int64_t)5);
		bignumber	r=b++;
		test("postfix ++: instance incremented",decis(&b,"6"));
		test("postfix ++: returns old value",decis(&r,"5"));
	}
	{
		bignumber	b((int64_t)5);
		bignumber	r=--b;
		test("prefix --: instance decremented",decis(&b,"4"));
		test("prefix --: returns new value",decis(&r,"4"));
	}
	{
		bignumber	b((int64_t)5);
		bignumber	r=b--;
		test("postfix --: instance decremented",decis(&b,"4"));
		test("postfix --: returns old value",decis(&r,"5"));
	}
	{
		// across a byte boundary
		bignumber	b((uint32_t)0xFFFFFF);
		++b;
		test("prefix ++: carry across byte boundary",
						decis(&b,"16777216"));
		--b;
		test("prefix --: borrow across byte boundary",
						decis(&b,"16777215"));
	}
	{
		// across zero
		bignumber	b;
		--b;
		test("prefix --: 0 becomes -1",decis(&b,"-1"));
		++b;
		test("prefix ++: -1 becomes 0",b.isZero());
		--b;
		--b;
		test("prefix --: -1 becomes -2",decis(&b,"-2"));
		++b;
		++b;
		test("prefix ++: -2 back to 0",b.isZero());
	}
	{
		bignumber	b(u64max);
		++b;
		test("prefix ++: uint64 max",
					decis(&b,"18446744073709551616"));
		--b;
		test("prefix --: back to uint64 max",
					decis(&b,"18446744073709551615"));
	}


	// bitwise, using the examples the header documents
	{
		bignumber	b((int64_t)-2);
		test("bitwiseAnd: -2&3 is 2",
			b.bitwiseAnd(bignumber((int64_t)3)) &&
							decis(&b,"2"));
		b.setValue((int64_t)-2);
		test("bitwiseOr: -2|3 is -1",
			b.bitwiseOr(bignumber((int64_t)3)) &&
							decis(&b,"-1"));
		b.setValue((int64_t)-2);
		test("bitwiseXor: -2^3 is -3",
			b.bitwiseXor(bignumber((int64_t)3)) &&
							decis(&b,"-3"));
		b.setValue((int64_t)0);
		test("bitwiseNot: ~0 is -1",b.bitwiseNot() &&
							decis(&b,"-1"));
		b.setValue((int64_t)5);
		test("bitwiseNot: ~5 is -6",b.bitwiseNot() &&
							decis(&b,"-6"));
		b.setValue((int64_t)-6);
		test("bitwiseNot: ~-6 is 5",b.bitwiseNot() &&
							decis(&b,"5"));
	}
	{
		bignumber	a((int64_t)-2);
		bignumber	three((int64_t)3);
		bignumber	c=a&three;
		test("operator&: -2&3 is 2",decis(&c,"2"));
		test("operator&: left operand unmodified",decis(&a,"-2"));
		bignumber	d=a|three;
		test("operator|: -2|3 is -1",decis(&d,"-1"));
		bignumber	e=a^three;
		test("operator^: -2^3 is -3",decis(&e,"-3"));
		bignumber	zero;
		bignumber	f=~zero;
		test("operator~: ~0 is -1",decis(&f,"-1"));
		test("operator~: instance unmodified",zero.isZero());
		bignumber	five((int64_t)5);
		bignumber	g=~five;
		test("operator~: ~5 is -6",decis(&g,"-6"));
	}
	{
		// positive operands match native int behavior
		bignumber	a((int64_t)5);
		bignumber	three((int64_t)3);
		bignumber	c=a&three;
		test("operator&: 5&3 is 1",decis(&c,"1"));
		bignumber	d=a|three;
		test("operator|: 5|3 is 7",decis(&d,"7"));
		bignumber	e=a^three;
		test("operator^: 5^3 is 6",decis(&e,"6"));
		test("operator^: x^x is 0",(a^bignumber((int64_t)5)).isZero());
	}
	{
		// both operands negative
		bignumber	a((int64_t)-5);
		bignumber	b((int64_t)-3);
		bignumber	c=a&b;
		test("operator&: -5&-3 is -7",decis(&c,"-7"));
		bignumber	d=a|b;
		test("operator|: -5|-3 is -1",decis(&d,"-1"));
		bignumber	e=a^b;
		test("operator^: -5^-3 is 6",decis(&e,"6"));
	}
	{
		// compound forms
		bignumber	b((int64_t)-2);
		b&=bignumber((int64_t)3);
		test("operator&=: -2&3",decis(&b,"2"));
		b.setValue((int64_t)-2);
		b|=bignumber((int64_t)3);
		test("operator|=: -2|3",decis(&b,"-1"));
		b.setValue((int64_t)-2);
		b^=bignumber((int64_t)3);
		test("operator^=: -2^3",decis(&b,"-3"));
	}
	{
		// multi-byte operands
		bignumber	a("281474976710655");	// 0xFFFFFFFFFFFF
		bignumber	b("18446744073709551360");	// ...FF00
		bignumber	c=a&b;
		test("operator&: multi-byte",decis(&c,"281474976710400"));
		bignumber	d=a|b;
		test("operator|: multi-byte",
					decis(&d,"18446744073709551615"));
	}


	// shifts
	{
		bignumber	b((int64_t)1);
		test("leftShift: 1<<64",b.leftShift(64) &&
					decis(&b,"18446744073709551616"));
		test("leftShift: 1<<64 bit count",b.getBitCount()==65);
		test("rightShift: back to 1",b.rightShift(64) &&
							decis(&b,"1"));
		test("leftShift: by 0",b.leftShift(0) && decis(&b,"1"));
		test("rightShift: by 0",b.rightShift(0) && decis(&b,"1"));
	}
	{
		bignumber	b((int64_t)-5);
		test("leftShift: -5<<3 preserves sign",b.leftShift(3) &&
							decis(&b,"-40"));
		test("rightShift: -40>>3",b.rightShift(3) &&
							decis(&b,"-5"));
	}
	{
		// right shift truncates toward zero, not negative infinity
		bignumber	b((int64_t)-7);
		test("rightShift: -7>>1 is -3, not -4",b.rightShift(1) &&
							decis(&b,"-3"));
		bignumber	c((int64_t)-7);
		bignumber	d=c>>1;
		test("operator>>: -7>>1 is -3, not -4",decis(&d,"-3"));
		test("operator>>: instance unmodified",decis(&c,"-7"));
		bignumber	e((int64_t)7);
		bignumber	f=e>>1;
		test("operator>>: 7>>1 is 3",decis(&f,"3"));
	}
	{
		// shifting far enough right gives 0, even for a negative
		bignumber	b((int64_t)-12345);
		test("rightShift: negative shifted away",b.rightShift(64) &&
								b.isZero());
		test("rightShift: negative shifted away is not -1",
							!b.isNegative());
		bignumber	c((int64_t)12345);
		test("rightShift: positive shifted away",c.rightShift(1000) &&
								c.isZero());
	}
	{
		bignumber	a((int64_t)3);
		bignumber	b=a<<10;
		test("operator<<: 3<<10",decis(&b,"3072"));
		test("operator<<: instance unmodified",decis(&a,"3"));
		a<<=10;
		test("operator<<=: 3<<10",decis(&a,"3072"));
		a>>=10;
		test("operator>>=: back to 3",decis(&a,"3"));
	}
	{
		// large values
		bignumber	a(bigdec);
		bignumber	b=a<<70;
		test("operator<<: large value <<70",decis(&b,
	"145752050628652680975897013633443949312730317455360"));
		bignumber	c=a>>70;
		test("operator>>: large value >>70",decis(&c,"104571967"));
		bignumber	d("-123456789012345678901234567890");
		bignumber	e=d>>3;
		test("operator>>: large negative value >>3",
			decis(&e,"-15432098626543209862654320986"));
	}
	{
		// a left shift is a multiplication by a power of 2
		bignumber	a(bigdec);
		bignumber	b=a<<8;
		bignumber	c=a*bignumber((int64_t)256);
		test("operator<<: <<8 matches *256",b==c);
	}
	{
		// shifting zero
		bignumber	b;
		test("leftShift: 0<<100",b.leftShift(100) && b.isZero());
		test("rightShift: 0>>100",b.rightShift(100) && b.isZero());
	}

	return 0;
}
