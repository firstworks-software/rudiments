// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("charstring");

	// zero
	stdoutput.printf("zero...\n");
	char	s[50];
	charstring::zero(s,50);
	bool	zeros=true;
	for (int zi=0; zi<50; zi++) {
		if (s[zi]!=0) {
			zeros=false;
		}
	}
	test("zeros",zeros);
	stdoutput.printf("\n");

	// append
	stdoutput.printf("append...\n");
	charstring::append(s,"hello ");
	charstring::append(s,"there ");
	charstring::append(s,"dave !!!",4);
	test("append",!charstring::compare(s,"hello there dave"));
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	charstring::copy(s,"what's ");
	test("append",!charstring::compare(s,"what's "));
	charstring::copy(s,"up !!!",2);
	test("append",!charstring::compare(s,"upat's "));
	charstring::copy(s,2," !!!");
	test("append",!charstring::compare(s,"up !!!"));
	charstring::copy(s,6,"!!!!!!",1);
	test("append",!charstring::compare(s,"up !!!!"));
	stdoutput.printf("\n");


	// compare
	stdoutput.printf("compare...\n");
	charstring::zero(s,50);
	charstring::append(s,"hello");
	test("compare",!charstring::compare(s,"hello"));
	test("compare",!charstring::compare(s,"hello",3));
	test("compare",!charstring::compareIgnoringCase(s,"HELLO"));
	test("compare",!charstring::compareIgnoringCase(s,"HELLO",3));
	stdoutput.printf("\n");


	// findFirst
	stdoutput.printf("findFirst...\n");
	test("findFirst",charstring::findFirst(s,"llo"));
	test("findFirstIgnoringCase",
			charstring::findFirstIgnoringCase(s,"LLO"));
	test("findFirstOrEnd",charstring::findFirstOrEnd(s,"llo"));
	test("findFirstOrEnd",
		charstring::findFirstOrEnd(s,"blah")==
						s+charstring::length(s));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,"LLO"));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,"BLAH")==
						s+charstring::length(s));
	test("findFirst",charstring::findFirst(s,'l'));
	test("findFirstIgnoringCase",charstring::findFirstIgnoringCase(s,'L'));
	test("findFirstOrEnd",
		charstring::findFirstOrEnd(s,'z')==s+charstring::length(s));
	test("findFirstOrEnd",charstring::findFirstOrEnd(s,'l'));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,'Z')==
						s+charstring::length(s));
	test("findFirstOrEndIgnoringCase",
			charstring::findFirstOrEndIgnoringCase(s,'l'));
	stdoutput.printf("\n");

	// findLast
	test("findLast",charstring::findLast(s,"llo"));
	test("findLastIgnoringCase",
			charstring::findLastIgnoringCase(s,"LLO"));
	test("findLast",charstring::findLast(s,'l'));
	test("findLastIgnoringCase",charstring::findLastIgnoringCase(s,'L'));
	stdoutput.printf("\n");

	// contains
	stdoutput.printf("contains...\n");
	test("contains",charstring::contains(s,"llo"));
	test("containsIgnoringCase",charstring::containsIgnoringCase(s,"LLO"));
	stdoutput.printf("\n");


	// startsWith/endsWith
	stdoutput.printf("startsWith/endsWith...\n");
	test("startsWith",charstring::startsWith("hello","he"));
	test("startsWith",charstring::startsWith("hello","hello"));
	test("startsWith",!charstring::startsWith("hello","el"));
	test("startsWith",!charstring::startsWith("hello","helloThere"));
	test("endsWith",charstring::endsWith("hello","lo"));
	test("endsWith",charstring::endsWith("hello","hello"));
	test("endsWith",!charstring::endsWith("hello","ll"));
	test("endsWith",!charstring::endsWith("hello","helloThere"));
	stdoutput.printf("\n");


	// duplicate
	stdoutput.printf("duplicate/subString...\n");
	char	*hellothere=charstring::duplicate(s);
	test("duplicate",!charstring::compare(hellothere,"hello"));
	char	*ell=charstring::subString(hellothere,1,3);
	test("subString",!charstring::compare(ell,"ell"));
	char	*llo=charstring::subString(hellothere,2,4);
	test("subString",!charstring::compare(llo,"llo"));
	delete[] hellothere;
	delete[] ell;
	delete[] llo;
	stdoutput.printf("\n");


	// printf
	stdoutput.printf("printf...\n");
	char	buffer[100];
	ssize_t	size=charstring::printf(buffer,20,"%c%d%s",'a',123,"hello");
	test("size",size==9);
	test("contents",!charstring::compare(buffer,"a123hello"));
	char	*buf=NULL;
	size=charstring::printf(&buf,"%c%d%s",'a',123,"hello");
	test("size",size==9);
	test("contents",!charstring::compare(buffer,"a123hello"));
	delete[] buf;
	stdoutput.printf("\n");


	// split
	stdoutput.printf("split...\n");
	char		**list;
	uint64_t	listlength;
	charstring::split("hello||hi||bye||goodbye","||",false,
						&list,&listlength);
	test("length",listlength==4);
	test("list[0]",!charstring::compare(list[0],"hello"));
	test("list[1]",!charstring::compare(list[1],"hi"));
	test("list[2]",!charstring::compare(list[2],"bye"));
	test("list[3]",!charstring::compare(list[3],"goodbye"));
	uint64_t i;
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	test("length",listlength==5);
	test("list[0]",!charstring::compare(list[0],"hello"));
	test("list[1]",!charstring::compare(list[1],"hi"));
	test("list[2]",!charstring::compare(list[2],"bye"));
	test("list[3]",!charstring::compare(list[3],"goodbye"));
	test("list[4]",!charstring::compare(list[4],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||hello||hi||bye||goodbye||","||",false,
						&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],"hello"));
	test("list[2]",!charstring::compare(list[2],"hi"));
	test("list[3]",!charstring::compare(list[3],"bye"));
	test("list[4]",!charstring::compare(list[4],"goodbye"));
	test("list[5]",!charstring::compare(list[5],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||hello||||hi||||bye||||goodbye||||","||",false,
							&list,&listlength);
	test("length",listlength==11);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],""));
	test("list[2]",!charstring::compare(list[2],"hello"));
	test("list[3]",!charstring::compare(list[3],""));
	test("list[4]",!charstring::compare(list[4],"hi"));
	test("list[5]",!charstring::compare(list[5],""));
	test("list[6]",!charstring::compare(list[6],"bye"));
	test("list[7]",!charstring::compare(list[7],""));
	test("list[8]",!charstring::compare(list[8],"goodbye"));
	test("list[9]",!charstring::compare(list[9],""));
	test("list[10]",!charstring::compare(list[10],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("||||||||||","||",false,&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!charstring::compare(list[0],""));
	test("list[1]",!charstring::compare(list[1],""));
	test("list[2]",!charstring::compare(list[2],""));
	test("list[3]",!charstring::compare(list[3],""));
	test("list[4]",!charstring::compare(list[4],""));
	test("list[5]",!charstring::compare(list[5],""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("http://www.firstworks.com/application/app.cgi/skin/module/template.html","/",true,&list,&listlength);
	test("length",listlength==7);
	test("list[0]",!charstring::compare(list[0],"http:"));
	test("list[1]",!charstring::compare(list[1],"www.firstworks.com"));
	test("list[2]",!charstring::compare(list[2],"application"));
	test("list[3]",!charstring::compare(list[3],"app.cgi"));
	test("list[4]",!charstring::compare(list[4],"skin"));
	test("list[5]",!charstring::compare(list[5],"module"));
	test("list[6]",!charstring::compare(list[6],"template.html"));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	charstring::split("1.2.3.4.5.6",".",false,NULL,&listlength);
	test("length",listlength==6);

	// this test just shouldn't crash
	charstring::split("1.2.3.4.5.6",".",false,NULL,NULL);
	stdoutput.printf("\n");

	// before, between, after
	stdoutput.printf("before/between/after...\n");
	char	*bef=charstring::before("one two three four five"," two");
	test("before",!charstring::compare(bef,"one"));
	char	*btw=charstring::between("one two three four five",
							"two "," four");
	test("between",!charstring::compare(btw,"three"));
	char	*aft=charstring::after("one two three four five","four ");
	test("after",!charstring::compare(aft,"five"));
	stdoutput.printf("\n");


	// escape/unescape
	stdoutput.printf("escape/unescape...\n");
	char	str[]="hello'\"\\hello'\"\\";
	char	*escapedstr=charstring::escape(str,"\"'");
	test("escape",!charstring::compare(escapedstr,
					"hello\\'\\\"\\\\hello\\'\\\"\\\\"));
	char	*unescapedstr=charstring::unescape(escapedstr);
	test("unescape",!charstring::compare(unescapedstr,
					"hello'\"\\hello'\"\\"));
	delete[] unescapedstr;
	delete[] escapedstr;
	stdoutput.printf("\n");


	// from Thomas Hobbes's Leviathan...
	byte_t	hexstr1[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	byte_t	hexstr2[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure";
	byte_t	hexstr3[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasur";
	byte_t	hexstr4[]="Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasu";

	// base64
	stdoutput.printf("base64...\n");
	char	*encoded1=charstring::base64Encode(hexstr1);
	byte_t	*decoded1=charstring::base64Decode(encoded1);
	test("base64 test 1",
		!charstring::compare((char *)decoded1,(char *)hexstr1));
	delete[] encoded1;
	delete[] decoded1;

	char	*encoded2=charstring::base64Encode(hexstr2);
	byte_t	*decoded2=charstring::base64Decode(encoded2);
	test("base64 test 2",
		!charstring::compare((char *)decoded2,(char *)hexstr2));
	delete[] encoded2;
	delete[] decoded2;

	char	*encoded3=charstring::base64Encode(hexstr3);
	byte_t	*decoded3=charstring::base64Decode(encoded3);
	test("base64 test 3",
		!charstring::compare((char *)decoded3,(char *)hexstr3));
	delete[] encoded3;
	delete[] decoded3;

	char	*encoded4=charstring::base64Encode(hexstr4);
	byte_t	*decoded4=charstring::base64Decode(encoded4);
	test("base64 test 4",
		!charstring::compare((char *)decoded4,(char *)hexstr4));
	delete[] encoded4;
	delete[] decoded4;

	byte_t		bytes[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	char		*encodedbytes;
	uint64_t	encodedlen;
	byte_t		*decodedbytes;
	uint64_t	decodedlen;
	charstring::base64Encode(bytes,6,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 5",
		!bytestring::compare(decodedbytes,bytes,6));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,5,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 6",
		!bytestring::compare(decodedbytes,bytes,5));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,4,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 7",
		!bytestring::compare(decodedbytes,bytes,4));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,3,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 8",
		!bytestring::compare(decodedbytes,bytes,3));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,2,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 9",
		!bytestring::compare(decodedbytes,bytes,2));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::base64Encode(bytes,1,&encodedbytes,&encodedlen);
	charstring::base64Decode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("base64 test 10",
		!bytestring::compare(decodedbytes,bytes,1));
	delete[] encodedbytes;
	delete[] decodedbytes;
	stdoutput.printf("\n");


	// hex
	stdoutput.printf("hex...\n");
	encoded1=charstring::hexEncode(hexstr1);
	decoded1=charstring::hexDecode(encoded1);
	test("hex test 1",
		!charstring::compare((char *)decoded1,(char *)hexstr1));
	delete[] encoded1;
	delete[] decoded1;

	encoded2=charstring::hexEncode(hexstr2);
	decoded2=charstring::hexDecode(encoded2);
	test("hex test 2",
		!charstring::compare((char *)decoded2,(char *)hexstr2));
	delete[] encoded2;
	delete[] decoded2;

	encoded3=charstring::hexEncode(hexstr3);
	decoded3=charstring::hexDecode(encoded3);
	test("hex test 3",
		!charstring::compare((char *)decoded3,(char *)hexstr3));
	delete[] encoded3;
	delete[] decoded3;

	encoded4=charstring::hexEncode(hexstr4);
	decoded4=charstring::hexDecode(encoded4);
	test("hex test 4",
		!charstring::compare((char *)decoded4,(char *)hexstr4));
	delete[] encoded4;
	delete[] decoded4;

	charstring::hexEncode(bytes,6,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 5",
		!bytestring::compare(decodedbytes,bytes,6));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::hexEncode(bytes,5,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 6",
		!bytestring::compare(decodedbytes,bytes,5));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::hexEncode(bytes,4,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 7",
		!bytestring::compare(decodedbytes,bytes,4));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::hexEncode(bytes,3,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 8",
		!bytestring::compare(decodedbytes,bytes,3));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::hexEncode(bytes,2,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 9",
		!bytestring::compare(decodedbytes,bytes,2));
	delete[] encodedbytes;
	delete[] decodedbytes;

	charstring::hexEncode(bytes,1,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 10",
		!bytestring::compare(decodedbytes,bytes,1));
	delete[] encodedbytes;
	delete[] decodedbytes;
	stdoutput.printf("\n");


	// http escape the buffer
	stdoutput.printf("http-escape...\n");
	const char	*original="!@#$%^&*()hello-+£";
	char    *escbuffer=charstring::httpEscape(original);
	test("escaped",!charstring::compare(escbuffer,
				"!%40%23$%25%5E%26*()hello-%2B%C2%A3"));
	char	*unescbuffer=charstring::httpUnescape(escbuffer);
	test("unescaped",!charstring::compare(unescbuffer,original));
	delete[] escbuffer;
	delete[] unescbuffer;
	stdoutput.printf("\n");


	// quoted printable
	stdoutput.printf("quoted printable...\n");
	bytebuffer	bb;
	stringbuffer	sb;
	uint16_t 	index=0;
	for (uint16_t i=0; i<256; i++) {
		bb.append((byte_t)i);
		if (index>=77) {
			sb.append("=\r\n");
			index=0;
		}
		if ((byte_t)i<' ' ||
			(byte_t)i>'~' ||
			(byte_t)i=='=') {
			sb.appendFormatted("=%02x",(byte_t)i);
			index+=3;
		} else {
			sb.append((byte_t)i);
			index++;
		}
	}
	char		*encoded=NULL;
	uint64_t	encodedlength=0;
	charstring::quotedPrintableEncode(bb.getBuffer(),bb.getSize(),
						&encoded,&encodedlength);
	test("encoded length",encodedlength==sb.getStringLength());
	test("encoded",!charstring::compare(encoded,sb.getString()));
	byte_t		*decoded=NULL;
	uint64_t	decodedlength=0;
	charstring::quotedPrintableDecode(encoded,encodedlength,
						&decoded,&decodedlength);
	test("decoded length",decodedlength==bb.getSize());
	test("decoded",!bytestring::compare(decoded,
					bb.getBuffer(),bb.getSize()));
	delete[] encoded;
	delete[] decoded;
	stdoutput.printf("\n");


	// sets
	stdoutput.printf("sets...\n");
	const char	*alphabet=
		"aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	test("lengthContainingSet",
		charstring::lengthContainingSet(
				alphabet,"gfedcba")==14);
	test("lengthNotContainingSet",
		charstring::lengthNotContainingSet(
				alphabet,"hijklmnopqrstuvwxyz")==14);
	test("findFirstOfSet",charstring::findFirstOfSet(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd",
			charstring::findFirstOfSetOrEnd(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd (end)",
			charstring::findFirstOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::length(alphabet)));
	test("findLastOfSetOrEnd",
			charstring::findLastOfSetOrEnd(alphabet,"abc")==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
			charstring::findLastOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::length(alphabet)));
	stdoutput.printf("\n");


	// trim
	stdoutput.printf("trim...\n");
	charstring::copy(buffer,"\r\n	     hello there buddy     	\r\n");
	charstring::rightTrim(buffer);
	test("rtrim",!charstring::compare(buffer,
				"\r\n	     hello there buddy"));
	charstring::leftTrim(buffer);
	test("ltrim",!charstring::compare(buffer,"hello there buddy"));
	stdoutput.printf("\n");


	// strip
	stdoutput.printf("strip...\n");
	charstring::copy(buffer,"   hello      there   buddy  ");
	charstring::strip(buffer,' ');
	test("strip",!charstring::compare(buffer,"hellotherebuddy"));
	charstring::copy(buffer,"   hello       there  buddy  ");
	charstring::strip(buffer,"   ");
	test("strip",!charstring::compare(buffer,"hello there  buddy  "));
	stdoutput.printf("\n");


	// justification
	stdoutput.printf("justification...\n");
	char	hello[16];
	charstring::copy(hello,"     hello     ");
	charstring::leftJustify(hello,15);
	test("left",!charstring::compare(hello,"hello          "));
	charstring::rightJustify(hello,15);
	test("right",!charstring::compare(hello,"          hello"));
	charstring::copy(hello,"hello          ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello," hello         ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"  hello        ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"   hello       ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"    hello      ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"     hello     ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"      hello    ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"       hello   ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"        hello  ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"         hello ");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	charstring::copy(hello,"          hello");
	charstring::center(hello,15);
	test("center",!charstring::compare(hello,"     hello     "));
	stdoutput.printf("\n");


	// case conversion
	stdoutput.printf("case conversion...\n");
	charstring::upper(buffer);
	test("upper",!charstring::compare(buffer,"HELLO THERE  BUDDY  "));
	charstring::lower(buffer);
	test("lower",!charstring::compare(buffer,"hello there  buddy  "));
	charstring::capitalize(buffer);
	test("caps",!charstring::compare(buffer,"Hello There  Buddy  "));
	stdoutput.printf("\n");
	

	// numbers
	stdoutput.printf("number...\n");
	test("isNumber: -100.5",charstring::isNumber("-100.5"));
	test("isNumber: -100.5.10",!charstring::isNumber("-100.5.10"));
	test("isInteger: -100",charstring::isInteger("-100"));
	test("isInteger: -100.5",!charstring::isInteger("-100.5"));
	test("isInteger: -100.5.10",!charstring::isInteger("-100.5.10"));
	test("integerLength: 0",
			charstring::integerLength((int64_t)0)==1);
	test("integerLength: 1",
			charstring::integerLength((int64_t)1)==1);
	test("integerLength: 10",
			charstring::integerLength((int64_t)10)==2);
	test("integerLength: 100",
			charstring::integerLength((int64_t)100)==3);
	test("integerLength: 1000",
			charstring::integerLength((int64_t)1000)==4);
	test("integerLength: -1",
			charstring::integerLength((int64_t)-1)==2);
	test("integerLength: -10",
			charstring::integerLength((int64_t)-10)==3);
	test("integerLength: -100",
			charstring::integerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			charstring::integerLength((int64_t)-1000)==5);
	test("integerLength: 0",
			charstring::integerLength((uint64_t)0)==1);
	test("integerLength: 1",
			charstring::integerLength((uint64_t)1)==1);
	test("integerLength: 10",
			charstring::integerLength((uint64_t)10)==2);
	test("integerLength: 100",
			charstring::integerLength((uint64_t)100)==3);
	test("integerLength: 1000",
			charstring::integerLength((uint64_t)1000)==4);

	uint64_t	testuint64=ULONG_MAX;
	char		*teststr=charstring::parseNumber(testuint64);
	test("parseNumber ULONG_MAX",
		!charstring::compare(teststr,
			(testuint64==4294967295UL)
				?"4294967295"
				:"18446744073709551615"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint64,40);
	test("parseNumber ULONG_MAX",
		!charstring::compare(teststr,
			(testuint64==4294967295UL)
				?"0000000000000000000000000000004294967295"
				:"0000000000000000000018446744073709551615"));
	delete[] teststr;

	int64_t		testint64=LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	test("parseNumber LONG_MAX",
		!charstring::compare(teststr,
			(testint64==2147483647)
				?"2147483647"
				:"9223372036854775807"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	test("parseNumber LONG_MAX",
		!charstring::compare(teststr,
			(testint64==2147483647)
				?"0000000000000000000000000000002147483647"
				:"0000000000000000000009223372036854775807"));
	testint64=-1*LONG_MAX;
	teststr=charstring::parseNumber(testint64);
	test("parseNumber -LONG_MAX",
		!charstring::compare(teststr,
			(testint64==-2147483647)
				?"-2147483647"
				:"-9223372036854775807"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint64,40);
	test("parseNumber -LONG_MAX",
		!charstring::compare(teststr,
			(testint64==-2147483647)
				?"-000000000000000000000000000002147483647"
				:"-000000000000000000009223372036854775807"));
	delete[] teststr;

	uint32_t	testuint32=4294967295UL;
	teststr=charstring::parseNumber(testuint32);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,"4294967295"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint32,40);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000004294967295"));
	delete[] teststr;

	int32_t		testint32=2147483647;
	teststr=charstring::parseNumber(testint32);
	test("parseNumber 2^31-1",
		!charstring::compare(teststr,"2147483647"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	test("parseNumber 2^32-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000002147483647"));
	delete[] teststr;
	testint32=-2147483647;
	teststr=charstring::parseNumber(testint32);
	test("parseNumber -2^31-1",
		!charstring::compare(teststr,"-2147483647"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint32,40);
	test("parseNumber -2^32-1",
		!charstring::compare(teststr,
			"-000000000000000000000000000002147483647"));
	delete[] teststr;

	uint16_t	testuint16=65535;
	teststr=charstring::parseNumber(testuint16);
	test("parseNumber 2^16-1",
		!charstring::compare(teststr,"65535"));
	delete[] teststr;
	teststr=charstring::parseNumber(testuint16,40);
	test("parseNumber 2^16-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000000000065535"));
	delete[] teststr;

	int16_t		testint16=32767;
	teststr=charstring::parseNumber(testint16);
	test("parseNumber 2^15-1",
		!charstring::compare(teststr,"32767"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	test("parseNumber 2^15-1",
		!charstring::compare(teststr,
			"0000000000000000000000000000000000032767"));
	testint16=-32767;
	teststr=charstring::parseNumber(testint16);
	test("parseNumber -2^15-1",
		!charstring::compare(teststr,"-32767"));
	delete[] teststr;
	teststr=charstring::parseNumber(testint16,40);
	test("parseNumber -2^15-1",
		!charstring::compare(teststr,
			"-000000000000000000000000000000000032767"));
	delete[] teststr;
	stdoutput.printf("\n");
	

	// empty/null/yes/no
	stdoutput.printf("empty/null/yes/no...\n");
	test("NULL",charstring::isNullOrEmpty((const char *)NULL));
	test("empty",charstring::isNullOrEmpty(""));
	test("not null",!charstring::isNullOrEmpty("not null"));
	test("not empty",!charstring::isNullOrEmpty("not empty"));
	test("yes",charstring::isYes("yes"));
	test("Yes",charstring::isYes("Yes"));
	test("YES",charstring::isYes("YES"));
	test("yes,",charstring::isYes("yes,"));
	test("yes ",charstring::isYes("yes "));
	test("yesterday",!charstring::isYes("yesterday"));
	test("Y",charstring::isYes("Y"));
	test("y",charstring::isYes("y"));
	test("yo",!charstring::isYes("yo"));
	test("true",charstring::isYes("true"));
	test("True",charstring::isYes("True"));
	test("TRUE",charstring::isYes("TRUE"));
	test("truest",!charstring::isYes("truest"));
	test("T",charstring::isYes("T"));
	test("t",charstring::isYes("t"));
	test("1",charstring::isYes("1"));
	test("123",!charstring::isYes("123"));
	test("NULL",!charstring::isYes(NULL));
	test("empty",!charstring::isYes(""));
	test("no",charstring::isNo("no"));
	test("No",charstring::isNo("No"));
	test("NO",charstring::isNo("NO"));
	test("nothing",!charstring::isNo("nothing"));
	test("N",charstring::isNo("N"));
	test("n",charstring::isNo("n"));
	test("ne",!charstring::isNo("ne"));
	test("false",charstring::isNo("false"));
	test("False",charstring::isNo("False"));
	test("FALSE",charstring::isNo("FALSE"));
	test("falsest",!charstring::isNo("falsest"));
	test("F",charstring::isNo("F"));
	test("f",charstring::isNo("f"));
	test("0",charstring::isNo("0"));
	test("012",!charstring::isNo("012"));
	test("NULL",!charstring::isNo(NULL));
	test("empty",!charstring::isNo(""));
	stdoutput.printf("\n");
	

	// replace
	stdoutput.printf("replace...\n");
	original="one:-two:-one:-three:-one:-four:-one:-five";
	char	*originalcopy=charstring::duplicate(original);
	charstring::replace(originalcopy,'-','|');
	test("replace char",
		!charstring::compare(originalcopy,
			"one:|two:|one:|three:|one:|four:|one:|five"));
	charstring::replace(originalcopy,'-','|');
	test("replace char (not found)",
		!charstring::compare(originalcopy,
			"one:|two:|one:|three:|one:|four:|one:|five"));
	charstring::replace(originalcopy,":|",'-');
	test("replace set",
		!charstring::compare(originalcopy,
			"one--two--one--three--one--four--one--five"));
	charstring::replace(originalcopy,":|",'-');
	test("replace set (not found)",
		!charstring::compare(originalcopy,
			"one--two--one--three--one--four--one--five"));
	delete[] originalcopy;
	char	*newstr=charstring::replace(original,"one","1");
	test("replace strings (smaller)",
		!charstring::compare(newstr,
			"1:-two:-1:-three:-1:-four:-1:-five"));
	delete[] newstr;
	newstr=charstring::replace(original,"one","oneone");
	test("replace strings (larger)",
		!charstring::compare(newstr,
		"oneone:-two:-oneone:-three:-oneone:-four:-oneone:-five"));
	delete[] newstr;

	newstr=charstring::replace(original,"onee","1");
	test("replace strings (not found)",
		!charstring::compare(newstr,
		"one:-two:-one:-three:-one:-four:-one:-five"));
	delete[] newstr;
	const char * const oldset[]={
		"one","two","three","four","five",NULL
	};
	size_t	oldlen[]={
		3,3,5,4,4
	};
	const char * const newset1[]={
		"1","2","3","4","5",NULL
	};
	newstr=charstring::replace(original,oldset,oldlen,newset1);
	test("replace strings set (smaller)",
		!charstring::compare(newstr,
			"1:-2:-1:-3:-1:-4:-1:-5"));
	delete[] newstr;
	const char * const newset2[]={
		"oneone","twotwo","threethree","fourfour","fivefive",NULL
	};
	newstr=charstring::replace(original,oldset,oldlen,newset2);
	test("replace strings set (larger)",
		!charstring::compare(newstr,
			"oneone:-twotwo:-oneone:-threethree:-"
			"oneone:-fourfour:-oneone:-fivefive"));
	delete[] newstr;

	original="0123456789,12345678,234567,3456,45";
	regularexpression	from;
	from.setPattern("[0-9]*");
	newstr=charstring::replace(original,&from,"numbers",true);
	test("replace regex/from-to (global)",
		!charstring::compare(newstr,
			"numbers,numbers,numbers,numbers,numbers"));
	delete[] newstr;

	newstr=charstring::replace(original,&from,"numbers",false);
	test("replace regex/from-to (first)",
		!charstring::compare(newstr,
			"numbers,12345678,234567,3456,45"));
	delete[] newstr;

	stdoutput.printf("\n");
	

	// compare with wildcard
	stdoutput.printf("compareWithWildcard...\n");
	test("degenerate",
		charstring::compareWithWildcards(NULL,NULL,'\0','\0'));
	test("string==string (no wildcards)",
		charstring::compareWithWildcards("string","string",'\0','\0'));
	test("str?ng==str?ng (no wildcards)",
		charstring::compareWithWildcards("str?ng","str?ng",'\0','\0'));
	test("str*ng==str*ng (no wildcards)",
		charstring::compareWithWildcards("str*ng","str*ng",'\0','\0'));
	char	string[17];
	charstring::copy(string,"0123456789abcdef");
	char	pattern[17];
	stringbuffer	title;
	for (uint16_t iter=0; iter<10; iter++) {

		switch (iter) {
			case 0:
				stdoutput.printf("single ?...\n");
				break;
			case 1:
				stdoutput.printf("single *...\n");
				break;
			case 2:
				stdoutput.printf("multiple ?...\n");
				break;
			case 3:
				stdoutput.printf("multiple *...\n");
				break;
			case 4:
				stdoutput.printf("interleaved ?...\n");
				break;
			case 5:
				stdoutput.printf("interleaved *...\n");
				break;
			case 6:
				stdoutput.printf("alternating */?...\n");
				break;
			case 7:
				stdoutput.printf("alternating ?/*...\n");
				break;
			case 8:
				stdoutput.printf("trailing *...\n");
				break;
			case 9:
				stdoutput.printf("leading *...\n");
				break;
		}

		for (uint16_t i=0; i<sizeof(pattern)-1; i++) {

			// initialize pattern
			charstring::copy(pattern,"0123456789abcdef");

			// swap in some set of wildcards
			switch (iter) {
				case 0:
					pattern[i]='?';
					break;
				case 1:
					pattern[i]='*';
					break;
				case 2:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j++) {
						pattern[j]='?';
					}
					break;
				case 3:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j++) {
						pattern[j]='*';
					}
					break;
				case 4:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j=j+2) {
						pattern[j]='?';
					}
					break;
				case 5:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j=j+2) {
						pattern[j]='*';
					}
					break;
				case 6:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j++) {
						if (j%2) {
							pattern[j]='?';
						} else {
							pattern[j]='*';
						}
					}
					break;
				case 7:
					for (uint16_t j=i;
						j<sizeof(pattern)-1; j++) {
						if (j%2) {
							pattern[j]='*';
						} else {
							pattern[j]='?';
						}
					}
					break;
				case 8:
					pattern[i]='*';
					pattern[i+1]='\0';
					break;
				case 9:
					pattern[0]='*';
					// do this rather than just
					// "0123456789abcdef"+i+1 directly in
					// the copy to silence compiler warnings
					// on freebsd 13.1 (and probably other
					// platforms)
					const char	*chars=
						"0123456789abcdef";
					charstring::copy(pattern+1,chars+i+1);
					break;
			}

			// build the title
			title.clear();
			title.append(string)->append('=')->append(pattern);

			// run the test
			test(title.getString(),
				charstring::compareWithWildcards(
						string,pattern,'?','*'));
		}
	}
	stdoutput.printf("\n");
	

	// compare versions
	stdoutput.printf("compareVersions...\n");
	test("1 vs. 2",charstring::compareVersions("1","2")<0);
	test("2 vs. 1",charstring::compareVersions("2","1")>0);
	test("1.1 vs. 1.2",charstring::compareVersions("1.1","1.2")<0);
	test("1.2 vs. 1.1",charstring::compareVersions("1.2","1.1")>0);
	test("1.1.1 vs. 1.1.2",charstring::compareVersions("1.1.1","1.1.2")<0);
	test("1.1.2 vs. 1.1.1",charstring::compareVersions("1.1.2","1.1.1")>0);
	test("1.1.2 vs. 1.1.12",
			charstring::compareVersions("1.1.2","1.1.12")<0);
	test("1.1.12 vs. 1.1.2",
			charstring::compareVersions("1.1.12","1.1.2")>0);
	test("1.1 vs. 1.1.1",charstring::compareVersions("1.1","1.1.1")<0);
	test("1.1.1 vs. 1.1",charstring::compareVersions("1.1.1","1.1")>0);
	test("1.1. vs. 1.1.1",charstring::compareVersions("1.1.","1.1.1")<0);
	test("1.1.1 vs. 1.1.",charstring::compareVersions("1.1.1","1.1.")>0);
	stdoutput.printf("\n");
	

	// compare natural
	stdoutput.printf("compareNatural...\n");
	test("rudiments-1 vs. rudiments-2",
		charstring::compareNatural("rudiments-1",
						"rudiments-2")<0);
	test("rudiments-2 vs. rudiments-1",
		charstring::compareNatural("rudiments-2",
						"rudiments-1")>0);
	test("rudiments-1.1 vs. rudiments-1.2",
		charstring::compareNatural("rudiments-1.1",
						"rudiments-1.2")<0);
	test("rudiments-1.2 vs. rudiments-1.1",
		charstring::compareNatural("rudiments-1.2",
						"rudiments-1.1")>0);
	test("rudiments-1.1.1 vs. rudiments-1.1.2",
		charstring::compareNatural("rudiments-1.1.1",
						"rudiments-1.1.2")<0);
	test("rudiments-1.1.2 vs. rudiments-1.1.1",
		charstring::compareNatural("rudiments-1.1.2",
						"rudiments-1.1.1")>0);
	test("rudiments-1.1.2 vs. rudiments-1.1.12",
		charstring::compareNatural("rudiments-1.1.2",
						"rudiments-1.1.12")<0);
	test("rudiments-1.1.12 vs. rudiments-1.1.2",
		charstring::compareNatural("rudiments-1.1.12",
						"rudiments-1.1.2")>0);
	test("rudiments-1.1 vs. rudiments-1.1.1",
		charstring::compareNatural("rudiments-1.1",
						"rudiments-1.1.1")<0);
	test("rudiments-1.1.1 vs. rudiments-1.1",
		charstring::compareNatural("rudiments-1.1.1",
						"rudiments-1.1")>0);
	test("rudiments-1.1. vs. rudiments-1.1.1",
		charstring::compareNatural("rudiments-1.1.",
						"rudiments-1.1.1")<0);
	test("rudiments-1.1.1 vs. rudiments-1.1.",
		charstring::compareNatural("rudiments-1.1.1",
						"rudiments-1.1.")>0);
	stdoutput.printf("\n");


	// human readable
	stdoutput.printf("humanReadable...\n");
	long double	val=2.1*1024.0*1024.0*1024.0*1024.0*
				1024.0*1024.0*1024.0*1024.0;
	char	*sval=charstring::humanReadable(val);
	test("humanReadable 2.1B",!charstring::compare(sval,"2.1B"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1Y",!charstring::compare(sval,"2.1Y"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1Z",!charstring::compare(sval,"2.1Z"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1P",!charstring::compare(sval,"2.1P"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1T",!charstring::compare(sval,"2.1T"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1G",!charstring::compare(sval,"2.1G"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1M",!charstring::compare(sval,"2.1M"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1K",!charstring::compare(sval,"2.1K"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::humanReadable(val);
	test("humanReadable 2.1",!charstring::compare(sval,"2.1"));
	delete[] sval;

	val=2.1*1000.0*1000.0*1000.0*1000.0*
		1000.0*1000.0*1000.0*1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1B",!charstring::compare(sval,"2.1B"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1Y",!charstring::compare(sval,"2.1Y"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1Z",!charstring::compare(sval,"2.1Z"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1P",!charstring::compare(sval,"2.1P"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1T",!charstring::compare(sval,"2.1T"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1G",!charstring::compare(sval,"2.1G"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1M",!charstring::compare(sval,"2.1M"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1K",!charstring::compare(sval,"2.1K"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::humanReadable(val,true);
	test("humanReadable 2.1",!charstring::compare(sval,"2.1"));
	delete[] sval;

	uint64_t	ival=((uint64_t)3)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024);
	sval=charstring::humanReadable(ival);
	test("humanReadable 3Z",!charstring::compare(sval,"3Z"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3P",!charstring::compare(sval,"3P"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3T",!charstring::compare(sval,"3T"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3G",!charstring::compare(sval,"3G"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3M",!charstring::compare(sval,"3M"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3K",!charstring::compare(sval,"3K"));
	delete[] sval;
	ival/=1024;
	sval=charstring::humanReadable(ival);
	test("humanReadable 3",!charstring::compare(sval,"3"));
	delete[] sval;

	int64_t	sival=((int64_t)-3)*
			((int64_t)1000)*((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000)*((int64_t)1000);
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3Z",!charstring::compare(sval,"-3Z"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3P",!charstring::compare(sval,"-3P"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3T",!charstring::compare(sval,"-3T"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3G",!charstring::compare(sval,"-3G"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3M",!charstring::compare(sval,"-3M"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3K",!charstring::compare(sval,"-3K"));
	delete[] sval;
	sival/=1000;
	sval=charstring::humanReadable(sival,true);
	test("humanReadable -3",!charstring::compare(sval,"-3"));
	delete[] sval;
	stdoutput.printf("\n");


	// conversions
	stdoutput.printf("conversions...\n");
	const char	*chars=" !\"#$&'()*+,-./01234567890:;<=>?@"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
				"abcdefghijklmnopqrstuvwxyz{|}~";
	// some compilers (SCO UW 7.0.1) won't let you split a wide character
	// string over multiple lines
	const wchar_t	*wchars=L" !\"#$&'()*+,-./01234567890:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	ucs2literal(ucs2chars," !\"#$&'()*+,-./01234567890:;<=>?@"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
				"abcdefghijklmnopqrstuvwxyz{|}~");
	char	*c=charstring::duplicate(wchars);
	test("from wchar_t",!charstring::compare(c,chars));
	delete[] c;
	c=charstring::duplicate(ucs2chars);
	test("from ucs2",!charstring::compare(c,chars));
	delete[] c;
	stdoutput.printf("\n");


	// hosttonet/nettohost (why are these here?)
	/*byte_t	v8=0xC0;
	uint16_t	v16=filedescriptor::hostToNet(
						(uint16_t)0xC0C0);
	uint32_t	v32=filedescriptor::hostToNet(
						(uint32_t)0xC0C0C0C0);
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	v64=filedescriptor::hostToNet(
					(uint64_t)0xC0C0C0C0C0C0C0C0ULL);
	#endif
	char	sv8=-1*0x01;
	int16_t	sv16=filedescriptor::hostToNet((uint16_t)
						(int16_t)(-1*0x0101));
	int32_t	sv32=filedescriptor::hostToNet((uint32_t)
						(int32_t)(-1*0x01010101));
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	int64_t	sv64=filedescriptor::hostToNet((uint64_t)
					(int64_t)(-1*0x0101010101010101LL));
	#endif
	byte_t	v8s[]={0xC0,0xC0,0xC0,0xC0};
	stdoutput.printBits(v8);
	stdoutput.printf("\n");
	stdoutput.printBits(v16);
	stdoutput.printf("\n");
	stdoutput.printBits(v32);
	stdoutput.printf("\n");
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	stdoutput.printBits(v64);
	stdoutput.printf("\n");
	#endif
	stdoutput.printBits(sv8);
	stdoutput.printf("\n");
	stdoutput.printBits(sv16);
	stdoutput.printf("\n");
	stdoutput.printBits(sv32);
	stdoutput.printf("\n");
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	stdoutput.printBits(sv64);
	stdoutput.printf("\n");
	#endif
	stdoutput.printBits(v8s,sizeof(v8s));
	stdoutput.printf("\n");*/

	return 0;
}
