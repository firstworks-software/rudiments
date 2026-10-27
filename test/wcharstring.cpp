// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/wcharstring.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/wstringbuffer.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/sys.h>
#include <rudiments/locale.h>
#include <rudiments/stdio.h>
#include <limits.h>
#include <rudiments/signalclasses.h>
#include "test.cpp"

// returns an exact-size copy of the ASCII string "str"; NULL stays NULL
static wchar_t *emptyPatternCopy(const char *str) {
	return (str)?wcharstring::duplicate(str):NULL;
}

// true if "result" holds the ASCII string "expected", or both are NULL
static bool emptyPatternMatches(const wchar_t *result, const char *expected) {
	if (!result || !expected) {
		return (!result && !expected);
	}
	wchar_t	*e=emptyPatternCopy(expected);
	bool	match=!wcharstring::compare(result,e);
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
	const wchar_t	*s=emptyPatternCopy(str);
	const wchar_t	*o=emptyPatternCopy(oldstr);
	const wchar_t	*n=emptyPatternCopy(newstr);
	stringbuffer	name;
	name.append(label)->append(" replace");
	wchar_t	*result=wcharstring::replace(s,o,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase");
	result=wcharstring::replaceIgnoringCase(s,o,n);
	test(name.getString(),emptyPatternMatches(result,expectedic));
	delete[] result;
	delete[] s;
	delete[] o;
	delete[] n;
}

// checks the set forms of replace() and replaceIgnoringCase() on exact-size
// heap copies of the inputs; if "lengths" is NULL, the lengths are those of
// the members
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
	wchar_t		*s=emptyPatternCopy(str);
	const wchar_t	**o=new const wchar_t *[count+1];
	const wchar_t	**n=new const wchar_t *[count+1];
	size_t		*l=new size_t[count+1];
	for (size_t i=0; i<count; i++) {
		o[i]=emptyPatternCopy(oldset[i]);
		n[i]=emptyPatternCopy(newset[i]);
		l[i]=(lengths)?lengths[i]:wcharstring::getLength(o[i]);
	}
	o[count]=NULL;
	n[count]=NULL;
	stringbuffer	name;
	wchar_t	*result=NULL;
	name.clear();
	name.append(label)->append(" replace length");
	result=wcharstring::replace(s,o,l,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase length");
	result=wcharstring::replaceIgnoringCase(s,o,l,n);
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
static void nullSetCheck(const char *label, const wchar_t *str, wchar_t *result,
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
	wchar_t		*s=emptyPatternCopy(str);
	const wchar_t	**o=new const wchar_t *[2];
	const wchar_t	**n=new const wchar_t *[2];
	const wchar_t	**d=new const wchar_t *[2];
	size_t		*l=new size_t[1];
	o[0]=emptyPatternCopy("b");
	o[1]=NULL;
	n[0]=emptyPatternCopy("X");
	n[1]=NULL;
	d[0]=NULL;
	d[1]=NULL;
	l[0]=1;
	const wchar_t * const	*nullset=NULL;
	size_t			*nulllen=NULL;
	stringbuffer		name;
	for (int ic=0; ic<2; ic++) {
		const char	*fn=(ic)?" replaceIgnoringCase":" replace";
		#define NULLSETNAME(what) \
			name.clear(); \
			name.append(label)->append(fn)->append(what)
		#define NULLSETCALL(args) \
			((ic)?wcharstring::replaceIgnoringCase args: \
				wcharstring::replace args)
		NULLSETNAME(" NULL oldstrset, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,nullset,l,n)),str);
		NULLSETNAME(" NULL oldstrlen");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,nulllen,n)),str);
		NULLSETNAME(" NULL newstrset, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,l,nullset)),str);
		NULLSETNAME(" NULL str, length");
		test(name.getString(),
			!NULLSETCALL((NULL,nullset,nulllen,nullset)));
		NULLSETNAME(" valid arrays, length");
		nullSetCheck(name.getString(),s,
			NULLSETCALL((s,o,l,n)),bx);
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
	wchar_t	*s=emptyPatternCopy(str1);
	const wchar_t	*p=emptyPatternCopy(str2);
	bool	retval=wcharstring::strip(s,p);
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
	wchar_t	*h=emptyPatternCopy(hay);
	wchar_t	*n=emptyPatternCopy(needle);
	const wchar_t	*ch=h;
	const wchar_t	*r;
	stringbuffer	name;
	name.append(label)->append(" findLast");
	r=wcharstring::findLast(ch,(const wchar_t *)n);
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=wcharstring::findLast(h,(const wchar_t *)n);
	name.clear();
	name.append(label)->append(" findLast non-const");
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=wcharstring::findLastIgnoringCase(ch,(const wchar_t *)n);
	name.clear();
	name.append(label)->append(" findLastIgnoringCase");
	test(name.getString(),(r)?(r-h==expic):(expic==-1));
	if (n[0] && !n[1]) {
		r=wcharstring::findLast(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLast char");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
		r=wcharstring::findLastIgnoringCase(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase char");
		test(name.getString(),(r)?(r-h==expic):(expic==-1));
		r=wcharstring::findLast(h,n[0]);
		name.clear();
		name.append(label)->append(" findLast char non-const");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
	}
	if (!n[0]) {
		r=wcharstring::findLast(ch,(wchar_t)0);
		name.clear();
		name.append(label)->append(" findLast terminator");
		test(name.getString(),r==h+wcharstring::getLength(h));
		r=wcharstring::findLastIgnoringCase(ch,(wchar_t)0);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase terminator");
		test(name.getString(),r==h+wcharstring::getLength(h));
	}
	delete[] h;
	delete[] n;
}

// checks the NULL arguments of findLast and findLastIgnoringCase
static void findLastNullTest() {
	wchar_t	*h=emptyPatternCopy("abc");
	const wchar_t	*ch=h;
	test("findLast NULL haystack",
		wcharstring::findLast((const wchar_t *)NULL,ch)==NULL);
	test("findLast NULL needle",
		wcharstring::findLast(ch,(const wchar_t *)NULL)==NULL);
	test("findLastIgnoringCase NULL haystack",
		wcharstring::findLastIgnoringCase(
				(const wchar_t *)NULL,ch)==NULL);
	test("findLastIgnoringCase NULL needle",
		wcharstring::findLastIgnoringCase(
				ch,(const wchar_t *)NULL)==NULL);
	test("findLast char NULL haystack",
		wcharstring::findLast((const wchar_t *)NULL,
						(wchar_t)'a')==NULL);
	test("findLast char 0, NULL haystack",
		wcharstring::findLast((const wchar_t *)NULL,
						(wchar_t)0)==NULL);
	test("findLastIgnoringCase char NULL haystack",
		wcharstring::findLastIgnoringCase((const wchar_t *)NULL,
							(wchar_t)'a')==NULL);
	test("findLastIgnoringCase char 0, NULL haystack",
		wcharstring::findLastIgnoringCase((const wchar_t *)NULL,
							(wchar_t)0)==NULL);
	delete[] h;
}

// checks leftTrim, strip and stripSet on exact-size heap copies; which is
// 0 leftTrim(str), 1 leftTrim(str,ch), 2 strip(str,ch), 3 strip(str1,str2),
// 4 stripSet(str,set); ch is x, str2 is "xy" and set is "xy"
static void sizeIndexTest(const char *label, int which, const char *str,
				const char *expected, bool expectedreturn) {
	wchar_t	*s=emptyPatternCopy(str);
	wchar_t	*xy=emptyPatternCopy("xy");
	bool	retval=expectedreturn;
	switch (which) {
		case 0:
			wcharstring::leftTrim(s);
			break;
		case 1:
			wcharstring::leftTrim(s,L'x');
			break;
		case 2:
			retval=wcharstring::strip(s,L'x');
			break;
		case 3:
			retval=wcharstring::strip(s,(const wchar_t *)xy);
			break;
		default:
			retval=wcharstring::stripSet(s,(const wchar_t *)xy);
			break;
	}
	test(label,retval==expectedreturn &&
			emptyPatternMatches(s,expected));
	delete[] s;
	delete[] xy;
}

// checks the length-taking forms of duplicate() and duplicateUcs2() for
// one length of a string that is at least that long
static void duplicateLengthTest(const char *label, const char *str,
				const wchar_t *wstr, const ucs2_t *ustr,
				size_t len) {
	bool		big=sys::isBigEndian();
	stringbuffer	name;
	wchar_t		*result=wcharstring::duplicate(wstr,len);
	name.append(label);
	name.append(" wchar_t");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
	result=wcharstring::duplicate(str,len);
	name.clear();
	name.append(label);
	name.append(" char");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
	result=wcharstring::duplicateUcs2(ustr,len);
	name.clear();
	name.append(label);
	name.append(" ucs2");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
	result=wcharstring::duplicateUcs2(ustr,len,big);
	name.clear();
	name.append(label);
	name.append(" ucs2 bigendian");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
	result=wcharstring::duplicateUcs2(ustr,len,L'#');
	name.clear();
	name.append(label);
	name.append(" ucs2 replacement");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
	result=wcharstring::duplicateUcs2(ustr,len,L'#',big);
	name.clear();
	name.append(label);
	name.append(" ucs2 replacement bigendian");
	test(name.getString(),result &&
		wcharstring::getLength(result)==len &&
		!wcharstring::compare(result,wstr,len));
	delete[] result;
}

// checks pad(), where "expected" is NULL if pad() must return NULL
static void padTest(const char *label, const wchar_t *str, wchar_t padchar,
			int16_t direction, uint64_t totallen,
			const wchar_t *expected) {
	wchar_t	*result=wcharstring::pad(str,padchar,direction,totallen);
	test(label,(expected)?
			(result && !wcharstring::compare(result,expected)):
			!result);
	delete[] result;
}

// checks pad(), element by element, against a result built here
static bool padMatches(const wchar_t *str, wchar_t padchar,
			int16_t direction, uint64_t totallen) {
	wchar_t	*result=wcharstring::pad(str,padchar,direction,totallen);
	if (!result) {
		return false;
	}
	uint64_t	len=(str)?wcharstring::getLength(str):0;
	uint64_t	lead=0;
	if (len<totallen) {
		lead=(direction<0)?totallen-len:
			(direction>0)?0:(totallen-len)/2;
	}
	bool	matches=(result[totallen]==0);
	for (uint64_t i=0; i<totallen; i++) {
		wchar_t	expected=padchar;
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
	wchar_t	*expected=wcharstring::duplicate(narrow);
	wchar_t	*result=wcharstring::convertAmount(amount);
	bool	matches=(result && !wcharstring::compare(result,expected));
	delete[] result;
	delete[] expected;
	delete[] narrow;
	uint16_t	paddings[]={0,3,8,12,20};
	for (size_t i=0; i<sizeof(paddings)/sizeof(paddings[0]); i++) {
		narrow=charstring::convertAmount(amount,paddings[i]);
		expected=wcharstring::duplicate(narrow);
		result=wcharstring::convertAmount(amount,paddings[i]);
		if (!result || wcharstring::compare(result,expected) ||
			wcharstring::getLength(result)!=
				charstring::getLength(narrow)) {
			matches=false;
		}
		delete[] result;
		delete[] expected;
		delete[] narrow;
	}
	return matches;
}

// checks insertString() on exact-size heap copies of the inputs, so that
// AddressSanitizer sees any read outside them; a NULL stays NULL
static wchar_t *exactCopy(const char *str) {
	if (!str) {
		return NULL;
	}
	size_t	len=charstring::getLength(str);
	wchar_t	*copy=new wchar_t[len+1];
	for (size_t i=0; i<=len; i++) {
		copy[i]=(wchar_t)str[i];
	}
	return copy;
}

static void insertStringExactTest(const char *label, const char *dest,
					const char *src, uint64_t index,
					const char *expected) {
	wchar_t	*d=exactCopy(dest);
	wchar_t	*s=exactCopy(src);
	wchar_t	*e=exactCopy(expected);
	wchar_t	*result=wcharstring::insertString(d,s,index);
	test(label,result && !wcharstring::compare(result,e));
	delete[] result;
	delete[] d;
	delete[] s;
	delete[] e;
}

// checks insertString()
static void insertStringTest(const char *label, const wchar_t *dest,
				const wchar_t *src, uint64_t index,
				const wchar_t *expected) {
	wchar_t	*result=wcharstring::insertString(dest,src,index);
	test(label,result && !wcharstring::compare(result,expected));
	delete[] result;
}

// checks the out-pointer forms of escape() and unescape()
static void escapeLengthTest(const char *label, const wchar_t *input,
				uint64_t inputlen, const wchar_t *characters,
				const wchar_t *expectedescaped) {
	wchar_t		*output=NULL;
	uint64_t	outputlen=0;
	wcharstring::escape(input,inputlen,&output,&outputlen,characters);
	test(label,output && !wcharstring::compare(output,expectedescaped) &&
			outputlen==wcharstring::getLength(expectedescaped));
	wchar_t		*escaped=output;
	uint64_t	escapedlen=outputlen;
	output=NULL;
	outputlen=0;
	wcharstring::unescape(escaped,escapedlen,&output,&outputlen);
	stringbuffer	name;
	name.append("un");
	name.append(label);
	test(name.getString(),output &&
			!wcharstring::compare(output,input,inputlen) &&
			outputlen==inputlen);
	delete[] output;
	delete[] escaped;
}

// returns a heap array of exactly "n" elements that holds the first "n"
// ASCII characters of "src"; there is no terminator unless "src" has one
static wchar_t *rangeMake(const char *src, size_t n) {
	wchar_t	*buf=new wchar_t[n];
	for (size_t i=0; i<n; i++) {
		buf[i]=(wchar_t)(unsigned char)src[i];
	}
	return buf;
}

// true if the first "n" elements of "buf" are the ASCII characters of "exp"
static bool rangeEqual(const wchar_t *buf, const char *exp, size_t n) {
	for (size_t i=0; i<n; i++) {
		if (buf[i]!=(wchar_t)(unsigned char)exp[i]) {
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
			wchar_t	*buf=rangeMake(str,n+1);
			if (ch) {
				wcharstring::rightTrim(buf,(wchar_t)ch);
			} else {
				wcharstring::rightTrim(buf);
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
	wchar_t	buf[1+8+5+1];
	buf[0]=(wchar_t)'X';
	for (size_t i=1; i<1+8+n; i++) {
		buf[i]=(wchar_t)fill;
	}
	buf[1+8+n]=(wchar_t)0;
	if (usech) {
		wcharstring::rightTrim(buf+1+8,(wchar_t)fill);
	} else {
		wcharstring::rightTrim(buf+1+8);
	}
	bool	ok=(buf[0]==(wchar_t)'X' && !buf[1+8]);
	for (size_t i=1; i<1+8; i++) {
		ok=(ok && buf[i]==(wchar_t)fill);
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
			wchar_t	*buf=rangeMake(str,n);
			if (which==0) {
				wcharstring::leftJustify(buf,(int32_t)n);
			} else if (which==1) {
				wcharstring::rightJustify(buf,(int32_t)n);
			} else {
				wcharstring::center(buf,(int32_t)n);
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
		wchar_t	*buf=rangeMake(" a ",n);
		wcharstring::leftJustify(buf,len);
		wcharstring::rightJustify(buf,len);
		wcharstring::center(buf,len);
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
		wchar_t	*buf=rangeMake(str,n);
		wcharstring::center(buf,(int32_t)n);
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
			wcharstring::center(buf,(int32_t)n);
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
	wchar_t	buf[4];
	buf[0]=(wchar_t)'x';
	buf[1]=(wchar_t)'1';
	buf[2]=(wchar_t)'2';
	buf[3]=(wchar_t)'3';
	bool	ok=(!wcharstring::isInteger(buf+1,len) &&
			!wcharstring::isNumber(buf+1,len));
	wchar_t	*exact=rangeMake("123",3);
	ok=(ok && !wcharstring::isInteger(exact,len) &&
			!wcharstring::isNumber(exact,len));
	delete[] exact;
	test(label,ok);
}

// checks isInteger() and isNumber() with a length, in an exact-size heap
// buffer that has no terminator
static void rangeNumberTest(const char *str, size_t n, bool isinteger,
							bool isnumber) {
	wchar_t	*buf=rangeMake(str,n);
	bool	ok=(wcharstring::isInteger(buf,(int32_t)n)==isinteger &&
			wcharstring::isNumber(buf,(int32_t)n)==isnumber);
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
	wchar_t	*r=wcharstring::convertAmount(c->amount);
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
		r=wcharstring::convertAmount(c->amount,spaces[i]);
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
	wchar_t	*r=wcharstring::parseNumber(number,zeropadding);
	name.append("parseNumber ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),emptyPatternMatches(r,expected));
	delete[] r;
	if (zeropadding==1) {
		r=wcharstring::parseNumber(number);
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
	wstringbuffer	sb;
	if (zeropadding==1) {
		sb.append(number);
	} else {
		sb.append(number,zeropadding);
	}
	wchar_t	*e=emptyPatternCopy(expected);
	name.append("append ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),!wcharstring::compare(sb.getString(),e));
	delete[] e;
}

int main(int argc, const char **argv) {

	header("wcharstring");

	// zero
	stdoutput.printf("zero...\n");
	wchar_t	s[50];
	wcharstring::zero(s,50);
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
	test("compare(NULL,NULL)",!wcharstring::compare(NULL,NULL));
	test("compare(NULL,a)",wcharstring::compare(NULL,L"a")<0);
	test("compare(a,NULL)",wcharstring::compare(L"a",NULL)>0);
	test("compareIgnoringCase(NULL,NULL)",
			!wcharstring::compareIgnoringCase(NULL,NULL));
	test("compareIgnoringCase(NULL,a)",
			wcharstring::compareIgnoringCase(NULL,L"a")<0);
	test("compareIgnoringCase(a,NULL)",
			wcharstring::compareIgnoringCase(L"a",NULL)>0);
	test("compareNatural(NULL,NULL)",
			!wcharstring::compareNatural(NULL,NULL));
	test("compareNatural(NULL,a)",
			wcharstring::compareNatural(NULL,L"a")<0);
	test("compareNatural(a,NULL)",
			wcharstring::compareNatural(L"a",NULL)>0);
	test("compareVersions(NULL,NULL)",
			!wcharstring::compareVersions(NULL,NULL));
	test("compareVersions(NULL,a)",
			wcharstring::compareVersions(NULL,L"a")<0);
	test("compareVersions(a,NULL)",
			wcharstring::compareVersions(L"a",NULL)>0);
	stdoutput.printf("\n");


	// append
	stdoutput.printf("append...\n");
	wcharstring::append(s,L"hello ");
	wcharstring::append(s,L"there ");
	wcharstring::append(s,L"dave !!!",4);
	test("append",!wcharstring::compare(s,L"hello there dave"));
	stdoutput.printf("\n");


	// copy
	stdoutput.printf("copy...\n");
	wcharstring::copy(s,L"what's ");
	test("append",!wcharstring::compare(s,L"what's "));
	wcharstring::copy(s,L"up !!!",2);
	test("append",!wcharstring::compare(s,L"upat's "));
	wcharstring::copy(s,2,L" !!!");
	test("append",!wcharstring::compare(s,L"up !!!"));
	wcharstring::copy(s,6,L"!!!!!!",1);
	test("append",!wcharstring::compare(s,L"up !!!!"));
	stdoutput.printf("\n");


	// compare
	stdoutput.printf("compare...\n");
	wcharstring::zero(s,50);
	wcharstring::append(s,L"hello");
	test("compare",!wcharstring::compare(s,L"hello"));
	test("compare",!wcharstring::compare(s,L"hello",3));
	test("compare",!wcharstring::compareIgnoringCase(s,L"HELLO"));
	test("compare",!wcharstring::compareIgnoringCase(s,L"HELLO",3));
	stdoutput.printf("\n");


	// findFirst
	stdoutput.printf("findFirst...\n");
	test("findFirst",wcharstring::findFirst(s,L"llo"));
	test("findFirstIgnoringCase",
			wcharstring::findFirstIgnoringCase(s,L"LLO"));
	test("findFirstOrEnd",wcharstring::findFirstOrEnd(s,L"llo"));
	test("findFirstOrEnd",
		wcharstring::findFirstOrEnd(s,L"blah")==
						s+wcharstring::getLength(s));
	test("findFirstOrEndIgnoringCase",
		wcharstring::findFirstOrEndIgnoringCase(s,L"LLO"));
	test("findFirstOrEndIgnoringCase",
		wcharstring::findFirstOrEndIgnoringCase(s,L"BLAH")==
						s+wcharstring::getLength(s));
	test("findFirst",wcharstring::findFirst(s,L'l'));
	test("findFirstIgnoringCase",
		wcharstring::findFirstIgnoringCase(s,L'L'));
	test("findFirstOrEnd",
		wcharstring::findFirstOrEnd(s,L'z')==s+wcharstring::getLength(s));
	test("findFirstOrEnd",wcharstring::findFirstOrEnd(s,L'l'));
	test("findFirstOrEndIgnoringCase",
		wcharstring::findFirstOrEndIgnoringCase(s,L'Z')==
						s+wcharstring::getLength(s));
	test("findFirstOrEndIgnoringCase",
			wcharstring::findFirstOrEndIgnoringCase(s,L'l'));
	stdoutput.printf("\n");


	// findLast
	test("findLast",wcharstring::findLast(s,L"llo"));
	test("findLastIgnoringCase",
			wcharstring::findLastIgnoringCase(s,L"LLO"));
	test("findLast",wcharstring::findLast(s,L'l'));
	test("findLastIgnoringCase",wcharstring::findLastIgnoringCase(s,L'L'));
	stdoutput.printf("\n");


	// contains
	stdoutput.printf("contains...\n");
	test("contains",wcharstring::contains(s,L"llo"));
	test("containsIgnoringCase",
			wcharstring::containsIgnoringCase(s,L"LLO"));
	stdoutput.printf("\n");


	// startsWith/endsWith
	stdoutput.printf("startsWith/endsWith...\n");
	test("startsWith",wcharstring::startsWith(L"hello",L"he"));
	test("startsWith",wcharstring::startsWith(L"hello",L"hello"));
	test("startsWith",!wcharstring::startsWith(L"hello",L"el"));
	test("startsWith",!wcharstring::startsWith(L"hello",L"helloThere"));
	test("endsWith",wcharstring::endsWith(L"hello",L"lo"));
	test("endsWith",wcharstring::endsWith(L"hello",L"hello"));
	test("endsWith",!wcharstring::endsWith(L"hello",L"ll"));
	test("endsWith",!wcharstring::endsWith(L"hello",L"helloThere"));
	stdoutput.printf("\n");


	// duplicate
	stdoutput.printf("duplicate/subString...\n");
	wchar_t	*hellothere=wcharstring::duplicate(s);
	test("duplicate",!wcharstring::compare(hellothere,L"hello"));
	wchar_t	*ell=wcharstring::getSubString(hellothere,1,3);
	test("subString",!wcharstring::compare(ell,L"ell"));
	wchar_t	*llo=wcharstring::getSubString(hellothere,2,4);
	test("subString",!wcharstring::compare(llo,L"llo"));
	delete[] hellothere;
	delete[] ell;
	delete[] llo;
	stdoutput.printf("\n");


	// printf
	stdoutput.printf("printf...\n");
	wchar_t	buffer[100];
	if (wcharstring::supportsPrintf()) {
		ssize_t	size=wcharstring::printf(buffer,20,
					L"%lc%d%ls",L'a',123,L"hello");
		test("size",size==9);
		test("contents",!wcharstring::compare(buffer,L"a123hello"));
		wchar_t	*buf=NULL;
		size=wcharstring::printf(&buf,L"%lc%d%ls",L'a',123,L"hello");
		test("size",size==9);
		test("contents",!wcharstring::compare(buffer,L"a123hello"));
		delete[] buf;

		// force the truncation-retry loop through a real retry
		// (ticket #10023 - each retry needs its own va_copy'd
		// va_list, or a %ls argument like "retrystr" below can pick
		// up a stale argument pointer on the second attempt).  The
		// auto-allocating overload always starts the loop at buflen
		// 0, guaranteeing at least one retry.
		const wchar_t	*retrystr=L"retrystring";
		wchar_t	*buf2=NULL;
		size=wcharstring::printf(&buf2,L"%lc%d%ls",L'a',123,retrystr);
		test("retry size",size==15);
		test("retry contents",
			!wcharstring::compare(buf2,L"a123retrystring"));
		delete[] buf2;

		// force the truncation-retry loop through several growths,
		// not just one (ticket #10139 - on linux/glibc, vswprintf()
		// reports truncation via E2BIG once the shortfall is more
		// than one wide character, which used to make the loop bail
		// out with a false failure instead of growing the buffer
		// again and retrying).  The auto-allocating overload starts
		// the loop at buflen 0 and grows it 16, 48, 112, 240, ...,
		// so a result this long forces several of those growths
		// before one is large enough.
		const size_t	longretrylen=300;
		wchar_t	longretrystr[longretrylen+1];
		for (size_t i=0; i<longretrylen; i++) {
			longretrystr[i]=L'a'+(wchar_t)(i%26);
		}
		longretrystr[longretrylen]=L'\0';
		wchar_t	*buf3=NULL;
		size=wcharstring::printf(&buf3,L"%ls",longretrystr);
		test("multi-step retry size",size==(ssize_t)longretrylen);
		test("multi-step retry contents",
			!wcharstring::compare(buf3,longretrystr));
		delete[] buf3;
	} else {
		stdoutput.printf("not supported\n");
	}
	stdoutput.printf("\n");


	// split
	stdoutput.printf("split...\n");
	wchar_t		**list;
	uint64_t	listlength;
	wcharstring::split(L"hello||hi||bye||goodbye",L"||",false,
						&list,&listlength);
	test("length",listlength==4);
	test("list[0]",!wcharstring::compare(list[0],L"hello"));
	test("list[1]",!wcharstring::compare(list[1],L"hi"));
	test("list[2]",!wcharstring::compare(list[2],L"bye"));
	test("list[3]",!wcharstring::compare(list[3],L"goodbye"));
	uint64_t i;
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"hello||hi||bye||goodbye||",L"||",false,
						&list,&listlength);
	test("length",listlength==5);
	test("list[0]",!wcharstring::compare(list[0],L"hello"));
	test("list[1]",!wcharstring::compare(list[1],L"hi"));
	test("list[2]",!wcharstring::compare(list[2],L"bye"));
	test("list[3]",!wcharstring::compare(list[3],L"goodbye"));
	test("list[4]",!wcharstring::compare(list[4],L""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"||hello||hi||bye||goodbye||",L"||",false,
						&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!wcharstring::compare(list[0],L""));
	test("list[1]",!wcharstring::compare(list[1],L"hello"));
	test("list[2]",!wcharstring::compare(list[2],L"hi"));
	test("list[3]",!wcharstring::compare(list[3],L"bye"));
	test("list[4]",!wcharstring::compare(list[4],L"goodbye"));
	test("list[5]",!wcharstring::compare(list[5],L""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"||||hello||||hi||||bye||||goodbye||||",L"||",false,
							&list,&listlength);
	test("length",listlength==11);
	test("list[0]",!wcharstring::compare(list[0],L""));
	test("list[1]",!wcharstring::compare(list[1],L""));
	test("list[2]",!wcharstring::compare(list[2],L"hello"));
	test("list[3]",!wcharstring::compare(list[3],L""));
	test("list[4]",!wcharstring::compare(list[4],L"hi"));
	test("list[5]",!wcharstring::compare(list[5],L""));
	test("list[6]",!wcharstring::compare(list[6],L"bye"));
	test("list[7]",!wcharstring::compare(list[7],L""));
	test("list[8]",!wcharstring::compare(list[8],L"goodbye"));
	test("list[9]",!wcharstring::compare(list[9],L""));
	test("list[10]",!wcharstring::compare(list[10],L""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"||||||||||",L"||",false,&list,&listlength);
	test("length",listlength==6);
	test("list[0]",!wcharstring::compare(list[0],L""));
	test("list[1]",!wcharstring::compare(list[1],L""));
	test("list[2]",!wcharstring::compare(list[2],L""));
	test("list[3]",!wcharstring::compare(list[3],L""));
	test("list[4]",!wcharstring::compare(list[4],L""));
	test("list[5]",!wcharstring::compare(list[5],L""));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"http://www.firstworks.com/application/app.cgi/skin/module/template.html",L"/",true,&list,&listlength);
	test("length",listlength==7);
	test("list[0]",!wcharstring::compare(list[0],L"http:"));
	test("list[1]",!wcharstring::compare(list[1],L"www.firstworks.com"));
	test("list[2]",!wcharstring::compare(list[2],L"application"));
	test("list[3]",!wcharstring::compare(list[3],L"app.cgi"));
	test("list[4]",!wcharstring::compare(list[4],L"skin"));
	test("list[5]",!wcharstring::compare(list[5],L"module"));
	test("list[6]",!wcharstring::compare(list[6],L"template.html"));
	for (i=0; i<listlength; i++) {
		delete[] list[i];
	}
	delete[] list;

	wcharstring::split(L"1.2.3.4.5.6",L".",false,NULL,&listlength);
	test("length",listlength==6);

	// this test just shouldn't crash
	wcharstring::split(L"1.2.3.4.5.6",L".",false,NULL,NULL);
	stdoutput.printf("\n");

	// isBefore, isBetween, isAfter
	stdoutput.printf("isBefore/isBetween/isAfter...\n");
	wchar_t	*bef=wcharstring::isBefore(L"one two three four five",L" two");
	test("isBefore",!wcharstring::compare(bef,L"one"));
	wchar_t	*btw=wcharstring::isBetween(L"one two three four five",
							L"two ",L" four");
	test("isBetween",!wcharstring::compare(btw,L"three"));
	wchar_t	*aft=wcharstring::isAfter(L"one two three four five",L"four ");
	test("isAfter",!wcharstring::compare(aft,L"five"));
	stdoutput.printf("\n");


	// escape/unescape
	stdoutput.printf("escape/unescape...\n");
	wchar_t	str[]=L"hello'\"\\hello'\"\\";
	wchar_t	*escapedstr=wcharstring::escape(str,L"\"'");
	test("escape",!wcharstring::compare(escapedstr,
					L"hello\\'\\\"\\\\hello\\'\\\"\\\\"));
	wchar_t	*unescapedstr=wcharstring::unescape(escapedstr);
	test("unescape",!wcharstring::compare(unescapedstr,
					L"hello'\"\\hello'\"\\"));
	delete[] unescapedstr;
	delete[] escapedstr;
	stdoutput.printf("\n");


	// sets
	stdoutput.printf("sets...\n");
	const wchar_t	*alphabet=
		L"aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	test("lengthContainingSet",
		wcharstring::getLengthContainingSet(
				alphabet,L"gfedcba")==14);
	test("lengthNotContainingSet",
		wcharstring::getLengthNotContainingSet(
				alphabet,L"hijklmnopqrstuvwxyz")==14);
	test("findFirstOfSet",wcharstring::findFirstOfSet(alphabet,L"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd",
			wcharstring::findFirstOfSetOrEnd(alphabet,L"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd (end)",
			wcharstring::findFirstOfSetOrEnd(alphabet,L"123")==
				(alphabet+wcharstring::getLength(alphabet)));
	test("findLastOfSetOrEnd",
			wcharstring::findLastOfSetOrEnd(alphabet,L"abc")==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
			wcharstring::findLastOfSetOrEnd(alphabet,L"123")==
				(alphabet+wcharstring::getLength(alphabet)));
	stdoutput.printf("\n");


	// trim
	stdoutput.printf("trim...\n");
	wcharstring::copy(buffer,L"\r\n	     hello there buddy     	\r\n");
	wcharstring::rightTrim(buffer);
	test("rtrim",!wcharstring::compare(buffer,
				L"\r\n	     hello there buddy"));
	wcharstring::leftTrim(buffer);
	test("ltrim",!wcharstring::compare(buffer,L"hello there buddy"));
	stdoutput.printf("\n");


	// strip
	stdoutput.printf("strip...\n");
	wcharstring::copy(buffer,L"   hello      there   buddy  ");
	wcharstring::strip(buffer,L' ');
	test("strip",!wcharstring::compare(buffer,L"hellotherebuddy"));
	wcharstring::copy(buffer,L"   hello       there  buddy  ");
	wcharstring::strip(buffer,L"   ");
	test("strip",!wcharstring::compare(buffer,L"hello there  buddy  "));
	stdoutput.printf("\n");


	// justification
	stdoutput.printf("justification...\n");
	wchar_t	hello[16];
	wcharstring::copy(hello,L"     hello     ");
	wcharstring::leftJustify(hello,15);
	test("left",!wcharstring::compare(hello,L"hello          "));
	wcharstring::rightJustify(hello,15);
	test("right",!wcharstring::compare(hello,L"          hello"));
	wcharstring::copy(hello,L"hello          ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L" hello         ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"  hello        ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"   hello       ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"    hello      ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"     hello     ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"      hello    ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"       hello   ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"        hello  ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"         hello ");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	wcharstring::copy(hello,L"          hello");
	wcharstring::center(hello,15);
	test("center",!wcharstring::compare(hello,L"     hello     "));
	stdoutput.printf("\n");


	// case conversion
	stdoutput.printf("case conversion...\n");
	wcharstring::upper(buffer);
	test("upper",!wcharstring::compare(buffer,L"HELLO THERE  BUDDY  "));
	wcharstring::lower(buffer);
	test("lower",!wcharstring::compare(buffer,L"hello there  buddy  "));
	wcharstring::capitalize(buffer);
	test("caps",!wcharstring::compare(buffer,L"Hello There  Buddy  "));
	stdoutput.printf("\n");
	

	// numbers
	stdoutput.printf("number...\n");
	test("isNumber: -100.5",wcharstring::isNumber(L"-100.5"));
	test("isNumber: -100.5.10",!wcharstring::isNumber(L"-100.5.10"));
	test("isInteger: -100",wcharstring::isInteger(L"-100"));
	test("isInteger: -100.5",!wcharstring::isInteger(L"-100.5"));
	test("isInteger: -100.5.10",!wcharstring::isInteger(L"-100.5.10"));
	test("integerLength: 0",
			wcharstring::getIntegerLength((int64_t)0)==1);
	test("integerLength: 1",
			wcharstring::getIntegerLength((int64_t)1)==1);
	test("integerLength: 10",
			wcharstring::getIntegerLength((int64_t)10)==2);
	test("integerLength: 100",
			wcharstring::getIntegerLength((int64_t)100)==3);
	test("integerLength: 1000",
			wcharstring::getIntegerLength((int64_t)1000)==4);
	test("integerLength: -1",
			wcharstring::getIntegerLength((int64_t)-1)==2);
	test("integerLength: -10",
			wcharstring::getIntegerLength((int64_t)-10)==3);
	test("integerLength: -100",
			wcharstring::getIntegerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			wcharstring::getIntegerLength((int64_t)-1000)==5);
	test("integerLength: 0",
			wcharstring::getIntegerLength((uint64_t)0)==1);
	test("integerLength: 1",
			wcharstring::getIntegerLength((uint64_t)1)==1);
	test("integerLength: 10",
			wcharstring::getIntegerLength((uint64_t)10)==2);
	test("integerLength: 100",
			wcharstring::getIntegerLength((uint64_t)100)==3);
	test("integerLength: 1000",
			wcharstring::getIntegerLength((uint64_t)1000)==4);

	uint64_t	testuint64=ULONG_MAX;
	wchar_t		*teststr=wcharstring::parseNumber(testuint64);
	test("parseNumber ULONG_MAX",
		!wcharstring::compare(teststr,
			(testuint64==4294967295UL)
				?L"4294967295"
				:L"18446744073709551615"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testuint64,40);
	test("parseNumber ULONG_MAX",
		!wcharstring::compare(teststr,
			(testuint64==4294967295UL)
				?L"0000000000000000000000000000004294967295"
				:L"0000000000000000000018446744073709551615"));
	delete[] teststr;

	int64_t		testint64=LONG_MAX;
	teststr=wcharstring::parseNumber(testint64);
	test("parseNumber LONG_MAX",
		!wcharstring::compare(teststr,
			(testint64==2147483647)
				?L"2147483647"
				:L"9223372036854775807"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint64,40);
	test("parseNumber LONG_MAX",
		!wcharstring::compare(teststr,
			(testint64==2147483647)
				?L"0000000000000000000000000000002147483647"
				:L"0000000000000000000009223372036854775807"));
	testint64=-1*LONG_MAX;
	teststr=wcharstring::parseNumber(testint64);
	test("parseNumber -LONG_MAX",
		!wcharstring::compare(teststr,
			(testint64==-2147483647)
				?L"-2147483647"
				:L"-9223372036854775807"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint64,40);
	test("parseNumber -LONG_MAX",
		!wcharstring::compare(teststr,
			(testint64==-2147483647)
				?L"-000000000000000000000000000002147483647"
				:L"-000000000000000000009223372036854775807"));
	delete[] teststr;

	uint32_t	testuint32=4294967295UL;
	teststr=wcharstring::parseNumber(testuint32);
	test("parseNumber 2^32-1",
		!wcharstring::compare(teststr,L"4294967295"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testuint32,40);
	test("parseNumber 2^32-1",
		!wcharstring::compare(teststr,
			L"0000000000000000000000000000004294967295"));
	delete[] teststr;

	int32_t		testint32=2147483647;
	teststr=wcharstring::parseNumber(testint32);
	test("parseNumber 2^31-1",
		!wcharstring::compare(teststr,L"2147483647"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint32,40);
	test("parseNumber 2^32-1",
		!wcharstring::compare(teststr,
			L"0000000000000000000000000000002147483647"));
	delete[] teststr;
	testint32=-2147483647;
	teststr=wcharstring::parseNumber(testint32);
	test("parseNumber -2^31-1",
		!wcharstring::compare(teststr,L"-2147483647"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint32,40);
	test("parseNumber -2^32-1",
		!wcharstring::compare(teststr,
			L"-000000000000000000000000000002147483647"));
	delete[] teststr;

	uint16_t	testuint16=65535;
	teststr=wcharstring::parseNumber(testuint16);
	test("parseNumber 2^16-1",
		!wcharstring::compare(teststr,L"65535"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testuint16,40);
	test("parseNumber 2^16-1",
		!wcharstring::compare(teststr,
			L"0000000000000000000000000000000000065535"));
	delete[] teststr;

	int16_t		testint16=32767;
	teststr=wcharstring::parseNumber(testint16);
	test("parseNumber 2^15-1",
		!wcharstring::compare(teststr,L"32767"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint16,40);
	test("parseNumber 2^15-1",
		!wcharstring::compare(teststr,
			L"0000000000000000000000000000000000032767"));
	testint16=-32767;
	teststr=wcharstring::parseNumber(testint16);
	test("parseNumber -2^15-1",
		!wcharstring::compare(teststr,L"-32767"));
	delete[] teststr;
	teststr=wcharstring::parseNumber(testint16,40);
	test("parseNumber -2^15-1",
		!wcharstring::compare(teststr,
			L"-000000000000000000000000000000000032767"));
	delete[] teststr;
	stdoutput.printf("\n");
	

	// empty/null/yes/no
	stdoutput.printf("empty/null/yes/no...\n");
	test("NULL",wcharstring::isNullOrEmpty((const wchar_t *)NULL));
	test("empty",wcharstring::isNullOrEmpty(L""));
	test("not null",!wcharstring::isNullOrEmpty(L"not null"));
	test("not empty",!wcharstring::isNullOrEmpty(L"not empty"));
	test("yes",wcharstring::isYes(L"yes"));
	test("Yes",wcharstring::isYes(L"Yes"));
	test("YES",wcharstring::isYes(L"YES"));
	test("yes,",wcharstring::isYes(L"yes,"));
	test("yes ",wcharstring::isYes(L"yes "));
	test("yesterday",!wcharstring::isYes(L"yesterday"));
	test("Y",wcharstring::isYes(L"Y"));
	test("y",wcharstring::isYes(L"y"));
	test("yo",!wcharstring::isYes(L"yo"));
	test("true",wcharstring::isYes(L"true"));
	test("True",wcharstring::isYes(L"True"));
	test("TRUE",wcharstring::isYes(L"TRUE"));
	test("truest",!wcharstring::isYes(L"truest"));
	test("T",wcharstring::isYes(L"T"));
	test("t",wcharstring::isYes(L"t"));
	test("1",wcharstring::isYes(L"1"));
	test("123",!wcharstring::isYes(L"123"));
	test("NULL",!wcharstring::isYes(NULL));
	test("empty",!wcharstring::isYes(L""));
	test("no",wcharstring::isNo(L"no"));
	test("No",wcharstring::isNo(L"No"));
	test("NO",wcharstring::isNo(L"NO"));
	test("nothing",!wcharstring::isNo(L"nothing"));
	test("N",wcharstring::isNo(L"N"));
	test("n",wcharstring::isNo(L"n"));
	test("ne",!wcharstring::isNo(L"ne"));
	test("false",wcharstring::isNo(L"false"));
	test("False",wcharstring::isNo(L"False"));
	test("FALSE",wcharstring::isNo(L"FALSE"));
	test("falsest",!wcharstring::isNo(L"falsest"));
	test("F",wcharstring::isNo(L"F"));
	test("f",wcharstring::isNo(L"f"));
	test("0",wcharstring::isNo(L"0"));
	test("012",!wcharstring::isNo(L"012"));
	test("NULL",!wcharstring::isNo(NULL));
	test("empty",!wcharstring::isNo(L""));
	stdoutput.printf("\n");
	

	// replace
	stdoutput.printf("replace...\n");
	const wchar_t *original=L"one:-two:-one:-three:-one:-four:-one:-five";
	wchar_t	*originalcopy=wcharstring::duplicate(original);
	wcharstring::replace(originalcopy,L'-',L'|');
	test("replace char",
		!wcharstring::compare(originalcopy,
			L"one:|two:|one:|three:|one:|four:|one:|five"));
	wcharstring::replace(originalcopy,L'-',L'|');
	test("replace char (not found)",
		!wcharstring::compare(originalcopy,
			L"one:|two:|one:|three:|one:|four:|one:|five"));
	wcharstring::replace(originalcopy,L":|",L'-');
	test("replace set",
		!wcharstring::compare(originalcopy,
			L"one--two--one--three--one--four--one--five"));
	wcharstring::replace(originalcopy,L":|",L'-');
	test("replace set (not found)",
		!wcharstring::compare(originalcopy,
			L"one--two--one--three--one--four--one--five"));
	delete[] originalcopy;
	wchar_t	*newstr=wcharstring::replace(original,L"one",L"1");
	test("replace strings (smaller)",
		!wcharstring::compare(newstr,
			L"1:-two:-1:-three:-1:-four:-1:-five"));
	delete[] newstr;
	newstr=wcharstring::replace(original,L"one",L"oneone");
	test("replace strings (larger)",
		!wcharstring::compare(newstr,
		L"oneone:-two:-oneone:-three:-oneone:-four:-oneone:-five"));
	delete[] newstr;

	newstr=wcharstring::replace(original,L"onee",L"1");
	test("replace strings (not found)",
		!wcharstring::compare(newstr,
		L"one:-two:-one:-three:-one:-four:-one:-five"));
	delete[] newstr;
	const wchar_t * const oldset[]={
		L"one",L"two",L"three",L"four",L"five",NULL
	};
	size_t	oldlen[]={
		3,3,5,4,4
	};
	const wchar_t * const newset1[]={
		L"1",L"2",L"3",L"4",L"5",NULL
	};
	newstr=wcharstring::replace(original,oldset,oldlen,newset1);
	test("replace strings set (smaller)",
		!wcharstring::compare(newstr,
			L"1:-2:-1:-3:-1:-4:-1:-5"));
	delete[] newstr;
	const wchar_t * const newset2[]={
		L"oneone",L"twotwo",L"threethree",L"fourfour",L"fivefive",NULL
	};
	newstr=wcharstring::replace(original,oldset,oldlen,newset2);
	test("replace strings set (larger)",
		!wcharstring::compare(newstr,
			L"oneone:-twotwo:-oneone:-threethree:-"
			L"oneone:-fourfour:-oneone:-fivefive"));
	delete[] newstr;
	stdoutput.printf("\n");


	// duplicate from char
	stdoutput.printf("duplicate from char...\n");
	char	chbuf[128];
	wchar_t	wchbuf[128];
	for (uint16_t c=1; c<=127; c++) {
		chbuf[c-1]=(char)c;
		wchbuf[c-1]=(wchar_t)c;
	}
	chbuf[127]='\0';
	wchbuf[127]=L'\0';
	wchar_t	*t=wcharstring::duplicate(chbuf);
	test("duplicate",!wcharstring::compare(t,wchbuf));
	delete[] t;
	t=wcharstring::duplicate(chbuf,20);
	test("duplicate",!wcharstring::compare(t,wchbuf,20));
	delete[] t;
	stdoutput.printf("\n");


	// compare with wildcard
	stdoutput.printf("compareWithWildcard...\n");
	test("degenerate",
		wcharstring::compareWithWildcards(
				NULL,NULL,L'\0',L'\0'));
	test("string==string (no wildcards)",
		wcharstring::compareWithWildcards(
				L"string",L"string",L'\0',L'\0'));
	test("str?ng==str?ng (no wildcards)",
		wcharstring::compareWithWildcards(
				L"str?ng",L"str?ng",L'\0',L'\0'));
	test("str*ng==str*ng (no wildcards)",
		wcharstring::compareWithWildcards(
				L"str*ng",L"str*ng",L'\0',L'\0'));
	wchar_t	string[17];
	wcharstring::copy(string,L"0123456789abcdef");
	wchar_t	pattern[17];
	wstringbuffer	title;
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

		uint16_t	count=sizeof(pattern)/sizeof(wchar_t);
		for (uint16_t i=0; i<count-1; i++) {

			// initialize pattern
			wcharstring::copy(pattern,L"0123456789abcdef");

			// swap in some set of wildcards
			switch (iter) {
				case 0:
					pattern[i]=L'?';
					break;
				case 1:
					pattern[i]=L'*';
					break;
				case 2:
					for (uint16_t j=i;
						j<count-1; j++) {
						pattern[j]=L'?';
					}
					break;
				case 3:
					for (uint16_t j=i;
						j<count-1; j++) {
						pattern[j]=L'*';
					}
					break;
				case 4:
					for (uint16_t j=i;
						j<count-1; j=j+2) {
						pattern[j]=L'?';
					}
					break;
				case 5:
					for (uint16_t j=i;
						j<count-1; j=j+2) {
						pattern[j]=L'*';
					}
					break;
				case 6:
					for (uint16_t j=i;
						j<count-1; j++) {
						if (j%2) {
							pattern[j]=L'?';
						} else {
							pattern[j]=L'*';
						}
					}
					break;
				case 7:
					for (uint16_t j=i;
						j<count-1; j++) {
						if (j%2) {
							pattern[j]=L'*';
						} else {
							pattern[j]=L'?';
						}
					}
					break;
				case 8:
					pattern[i]=L'*';
					pattern[i+1]=L'\0';
					break;
				case 9:
					pattern[0]=L'*';
                                        // do this rather than just
                                        // L"0123456789abcdef"+i+1 directly in
                                        // the copy to silence compiler warnings
                                        // on freebsd 13.1 (and probably other
                                        // platforms)
					const wchar_t	*chars=
						L"0123456789abcdef";
					wcharstring::copy(pattern+1,chars+i+1);
					break;
			}

			// build the title
			title.clear();
			title.append(string)->append(L'=')->append(pattern);

			// run the test
			test(title.getString(),
				wcharstring::compareWithWildcards(
						string,pattern,L'?',L'*'));
		}
	}
	stdoutput.printf("\n");
	

	// compare versions
	stdoutput.printf("compareVersions...\n");
	test("1 vs. 2",
		wcharstring::compareVersions(L"1",L"2")<0);
	test("2 vs. 1",
		wcharstring::compareVersions(L"2",L"1")>0);
	test("1.1 vs. 1.2",
		wcharstring::compareVersions(L"1.1",L"1.2")<0);
	test("1.2 vs. 1.1",
		wcharstring::compareVersions(L"1.2",L"1.1")>0);
	test("1.1.1 vs. 1.1.2",
		wcharstring::compareVersions(L"1.1.1",L"1.1.2")<0);
	test("1.1.2 vs. 1.1.1",
		wcharstring::compareVersions(L"1.1.2",L"1.1.1")>0);
	test("1.1.2 vs. 1.1.12",
		wcharstring::compareVersions(L"1.1.2",L"1.1.12")<0);
	test("1.1.12 vs. 1.1.2",
		wcharstring::compareVersions(L"1.1.12",L"1.1.2")>0);
	test("1.1 vs. 1.1.1",
		wcharstring::compareVersions(L"1.1",L"1.1.1")<0);
	test("1.1.1 vs. 1.1",
		wcharstring::compareVersions(L"1.1.1",L"1.1")>0);
	test("1.1. vs. 1.1.1",
		wcharstring::compareVersions(L"1.1.",L"1.1.1")<0);
	test("1.1.1 vs. 1.1.",
		wcharstring::compareVersions(L"1.1.1",L"1.1.")>0);
	stdoutput.printf("\n");
	

	// compare natural
	stdoutput.printf("compareNatural...\n");
	test("rudiments-1 vs. rudiments-2",
		wcharstring::compareNatural(L"rudiments-1",
						L"rudiments-2")<0);
	test("rudiments-2 vs. rudiments-1",
		wcharstring::compareNatural(L"rudiments-2",
						L"rudiments-1")>0);
	test("rudiments-1.1 vs. rudiments-1.2",
		wcharstring::compareNatural(L"rudiments-1.1",
						L"rudiments-1.2")<0);
	test("rudiments-1.2 vs. rudiments-1.1",
		wcharstring::compareNatural(L"rudiments-1.2",
						L"rudiments-1.1")>0);
	test("rudiments-1.1.1 vs. rudiments-1.1.2",
		wcharstring::compareNatural(L"rudiments-1.1.1",
						L"rudiments-1.1.2")<0);
	test("rudiments-1.1.2 vs. rudiments-1.1.1",
		wcharstring::compareNatural(L"rudiments-1.1.2",
						L"rudiments-1.1.1")>0);
	test("rudiments-1.1.2 vs. rudiments-1.1.12",
		wcharstring::compareNatural(L"rudiments-1.1.2",
						L"rudiments-1.1.12")<0);
	test("rudiments-1.1.12 vs. rudiments-1.1.2",
		wcharstring::compareNatural(L"rudiments-1.1.12",
						L"rudiments-1.1.2")>0);
	test("rudiments-1.1 vs. rudiments-1.1.1",
		wcharstring::compareNatural(L"rudiments-1.1",
						L"rudiments-1.1.1")<0);
	test("rudiments-1.1.1 vs. rudiments-1.1",
		wcharstring::compareNatural(L"rudiments-1.1.1",
						L"rudiments-1.1")>0);
	test("rudiments-1.1. vs. rudiments-1.1.1",
		wcharstring::compareNatural(L"rudiments-1.1.",
						L"rudiments-1.1.1")<0);
	test("rudiments-1.1.1 vs. rudiments-1.1.",
		wcharstring::compareNatural(L"rudiments-1.1.1",
						L"rudiments-1.1.")>0);
	stdoutput.printf("\n");

	// human readable
	stdoutput.printf("humanReadable...\n");
	long double	val=2.1*1024.0*1024.0*1024.0*1024.0*
				1024.0*1024.0*1024.0*1024.0;
	wchar_t	*sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1B",!wcharstring::compare(sval,L"2.1B"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1Y",!wcharstring::compare(sval,L"2.1Y"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1Z",!wcharstring::compare(sval,L"2.1Z"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1P",!wcharstring::compare(sval,L"2.1P"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1T",!wcharstring::compare(sval,L"2.1T"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1G",!wcharstring::compare(sval,L"2.1G"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1M",!wcharstring::compare(sval,L"2.1M"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1K",!wcharstring::compare(sval,L"2.1K"));
	delete[] sval;
	val/=1024.0;
	sval=wcharstring::getHumanReadable(val);
	test("humanReadable 2.1",!wcharstring::compare(sval,L"2.1"));
	delete[] sval;

	val=2.1*1000.0*1000.0*1000.0*1000.0*
		1000.0*1000.0*1000.0*1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1B",!wcharstring::compare(sval,L"2.1B"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1Y",!wcharstring::compare(sval,L"2.1Y"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1Z",!wcharstring::compare(sval,L"2.1Z"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1P",!wcharstring::compare(sval,L"2.1P"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1T",!wcharstring::compare(sval,L"2.1T"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1G",!wcharstring::compare(sval,L"2.1G"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1M",!wcharstring::compare(sval,L"2.1M"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1K",!wcharstring::compare(sval,L"2.1K"));
	delete[] sval;
	val/=1000.0;
	sval=wcharstring::getHumanReadable(val,true);
	test("humanReadable 2.1",!wcharstring::compare(sval,L"2.1"));
	delete[] sval;

#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	ival=((uint64_t)3)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024);
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3Z",!wcharstring::compare(sval,L"3Z"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3P",!wcharstring::compare(sval,L"3P"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3T",!wcharstring::compare(sval,L"3T"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3G",!wcharstring::compare(sval,L"3G"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3M",!wcharstring::compare(sval,L"3M"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3K",!wcharstring::compare(sval,L"3K"));
	delete[] sval;
	ival/=1024;
	sval=wcharstring::getHumanReadable(ival);
	test("humanReadable 3",!wcharstring::compare(sval,L"3"));
	delete[] sval;

	int64_t	sival=((int64_t)-3)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000);
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3Z",!wcharstring::compare(sval,L"-3Z"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3P",!wcharstring::compare(sval,L"-3P"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3T",!wcharstring::compare(sval,L"-3T"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3G",!wcharstring::compare(sval,L"-3G"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3M",!wcharstring::compare(sval,L"-3M"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3K",!wcharstring::compare(sval,L"-3K"));
	delete[] sval;
	sival/=1000;
	sval=wcharstring::getHumanReadable(sival,true);
	test("humanReadable -3",!wcharstring::compare(sval,L"-3"));
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
	wchar_t	*c=wcharstring::duplicate(chars);
	test("from char",!wcharstring::compare(c,wchars));
	delete[] c;
	c=wcharstring::duplicateUcs2(ucs2chars);
	test("from ucs2",!wcharstring::compare(c,wchars));
	delete[] c;
	stdoutput.printf("\n");


	// escaping
	stdoutput.printf("quoted strings...\n");
	const wchar_t * const goodstr[]={
		// properly terminated
		L"'test'test",
		L"'''test'''test",
		L"'\\'test\\''test",
		L"'\\'''test''\\''test",
		L"'\\\\''test''\\\\'test",
		L"\"test\"test",
		L"\"\"\"test\"\"\"test",
		L"\"\\\"test\\\"\"test",
		L"\"\\\"\"\"test\"\"\\\"\"test",
		L"\"\\\\\"\"test\"\"\\\\\"test",
		L"`test`test",
		L"```test```test",
		L"`\\`test\\``test",
		L"`\\```test``\\``test",
		L"`\\\\``test``\\\\`test",
		L"[test]test",
		NULL
	};
	for (const wchar_t * const *s=goodstr; *s; s++) {
		test(*s,wcharstring::findEndOfQuotedString(*s,
					wcharstring::getLength(*s),true,true)==
					*s+wcharstring::getLength(*s)-4);
	}
	const wchar_t * const badstr[]={
		// unterminated
		L"'testtest",
		L"'''test''test",
		L"'\\'test\\'test",
		L"'\\'''test''\\'test",
		L"'\\\\''test''\\test",
		L"\"testtest",
		L"\"\"\"test\"\"test",
		L"\"\\\"test\\\"test",
		L"\"\\\"\"\"test\"\"\\\"test",
		L"\"\\\\\"\"test\"\"\\test",
		L"`testtest",
		L"```test``test",
		L"`\\`test\\`test",
		L"`\\```test``\\`test",
		L"`\\\\``test``\\test",
		L"[testtest",
		NULL
	};
	for (const wchar_t * const *s=badstr; *s; s++) {
		test(*s,wcharstring::findEndOfQuotedString(*s,
					wcharstring::getLength(*s),true,true)==
					*s+wcharstring::getLength(*s));
	}
	stdoutput.printf("\n");


	// escape/unescape, pad, insertString, and the length forms of duplicate
	stdoutput.printf("escape/pad/insertString/duplicate length...\n");
	escapeLengthTest("escape length 0",L"",0,L"\"",L"");
	escapeLengthTest("escape length 1",L"a",1,L"\"",L"a");
	escapeLengthTest("escape length 3",L"a\"b",3,L"\"",L"a\\\"b");
	escapeLengthTest("escape backslash",L"a\\b",3,L"\"",L"a\\\\b");
	padTest("pad exact length",L"abc",L'*',-1,3,L"abc");
	padTest("pad truncate",L"abcdef",L'*',-1,3,L"abc");
	padTest("pad truncate right",L"abcdef",L'*',1,4,L"abcd");
	padTest("pad zero length",L"abc",L'*',1,0,NULL);
	padTest("pad NULL zero length",NULL,L'*',1,0,NULL);
	insertStringTest("insertString start",L"hello",L"XY",0,L"XYhello");
	insertStringTest("insertString middle",L"hello",L"XY",2,L"heXYllo");
	insertStringTest("insertString end",L"hello",L"XY",5,L"helloXY");
	insertStringTest("insertString empty dest",L"",L"XY",0,L"XY");
	insertStringTest("insertString one",L"hello",L"-",3,L"hel-lo");

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


	// the methods below size their buffer in wide characters, so the first
	// length whose (length,1,1) size does not fit in a size_t is the
	// largest size_t divided by the size of a wide character
	//
	// these calls give a zero-element input buffer and a length too large
	// for the result to be sized (AddressSanitizer reports any read), and
	// rely on the size check coming before any read or write
	stdoutput.printf("escape/pad/duplicate oversized length...\n");
	size_t		none=0;
	wchar_t		*zin=(wchar_t *)new char[none];
	ucs2_t		*zuin=(ucs2_t *)new char[none];
	char		*zcin=new char[none];
	size_t		maxsize=(size_t)-1;
	size_t		firstfail=maxsize/sizeof(wchar_t);
	uint64_t	maxlen=(uint64_t)-1;
	uint64_t	lens[]={firstfail,maxlen};
	const char	*lenlabels[]={"first failing","-1"};
	for (uint16_t i=0; i<2; i++) {
		stringbuffer	name;
		wchar_t		*oversized;

		// the out-pointers are preset to non-NULL
		wchar_t		*output=zin;
		uint64_t	outputlen=1;
		wcharstring::escape(zin,lens[i],&output,&outputlen,L"\"");
		name.append("escape ");
		name.append(lenlabels[i]);
		test(name.getString(),!output && !outputlen);
		delete[] output;
		output=zin;
		outputlen=1;
		wcharstring::unescape(zin,lens[i],&output,&outputlen);
		name.clear();
		name.append("unescape ");
		name.append(lenlabels[i]);
		test(name.getString(),!output && !outputlen);
		delete[] output;

		oversized=wcharstring::pad(zin,L'*',-1,lens[i]);
		name.clear();
		name.append("pad left ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::pad(zin,L'*',0,lens[i]);
		name.clear();
		name.append("pad center ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::pad(zin,L'*',1,lens[i]);
		name.clear();
		name.append("pad right ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;

		oversized=wcharstring::duplicate(zin,(size_t)lens[i]);
		name.clear();
		name.append("duplicate wchar_t ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::duplicate(zcin,(size_t)lens[i]);
		name.clear();
		name.append("duplicate char ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::duplicateUcs2(zuin,(size_t)lens[i]);
		name.clear();
		name.append("duplicateUcs2 ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::duplicateUcs2(zuin,(size_t)lens[i],true);
		name.clear();
		name.append("duplicateUcs2 bigendian ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::duplicateUcs2(zuin,(size_t)lens[i],L'#');
		name.clear();
		name.append("duplicateUcs2 replacement ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
		oversized=wcharstring::duplicateUcs2(zuin,
					(size_t)lens[i],L'#',false);
		name.clear();
		name.append("duplicateUcs2 replacement bigendian ");
		name.append(lenlabels[i]);
		test(name.getString(),!oversized);
		delete[] oversized;
	}

	// getSubString() asks duplicate() for a length of (size_t)-1
	// when "start" is 1 past the end of an empty string
	wchar_t	*substr=wcharstring::getSubString(L"",1);
	test("getSubString past end",!substr);
	delete[] substr;

	// a NULL input is handled before the length is checked
	const wchar_t	*nullwstr=NULL;
	const char	*nullstr=NULL;
	const ucs2_t	*nullustr=NULL;
	wchar_t		*nulloutput=zin;
	uint64_t	nulloutputlen=1;
	wcharstring::escape(nullwstr,maxlen,&nulloutput,&nulloutputlen,L"\"");
	test("escape NULL, -1",!nulloutput && !nulloutputlen);
	nulloutput=zin;
	nulloutputlen=1;
	wcharstring::unescape(nullwstr,maxlen,&nulloutput,&nulloutputlen);
	test("unescape NULL, -1",!nulloutput && !nulloutputlen);
	test("pad NULL, -1",!wcharstring::pad(nullwstr,L'*',1,maxlen));
	test("duplicate NULL, -1 wchar_t",
			!wcharstring::duplicate(nullwstr,maxsize));
	test("duplicate NULL, -1 char",
			!wcharstring::duplicate(nullstr,maxsize));
	test("duplicateUcs2 NULL, -1",
			!wcharstring::duplicateUcs2(nullustr,maxsize));
	delete[] (char *)zin;
	delete[] (char *)zuin;
	delete[] zcin;

	// split sizes its list from the number of parts of a real string and
	// insertString sizes its result from the lengths of two real strings,
	// so a size that does not fit in a size_t needs a string larger than
	// the address space and cannot be tested on a 64-bit platform
	stdoutput.printf("\n");


	// a scratch buffer too large to size makes printf fail without
	// allocating
	stdoutput.printf("printf oversized length...\n");
	if (wcharstring::supportsPrintf()) {
		wchar_t	*nullbuffer=NULL;
		test("printf first failing",wcharstring::printf(nullbuffer,
					firstfail+1,L"%d",1)==-1);
		test("printf -1",wcharstring::printf(nullbuffer,
					maxsize,L"%d",1)==-1);
	} else {
		stdoutput.printf("not supported\n");
	}
	stdoutput.printf("\n");


	// pad and convertAmount count characters, not bytes
	stdoutput.printf("pad/convertAmount characters...\n");
	padTest("pad left",L"abc",L'*',-1,8,L"*****abc");
	padTest("pad right",L"abc",L'*',1,8,L"abc*****");
	padTest("pad centre",L"abc",L'*',0,8,L"**abc***");
	padTest("pad empty",L"",L'*',-1,4,L"****");
	padTest("pad NULL",NULL,L'*',1,3,L"***");
	wchar_t		padchars[]={
				L' ',
				(wchar_t)0x00E9,
				(wchar_t)0x263A,
				(wchar_t)((sizeof(wchar_t)>=4)?0x1F600:0x263B)
			};
	wchar_t		special[]={(wchar_t)0x00E9,L'x',(wchar_t)0x263A,0};
	const wchar_t	*padstrs[]={NULL,L"",L"a",L"abc",L"abcd",special};
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
	int64_t		amounts[]={0,5,99,100,101,12345,99999,1234567,
					987654321,-100,-12345,-1234567};
	for (size_t i=0; i<sizeof(amounts)/sizeof(amounts[0]); i++) {
		stringbuffer	name;
		name.append("convertAmount ");
		name.append(amounts[i]);
		test(name.getString(),amountMatches(amounts[i]));
	}
	wchar_t	*amount=wcharstring::convertAmount(12345,8);
	test("convertAmount padding",amount &&
		!wcharstring::compare(amount,L"$ 123.45"));
	delete[] amount;
	stdoutput.printf("\n");


	// convertToFloatC converts a string with a "." decimal point when the
	// locale uses another one
	stdoutput.printf("convertToFloatC...\n");
	if (locale::setNumeric("de_DE.utf8") &&
		wcharstring::convertToFloat(L"1,5")==1.5) {
		test("convertToFloatC 1.5",
			wcharstring::convertToFloatC(L"1.5")==1.5);
		test("convertToFloatC -0.5",
			wcharstring::convertToFloatC(L"-0.5")==-0.5);
		test("convertToFloatC 123.25",
			wcharstring::convertToFloatC(L"123.25")==123.25);
		test("convertToFloatC no decimal point",
			wcharstring::convertToFloatC(L"42")==42.0);

		// the longest string that is converted in place
		wchar_t	longstr[512];
		for (size_t i=0; i<252; i++) {
			longstr[i]=L'0';
		}
		longstr[252]=L'1';
		longstr[253]=L'.';
		longstr[254]=L'5';
		longstr[255]=0;
		test("convertToFloatC 255 characters",
			wcharstring::convertToFloatC(longstr)==1.5);

		// a string too long for the buffer must not write past it
		longstr[0]=L'1';
		for (size_t i=1; i<290; i++) {
			longstr[i]=L'0';
		}
		longstr[290]=0;
		long double	expected=wcharstring::convertToFloat(longstr);
		longstr[290]=L'.';
		for (size_t i=291; i<300; i++) {
			longstr[i]=L'5';
		}
		longstr[300]=0;
		test("convertToFloatC 300 characters",
			wcharstring::convertToFloatC(longstr)==expected);
	} else {
		stdoutput.printf("de_DE.utf8 locale not available, "
					"skipped\n");
	}
	locale::setNumeric("C");
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

	// a NULL array is an empty set, so the result is a copy of str; an
	// alarm turns a crash or hang into a failure
	signalmanager::alarm(60);
	nullSetTest("NULL array","abcabc","aXcaXc","acac");
	nullSetTest("NULL array, empty str","","","");
	nullSetTest("NULL array, no match","aaa","aaa","aaa");
	signalmanager::alarm(0);

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
	wcharstring::rightTrim((wchar_t *)NULL);
	wcharstring::rightTrim((wchar_t *)NULL,(wchar_t)'x');
	wchar_t	*empty=rangeMake("",1);
	wcharstring::rightTrim(empty);
	wcharstring::rightTrim(empty,(wchar_t)'x');
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
			!wcharstring::isInteger((const wchar_t *)NULL,-1) &&
			!wcharstring::isNumber((const wchar_t *)NULL,-1));
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
	wchar_t	*r=wcharstring::convertAmount((int64_t)-1,8);
	test("convertAmount -1, 8 spaces",emptyPatternMatches(r,"$  -0.01"));
	delete[] r;
	r=wcharstring::convertAmount(numberMin,8);
	test("convertAmount minimum, 8 spaces",
		emptyPatternMatches(r,"$-92233720368547758.08"));
	delete[] r;
	}
	test("getIntegerLength INT16_MIN",
		wcharstring::getIntegerLength((int16_t)(-32767-1))==6);
	test("getIntegerLength INT16_MIN+1",
		wcharstring::getIntegerLength((int16_t)-32767)==6);
	test("getIntegerLength INT16_MAX",
		wcharstring::getIntegerLength((int16_t)32767)==5);
	test("getIntegerLength int16 -1",
		wcharstring::getIntegerLength((int16_t)-1)==2);
	test("getIntegerLength int16 0",
		wcharstring::getIntegerLength((int16_t)0)==1);
	test("getIntegerLength INT32_MIN",
		wcharstring::getIntegerLength((int32_t)(-2147483647-1))==11);
	test("getIntegerLength INT32_MIN+1",
		wcharstring::getIntegerLength((int32_t)-2147483647)==11);
	test("getIntegerLength INT32_MAX",
		wcharstring::getIntegerLength((int32_t)2147483647)==10);
	test("getIntegerLength int32 -1",
		wcharstring::getIntegerLength((int32_t)-1)==2);
	test("getIntegerLength int32 0",
		wcharstring::getIntegerLength((int32_t)0)==1);
	test("getIntegerLength INT64_MIN",
		wcharstring::getIntegerLength(numberMin)==20);
	test("getIntegerLength INT64_MIN+1",
		wcharstring::getIntegerLength(numberMin+1)==20);
	test("getIntegerLength INT64_MAX",
		wcharstring::getIntegerLength(numberMax)==19);
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
	wchar_t	*r=wcharstring::parseNumber((int32_t)(-2147483647-1));
	test("parseNumber int32 INT32_MIN",
		emptyPatternMatches(r,"-2147483648"));
	delete[] r;
	r=wcharstring::parseNumber((int16_t)(-32767-1));
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

	return 0;
}
