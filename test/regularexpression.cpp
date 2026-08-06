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

	// an offset past the end, or a negative one, fails, and leaves
	// nothing to report
	test("offset past the end",!re.match(str,3,4));
	test("match count past the end",!re.getSubstringCount());
	test("negative offset",!re.match(str,3,-1));
	test("match count negative",!re.getSubstringCount());

	// the POSIX arms skip their copy of the subject for an offset they
	// won't run the engine at, so the match after one has to set it up
	// itself rather than inherit it
	test("match after an out-of-range offset",re.match(str,3,0));
	test("start offset after",re.getSubstringStartOffset(0)==0);
	test("end offset after",re.getSubstringEndOffset(0)==1);
	test("start after",re.getSubstringStart(0)==str);

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


        stdoutput.printf("matchNext\n");

	// the same pattern of characters at several locations, which only
	// reported the first one before matchNext()
	test("setPattern",re.setPattern("Dave"));
        str="Dave, Dave, Dave!";
	test("first",re.match(str));
	test("first start offset",re.getSubstringStartOffset(0)==0);
	test("first end offset",re.getSubstringEndOffset(0)==4);
	test("second",re.matchNext());
	test("second start offset",re.getSubstringStartOffset(0)==6);
	test("second end offset",re.getSubstringEndOffset(0)==10);
	test("second start",re.getSubstringStart(0)==(str+6));
	test("second end",re.getSubstringEnd(0)==(str+10));
	test("third",re.matchNext());
	test("third start offset",re.getSubstringStartOffset(0)==12);
	test("third end offset",re.getSubstringEndOffset(0)==16);
	test("no fourth",!re.matchNext());

	// when it returns false the substring methods report nothing, the
	// same state a failed match() leaves, and it keeps returning false
	test("match count",!re.getSubstringCount());
	test("match start",!re.getSubstringStart(0));
	test("match end",!re.getSubstringEnd(0));
	test("match start offset",re.getSubstringStartOffset(0)==-1);
	test("match end offset",re.getSubstringEndOffset(0)==-1);
	test("no fifth",!re.matchNext());
	test("no sixth",!re.matchNext());

	// match() starts the walk over
	test("restart",re.match(str));
	test("restart start offset",re.getSubstringStartOffset(0)==0);

	// the capture groups report the match the walk is on, which they
	// didn't before either - the same subject twice over gave the same
	// groups as one copy of it
	test("setPattern",re.setPattern("([a-z]+) ([a-z]+)"));
        str="hello world hello world";
	test("groups first",re.match(str));
	test("match count",re.getSubstringCount()==3);
	test("groups first 0 start",re.getSubstringStartOffset(0)==0);
	test("groups first 0 end",re.getSubstringEndOffset(0)==11);
	test("groups first 1 start",re.getSubstringStartOffset(1)==0);
	test("groups first 1 end",re.getSubstringEndOffset(1)==5);
	test("groups first 2 start",re.getSubstringStartOffset(2)==6);
	test("groups first 2 end",re.getSubstringEndOffset(2)==11);
	test("groups second",re.matchNext());
	test("groups second 0 start",re.getSubstringStartOffset(0)==12);
	test("groups second 0 end",re.getSubstringEndOffset(0)==23);
	test("groups second 1 start",re.getSubstringStartOffset(1)==12);
	test("groups second 1 end",re.getSubstringEndOffset(1)==17);
	test("groups second 2 start",re.getSubstringStartOffset(2)==18);
	test("groups second 2 end",re.getSubstringEndOffset(2)==23);
	test("groups no third",!re.matchNext());

	// An empty match is reported like any other, and the walk resumes
	// one character past where it started, so the same one can't come
	// back twice.  This is what perl's /g gives for a* against "xaay".
	test("setPattern",re.setPattern("a*"));
        str="xaay";
	test("empty first",re.match(str));
	test("empty first start offset",re.getSubstringStartOffset(0)==0);
	test("empty first end offset",re.getSubstringEndOffset(0)==0);
	test("empty second",re.matchNext());
	test("empty second start offset",re.getSubstringStartOffset(0)==1);
	test("empty second end offset",re.getSubstringEndOffset(0)==3);
	test("empty third",re.matchNext());
	test("empty third start offset",re.getSubstringStartOffset(0)==3);
	test("empty third end offset",re.getSubstringEndOffset(0)==3);

	// including one at the very end of the subject, which the walk only
	// reaches because an offset equal to the length is still matchable
	test("empty at the end",re.matchNext());
	test("empty at the end start offset",re.getSubstringStartOffset(0)==4);
	test("empty at the end end offset",re.getSubstringEndOffset(0)==4);
	test("empty no more",!re.matchNext());

	// an empty match found well past the resume point still comes back
	// only once, since the walk moves on from the match rather than
	// from the resume point
	test("setPattern",re.setPattern("$"));
        str="abc";
	test("empty ahead",re.match(str));
	test("empty ahead start offset",re.getSubstringStartOffset(0)==3);
	test("empty ahead end offset",re.getSubstringEndOffset(0)==3);
	test("empty ahead no more",!re.matchNext());

	// a subject with nothing in it
	test("setPattern",re.setPattern("x"));
	test("empty subject",!re.match(""));
	test("empty subject next",!re.matchNext());

	// the length still bounds the walk
	test("setPattern",re.setPattern("o"));
        str="foo boo";
	test("bounded first",re.match(str,3));
	test("bounded first start offset",re.getSubstringStartOffset(0)==1);
	test("bounded second",re.matchNext());
	test("bounded second start offset",re.getSubstringStartOffset(0)==2);
	test("bounded no third",!re.matchNext());

	// a walk can start partway through the subject too
	test("offset first",re.match(str,7,3));
	test("offset first start offset",re.getSubstringStartOffset(0)==5);
	test("offset second",re.matchNext());
	test("offset second start offset",re.getSubstringStartOffset(0)==6);
	test("offset no third",!re.matchNext());

	// A walk armed by match(str) and one armed by match(str,length)
	// hand the POSIX regular expression engines different buffers - the
	// caller's string, and a null-terminated copy of it - but the
	// substrings come back relative to the caller's string either way.
	test("walk from match(str)",re.match(str));
	test("walk from match(str) start",re.getSubstringStart(0)==(str+1));
	test("walk from match(str) next",re.matchNext());
	test("walk from match(str) next start",
					re.getSubstringStart(0)==(str+2));
	test("walk from match(str,length)",re.match(str,7));
	test("walk from match(str,length) start",
					re.getSubstringStart(0)==(str+1));
	test("walk from match(str,length) next",re.matchNext());
	test("walk from match(str,length) next start",
					re.getSubstringStart(0)==(str+2));

	// there's nothing to continue before any match at all
	{
		regularexpression	re2;
		test("setPattern",re2.setPattern("o"));
		test("next before any match",!re2.matchNext());
		test("match count",!re2.getSubstringCount());
	}

	// or after a failed one
	test("setPattern",re.setPattern("z"));
	test("failed match",!re.match(str));
	test("next after a failed match",!re.matchNext());

	// setPattern() ends a walk in progress
	test("setPattern",re.setPattern("o"));
	test("match",re.match(str));
	test("setPattern mid-walk",re.setPattern("o"));
	test("next after setPattern",!re.matchNext());

	// and so does a null pattern, which makes every accessor report
	// nothing anyway
	test("match",re.match(str));
	test("setPattern NULL",re.setPattern(NULL));
	test("next with a null pattern",!re.matchNext());

	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	// a lookahead matches empty everywhere it holds, and its capture
	// groups still report what they saw ahead of the match
	test("setPattern",re.setPattern("(?=(ab))"));
        str="abab";
	test("lookahead first",re.match(str));
	test("lookahead first start offset",re.getSubstringStartOffset(0)==0);
	test("lookahead first end offset",re.getSubstringEndOffset(0)==0);
	test("lookahead first group 1 end",re.getSubstringEndOffset(1)==2);
	test("lookahead second",re.matchNext());
	test("lookahead second start offset",re.getSubstringStartOffset(0)==2);
	test("lookahead second end offset",re.getSubstringEndOffset(0)==2);
	test("lookahead no third",!re.matchNext());
	#endif
        stdoutput.printf("\n");


        stdoutput.printf("compile options\n");

	// case insensitivity, which every arm can honor
	test("setPattern",re.setPattern("dave"));
        str="Hello DAVE";
	test("no options",!re.match(str));
	test("setPattern",
		re.setPattern("dave",REGULAR_EXPRESSION_CASE_INSENSITIVE));
	test("case insensitive",re.match(str));
	test("start",re.getSubstringStart(0)==(str+6));
        str="Hello dave";
	test("case insensitive, same case",re.match(str));
        str="Hello Steve";
	test("case insensitive, no match",!re.match(str));

	// multiline, which makes ^ match after a newline
	test("setPattern",re.setPattern("^b"));
        str="a\nb";
	test("no options",!re.match(str));
	test("setPattern",re.setPattern("^b",REGULAR_EXPRESSION_MULTILINE));
	test("multiline",re.match(str));
	test("start",re.getSubstringStart(0)==(str+2));

	// options or'ed together
	test("setPattern",re.setPattern("^b",
				REGULAR_EXPRESSION_MULTILINE|
				REGULAR_EXPRESSION_CASE_INSENSITIVE));
        str="a\nB";
	test("multiline and case insensitive",re.match(str));

	// an option that isn't one of the flags is ignored, rather than
	// reaching the engine and failing the compile
	test("setPattern",re.setPattern("dave",0x8000));
        str="Hello DAVE";
	test("unknown option",!re.match(str));
	test("setPattern",re.setPattern("dave",
				0x8000|REGULAR_EXPRESSION_CASE_INSENSITIVE));
	test("unknown option alongside a real one",re.match(str));

	// dot-all, which the two arms spell differently.  On PCRE, . doesn't
	// match a newline without it.  On the POSIX arms it does with or
	// without, and asking for multiline is what turns it off, since
	// there they're one switch rather than two.
	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	test("setPattern",re.setPattern("a.b"));
        str="a\nb";
	test("no options",!re.match(str));
	test("setPattern",re.setPattern("a.b",REGULAR_EXPRESSION_DOT_ALL));
	test("dot all",re.match(str));
	test("setPattern",re.setPattern("a.b",
				REGULAR_EXPRESSION_DOT_ALL|
				REGULAR_EXPRESSION_MULTILINE));
	test("dot all and multiline",re.match(str));
	// a negated character class isn't affected by multiline here
	test("setPattern",re.setPattern("a[^x]b"));
	test("negated class",re.match(str));
	test("setPattern",re.setPattern("a[^x]b",
				REGULAR_EXPRESSION_MULTILINE));
	test("negated class, multiline",re.match(str));
	#else
	test("setPattern",re.setPattern("a.b"));
        str="a\nb";
	test("no options",re.match(str));
	test("setPattern",re.setPattern("a.b",REGULAR_EXPRESSION_DOT_ALL));
	test("dot all",re.match(str));
	// multiline drags dot-all off with it, so this one can't match
	test("setPattern",re.setPattern("a.b",
				REGULAR_EXPRESSION_DOT_ALL|
				REGULAR_EXPRESSION_MULTILINE));
	test("dot all dropped for multiline",!re.match(str));
	// ...and it takes a negated character class with it, which is the
	// third thing REG_NEWLINE does
	test("setPattern",re.setPattern("a[^x]b"));
	test("negated class",re.match(str));
	test("setPattern",re.setPattern("a[^x]b",
				REGULAR_EXPRESSION_MULTILINE));
	test("negated class, multiline",!re.match(str));
	#endif

	// the one-argument setPattern() still compiles with no options
	test("setPattern",re.setPattern("dave"));
        str="Hello DAVE";
	test("still case sensitive",!re.match(str));
	test("setPattern",re.setPattern("dave",0));
	test("no options is the same thing",!re.match(str));

	// a bad pattern still fails to compile, with options or without
	test("bad pattern",!re.setPattern("(unterminated"));
	test("bad pattern with options",
		!re.setPattern("(unterminated",
				REGULAR_EXPRESSION_CASE_INSENSITIVE));
	test("no match",!re.match(str));
	test("match count",!re.getSubstringCount());
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
