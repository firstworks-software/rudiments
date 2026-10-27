// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include <rudiments/signalclasses.h>
#include "test.cpp"

// reference findLast, written without any pointer before the start
static const byte_t *refFindLast(const byte_t *haystack,
						size_t haystacksize,
						const byte_t *needle,
						size_t needlesize) {
	if (needlesize>haystacksize) {
		return NULL;
	}
	for (size_t i=haystacksize-needlesize+1; i>0; i--) {
		if (!bytestring::compare(haystack+i-1,needle,needlesize)) {
			return haystack+i-1;
		}
	}
	return NULL;
}

static void findLastTests() {

	stdoutput.printf("findLast edge cases...\n");

	// exact-size buffers, so that a read past either end is detectable
	byte_t	*hay=new byte_t[5];
	bytestring::copy(hay,"abcab",5);
	byte_t	*nd=new byte_t[6];
	bytestring::copy(nd,"zzzzzz",6);

	// 4 arguments, no match
	test("no match, non-const",
		!bytestring::findLast((void *)hay,5,nd,2));
	test("no match, const",
		!bytestring::findLast((const void *)hay,5,nd,2));
	test("no match, one byte",
		!bytestring::findLast((const void *)hay,5,nd,1));

	// 4 arguments, matches
	bytestring::copy(nd,"ab",2);
	test("last match",bytestring::findLast((const void *)hay,5,nd,2)==
							hay+3);
	test("last match, non-const",
		bytestring::findLast((void *)hay,5,nd,2)==hay+3);
	bytestring::copy(nd,"abc",3);
	test("match at the start only",
		bytestring::findLast((const void *)hay,5,nd,3)==hay);
	bytestring::copy(nd,"cab",3);
	test("match at the end",
		bytestring::findLast((const void *)hay,5,nd,3)==hay+2);
	bytestring::copy(nd,"abcab",5);
	test("needle is the haystack",
		bytestring::findLast((const void *)hay,5,nd,5)==hay);
	bytestring::copy(nd,"b",1);
	test("one byte needle",
		bytestring::findLast((const void *)hay,5,nd,1)==hay+4);
	test("only the first 3 bytes",
		bytestring::findLast((const void *)hay,3,nd,1)==hay+1);

	// 4 arguments, needle longer than haystack
	bytestring::copy(nd,"abcabz",6);
	test("needle 1 byte too long",
		!bytestring::findLast((const void *)hay,5,nd,6));
	test("needle 1 byte too long, non-const",
		!bytestring::findLast((void *)hay,5,nd,6));
	size_t	huge=((size_t)1)<<(sizeof(size_t)*8-1);
	test("needle size 2^63",
		!bytestring::findLast((const void *)hay,5,nd,huge));
	test("needle size 2^63+1",
		!bytestring::findLast((const void *)hay,5,nd,huge+1));
	test("needle size SIZE_MAX",
		!bytestring::findLast((const void *)hay,5,nd,(size_t)-1));
	test("needle in empty haystack",
		!bytestring::findLast((const void *)hay,0,nd,1));

	// 4 arguments, empty needle
	test("empty needle",
		bytestring::findLast((const void *)hay,5,nd,0)==hay+5);
	test("empty needle, non-const",
		bytestring::findLast((void *)hay,5,nd,0)==hay+5);
	test("empty needle, empty haystack",
		bytestring::findLast((const void *)hay,0,nd,0)==hay);

	// 4 arguments, overlapping matches
	bytestring::copy(hay,"aaaaa",5);
	bytestring::copy(nd,"aa",2);
	test("overlapping matches",
		bytestring::findLast((const void *)hay,5,nd,2)==hay+3);

	// 4 arguments, NULL
	test("NULL haystack",!bytestring::findLast((const void *)NULL,5,nd,2));
	test("NULL needle",!bytestring::findLast((const void *)hay,5,NULL,2));
	test("NULL both",!bytestring::findLast((const void *)NULL,5,NULL,0));
	stdoutput.printf("\n");

	// 4 arguments, every short haystack and needle over a and b
	stdoutput.printf("findLast exhaustive...\n");
	bool	allright=true;
	for (size_t hs=0; hs<=6 && allright; hs++) {
		for (uint32_t hb=0; hb<(1U<<hs) && allright; hb++) {
			byte_t	*h=new byte_t[hs+1];
			for (size_t i=0; i<hs; i++) {
				h[i]=(hb&(1U<<i))?'b':'a';
			}
			for (size_t ns=0; ns<=hs+2 && allright; ns++) {
				for (uint32_t nb=0; nb<(1U<<ns) &&
							allright; nb++) {
					byte_t	*n=new byte_t[ns+1];
					for (size_t i=0; i<ns; i++) {
						n[i]=(nb&(1U<<i))?'b':'a';
					}
					const void	*r=
						bytestring::findLast(
							(const void *)h,
							hs,n,ns);
					allright=(r==refFindLast(
							h,hs,n,ns));
					delete[] n;
				}
			}
			delete[] h;
		}
	}
	test("every haystack and needle up to 6 and 8 bytes",allright);
	stdoutput.printf("\n");

	// character form
	stdoutput.printf("findLast character...\n");
	bytestring::copy(hay,"abcab",5);
	test("character, last",
		bytestring::findLast((const void *)hay,'b',5)==hay+4);
	test("character, last, non-const",
		bytestring::findLast((void *)hay,'a',5)==hay+3);
	test("character, first",
		bytestring::findLast((const void *)hay,'c',5)==hay+2);
	test("character, at index 0 only",
		bytestring::findLast((const void *)hay,'a',3)==hay);
	test("character, no match",
		!bytestring::findLast((const void *)hay,'z',5));
	test("character, only the first 2 bytes",
		!bytestring::findLast((const void *)hay,'c',2));
	test("character, size 0",
		!bytestring::findLast((const void *)hay,'a',0));
	test("character, NULL haystack",
		!bytestring::findLast((const void *)NULL,'a',5));
	byte_t	*one=new byte_t[1];
	one[0]='x';
	test("character, 1 byte match",
		bytestring::findLast((const void *)one,'x',1)==one);
	test("character, 1 byte no match",
		!bytestring::findLast((const void *)one,'y',1));
	delete[] one;
	stdoutput.printf("\n");

	delete[] nd;
	delete[] hay;
}

int main(int argc, const char **argv) {

	header("bytestring");

	signalmanager::alarm(60);

	// NULL comparisons
	stdoutput.printf("NULL comparisons...\n");
	test("compare(NULL,NULL)",!bytestring::compare(NULL,NULL,0));
	test("compare(NULL,a)",bytestring::compare(NULL,(const void *)"a",1)<0);
	test("compare(a,NULL)",bytestring::compare((const void *)"a",NULL,1)>0);
	stdoutput.printf("\n");


	// duplicate
	stdoutput.printf("duplicate...\n");
	byte_t	*dup=(byte_t *)bytestring::duplicate(
					(const void *)"1234567890",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"1234567890",10));
	delete[] dup;
	dup=(byte_t *)bytestring::duplicate(
					(const void *)"2345678901",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"2345678901",10));
	delete[] dup;
	dup=(byte_t *)bytestring::duplicate(
					(const void *)"3456789012",10);
	test("duplicate(), compare()",
			!bytestring::compare(dup,"3456789012",10));
	delete[] dup;
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	byte_t	data[10];
	bytestring::copy(data,"1234567890",10);
	test("copy(), compare()",!bytestring::compare(data,"1234567890",10));
	bytestring::copy(data,"2345678901",10);
	test("copy(), compare()",!bytestring::compare(data,"2345678901",10));
	bytestring::copy(data,"3456789012",10);
	test("copy(), compare()",!bytestring::compare(data,"3456789012",10));
	stdoutput.printf("\n");
	

	// copy with overlap
	stdoutput.printf("copyWithOverlap...\n");
	bytestring::copy(data,"1234567890",10);
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"3456767890",10));
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"5676767890",10));
	bytestring::copyWithOverlap(data,data+2,5);
	test("copyWithOverlap(), compare()",
				!bytestring::compare(data,"7676767890",10));
	stdoutput.printf("\n");


	// copy until
	stdoutput.printf("copyUntil...\n");
	bytestring::copy(data,"0000000000",10);
	bytestring::copyUntil(data,"1234567890",'5',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234500000",10));
	bytestring::copyUntil(data,"1234567890",'6',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234560000",10));
	bytestring::copyUntil(data,"1234567890",'7',10);
	test("copyUntil(), compare()",
				!bytestring::compare(data,"1234567000",10));
	stdoutput.printf("\n");

	// This always fails on NetBSD/pmax.  I'm not sure if there's some
	// error in my code, or if swab() is just implemented incorrectly on
	// that platform, or if it's a subtle bug in gxemul.  For now, this
	// test is disabled for that platform though.
	char	*os=sys::getOperatingSystemName();
	char	*arch=sys::getOperatingSystemArchitecture();
	if (!(!charstring::compare(os,"NetBSD") &&
		!charstring::compare(arch,"pmax"))) {

		// copy swap bytes
		stdoutput.printf("copySwapBytes...\n");
		bytestring::copy(data,"0000000000",10);
		bytestring::copySwapBytes(data,"1234567890",10);
		test("copySwapBytes(), compare()",
				!bytestring::compare(data,"2143658709",10));
		stdoutput.printf("\n");
	}
	delete[] os;
	delete[] arch;

	// set
	stdoutput.printf("set...\n");
	bytestring::set(data,'0',10);
	test("set(), compare()",!bytestring::compare(data,"0000000000",10));
	bytestring::set(data,'1',5);
	test("set(), compare()",!bytestring::compare(data,"1111100000",10));
	bytestring::set(data,'2',2);
	test("set(), compare()",!bytestring::compare(data,"2211100000",10));
	stdoutput.printf("\n");

	// zero
	stdoutput.printf("zero...\n");
	bytestring::set(data,'1',10);
	bytestring::zero(data,5);
	byte_t	comp[]={0,0,0,0,0,'1','1','1','1','1'};
	test("set(), compare()",!bytestring::compare(data,comp,10));
	stdoutput.printf("\n");

	// find first
	stdoutput.printf("findFirst...\n");
	bytestring::copy(data,"1234567890",10);
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"123",3),
			"1234567890",10));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"234",3),
			"234567890",9));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"567",3),
			"567890",6));
	test("findFirst(), compare()",
		!bytestring::compare(
			bytestring::findFirst(data,10,"678",3),
			"67890",5));
	stdoutput.printf("\n");

	// find last
	stdoutput.printf("findLast...\n");
	bytestring::copy(data,"1212120000",10);
	test("findLast(), compare()",
		!bytestring::compare(
			bytestring::findLast(data,10,"12",2),
			"120000",6));
	test("findLast(), compare()",
		!bytestring::compare(
			bytestring::findLast(data,10,"121",3),
			"12120000",8));
	stdoutput.printf("\n");

	findLastTests();

	signalmanager::alarm(0);

	return 0;
}
