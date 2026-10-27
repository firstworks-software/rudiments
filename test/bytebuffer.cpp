// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/signalclasses.h>
#include <rudiments/resultcodes.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// seconds before a hang kills the test
#define HANGTIMEOUT 60

// byte "i" of the pattern the growth tests write
static byte_t pattern(size_t i) {
	return (byte_t)(i%251);
}

// true if the first "count" bytes of "bb" hold the pattern
static bool hasPattern(bytebuffer *bb, size_t count) {
	if (bb->getSize()<count) {
		return false;
	}
	const byte_t	*buf=bb->getBuffer();
	for (size_t i=0; i<count; i++) {
		if (buf[i]!=pattern(i)) {
			return false;
		}
	}
	return true;
}

// writes the pattern one byte at a time and checks the result
static bool growByWrite(bytebuffer *bb, size_t count) {
	for (size_t i=0; i<count; i++) {
		if (bb->write(pattern(i))!=1) {
			return false;
		}
		if (bb->getActualSize()<bb->getSize()) {
			return false;
		}
	}
	return bb->getSize()==count &&
		bb->getPosition()==(off64_t)count &&
		hasPattern(bb,count);
}

// appends the pattern one byte at a time and checks the result
static bool growByAppend(bytebuffer *bb, size_t count) {
	for (size_t i=0; i<count; i++) {
		if (bb->append(pattern(i))!=bb) {
			return false;
		}
		if (bb->getActualSize()<bb->getSize()) {
			return false;
		}
	}
	return bb->getSize()==count &&
		bb->getPosition()==(off64_t)count &&
		hasPattern(bb,count);
}

// true if "bb" holds exactly the "size" bytes of "expected"
static bool holds(bytebuffer *bb, const void *expected, size_t size) {
	return bb->getSize()==size &&
		bb->getPosition()==(off64_t)size &&
		!bytestring::compare(bb->getBuffer(),expected,size);
}

// the sequence of sizes a default buffer grows through
static void testGrowthSequence() {
	stdoutput.printf("growth sequence...\n");
	bytebuffer	bb;
	byte_t		one=1;
	test("getActualSize(), initial",bb.getActualSize()==32);
	for (size_t i=0; i<32; i++) {
		bb.write(&one,1);
	}
	test("getActualSize(), full",bb.getActualSize()==32);
	bb.write(&one,1);
	test("getActualSize(), 33 bytes",bb.getActualSize()==48);
	for (size_t i=33; i<48; i++) {
		bb.write(&one,1);
	}
	test("getActualSize(), 48 bytes",bb.getActualSize()==48);
	bb.write(&one,1);
	test("getActualSize(), 49 bytes",bb.getActualSize()==72);
	for (size_t i=49; i<73; i++) {
		bb.write(&one,1);
	}
	test("getActualSize(), 73 bytes",bb.getActualSize()==108);
	for (size_t i=73; i<109; i++) {
		bb.write(&one,1);
	}
	test("getActualSize(), 109 bytes",bb.getActualSize()==162);

	// a jump of more than one step grows by as many steps as it takes
	byte_t	big[1000];
	bytestring::set(big,1,sizeof(big));
	bb.append(big,sizeof(big));
	test("getActualSize(), big append",
			bb.getSize()==1109 && bb.getActualSize()>=1109);
	stdoutput.printf("\n");
}

// grow from several initial sizes
static void testGrowthSizes() {
	stdoutput.printf("growth from small initial sizes...\n");
	for (size_t initial=2; initial<=3; initial++) {
		bytebuffer	bb(initial);
		test("initial size, getActualSize()",
				bb.getActualSize()==initial);
		test("initial size, grow by write",growByWrite(&bb,10000));
		bytebuffer	cc(initial);
		test("initial size, grow by append",growByAppend(&cc,10000));
		test("initial size, getInitialSize()",
				cc.getInitialSize()==initial);
	}
	for (size_t initial=2; initial<=3; initial++) {
		byte_t	*contents=new byte_t[initial];
		for (size_t i=0; i<initial; i++) {
			contents[i]=pattern(i);
		}
		bytebuffer	bb(contents,initial);
		test("initial contents, getSize()",bb.getSize()==initial);
		for (size_t i=initial; i<10000; i++) {
			bb.append(pattern(i));
		}
		test("initial contents, grow by append",
				bb.getSize()==10000 && hasPattern(&bb,10000));
	}
	{
	bytebuffer	bb(2);
	bb.clear(3);
	test("clear(3), getActualSize()",bb.getActualSize()==3);
	test("clear(3), grow by write",growByWrite(&bb,10000));
	bb.clear(2);
	test("clear(2), grow by append",growByAppend(&bb,10000));
	bb.clear();
	test("clear(), getSize()",bb.getSize()==0 && bb.getPosition()==0);
	test("clear(), grow again",growByWrite(&bb,10000));
	}
	stdoutput.printf("\n");
}

// tiny and zero initial sizes, which used to loop forever on growth
static void testTinyBuffers() {

	stdoutput.printf("tiny buffers...\n");
	byte_t	data[100];
	for (size_t i=0; i<sizeof(data); i++) {
		data[i]=pattern(i);
	}

	// writes of 2, 3 and 100 bytes into a 1 byte buffer
	bytebuffer	one(1);
	test("bytebuffer(1), getActualSize()",one.getActualSize()==1);
	test("bytebuffer(1), write 2",one.write(data,2)==2);
	test("bytebuffer(1), write 2, data",holds(&one,data,2));
	one.clear(1);
	test("bytebuffer(1), write 3",one.write(data,3)==3);
	test("bytebuffer(1), write 3, data",holds(&one,data,3));
	one.clear(1);
	test("bytebuffer(1), write 100",one.write(data,100)==100);
	test("bytebuffer(1), write 100, data",holds(&one,data,100));
	one.clear(1);
	one.append(data,2);
	test("bytebuffer(1), append 2",holds(&one,data,2));
	one.clear(1);
	one.append(data,100);
	test("bytebuffer(1), append 100",holds(&one,data,100));

	// grow byte by byte from 0 and 1
	bytebuffer	bb(1);
	test("bytebuffer(1), grow by write",growByWrite(&bb,10000));
	bytebuffer	cc(1);
	test("bytebuffer(1), grow by append",growByAppend(&cc,10000));
	bytebuffer	dd(100);
	dd.clear(0);
	test("clear(0), getActualSize()",dd.getActualSize()==0);
	test("clear(0), getInitialSize()",dd.getInitialSize()==0);
	test("clear(0), grow by write",growByWrite(&dd,10000));
	dd.clear(0);
	test("clear(0), grow by append",growByAppend(&dd,10000));
	dd.clear(1);
	test("clear(1), getActualSize()",dd.getActualSize()==1);
	test("clear(1), grow by write",growByWrite(&dd,10000));
	dd.clear(1);
	test("clear(1), grow by append",growByAppend(&dd,10000));

	// initial contents of 1 byte
	byte_t	*contents=new byte_t[1];
	contents[0]=pattern(0);
	bytebuffer	ee(contents,1);
	test("initial contents of 1, append 2",
			ee.append(data+1,2)==&ee && holds(&ee,data,3));
	for (size_t i=3; i<10000; i++) {
		ee.append(pattern(i));
	}
	test("initial contents of 1, grow",
			ee.getSize()==10000 && hasPattern(&ee,10000));

	// the other length forms into a 1 byte buffer
	wchar_t	wide[3]={L'a',L'b',L'c'};
	ucs2_t	u[3]={'a','b','c'};
	size_t	ws=sizeof(wchar_t);
	size_t	us=sizeof(ucs2_t);
	bytebuffer	ff(1);
	test("bytebuffer(1), write wide",ff.write(wide,3)==(ssize_t)(3*ws));
	test("bytebuffer(1), write wide, data",holds(&ff,wide,3*ws));
	ff.clear(1);
	test("bytebuffer(1), write ucs2",ff.writeUcs2(u,3)==(ssize_t)(3*us));
	test("bytebuffer(1), write ucs2, data",holds(&ff,u,3*us));
	ff.clear(1);
	test("bytebuffer(1), write string",ff.write("abcdefgh")==8);
	test("bytebuffer(1), write string, data",holds(&ff,"abcdefgh",8));
	ff.clear(1);
	ff.append(wide,3);
	test("bytebuffer(1), append wide",holds(&ff,wide,3*ws));
	ff.clear(1);
	ff.appendUcs2(u,3);
	test("bytebuffer(1), append ucs2",holds(&ff,u,3*us));
	ff.clear(1);
	test("bytebuffer(1), printf",ff.printf("n=%d",12345)==7);
	test("bytebuffer(1), printf, data",holds(&ff,"n=12345",7));
	stdoutput.printf("\n");
}

// true if a failed call changed nothing
static bool unchanged(bytebuffer *bb, size_t size, off64_t pos,
					size_t actualsize, byte_t first) {
	return bb->getSize()==size &&
		bb->getPosition()==pos &&
		bb->getActualSize()==actualsize &&
		(!size || bb->getBuffer()[0]==first);
}

// lengths that do not fit are refused and change nothing
static void testOversized() {

	stdoutput.printf("oversized lengths...\n");

	// the largest length the buffer accepts is SSIZE_MAX
	const size_t	sizemax=(size_t)-1;
	const size_t	limit=sizemax>>1;
	byte_t		data[1]={'B'};
	wchar_t		wide[1]={L'w'};
	ucs2_t		u[1]={'u'};
	const size_t	widebad=limit/sizeof(wchar_t)+1;
	const size_t	widewrap=sizemax/sizeof(wchar_t)+1;
	const size_t	ucs2bad=limit/sizeof(ucs2_t)+1;
	const size_t	ucs2wrap=sizemax/sizeof(ucs2_t)+1;

	// a 1 byte buffer holding one byte, position at the end
	bytebuffer	one(1);
	one.write((byte_t)'A');
	off64_t		pos=one.getPosition();
	test("setup, size",one.getSize()==1 && pos==1);
	test("write(SIZE_MAX) at 1",one.write(data,sizemax)==RESULT_ERROR);
	test("write(SIZE_MAX) at 1, unchanged",unchanged(&one,1,pos,1,'A'));
	test("write(limit) at 1",one.write(data,limit)==RESULT_ERROR);
	test("write(limit) at 1, unchanged",unchanged(&one,1,pos,1,'A'));
	test("write(char *,limit) at 1",
			one.write((const char *)data,limit)==RESULT_ERROR);
	test("write(char *,limit) at 1, unchanged",
			unchanged(&one,1,pos,1,'A'));
	one.setPositionRelativeToBeginning(0);
	pos=one.getPosition();
	test("write(SIZE_MAX) at 0",one.write(data,sizemax)==RESULT_ERROR);
	test("write(SIZE_MAX) at 0, unchanged",unchanged(&one,1,pos,1,'A'));
	test("write(limit+1) at 0",one.write(data,limit+1)==RESULT_ERROR);
	test("write(limit+1) at 0, unchanged",unchanged(&one,1,pos,1,'A'));

	// appends
	pos=one.getPosition();
	test("append(SIZE_MAX)",one.append(data,sizemax)==&one);
	test("append(SIZE_MAX), unchanged",unchanged(&one,1,pos,1,'A'));
	test("append(limit)",one.append(data,limit)==&one);
	test("append(limit), unchanged",unchanged(&one,1,pos,1,'A'));
	test("append(char *,limit)",
			one.append((const char *)data,limit)==&one);
	test("append(char *,limit), unchanged",unchanged(&one,1,pos,1,'A'));

	// wide lengths, including ones whose byte count wraps
	test("write(wchar_t *,limit/size+1)",
			one.write(wide,widebad)==RESULT_ERROR);
	test("write(wchar_t *,limit/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("write(wchar_t *,SIZE_MAX/size+1)",
			one.write(wide,widewrap)==RESULT_ERROR);
	test("write(wchar_t *,SIZE_MAX/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("write(wchar_t *,SIZE_MAX)",
			one.write(wide,sizemax)==RESULT_ERROR);
	test("write(wchar_t *,SIZE_MAX), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("writeUcs2(limit/size+1)",
			one.writeUcs2(u,ucs2bad)==RESULT_ERROR);
	test("writeUcs2(limit/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("writeUcs2(SIZE_MAX/size+1)",
			one.writeUcs2(u,ucs2wrap)==RESULT_ERROR);
	test("writeUcs2(SIZE_MAX/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("append(wchar_t *,limit/size+1)",
			one.append(wide,widebad)==&one);
	test("append(wchar_t *,limit/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("append(wchar_t *,SIZE_MAX/size+1)",
			one.append(wide,widewrap)==&one);
	test("append(wchar_t *,SIZE_MAX/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("appendUcs2(limit/size+1)",
			one.appendUcs2(u,ucs2bad)==&one);
	test("appendUcs2(limit/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("appendUcs2(SIZE_MAX/size+1)",
			one.appendUcs2(u,ucs2wrap)==&one);
	test("appendUcs2(SIZE_MAX/size+1), unchanged",
			unchanged(&one,1,pos,1,'A'));

	// a position before the start wraps to a huge value
	one.setPositionRelativeToEnd(-2);
	pos=one.getPosition();
	test("write(1) at wrapped position",one.write(data,1)==RESULT_ERROR);
	test("write(1) at wrapped position, unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("write(2) at wrapped position",one.write(data,2)==RESULT_ERROR);
	test("write(2) at wrapped position, unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("write(character) at wrapped position",
			one.write('x')==RESULT_ERROR);
	test("write(character) at wrapped position, unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("write(wchar_t *,1) at wrapped position",
			one.write(wide,1)==RESULT_ERROR);
	test("write(wchar_t *,1) at wrapped position, unchanged",
			unchanged(&one,1,pos,1,'A'));
	test("printf() at wrapped position",
			one.printf("%d",5)==RESULT_ERROR);
	test("printf() at wrapped position, unchanged",
			unchanged(&one,1,pos,1,'A'));
	if (wcharstring::supportsPrintf()) {
		test("wide printf() at wrapped position",
				one.printf(L"%d",5)==RESULT_ERROR);
		test("wide printf() at wrapped position, unchanged",
				unchanged(&one,1,pos,1,'A'));
	}
	if (ucs2charstring::supportsPrintf()) {
		ucs2literal(format,"%hd");
		test("printfUcs2() at wrapped position",
				one.printfUcs2(format,(uint16_t)5)==
							RESULT_ERROR);
		test("printfUcs2() at wrapped position, unchanged",
				unchanged(&one,1,pos,1,'A'));
	}

	// an append ignores the position
	one.append((byte_t)'C');
	test("append at wrapped position",
			one.getSize()==2 && one.getPosition()==2 &&
			one.getBuffer()[0]=='A' && one.getBuffer()[1]=='C');

	// a buffer with no storage at all
	bytebuffer	zero(1);
	zero.clear(0);
	pos=zero.getPosition();
	test("zero, setup",zero.getActualSize()==0 && pos==0);
	test("zero, write(SIZE_MAX)",zero.write(data,sizemax)==RESULT_ERROR);
	test("zero, write(limit+1)",zero.write(data,limit+1)==RESULT_ERROR);
	test("zero, write(wchar_t *,limit/size+1)",
			zero.write(wide,widebad)==RESULT_ERROR);
	test("zero, write(wchar_t *,SIZE_MAX/size+1)",
			zero.write(wide,widewrap)==RESULT_ERROR);
	test("zero, writeUcs2(SIZE_MAX/size+1)",
			zero.writeUcs2(u,ucs2wrap)==RESULT_ERROR);
	test("zero, append(SIZE_MAX)",zero.append(data,sizemax)==&zero);
	test("zero, append(limit+1)",zero.append(data,limit+1)==&zero);
	test("zero, append(wchar_t *,SIZE_MAX/size+1)",
			zero.append(wide,widewrap)==&zero);
	test("zero, appendUcs2(SIZE_MAX/size+1)",
			zero.appendUcs2(u,ucs2wrap)==&zero);
	test("zero, unchanged",unchanged(&zero,0,pos,0,0));
	test("zero, write(0)",zero.write(data,0)==0);
	test("zero, write(0), unchanged",unchanged(&zero,0,pos,0,0));
	test("zero, write(1)",zero.write(data,1)==1);
	test("zero, write(1), data",holds(&zero,data,1));
	stdoutput.printf("\n");
}

// write and append the string, character and number forms
static void testWritePaths() {

	stdoutput.printf("write and append paths...\n");
	bytebuffer	bb(4);
	byte_t		bytes[3]={1,2,3};

	// byte and char forms
	test("write(byte_t *), return",bb.write(bytes,3)==3);
	test("write(byte_t *), data",holds(&bb,bytes,3));
	bb.clear();
	test("write(byte_t *,0), return",bb.write(bytes,0)==0);
	test("write(byte_t *,0), size",bb.getSize()==0);
	test("write(const char *,len), return",bb.write("abcdef",4)==4);
	test("write(const char *,len), data",holds(&bb,"abcd",4));
	test("write(const char *), return",bb.write("0123456789")==10);
	test("write(const char *), data",
				!bytestring::compare(bb.getBuffer(),
						"abcd0123456789",14) &&
				bb.getSize()==14);
	bb.clear();
	test("write(char), return",bb.write('x')==1);
	test("write(char), data",holds(&bb,"x",1));
	bb.clear();
	test("append(byte_t *), return",bb.append(bytes,3)==&bb);
	test("append(byte_t *), data",holds(&bb,bytes,3));
	bb.append("abcdef",2);
	bb.append("0123456789");
	bb.append('x');
	test("append(char *), data",holds(&bb,
				"\001\002\003ab0123456789x",16));
	bb.clear();

	// wchar forms
	wchar_t		wide[4]={L'a',L'b',L'c',L'\0'};
	size_t		ws=sizeof(wchar_t);
	test("write(wchar_t *,len), return",bb.write(wide,2)==(ssize_t)(2*ws));
	test("write(wchar_t *,len), data",holds(&bb,wide,2*ws));
	bb.clear();
	test("write(wchar_t *), return",bb.write(wide)==(ssize_t)(3*ws));
	test("write(wchar_t *), data",holds(&bb,wide,3*ws));
	test("write(wchar_t), return",bb.write(L'z')==(ssize_t)ws);
	bb.clear();
	bb.append(wide,2);
	bb.append(wide);
	bb.append(L'z');
	test("append(wchar_t *), size",bb.getSize()==6*ws);
	test("append(wchar_t *), data",
			!bytestring::compare(bb.getBuffer(),wide,2*ws) &&
			!bytestring::compare(bb.getBuffer()+2*ws,wide,3*ws) &&
			!bytestring::compare(bb.getBuffer()+5*ws,L"z",ws));
	bb.clear();

	// ucs2 forms
	ucs2literal(u,"abc");
	size_t		us=sizeof(ucs2_t);
	test("writeUcs2(len), return",bb.writeUcs2(u,2)==(ssize_t)(2*us));
	test("writeUcs2(len), data",holds(&bb,u,2*us));
	bb.clear();
	test("writeUcs2(), return",bb.writeUcs2(u)==(ssize_t)(3*us));
	test("writeUcs2(), data",holds(&bb,u,3*us));
	test("writeUcs2(char), return",bb.writeUcs2(u[0])==(ssize_t)us);
	bb.clear();
	bb.appendUcs2(u,2);
	bb.appendUcs2(u);
	bb.appendUcs2(u[0]);
	test("appendUcs2(), size",bb.getSize()==6*us);
	test("appendUcs2(), data",
			!bytestring::compare(bb.getBuffer(),u,2*us) &&
			!bytestring::compare(bb.getBuffer()+2*us,u,3*us) &&
			!bytestring::compare(bb.getBuffer()+5*us,u,us));
	bb.clear();

	// number forms
	uint32_t	n32=0x01020304;
	uint64_t	n64=0x0102030405060708ULL;
	double		d=1.5;
	test("write(uint32_t), return",bb.write(n32)==4);
	test("write(uint64_t), return",bb.write(n64)==8);
	test("write(double), return",bb.write(d)==(ssize_t)sizeof(double));
	bb.append(n32);
	bb.append(n64);
	bb.append(d);
	test("write/append numbers, size",
			bb.getSize()==2*(4+8+sizeof(double)));
	test("write/append numbers, data",
		!bytestring::compare(bb.getBuffer(),&n32,4) &&
		!bytestring::compare(bb.getBuffer()+4,&n64,8) &&
		!bytestring::compare(bb.getBuffer()+12,&d,sizeof(double)) &&
		!bytestring::compare(bb.getBuffer()+12+sizeof(double),&n32,4));
	stdoutput.printf("\n");
}

// formatted writes, small and large
static void testPrintfPaths() {

	stdoutput.printf("printf paths...\n");
	bytebuffer	bb(4);

	// char
	test("printf(), return",bb.printf("n=%d",12)==4);
	test("printf(), data",holds(&bb,"n=12",4));
	test("printf(), large return",bb.printf("%0200d",1)==200);
	test("printf(), large size",bb.getSize()==204);
	bool	ok=!bytestring::compare(bb.getBuffer()+4,
					"000000000000000000000000",24) &&
		bb.getBuffer()[203]=='1';
	test("printf(), large data",ok);
	bb.clear();
	bb.setPositionRelativeToBeginning(10);
	test("printf(), after position",bb.printf("%s","abc")==3);
	test("printf(), after position, size",bb.getSize()==13);
	test("printf(), after position, data",
		!bytestring::compare(bb.getBuffer()+10,"abc",3));
	bb.clear();

	// wchar
	if (wcharstring::supportsPrintf()) {
		size_t	ws=sizeof(wchar_t);
		test("wide printf(), return",bb.printf(L"n=%d",12)==4);
		test("wide printf(), data",holds(&bb,L"n=12",4*ws));
		test("wide printf(), large return",
				bb.printf(L"%0200d",1)==200);
		test("wide printf(), large size",bb.getSize()==204*ws);
		test("wide printf(), large data",
			!bytestring::compare(bb.getBuffer()+4*ws,
					L"000000000000",12*ws));
		bb.clear();
	}

	// ucs2
	if (ucs2charstring::supportsPrintf()) {
		size_t	us=sizeof(ucs2_t);
		ucs2literal(format,"n=%hd");
		ucs2literal(expected,"n=12");
		ucs2literal(bigformat,"%0200hd");
		test("printfUcs2(), return",
			bb.printfUcs2(format,(uint16_t)12)==4);
		test("printfUcs2(), data",holds(&bb,expected,4*us));
		test("printfUcs2(), large return",
			bb.printfUcs2(bigformat,(uint16_t)1)==200);
		test("printfUcs2(), large size",bb.getSize()==204*us);
		bb.clear();
	}
	stdoutput.printf("\n");
}

// copies, assignment, clear(size), truncate, and attached contents
static void testStateFixes() {
	stdoutput.printf("state fixes...\n");
	byte_t	fill[100];
	for (size_t i=0; i<sizeof(fill); i++) {
		fill[i]=pattern(i);
	}

	// copy, then append within the spare capacity
	{
	bytebuffer	a(64);
	a.append(fill,3);
	bytebuffer	b(a);
	test("copy, same actual size",b.getActualSize()==64);
	test("copy, same size",b.getSize()==3);
	b.append(fill+3,40);
	test("copy, append in spare capacity",
		b.getSize()==43 && hasPattern(&b,43) &&
		b.getActualSize()==64);
	test("copy, original unchanged",a.getSize()==3 && hasPattern(&a,3));
	}

	// copy an empty buffer, then append
	{
	bytebuffer	a(16);
	bytebuffer	b(a);
	test("copy empty, actual size",
		b.getActualSize()==16 && b.getSize()==0);
	b.append(fill,10);
	test("copy empty, append",b.getSize()==10 && hasPattern(&b,10));
	}

	// operator=, with spare capacity and with an empty source
	{
	bytebuffer	a(64);
	a.append(fill,3);
	bytebuffer	b(8);
	b.append(fill,8);
	b=a;
	test("operator=, same actual size",b.getActualSize()==64);
	b.append(fill+3,40);
	test("operator=, append in spare capacity",
		b.getSize()==43 && hasPattern(&b,43));
	bytebuffer	e(16);
	b=e;
	test("operator=, empty source",
		b.getSize()==0 && b.getActualSize()==16);
	b.append(fill,10);
	test("operator=, empty source, append",
		b.getSize()==10 && hasPattern(&b,10));
	}

	// self-assignment
	{
	bytebuffer	a(64);
	a.append(fill,20);
	bytebuffer	*self=&a;
	a=*self;
	test("self-assignment, unchanged",
		a.getSize()==20 && a.getActualSize()==64 &&
		hasPattern(&a,20));
	a.append(fill+20,30);
	test("self-assignment, append",a.getSize()==50 && hasPattern(&a,50));
	}

	// truncate past the end, then append
	{
	bytebuffer	a(8);
	a.append(fill,5);
	a.truncate(1000);
	test("truncate past the end, size",a.getSize()==5);
	a.setPositionRelativeToBeginning(1000);
	a.truncate();
	test("truncate() past the end, size",a.getSize()==5);
	a.truncate(5);
	test("truncate at the end, size",a.getSize()==5);
	a.append(fill+5,20);
	test("truncate past the end, append",
		a.getSize()==25 && hasPattern(&a,25) &&
		a.getActualSize()>=a.getSize());
	a.truncate(0);
	test("truncate(0), size",a.getSize()==0);
	}

	// clear(size), then use
	{
	bytebuffer	a;
	a.append(fill,30);
	a.clear(4);
	test("clear(4), state",a.getSize()==0 && a.getActualSize()==4 &&
				a.getPosition()==0 && a.getInitialSize()==4);
	a.append(fill,50);
	test("clear(4), append",a.getSize()==50 && hasPattern(&a,50));
	a.clear(0);
	test("clear(0), state",a.getSize()==0 && a.getActualSize()==0);
	a.write(fill,10);
	test("clear(0), write",a.getSize()==10 && hasPattern(&a,10));
	}

	// attached contents of size 0
	{
	byte_t	*contents=new byte_t[1];
	bytebuffer	a(contents,0);
	test("attached 0, state",a.getSize()==0 && a.getActualSize()==0);
	a.append(fill,40);
	test("attached 0, append",a.getSize()==40 && hasPattern(&a,40));
	}
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	header("bytebuffer");

	// a hang kills the test instead of stalling it
	signalmanager::alarm(HANGTIMEOUT);

	// Create a bytebuffer.
	bytebuffer	bb;
	stdoutput.printf("create...\n");
	test("create(), getSize()",bb.getSize()==0);
	test("create(), getPosition()",bb.getPosition()==0);
	//test("create(), getActualSize()",bb.getActualSize()==128);
	//test("create(), getInitialSize()",bb.getInitialSize()==128);
	bb.clear(10);
	stdoutput.printf("create...\n");
	test("create(),clear(), getSize()",bb.getSize()==0);
	test("create(),clear(), getPosition()",bb.getPosition()==0);
	//test("create(),clear(), getActualSize()",bb.getActualSize()==10);
	test("create(),clear(), getInitialSize()",bb.getInitialSize()==10);
	stdoutput.printf("\n");

	// Append some string sequences to the buffer.
	stdoutput.printf("append 12345...\n");
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==5);
	test("append(), getPosition()",bb.getPosition()==5);
	//test("append(), getActualSize()",bb.getActualSize()==10);
	stdoutput.printf("append 12345...\n");
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==10);
	test("append(), getPosition()",bb.getPosition()==10);
	//test("append(), getActualSize()",bb.getActualSize()==10);
	stdoutput.printf("append 12345...\n");
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==15);
	test("append(), getPosition()",bb.getPosition()==15);
	//test("append(), getActualSize()",bb.getActualSize()==20);
	stdoutput.printf("append 12345...\n");
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==20);
	test("append(), getPosition()",bb.getPosition()==20);
	//test("append(), getActualSize()",bb.getActualSize()==20);
	stdoutput.printf("append 12345...\n");
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==25);
	test("append(), getPosition()",bb.getPosition()==25);
	//test("append(), getActualSize()",bb.getActualSize()==30);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer(),
					"1234512345123451234512345",25)==0);
	test("getBuffer(), getSize()",bb.getSize()==25);
	test("getBuffer(), getPosition()",bb.getPosition()==25);
	//test("getBuffer(), getActualSize()",bb.getActualSize()==25);
	stdoutput.printf("\n");


	// Write 66666 to the buffer at position 0.
	// (the first 5 bytes should be overwritten)
	stdoutput.printf("write 66666 at position 0...\n");
	bb.setPositionRelativeToBeginning(0);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==0);
	bb.write((byte_t *)"66666",5);
	test("write(), getSize()",bb.getSize()==25);
	test("write(), getPosition()",bb.getPosition()==5);
	//test("write(), getActualSize()",bb.getActualSize()==25);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer(),
					"6666612345123451234512345",25)==0);
	test("getBuffer(), getSize()",bb.getSize()==25);
	test("getBuffer(), getPosition()",bb.getPosition()==5);
	//test("getBuffer(), getActualSize()",bb.getActualSize()==25);
	stdoutput.printf("\n");


	// Write 66666 to the buffer at position 30.
	// (there should be a gap in the buffer now containing random data)
	stdoutput.printf("write 66666 at position 30...\n");
	bb.setPositionRelativeToBeginning(30);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==30);
	bb.write((byte_t *)"66666",5);
	test("write(), getSize()",bb.getSize()==35);
	test("write(), getPosition()",bb.getPosition()==35);
	//test("write(), getActualSize()",bb.getActualSize()==35);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"66666",5)==0);
	test("getBuffer(), getSize()",bb.getSize()==35);
	test("getBuffer(), getPosition()",bb.getPosition()==35);
	//test("getBuffer(), getActualSize()",bb.getActualSize()==35);
	stdoutput.printf("\n");

	// Set the current position to 50.
	// Append 12345 to the buffer.
	// Since we used append() instead of write(), the data should not be
	// written at position 50, but rather just at the current end of
	// the buffer.
	stdoutput.printf("append 66666 after setting position to 50...\n");
	bb.setPositionRelativeToBeginning(50);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==50);
	bb.append((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==40);
	test("append(), getPosition()",bb.getPosition()==40);
	//test("append(), getActualSize()",bb.getActualSize()==45);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"6666612345",10)==0);
	test("getBuffer(), getSize()",bb.getSize()==40);
	test("getBuffer(), getPosition()",bb.getPosition()==40);
	//test("getBuffer(), getActualSize()",bb.getActualSize()==40);
	stdoutput.printf("\n");

	// Write 12345 to the buffer at the current position.
	// The current position should just be the end of the buffer, since
	// we just appended.  So calling write() here is equivalent to calling
	// append.
	stdoutput.printf("write 12345 at current position...\n");
	bb.write((byte_t *)"12345",5);
	test("append(), getSize()",bb.getSize()==45);
	test("append(), getPosition()",bb.getPosition()==45);
	//test("append(), getActualSize()",bb.getActualSize()==50);
	test("getBuffer()",
		bytestring::compare(bb.getBuffer()+30,"666661234512345",15)==0);
	test("getBuffer(), getSize()",bb.getSize()==45);
	test("getBuffer(), getPosition()",bb.getPosition()==45);
	//test("getBuffer(), getActualSize()",bb.getActualSize()==45);
	stdoutput.printf("\n");


	// read 4 bytes from position 0 of the buffer
	stdoutput.printf("read from position 0...\n");
	byte_t	buffer[5];
	buffer[4]='\0';
	bb.setPositionRelativeToBeginning(0);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==0);
	ssize_t	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==4);
	test("read(), data",bytestring::compare(buffer,"6666",4)==0);
	stdoutput.printf("\n");

	// read 4 bytes from position 5 of the buffer
	stdoutput.printf("read from position 5...\n");
	bb.setPositionRelativeToBeginning(5);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==5);
	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==4);
	test("read(), data",bytestring::compare(buffer,"1234",4)==0);
	stdoutput.printf("\n");

	// read 4 bytes from position 60 of the buffer
	// (since this is off of the end of the buffer, nothing should be read)
	stdoutput.printf("read from position 80 (off the end)...\n");
	bb.setPositionRelativeToBeginning(80);
	test("setPositionRelativeToBeginning(), getPosition()",bb.getPosition()==80);
	sizeread=bb.read(buffer,4);
	test("read(), size",sizeread==0);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear\n");
	bb.clear();
	test("clear(), getSize()",bb.getSize()==0);
	test("clear(), getPosition()",bb.getPosition()==0);
	//test("clear(), getActualSize()",bb.getActualSize()==45);
	stdoutput.printf("\n");

	// detach
	stdoutput.printf("detach\n");
	for (uint64_t i=0; i<10; i++) {
		bb.append("1234567890");
	}
	test("append(), getSize()",bb.getSize()==100);
	test("append(), getPosition()",bb.getPosition()==100);
	//test("append(), getActualSize()",bb.getActualSize()==105);
	byte_t	*detachedbuffer=bb.detachBuffer();
	test("detach(), getSize()",bb.getSize()==0);
	test("detach(), getPosition()",bb.getPosition()==0);
	//test("detach(), getActualSize()",bb.getActualSize()==45);
	bool	validdata=true;
	for (uint64_t j=0; j<100; j=j+10) {
		if (bytestring::compare(detachedbuffer+j,"1234567890",10)) {
			stdoutput.printf("	"
				"data invalid at position %lld\n",j);
			validdata=false;
			break;
		}
	}
	delete[] detachedbuffer;
	test("detach(), data",validdata);
	stdoutput.printf("\n");
	

	// formatted writes
	stdoutput.printf("formatted writes\n");
	bb.printf("%0.5f",12.12345);
	test("printf(), data",
		!bytestring::compare(bb.getBuffer(),"12.12345",8));
	bb.printf("%0.5f",12.12345);
	test("printf(), data",
		!bytestring::compare(bb.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");


	// read past the end
	stdoutput.printf("read past the end\n");
	bb.setPositionRelativeToBeginning(0);
	byte_t	readbuf[100];
	test("read(), length",bb.read(readbuf,100)==16);
	test("read(), data",
		!bytestring::compare(readbuf,"12.1234512.12345",16));
	stdoutput.printf("\n");


	// copy constructor
	stdoutput.printf("copy constructor\n");
	bytebuffer	cc(bb);
	test("copy constructor, getSize()",cc.getSize()==16);
	test("copy constructor, getPosition()",cc.getPosition()==16);
	//test("copy constructor, getActualSize()",cc.getActualSize()==45);
	test("copy constructor, data",
		!bytestring::compare(cc.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");

	// = operator
	stdoutput.printf("= operator\n");
	bytebuffer	dd(100);
	dd=bb;
	test("= operator, getSize()",dd.getSize()==16);
	test("= operator, getPosition()",dd.getPosition()==16);
	//test("= operator, getActualSize()",dd.getActualSize()==45);
	test("= operator, data",
		!bytestring::compare(dd.getBuffer(),"12.1234512.12345",16));
	stdoutput.printf("\n");

	// initial contents
	stdoutput.printf("initial contents\n");
	byte_t	*initialcontents=new byte_t[100];
	for (uint16_t k=0; k<10; k=k+10) {
		bytestring::copy(initialcontents+k,"1234567890",10);
	}
	byte_t	*iccopy=(byte_t *)bytestring::duplicate(initialcontents,100);
	bytebuffer	ee(initialcontents,100);
	test("initial contents, getSize()",ee.getSize()==100);
	test("initial contents, getPosition()",ee.getPosition()==100);
	//test("initial contents, getActualSize()",ee.getActualSize()==100);
	test("initial contents, data",
		!bytestring::compare(ee.getBuffer(),iccopy,100));
	test("getBuffer(), getSize()",ee.getSize()==100);
	test("getBuffer(), getPosition()",ee.getPosition()==100);
	//test("getBuffer(), getActualSize()",ee.getActualSize()==100);
	ee.append("1234567890");
	test("append(), getSize()",ee.getSize()==110);
	test("append(), getPosition()",ee.getPosition()==110);
	//test("append(), getActualSize()",ee.getActualSize()==120);
	test("append(), data",
		!bytestring::compare(ee.getBuffer(),iccopy,100) &&
		!bytestring::compare(ee.getBuffer()+100,"1234567890",10));
	delete[] iccopy;
	stdoutput.printf("\n");

	// large writes
	bb.clear();
	uint16_t	count=10;
	uint64_t	size=1024*100*10;
	byte_t		*data=(byte_t *)"1234567890";
	stdoutput.printf("%hd, %lld byte writes\n",count,size);
	for (uint16_t l=0; l<count; l++) {

		for (uint64_t m=0; m<size/10; m++) {
			bb.write(data,10);
		}

		const byte_t	*buffer=bb.getBuffer();
		bool		validdata=true;
		for (uint64_t n=0; n<size; n=n+10) {
			if (bytestring::compare(buffer+n,data,10)) {
				stdoutput.printf("	"
					"data invalid at position %lld\n",n);
				validdata=false;
				break;
			}
		}

		test("write(), get various stats",validdata &&
			(bb.getSize()==size) &&
			(bb.getPosition()==(off64_t)size)
			//&& (bb.getActualSize()==size)
			);

		byte_t	buf[10];
		validdata=true;
		for (uint64_t o=0; o<size; o=o+10) {
			bb.setPositionRelativeToBeginning(o);
			bb.read(buf,10);
			if (bytestring::compare(buf,data,10)) {
				stdoutput.printf("	"
					"data invalid at position %lld\n",o);
				validdata=false;
				break;
			}
		}

		test("read(), data",validdata);

		bb.clear();
	}
	stdoutput.printf("\n");

	// truncate
	stdoutput.printf("truncate...\n");
	bb.clear();
	bb.append("1234567890");
	bb.truncate(5);
	test("truncate(5), getSize",bb.getSize()==5);
	test("truncate(5), compare",
			!bytestring::compare(bb.getBuffer(),"12345",5));
	bb.truncate(4);
	test("truncate(4), getSize",bb.getSize()==4);
	test("truncate(4), compare",
			!bytestring::compare(bb.getBuffer(),"1234",4));
	bb.truncate(3);
	test("truncate(3), getSize",bb.getSize()==3);
	test("truncate(3), compare",
			!bytestring::compare(bb.getBuffer(),"123",3));
	bb.setPositionRelativeToBeginning(0);
	bb.truncate();
	test("truncate(), getSize",!bb.getSize());
	stdoutput.printf("\n");

	testStateFixes();
	testGrowthSequence();
	testGrowthSizes();
	testTinyBuffers();
	testOversized();
	testWritePaths();
	testPrintfPaths();

	signalmanager::alarm(0);
	return 0;
}
