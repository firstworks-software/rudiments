// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("ucs2charstring");

	// zero
	stdoutput.printf("zero...\n");
	ucs2_t	s[50];
	ucs2charstring::zero(s,50);
	bool	zeros=true;
	for (int zi=0; zi<50; zi++) {
		if (s[zi]!=0) {
			zeros=false;
		}
	}
	test("zeros",zeros);
	stdoutput.printf("\n");


	// comparisons
	stdoutput.printf("compare...\n");
	ucs2literal(a,"a");
	test("compare(NULL,NULL)",!ucs2charstring::compare(NULL,NULL));
	test("compare(NULL,a)",ucs2charstring::compare(NULL,a)<0);
	test("compare(a,NULL)",ucs2charstring::compare(a,NULL)>0);
	test("compareIgnoringCase(NULL,NULL)",
			!ucs2charstring::compareIgnoringCase(NULL,NULL));
	test("compareIgnoringCase(NULL,a)",
			ucs2charstring::compareIgnoringCase(NULL,a)<0);
	test("compareIgnoringCase(a,NULL)",
			ucs2charstring::compareIgnoringCase(a,NULL)>0);
	test("compareNatural(NULL,NULL)",
			!ucs2charstring::compareNatural(NULL,NULL));
	test("compareNatural(NULL,a)",
			ucs2charstring::compareNatural(NULL,a)<0);
	test("compareNatural(a,NULL)",
			ucs2charstring::compareNatural(a,NULL)>0);
	test("compareVersions(NULL,NULL)",
			!ucs2charstring::compareVersions(NULL,NULL));
	test("compareVersions(NULL,a)",
			ucs2charstring::compareVersions(NULL,a)<0);
	test("compareVersions(a,NULL)",
			ucs2charstring::compareVersions(a,NULL)>0);
	stdoutput.printf("\n");


	// append
	stdoutput.printf("append...\n");
	ucs2literal(hellosp,"hello ");
	ucs2charstring::append(s,hellosp);
	ucs2literal(there,"there ");
	ucs2charstring::append(s,there);
	ucs2literal(dave,"dave !!!");
	ucs2charstring::append(s,dave,4);
	ucs2literal(hellotheredave,"hello there dave");
	test("append",!ucs2charstring::compare(s,hellotheredave));
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	ucs2literal(whats,"what's ");
	ucs2charstring::copy(s,whats);
	test("append",!ucs2charstring::compare(s,whats));
	ucs2literal(up,"up !!!");
	ucs2charstring::copy(s,up,2);
	ucs2literal(upats,"upat's ");
	test("append",!ucs2charstring::compare(s,upats));
	ucs2literal(excl," !!!");
	ucs2charstring::copy(s,2,excl);
	ucs2literal(up3,"up !!!");
	test("append",!ucs2charstring::compare(s,up3));
	ucs2literal(excl6,"!!!!!!");
	ucs2literal(up4,"up !!!!");
	ucs2charstring::copy(s,6,excl6,1);
	test("append",!ucs2charstring::compare(s,up4));
	stdoutput.printf("\n");


	// compare
	stdoutput.printf("compare...\n");
	ucs2charstring::zero(s,50);
	ucs2literal(hello,"hello");
	ucs2literal(HELLO,"HELLO");
	ucs2charstring::append(s,hello);
	test("compare",!ucs2charstring::compare(s,hello));
	test("compare",!ucs2charstring::compare(s,hello,3));
	test("compare",!ucs2charstring::compareIgnoringCase(s,HELLO));
	test("compare",!ucs2charstring::compareIgnoringCase(s,HELLO,3));
	stdoutput.printf("\n");


	// findFirst
	stdoutput.printf("findFirst...\n");
	ucs2literal(llo,"llo");
	ucs2literal(LLO,"LLO");
	test("findFirst",ucs2charstring::findFirst(s,llo));
	test("findFirstIgnoringCase",
			ucs2charstring::findFirstIgnoringCase(s,LLO));
	test("findFirstOrEnd",ucs2charstring::findFirstOrEnd(s,llo));
	ucs2literal(blah,"blah");
	ucs2literal(BLAH,"BLAH");
	test("findFirstOrEnd",
		ucs2charstring::findFirstOrEnd(s,blah)==
						s+ucs2charstring::length(s));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,LLO));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,BLAH)==
						s+ucs2charstring::length(s));
	test("findFirst",ucs2charstring::findFirst(s,(ucs2_t)'l'));
	test("findFirstIgnoringCase",
		ucs2charstring::findFirstIgnoringCase(s,(ucs2_t)'L'));
	test("findFirstOrEnd",
		ucs2charstring::findFirstOrEnd(s,(ucs2_t)'z')==
				s+ucs2charstring::length(s));
	test("findFirstOrEnd",ucs2charstring::findFirstOrEnd(s,(ucs2_t)'l'));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,(ucs2_t)'Z')==
						s+ucs2charstring::length(s));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,(ucs2_t)'l'));
	stdoutput.printf("\n");

	// findLast
	test("findLast",ucs2charstring::findLast(s,llo));
	test("findLastIgnoringCase",
			ucs2charstring::findLastIgnoringCase(s,LLO));
	test("findLast",ucs2charstring::findLast(s,(ucs2_t)'l'));
	test("findLastIgnoringCase",
			ucs2charstring::findLastIgnoringCase(s,(ucs2_t)'L'));
	stdoutput.printf("\n");


	// contains
	stdoutput.printf("contains...\n");
	test("contains",ucs2charstring::contains(s,llo));
	test("containsIgnoringCase",
			ucs2charstring::containsIgnoringCase(s,LLO));
	stdoutput.printf("\n");


	// startsWith/endsWith
	stdoutput.printf("startsWith/endsWith...\n");
	ucs2literal(he,"he");
	test("startsWith",ucs2charstring::startsWith(hello,he));
	test("startsWith",ucs2charstring::startsWith(hello,hello));
	ucs2literal(el,"el");
	test("startsWith",!ucs2charstring::startsWith(hello,el));
	ucs2literal(helloThere,"helloThere");
	test("startsWith",!ucs2charstring::startsWith(hello,helloThere));
	ucs2literal(lo,"lo");
	test("endsWith",ucs2charstring::endsWith(hello,lo));
	test("endsWith",ucs2charstring::endsWith(hello,hello));
	ucs2literal(ll,"ll");
	test("endsWith",!ucs2charstring::endsWith(hello,ll));
	test("endsWith",!ucs2charstring::endsWith(hello,helloThere));
	stdoutput.printf("\n");


	// duplicate
	stdoutput.printf("duplicate/subString...\n");
	ucs2_t	*hellothere=ucs2charstring::duplicate(s);
	test("duplicate",!ucs2charstring::compare(hellothere,hello));
	ucs2_t	*elll=ucs2charstring::subString(hellothere,1,3);
	ucs2literal(ellr,"ell");
	test("subString",!ucs2charstring::compare(elll,ellr));
	ucs2_t	*llol=ucs2charstring::subString(hellothere,2,4);
	ucs2literal(llor,"llo");
	test("subString",!ucs2charstring::compare(llol,llor));
	delete[] hellothere;
	delete[] elll;
	delete[] llol;
	stdoutput.printf("\n");


	// printf
	stdoutput.printf("printf...\n");
	ucs2_t	buffer[100];
	if (ucs2charstring::supportsPrintf()) {
		ucs2literal(format,"%lc%d%ls");
		ssize_t	size=ucs2charstring::printf(
					buffer,20,format,'a',123,hello);
		test("size",size==9);
		ucs2literal(a123hello,"a123hello");
		test("contents",!ucs2charstring::compare(buffer,a123hello));
		ucs2_t	*buf=NULL;
		size=ucs2charstring::printf(&buf,format,'a',123,hello);
		test("size",size==9);
		test("contents",!ucs2charstring::compare(buffer,a123hello));
		delete[] buf;
	} else {
		stdoutput.printf("not supported\n");
	}
	stdoutput.printf("\n");


	// split
	stdoutput.printf("split...\n");
	ucs2literal(splitstring1,"hello||hi||bye||goodbye");
	ucs2literal(delimiter,"||");
	ucs2_t		**list;
	uint64_t	listlength;
	ucs2charstring::split(splitstring1,delimiter,false,&list,&listlength);
	test("length",listlength==4);
	test("list[0]",!ucs2charstring::compare(list[0],hello));
	ucs2literal(hi,"hi");
	test("list[1]",!ucs2charstring::compare(list[1],hi));
	ucs2literal(bye,"bye");
	test("list[2]",!ucs2charstring::compare(list[2],bye));
	ucs2literal(goodbye,"goodbye");
	test("list[3]",!ucs2charstring::compare(list[3],goodbye));
	uint64_t i;
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring2,"hello||hi||bye||goodbye||");
	ucs2charstring::split(splitstring2,delimiter,false,&list,&listlength);
	test("length",listlength==5);
	test("list[0]",!ucs2charstring::compare(list[0],hello));
	test("list[1]",!ucs2charstring::compare(list[1],hi));
	test("list[2]",!ucs2charstring::compare(list[2],bye));
	test("list[3]",!ucs2charstring::compare(list[3],goodbye));
	ucs2literal(empty,"");
	test("list[4]",!ucs2charstring::compare(list[4],empty));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring3,"||hello||hi||bye||goodbye||");
	ucs2charstring::split(splitstring3,delimiter,false,&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!ucs2charstring::compare(list[0],empty));
	test("list[1]",!ucs2charstring::compare(list[1],hello));
	test("list[2]",!ucs2charstring::compare(list[2],hi));
	test("list[3]",!ucs2charstring::compare(list[3],bye));
	test("list[4]",!ucs2charstring::compare(list[4],goodbye));
	test("list[5]",!ucs2charstring::compare(list[5],empty));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring4,"||||hello||||hi||||bye||||goodbye||||");
	ucs2charstring::split(splitstring4,delimiter,false,&list,&listlength);
	test("length",listlength==11);
	test("list[0]",!ucs2charstring::compare(list[0],empty));
	test("list[1]",!ucs2charstring::compare(list[1],empty));
	test("list[2]",!ucs2charstring::compare(list[2],hello));
	test("list[3]",!ucs2charstring::compare(list[3],empty));
	test("list[4]",!ucs2charstring::compare(list[4],hi));
	test("list[5]",!ucs2charstring::compare(list[5],empty));
	test("list[6]",!ucs2charstring::compare(list[6],bye));
	test("list[7]",!ucs2charstring::compare(list[7],empty));
	test("list[8]",!ucs2charstring::compare(list[8],goodbye));
	test("list[9]",!ucs2charstring::compare(list[9],empty));
	test("list[10]",!ucs2charstring::compare(list[10],empty));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring5,"||||||||||");
	ucs2charstring::split(splitstring5,delimiter,false,&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!ucs2charstring::compare(list[0],empty));
	test("list[1]",!ucs2charstring::compare(list[1],empty));
	test("list[2]",!ucs2charstring::compare(list[2],empty));
	test("list[3]",!ucs2charstring::compare(list[3],empty));
	test("list[4]",!ucs2charstring::compare(list[4],empty));
	test("list[5]",!ucs2charstring::compare(list[5],empty));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring6,
			"http://www.firstworks.com/"
			"application/app.cgi/skin/module/template.html");
	ucs2literal(slash,"/");
	ucs2charstring::split(splitstring6,slash,true,&list,&listlength);
	test("length",listlength==7);
	ucs2literal(http,"http:");
	test("list[0]",!ucs2charstring::compare(list[0],http));
	ucs2literal(wwwfirstworkscom,"www.firstworks.com");
	test("list[1]",!ucs2charstring::compare(list[1],wwwfirstworkscom));
	ucs2literal(application,"application");
	test("list[2]",!ucs2charstring::compare(list[2],application));
	ucs2literal(appcgi,"app.cgi");
	test("list[3]",!ucs2charstring::compare(list[3],appcgi));
	ucs2literal(skin,"skin");
	test("list[4]",!ucs2charstring::compare(list[4],skin));
	ucs2literal(module,"module");
	test("list[5]",!ucs2charstring::compare(list[5],module));
	ucs2literal(templatehtml,"template.html");
	test("list[6]",!ucs2charstring::compare(list[6],templatehtml));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	ucs2literal(splitstring7,"1.2.3.4.5.6");
	ucs2literal(dot,".");
	ucs2charstring::split(splitstring7,dot,false,NULL,&listlength);
	test("length",listlength==6);

	// this test just shouldn't crash
	ucs2charstring::split(splitstring6,dot,false,NULL,NULL);
	stdoutput.printf("\n");

	// before, between, after
	stdoutput.printf("before/between/after...\n");
	ucs2literal(onetwothreefourfive,"one two three four five");
	ucs2literal(sptwo," two");
	ucs2_t	*bef=ucs2charstring::before(onetwothreefourfive,sptwo);
	ucs2literal(one,"one");
	test("before",!ucs2charstring::compare(bef,one));
	ucs2literal(twosp,"two ");
	ucs2literal(spfour," four");
	ucs2_t	*btw=ucs2charstring::between(onetwothreefourfive,twosp,spfour);
	ucs2literal(three,"three");
	test("between",!ucs2charstring::compare(btw,three));
	ucs2literal(foursp,"four ");
	ucs2_t	*aft=ucs2charstring::after(onetwothreefourfive,foursp);
	ucs2literal(five,"five");
	test("after",!ucs2charstring::compare(aft,five));
	stdoutput.printf("\n");


	// escape/unescape
	stdoutput.printf("escape/unescape...\n");
	ucs2literal(str,"hello'\"\\hello'\"\\");
	ucs2literal(esc,"\"'");
	ucs2_t	*escapedstrl=ucs2charstring::escape(str,esc);
	ucs2literal(escapedstrr,"hello\\'\\\"\\\\hello\\'\\\"\\\\");
	test("escape",!ucs2charstring::compare(escapedstrl,escapedstrr));
	ucs2_t	*unescapedstrl=ucs2charstring::unescape(escapedstrl);
	test("unescape",!ucs2charstring::compare(unescapedstrl,str));
	delete[] unescapedstrl;
	delete[] escapedstrl;
	stdoutput.printf("\n");


	// sets
	stdoutput.printf("sets...\n");
	ucs2literal(alphabet,
		"aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz");
	ucs2literal(gfedcba,"gfedcba");
	test("lengthContainingSet",
		ucs2charstring::lengthContainingSet(
				alphabet,gfedcba)==14);
	ucs2literal(hijklmnopqrstuvwxyz,"hijklmnopqrstuvwxyz");
	test("lengthNotContainingSet",
		ucs2charstring::lengthNotContainingSet(
				alphabet,hijklmnopqrstuvwxyz)==14);
	ucs2literal(klm,"klm");
	test("findFirstOfSet",ucs2charstring::findFirstOfSet(alphabet,klm)==
								(alphabet+20));
	test("findFirstOfSetOrEnd",
			ucs2charstring::findFirstOfSetOrEnd(alphabet,klm)==
								(alphabet+20));
	ucs2literal(onetwothree,"123");
	test("findFirstOfSetOrEnd (end)",
		ucs2charstring::findFirstOfSetOrEnd(alphabet,onetwothree)==
				(alphabet+ucs2charstring::length(alphabet)));
	ucs2literal(abc,"abc");
	test("findLastOfSetOrEnd",
			ucs2charstring::findLastOfSetOrEnd(alphabet,abc)==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
		ucs2charstring::findLastOfSetOrEnd(alphabet,onetwothree)==
				(alphabet+ucs2charstring::length(alphabet)));
	stdoutput.printf("\n");


	// trim
	ucs2literal(untrimmed,"\r\n	     hello there buddy     	\r\n");
	ucs2literal(rtrimmed,"\r\n	     hello there buddy");
	ucs2literal(ltrimmed,"hello there buddy");
	stdoutput.printf("trim...\n");
	ucs2charstring::copy(buffer,untrimmed);
	ucs2charstring::rightTrim(buffer);
	test("rtrim",!ucs2charstring::compare(buffer,rtrimmed));
	ucs2charstring::leftTrim(buffer);
	test("ltrim",!ucs2charstring::compare(buffer,ltrimmed));
	stdoutput.printf("\n");


	// strip
	ucs2literal(unstripped1,"   hello      there   buddy  ");
	stdoutput.printf("strip...\n");
	ucs2charstring::copy(buffer,unstripped1);
	ucs2charstring::strip(buffer,(ucs2_t)' ');
	ucs2literal(stripped1,"hellotherebuddy");
	test("strip",!ucs2charstring::compare(buffer,stripped1));
	ucs2literal(unstripped2,"   hello       there  buddy  ");
	ucs2charstring::copy(buffer,unstripped2);
	ucs2literal(spaces,"   ");
	ucs2charstring::strip(buffer,spaces);
	ucs2literal(stripped2,"hello there  buddy  ");
	test("strip",!ucs2charstring::compare(buffer,stripped2));
	stdoutput.printf("\n");


	// justification
	stdoutput.printf("justification...\n");
	ucs2_t	hellobuf[16];
	ucs2literal(hellocentered,"     hello     ");
	ucs2charstring::copy(hellobuf,hellocentered);
	ucs2charstring::leftJustify(hellobuf,15);
	ucs2literal(helloleft,"hello          ");
	test("left",!ucs2charstring::compare(hellobuf,helloleft));
	ucs2charstring::rightJustify(hellobuf,15);
	ucs2literal(helloright,"          hello");
	test("right",!ucs2charstring::compare(hellobuf,helloright));
	ucs2charstring::copy(hellobuf,helloleft);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello1," hello         ");
	ucs2charstring::copy(hellobuf,hello1);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello2,"  hello        ");
	ucs2charstring::copy(hellobuf,hello2);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello3,"   hello       ");
	ucs2charstring::copy(hellobuf,hello3);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello4,"    hello      ");
	ucs2charstring::copy(hellobuf,hello4);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello5,"     hello     ");
	ucs2charstring::copy(hellobuf,hello5);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello6,"      hello    ");
	ucs2charstring::copy(hellobuf,hello6);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello7,"       hello   ");
	ucs2charstring::copy(hellobuf,hello7);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello8,"        hello  ");
	ucs2charstring::copy(hellobuf,hello8);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello9,"         hello ");
	ucs2charstring::copy(hellobuf,hello9);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	ucs2literal(hello10,"          hello");
	ucs2charstring::copy(hellobuf,hello10);
	ucs2charstring::center(hellobuf,15);
	test("center",!ucs2charstring::compare(hellobuf,hellocentered));
	stdoutput.printf("\n");

	// case conversion
	stdoutput.printf("case conversion...\n");
	ucs2literal(hellotherebuddy,"hello there  buddy  ");
	ucs2_t	cbuffer[21];
	ucs2charstring::copy(cbuffer,hellotherebuddy);
	ucs2charstring::upper(cbuffer);
	ucs2literal(hellotherebuddyu,"HELLO THERE  BUDDY  ");
	test("upper",!ucs2charstring::compare(cbuffer,hellotherebuddyu));
	ucs2charstring::lower(cbuffer);
	test("lower",!ucs2charstring::compare(cbuffer,hellotherebuddy));
	ucs2charstring::capitalize(cbuffer);
	ucs2literal(hellotherebuddycaps,"Hello There  Buddy  ");
	test("caps",!ucs2charstring::compare(cbuffer,hellotherebuddycaps));
	stdoutput.printf("\n");
	

	// numbers
	stdoutput.printf("number...\n");
	ucs2literal(num1005,"-100.5");
	test("isNumber: -100.5",ucs2charstring::isNumber(num1005));
	ucs2literal(num100510,"-100.5.10");
	test("isNumber: -100.5.10",!ucs2charstring::isNumber(num100510));
	ucs2literal(num100,"-100");
	test("isInteger: -100",ucs2charstring::isInteger(num100));
	test("isInteger: -100.5",!ucs2charstring::isInteger(num1005));
	test("isInteger: -100.5.10",!ucs2charstring::isInteger(num100510));
	test("integerLength: 0",
			ucs2charstring::integerLength((int64_t)0)==1);
	test("integerLength: 1",
			ucs2charstring::integerLength((int64_t)1)==1);
	test("integerLength: 10",
			ucs2charstring::integerLength((int64_t)10)==2);
	test("integerLength: 100",
			ucs2charstring::integerLength((int64_t)100)==3);
	test("integerLength: 1000",
			ucs2charstring::integerLength((int64_t)1000)==4);
	test("integerLength: -1",
			ucs2charstring::integerLength((int64_t)-1)==2);
	test("integerLength: -10",
			ucs2charstring::integerLength((int64_t)-10)==3);
	test("integerLength: -100",
			ucs2charstring::integerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			ucs2charstring::integerLength((int64_t)-1000)==5);
	test("integerLength: 0",
			ucs2charstring::integerLength((uint64_t)0)==1);
	test("integerLength: 1",
			ucs2charstring::integerLength((uint64_t)1)==1);
	test("integerLength: 10",
			ucs2charstring::integerLength((uint64_t)10)==2);
	test("integerLength: 100",
			ucs2charstring::integerLength((uint64_t)100)==3);
	test("integerLength: 1000",
			ucs2charstring::integerLength((uint64_t)1000)==4);

	uint64_t	testuint64=ULONG_MAX;
	ucs2_t		*teststr=ucs2charstring::parseNumber(testuint64);
	ucs2literal(ulongmax1,"4294967295");
	ucs2literal(ulongmax2,"18446744073709551615");
	test("parseNumber ULONG_MAX",
		!ucs2charstring::compare(teststr,
			(testuint64==4294967295UL)?ulongmax1:ulongmax2));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testuint64,40);
	ucs2literal(ulongmax1padded,"0000000000000000000000000000004294967295");
	ucs2literal(ulongmax2padded,"0000000000000000000018446744073709551615");
	test("parseNumber ULONG_MAX",
		!ucs2charstring::compare(teststr,
			(testuint64==4294967295UL)
				?ulongmax1padded
				:ulongmax2padded));
	delete[] teststr;

	int64_t		testint64=LONG_MAX;
	teststr=ucs2charstring::parseNumber(testint64);
	ucs2literal(longmax1,"2147483647");
	ucs2literal(longmax2,"9223372036854775807");
	test("parseNumber LONG_MAX",
		!ucs2charstring::compare(teststr,
			(testint64==2147483647)?longmax1:longmax2));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint64,40);
	ucs2literal(longmax1padded,"0000000000000000000000000000002147483647");
	ucs2literal(longmax2padded,"0000000000000000000009223372036854775807");
	test("parseNumber LONG_MAX",
		!ucs2charstring::compare(teststr,
			(testint64==2147483647)
				?longmax1padded
				:longmax2padded));
	testint64=-1*LONG_MAX;
	teststr=ucs2charstring::parseNumber(testint64);
	ucs2literal(neglongmax1,"-2147483647");
	ucs2literal(neglongmax2,"-9223372036854775807");
	test("parseNumber -LONG_MAX",
		!ucs2charstring::compare(teststr,
			(testint64==-2147483647)?neglongmax1:neglongmax2));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint64,40);
	ucs2literal(neglongmax1padded,
			"-000000000000000000000000000002147483647");
	ucs2literal(neglongmax2padded,
			"-000000000000000000009223372036854775807");
	test("parseNumber -LONG_MAX",
		!ucs2charstring::compare(teststr,
			(testint64==-2147483647)
				?neglongmax1padded
				:neglongmax2padded));
	delete[] teststr;

	uint32_t	testuint32=4294967295UL;
	teststr=ucs2charstring::parseNumber(testuint32);
	test("parseNumber 2^32-1",
		!ucs2charstring::compare(teststr,ulongmax1));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testuint32,40);
	test("parseNumber 2^32-1",
		!ucs2charstring::compare(teststr,ulongmax1padded));
	delete[] teststr;

	int32_t		testint32=2147483647;
	teststr=ucs2charstring::parseNumber(testint32);
	test("parseNumber 2^31-1",
		!ucs2charstring::compare(teststr,longmax1));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint32,40);
	test("parseNumber 2^32-1",
		!ucs2charstring::compare(teststr,longmax1padded));
	delete[] teststr;
	testint32=-2147483647;
	teststr=ucs2charstring::parseNumber(testint32);
	test("parseNumber -2^31-1",
		!ucs2charstring::compare(teststr,neglongmax1));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint32,40);
	test("parseNumber -2^32-1",
		!ucs2charstring::compare(teststr,neglongmax1padded));
	delete[] teststr;

	uint16_t	testuint16=65535;
	teststr=ucs2charstring::parseNumber(testuint16);
	ucs2literal(ushortmax,"65535");
	test("parseNumber 2^16-1",
		!ucs2charstring::compare(teststr,ushortmax));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testuint16,40);
	ucs2literal(ushortmaxpadded,"0000000000000000000000000000000000065535");
	test("parseNumber 2^16-1",
		!ucs2charstring::compare(teststr,ushortmaxpadded));
	delete[] teststr;

	int16_t		testint16=32767;
	teststr=ucs2charstring::parseNumber(testint16);
	ucs2literal(shortmax,"32767");
	test("parseNumber 2^15-1",
		!ucs2charstring::compare(teststr,shortmax));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint16,40);
	ucs2literal(shortmaxpadded,"0000000000000000000000000000000000032767");
	test("parseNumber 2^15-1",
		!ucs2charstring::compare(teststr,shortmaxpadded));
	testint16=-32767;
	teststr=ucs2charstring::parseNumber(testint16);
	ucs2literal(negshortmax,"-32767");
	test("parseNumber -2^15-1",
		!ucs2charstring::compare(teststr,negshortmax));
	delete[] teststr;
	teststr=ucs2charstring::parseNumber(testint16,40);
	ucs2literal(negshortmaxpadded,
			"-000000000000000000000000000000000032767");
	test("parseNumber -2^15-1",
		!ucs2charstring::compare(teststr,negshortmaxpadded));
	delete[] teststr;
	stdoutput.printf("\n");
	

	// empty/null/yes/no
	stdoutput.printf("empty/null/yes/no...\n");
	test("NULL",ucs2charstring::isNullOrEmpty((const ucs2_t *)NULL));
	test("empty",ucs2charstring::isNullOrEmpty(empty));
	ucs2literal(notnull,"not null");
	test("not null",!ucs2charstring::isNullOrEmpty(notnull));
	ucs2literal(notempty,"not empty");
	test("not empty",!ucs2charstring::isNullOrEmpty(notempty));
	ucs2literal(yes,"yes");
	test("yes",ucs2charstring::isYes(yes));
	ucs2literal(Yes,"Yes");
	test("Yes",ucs2charstring::isYes(Yes));
	ucs2literal(YES,"YES");
	test("YES",ucs2charstring::isYes(YES));
	ucs2literal(yescomma,"yes,");
	test("yes,",ucs2charstring::isYes(yescomma));
	ucs2literal(yessp,"yes ");
	test("yes ",ucs2charstring::isYes(yessp));
	ucs2literal(yesterday,"yesterday");
	test("yesterday",!ucs2charstring::isYes(yesterday));
	ucs2literal(Y,"Y");
	test("Y",ucs2charstring::isYes(Y));
	ucs2literal(y,"y");
	test("y",ucs2charstring::isYes(y));
	ucs2literal(yo,"yo");
	test("yo",!ucs2charstring::isYes(yo));
	ucs2literal(truestr,"true");
	test("true",ucs2charstring::isYes(truestr));
	ucs2literal(True,"True");
	test("True",ucs2charstring::isYes(True));
	ucs2literal(TRUE,"TRUE");
	test("TRUE",ucs2charstring::isYes(TRUE));
	ucs2literal(truest,"truest");
	test("truest",!ucs2charstring::isYes(truest));
	ucs2literal(T,"T");
	test("T",ucs2charstring::isYes(T));
	ucs2literal(t,"t");
	test("t",ucs2charstring::isYes(t));
	ucs2literal(num1,"1");
	test("1",ucs2charstring::isYes(num1));
	ucs2literal(num123,"123");
	test("123",!ucs2charstring::isYes(num123));
	test("NULL",!ucs2charstring::isYes(NULL));
	test("empty",!ucs2charstring::isYes(empty));
	ucs2literal(no,"no");
	test("no",ucs2charstring::isNo(no));
	ucs2literal(No,"No");
	test("No",ucs2charstring::isNo(No));
	ucs2literal(NO,"NO");
	test("NO",ucs2charstring::isNo(NO));
	ucs2literal(nothing,"nothing");
	test("nothing",!ucs2charstring::isNo(nothing));
	ucs2literal(N,"N");
	test("N",ucs2charstring::isNo(N));
	ucs2literal(n,"n");
	test("n",ucs2charstring::isNo(n));
	ucs2literal(ne,"ne");
	test("ne",!ucs2charstring::isNo(ne));
	ucs2literal(falsestr,"false");
	test("false",ucs2charstring::isNo(falsestr));
	ucs2literal(False,"False");
	test("False",ucs2charstring::isNo(False));
	ucs2literal(FALSE,"FALSE");
	test("FALSE",ucs2charstring::isNo(FALSE));
	ucs2literal(falsest,"falsest");
	test("falsest",!ucs2charstring::isNo(falsest));
	ucs2literal(F,"F");
	test("F",ucs2charstring::isNo(F));
	ucs2literal(f,"f");
	test("f",ucs2charstring::isNo(f));
	ucs2literal(num0,"0");
	test("0",ucs2charstring::isNo(num0));
	ucs2literal(num012,"012");
	test("012",!ucs2charstring::isNo(num012));
	test("NULL",!ucs2charstring::isNo(NULL));
	test("empty",!ucs2charstring::isNo(empty));
	stdoutput.printf("\n");
	

	// replace
	stdoutput.printf("replace...\n");
	ucs2literal(original,"one:-two:-one:-three:-one:-four:-one:-five");
	ucs2_t	*originalcopy=ucs2charstring::duplicate(original);
	ucs2charstring::replace(originalcopy,(ucs2_t)'-',(ucs2_t)'|');
	ucs2literal(replacechar,"one:|two:|one:|three:|one:|four:|one:|five");
	test("replace char",
		!ucs2charstring::compare(originalcopy,replacechar));
	ucs2charstring::replace(originalcopy,(ucs2_t)'-',(ucs2_t)'|');
	test("replace char (not found)",
		!ucs2charstring::compare(originalcopy,replacechar));
	ucs2literal(colonpipe,":|");
	ucs2charstring::replace(originalcopy,colonpipe,(ucs2_t)'-');
	ucs2literal(replaceset,"one--two--one--three--one--four--one--five");
	test("replace set",
		!ucs2charstring::compare(originalcopy,replaceset));
	ucs2charstring::replace(originalcopy,colonpipe,(ucs2_t)'-');
	test("replace set (not found)",
		!ucs2charstring::compare(originalcopy,replaceset));
	delete[] originalcopy;
	ucs2_t	*newstr=ucs2charstring::replace(original,one,num1);
	ucs2literal(replacestringss,"1:-two:-1:-three:-1:-four:-1:-five");
	test("replace strings (smaller)",
		!ucs2charstring::compare(newstr,replacestringss));
	delete[] newstr;
	ucs2literal(oneone,"oneone");
	newstr=ucs2charstring::replace(original,one,oneone);
	ucs2literal(replacestringsl,
		"oneone:-two:-oneone:-three:-oneone:-four:-oneone:-five");
	test("replace strings (larger)",
		!ucs2charstring::compare(newstr,replacestringsl));
	delete[] newstr;

	ucs2literal(onee,"onee");
	newstr=ucs2charstring::replace(original,onee,num1);
	test("replace strings (not found)",
		!ucs2charstring::compare(newstr,original));
	delete[] newstr;
	ucs2literal(two,"two");
	ucs2literal(four,"four");
	const ucs2_t * const oldset[]={
		one,two,three,four,five,NULL
	};
	size_t	oldlen[]={
		3,3,5,4,4
	};
	ucs2literal(num2,"2");
	ucs2literal(num3,"3");
	ucs2literal(num4,"4");
	ucs2literal(num5,"5");
	const ucs2_t * const newset1[]={
		num1,num2,num3,num4,num5,NULL
	};
	newstr=ucs2charstring::replace(original,oldset,oldlen,newset1);
	ucs2literal(replacestringssets,"1:-2:-1:-3:-1:-4:-1:-5");
	test("replace strings set (smaller)",
		!ucs2charstring::compare(newstr,replacestringssets));
	delete[] newstr;
	ucs2literal(twotwo,"twotwo");
	ucs2literal(threethree,"threethree");
	ucs2literal(fourfour,"fourfour");
	ucs2literal(fivefive,"fivefive");
	const ucs2_t * const newset2[]={
		oneone,twotwo,threethree,fourfour,fivefive,NULL
	};
	newstr=ucs2charstring::replace(original,oldset,oldlen,newset2);
	ucs2literal(replacestringssetl,
			"oneone:-twotwo:-oneone:-threethree:-"
			"oneone:-fourfour:-oneone:-fivefive");
	test("replace strings set (larger)",
		!ucs2charstring::compare(newstr,replacestringssetl));
	delete[] newstr;
	stdoutput.printf("\n");


#if 0
unimplemented...
	// duplicate from char
	stdoutput.printf("duplicate from char...\n");
	char	chbuf[128];
	ucs2_t	wchbuf[128];
	for (uint16_t c=1; c<=127; c++) {
		chbuf[c-1]=(char)c;
		wchbuf[c-1]=(ucs2_t)c;
	}
	chbuf[127]='\0';
	wchbuf[127]=L'\0';
	ucs2_t	*t=ucs2charstring::duplicate(chbuf);
	test("duplicate",!ucs2charstring::compare(t,wchbuf));
	delete[] t;
	t=ucs2charstring::duplicate(chbuf,20);
	test("duplicate",!ucs2charstring::compare(t,wchbuf,20));
	delete[] t;
	stdoutput.printf("\n");
#endif


	// compare with wildcard
	stdoutput.printf("compareWithWildcard...\n");
	test("degenerate",
		ucs2charstring::compareWithWildcards(
				NULL,NULL,(ucs2_t)'\0',(ucs2_t)'\0'));
	ucs2literal(stringlit,"string");
	test("string==string (no wildcards)",
		ucs2charstring::compareWithWildcards(
				stringlit,stringlit,(ucs2_t)'\0',(ucs2_t)'\0'));
	ucs2literal(strqng,"str?ng");
	test("str?ng==str?ng (no wildcards)",
		ucs2charstring::compareWithWildcards(
				strqng,strqng,(ucs2_t)'\0',(ucs2_t)'\0'));
	ucs2literal(strsng,"strsng");
	test("str*ng==str*ng (no wildcards)",
		ucs2charstring::compareWithWildcards(
				strsng,strsng,(ucs2_t)'\0',(ucs2_t)'\0'));
	ucs2_t	string[17];
	ucs2literal(zerotof,"0123456789abcdef");
	ucs2charstring::copy(string,zerotof);
	ucs2_t	pattern[17];
	ucs2stringbuffer	title;
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

		uint16_t	count=sizeof(pattern)/sizeof(ucs2_t);
		for (uint16_t i=0; i<count-1; i++) {

			// initialize pattern
			ucs2charstring::copy(pattern,zerotof);

			// swap in some set of wildcards
			switch (iter) {
				case 0:
					pattern[i]=(ucs2_t)'?';
					break;
				case 1:
					pattern[i]=(ucs2_t)'*';
					break;
				case 2:
					for (uint16_t j=i;
						j<count-1; j++) {
						pattern[j]=(ucs2_t)'?';
					}
					break;
				case 3:
					for (uint16_t j=i;
						j<count-1; j++) {
						pattern[j]=(ucs2_t)'*';
					}
					break;
				case 4:
					for (uint16_t j=i;
						j<count-1; j=j+2) {
						pattern[j]=(ucs2_t)'?';
					}
					break;
				case 5:
					for (uint16_t j=i;
						j<count-1; j=j+2) {
						pattern[j]=(ucs2_t)'*';
					}
					break;
				case 6:
					for (uint16_t j=i;
						j<count-1; j++) {
						if (j%2) {
							pattern[j]=(ucs2_t)'?';
						} else {
							pattern[j]=(ucs2_t)'*';
						}
					}
					break;
				case 7:
					for (uint16_t j=i;
						j<count-1; j++) {
						if (j%2) {
							pattern[j]=(ucs2_t)'*';
						} else {
							pattern[j]=(ucs2_t)'?';
						}
					}
					break;
				case 8:
					pattern[i]=(ucs2_t)'*';
					pattern[i+1]=(ucs2_t)'\0';
					break;
				case 9:
					pattern[0]=(ucs2_t)'*';
                                        // do this rather than just
                                        // "0123456789abcdef"+i+1 directly in
                                        // the copy to silence compiler warnings
                                        // on freebsd 13.1 (and probably other
                                        // platforms)
                                        ucs2literal(chars,"0123456789abcdef");
					ucs2charstring::copy(pattern+1,
								chars+i+1);
					break;
			}

			// build the title
			title.clear();
			title.appendUcs2(string)->appendUcs2((ucs2_t)'=')->
							appendUcs2(pattern);

			// run the test
			char	*t=charstring::duplicateUcs2(title.getString());
			test(t,ucs2charstring::compareWithWildcards(
						string,pattern,
						(ucs2_t)'?',(ucs2_t)'*'));
			delete[] t;
		}
	}
	stdoutput.printf("\n");
	

	// compare versions
	stdoutput.printf("compareVersions...\n");
	test("1 vs. 2",
		ucs2charstring::compareVersions(num1,num2)<0);
	test("2 vs. 1",
		ucs2charstring::compareVersions(num2,num1)>0);
	ucs2literal(num1dot1,"1.1");
	ucs2literal(num1dot2,"1.2");
	test("1.1 vs. 1.2",
		ucs2charstring::compareVersions(num1dot1,num1dot2)<0);
	test("1.2 vs. 1.1",
		ucs2charstring::compareVersions(num1dot2,num1dot1)>0);
	ucs2literal(num1dot1dot1,"1.1.1");
	ucs2literal(num1dot1dot2,"1.1.2");
	test("1.1.1 vs. 1.1.2",
		ucs2charstring::compareVersions(num1dot1dot1,num1dot1dot2)<0);
	test("1.1.2 vs. 1.1.1",
		ucs2charstring::compareVersions(num1dot1dot2,num1dot1dot1)>0);
	ucs2literal(num1dot1dot12,"1.1.12");
	test("1.1.2 vs. 1.1.12",
		ucs2charstring::compareVersions(num1dot1dot2,num1dot1dot12)<0);
	test("1.1.12 vs. 1.1.2",
		ucs2charstring::compareVersions(num1dot1dot12,num1dot1dot2)>0);
	test("1.1 vs. 1.1.1",
		ucs2charstring::compareVersions(num1dot1,num1dot1dot1)<0);
	test("1.1.1 vs. 1.1",
		ucs2charstring::compareVersions(num1dot1dot1,num1dot1)>0);
	ucs2literal(num1dot1dot,"1.1.");
	test("1.1. vs. 1.1.1",
		ucs2charstring::compareVersions(num1dot1dot,num1dot1dot1)<0);
	test("1.1.1 vs. 1.1.",
		ucs2charstring::compareVersions(num1dot1dot1,num1dot1dot)>0);
	stdoutput.printf("\n");
	

	// compare natural
	stdoutput.printf("compareNatural...\n");
	ucs2literal(rudiments1,"rudiments-1");
	ucs2literal(rudiments2,"rudiments-2");
	test("rudiments-1 vs. rudiments-2",
		ucs2charstring::compareNatural(rudiments1,rudiments2)<0);
	test("rudiments-2 vs. rudiments-1",
		ucs2charstring::compareNatural(rudiments2,rudiments1)>0);
	ucs2literal(rudiments1dot1,"rudiments-1.1");
	ucs2literal(rudiments1dot2,"rudiments-1.2");
	test("rudiments-1.1 vs. rudiments-1.2",
		ucs2charstring::compareNatural(rudiments1dot1,
						rudiments1dot2)<0);
	test("rudiments-1.2 vs. rudiments-1.1",
		ucs2charstring::compareNatural(rudiments1dot2,
						rudiments1dot1)>0);
	ucs2literal(rudiments1dot1dot1,"rudiments-1.1.1");
	ucs2literal(rudiments1dot1dot2,"rudiments-1.1.2");
	test("rudiments-1.1.1 vs. rudiments-1.1.2",
		ucs2charstring::compareNatural(rudiments1dot1dot1,
						rudiments1dot1dot2)<0);
	test("rudiments-1.1.2 vs. rudiments-1.1.1",
		ucs2charstring::compareNatural(rudiments1dot1dot2,
						rudiments1dot1dot1)>0);
	ucs2literal(rudiments1dot1dot12,"rudiments-1.1.12");
	test("rudiments-1.1.2 vs. rudiments-1.1.12",
		ucs2charstring::compareNatural(rudiments1dot1dot2,
						rudiments1dot1dot12)<0);
	test("rudiments-1.1.12 vs. rudiments-1.1.2",
		ucs2charstring::compareNatural(rudiments1dot1dot12,
						rudiments1dot1dot2)>0);
	test("rudiments-1.1 vs. rudiments-1.1.1",
		ucs2charstring::compareNatural(rudiments1dot1,
						rudiments1dot1dot1)<0);
	test("rudiments-1.1.1 vs. rudiments-1.1",
		ucs2charstring::compareNatural(rudiments1dot1dot1,
						rudiments1dot1)>0);
	ucs2literal(rudiments1dot1dot,"rudiments-1.1.");
	test("rudiments-1.1. vs. rudiments-1.1.1",
		ucs2charstring::compareNatural(rudiments1dot1dot,
						rudiments1dot1dot1)<0);
	test("rudiments-1.1.1 vs. rudiments-1.1.",
		ucs2charstring::compareNatural(rudiments1dot1dot1,
						rudiments1dot1dot)>0);
	stdoutput.printf("\n");


	// human readable
	stdoutput.printf("humanReadable...\n");
	long double	val=2.1*1024.0*1024.0*1024.0*1024.0*
				1024.0*1024.0*1024.0*1024.0;
	ucs2_t	*sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1B,"2.1B");
	test("humanReadable 2.1B",!ucs2charstring::compare(sval,num2dot1B));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1Y,"2.1Y");
	test("humanReadable 2.1Y",!ucs2charstring::compare(sval,num2dot1Y));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1Z,"2.1Z");
	test("humanReadable 2.1Z",!ucs2charstring::compare(sval,num2dot1Z));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1P,"2.1P");
	test("humanReadable 2.1P",!ucs2charstring::compare(sval,num2dot1P));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1T,"2.1T");
	test("humanReadable 2.1T",!ucs2charstring::compare(sval,num2dot1T));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1G,"2.1G");
	test("humanReadable 2.1G",!ucs2charstring::compare(sval,num2dot1G));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1M,"2.1M");
	test("humanReadable 2.1M",!ucs2charstring::compare(sval,num2dot1M));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1K,"2.1K");
	test("humanReadable 2.1K",!ucs2charstring::compare(sval,num2dot1K));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::humanReadable(val);
	ucs2literal(num2dot1,"2.1");
	test("humanReadable 2.1",!ucs2charstring::compare(sval,num2dot1));
	delete[] sval;

	val=2.1*1000.0*1000.0*1000.0*1000.0*
		1000.0*1000.0*1000.0*1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1B",!ucs2charstring::compare(sval,num2dot1B));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1Y",!ucs2charstring::compare(sval,num2dot1Y));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1Z",!ucs2charstring::compare(sval,num2dot1Z));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1P",!ucs2charstring::compare(sval,num2dot1P));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1T",!ucs2charstring::compare(sval,num2dot1T));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1G",!ucs2charstring::compare(sval,num2dot1G));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1M",!ucs2charstring::compare(sval,num2dot1M));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1K",!ucs2charstring::compare(sval,num2dot1K));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::humanReadable(val,true);
	test("humanReadable 2.1",!ucs2charstring::compare(sval,num2dot1));
	delete[] sval;

	uint64_t	ival=((uint64_t)3)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024);
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3Z,"3Z");
	test("humanReadable 3Z",!ucs2charstring::compare(sval,num3Z));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3P,"3P");
	test("humanReadable 3P",!ucs2charstring::compare(sval,num3P));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3T,"3T");
	test("humanReadable 3T",!ucs2charstring::compare(sval,num3T));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3G,"3G");
	test("humanReadable 3G",!ucs2charstring::compare(sval,num3G));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3M,"3M");
	test("humanReadable 3M",!ucs2charstring::compare(sval,num3M));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	ucs2literal(num3K,"3K");
	test("humanReadable 3K",!ucs2charstring::compare(sval,num3K));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::humanReadable(ival);
	test("humanReadable 3",!ucs2charstring::compare(sval,num3));
	delete[] sval;

	int64_t	sival=((int64_t)-3)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000);
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3Z,"-3Z");
	test("humanReadable -3Z",!ucs2charstring::compare(sval,neg3Z));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3P,"-3P");
	test("humanReadable -3P",!ucs2charstring::compare(sval,neg3P));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3T,"-3T");
	test("humanReadable -3T",!ucs2charstring::compare(sval,neg3T));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3G,"-3G");
	test("humanReadable -3G",!ucs2charstring::compare(sval,neg3G));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3M,"-3M");
	test("humanReadable -3M",!ucs2charstring::compare(sval,neg3M));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3K,"-3K");
	test("humanReadable -3K",!ucs2charstring::compare(sval,neg3K));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::humanReadable(sival,true);
	ucs2literal(neg3,"-3");
	test("humanReadable -3",!ucs2charstring::compare(sval,neg3));
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
	ucs2_t	*c=ucs2charstring::duplicate(chars);
	test("from char",!ucs2charstring::compare(c,ucs2chars));
	delete[] c;
	c=ucs2charstring::duplicate(wchars);
	test("from wchar_t",!ucs2charstring::compare(c,ucs2chars));
	delete[] c;
	stdoutput.printf("\n");


	return 0;
}
