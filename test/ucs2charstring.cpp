// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/wcharstring.h>
#include <rudiments/sys.h>
#include <rudiments/locale.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include <rudiments/signalclasses.h>
#include "test.cpp"

// returns an exact-size copy of the ASCII string "str"; NULL stays NULL
static ucs2_t *emptyPatternCopy(const char *str) {
	return (str)?ucs2charstring::duplicate(str):NULL;
}

// true if "result" holds the ASCII string "expected", or both are NULL
static bool emptyPatternMatches(const ucs2_t *result, const char *expected) {
	if (!result || !expected) {
		return (!result && !expected);
	}
	ucs2_t	*e=emptyPatternCopy(expected);
	bool	match=!ucs2charstring::compare(result,e);
	delete[] e;
	return match;
}

// checks replace() and replaceIgnoringCase() on exact-size heap copies of
// the inputs, so that AddressSanitizer sees any read outside them
static void emptyPatternReplaceTest(const char *label, const char *str,
					const char *oldstr,
					const char *newstr,
					const char *expected,
					const char *expectedic) {
	const ucs2_t	*s=emptyPatternCopy(str);
	const ucs2_t	*o=emptyPatternCopy(oldstr);
	const ucs2_t	*n=emptyPatternCopy(newstr);
	stringbuffer	name;
	name.append(label)->append(" replace");
	ucs2_t	*result=ucs2charstring::replace(s,o,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase");
	result=ucs2charstring::replaceIgnoringCase(s,o,n);
	test(name.getString(),emptyPatternMatches(result,expectedic));
	delete[] result;
	delete[] s;
	delete[] o;
	delete[] n;
}

// checks the set forms of replace() and replaceIgnoringCase() on exact-size
// heap copies of the inputs; if "lengths" is NULL, the lengths are those of
// the members (the string forms are also checked)
static void emptyPatternSetTest(const char *label, const char *str,
					const char * const *oldset,
					const size_t *lengths,
					const char * const *newset,
					const char *expected,
					const char *expectedic) {
	size_t	count=0;
	while (oldset[count]) {
		count++;
	}
	ucs2_t		*s=emptyPatternCopy(str);
	const ucs2_t	**o=new const ucs2_t *[count+1];
	const ucs2_t	**n=new const ucs2_t *[count+1];
	size_t		*l=new size_t[count+1];
	for (size_t i=0; i<count; i++) {
		o[i]=emptyPatternCopy(oldset[i]);
		n[i]=emptyPatternCopy(newset[i]);
		l[i]=(lengths)?lengths[i]:ucs2charstring::getLength(o[i]);
	}
	o[count]=NULL;
	n[count]=NULL;
	stringbuffer	name;
	ucs2_t	*result=NULL;
	if (!lengths) {
		name.append(label)->append(" replace");
		result=ucs2charstring::replace(s,o,n);
		test(name.getString(),emptyPatternMatches(result,expected));
		delete[] result;
		name.clear();
		name.append(label)->append(" replaceIgnoringCase");
		result=ucs2charstring::replaceIgnoringCase(s,o,n);
		test(name.getString(),emptyPatternMatches(result,expectedic));
		delete[] result;
	}
	name.clear();
	name.append(label)->append(" replace length");
	result=ucs2charstring::replace(s,o,l,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase length");
	result=ucs2charstring::replaceIgnoringCase(s,o,l,n);
	test(name.getString(),emptyPatternMatches(result,expectedic));
	delete[] result;
	for (size_t i=0; i<count; i++) {
		delete[] o[i];
		delete[] n[i];
	}
	delete[] o;
	delete[] n;
	delete[] l;
	delete[] s;
}

// checks one result of a set form of replace() or replaceIgnoringCase():
// it must be a new copy, equal to "expected"
static void nullSetCheck(const char *label, const ucs2_t *str, ucs2_t *result,
							const char *expected) {
	test(label,(result && result!=str &&
			emptyPatternMatches(result,expected)));
	delete[] result;
}

// checks the set forms of replace() and replaceIgnoringCase() with a NULL
// oldstrset, newstrset or oldstrlen, which each count as an empty set, on
// exact-size heap copies of the inputs; a NULL member of newstrset deletes
// its match; "bx" is the result of replacing b with X
static void nullSetTest(const char *label, const char *str, const char *bx,
							const char *nob) {
	ucs2_t		*s=emptyPatternCopy(str);
	const ucs2_t	**o=new const ucs2_t *[2];
	const ucs2_t	**n=new const ucs2_t *[2];
	const ucs2_t	**d=new const ucs2_t *[2];
	size_t		*l=new size_t[1];
	o[0]=emptyPatternCopy("b");
	o[1]=NULL;
	n[0]=emptyPatternCopy("X");
	n[1]=NULL;
	d[0]=NULL;
	d[1]=NULL;
	l[0]=1;
	const ucs2_t * const	*nullset=NULL;
	size_t			*nulllen=NULL;
	stringbuffer		name;
	for (int ic=0; ic<2; ic++) {
		const char	*fn=(ic)?" replaceIgnoringCase":" replace";
		#define NULLSETNAME(what) \
			name.clear(); \
			name.append(label)->append(fn)->append(what)
		#define NULLSETCALL(args) \
			((ic)?ucs2charstring::replaceIgnoringCase args: \
				ucs2charstring::replace args)
		NULLSETNAME(" NULL oldstrset");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,nullset,n)),str);
		NULLSETNAME(" NULL newstrset");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,nullset)),str);
		NULLSETNAME(" NULL oldstrset, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,nullset,l,n)),str);
		NULLSETNAME(" NULL oldstrlen");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,nulllen,n)),str);
		NULLSETNAME(" NULL newstrset, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,l,nullset)),str);
		NULLSETNAME(" NULL str");
		test(name.getString(),!NULLSETCALL((NULL,nullset,nullset)));
		NULLSETNAME(" NULL str, length");
		test(name.getString(),
			!NULLSETCALL((NULL,nullset,nulllen,nullset)));
		NULLSETNAME(" valid arrays");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,n)),bx);
		NULLSETNAME(" valid arrays, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,l,n)),bx);
		NULLSETNAME(" NULL member of newstrset");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,d)),nob);
		NULLSETNAME(" NULL member of newstrset, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,l,d)),nob);
		#undef NULLSETNAME
		#undef NULLSETCALL
	}
	delete[] o[0];
	delete[] n[0];
	delete[] o;
	delete[] n;
	delete[] d;
	delete[] l;
	delete[] s;
}

// checks strip(str1,str2) on exact-size heap copies of the inputs
static void emptyPatternStripTest(const char *label, const char *str1,
					const char *str2,
					const char *expected,
					bool expectedreturn) {
	ucs2_t	*s=emptyPatternCopy(str1);
	const ucs2_t	*p=emptyPatternCopy(str2);
	bool	retval=ucs2charstring::strip(s,p);
	test(label,retval==expectedreturn &&
			emptyPatternMatches(s,expected));
	delete[] s;
	delete[] p;
}

// checks findLast and findLastIgnoringCase, string and character forms,
// const and non-const, on exact-size heap copies; the expected values are
// offsets into the haystack, or -1 for NULL
static void findLastTest(const char *label, const char *hay,
				const char *needle,
				int exp, int expic) {
	ucs2_t	*h=emptyPatternCopy(hay);
	ucs2_t	*n=emptyPatternCopy(needle);
	const ucs2_t	*ch=h;
	const ucs2_t	*r;
	stringbuffer	name;
	name.append(label)->append(" findLast");
	r=ucs2charstring::findLast(ch,(const ucs2_t *)n);
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=ucs2charstring::findLast(h,(const ucs2_t *)n);
	name.clear();
	name.append(label)->append(" findLast non-const");
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=ucs2charstring::findLastIgnoringCase(ch,(const ucs2_t *)n);
	name.clear();
	name.append(label)->append(" findLastIgnoringCase");
	test(name.getString(),(r)?(r-h==expic):(expic==-1));
	r=ucs2charstring::findLastIgnoringCase(h,(const ucs2_t *)n);
	name.clear();
	name.append(label)->append(" findLastIgnoringCase non-const");
	test(name.getString(),(r)?(r-h==expic):(expic==-1));
	if (n[0] && !n[1]) {
		r=ucs2charstring::findLast(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLast char");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
		r=ucs2charstring::findLastIgnoringCase(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase char");
		test(name.getString(),(r)?(r-h==expic):(expic==-1));
		r=ucs2charstring::findLast(h,n[0]);
		name.clear();
		name.append(label)->append(" findLast char non-const");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
		r=ucs2charstring::findLastIgnoringCase(h,n[0]);
		name.clear();
		name.append(label);
		name.append(" findLastIgnoringCase char non-const");
		test(name.getString(),(r)?(r-h==expic):(expic==-1));
	}
	if (!n[0]) {
		r=ucs2charstring::findLast(ch,(ucs2_t)0);
		name.clear();
		name.append(label)->append(" findLast terminator");
		test(name.getString(),r==h+ucs2charstring::getLength(h));
		r=ucs2charstring::findLastIgnoringCase(ch,(ucs2_t)0);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase terminator");
		test(name.getString(),r==h+ucs2charstring::getLength(h));
	}
	delete[] h;
	delete[] n;
}

// checks the NULL arguments of findLast and findLastIgnoringCase
static void findLastNullTest() {
	ucs2_t	*h=emptyPatternCopy("abc");
	const ucs2_t	*ch=h;
	test("findLast NULL haystack",
		ucs2charstring::findLast((const ucs2_t *)NULL,ch)==NULL);
	test("findLast NULL needle",
		ucs2charstring::findLast(ch,(const ucs2_t *)NULL)==NULL);
	test("findLastIgnoringCase NULL haystack",
		ucs2charstring::findLastIgnoringCase(
				(const ucs2_t *)NULL,ch)==NULL);
	test("findLastIgnoringCase NULL needle",
		ucs2charstring::findLastIgnoringCase(
				ch,(const ucs2_t *)NULL)==NULL);
	test("findLast char NULL haystack",
		ucs2charstring::findLast((const ucs2_t *)NULL,
						(ucs2_t)'a')==NULL);
	test("findLast char 0, NULL haystack",
		ucs2charstring::findLast((const ucs2_t *)NULL,
						(ucs2_t)0)==NULL);
	test("findLastIgnoringCase char NULL haystack",
		ucs2charstring::findLastIgnoringCase((const ucs2_t *)NULL,
							(ucs2_t)'a')==NULL);
	test("findLastIgnoringCase char 0, NULL haystack",
		ucs2charstring::findLastIgnoringCase((const ucs2_t *)NULL,
							(ucs2_t)0)==NULL);
	delete[] h;
}

// checks leftTrim, strip and stripSet on exact-size heap copies; which is
// 0 leftTrim(str), 1 leftTrim(str,ch), 2 strip(str,ch), 3 strip(str1,str2),
// 4 stripSet(str,set); ch is x, str2 is "xy" and set is "xy"
static void sizeIndexTest(const char *label, int which, const char *str,
				const char *expected, bool expectedreturn) {
	ucs2_t	*s=emptyPatternCopy(str);
	ucs2_t	*xy=emptyPatternCopy("xy");
	bool	retval=expectedreturn;
	switch (which) {
		case 0:
			ucs2charstring::leftTrim(s);
			break;
		case 1:
			ucs2charstring::leftTrim(s,(ucs2_t)'x');
			break;
		case 2:
			retval=ucs2charstring::strip(s,(ucs2_t)'x');
			break;
		case 3:
			retval=ucs2charstring::strip(s,(const ucs2_t *)xy);
			break;
		default:
			retval=ucs2charstring::stripSet(s,(const ucs2_t *)xy);
			break;
	}
	test(label,retval==expectedreturn &&
			emptyPatternMatches(s,expected));
	delete[] s;
	delete[] xy;
}

// checks the length-taking forms of duplicate() for one length of a string
// that is at least that long
static void duplicateLengthTest(const char *label, const char *str,
				const wchar_t *wstr, const ucs2_t *ustr,
				size_t len) {
	bool		big=sys::isBigEndian();
	stringbuffer	name;
	ucs2_t		*result=ucs2charstring::duplicate(ustr,len);
	name.append(label);
	name.append(" ucs2");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
	result=ucs2charstring::duplicate(str,len);
	name.clear();
	name.append(label);
	name.append(" char");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
	result=ucs2charstring::duplicate(str,len,big);
	name.clear();
	name.append(label);
	name.append(" char bigendian");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
	result=ucs2charstring::duplicate(wstr,len);
	name.clear();
	name.append(label);
	name.append(" wchar_t");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
	result=ucs2charstring::duplicate(wstr,len,big);
	name.clear();
	name.append(label);
	name.append(" wchar_t bigendian");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
	result=ucs2charstring::duplicate(wstr,len,(ucs2_t)'#',big);
	name.clear();
	name.append(label);
	name.append(" wchar_t replacement bigendian");
	test(name.getString(),result &&
		ucs2charstring::getLength(result)==len &&
		!ucs2charstring::compare(result,ustr,len));
	delete[] result;
}

// checks pad(), where "expected" is NULL if pad() must return NULL
static void padTest(const char *label, const ucs2_t *str, ucs2_t padchar,
			int16_t direction, uint64_t totallen,
			const ucs2_t *expected) {
	ucs2_t	*result=ucs2charstring::pad(str,padchar,direction,totallen);
	test(label,(expected)?
			(result && !ucs2charstring::compare(result,expected)):
			!result);
	delete[] result;
}

// checks pad(), element by element, against a result built here
static bool padMatches(const ucs2_t *str, ucs2_t padchar,
			int16_t direction, uint64_t totallen) {
	ucs2_t	*result=ucs2charstring::pad(str,padchar,direction,totallen);
	if (!result) {
		return false;
	}
	uint64_t	len=(str)?ucs2charstring::getLength(str):0;
	uint64_t	lead=0;
	if (len<totallen) {
		lead=(direction<0)?totallen-len:
			(direction>0)?0:(totallen-len)/2;
	}
	bool	matches=(result[totallen]==0);
	for (uint64_t i=0; i<totallen; i++) {
		ucs2_t	expected=padchar;
		if (i>=lead && i-lead<len) {
			expected=str[i-lead];
		}
		if (result[i]!=expected) {
			matches=false;
		}
	}
	delete[] result;
	return matches;
}

// checks convertAmount() with and without padding against charstring
static bool amountMatches(int64_t amount) {
	char	*narrow=charstring::convertAmount(amount);
	ucs2_t	*expected=ucs2charstring::duplicate(narrow);
	ucs2_t	*result=ucs2charstring::convertAmount(amount);
	bool	matches=(result && !ucs2charstring::compare(result,expected));
	delete[] result;
	delete[] expected;
	delete[] narrow;
	uint16_t	paddings[]={0,3,8,12,20};
	for (size_t i=0; i<sizeof(paddings)/sizeof(paddings[0]); i++) {
		narrow=charstring::convertAmount(amount,paddings[i]);
		expected=ucs2charstring::duplicate(narrow);
		result=ucs2charstring::convertAmount(amount,paddings[i]);
		if (!result || ucs2charstring::compare(result,expected) ||
			ucs2charstring::getLength(result)!=
				charstring::getLength(narrow)) {
			matches=false;
		}
		delete[] result;
		delete[] expected;
		delete[] narrow;
	}
	return matches;
}

// converts the ASCII string "str" with convertToFloatC()
static long double floatC(const char *str) {
	ucs2_t		*ustr=ucs2charstring::duplicate(str);
	long double	result=ucs2charstring::convertToFloatC(ustr);
	delete[] ustr;
	return result;
}

// checks insertString() on exact-size heap copies of the inputs, so that
// AddressSanitizer sees any read outside them; a NULL stays NULL
static ucs2_t *exactCopy(const char *str) {
	if (!str) {
		return NULL;
	}
	size_t	len=charstring::getLength(str);
	ucs2_t	*copy=new ucs2_t[len+1];
	for (size_t i=0; i<=len; i++) {
		copy[i]=(ucs2_t)str[i];
	}
	return copy;
}

static void insertStringExactTest(const char *label, const char *dest,
					const char *src, uint64_t index,
					const char *expected) {
	ucs2_t	*d=exactCopy(dest);
	ucs2_t	*s=exactCopy(src);
	ucs2_t	*e=exactCopy(expected);
	ucs2_t	*result=ucs2charstring::insertString(d,s,index);
	test(label,result && !ucs2charstring::compare(result,e));
	delete[] result;
	delete[] d;
	delete[] s;
	delete[] e;
}

// checks insertString()
static void insertStringTest(const char *label, const ucs2_t *dest,
				const ucs2_t *src, uint64_t index,
				const ucs2_t *expected) {
	ucs2_t	*result=ucs2charstring::insertString(dest,src,index);
	test(label,result && !ucs2charstring::compare(result,expected));
	delete[] result;
}

// checks the out-pointer forms of escape() and unescape()
static void escapeLengthTest(const char *label, const ucs2_t *input,
				uint64_t inputlen, const ucs2_t *characters,
				const ucs2_t *expectedescaped) {
	ucs2_t		*output=NULL;
	uint64_t	outputlen=0;
	ucs2charstring::escape(input,inputlen,&output,&outputlen,characters);
	test(label,output && !ucs2charstring::compare(output,expectedescaped) &&
			outputlen==ucs2charstring::getLength(expectedescaped));
	ucs2_t		*escaped=output;
	uint64_t	escapedlen=outputlen;
	output=NULL;
	outputlen=0;
	ucs2charstring::unescape(escaped,escapedlen,&output,&outputlen);
	stringbuffer	name;
	name.append("un");
	name.append(label);
	test(name.getString(),output &&
			!ucs2charstring::compare(output,input,inputlen) &&
			outputlen==inputlen);
	delete[] output;
	delete[] escaped;
}

// returns a heap array of exactly "n" elements that holds the first "n"
// ASCII characters of "src"; there is no terminator unless "src" has one
static ucs2_t *rangeMake(const char *src, size_t n) {
	ucs2_t	*buf=new ucs2_t[n];
	for (size_t i=0; i<n; i++) {
		buf[i]=(ucs2_t)(unsigned char)src[i];
	}
	return buf;
}

// true if the first "n" elements of "buf" are the ASCII characters of "exp"
static bool rangeEqual(const ucs2_t *buf, const char *exp, size_t n) {
	for (size_t i=0; i<n; i++) {
		if (buf[i]!=(ucs2_t)(unsigned char)exp[i]) {
			return false;
		}
	}
	return true;
}

static const int32_t rangeMinLen=(int32_t)(-2147483647-1);

static bool rangeIsWhitespace(char c) {
	return (c==' ' || c=='\t' || c=='\n');
}

// makes the "m"th string of length "n" over the characters of "alpha"
static void rangeString(char *str, const char *alpha, size_t k,
						size_t n, size_t m) {
	for (size_t i=0; i<n; i++) {
		str[i]=alpha[m%k];
		m/=k;
	}
	str[n]='\0';
}

static size_t rangeCount(size_t k, size_t n) {
	size_t	total=1;
	for (size_t i=0; i<n; i++) {
		total*=k;
	}
	return total;
}

// checks rightTrim() on every string over "alpha" of 0 to 5 characters, in
// exact-size heap buffers; "ch" is the character to trim, or 0 for whitespace
static void rangeRightTrimTest(const char *label, const char *alpha, char ch) {
	size_t	k=0;
	while (alpha[k]) {
		k++;
	}
	bool	ok=true;
	for (size_t n=0; n<=5; n++) {
		size_t	total=rangeCount(k,n);
		for (size_t m=0; m<total; m++) {
			char	str[8];
			rangeString(str,alpha,k,n,m);
			size_t	e=n;
			while (e && ((ch)?str[e-1]==ch:
					rangeIsWhitespace(str[e-1]))) {
				e--;
			}
			ucs2_t	*buf=rangeMake(str,n+1);
			if (ch) {
				ucs2charstring::rightTrim(buf,(ucs2_t)ch);
			} else {
				ucs2charstring::rightTrim(buf);
			}
			ok=(ok && rangeEqual(buf,str,e) && !buf[e]);
			delete[] buf;
		}
	}
	test(label,ok);
}

// checks that rightTrim() of "n" elements of "fill" empties the string
// without touching the elements in front of it, which are a sentinel and
// then more "fill"
static void rangeRightTrimGuardTest(const char *label, size_t n, char fill,
							bool usech) {
	ucs2_t	buf[1+8+5+1];
	buf[0]=(ucs2_t)'X';
	for (size_t i=1; i<1+8+n; i++) {
		buf[i]=(ucs2_t)fill;
	}
	buf[1+8+n]=(ucs2_t)0;
	if (usech) {
		ucs2charstring::rightTrim(buf+1+8,(ucs2_t)fill);
	} else {
		ucs2charstring::rightTrim(buf+1+8);
	}
	bool	ok=(buf[0]==(ucs2_t)'X' && !buf[1+8]);
	for (size_t i=1; i<1+8; i++) {
		ok=(ok && buf[i]==(ucs2_t)fill);
	}
	test(label,ok);
}

// checks leftJustify(), rightJustify() or center() ("which" 0, 1 or 2) on
// every string over space, a and b of 1 to 5 characters, in exact-size heap
// buffers that have no terminator
static void rangeJustifyTest(const char *label, int which) {
	const char	*alpha=" ab";
	bool		ok=true;
	for (size_t n=1; n<=5; n++) {
		size_t	total=rangeCount(3,n);
		for (size_t m=0; m<total; m++) {
			char	str[8];
			rangeString(str,alpha,3,n,m);
			size_t	lead=0;
			while (lead<n && str[lead]==' ') {
				lead++;
			}
			size_t	trail=0;
			while (trail<n && str[n-1-trail]==' ') {
				trail++;
			}
			char	exp[8];
			for (size_t i=0; i<n; i++) {
				exp[i]=' ';
			}
			exp[n]='\0';
			if (lead<n) {
				size_t	textlen=n-lead-trail;
				size_t	pad=0;
				if (which==0) {
					pad=0;
				} else if (which==1) {
					pad=n-textlen;
				} else {
					pad=(lead+trail)/2;
				}
				for (size_t i=0; i<textlen; i++) {
					exp[pad+i]=str[lead+i];
				}
			}
			ucs2_t	*buf=rangeMake(str,n);
			if (which==0) {
				ucs2charstring::leftJustify(buf,(int32_t)n);
			} else if (which==1) {
				ucs2charstring::rightJustify(buf,(int32_t)n);
			} else {
				ucs2charstring::center(buf,(int32_t)n);
			}
			ok=(ok && rangeEqual(buf,exp,n));
			delete[] buf;
		}
	}
	test(label,ok);
}

// checks that leftJustify(), rightJustify() and center() do nothing and
// touch nothing for a length that is 0 or negative
static void rangeJustifyLengthTest(const char *label, int32_t len) {
	bool	ok=true;
	for (size_t n=0; n<=3; n+=3) {
		ucs2_t	*buf=rangeMake(" a ",n);
		ucs2charstring::leftJustify(buf,len);
		ucs2charstring::rightJustify(buf,len);
		ucs2charstring::center(buf,len);
		ok=(ok && rangeEqual(buf," a ",n));
		delete[] buf;
	}
	test(label,ok);
}

// checks center() on all-space strings and on one character in a long run of
// spaces, in exact-size heap buffers that have no terminator
static void rangeCenterSpacesTest(const char *label) {
	bool	ok=true;
	const size_t	lens[]={1,2,3,7,100,70001};
	for (size_t l=0; l<sizeof(lens)/sizeof(lens[0]); l++) {
		size_t	n=lens[l];
		char	*str=new char[n];
		for (size_t i=0; i<n; i++) {
			str[i]=' ';
		}
		ucs2_t	*buf=rangeMake(str,n);
		ucs2charstring::center(buf,(int32_t)n);
		ok=(ok && rangeEqual(buf,str,n));
		delete[] buf;
		if (n>2) {
			// "x" in the first position moves to the middle
			str[0]='x';
			char	*exp=new char[n];
			for (size_t i=0; i<n; i++) {
				exp[i]=' ';
			}
			exp[(n-1)/2]='x';
			buf=rangeMake(str,n);
			ucs2charstring::center(buf,(int32_t)n);
			ok=(ok && rangeEqual(buf,exp,n));
			delete[] buf;
			delete[] exp;
		}
		delete[] str;
	}
	test(label,ok);
}

// checks isInteger() and isNumber() with a length that is 0 or negative; the
// element in front of the string is not '-' and would give true if it were
// read
static void rangeNumberLengthTest(const char *label, int32_t len) {
	ucs2_t	buf[4];
	buf[0]=(ucs2_t)'x';
	buf[1]=(ucs2_t)'1';
	buf[2]=(ucs2_t)'2';
	buf[3]=(ucs2_t)'3';
	bool	ok=(!ucs2charstring::isInteger(buf+1,len) &&
			!ucs2charstring::isNumber(buf+1,len));
	ucs2_t	*exact=rangeMake("123",3);
	ok=(ok && !ucs2charstring::isInteger(exact,len) &&
			!ucs2charstring::isNumber(exact,len));
	delete[] exact;
	test(label,ok);
}

// checks isInteger() and isNumber() with a length, in an exact-size heap
// buffer that has no terminator
static void rangeNumberTest(const char *str, size_t n, bool isinteger,
							bool isnumber) {
	ucs2_t	*buf=rangeMake(str,n);
	bool	ok=(ucs2charstring::isInteger(buf,(int32_t)n)==isinteger &&
			ucs2charstring::isNumber(buf,(int32_t)n)==isnumber);
	delete[] buf;
	stringbuffer	name;
	name.append("isInteger/isNumber, length form ")->append(str);
	test(name.getString(),ok);
}

// one convertAmount() argument and its dollar string
struct amountcase {
	int64_t		amount;
	const char	*text;
};

static const int64_t numberMin=(-9223372036854775807LL-1);
static const int64_t numberMax=9223372036854775807LL;

static const amountcase amountcases[]={
	{-9,"$-0.09"},{-8,"$-0.08"},{-7,"$-0.07"},{-6,"$-0.06"},
	{-5,"$-0.05"},{-4,"$-0.04"},{-3,"$-0.03"},{-2,"$-0.02"},
	{-1,"$-0.01"},{-10,"$-0.10"},{-99,"$-0.99"},{-100,"$-1.00"},
	{-101,"$-1.01"},{-12345,"$-123.45"},
	{0,"$0.00"},{1,"$0.01"},{2,"$0.02"},{3,"$0.03"},{4,"$0.04"},
	{5,"$0.05"},{6,"$0.06"},{7,"$0.07"},{8,"$0.08"},{9,"$0.09"},
	{10,"$0.10"},{99,"$0.99"},{100,"$1.00"},{101,"$1.01"},
	{12345,"$123.45"},
	{numberMax,"$92233720368547758.07"},
	{numberMin,"$-92233720368547758.08"},
	{numberMin+1,"$-92233720368547758.07"},
	{numberMin+100,"$-92233720368547757.08"},
	{numberMin+101,"$-92233720368547757.07"},
	{numberMax-100,"$92233720368547757.07"}
};

// checks convertAmount() with and without spaces on "c"; the padded
// string is the unpadded one without its dollar sign, right justified
static void amountCaseTest(const amountcase *c) {
	stringbuffer	name;
	ucs2_t	*r=ucs2charstring::convertAmount(c->amount);
	name.append("convertAmount ")->append(c->text);
	test(name.getString(),emptyPatternMatches(r,c->text));
	delete[] r;
	static const uint16_t	spaces[]={0,1,3,8,12,25};
	size_t	digits=charstring::getLength(c->text+1);
	for (size_t i=0; i<sizeof(spaces)/sizeof(spaces[0]); i++) {
		size_t	real=(digits+1>spaces[i])?digits+1:spaces[i];
		char	*expected=new char[real+1];
		bytestring::set(expected,' ',real);
		expected[real]='\0';
		bytestring::copy(expected+real-digits,c->text+1,digits);
		if (expected[0]==' ') {
			expected[0]='$';
		}
		r=ucs2charstring::convertAmount(c->amount,spaces[i]);
		name.clear();
		name.append("convertAmount ")->append(c->text);
		name.append(" spaces ")->append((uint64_t)spaces[i]);
		test(name.getString(),emptyPatternMatches(r,expected));
		delete[] r;
		delete[] expected;
	}
}

// checks parseNumber(number) and parseNumber(number,zeropadding)
static void parseNumberTest(const char *label, int64_t number,
				uint16_t zeropadding, const char *expected) {
	stringbuffer	name;
	ucs2_t	*r=ucs2charstring::parseNumber(number,zeropadding);
	name.append("parseNumber ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),emptyPatternMatches(r,expected));
	delete[] r;
	if (zeropadding==1) {
		r=ucs2charstring::parseNumber(number);
		name.clear();
		name.append("parseNumber ")->append(label);
		test(name.getString(),emptyPatternMatches(r,expected));
		delete[] r;
	}
}

// checks that appending "number" to a string buffer gives "expected"
static void appendNumberTest(const char *label, int64_t number,
				uint16_t zeropadding, const char *expected) {
	stringbuffer	name;
	ucs2stringbuffer	sb;
	if (zeropadding==1) {
		sb.append(number);
	} else {
		sb.append(number,zeropadding);
	}
	ucs2_t	*e=emptyPatternCopy(expected);
	name.append("append ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),!ucs2charstring::compare(sb.getString(),e));
	delete[] e;
}

// true if the length-"len" strings "a" and "b" hold the same elements
static bool dupSame(const ucs2_t *a, const ucs2_t *b, size_t len) {
	if (!a || !b) {
		return (!a && !b);
	}
	for (size_t i=0; i<=len; i++) {
		if (a[i]!=b[i]) {
			return false;
		}
	}
	return true;
}

// checks the wchar_t duplicate() forms that take a replacement against the
// explicit 4-argument form, in both byte orders
static void dupReplacementTest(const char *label, const wchar_t *str,
						ucs2_t replacement) {
	size_t		len=wcharstring::getLength(str);
	stringbuffer	name;
	for (int b=0; b<2; b++) {
		bool	big=(b==1);
		ucs2_t	*expected=ucs2charstring::duplicate(str,len,
							replacement,big);
		ucs2_t	*result=ucs2charstring::duplicate(str,replacement,big);
		name.clear();
		name.append(label)->append(" (str,replacement,bigendian) ");
		name.append((big)?"big":"little");
		test(name.getString(),dupSame(result,expected,len));
		delete[] result;
		result=ucs2charstring::duplicate(str,len,replacement,big);
		name.clear();
		name.append(label);
		name.append(" (str,len,replacement,bigendian) ");
		name.append((big)?"big":"little");
		test(name.getString(),dupSame(result,expected,len));
		delete[] result;
		// the two-argument form must follow "bigendian"
		result=ucs2charstring::duplicate(str,big);
		ucs2_t	*e2=ucs2charstring::duplicate(str,len,
							(ucs2_t)'?',big);
		name.clear();
		name.append(label)->append(" (str,bigendian) ");
		name.append((big)?"big":"little");
		test(name.getString(),dupSame(result,e2,len));
		delete[] result;
		delete[] e2;
		delete[] expected;
	}
	// the forms without a byte order use the byte order of the system
	ucs2_t	*expected=ucs2charstring::duplicate(str,len,replacement,
							sys::isBigEndian());
	ucs2_t	*result=ucs2charstring::duplicate(str,len,replacement);
	name.clear();
	name.append(label)->append(" (str,len,replacement)");
	test(name.getString(),dupSame(result,expected,len));
	delete[] result;
	delete[] expected;
}


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
						s+ucs2charstring::getLength(s));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,LLO));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,BLAH)==
						s+ucs2charstring::getLength(s));
	test("findFirst",ucs2charstring::findFirst(s,(ucs2_t)'l'));
	test("findFirstIgnoringCase",
		ucs2charstring::findFirstIgnoringCase(s,(ucs2_t)'L'));
	test("findFirstOrEnd",
		ucs2charstring::findFirstOrEnd(s,(ucs2_t)'z')==
				s+ucs2charstring::getLength(s));
	test("findFirstOrEnd",ucs2charstring::findFirstOrEnd(s,(ucs2_t)'l'));
	test("findFirstOrEndIgnoringCase",
		ucs2charstring::findFirstOrEndIgnoringCase(s,(ucs2_t)'Z')==
						s+ucs2charstring::getLength(s));
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
	ucs2_t	*elll=ucs2charstring::getSubString(hellothere,1,3);
	ucs2literal(ellr,"ell");
	test("subString",!ucs2charstring::compare(elll,ellr));
	ucs2_t	*llol=ucs2charstring::getSubString(hellothere,2,4);
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

	// isBefore, isBetween, isAfter
	stdoutput.printf("isBefore/isBetween/isAfter...\n");
	ucs2literal(onetwothreefourfive,"one two three four five");
	ucs2literal(sptwo," two");
	ucs2_t	*bef=ucs2charstring::isBefore(onetwothreefourfive,sptwo);
	ucs2literal(one,"one");
	test("isBefore",!ucs2charstring::compare(bef,one));
	ucs2literal(twosp,"two ");
	ucs2literal(spfour," four");
	ucs2_t	*btw=ucs2charstring::isBetween(
				onetwothreefourfive,twosp,spfour);
	ucs2literal(three,"three");
	test("isBetween",!ucs2charstring::compare(btw,three));
	ucs2literal(foursp,"four ");
	ucs2_t	*aft=ucs2charstring::isAfter(onetwothreefourfive,foursp);
	ucs2literal(five,"five");
	test("isAfter",!ucs2charstring::compare(aft,five));
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
		ucs2charstring::getLengthContainingSet(
				alphabet,gfedcba)==14);
	ucs2literal(hijklmnopqrstuvwxyz,"hijklmnopqrstuvwxyz");
	test("lengthNotContainingSet",
		ucs2charstring::getLengthNotContainingSet(
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
				(alphabet+ucs2charstring::getLength(alphabet)));
	ucs2literal(abc,"abc");
	test("findLastOfSetOrEnd",
			ucs2charstring::findLastOfSetOrEnd(alphabet,abc)==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
		ucs2charstring::findLastOfSetOrEnd(alphabet,onetwothree)==
				(alphabet+ucs2charstring::getLength(alphabet)));
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
			ucs2charstring::getIntegerLength((int64_t)0)==1);
	test("integerLength: 1",
			ucs2charstring::getIntegerLength((int64_t)1)==1);
	test("integerLength: 10",
			ucs2charstring::getIntegerLength((int64_t)10)==2);
	test("integerLength: 100",
			ucs2charstring::getIntegerLength((int64_t)100)==3);
	test("integerLength: 1000",
			ucs2charstring::getIntegerLength((int64_t)1000)==4);
	test("integerLength: -1",
			ucs2charstring::getIntegerLength((int64_t)-1)==2);
	test("integerLength: -10",
			ucs2charstring::getIntegerLength((int64_t)-10)==3);
	test("integerLength: -100",
			ucs2charstring::getIntegerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			ucs2charstring::getIntegerLength((int64_t)-1000)==5);
	test("integerLength: 0",
			ucs2charstring::getIntegerLength((uint64_t)0)==1);
	test("integerLength: 1",
			ucs2charstring::getIntegerLength((uint64_t)1)==1);
	test("integerLength: 10",
			ucs2charstring::getIntegerLength((uint64_t)10)==2);
	test("integerLength: 100",
			ucs2charstring::getIntegerLength((uint64_t)100)==3);
	test("integerLength: 1000",
			ucs2charstring::getIntegerLength((uint64_t)1000)==4);

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
	ucs2literal(TRUESTR,"TRUE");
	test("TRUE",ucs2charstring::isYes(TRUESTR));
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
	ucs2literal(FALSESTR,"FALSE");
	test("FALSE",ucs2charstring::isNo(FALSESTR));
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

	// an empty or NULL pattern matches nothing; an alarm turns a hang
	// into a failure
	stdoutput.printf("replace/strip empty pattern...\n");
	signalmanager::alarm(60);
	emptyPatternReplaceTest("empty oldstr","abc","","X","abc","abc");
	emptyPatternReplaceTest("empty oldstr, empty newstr",
					"abc","","","abc","abc");
	emptyPatternReplaceTest("empty oldstr, NULL newstr",
					"abc","",NULL,"abc","abc");
	emptyPatternReplaceTest("NULL oldstr","abc",NULL,"X","abc","abc");
	emptyPatternReplaceTest("NULL oldstr, NULL newstr",
					"abc",NULL,NULL,"abc","abc");
	emptyPatternReplaceTest("one char, empty oldstr","a","","X","a","a");
	emptyPatternReplaceTest("empty str, empty oldstr","","","X","","");
	emptyPatternReplaceTest("empty str, NULL oldstr","",NULL,"X","","");
	emptyPatternReplaceTest("empty str, oldstr","","a","X","","");
	emptyPatternReplaceTest("NULL str, empty oldstr",
					NULL,"","X",NULL,NULL);
	emptyPatternReplaceTest("NULL str, oldstr",NULL,"a","X",NULL,NULL);
	emptyPatternReplaceTest("oldstr","abcabc","b","X","aXcaXc","aXcaXc");
	emptyPatternReplaceTest("oldstr case","aBcAbC","b","X",
					"aBcAXC","aXcAXC");
	emptyPatternReplaceTest("oldstr deleted","abcabc","bc","","aa","aa");
	emptyPatternReplaceTest("oldstr grows","abc","b","XYZ",
					"aXYZc","aXYZc");
	emptyPatternReplaceTest("oldstr is str","abc","abc","X","X","X");
	emptyPatternReplaceTest("oldstr at ends","abcab","ab","X",
					"XcX","XcX");
	emptyPatternReplaceTest("oldstr not found","abc","d","X","abc","abc");
	emptyPatternReplaceTest("oldstr longer than str","ab","abc","X",
					"ab","ab");

	const char	*setnone[]={NULL};
	const char	*setnew1[]={"1",NULL};
	const char	*setnew2[]={"1","2",NULL};
	const char	*setnew3[]={"1","2","3",NULL};
	const char	*setempty[]={"",NULL};
	const char	*set2empty[]={"","",NULL};
	const char	*setfirst[]={"","b",NULL};
	const char	*setmiddle[]={"a","","c",NULL};
	const char	*setlast[]={"b","",NULL};
	const char	*setabc[]={"a","b","c",NULL};
	const char	*setac[]={"a","c",NULL};
	const char	*setupper[]={"A","C",NULL};
	const char	*setmulti[]={"ab","bc",NULL};
	const char	*setbc[]={"a","bc",NULL};
	const char	*setwhole[]={"abc",NULL};
	emptyPatternSetTest("set no members","abc",setnone,NULL,setnone,
					"abc","abc");
	emptyPatternSetTest("set only empty","abc",setempty,NULL,setnew1,
					"abc","abc");
	emptyPatternSetTest("set two empty","abc",set2empty,NULL,setnew2,
					"abc","abc");
	emptyPatternSetTest("set empty first","abc",setfirst,NULL,setnew2,
					"a2c","a2c");
	emptyPatternSetTest("set empty middle","abc",setmiddle,NULL,setnew3,
					"1b3","1b3");
	emptyPatternSetTest("set empty last","abc",setlast,NULL,setnew2,
					"a1c","a1c");
	emptyPatternSetTest("set empty first, empty str","",setfirst,NULL,
					setnew2,"","");
	emptyPatternSetTest("set empty first, one char","b",setfirst,NULL,
					setnew2,"2","2");
	emptyPatternSetTest("set empty middle, repeats","acac",setmiddle,NULL,
					setnew3,"1313","1313");
	emptyPatternSetTest("set","abcabc",setac,NULL,setnew2,
					"1b2" "1b2","1b2" "1b2");
	emptyPatternSetTest("set case","aBc",setupper,NULL,setnew2,
					"aBc","1B2");
	emptyPatternSetTest("set order","abc",setmulti,NULL,setnew2,
					"1c","1c");
	emptyPatternSetTest("set every member","abc",setabc,NULL,setnew3,
					"123","123");
	size_t	len10[]={1,0};
	size_t	len01[]={0,2};
	size_t	len00[]={0,0};
	size_t	len0[]={0};
	size_t	len2[]={2};
	size_t	len111[]={1,1,1};
	emptyPatternSetTest("length 0 last","abc",setbc,len10,setnew2,
					"1bc","1bc");
	emptyPatternSetTest("length 0 first","abc",setbc,len01,setnew2,
					"a2","a2");
	emptyPatternSetTest("lengths all 0","abc",setbc,len00,setnew2,
					"abc","abc");
	emptyPatternSetTest("length 0 whole","abc",setwhole,len0,setnew1,
					"abc","abc");
	emptyPatternSetTest("length 0, empty str","",setwhole,len0,setnew1,
					"","");
	emptyPatternSetTest("length short","abcabc",setwhole,len2,setnew1,
					"1c1c","1c1c");
	emptyPatternSetTest("length","abc",setabc,len111,setnew3,
					"123","123");
	emptyPatternSetTest("length case","aBc",setupper,len10,setnew2,
					"aBc","1Bc");

	// a NULL array is an empty set, so the result is a copy of str
	nullSetTest("NULL array","abcabc","aXcaXc","acac");
	nullSetTest("NULL array, empty str","","","");
	nullSetTest("NULL array, no match","aaa","aaa","aaa");

	emptyPatternStripTest("strip empty str2","abc","","abc",false);
	emptyPatternStripTest("strip NULL str2","abc",NULL,"abc",false);
	emptyPatternStripTest("strip empty str1, empty str2","","","",false);
	emptyPatternStripTest("strip empty str1","","a","",false);
	emptyPatternStripTest("strip NULL str1, empty str2",
						NULL,"",NULL,false);
	emptyPatternStripTest("strip NULL str1",NULL,"a",NULL,false);
	emptyPatternStripTest("strip str2","abcabc","bc","aa",true);
	emptyPatternStripTest("strip all","abcabc","abc","",true);
	emptyPatternStripTest("strip repeats","xxxx","xx","",true);
	emptyPatternStripTest("strip not found","abc","x","abc",false);
	emptyPatternStripTest("strip str2 longer","abc","abcd","abc",false);
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	// leftTrim, strip and stripSet use size_t indexes
	stdoutput.printf("leftTrim/strip/stripSet indexes...\n");
	sizeIndexTest("leftTrim",0,"  \t ab c ","ab c ",false);
	sizeIndexTest("leftTrim, all space",0,"   ","",false);
	sizeIndexTest("leftTrim, none",0,"ab","ab",false);
	sizeIndexTest("leftTrim ch",1,"xxabx","abx",false);
	sizeIndexTest("leftTrim ch, all",1,"xxx","",false);
	sizeIndexTest("leftTrim ch, none",1,"ab","ab",false);
	sizeIndexTest("strip ch",2,"xaxbxx","ab",true);
	sizeIndexTest("strip ch, all",2,"xxx","",true);
	sizeIndexTest("strip ch, none",2,"ab","ab",false);
	sizeIndexTest("strip str2",3,"xyaxybxyxy","ab",true);
	sizeIndexTest("strip str2, all",3,"xyxy","",true);
	sizeIndexTest("strip str2, partial",3,"xaxyb","xab",true);
	sizeIndexTest("strip str2, none",3,"xayb","xayb",false);
	sizeIndexTest("stripSet",4,"xaybxyc","abc",true);
	sizeIndexTest("stripSet, all",4,"xyyx","",true);
	sizeIndexTest("stripSet, none",4,"abc","abc",false);
	stdoutput.printf("\n");

	// findLast and findLastIgnoringCase test every position
	stdoutput.printf("findLast...\n");
	signalmanager::alarm(60);
	findLastTest("abc / abc",
			"abc","abc",0,0);
	findLastTest("abc / a",
			"abc","a",0,0);
	findLastTest("ABC / abc",
			"ABC","abc",-1,0);
	findLastTest("abc / ABC",
			"abc","ABC",-1,0);
	findLastTest("abc / ",
			"abc","",3,3);
	findLastTest(" / ",
			"","",0,0);
	findLastTest(" / a",
			"","a",-1,-1);
	findLastTest("abcabc / abc",
			"abcabc","abc",3,3);
	findLastTest("abcabc / ABC",
			"abcabc","ABC",-1,3);
	findLastTest("abcabc / bc",
			"abcabc","bc",4,4);
	findLastTest("abca / a",
			"abca","a",3,3);
	findLastTest("xabc / abc",
			"xabc","abc",1,1);
	findLastTest("abc / abcd",
			"abc","abcd",-1,-1);
	findLastTest("abc / x",
			"abc","x",-1,-1);
	findLastTest("abcabc / b",
			"abcabc","b",4,4);
	findLastTest("abcabc / B",
			"abcabc","B",-1,4);
	findLastTest("abcabc / z",
			"abcabc","z",-1,-1);
	findLastTest("aAaA / A",
			"aAaA","A",3,3);
	findLastTest("aAaA / a",
			"aAaA","a",2,3);
	findLastTest("ab / b",
			"ab","b",1,1);
	findLastTest("a / a",
			"a","a",0,0);
	findLastTest("a / A",
			"a","A",-1,0);
	findLastTest("AbcaBC / bc",
			"AbcaBC","bc",1,4);
	findLastTest("abAB / ab",
			"abAB","ab",0,2);
	findLastNullTest();
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	// out-of-range reads; an alarm turns a hang into a failure
	stdoutput.printf("trim/justify/isInteger out of range...\n");
	signalmanager::alarm(60);
	rangeRightTrimTest("rightTrim, every string"," \ta",0);
	rangeRightTrimTest("rightTrim ch, every string","xya",'x');
	rangeRightTrimTest("rightTrim ch, every string with spaces","x y",'x');
	rangeRightTrimGuardTest("rightTrim, 1 space",1,' ',false);
	rangeRightTrimGuardTest("rightTrim, 2 spaces",2,' ',false);
	rangeRightTrimGuardTest("rightTrim, 3 spaces",3,' ',false);
	rangeRightTrimGuardTest("rightTrim, 4 spaces",4,' ',false);
	rangeRightTrimGuardTest("rightTrim, 5 spaces",5,' ',false);
	rangeRightTrimGuardTest("rightTrim, 1 tab",1,'\t',false);
	rangeRightTrimGuardTest("rightTrim, 3 tabs",3,'\t',false);
	rangeRightTrimGuardTest("rightTrim ch, 1",1,'x',true);
	rangeRightTrimGuardTest("rightTrim ch, 2",2,'x',true);
	rangeRightTrimGuardTest("rightTrim ch, 3",3,'x',true);
	rangeRightTrimGuardTest("rightTrim ch, 4",4,'x',true);
	rangeRightTrimGuardTest("rightTrim ch, 5",5,'x',true);
	{
	ucs2charstring::rightTrim((ucs2_t *)NULL);
	ucs2charstring::rightTrim((ucs2_t *)NULL,(ucs2_t)'x');
	ucs2_t	*empty=rangeMake("",1);
	ucs2charstring::rightTrim(empty);
	ucs2charstring::rightTrim(empty,(ucs2_t)'x');
	test("rightTrim NULL and empty",!empty[0]);
	delete[] empty;
	}
	rangeJustifyTest("leftJustify, every string",0);
	rangeJustifyTest("rightJustify, every string",1);
	rangeJustifyTest("center, every string",2);
	rangeJustifyLengthTest("justify, length 0",0);
	rangeJustifyLengthTest("justify, length -1",-1);
	rangeJustifyLengthTest("justify, length -5",-5);
	rangeJustifyLengthTest("justify, length INT32_MIN",rangeMinLen);
	rangeCenterSpacesTest("center, all spaces and one character");
	rangeNumberLengthTest("isInteger/isNumber, length 0",0);
	rangeNumberLengthTest("isInteger/isNumber, length -1",-1);
	rangeNumberLengthTest("isInteger/isNumber, length -5",-5);
	rangeNumberLengthTest("isInteger/isNumber, length INT32_MIN",
					rangeMinLen);
	{
	test("isInteger/isNumber NULL, length -1",
			!ucs2charstring::isInteger((const ucs2_t *)NULL,-1) &&
			!ucs2charstring::isNumber((const ucs2_t *)NULL,-1));
	}
	rangeNumberTest("1",1,true,true);
	rangeNumberTest("123",3,true,true);
	rangeNumberTest("-12",3,true,true);
	rangeNumberTest("12-",3,false,false);
	rangeNumberTest("-",1,false,false);
	rangeNumberTest("1.5",3,false,true);
	rangeNumberTest(".",1,false,false);
	rangeNumberTest("1.",2,false,false);
	rangeNumberTest("1.5.2",5,false,false);
	rangeNumberTest("12x",3,false,false);
	signalmanager::alarm(0);
	stdoutput.printf("\n");

	// amounts, integer lengths and number strings at the extremes of
	// int64_t; an alarm turns a hang into a failure
	stdoutput.printf("amounts and numbers at the extremes...\n");
	signalmanager::alarm(60);
	for (size_t i=0; i<sizeof(amountcases)/sizeof(amountcases[0]); i++) {
		amountCaseTest(&amountcases[i]);
	}
	{
	ucs2_t	*r=ucs2charstring::convertAmount((int64_t)-1,8);
	test("convertAmount -1, 8 spaces",emptyPatternMatches(r,"$  -0.01"));
	delete[] r;
	r=ucs2charstring::convertAmount(numberMin,8);
	test("convertAmount minimum, 8 spaces",
		emptyPatternMatches(r,"$-92233720368547758.08"));
	delete[] r;
	}
	test("getIntegerLength INT16_MIN",
		ucs2charstring::getIntegerLength((int16_t)(-32767-1))==6);
	test("getIntegerLength INT16_MIN+1",
		ucs2charstring::getIntegerLength((int16_t)-32767)==6);
	test("getIntegerLength INT16_MAX",
		ucs2charstring::getIntegerLength((int16_t)32767)==5);
	test("getIntegerLength int16 -1",
		ucs2charstring::getIntegerLength((int16_t)-1)==2);
	test("getIntegerLength int16 0",
		ucs2charstring::getIntegerLength((int16_t)0)==1);
	test("getIntegerLength INT32_MIN",
		ucs2charstring::getIntegerLength(
				(int32_t)(-2147483647-1))==11);
	test("getIntegerLength INT32_MIN+1",
		ucs2charstring::getIntegerLength((int32_t)-2147483647)==11);
	test("getIntegerLength INT32_MAX",
		ucs2charstring::getIntegerLength((int32_t)2147483647)==10);
	test("getIntegerLength int32 -1",
		ucs2charstring::getIntegerLength((int32_t)-1)==2);
	test("getIntegerLength int32 0",
		ucs2charstring::getIntegerLength((int32_t)0)==1);
	test("getIntegerLength INT64_MIN",
		ucs2charstring::getIntegerLength(numberMin)==20);
	test("getIntegerLength INT64_MIN+1",
		ucs2charstring::getIntegerLength(numberMin+1)==20);
	test("getIntegerLength INT64_MAX",
		ucs2charstring::getIntegerLength(numberMax)==19);
	parseNumberTest("INT64_MIN",numberMin,1,"-9223372036854775808");
	parseNumberTest("INT64_MIN",numberMin,0,"-9223372036854775808");
	parseNumberTest("INT64_MIN",numberMin,20,"-9223372036854775808");
	parseNumberTest("INT64_MIN",numberMin,21,"-09223372036854775808");
	parseNumberTest("INT64_MIN",numberMin,25,"-000009223372036854775808");
	parseNumberTest("INT64_MIN+1",numberMin+1,1,"-9223372036854775807");
	parseNumberTest("INT64_MIN+1",numberMin+1,22,
					"-009223372036854775807");
	parseNumberTest("INT64_MAX",numberMax,1,"9223372036854775807");
	parseNumberTest("INT64_MAX",numberMax,25,"0000009223372036854775807");
	parseNumberTest("-5",-5,1,"-5");
	parseNumberTest("-5",-5,3,"-05");
	parseNumberTest("-1",-1,0,"-1");
	parseNumberTest("0",0,3,"000");
	parseNumberTest("INT32_MIN",(int32_t)(-2147483647-1),1,"-2147483648");
	parseNumberTest("INT16_MIN",(int16_t)(-32767-1),8,"-0032768");
	{
	ucs2_t	*r=ucs2charstring::parseNumber((int32_t)(-2147483647-1));
	test("parseNumber int32 INT32_MIN",
		emptyPatternMatches(r,"-2147483648"));
	delete[] r;
	r=ucs2charstring::parseNumber((int16_t)(-32767-1));
	test("parseNumber int16 INT16_MIN",
		emptyPatternMatches(r,"-32768"));
	delete[] r;
	}
	appendNumberTest("INT64_MIN",numberMin,1,"-9223372036854775808");
	appendNumberTest("INT64_MIN",numberMin,25,"-000009223372036854775808");
	appendNumberTest("INT64_MIN+1",numberMin+1,1,"-9223372036854775807");
	appendNumberTest("-5",-5,3,"-05");
	signalmanager::alarm(0);
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
	ucs2_t	*sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1B,"2.1B");
	test("humanReadable 2.1B",!ucs2charstring::compare(sval,num2dot1B));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1Y,"2.1Y");
	test("humanReadable 2.1Y",!ucs2charstring::compare(sval,num2dot1Y));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1Z,"2.1Z");
	test("humanReadable 2.1Z",!ucs2charstring::compare(sval,num2dot1Z));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1P,"2.1P");
	test("humanReadable 2.1P",!ucs2charstring::compare(sval,num2dot1P));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1T,"2.1T");
	test("humanReadable 2.1T",!ucs2charstring::compare(sval,num2dot1T));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1G,"2.1G");
	test("humanReadable 2.1G",!ucs2charstring::compare(sval,num2dot1G));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1M,"2.1M");
	test("humanReadable 2.1M",!ucs2charstring::compare(sval,num2dot1M));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1K,"2.1K");
	test("humanReadable 2.1K",!ucs2charstring::compare(sval,num2dot1K));
	delete[] sval;
	val/=1024.0;
	sval=ucs2charstring::getHumanReadable(val);
	ucs2literal(num2dot1,"2.1");
	test("humanReadable 2.1",!ucs2charstring::compare(sval,num2dot1));
	delete[] sval;

	val=2.1*1000.0*1000.0*1000.0*1000.0*
		1000.0*1000.0*1000.0*1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1B",!ucs2charstring::compare(sval,num2dot1B));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1Y",!ucs2charstring::compare(sval,num2dot1Y));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1Z",!ucs2charstring::compare(sval,num2dot1Z));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1P",!ucs2charstring::compare(sval,num2dot1P));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1T",!ucs2charstring::compare(sval,num2dot1T));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1G",!ucs2charstring::compare(sval,num2dot1G));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1M",!ucs2charstring::compare(sval,num2dot1M));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1K",!ucs2charstring::compare(sval,num2dot1K));
	delete[] sval;
	val/=1000.0;
	sval=ucs2charstring::getHumanReadable(val,true);
	test("humanReadable 2.1",!ucs2charstring::compare(sval,num2dot1));
	delete[] sval;

#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	ival=((uint64_t)3)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024);
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3Z,"3Z");
	test("humanReadable 3Z",!ucs2charstring::compare(sval,num3Z));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3P,"3P");
	test("humanReadable 3P",!ucs2charstring::compare(sval,num3P));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3T,"3T");
	test("humanReadable 3T",!ucs2charstring::compare(sval,num3T));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3G,"3G");
	test("humanReadable 3G",!ucs2charstring::compare(sval,num3G));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3M,"3M");
	test("humanReadable 3M",!ucs2charstring::compare(sval,num3M));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	ucs2literal(num3K,"3K");
	test("humanReadable 3K",!ucs2charstring::compare(sval,num3K));
	delete[] sval;
	ival/=1024;
	sval=ucs2charstring::getHumanReadable(ival);
	test("humanReadable 3",!ucs2charstring::compare(sval,num3));
	delete[] sval;

	int64_t	sival=((int64_t)-3)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000);
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3Z,"-3Z");
	test("humanReadable -3Z",!ucs2charstring::compare(sval,neg3Z));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3P,"-3P");
	test("humanReadable -3P",!ucs2charstring::compare(sval,neg3P));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3T,"-3T");
	test("humanReadable -3T",!ucs2charstring::compare(sval,neg3T));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3G,"-3G");
	test("humanReadable -3G",!ucs2charstring::compare(sval,neg3G));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3M,"-3M");
	test("humanReadable -3M",!ucs2charstring::compare(sval,neg3M));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3K,"-3K");
	test("humanReadable -3K",!ucs2charstring::compare(sval,neg3K));
	delete[] sval;
	sival/=1000;
	sval=ucs2charstring::getHumanReadable(sival,true);
	ucs2literal(neg3,"-3");
	test("humanReadable -3",!ucs2charstring::compare(sval,neg3));
	delete[] sval;
#endif
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


	// escaping
	stdoutput.printf("quoted strings...\n");
	const char * const goodstr[]={
		// properly terminated
		"'test'test",
		"'''test'''test",
		"'\\'test\\''test",
		"'\\'''test''\\''test",
		"'\\\\''test''\\\\'test",
		"\"test\"test",
		"\"\"\"test\"\"\"test",
		"\"\\\"test\\\"\"test",
		"\"\\\"\"\"test\"\"\\\"\"test",
		"\"\\\\\"\"test\"\"\\\\\"test",
		"`test`test",
		"```test```test",
		"`\\`test\\``test",
		"`\\```test``\\``test",
		"`\\\\``test``\\\\`test",
		"[test]test",
		NULL
	};
	for (const char * const *s=goodstr; *s; s++) {
		ucs2_t	*u2s=ucs2charstring::duplicate(*s);
		test(*s,ucs2charstring::findEndOfQuotedString(u2s,
				ucs2charstring::getLength(u2s),true,true)==
				u2s+ucs2charstring::getLength(u2s)-4);
		delete[] u2s;
	}
	const char * const badstr[]={
		// unterminated
		"'testtest",
		"'''test''test",
		"'\\'test\\'test",
		"'\\'''test''\\'test",
		"'\\\\''test''\\test",
		"\"testtest",
		"\"\"\"test\"\"test",
		"\"\\\"test\\\"test",
		"\"\\\"\"\"test\"\"\\\"test",
		"\"\\\\\"\"test\"\"\\test",
		"`testtest",
		"```test``test",
		"`\\`test\\`test",
		"`\\```test``\\`test",
		"`\\\\``test``\\test",
		"[testtest",
		NULL
	};
	for (const char * const *s=badstr; *s; s++) {
		ucs2_t	*u2s=ucs2charstring::duplicate(*s);
		test(*s,ucs2charstring::findEndOfQuotedString(u2s,
				ucs2charstring::getLength(u2s),true,true)==
				u2s+ucs2charstring::getLength(u2s));
		delete[] u2s;
	}
	stdoutput.printf("\n");


	// escape/unescape, pad, insertString, and the length forms of duplicate
	stdoutput.printf("escape/pad/insertString/duplicate length...\n");
	ucs2literal(quote,"\"");
	ucs2literal(emptystr,"");
	ucs2literal(lena,"a");
	ucs2literal(lenaqb,"a\"b");
	ucs2literal(lenaqbesc,"a\\\"b");
	ucs2literal(lenabsb,"a\\b");
	ucs2literal(lenabsbesc,"a\\\\b");
	escapeLengthTest("escape length 0",emptystr,0,quote,emptystr);
	escapeLengthTest("escape length 1",lena,1,quote,lena);
	escapeLengthTest("escape length 3",lenaqb,3,quote,lenaqbesc);
	escapeLengthTest("escape backslash",lenabsb,3,quote,lenabsbesc);
	ucs2literal(abcstr,"abc");
	ucs2literal(abcdef,"abcdef");
	ucs2literal(abcd,"abcd");
	ucs2_t	star=(ucs2_t)'*';
	padTest("pad exact length",abcstr,star,-1,3,abcstr);
	padTest("pad truncate",abcdef,star,-1,3,abcstr);
	padTest("pad truncate right",abcdef,star,1,4,abcd);
	padTest("pad zero length",abcstr,star,1,0,NULL);
	padTest("pad NULL zero length",NULL,star,1,0,NULL);
	ucs2literal(hellostr,"hello");
	ucs2literal(xy,"XY");
	ucs2literal(dash,"-");
	ucs2literal(xyhello,"XYhello");
	ucs2literal(hexyllo,"heXYllo");
	ucs2literal(helloxy,"helloXY");
	ucs2literal(helldashlo,"hel-lo");
	insertStringTest("insertString start",hellostr,xy,0,xyhello);
	insertStringTest("insertString middle",hellostr,xy,2,hexyllo);
	insertStringTest("insertString end",hellostr,xy,5,helloxy);
	insertStringTest("insertString empty dest",emptystr,xy,0,xy);
	insertStringTest("insertString one",hellostr,dash,3,helldashlo);

	// empty and NULL arguments and out-of-range indices; an alarm turns
	// a hang into a failure
	signalmanager::alarm(60);
	const char	*isdest="abc";
	const char	*isexpected[]={
		"XYabc","aXYbc","abXYc","abcXY","abcXY","abcXY","abcXY"
	};
	for (uint64_t i=0; i<=6; i++) {
		stringbuffer	name;
		name.append("insertString empty src, index ");
		name.append(i);
		insertStringExactTest(name.getString(),isdest,"",i,isdest);
		name.clear();
		name.append("insertString src, index ");
		name.append(i);
		insertStringExactTest(name.getString(),isdest,"XY",i,
							isexpected[i]);
	}
	insertStringExactTest("insertString empty both","","",0,"");
	insertStringExactTest("insertString empty both, 3","","",3,"");
	insertStringExactTest("insertString NULL src",isdest,NULL,1,isdest);
	insertStringExactTest("insertString NULL src, 0",isdest,NULL,0,isdest);
	insertStringExactTest("insertString NULL src, past",
						isdest,NULL,10,isdest);
	insertStringExactTest("insertString NULL dest",NULL,"XY",0,"XY");
	insertStringExactTest("insertString NULL dest, 5",NULL,"XY",5,"XY");
	insertStringExactTest("insertString NULL dest, empty src",
							NULL,"",0,"");
	insertStringExactTest("insertString both NULL",NULL,NULL,0,"");
	insertStringExactTest("insertString both NULL, 7",NULL,NULL,7,"");
	insertStringExactTest("insertString len+1",isdest,"XY",4,"abcXY");
	insertStringExactTest("insertString len+100",isdest,"XY",103,"abcXY");
	insertStringExactTest("insertString UINT64_MAX",
				isdest,"XY",(uint64_t)-1,"abcXY");
	insertStringExactTest("insertString empty src, len+1",
						isdest,"",4,isdest);
	insertStringExactTest("insertString empty src, UINT64_MAX",
				isdest,"",(uint64_t)-1,isdest);
	signalmanager::alarm(0);
	const char	*dupstr="hello world";
	const wchar_t	*dupwstr=L"hello world";
	ucs2_t		dupustr[12];
	for (uint16_t i=0; i<12; i++) {
		dupustr[i]=(ucs2_t)dupstr[i];
	}
	size_t	duplens[]={0,1,2,5,11};
	for (size_t i=0; i<sizeof(duplens)/sizeof(duplens[0]); i++) {
		stringbuffer	label;
		label.append("duplicate length ");
		label.append((uint64_t)duplens[i]);
		duplicateLengthTest(label.getString(),
					dupstr,dupwstr,dupustr,duplens[i]);
	}
	stdoutput.printf("\n");


	// the methods below size their buffer in UCS-2 characters, so the first
	// length whose (length,1,1) size does not fit in a size_t is the
	// largest size_t divided by the size of a UCS-2 character
	//
	// these calls give a zero-element input buffer and a length too large
	// for the result to be sized (AddressSanitizer reports any read), and
	// rely on the size check coming before any read or write
	stdoutput.printf("escape/pad/duplicate oversized length...\n");
	size_t		none=0;
	ucs2_t		*zin=(ucs2_t *)new char[none];
	wchar_t		*zwin=(wchar_t *)new char[none];
	char		*zcin=new char[none];
	size_t		maxsize=(size_t)-1;
	size_t		firstfail=maxsize/sizeof(ucs2_t);
	uint64_t	maxlen=(uint64_t)-1;
	uint64_t	lens[]={firstfail,maxlen};
	const char	*lenlabels[]={"first failing","-1"};
	for (uint16_t i=0; i<2; i++) {
		stringbuffer	name;
		ucs2_t		*oversized;

		// the out-pointers are preset to non-NULL
		ucs2_t		*output=zin;
		uint64_t	outputlen=1;
		ucs2charstring::escape(zin,lens[i],&output,&outputlen,quote);
		name.append("escape ");
		name.append(lenlabels[i]);
		test(name.getString(),!output && !outputlen);
		delete[] output;
		output=zin;
		outputlen=1;
		ucs2charstring::unescape(zin,lens[i],&output,&outputlen);
		name.clear();
		name.append("unescape ");
		name.append(lenlabels[i]);
		test(name.getString(),!output && !outputlen);
		delete[] output;

		oversized=ucs2charstring::pad(zin,star,-1,lens[i]);
		name.clear();
		name.append("pad left ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::pad(zin,star,0,lens[i]);
		name.clear();
		name.append("pad center ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::pad(zin,star,1,lens[i]);
		name.clear();
		name.append("pad right ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;

		oversized=ucs2charstring::duplicate(zin,(size_t)lens[i]);
		name.clear();
		name.append("duplicate ucs2 ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::duplicate(zcin,(size_t)lens[i]);
		name.clear();
		name.append("duplicate char ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::duplicate(zcin,(size_t)lens[i],true);
		name.clear();
		name.append("duplicate char bigendian ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::duplicate(zwin,(size_t)lens[i]);
		name.clear();
		name.append("duplicate wchar_t ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::duplicate(zwin,(size_t)lens[i],true);
		name.clear();
		name.append("duplicate wchar_t bigendian ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=ucs2charstring::duplicate(zwin,
					(size_t)lens[i],(ucs2_t)'#',false);
		name.clear();
		name.append("duplicate wchar_t replacement bigendian ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
	}

	// getSubString() asks duplicate() for a length of (size_t)-1
	// when "start" is 1 past the end of an empty string
	ucs2_t	*substr=ucs2charstring::getSubString(emptystr,1);
	test("getSubString past end",!substr);
	delete[] substr;

	// a NULL input is handled before the length is checked
	const ucs2_t	*nullustr=NULL;
	const char	*nullstr=NULL;
	const wchar_t	*nullwstr=NULL;
	ucs2_t		*nulloutput=zin;
	uint64_t	nulloutputlen=1;
	ucs2charstring::escape(nullustr,maxlen,
				&nulloutput,&nulloutputlen,quote);
	test("escape NULL, -1",!nulloutput && !nulloutputlen);
	nulloutput=zin;
	nulloutputlen=1;
	ucs2charstring::unescape(nullustr,maxlen,&nulloutput,&nulloutputlen);
	test("unescape NULL, -1",!nulloutput && !nulloutputlen);
	test("pad NULL, -1",!ucs2charstring::pad(nullustr,star,1,maxlen));
	test("duplicate NULL, -1 ucs2",
			!ucs2charstring::duplicate(nullustr,maxsize));
	test("duplicate NULL, -1 char",
			!ucs2charstring::duplicate(nullstr,maxsize));
	test("duplicate NULL, -1 wchar_t",
			!ucs2charstring::duplicate(nullwstr,maxsize));
	delete[] (char *)zin;
	delete[] (char *)zwin;
	delete[] zcin;

	// split sizes its list from the number of parts of a real string and
	// insertString sizes its result from the lengths of two real strings,
	// so a size that does not fit in a size_t needs a string larger than
	// the address space and cannot be tested on a 64-bit platform
	stdoutput.printf("\n");


	// pad and convertAmount count characters, not bytes
	stdoutput.printf("pad/convertAmount characters...\n");
	ucs2_t		*padabc=ucs2charstring::duplicate("abc");
	ucs2_t		*pada=ucs2charstring::duplicate("a");
	ucs2_t		*padabcd=ucs2charstring::duplicate("abcd");
	ucs2_t		*padempty=ucs2charstring::duplicate("");
	ucs2_t		*expectedpad=ucs2charstring::duplicate("*****abc");
	ucs2_t		starchar=(ucs2_t)'*';
	padTest("pad left",padabc,starchar,-1,8,expectedpad);
	delete[] expectedpad;
	expectedpad=ucs2charstring::duplicate("abc*****");
	padTest("pad right",padabc,starchar,1,8,expectedpad);
	delete[] expectedpad;
	expectedpad=ucs2charstring::duplicate("**abc***");
	padTest("pad centre",padabc,starchar,0,8,expectedpad);
	delete[] expectedpad;
	expectedpad=ucs2charstring::duplicate("****");
	padTest("pad empty",padempty,starchar,-1,4,expectedpad);
	delete[] expectedpad;
	expectedpad=ucs2charstring::duplicate("***");
	padTest("pad NULL",NULL,starchar,1,3,expectedpad);
	delete[] expectedpad;
	ucs2_t		padchars[]={(ucs2_t)' ',0x00E9,0x263A,0x263B};
	ucs2_t		special[]={0x00E9,(ucs2_t)'x',0x263A,0};
	const ucs2_t	*padstrs[]={NULL,padempty,pada,padabc,padabcd,
							special};
	uint64_t	padlens[]={1,2,3,4,5,8,17,64};
	const char	*dirnames[]={"left","centre","right"};
	for (size_t p=0; p<sizeof(padchars)/sizeof(padchars[0]); p++) {
		for (int16_t d=-1; d<=1; d++) {
			bool	matches=true;
			for (size_t i=0; i<sizeof(padstrs)/
						sizeof(padstrs[0]); i++) {
				for (size_t j=0; j<sizeof(padlens)/
						sizeof(padlens[0]); j++) {
					if (!padMatches(padstrs[i],padchars[p],
							d,padlens[j])) {
						matches=false;
					}
				}
			}
			stringbuffer	name;
			name.append("pad ");
			name.append(dirnames[d+1]);
			name.append(" with character ");
			name.append((uint64_t)padchars[p]);
			test(name.getString(),matches);
		}
	}
	delete[] padabc;
	delete[] pada;
	delete[] padabcd;
	delete[] padempty;
	int64_t		amounts[]={0,5,99,100,101,12345,99999,1234567,
					987654321,-100,-12345,-1234567};
	for (size_t i=0; i<sizeof(amounts)/sizeof(amounts[0]); i++) {
		stringbuffer	name;
		name.append("convertAmount ");
		name.append(amounts[i]);
		test(name.getString(),amountMatches(amounts[i]));
	}
	ucs2_t	*amount=ucs2charstring::convertAmount(12345,8);
	ucs2_t	*expectedamount=ucs2charstring::duplicate("$ 123.45");
	test("convertAmount padding",amount &&
		!ucs2charstring::compare(amount,expectedamount));
	delete[] amount;
	delete[] expectedamount;
	stdoutput.printf("\n");


	// convertToFloatC converts a string with a "." decimal point when the
	// locale uses another one
	stdoutput.printf("convertToFloatC...\n");
	ucs2_t	*comma=ucs2charstring::duplicate("1,5");
	if (locale::setNumeric("de_DE.utf8") &&
		ucs2charstring::convertToFloat(comma)==1.5) {
		test("convertToFloatC 1.5",floatC("1.5")==1.5);
		test("convertToFloatC -0.5",floatC("-0.5")==-0.5);
		test("convertToFloatC 123.25",floatC("123.25")==123.25);
		test("convertToFloatC no decimal point",floatC("42")==42.0);

		// the longest string that is converted in place
		char	longstr[512];
		for (size_t i=0; i<252; i++) {
			longstr[i]='0';
		}
		longstr[252]='1';
		longstr[253]='.';
		longstr[254]='5';
		longstr[255]='\0';
		test("convertToFloatC 255 characters",
			floatC(longstr)==1.5);

		// a string too long for the buffer must not write past it
		longstr[0]='1';
		for (size_t i=1; i<290; i++) {
			longstr[i]='0';
		}
		longstr[290]='\0';
		ucs2_t		*ulongstr=ucs2charstring::duplicate(longstr);
		long double	expected=
				ucs2charstring::convertToFloat(ulongstr);
		delete[] ulongstr;
		longstr[290]='.';
		for (size_t i=291; i<300; i++) {
			longstr[i]='5';
		}
		longstr[300]='\0';
		test("convertToFloatC 300 characters",
			floatC(longstr)==expected);
	} else {
		stdoutput.printf("de_DE.utf8 locale not available, "
					"skipped\n");
	}
	locale::setNumeric("C");
	delete[] comma;
	stdoutput.printf("\n");


	// wchar_t duplicate: the forms with a replacement, and the byte order
	// of the form with only a byte order; an alarm turns a hang into a
	// failure
	stdoutput.printf("duplicate wchar_t byte order...\n");
	signalmanager::alarm(60);
	{
		wchar_t	*dw=new wchar_t[4];
		dw[0]=L'A';
		dw[1]=L'b';
		dw[2]=L'C';
		dw[3]=0;
		dupReplacementTest("duplicate ABC",dw,(ucs2_t)'#');
		ucs2_t	*lil=ucs2charstring::duplicate(dw,false);
		ucs2_t	*big=ucs2charstring::duplicate(dw,true);
		test("duplicate L\"A\" little endian 0x0041",
				lil[0]==(ucs2_t)(sys::isBigEndian()?
							0x4100:0x0041));
		test("duplicate L\"A\" big endian 0x4100",
				big[0]==(ucs2_t)(sys::isBigEndian()?
							0x0041:0x4100));
		test("duplicate bigendian differs from little endian",
				lil[0]!=big[0] && lil[1]!=big[1]);
		delete[] lil;
		delete[] big;
		delete[] dw;

		// a character above 0xFFFF becomes the replacement
		if (sizeof(wchar_t)>2) {
			wchar_t	*hw=new wchar_t[4];
			hw[0]=L'a';
			hw[1]=(wchar_t)0x10000;
			hw[2]=L'z';
			hw[3]=0;
			ucs2_t	rep=(ucs2_t)'#';
			dupReplacementTest("duplicate above 0xFFFF",hw,rep);
			for (int b=0; b<2; b++) {
				ucs2_t	*r=ucs2charstring::duplicate(hw,rep,
								b==1);
				test("duplicate above 0xFFFF replaced",
							r[1]==rep && r[3]==0);
				delete[] r;
				r=ucs2charstring::duplicate(hw,3,rep,b==1);
				test("duplicate above 0xFFFF len replaced",
							r[1]==rep && r[3]==0);
				delete[] r;
			}
			ucs2_t	*r=ucs2charstring::duplicate(hw,3,rep);
			test("duplicate above 0xFFFF (str,len,rep)",
							r[1]==rep && r[3]==0);
			delete[] r;
			delete[] hw;
		}

		// empty string, and a length shorter than the string
		wchar_t	*ew=new wchar_t[1];
		ew[0]=0;
		dupReplacementTest("duplicate empty",ew,(ucs2_t)'#');
		delete[] ew;
		wchar_t	*sw=new wchar_t[3];
		sw[0]=L'x';
		sw[1]=L'y';
		sw[2]=0;
		for (int b=0; b<2; b++) {
			ucs2_t	*r=ucs2charstring::duplicate(sw,1,
							(ucs2_t)'#',b==1);
			test("duplicate length 1 of 2",
				r[0]==(ucs2_t)((b==1)==sys::isBigEndian()?
								0x0078:0x7800)
				&& r[1]==0);
			delete[] r;
		}
		ucs2_t	*r=ucs2charstring::duplicate(sw,1,(ucs2_t)'#');
		test("duplicate (str,1,rep) length",r[1]==0);
		delete[] r;
		delete[] sw;

		// NULL input
		const wchar_t	*nw=NULL;
		test("duplicate NULL (str,rep,bigendian)",
			!ucs2charstring::duplicate(nw,(ucs2_t)'#',true));
		test("duplicate NULL (str,rep,bigendian) little",
			!ucs2charstring::duplicate(nw,(ucs2_t)'#',false));
		test("duplicate NULL (str,len,rep)",
			!ucs2charstring::duplicate(nw,(size_t)3,(ucs2_t)'#'));
		test("duplicate NULL (str,len,rep) len 0",
			!ucs2charstring::duplicate(nw,(size_t)0,(ucs2_t)'#'));
		test("duplicate NULL (str,bigendian)",
			!ucs2charstring::duplicate(nw,true));
		test("duplicate NULL (str,bigendian) little",
			!ucs2charstring::duplicate(nw,false));
	}
	signalmanager::alarm(0);
	stdoutput.printf("\n");


	return 0;
}
