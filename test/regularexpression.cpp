// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/regularexpression.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void printMatches(regularexpression *re) {
	for (int32_t i=0; i<re->getSubstringCount(); i++) {
		stdoutput.printf("match %d start: \"%s\"\n",
					i,re->getSubstringStart(i));
		stdoutput.printf("match %d end  : \"%s\"\n",
					i,re->getSubstringEnd(i));
	}
	stdoutput.printf("%d matches\n\n",re->getSubstringCount());
}

int main(int argc, const char **argv) {

	header("regularexpression");

        const char	*mtc="Dave";
        const char	*str=NULL;

        stdoutput.printf("static match %s\n",mtc);
        str="Hello";
        test(str,!regularexpression::match(str,mtc));
        str="Hello Dave";
        test(str,regularexpression::match(str,mtc));
        str="Hello Dave Goodbye";
        test(str,regularexpression::match(str,mtc));
        str="Dave Goodbye";
        test(str,regularexpression::match(str,mtc));
        stdoutput.printf("\n");


        stdoutput.printf("static match %s (with length)\n",mtc);
        str="Hello";
        test(str,!regularexpression::match(str,4,mtc));
        str="Hello Dave";
        test(str,!regularexpression::match(str,4,mtc));
        test(str,regularexpression::match(str,10,mtc));
        str="Hello Dave Goodbye";
        test(str,!regularexpression::match(str,4,mtc));
        test(str,regularexpression::match(str,11,mtc));
        str="Dave Goodbye";
        test(str,!regularexpression::match(str,3,mtc));
        test(str,regularexpression::match(str,4,mtc));
        stdoutput.printf("\n");


        stdoutput.printf("instance match %s\n",mtc);
        regularexpression       re;
	test("setPattern",re.setPattern(mtc));
        str="Hello";
        test(str,!re.match(str));
	//printMatches(&re);
	test("match count",!re.getSubstringCount());
	test("match start",!re.getSubstringStart(0));
	test("match end",!re.getSubstringEnd(0));

        str="Hello Dave";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==1);
	test("match start",re.getSubstringStart(0)==(str+6));
	test("match end",re.getSubstringEnd(0)==(str+10));

	test("study",re.study());
        str="Hello Dave Goodbye";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==1);
	test("match start",re.getSubstringStart(0)==(str+6));
	test("match end",re.getSubstringEnd(0)==(str+10));

        str="Dave Goodbye";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==1);
	test("match start",re.getSubstringStart(0)==str);
	test("match end",re.getSubstringEnd(0)==(str+4));

        str="Dave Dave Dave";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==1);
	test("match start",re.getSubstringStart(0)==str);
	test("match end",re.getSubstringEnd(0)==(str+4));
        stdoutput.printf("\n");


        stdoutput.printf("optional whitespace match\n");
	mtc="^[ 	\r\n]*((create|CREATE|drop|DROP)[ 	\r\n]+)|"
			"((begin|BEGIN|rollback|ROLLBACK)[ 	\r\n]*)";
	test("setPattern",re.setPattern(mtc));
	test("study",re.study());
        str="create table";
        test(str,re.match(str));
        str=" create  table";
        test(str,re.match(str));
        str="\n create  table";
        test(str,re.match(str));
        str="\n	 create\n	 table";
        test(str,re.match(str));
        str="drop table";
        test(str,re.match(str));
        str=" drop  table";
        test(str,re.match(str));
        str="\n drop  table";
        test(str,re.match(str));
        str="\n	 drop\n	 table";
        test(str,re.match(str));
        str="begin";
        test(str,re.match(str));
        str="begin ";
        test(str,re.match(str));
        str=" begin ";
        test(str,re.match(str));
        str="rollback";
        test(str,re.match(str));
        str="rollback ";
        test(str,re.match(str));
        str=" rollback ";
        test(str,re.match(str));
        stdoutput.printf("\n");


	char	*osname=sys::getOperatingSystemName();

	// solaris, darwin, and irix regex can't handle this, apparently
	if (charstring::compare(osname,"SunOS") &&
		charstring::compare(osname,"Darwin") &&
		charstring::compare(osname,"IRIX")) {

        	stdoutput.printf("wildcard word match\n");
		test("setPattern",re.setPattern("(\\w+) (\\w+)"));
        	str="hello world";
        	test(str,re.match(str));
		//printMatches(&re);
		test("match count",re.getSubstringCount()==3);
		test("match 0 start",re.getSubstringStart(0)==str);
		test("match 0 end",re.getSubstringEnd(0)==(str+11));
		test("match 1 start",re.getSubstringStart(1)==str);
		test("match 1 end",re.getSubstringEnd(1)==(str+5));
		test("match 2 start",re.getSubstringStart(2)==(str+6));
		test("match 2 end",re.getSubstringEnd(2)==(str+11));
        	str="hello world hello world";
        	test(str,re.match(str));
		//printMatches(&re);
		test("match count",re.getSubstringCount()==3);
		test("match 0 start",re.getSubstringStart(0)==str);
		test("match 0 end",re.getSubstringEnd(0)==(str+11));
		test("match 1 start",re.getSubstringStart(1)==str);
		test("match 1 end",re.getSubstringEnd(1)==(str+5));
		test("match 2 start",re.getSubstringStart(2)==(str+6));
		test("match 2 end",re.getSubstringEnd(2)==(str+11));
        	stdoutput.printf("\n");
	}
	delete[] osname;


        stdoutput.printf("NULLs\n");
	test("setPattern",re.setPattern(NULL));
	test("study",re.study());
        test("match",re.match(NULL));
	//printMatches(&re);
	test("match count",!re.getSubstringCount());
	test("match start",!re.getSubstringStart(0));
	test("match end",!re.getSubstringEnd(0));
        stdoutput.printf("\n");

	return 0;
}
