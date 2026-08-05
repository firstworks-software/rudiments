// Copyright (c) David Muse
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
printMatches(&re);
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
		charstring::compare(osname,"IRIX") &&
		charstring::compare(osname,"HP-UX")) {

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


        stdoutput.printf("non-participating groups\n");

	// a group that didn't participate must not hide the groups after it
	test("setPattern",re.setPattern("(a)?(b)"));
        str="b";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==3);
	test("match 0 start",re.getSubstringStart(0)==str);
	test("match 0 end",re.getSubstringEnd(0)==(str+1));
	test("match 1 start",!re.getSubstringStart(1));
	test("match 1 end",!re.getSubstringEnd(1));
	test("match 1 start offset",re.getSubstringStartOffset(1)==-1);
	test("match 1 end offset",re.getSubstringEndOffset(1)==-1);
	test("match 2 start",re.getSubstringStart(2)==str);
	test("match 2 end",re.getSubstringEnd(2)==(str+1));

	// ...and a trailing group that didn't participate must still be
	// a valid index
	test("setPattern",re.setPattern("(a)(b)?"));
        str="a";
        test(str,re.match(str));
	//printMatches(&re);
	test("match count",re.getSubstringCount()==3);
	test("match 0 start",re.getSubstringStart(0)==str);
	test("match 0 end",re.getSubstringEnd(0)==(str+1));
	test("match 1 start",re.getSubstringStart(1)==str);
	test("match 1 end",re.getSubstringEnd(1)==(str+1));
	test("match 2 start",!re.getSubstringStart(2));
	test("match 2 end",!re.getSubstringEnd(2));
	test("match 2 start offset",re.getSubstringStartOffset(2)==-1);
	test("match 2 end offset",re.getSubstringEndOffset(2)==-1);

	// a group count is a property of the pattern, so it doesn't change
	// when the match fails, only the reported substring count does
        str="c";
        test(str,!re.match(str));
	test("match count",!re.getSubstringCount());
	test("match start",!re.getSubstringStart(0));
	test("match end",!re.getSubstringEnd(0));

	// past the last group
	test("setPattern",re.setPattern("(a)(b)"));
        str="ab";
        test(str,re.match(str));
	test("match count",re.getSubstringCount()==3);
	test("past the end start",!re.getSubstringStart(3));
	test("past the end end",!re.getSubstringEnd(3));
	test("past the end start offset",re.getSubstringStartOffset(3)==-1);
	test("past the end end offset",re.getSubstringEndOffset(3)==-1);
        stdoutput.printf("\n");


        stdoutput.printf("match with an offset\n");

	test("setPattern",re.setPattern("a"));
        str="aXa";
	test("offset 0",re.match(str,3,0));
	test("offset 0 start offset",re.getSubstringStartOffset(0)==0);
	test("offset 0 end offset",re.getSubstringEndOffset(0)==1);
	// a pointer into str, not into the copy the POSIX arms make of it
	test("offset 0 start",re.getSubstringStart(0)==str);
	test("offset 0 end",re.getSubstringEnd(0)==(str+1));

	// resuming past the first match finds the second one, and the
	// offsets stay relative to the start of str, not to the offset
	test("offset 1",re.match(str,3,1));
	test("offset 1 start offset",re.getSubstringStartOffset(0)==2);
	test("offset 1 end offset",re.getSubstringEndOffset(0)==3);
	test("offset 1 start",re.getSubstringStart(0)==(str+2));
	test("offset 1 end",re.getSubstringEnd(0)==(str+3));
	test("offset 2",re.match(str,3,2));
	test("offset 2 start offset",re.getSubstringStartOffset(0)==2);

	// an offset at the end of the subject is valid, it just can't match
	test("offset at the end",!re.match(str,3,3));
	test("match count",!re.getSubstringCount());

	// an offset past the end, or a negative one, fails
	test("offset past the end",!re.match(str,3,4));
	test("negative offset",!re.match(str,3,-1));

	// the length still bounds the subject
	test("length bound",!re.match(str,2,2));

	// ^ doesn't match at the offset, the way it does when the caller
	// passes a pointer into the middle of the subject instead
	test("setPattern",re.setPattern("^b"));
        str="ab";
	test("^ at an offset",!re.match(str,2,1));
	test("^ with a pointer into the middle",re.match(str+1,1,0));

	// capture group offsets get the same treatment
	test("setPattern",re.setPattern("(x)(y)"));
        str="xy-xy";
	test("groups at an offset",re.match(str,5,2));
	test("group 0 start offset",re.getSubstringStartOffset(0)==3);
	test("group 0 end offset",re.getSubstringEndOffset(0)==5);
	test("group 1 start offset",re.getSubstringStartOffset(1)==3);
	test("group 1 end offset",re.getSubstringEndOffset(1)==4);
	test("group 2 start offset",re.getSubstringStartOffset(2)==4);
	test("group 2 end offset",re.getSubstringEndOffset(2)==5);
	test("group 2 start",re.getSubstringStart(2)==(str+4));
	test("group 2 end",re.getSubstringEnd(2)==(str+5));

	// ...except a group that didn't participate, which stays unset
	test("setPattern",re.setPattern("(a)?(b)"));
        str="zzb";
	test("optional group at an offset",re.match(str,3,1));
	test("group 1 start offset",re.getSubstringStartOffset(1)==-1);
	test("group 1 end offset",re.getSubstringEndOffset(1)==-1);
	test("group 2 start offset",re.getSubstringStartOffset(2)==2);
	test("group 2 end offset",re.getSubstringEndOffset(2)==3);

	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	// lookbehind can see what precedes the offset, which it can't when
	// the caller passes a pointer into the middle of the subject
	test("setPattern",re.setPattern("(?<=a)b"));
        str="ab";
	test("lookbehind at an offset",re.match(str,2,1));
	test("lookbehind start offset",re.getSubstringStartOffset(0)==1);
	test("lookbehind with a pointer into the middle",!re.match(str+1,1,0));
	#endif

	// walk a subject the way a global substitution does
	test("setPattern",re.setPattern("o"));
        str="foo boo";
	int32_t	walkoffset=0;
	int32_t	walkcount=0;
	while (re.match(str,7,walkoffset)) {
		walkcount++;
		walkoffset=re.getSubstringEndOffset(0);
	}
	test("global walk",walkcount==4);
	test("global walk offset",walkoffset==7);
        stdoutput.printf("\n");


        stdoutput.printf("named capture groups\n");

	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	test("setPattern",re.setPattern("(?<year>[0-9]{4})-(?<month>[0-9]{2})"));
        str="on 2026-08 sometime";
        test(str,re.match(str));

	// a name resolves to the index of the group that has it
	test("year index",re.getSubstringIndex("year")==1);
	test("month index",re.getSubstringIndex("month")==2);

	// ...and an unknown one, or a NULL one, resolves to nothing
	test("unknown index",re.getSubstringIndex("day")==-1);
	test("NULL index",re.getSubstringIndex(NULL)==-1);

	// the name-taking getters agree with the index-taking ones
	test("year start",re.getSubstringStart("year")==(str+3));
	test("year end",re.getSubstringEnd("year")==(str+7));
	test("year start offset",re.getSubstringStartOffset("year")==3);
	test("year end offset",re.getSubstringEndOffset("year")==7);
	test("month start",re.getSubstringStart("month")==(str+8));
	test("month end",re.getSubstringEnd("month")==(str+10));
	test("month start offset",re.getSubstringStartOffset("month")==8);
	test("month end offset",re.getSubstringEndOffset("month")==10);
	test("year start by index",
		re.getSubstringStart("year")==re.getSubstringStart(1));
	test("month start by index",
		re.getSubstringStart("month")==re.getSubstringStart(2));

	// an unknown name reports nothing rather than the whole match
	test("unknown start",!re.getSubstringStart("day"));
	test("unknown end",!re.getSubstringEnd("day"));
	test("unknown start offset",re.getSubstringStartOffset("day")==-1);
	test("unknown end offset",re.getSubstringEndOffset("day")==-1);
	test("NULL start",!re.getSubstringStart((const char *)NULL));
	test("NULL end",!re.getSubstringEnd((const char *)NULL));

	// a named group that didn't participate keeps its index, but
	// reports no substring
	test("setPattern",re.setPattern("(?<a>x)?(?<b>y)"));
        str="y";
        test(str,re.match(str));
	test("a index",re.getSubstringIndex("a")==1);
	test("b index",re.getSubstringIndex("b")==2);
	test("a start",!re.getSubstringStart("a"));
	test("a start offset",re.getSubstringStartOffset("a")==-1);
	test("b start",re.getSubstringStart("b")==str);
	test("b end offset",re.getSubstringEndOffset("b")==1);

	// a name that more than one group has resolves to nothing, since
	// there's no single index to report.  (?J) turns duplicate names
	// on from inside the pattern, so this is reachable even though
	// setPattern() never asks for them.
	test("setPattern",re.setPattern("(?J)(?<n>aa)|(?<n>bb)"));
        str="zzaa";
        test(str,re.match(str));
	test("duplicate name index",re.getSubstringIndex("n")==-1);
	test("duplicate name start",!re.getSubstringStart("n"));
	test("duplicate name start offset",re.getSubstringStartOffset("n")==-1);

	// ...but a branch reset gives every branch the same index, so the
	// name still resolves
	test("setPattern",re.setPattern("(?|(?<n>aa)|(?<n>bb))"));
        str="zzbb";
        test(str,re.match(str));
	test("branch reset index",re.getSubstringIndex("n")==1);
	test("branch reset start",re.getSubstringStart("n")==(str+2));
	test("branch reset end",re.getSubstringEnd("n")==(str+4));

	// the index is a property of the pattern, so it resolves before a
	// match has been tried, and stops resolving when the pattern changes
	test("setPattern",re.setPattern("(?<zzz>z)"));
	test("index before a match",re.getSubstringIndex("zzz")==1);
	test("no substring before a match",!re.getSubstringStart("zzz"));
	test("setPattern",re.setPattern("(z)"));
	test("index after a new pattern",re.getSubstringIndex("zzz")==-1);
	#else
	// the POSIX engines have no named capture groups
	test("setPattern",re.setPattern("(a)(b)"));
        str="ab";
        test(str,re.match(str));
	test("index",re.getSubstringIndex("a")==-1);
	test("NULL index",re.getSubstringIndex(NULL)==-1);
	test("start",!re.getSubstringStart("a"));
	test("end",!re.getSubstringEnd("a"));
	test("start offset",re.getSubstringStartOffset("a")==-1);
	test("end offset",re.getSubstringEndOffset("a")==-1);
	#endif
        stdoutput.printf("\n");


        stdoutput.printf("NULLs\n");
	test("setPattern",re.setPattern(NULL));
	test("study",re.study());
        test("match",re.match(NULL));
        test("match with an offset",re.match(NULL,0,0));
	test("index by name",re.getSubstringIndex("name")==-1);
	test("start by name",!re.getSubstringStart("name"));
	//printMatches(&re);
	test("match count",!re.getSubstringCount());
	test("match start",!re.getSubstringStart(0));
	test("match end",!re.getSubstringEnd(0));
        stdoutput.printf("\n");


        stdoutput.printf("lifecycle\n");

	// destruct after a match with a length, which allocates the
	// string copy on the POSIX regex arm
	{
		regularexpression	re2;
		test("setPattern",re2.setPattern("Dave"));
		test("match",re2.match("Hello Dave",10));
	}

	// destruct with no pattern ever set
	{
		regularexpression	re2;
	}

	// match with no pattern ever set
	{
		regularexpression	re2;
		test("match",!re2.match("Hello Dave",10));
		test("match with an offset",!re2.match("Hello Dave",10,6));
		test("match count",!re2.getSubstringCount());
		test("match start",!re2.getSubstringStart(0));
		test("match end",!re2.getSubstringEnd(0));
	}

	// re-use with a new pattern
	{
		regularexpression	re2;
		test("setPattern",re2.setPattern("Dave"));
		test("match",re2.match("Hello Dave",10));
		test("setPattern again",re2.setPattern("Goodbye"));
		test("match",re2.match("Hello Goodbye",13));
		test("match count",re2.getSubstringCount()==1);
		test("no match",!re2.match("Hello Dave",10));
		test("match count",!re2.getSubstringCount());
	}
        stdoutput.printf("\n");

	return 0;
}
