// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/stdio.h>
#include <rudiments/signalclasses.h>
#include <limits.h>
#include "test.cpp"

// Accepts "limit" bytes in all, then fails each write, either with a short
// write or with RESULT_ERROR.  Counts the byte_t and char writes apart.
class limitedbuffer : public stringbuffer {
	public:
			limitedbuffer(size_t limit, bool error);
		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string, size_t length);
		size_t	bytewrites;
		size_t	charwrites;
	private:
		ssize_t	limitedWrite(const byte_t *data, size_t size);
		size_t	limit;
		bool	error;
};

limitedbuffer::limitedbuffer(size_t limit, bool error) : stringbuffer() {
	bytewrites=0;
	charwrites=0;
	this->limit=limit;
	this->error=error;
}

ssize_t limitedbuffer::write(const byte_t *string, size_t size) {
	bytewrites++;
	return limitedWrite(string,size);
}

ssize_t limitedbuffer::write(const char *string, size_t length) {
	charwrites++;
	return limitedWrite((const byte_t *)string,length);
}

ssize_t limitedbuffer::limitedWrite(const byte_t *data, size_t size) {
	if (getSize()+size<=limit) {
		return stringbuffer::write(data,size);
	}
	if (error) {
		return RESULT_ERROR;
	}
	return stringbuffer::write(data,limit-getSize());
}

// returns a copy of "input" with a NULL terminator after "size" bytes
static char *terminatedCopy(const void *input, uint64_t size) {
	if (!input) {
		return NULL;
	}
	char	*copy=new char[size+1];
	bytestring::copy(copy,input,size);
	copy[size]='\0';
	return copy;
}

// Returns a copy of "input" in a buffer of exactly "size" bytes, so that
// AddressSanitizer catches a read past "size".
static char *exactCopy(const void *input, uint64_t size) {
	if (!input) {
		return NULL;
	}
	char	*copy=new char[size];
	bytestring::copy(copy,input,size);
	return copy;
}

// Checks the result of one overload against "expected", the result of the
// input-and-size, out-pointer overload.  Results from the overloads that
// return a buffer carry no size, so their callers pass "expectedsize".
static void consistencyCheck(const char *label, const char *overload,
				const void *result, uint64_t resultsize,
				const void *expected, uint64_t expectedsize) {
	stringbuffer	name;
	name.append(label)->append(" ")->append(overload);
	test(name.getString(),
		(!result && !expected) ||
		(result && expected && resultsize==expectedsize &&
		!bytestring::compare(result,expected,expectedsize)));
}

// checks what an output * overload wrote to "sb" against "expected"
static void outputConsistencyCheck(const char *label, const char *overload,
				stringbuffer *sb,
				const void *expected, uint64_t expectedsize) {
	stringbuffer	name;
	name.append(label)->append(" ")->append(overload);
	test(name.getString(),
		sb->getSize()==expectedsize &&
		(!expectedsize ||
		!bytestring::compare(sb->getBuffer(),expected,expectedsize)));
}

typedef	char	*(*bytestochars1a_t)(const byte_t *);
typedef	char	*(*bytestochars1b_t)(const byte_t *, uint64_t);
typedef	void	(*bytestochars2a_t)(const byte_t *, char **, uint64_t *);
typedef	void	(*bytestochars2b_t)(const byte_t *, uint64_t,
							char **, uint64_t *);
typedef	void	(*bytestochars3a_t)(const byte_t *, output *);
typedef	void	(*bytestochars3b_t)(const byte_t *, uint64_t, output *);

// Runs "input" through the six overloads of a byte_t-to-char family and
// checks that they all agree with the input-and-size, out-pointer overload.
// The NULL-terminated overloads only run if "input" has no NULL in its first
// "inputsize" bytes.  The input-and-size overloads get an exact-size copy of
// "input".  The output * overloads write to a stringbuffer, so their sizes
// also prove that no NULL terminator was written.
static void bytesToCharsConsistencyTest(const char *label,
				bytestochars1a_t f1a, bytestochars1b_t f1b,
				bytestochars2a_t f2a, bytestochars2b_t f2b,
				bytestochars3a_t f3a, bytestochars3b_t f3b,
				const byte_t *input, uint64_t inputsize) {

	// make a NULL-terminated copy of the input
	byte_t	*in=(byte_t *)terminatedCopy(input,inputsize);
	bool	terminated=(!in ||
			charstring::getLength((char *)in)==inputsize);
	byte_t	*exact=(byte_t *)exactCopy(input,inputsize);

	// get the expected result
	char		*expected;
	uint64_t	expectedlen;
	f2b(exact,inputsize,&expected,&expectedlen);

	// buffer-returning overloads
	char	*result;
	if (terminated) {
		result=f1a(in);
		consistencyCheck(label,"1a",result,expectedlen,
						expected,expectedlen);
		delete[] result;
	}
	result=f1b(exact,inputsize);
	consistencyCheck(label,"1b",result,expectedlen,expected,expectedlen);
	delete[] result;

	// out-pointer overload
	if (terminated) {
		uint64_t	resultlen;
		f2a(in,&result,&resultlen);
		consistencyCheck(label,"2a",result,resultlen,
						expected,expectedlen);
		delete[] result;
	}

	// output * overloads, including a NULL output
	stringbuffer	sb;
	if (terminated) {
		f3a(in,&sb);
		outputConsistencyCheck(label,"3a",&sb,expected,expectedlen);
		f3a(in,NULL);
		sb.clear();
	}
	f3b(exact,inputsize,&sb);
	outputConsistencyCheck(label,"3b",&sb,expected,expectedlen);
	f3b(exact,inputsize,NULL);

	delete[] expected;
	delete[] exact;
	delete[] in;
}

typedef	byte_t	*(*charstobytes1a_t)(const char *);
typedef	byte_t	*(*charstobytes1b_t)(const char *, uint64_t);
typedef	void	(*charstobytes2a_t)(const char *, byte_t **, uint64_t *);
typedef	void	(*charstobytes2b_t)(const char *, uint64_t,
							byte_t **, uint64_t *);
typedef	void	(*charstobytes3a_t)(const char *, output *);
typedef	void	(*charstobytes3b_t)(const char *, uint64_t, output *);

// Similar to bytesToCharsConsistencyTest(), for a char-to-byte_t family.
// base64Decode() is the one exception to all six overloads agreeing: its
// out-pointer overloads report a size that counts the NULL terminator, unless
// the size is 0, and its output * overloads write one byte less.  Its callers
// set "sizecountsnull".
static void charsToBytesConsistencyTest(const char *label,
				charstobytes1a_t f1a, charstobytes1b_t f1b,
				charstobytes2a_t f2a, charstobytes2b_t f2b,
				charstobytes3a_t f3a, charstobytes3b_t f3b,
				const char *input, uint64_t inputsize,
				bool sizecountsnull) {

	// make a NULL-terminated copy of the input
	char	*in=terminatedCopy(input,inputsize);
	bool	terminated=(!in || charstring::getLength(in)==inputsize);
	char	*exact=exactCopy(input,inputsize);

	// get the expected result
	byte_t		*expected;
	uint64_t	expectedsize;
	f2b(exact,inputsize,&expected,&expectedsize);
	uint64_t	writtensize=(sizecountsnull && expectedsize)?
						expectedsize-1:expectedsize;

	// buffer-returning overloads
	byte_t	*result;
	if (terminated) {
		result=f1a(in);
		consistencyCheck(label,"1a",result,expectedsize,
						expected,expectedsize);
		delete[] result;
	}
	result=f1b(exact,inputsize);
	consistencyCheck(label,"1b",result,expectedsize,
					expected,expectedsize);
	delete[] result;

	// out-pointer overload
	if (terminated) {
		uint64_t	resultsize;
		f2a(in,&result,&resultsize);
		consistencyCheck(label,"2a",result,resultsize,
						expected,expectedsize);
		delete[] result;
	}

	// output * overloads, including a NULL output
	stringbuffer	sb;
	if (terminated) {
		f3a(in,&sb);
		outputConsistencyCheck(label,"3a",&sb,expected,writtensize);
		f3a(in,NULL);
		sb.clear();
	}
	f3b(exact,inputsize,&sb);
	outputConsistencyCheck(label,"3b",&sb,expected,writtensize);
	f3b(exact,inputsize,NULL);

	delete[] expected;
	delete[] exact;
	delete[] in;
}

typedef	char	*(*charstochars1a_t)(const char *);
typedef	char	*(*charstochars1b_t)(const char *, uint64_t);
typedef	void	(*charstochars2a_t)(const char *, char **, uint64_t *);
typedef	void	(*charstochars2b_t)(const char *, uint64_t,
							char **, uint64_t *);
typedef	void	(*charstochars3a_t)(const char *, output *);
typedef	void	(*charstochars3b_t)(const char *, uint64_t, output *);

// similar to bytesToCharsConsistencyTest(), for a char-to-char family
static void charsToCharsConsistencyTest(const char *label,
				charstochars1a_t f1a, charstochars1b_t f1b,
				charstochars2a_t f2a, charstochars2b_t f2b,
				charstochars3a_t f3a, charstochars3b_t f3b,
				const char *input, uint64_t inputsize) {

	// make a NULL-terminated copy of the input
	char	*in=terminatedCopy(input,inputsize);
	bool	terminated=(!in || charstring::getLength(in)==inputsize);
	char	*exact=exactCopy(input,inputsize);

	// get the expected result
	char		*expected;
	uint64_t	expectedlen;
	f2b(exact,inputsize,&expected,&expectedlen);

	// buffer-returning overloads
	char	*result;
	if (terminated) {
		result=f1a(in);
		consistencyCheck(label,"1a",result,expectedlen,
						expected,expectedlen);
		delete[] result;
	}
	result=f1b(exact,inputsize);
	consistencyCheck(label,"1b",result,expectedlen,expected,expectedlen);
	delete[] result;

	// out-pointer overload
	if (terminated) {
		uint64_t	resultlen;
		f2a(in,&result,&resultlen);
		consistencyCheck(label,"2a",result,resultlen,
						expected,expectedlen);
		delete[] result;
	}

	// output * overloads, including a NULL output
	stringbuffer	sb;
	if (terminated) {
		f3a(in,&sb);
		outputConsistencyCheck(label,"3a",&sb,expected,expectedlen);
		f3a(in,NULL);
		sb.clear();
	}
	f3b(exact,inputsize,&sb);
	outputConsistencyCheck(label,"3b",&sb,expected,expectedlen);
	f3b(exact,inputsize,NULL);

	delete[] expected;
	delete[] exact;
	delete[] in;
}

typedef	char	*(*charstocharswithchars1a_t)(const char *,
							const char *);
typedef	char	*(*charstocharswithchars1b_t)(const char *, uint64_t,
							const char *);
typedef	void	(*charstocharswithchars2a_t)(const char *,
							char **, uint64_t *,
							const char *);
typedef	void	(*charstocharswithchars2b_t)(const char *, uint64_t,
							char **, uint64_t *,
							const char *);
typedef	void	(*charstocharswithchars3a_t)(const char *, output *,
							const char *);
typedef	void	(*charstocharswithchars3b_t)(const char *, uint64_t,
							output *,
							const char *);

// Similar to charsToCharsConsistencyTest(), for a char-to-char family whose
// overloads all take a trailing "characters" argument, like escape().
static void charsToCharsWithCharactersConsistencyTest(const char *label,
			charstocharswithchars1a_t f1a,
			charstocharswithchars1b_t f1b,
			charstocharswithchars2a_t f2a,
			charstocharswithchars2b_t f2b,
			charstocharswithchars3a_t f3a,
			charstocharswithchars3b_t f3b,
			const char *input, uint64_t inputsize,
			const char *characters) {

	// make a NULL-terminated copy of the input
	char	*in=terminatedCopy(input,inputsize);
	bool	terminated=(!in || charstring::getLength(in)==inputsize);
	char	*exact=exactCopy(input,inputsize);

	// get the expected result
	char		*expected;
	uint64_t	expectedlen;
	f2b(exact,inputsize,&expected,&expectedlen,characters);

	// buffer-returning overloads
	char	*result;
	if (terminated) {
		result=f1a(in,characters);
		consistencyCheck(label,"1a",result,expectedlen,
						expected,expectedlen);
		delete[] result;
	}
	result=f1b(exact,inputsize,characters);
	consistencyCheck(label,"1b",result,expectedlen,expected,expectedlen);
	delete[] result;

	// out-pointer overload
	if (terminated) {
		uint64_t	resultlen;
		f2a(in,&result,&resultlen,characters);
		consistencyCheck(label,"2a",result,resultlen,
						expected,expectedlen);
		delete[] result;
	}

	// output * overloads, including a NULL output
	stringbuffer	sb;
	if (terminated) {
		f3a(in,&sb,characters);
		outputConsistencyCheck(label,"3a",&sb,expected,expectedlen);
		f3a(in,NULL,characters);
		sb.clear();
	}
	f3b(exact,inputsize,&sb,characters);
	outputConsistencyCheck(label,"3b",&sb,expected,expectedlen);
	f3b(exact,inputsize,NULL,characters);

	delete[] expected;
	delete[] exact;
	delete[] in;
}

static void hexEncodeTest(const char *label,
				const byte_t *input, uint64_t inputsize,
				const char *expected) {
	char		*result;
	uint64_t	resultlen;
	charstring::hexEncode(input,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==charstring::getLength(expected) &&
			!charstring::compare(result,expected)));
	delete[] result;
	bytesToCharsConsistencyTest(label,
			charstring::hexEncode,charstring::hexEncode,
			charstring::hexEncode,charstring::hexEncode,
			charstring::hexEncode,charstring::hexEncode,
			input,inputsize);
}

static void hexDecodeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedsize) {
	byte_t		*result;
	uint64_t	resultsize;
	charstring::hexDecode(input,inputsize,&result,&resultsize);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultsize==expectedsize &&
			!bytestring::compare(result,expected,expectedsize)));
	delete[] result;
	charsToBytesConsistencyTest(label,
			charstring::hexDecode,charstring::hexDecode,
			charstring::hexDecode,charstring::hexDecode,
			charstring::hexDecode,charstring::hexDecode,
			input,inputsize,false);
}

static void base64EncodeTest(const char *label,
				const byte_t *input, uint64_t inputsize,
				const char *expected) {
	byte_t		*in=(byte_t *)exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::base64Encode(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==charstring::getLength(expected) &&
			!charstring::compare(result,expected)));
	delete[] result;
	delete[] in;
	bytesToCharsConsistencyTest(label,
			charstring::base64Encode,charstring::base64Encode,
			charstring::base64Encode,charstring::base64Encode,
			charstring::base64Encode,charstring::base64Encode,
			input,inputsize);
}

// "expectedsize" is the size that the out-pointer overloads report, which
// counts the NULL terminator, so "expected" includes the terminator
static void base64DecodeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedsize) {
	char		*in=exactCopy(input,inputsize);
	byte_t		*result;
	uint64_t	resultsize;
	charstring::base64Decode(in,inputsize,&result,&resultsize);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultsize==expectedsize &&
			!bytestring::compare(result,expected,expectedsize)));
	delete[] result;
	delete[] in;
	charsToBytesConsistencyTest(label,
			charstring::base64Decode,charstring::base64Decode,
			charstring::base64Decode,charstring::base64Decode,
			charstring::base64Decode,charstring::base64Decode,
			input,inputsize,true);
}

// appends "count" copies of "str" to "sb"
static void appendRepeated(stringbuffer *sb, const char *str, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		sb->append(str);
	}
}

static void quotedPrintableEncodeTest(const char *label,
				const byte_t *input, uint64_t inputsize,
				const char *expected) {
	byte_t		*in=(byte_t *)exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::quotedPrintableEncode(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==charstring::getLength(expected) &&
			!charstring::compare(result,expected)));
	delete[] result;
	delete[] in;
	bytesToCharsConsistencyTest(label,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		input,inputsize);
}

static void quotedPrintableDecodeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedsize) {
	char		*in=exactCopy(input,inputsize);
	byte_t		*result;
	uint64_t	resultsize;
	charstring::quotedPrintableDecode(in,inputsize,&result,&resultsize);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultsize==expectedsize &&
			!bytestring::compare(result,expected,expectedsize)));
	delete[] result;
	delete[] in;
	charsToBytesConsistencyTest(label,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		input,inputsize,false);
}

static void escapeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *characters,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::escape(in,inputsize,&result,&resultlen,characters);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1)));
	delete[] result;
	delete[] in;
	charsToCharsWithCharactersConsistencyTest(label,
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			input,inputsize,characters);
}

static void unescapeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::unescape(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1)));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			input,inputsize);
}

static void urlEncodeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::urlEncode(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1)));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			input,inputsize);
}

static void urlDecodeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::urlDecode(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected) ||
			(result && expected &&
			resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1)));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			input,inputsize);
}

static void htmlEncodeTest(const char *label,
				const char *input, const char *expected) {
	char	*output=charstring::htmlEncode(input);
	test(label,!charstring::compare(output,expected));
	delete[] output;
	charsToCharsConsistencyTest(label,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			input,charstring::getLength(input));
}

static void htmlDecodeTest(const char *label,
				const char *input, const char *expected) {
	char	*output=charstring::htmlDecode(input);
	test(label,!charstring::compare(output,expected) &&
			charstring::getLength(output)<=
			charstring::getLength(input));
	delete[] output;
	charsToCharsConsistencyTest(label,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			input,charstring::getLength(input));
}

static void htmlRoundTripTest(const char *label, const char *input) {
	char	*encoded=charstring::htmlEncode(input);
	char	*decoded=charstring::htmlDecode(encoded);
	test(label,!charstring::compare(decoded,input));
	delete[] encoded;
	delete[] decoded;
}

static void htmlEncodeSizeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::htmlEncode(in,inputsize,&result,&resultlen);
	test(label,result && resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			input,inputsize);
}

static void htmlDecodeSizeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::htmlDecode(in,inputsize,&result,&resultlen);
	test(label,result && resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			input,inputsize);
}

static void jsonEncodeTest(const char *label,
				const char *input, const char *expected) {
	char	*output=charstring::jsonEncode(input);
	test(label,!charstring::compare(output,expected));
	delete[] output;
	charsToCharsConsistencyTest(label,
			charstring::jsonEncode,charstring::jsonEncode,
			charstring::jsonEncode,charstring::jsonEncode,
			charstring::jsonEncode,charstring::jsonEncode,
			input,charstring::getLength(input));
}

static void jsonEncodeSizeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::jsonEncode(in,inputsize,&result,&resultlen);
	test(label,result && resultlen==expectedlen &&
			!bytestring::compare(result,expected,expectedlen+1));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::jsonEncode,charstring::jsonEncode,
			charstring::jsonEncode,charstring::jsonEncode,
			charstring::jsonEncode,charstring::jsonEncode,
			input,inputsize);
}

// "expected" NULL means the NULL result, which has an "expectedlen" of 0
static void jsonDecodeSizeTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *expected, uint64_t expectedlen) {
	char		*in=exactCopy(input,inputsize);
	char		*result;
	uint64_t	resultlen;
	charstring::jsonDecode(in,inputsize,&result,&resultlen);
	test(label,(!result && !expected && !resultlen) ||
			(result && expected &&
			resultlen==expectedlen && resultlen<=inputsize &&
			!bytestring::compare(result,expected,expectedlen) &&
			!result[resultlen]));
	delete[] result;
	delete[] in;
	charsToCharsConsistencyTest(label,
			charstring::jsonDecode,charstring::jsonDecode,
			charstring::jsonDecode,charstring::jsonDecode,
			charstring::jsonDecode,charstring::jsonDecode,
			input,inputsize);
}

// also checks the NULL-terminated overload as a C string, which ends at the
// first decoded NULL
static void jsonDecodeTest(const char *label,
				const char *input,
				const char *expected, uint64_t expectedlen) {
	char	*output=charstring::jsonDecode(input);
	test(label,(!output && !expected) ||
			(output && expected &&
			!charstring::compare(output,expected)));
	delete[] output;
	jsonDecodeSizeTest(label,input,charstring::getLength(input),
						expected,expectedlen);
}

// encodes "input" to "encoded" and decodes "encoded" back to "input"
static void jsonRoundTripTest(const char *label,
				const char *input, uint64_t inputsize,
				const char *encoded, uint64_t encodedlen) {
	jsonEncodeSizeTest(label,input,inputsize,encoded,encodedlen);
	jsonDecodeSizeTest(label,encoded,encodedlen,input,inputsize);
}

// appends the JSON encoding of "input" to "sb", written independently of
// charstring::jsonEncode()
static void jsonReference(stringbuffer *sb,
				const char *input, uint64_t inputsize) {
	const char	*digits="0123456789abcdef";
	sb->append('"');
	for (uint64_t i=0; i<inputsize; i++) {
		byte_t	b=(byte_t)input[i];
		if (b=='"') {
			sb->append("\\\"");
		} else if (b=='\\') {
			sb->append("\\\\");
		} else if (b<0x20) {
			sb->append("\\u00");
			sb->append(digits[b/16]);
			sb->append(digits[b%16]);
		} else {
			sb->append((char)b);
		}
	}
	sb->append('"');
}

// Calls the input-and-size overloads of a char-to-char encoder with a
// zero-byte buffer and a length too large for the result to fit in a size_t.
// Both must fail without reading the input (AddressSanitizer reports any read)
// and without allocating: the allocating form returns NULL and the
// out-pointer form sets "output" to NULL and "outputlen" to 0.  This relies on
// the size check coming before any read of the input.
static void oversizedCharsTest(const char *label,
				charstochars1b_t f1b, charstochars2b_t f2b,
				uint64_t inputlen) {
	size_t		none=0;
	char		*in=new char[none];
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	char	*result=f1b(in,inputlen);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	char		*output=in;
	uint64_t	outputlen=1;
	f2b(in,inputlen,&output,&outputlen);
	test(name.getString(),!output && !outputlen);
	delete[] output;
	delete[] in;
}

// similar to oversizedCharsTest(), for a byte_t-to-char encoder
static void oversizedBytesTest(const char *label,
				bytestochars1b_t f1b, bytestochars2b_t f2b,
				uint64_t inputlen) {
	size_t		none=0;
	byte_t		*in=new byte_t[none];
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	char	*result=f1b(in,inputlen);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	char		*output=(char *)in;
	uint64_t	outputlen=1;
	f2b(in,inputlen,&output,&outputlen);
	test(name.getString(),!output && !outputlen);
	delete[] output;
	delete[] in;
}

// similar to oversizedCharsTest(), for escape(), which takes "characters"
static void oversizedEscapeTest(const char *label, uint64_t inputlen,
					const char *characters) {
	size_t		none=0;
	char		*in=new char[none];
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	char	*result=charstring::escape(in,inputlen,characters);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	char		*output=in;
	uint64_t	outputlen=1;
	charstring::escape(in,inputlen,&output,&outputlen,characters);
	test(name.getString(),!output && !outputlen);
	delete[] output;
	delete[] in;
}

// checks pad(), where "expected" is NULL if pad() must return NULL
static void padTest(const char *label, const char *str, char padchar,
			int16_t direction, uint64_t totallen,
			const char *expected) {
	char	*result=charstring::pad(str,padchar,direction,totallen);
	test(label,(expected)?
			(result && !charstring::compare(result,expected)):
			!result);
	delete[] result;
}

// checks insertString()
static void insertStringTest(const char *label, const char *dest,
				const char *src, uint64_t index,
				const char *expected) {
	char	*result=charstring::insertString(dest,src,index);
	test(label,result && !charstring::compare(result,expected));
	delete[] result;
}

// checks insertString() on exact-size heap copies of the inputs, so that
// AddressSanitizer sees any read outside them; a NULL stays NULL
static void insertStringExactTest(const char *label, const char *dest,
					const char *src, uint64_t index,
					const char *expected) {
	size_t	destlen=charstring::getLength(dest);
	size_t	srclen=charstring::getLength(src);
	char	*d=NULL;
	if (dest) {
		d=new char[destlen+1];
		bytestring::copy(d,dest,destlen+1);
	}
	char	*s=NULL;
	if (src) {
		s=new char[srclen+1];
		bytestring::copy(s,src,srclen+1);
	}
	char	*result=charstring::insertString(d,s,index);
	test(label,result && !charstring::compare(result,expected));
	delete[] result;
	delete[] d;
	delete[] s;
}

// returns an exact-size copy of the ASCII string "str"; NULL stays NULL
static char *emptyPatternCopy(const char *str) {
	return (str)?charstring::duplicate(str):NULL;
}

// true if "result" holds the ASCII string "expected", or both are NULL
static bool emptyPatternMatches(const char *result, const char *expected) {
	if (!result || !expected) {
		return (!result && !expected);
	}
	char	*e=emptyPatternCopy(expected);
	bool	match=!charstring::compare(result,e);
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
	const char	*s=emptyPatternCopy(str);
	const char	*o=emptyPatternCopy(oldstr);
	const char	*n=emptyPatternCopy(newstr);
	stringbuffer	name;
	name.append(label)->append(" replace");
	char	*result=charstring::replace(s,o,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase");
	result=charstring::replaceIgnoringCase(s,o,n);
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
	char		*s=emptyPatternCopy(str);
	const char	**o=new const char *[count+1];
	const char	**n=new const char *[count+1];
	size_t		*l=new size_t[count+1];
	for (size_t i=0; i<count; i++) {
		o[i]=emptyPatternCopy(oldset[i]);
		n[i]=emptyPatternCopy(newset[i]);
		l[i]=(lengths)?lengths[i]:charstring::getLength(o[i]);
	}
	o[count]=NULL;
	n[count]=NULL;
	stringbuffer	name;
	char	*result=NULL;
	if (!lengths) {
		name.append(label)->append(" replace");
		result=charstring::replace(s,o,n);
		test(name.getString(),emptyPatternMatches(result,expected));
		delete[] result;
		name.clear();
		name.append(label)->append(" replaceIgnoringCase");
		result=charstring::replaceIgnoringCase(s,o,n);
		test(name.getString(),emptyPatternMatches(result,expectedic));
		delete[] result;
	}
	name.clear();
	name.append(label)->append(" replace length");
	result=charstring::replace(s,o,l,n);
	test(name.getString(),emptyPatternMatches(result,expected));
	delete[] result;
	name.clear();
	name.append(label)->append(" replaceIgnoringCase length");
	result=charstring::replaceIgnoringCase(s,o,l,n);
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
static void nullSetCheck(const char *label, const char *str, char *result,
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
	char		*s=emptyPatternCopy(str);
	const char	**o=new const char *[2];
	const char	**n=new const char *[2];
	const char	**d=new const char *[2];
	size_t		*l=new size_t[1];
	o[0]=emptyPatternCopy("b");
	o[1]=NULL;
	n[0]=emptyPatternCopy("X");
	n[1]=NULL;
	d[0]=NULL;
	d[1]=NULL;
	l[0]=1;
	const char * const	*nullset=NULL;
	size_t			*nulllen=NULL;
	stringbuffer		name;
	for (int ic=0; ic<2; ic++) {
		const char	*fn=(ic)?" replaceIgnoringCase":" replace";
		#define NULLSETNAME(what) \
			name.clear(); \
			name.append(label)->append(fn)->append(what)
		#define NULLSETCALL(args) \
			((ic)?charstring::replaceIgnoringCase args: \
				charstring::replace args)
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
	char	*s=emptyPatternCopy(str1);
	const char	*p=emptyPatternCopy(str2);
	bool	retval=charstring::strip(s,p);
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
	char	*h=emptyPatternCopy(hay);
	char	*n=emptyPatternCopy(needle);
	const char	*ch=h;
	const char	*r;
	stringbuffer	name;
	name.append(label)->append(" findLast");
	r=charstring::findLast(ch,(const char *)n);
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=charstring::findLast(h,(const char *)n);
	name.clear();
	name.append(label)->append(" findLast non-const");
	test(name.getString(),(r)?(r-h==exp):(exp==-1));
	r=charstring::findLastIgnoringCase(ch,(const char *)n);
	name.clear();
	name.append(label)->append(" findLastIgnoringCase");
	test(name.getString(),(r)?(r-h==expic):(expic==-1));
	r=charstring::findLastIgnoringCase(h,(const char *)n);
	name.clear();
	name.append(label)->append(" findLastIgnoringCase non-const");
	test(name.getString(),(r)?(r-h==expic):(expic==-1));
	if (n[0] && !n[1]) {
		r=charstring::findLast(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLast char");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
		r=charstring::findLastIgnoringCase(ch,n[0]);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase char");
		test(name.getString(),(r)?(r-h==expic):(expic==-1));
		r=charstring::findLast(h,n[0]);
		name.clear();
		name.append(label)->append(" findLast char non-const");
		test(name.getString(),(r)?(r-h==exp):(exp==-1));
		r=charstring::findLastIgnoringCase(h,n[0]);
		name.clear();
		name.append(label);
		name.append(" findLastIgnoringCase char non-const");
		test(name.getString(),(r)?(r-h==expic):(expic==-1));
	}
	if (!n[0]) {
		r=charstring::findLast(ch,(char)0);
		name.clear();
		name.append(label)->append(" findLast terminator");
		test(name.getString(),r==h+charstring::getLength(h));
		r=charstring::findLastIgnoringCase(ch,(char)0);
		name.clear();
		name.append(label)->append(" findLastIgnoringCase terminator");
		test(name.getString(),r==h+charstring::getLength(h));
	}
	delete[] h;
	delete[] n;
}

// checks the NULL arguments of findLast and findLastIgnoringCase
static void findLastNullTest() {
	char	*h=emptyPatternCopy("abc");
	const char	*ch=h;
	test("findLast NULL haystack",
		charstring::findLast((const char *)NULL,ch)==NULL);
	test("findLast NULL needle",
		charstring::findLast(ch,(const char *)NULL)==NULL);
	test("findLastIgnoringCase NULL haystack",
		charstring::findLastIgnoringCase((const char *)NULL,ch)==NULL);
	test("findLastIgnoringCase NULL needle",
		charstring::findLastIgnoringCase(ch,(const char *)NULL)==NULL);
	test("findLast char NULL haystack",
		charstring::findLast((const char *)NULL,(char)'a')==NULL);
	test("findLast char 0, NULL haystack",
		charstring::findLast((const char *)NULL,(char)0)==NULL);
	test("findLastIgnoringCase char NULL haystack",
		charstring::findLastIgnoringCase((const char *)NULL,
							(char)'a')==NULL);
	test("findLastIgnoringCase char 0, NULL haystack",
		charstring::findLastIgnoringCase((const char *)NULL,
							(char)0)==NULL);
	delete[] h;
}

// checks leftTrim, strip and stripSet on exact-size heap copies; which is
// 0 leftTrim(str), 1 leftTrim(str,ch), 2 strip(str,ch), 3 strip(str1,str2),
// 4 stripSet(str,set); ch is x, str2 is "xy" and set is "xy"
static void sizeIndexTest(const char *label, int which, const char *str,
				const char *expected, bool expectedreturn) {
	char	*s=emptyPatternCopy(str);
	char	*xy=emptyPatternCopy("xy");
	bool	retval=expectedreturn;
	switch (which) {
		case 0:
			charstring::leftTrim(s);
			break;
		case 1:
			charstring::leftTrim(s,'x');
			break;
		case 2:
			retval=charstring::strip(s,'x');
			break;
		case 3:
			retval=charstring::strip(s,(const char *)xy);
			break;
		default:
			retval=charstring::stripSet(s,(const char *)xy);
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
	char	*expected=charstring::duplicate(str,len);
	stringbuffer	name;
	name.append(label);
	name.append(" char");
	test(name.getString(),expected &&
		charstring::getLength(expected)==len &&
		!bytestring::compare(expected,str,len));
	char	*result=charstring::duplicate(wstr,len);
	name.clear();
	name.append(label);
	name.append(" wchar_t");
	test(name.getString(),result &&
		charstring::getLength(result)==len &&
		!bytestring::compare(result,str,len));
	delete[] result;
	result=charstring::duplicate(wstr,len,'#');
	name.clear();
	name.append(label);
	name.append(" wchar_t replacement");
	test(name.getString(),result &&
		charstring::getLength(result)==len &&
		!bytestring::compare(result,str,len));
	delete[] result;
	result=charstring::duplicateUcs2(ustr,len);
	name.clear();
	name.append(label);
	name.append(" ucs2");
	test(name.getString(),result &&
		charstring::getLength(result)==len &&
		!bytestring::compare(result,str,len));
	delete[] result;
	delete[] expected;
}

// similar to oversizedCharsTest(), for a char-to-byte_t decoder
static void oversizedDecodeTest(const char *label,
				charstobytes1b_t f1b, charstobytes2b_t f2b,
				uint64_t inputlen) {
	size_t		none=0;
	char		*in=new char[none];
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	byte_t	*result=f1b(in,inputlen);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	byte_t		*output=(byte_t *)in;
	uint64_t	outputlen=1;
	f2b(in,inputlen,&output,&outputlen);
	test(name.getString(),!output && !outputlen);
	delete[] output;
	delete[] in;
}

// Calls the input-and-size overloads of a char-to-char decoder with a NULL
// input and a huge length.  Both must give NULL and 0 without reading.
static void nullCharsTest(const char *label,
				charstochars1b_t f1b, charstochars2b_t f2b,
				uint64_t inputlen) {
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	char	*result=f1b(NULL,inputlen);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	char		*output=new char[1];
	uint64_t	outputlen=1;
	f2b(NULL,inputlen,&output,&outputlen);
	test(name.getString(),!output && !outputlen);
	delete[] output;
}

// similar to nullCharsTest(), for a char-to-byte_t decoder
static void nullDecodeTest(const char *label,
				charstobytes1b_t f1b, charstobytes2b_t f2b,
				uint64_t inputlen) {
	stringbuffer	name;
	name.append(label);
	name.append(" 1b");
	byte_t	*result=f1b(NULL,inputlen);
	test(name.getString(),!result);
	delete[] result;
	name.clear();
	name.append(label);
	name.append(" 2b");
	byte_t		*output=new byte_t[1];
	uint64_t	outputlen=1;
	f2b(NULL,inputlen,&output,&outputlen);
	test(name.getString(),!output && !outputlen);
	delete[] output;
}

// returns a heap array of exactly "n" elements that holds the first "n"
// ASCII characters of "src"; there is no terminator unless "src" has one
static char *rangeMake(const char *src, size_t n) {
	char	*buf=new char[n];
	for (size_t i=0; i<n; i++) {
		buf[i]=(char)(unsigned char)src[i];
	}
	return buf;
}

// true if the first "n" elements of "buf" are the ASCII characters of "exp"
static bool rangeEqual(const char *buf, const char *exp, size_t n) {
	for (size_t i=0; i<n; i++) {
		if (buf[i]!=(char)(unsigned char)exp[i]) {
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
			char	*buf=rangeMake(str,n+1);
			if (ch) {
				charstring::rightTrim(buf,(char)ch);
			} else {
				charstring::rightTrim(buf);
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
	char	buf[1+8+5+1];
	buf[0]=(char)'X';
	for (size_t i=1; i<1+8+n; i++) {
		buf[i]=(char)fill;
	}
	buf[1+8+n]=(char)0;
	if (usech) {
		charstring::rightTrim(buf+1+8,(char)fill);
	} else {
		charstring::rightTrim(buf+1+8);
	}
	bool	ok=(buf[0]==(char)'X' && !buf[1+8]);
	for (size_t i=1; i<1+8; i++) {
		ok=(ok && buf[i]==(char)fill);
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
			char	*buf=rangeMake(str,n);
			if (which==0) {
				charstring::leftJustify(buf,(int32_t)n);
			} else if (which==1) {
				charstring::rightJustify(buf,(int32_t)n);
			} else {
				charstring::center(buf,(int32_t)n);
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
		char	*buf=rangeMake(" a ",n);
		charstring::leftJustify(buf,len);
		charstring::rightJustify(buf,len);
		charstring::center(buf,len);
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
		char	*buf=rangeMake(str,n);
		charstring::center(buf,(int32_t)n);
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
			charstring::center(buf,(int32_t)n);
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
	char	buf[4];
	buf[0]=(char)'x';
	buf[1]=(char)'1';
	buf[2]=(char)'2';
	buf[3]=(char)'3';
	bool	ok=(!charstring::isInteger(buf+1,len) &&
			!charstring::isNumber(buf+1,len));
	char	*exact=rangeMake("123",3);
	ok=(ok && !charstring::isInteger(exact,len) &&
			!charstring::isNumber(exact,len));
	delete[] exact;
	test(label,ok);
}

// checks isInteger() and isNumber() with a length, in an exact-size heap
// buffer that has no terminator
static void rangeNumberTest(const char *str, size_t n, bool isinteger,
							bool isnumber) {
	char	*buf=rangeMake(str,n);
	bool	ok=(charstring::isInteger(buf,(int32_t)n)==isinteger &&
			charstring::isNumber(buf,(int32_t)n)==isnumber);
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
	char	*r=charstring::convertAmount(c->amount);
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
		r=charstring::convertAmount(c->amount,spaces[i]);
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
	char	*r=charstring::parseNumber(number,zeropadding);
	name.append("parseNumber ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),emptyPatternMatches(r,expected));
	delete[] r;
	if (zeropadding==1) {
		r=charstring::parseNumber(number);
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
	stringbuffer	sb;
	if (zeropadding==1) {
		sb.append(number);
	} else {
		sb.append(number,zeropadding);
	}
	char	*e=emptyPatternCopy(expected);
	name.append("append ")->append(label);
	name.append(" padding ")->append((uint64_t)zeropadding);
	test(name.getString(),!charstring::compare(sb.getString(),e));
	delete[] e;
}

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


	// NULL comparisons
	stdoutput.printf("NULL comparisons...\n");
	test("compare(NULL,NULL)",!charstring::compare(NULL,NULL));
	test("compare(NULL,a)",charstring::compare(NULL,"a")<0);
	test("compare(a,NULL)",charstring::compare("a",NULL)>0);
	test("compareIgnoringCase(NULL,NULL)",
			!charstring::compareIgnoringCase(NULL,NULL));
	test("compareIgnoringCase(NULL,a)",
			charstring::compareIgnoringCase(NULL,"a")<0);
	test("compareIgnoringCase(a,NULL)",
			charstring::compareIgnoringCase("a",NULL)>0);
	test("compareNatural(NULL,NULL)",
			!charstring::compareNatural(NULL,NULL));
	test("compareNatural(NULL,a)",
			charstring::compareNatural(NULL,"a")<0);
	test("compareNatural(a,NULL)",
			charstring::compareNatural("a",NULL)>0);
	test("compareVersions(NULL,NULL)",
			!charstring::compareVersions(NULL,NULL));
	test("compareVersions(NULL,a)",
			charstring::compareVersions(NULL,"a")<0);
	test("compareVersions(a,NULL)",
			charstring::compareVersions("a",NULL)>0);
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
						s+charstring::getLength(s));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,"LLO"));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,"BLAH")==
						s+charstring::getLength(s));
	test("findFirst",charstring::findFirst(s,'l'));
	test("findFirstIgnoringCase",charstring::findFirstIgnoringCase(s,'L'));
	test("findFirstOrEnd",
		charstring::findFirstOrEnd(s,'z')==s+charstring::getLength(s));
	test("findFirstOrEnd",charstring::findFirstOrEnd(s,'l'));
	test("findFirstOrEndIgnoringCase",
		charstring::findFirstOrEndIgnoringCase(s,'Z')==
						s+charstring::getLength(s));
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
	char	*ell=charstring::getSubString(hellothere,1,3);
	test("subString",!charstring::compare(ell,"ell"));
	char	*llo=charstring::getSubString(hellothere,2,4);
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

	// isBefore, isBetween, isAfter
	stdoutput.printf("isBefore/isBetween/isAfter...\n");
	char	*bef=charstring::isBefore("one two three four five"," two");
	test("isBefore",!charstring::compare(bef,"one"));
	char	*btw=charstring::isBetween("one two three four five",
							"two "," four");
	test("isBetween",!charstring::compare(btw,"three"));
	char	*aft=charstring::isAfter("one two three four five","four ");
	test("isAfter",!charstring::compare(aft,"five"));
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

	// input larger than a 16-bit loop counter can index, to guard against
	// the ii<inputsize overflow fixed in #10043
	uint64_t	bigsize=70000;
	byte_t		*bigbytes=new byte_t[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigbytes[bi]=(byte_t)(bi%256);
	}
	charstring::hexEncode(bigbytes,bigsize,&encodedbytes,&encodedlen);
	charstring::hexDecode(encodedbytes,encodedlen,
					&decodedbytes,&decodedlen);
	test("hex test 11",
		encodedlen==bigsize*2 &&
		decodedlen==bigsize &&
		!bytestring::compare(decodedbytes,bigbytes,bigsize));
	delete[] bigbytes;
	delete[] encodedbytes;
	delete[] decodedbytes;
	stdoutput.printf("\n");


	// hex encode
	stdoutput.printf("hex-encode...\n");
	uint64_t	zero=0;
	hexEncodeTest("NULL",NULL,zero,NULL);
	hexEncodeTest("NULL, size 3",NULL,3,NULL);
	hexEncodeTest("empty",(const byte_t *)"",zero,"");
	hexEncodeTest("abc, size 0",(const byte_t *)"abc",zero,"");
	hexEncodeTest("A",(const byte_t *)"A",1,"41");
	hexEncodeTest("\\x01\\xab\\xff",(const byte_t *)"\x01\xab\xff",3,
								"01abff");
	hexEncodeTest("a\\0b",(const byte_t *)"a\0b",3,"610062");
	hexEncodeTest("\\0",(const byte_t *)"\0",1,"00");
	stdoutput.printf("\n");


	// hex decode
	stdoutput.printf("hex-decode...\n");
	hexDecodeTest("NULL",NULL,zero,NULL,0);
	hexDecodeTest("NULL, size 3",NULL,3,NULL,0);
	hexDecodeTest("empty","",zero,"",0);
	hexDecodeTest("4142, size 0","4142",zero,"",0);
	hexDecodeTest("41","41",2,"\x41",1);
	hexDecodeTest("00abFF","00abFF",6,"\x00\xab\xff",3);
	hexDecodeTest("4142","4142",4,"\x41\x42",2);
	hexDecodeTest("aBcD","aBcD",4,"\xab\xcd",2);
	hexDecodeTest("41\\0" "042","41\0" "042",6,"\x41\x00\x42",3);
	stdoutput.printf("\n");


	// hex decode odd lengths
	stdoutput.printf("hex-decode odd lengths...\n");
	hexDecodeTest("414","414",3,"\x41",1);
	hexDecodeTest("4","4",1,"",0);
	hexDecodeTest(" 41"," 41",3,"\x04",1);
	stdoutput.printf("\n");


	// hex decode bad digits
	stdoutput.printf("hex-decode bad digits...\n");
	hexDecodeTest("4G","4G",2,"\x40",1);
	hexDecodeTest("G4","G4",2,"\x04",1);
	hexDecodeTest("zz","zz",2,"\x00",1);
	hexDecodeTest("0x41","0x41",4,"\x00\x41",2);
	hexDecodeTest("\\xc3\\xa9","\xc3\xa9",2,"\x00",1);
	stdoutput.printf("\n");


	// hex large input, through several chunks of the output * overloads
	stdoutput.printf("hex large input...\n");
	bigbytes=new byte_t[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigbytes[bi]=(byte_t)(bi%256);
	}
	bytesToCharsConsistencyTest("encode",
			charstring::hexEncode,charstring::hexEncode,
			charstring::hexEncode,charstring::hexEncode,
			charstring::hexEncode,charstring::hexEncode,
			bigbytes,bigsize);
	charstring::hexEncode(bigbytes,bigsize,&encodedbytes,&encodedlen);
	charsToBytesConsistencyTest("decode",
			charstring::hexDecode,charstring::hexDecode,
			charstring::hexDecode,charstring::hexDecode,
			charstring::hexDecode,charstring::hexDecode,
			encodedbytes,encodedlen,false);
	stringbuffer	hexsb;
	charstring::hexDecode(encodedbytes,&hexsb);
	test("decode 3a round trip",
		hexsb.getSize()==bigsize &&
		!bytestring::compare(hexsb.getBuffer(),bigbytes,bigsize));
	stdoutput.printf("\n");


	// hex output * overloads stop at the first failed or short write
	stdoutput.printf("hex failed writes...\n");
	limitedbuffer	shortenc(300,false);
	charstring::hexEncode(bigbytes,bigsize,&shortenc);
	test("encode short write",
		shortenc.charwrites==2 && !shortenc.bytewrites &&
		shortenc.getSize()==300 &&
		!bytestring::compare(shortenc.getBuffer(),encodedbytes,300));
	limitedbuffer	errorenc(300,true);
	charstring::hexEncode(bigbytes,bigsize,&errorenc);
	test("encode error",
		errorenc.charwrites==2 && !errorenc.bytewrites &&
		errorenc.getSize()==256 &&
		!bytestring::compare(errorenc.getBuffer(),encodedbytes,256));
	limitedbuffer	firstenc(0,true);
	charstring::hexEncode(bigbytes,bigsize,&firstenc);
	test("encode error on first write",
		firstenc.charwrites==1 && !firstenc.bytewrites &&
		!firstenc.getSize());
	limitedbuffer	shortdec(300,false);
	charstring::hexDecode(encodedbytes,encodedlen,&shortdec);
	test("decode short write",
		shortdec.bytewrites==2 && !shortdec.charwrites &&
		shortdec.getSize()==300 &&
		!bytestring::compare(shortdec.getBuffer(),bigbytes,300));
	limitedbuffer	errordec(300,true);
	charstring::hexDecode(encodedbytes,encodedlen,&errordec);
	test("decode error",
		errordec.bytewrites==2 && !errordec.charwrites &&
		errordec.getSize()==256 &&
		!bytestring::compare(errordec.getBuffer(),bigbytes,256));
	delete[] bigbytes;
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// base64 encode
	stdoutput.printf("base64-encode...\n");
	base64EncodeTest("NULL",NULL,zero,NULL);
	base64EncodeTest("NULL, size 3",NULL,3,NULL);
	base64EncodeTest("empty",(const byte_t *)"",zero,"");
	base64EncodeTest("abc, size 0",(const byte_t *)"abc",zero,"");
	base64EncodeTest("M",(const byte_t *)"M",1,"TQ==");
	base64EncodeTest("Ma",(const byte_t *)"Ma",2,"TWE=");
	base64EncodeTest("Man",(const byte_t *)"Man",3,"TWFu");
	base64EncodeTest("Mann",(const byte_t *)"Mann",4,"TWFubg==");
	base64EncodeTest("hello world",(const byte_t *)"hello world",11,
							"aGVsbG8gd29ybGQ=");
	base64EncodeTest("AAA",(const byte_t *)"AAA",3,"QUFB");
	base64EncodeTest("a\\0b",(const byte_t *)"a\0b",3,"YQBi");
	base64EncodeTest("\\0",(const byte_t *)"\0",1,"AA==");
	base64EncodeTest("\\0\\0\\0",(const byte_t *)"\0\0\0",3,"AAAA");
	base64EncodeTest("\\xff\\xfe\\x80",(const byte_t *)"\xff\xfe\x80",3,
									"//6A");
	base64EncodeTest("\\xfb\\xff",(const byte_t *)"\xfb\xff",2,"+/8=");
	stdoutput.printf("\n");


	// base64 decode
	stdoutput.printf("base64-decode...\n");
	base64DecodeTest("NULL",NULL,zero,NULL,0);
	base64DecodeTest("NULL, size 3",NULL,3,NULL,0);
	base64DecodeTest("empty","",zero,"",0);
	base64DecodeTest("TQ==, size 0","TQ==",zero,"",0);
	base64DecodeTest("TQ==","TQ==",4,"M\0\0",4);
	base64DecodeTest("TWE=","TWE=",4,"Ma\0",4);
	base64DecodeTest("TWFu","TWFu",4,"Man",4);
	base64DecodeTest("TWFubg==","TWFubg==",8,"Mann\0\0",7);
	base64DecodeTest("aGVsbG8gd29ybGQ=","aGVsbG8gd29ybGQ=",16,
							"hello world\0\0",13);
	base64DecodeTest("QUFB","QUFB",4,"AAA",4);
	base64DecodeTest("AAAA","AAAA",4,"\0\0\0",4);
	base64DecodeTest("YQBi","YQBi",4,"a\0b",4);
	base64DecodeTest("//7/","//7/",4,"\xff\xfe\xff",4);
	base64DecodeTest("QQ==QQ==","QQ==QQ==",8,"A\0\0A\0\0",7);
	base64DecodeTest("====","====",4,"\0\0\0",4);
	base64DecodeTest("Q=Q=","Q=Q=",4,"\x40\x04\0",4);
	stdoutput.printf("\n");


	// base64 decode whitespace, skipped where a group starts
	stdoutput.printf("base64-decode whitespace...\n");
	base64DecodeTest("QUJD\\n","QUJD\n",5,"ABC",4);
	base64DecodeTest("\\nQUJD","\nQUJD",5,"ABC",4);
	base64DecodeTest("  QUJD","  QUJD",6,"ABC",4);
	base64DecodeTest("QUJD QUJD","QUJD QUJD",9,"ABCABC",7);
	base64DecodeTest("QUJD\\r\\nQUJD","QUJD\r\nQUJD",10,"ABCABC",7);
	base64DecodeTest("2 spaces","  ",2,"",1);
	stdoutput.printf("\n");


	// base64 decode characters outside the alphabet
	stdoutput.printf("base64-decode bad characters...\n");
	base64DecodeTest("QU\\0D","QU\0D",4,"\x41\x7f\xc3",4);
	base64DecodeTest("QU-_","QU-_",4,"\x41\x7f\xff",4);
	base64DecodeTest("!!!!","!!!!",4,"\xff\xff\xff",4);
	base64DecodeTest("\\x80\\x80\\x80\\x80","\x80\x80\x80\x80",4,
							"\xff\xff\xff",4);
	base64DecodeTest("\\xc3\\xa9AA","\xc3\xa9" "AA",4,"\xff\xf0\0",4);
	base64DecodeTest("QUJ\\x80","QUJ\x80",4,"AB\xff",4);
	stdoutput.printf("\n");


	// base64 decode partial groups, missing characters count as =
	stdoutput.printf("base64-decode partial groups...\n");
	base64DecodeTest("A","A",1,"\0\0\0",4);
	base64DecodeTest("QQ","QQ",2,"A\0\0",4);
	base64DecodeTest("QQ=","QQ=",3,"A\0\0",4);
	base64DecodeTest("QUJDR","QUJDR",5,"ABCD\0\0",7);
	base64DecodeTest("QUJDRA","QUJDRA",6,"ABCD\0\0",7);
	base64DecodeTest("QUJDRA=","QUJDRA=",7,"ABCD\0\0",7);
	base64DecodeTest("QU JD","QU JD",5,"\x41\x7f\xc9\x0c\0\0",7);
	base64DecodeTest("QU\\nJD==","QU\nJD==",7,"\x41\x7f\xc9\x0c\0\0",7);
	base64DecodeTest("QQ==\\nQ","QQ==\nQ",6,"A\0\0@\0\0",7);
	base64DecodeTest("QUJD\\x01","QUJD\x01",5,"ABC\xfc\0\0",7);
	base64DecodeTest("QQ==QQ","QQ==QQ",6,"A\0\0A\0\0",7);
	stdoutput.printf("\n");


	// base64 large input, through several chunks of the output * overloads
	stdoutput.printf("base64 large input...\n");
	bigbytes=new byte_t[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigbytes[bi]=(byte_t)(bi%256);
	}
	bytesToCharsConsistencyTest("encode",
			charstring::base64Encode,charstring::base64Encode,
			charstring::base64Encode,charstring::base64Encode,
			charstring::base64Encode,charstring::base64Encode,
			bigbytes,bigsize);
	charstring::base64Encode(bigbytes,bigsize,&encodedbytes,&encodedlen);
	charsToBytesConsistencyTest("decode",
			charstring::base64Decode,charstring::base64Decode,
			charstring::base64Decode,charstring::base64Decode,
			charstring::base64Decode,charstring::base64Decode,
			encodedbytes,encodedlen,true);
	stringbuffer	b64sb;
	charstring::base64Decode(encodedbytes,&b64sb);
	uint64_t	padsize=(3-bigsize%3)%3;
	byte_t		padzeros[2]={0x00,0x00};
	test("decode 3a round trip",
		b64sb.getSize()==bigsize+padsize &&
		!bytestring::compare(b64sb.getBuffer(),bigbytes,bigsize) &&
		!bytestring::compare(b64sb.getBuffer()+bigsize,
						padzeros,padsize));
	stdoutput.printf("\n");


	// base64 output * overloads stop at the first failed or short write
	stdoutput.printf("base64 failed writes...\n");
	limitedbuffer	b64shortenc(300,false);
	charstring::base64Encode(bigbytes,bigsize,&b64shortenc);
	test("encode short write",
		b64shortenc.charwrites==2 && !b64shortenc.bytewrites &&
		b64shortenc.getSize()==300 &&
		!bytestring::compare(b64shortenc.getBuffer(),encodedbytes,300));
	limitedbuffer	b64errorenc(300,true);
	charstring::base64Encode(bigbytes,bigsize,&b64errorenc);
	test("encode error",
		b64errorenc.charwrites==2 && !b64errorenc.bytewrites &&
		b64errorenc.getSize()==256 &&
		!bytestring::compare(b64errorenc.getBuffer(),encodedbytes,256));
	limitedbuffer	b64firstenc(0,true);
	charstring::base64Encode(bigbytes,bigsize,&b64firstenc);
	test("encode error on first write",
		b64firstenc.charwrites==1 && !b64firstenc.bytewrites &&
		!b64firstenc.getSize());
	limitedbuffer	b64shortdec(300,false);
	charstring::base64Decode(encodedbytes,encodedlen,&b64shortdec);
	test("decode short write",
		b64shortdec.bytewrites==2 && !b64shortdec.charwrites &&
		b64shortdec.getSize()==300 &&
		!bytestring::compare(b64shortdec.getBuffer(),bigbytes,300));
	limitedbuffer	b64errordec(300,true);
	charstring::base64Decode(encodedbytes,encodedlen,&b64errordec);
	test("decode error",
		b64errordec.bytewrites==2 && !b64errordec.charwrites &&
		b64errordec.getSize()==256 &&
		!bytestring::compare(b64errordec.getBuffer(),bigbytes,256));
	limitedbuffer	b64firstdec(0,true);
	charstring::base64Decode(encodedbytes,encodedlen,&b64firstdec);
	test("decode error on first write",
		b64firstdec.bytewrites==1 && !b64firstdec.charwrites &&
		!b64firstdec.getSize());
	delete[] bigbytes;
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// url encode the buffer
	stdoutput.printf("url-encode...\n");
	const char	*original="!@#$%^&*()hello-+£";
	char    *escbuffer=charstring::urlEncode(original);
	test("escaped",!charstring::compare(escbuffer,
				"!%40%23$%25%5E%26*()hello-%2B%C2%A3"));
	char	*unescbuffer=charstring::urlDecode(escbuffer);
	test("unescaped",!charstring::compare(unescbuffer,original));
	delete[] escbuffer;
	delete[] unescbuffer;
	stdoutput.printf("\n");


	// url overloads, called directly
	stdoutput.printf("url overloads...\n");
	const char	*urlin="a b+c/d";
	uint64_t	urlinlen=charstring::getLength(urlin);
	const char	*urlout="a+b%2Bc%2Fd";
	uint64_t	urloutlen=charstring::getLength(urlout);
	char		*urlresult;
	uint64_t	urlresultlen;
	urlresult=charstring::urlEncode(urlin,urlinlen);
	test("encode 1b",!charstring::compare(urlresult,urlout));
	delete[] urlresult;
	charstring::urlEncode(urlin,&urlresult,&urlresultlen);
	test("encode 2a",urlresultlen==urloutlen &&
			!charstring::compare(urlresult,urlout));
	delete[] urlresult;
	charstring::urlEncode(urlin,urlinlen,&urlresult,&urlresultlen);
	test("encode 2b",urlresultlen==urloutlen &&
			!charstring::compare(urlresult,urlout));
	delete[] urlresult;
	stringbuffer	urlsb;
	charstring::urlEncode(urlin,&urlsb);
	test("encode 3a",urlsb.getSize()==urloutlen &&
			!charstring::compare(urlsb.getString(),urlout));
	urlsb.clear();
	charstring::urlEncode(urlin,urlinlen,&urlsb);
	test("encode 3b",urlsb.getSize()==urloutlen &&
			!charstring::compare(urlsb.getString(),urlout));
	urlsb.clear();
	urlresult=charstring::urlDecode(urlout,urloutlen);
	test("decode 1b",!charstring::compare(urlresult,urlin));
	delete[] urlresult;
	charstring::urlDecode(urlout,&urlresult,&urlresultlen);
	test("decode 2a",urlresultlen==urlinlen &&
			!charstring::compare(urlresult,urlin));
	delete[] urlresult;
	charstring::urlDecode(urlout,urloutlen,&urlresult,&urlresultlen);
	test("decode 2b",urlresultlen==urlinlen &&
			!charstring::compare(urlresult,urlin));
	delete[] urlresult;
	charstring::urlDecode(urlout,&urlsb);
	test("decode 3a",urlsb.getSize()==urlinlen &&
			!charstring::compare(urlsb.getString(),urlin));
	urlsb.clear();
	charstring::urlDecode(urlout,urloutlen,&urlsb);
	test("decode 3b",urlsb.getSize()==urlinlen &&
			!charstring::compare(urlsb.getString(),urlin));
	urlsb.clear();
	charstring::urlEncode(urlin,(output *)NULL);
	charstring::urlEncode(urlin,urlinlen,(output *)NULL);
	charstring::urlDecode(urlout,(output *)NULL);
	charstring::urlDecode(urlout,urloutlen,(output *)NULL);
	charstring::urlEncode((const char *)NULL,&urlsb);
	charstring::urlEncode((const char *)NULL,urlinlen,&urlsb);
	charstring::urlDecode((const char *)NULL,&urlsb);
	charstring::urlDecode((const char *)NULL,urloutlen,&urlsb);
	test("3a/3b NULL input",!urlsb.getSize());
	stdoutput.printf("\n");


	// url encode
	stdoutput.printf("url-encode overloads...\n");
	urlEncodeTest("NULL",NULL,zero,NULL,0);
	urlEncodeTest("NULL, size 3",NULL,3,NULL,0);
	urlEncodeTest("empty","",zero,"",0);
	urlEncodeTest("abc, size 0","abc",zero,"",0);
	urlEncodeTest("Hello World!/?&=~","Hello World!/?&=~",17,
					"Hello+World!%2F%3F%26%3D%7E",27);
	urlEncodeTest("AZaz09","AZaz09",6,"AZaz09",6);
	urlEncodeTest("az09$-_.!*'(),","az09$-_.!*'(),",14,
					"az09$-_.!*'(),",14);
	urlEncodeTest("+%\\t\\r\\n","+%\t\r\n",5,"%2B%25%09%0D%0A",15);
	urlEncodeTest("\" + \""," + ",3,"+%2B+",5);
	urlEncodeTest("\\303\\251\\377\\200","\303\251\377\200",4,
					"%C3%A9%FF%80",12);
	stdoutput.printf("\n");


	// url encode treats a NULL within the length as data
	stdoutput.printf("url-encode embedded NULL...\n");
	urlEncodeTest("a\\0b","a\0b",3,"a%00b",5);
	urlEncodeTest("\\0","\0",1,"%00",3);
	urlEncodeTest("ab\\0","ab\0",3,"ab%00",5);
	stdoutput.printf("\n");


	// url decode
	stdoutput.printf("url-decode overloads...\n");
	urlDecodeTest("NULL",NULL,zero,NULL,0);
	urlDecodeTest("NULL, size 3",NULL,3,NULL,0);
	urlDecodeTest("empty","",zero,"",0);
	urlDecodeTest("%41, size 0","%41",zero,"",0);
	urlDecodeTest("Hello+World%21%2f%2F","Hello+World%21%2f%2F",20,
					"Hello World!//",14);
	urlDecodeTest("%c3%A9","%c3%A9",6,"\xc3\xa9",2);
	urlDecodeTest("abc","abc",3,"abc",3);
	urlDecodeTest("%7e%7E","%7e%7E",6,"~~",2);
	urlDecodeTest("+","+",1," ",1);
	urlDecodeTest("%2B%20","%2B%20",6,"+ ",2);
	urlDecodeTest("\\xff%FF\\x80","\xff%FF\x80",5,"\xff\xff\x80",3);
	stdoutput.printf("\n");


	// url decode copies a truncated or invalid % sequence as-is, and
	// carries on decoding after the %
	stdoutput.printf("url-decode malformed...\n");
	urlDecodeTest("%","%",1,"%",1);
	urlDecodeTest("a%","a%",2,"a%",2);
	urlDecodeTest("%4","%4",2,"%4",2);
	urlDecodeTest("a%4","a%4",3,"a%4",3);
	urlDecodeTest("%41%4","%41%4",5,"A%4",3);
	urlDecodeTest("%ZZ","%ZZ",3,"%ZZ",3);
	urlDecodeTest("a%ZZb","a%ZZb",5,"a%ZZb",5);
	urlDecodeTest("%-1","%-1",3,"%-1",3);
	urlDecodeTest("%+1","%+1",3,"% 1",3);
	urlDecodeTest("% 4","% 4",3,"% 4",3);
	urlDecodeTest("%G1","%G1",3,"%G1",3);
	urlDecodeTest("%1G","%1G",3,"%1G",3);
	urlDecodeTest("%0x","%0x",3,"%0x",3);
	urlDecodeTest("%%41","%%41",4,"%A",2);
	urlDecodeTest("%41, size 2","%41",2,"%4",2);
	urlDecodeTest("a%41, size 3","a%41",3,"a%4",3);
	stdoutput.printf("\n");


	// url decode treats a NULL within the length as data, and decodes %00
	// to a NULL, which ends the string that the NULL-terminated overload
	// returns
	stdoutput.printf("url-decode embedded NULL...\n");
	urlDecodeTest("%00x","%00x",4,"\0x",2);
	urlDecodeTest("a%00b","a%00b",5,"a\0b",3);
	urlDecodeTest("a\\0b","a\0b",3,"a\0b",3);
	urlDecodeTest("%\\041","%\00041",4,"%\00041",4);
	urlDecodeTest("%4\\01","%4\0001",4,"%4\0001",4);
	urlresult=charstring::urlDecode("%00x");
	test("%00x 1a",urlresult && !urlresult[0]);
	delete[] urlresult;
	stdoutput.printf("\n");


	// url encode and decode large input, through several chunks of the
	// output * overloads
	stdoutput.printf("url large input...\n");
	char	*urlbig=new char[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		urlbig[bi]=(char)(bi%255+1);
	}
	charsToCharsConsistencyTest("encode",
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			urlbig,bigsize);
	charstring::urlEncode(urlbig,bigsize,&encodedbytes,&encodedlen);
	charsToCharsConsistencyTest("decode",
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			encodedbytes,encodedlen);
	charstring::urlDecode(encodedbytes,&urlsb);
	test("decode 3a round trip",
		urlsb.getSize()==bigsize &&
		!bytestring::compare(urlsb.getBuffer(),urlbig,bigsize));
	urlsb.clear();
	stdoutput.printf("\n");


	// url encode and decode output * overloads stop at the first failed or
	// short write
	stdoutput.printf("url failed writes...\n");
	limitedbuffer	urlshortenc(300,false);
	charstring::urlEncode(urlbig,bigsize,&urlshortenc);
	test("encode short write",
		urlshortenc.charwrites==2 && !urlshortenc.bytewrites &&
		urlshortenc.getSize()==300 &&
		!bytestring::compare(urlshortenc.getBuffer(),encodedbytes,300));
	limitedbuffer	urlerrorenc(300,true);
	charstring::urlEncode(urlbig,bigsize,&urlerrorenc);
	test("encode error",
		urlerrorenc.charwrites==2 && !urlerrorenc.bytewrites &&
		urlerrorenc.getSize()==256 &&
		!bytestring::compare(urlerrorenc.getBuffer(),encodedbytes,256));
	limitedbuffer	urlfirstenc(0,true);
	charstring::urlEncode(urlbig,bigsize,&urlfirstenc);
	test("encode error on first write",
		urlfirstenc.charwrites==1 && !urlfirstenc.bytewrites &&
		!urlfirstenc.getSize());
	limitedbuffer	urlshortdec(300,false);
	charstring::urlDecode(encodedbytes,encodedlen,&urlshortdec);
	test("decode short write",
		urlshortdec.charwrites==2 && !urlshortdec.bytewrites &&
		urlshortdec.getSize()==300 &&
		!bytestring::compare(urlshortdec.getBuffer(),urlbig,300));
	limitedbuffer	urlerrordec(300,true);
	charstring::urlDecode(encodedbytes,encodedlen,&urlerrordec);
	test("decode error",
		urlerrordec.charwrites==2 && !urlerrordec.bytewrites &&
		urlerrordec.getSize()==256 &&
		!bytestring::compare(urlerrordec.getBuffer(),urlbig,256));
	limitedbuffer	urlfirstdec(0,true);
	charstring::urlDecode(encodedbytes,encodedlen,&urlfirstdec);
	test("decode error on first write",
		urlfirstdec.charwrites==1 && !urlfirstdec.bytewrites &&
		!urlfirstdec.getSize());
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// url encode and decode large input with NULLs, through the
	// input-and-size overloads
	stdoutput.printf("url large input with NULLs...\n");
	for (uint64_t bi=0; bi<bigsize; bi++) {
		urlbig[bi]=(char)(bi%256);
	}
	charsToCharsConsistencyTest("encode",
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			charstring::urlEncode,charstring::urlEncode,
			urlbig,bigsize);
	charstring::urlEncode(urlbig,bigsize,&encodedbytes,&encodedlen);
	charsToCharsConsistencyTest("decode",
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			charstring::urlDecode,charstring::urlDecode,
			encodedbytes,encodedlen);
	charstring::urlDecode(encodedbytes,encodedlen,&urlsb);
	test("decode 3b round trip",
		urlsb.getSize()==bigsize &&
		!bytestring::compare(urlsb.getBuffer(),urlbig,bigsize));
	urlsb.clear();
	delete[] encodedbytes;
	delete[] urlbig;
	stdoutput.printf("\n");


	// html encode
	stdoutput.printf("html-encode...\n");
	htmlEncodeTest("NULL",NULL,"");
	htmlEncodeTest("empty","","");
	htmlEncodeTest("hello world","hello world","hello world");
	htmlEncodeTest("&","&","&amp;");
	htmlEncodeTest("<","<","&lt;");
	htmlEncodeTest(">",">","&gt;");
	htmlEncodeTest("\"","\"","&quot;");
	htmlEncodeTest("'","'","&#39;");
	htmlEncodeTest("<b>Tom & Jerry's \"x\"</b>",
			"<b>Tom & Jerry's \"x\"</b>",
		"&lt;b&gt;Tom &amp; Jerry&#39;s &quot;x&quot;&lt;/b&gt;");
	htmlEncodeTest("caf\\xC3\\xA9","caf\xC3\xA9","caf\xC3\xA9");
	stdoutput.printf("\n");


	// html decode
	stdoutput.printf("html-decode...\n");
	htmlDecodeTest("NULL",NULL,"");
	htmlDecodeTest("empty","","");
	htmlDecodeTest("hello","hello","hello");
	htmlDecodeTest("&amp;","&amp;","&");
	htmlDecodeTest("&lt;","&lt;","<");
	htmlDecodeTest("&gt;","&gt;",">");
	htmlDecodeTest("&quot;","&quot;","\"");
	htmlDecodeTest("&apos;","&apos;","'");
	htmlDecodeTest("&#39;","&#39;","'");
	htmlDecodeTest("&lt;b&gt;Tom &amp; Jerry&#39;s&lt;/b&gt;",
			"&lt;b&gt;Tom &amp; Jerry&#39;s&lt;/b&gt;",
			"<b>Tom & Jerry's</b>");
	stdoutput.printf("\n");


	// html decode numeric references
	stdoutput.printf("html-decode numeric references...\n");
	htmlDecodeTest("&#65;","&#65;","A");
	htmlDecodeTest("&#x41;","&#x41;","A");
	htmlDecodeTest("&#X41;","&#X41;","A");
	htmlDecodeTest("&#x0041;","&#x0041;","A");
	htmlDecodeTest("&#00000065;","&#00000065;","A");
	htmlDecodeTest("&#x7F;","&#x7F;","\x7F");
	htmlDecodeTest("&#x80;","&#x80;","\xC2\x80");
	htmlDecodeTest("&#233;","&#233;","\xC3\xA9");
	htmlDecodeTest("&#xe9;","&#xe9;","\xC3\xA9");
	htmlDecodeTest("&#xE9;","&#xE9;","\xC3\xA9");
	htmlDecodeTest("&#x7FF;","&#x7FF;","\xDF\xBF");
	htmlDecodeTest("&#x800;","&#x800;","\xE0\xA0\x80");
	htmlDecodeTest("&#8364;","&#8364;","\xE2\x82\xAC");
	htmlDecodeTest("&#x20AC;","&#x20AC;","\xE2\x82\xAC");
	htmlDecodeTest("&#xD7FF;","&#xD7FF;","\xED\x9F\xBF");
	htmlDecodeTest("&#xE000;","&#xE000;","\xEE\x80\x80");
	htmlDecodeTest("&#xFFFF;","&#xFFFF;","\xEF\xBF\xBF");
	htmlDecodeTest("&#x10000;","&#x10000;","\xF0\x90\x80\x80");
	htmlDecodeTest("&#128512;","&#128512;","\xF0\x9F\x98\x80");
	htmlDecodeTest("&#x1F600;","&#x1F600;","\xF0\x9F\x98\x80");
	htmlDecodeTest("&#x10FFFF;","&#x10FFFF;","\xF4\x8F\xBF\xBF");
	stdoutput.printf("\n");


	// html decode invalid code points
	stdoutput.printf("html-decode invalid code points...\n");
	htmlDecodeTest("&#0;","&#0;","&#0;");
	htmlDecodeTest("&#x0;","&#x0;","&#x0;");
	htmlDecodeTest("&#xD800;","&#xD800;","&#xD800;");
	htmlDecodeTest("&#xDBFF;","&#xDBFF;","&#xDBFF;");
	htmlDecodeTest("&#xDC00;","&#xDC00;","&#xDC00;");
	htmlDecodeTest("&#xDFFF;","&#xDFFF;","&#xDFFF;");
	htmlDecodeTest("&#55296;","&#55296;","&#55296;");
	htmlDecodeTest("&#x110000;","&#x110000;","&#x110000;");
	htmlDecodeTest("&#1114112;","&#1114112;","&#1114112;");
	htmlDecodeTest("&#4294967361;","&#4294967361;","&#4294967361;");
	htmlDecodeTest("&#xFFFFFFFF41;","&#xFFFFFFFF41;","&#xFFFFFFFF41;");
	htmlDecodeTest("&#99999999999999999999999999;",
			"&#99999999999999999999999999;",
			"&#99999999999999999999999999;");
	stdoutput.printf("\n");


	// html decode malformed entities
	stdoutput.printf("html-decode malformed entities...\n");
	htmlDecodeTest("&#65","&#65","&#65");
	htmlDecodeTest("&#x41","&#x41","&#x41");
	htmlDecodeTest("&amp","&amp","&amp");
	htmlDecodeTest("&amp ;","&amp ;","&amp ;");
	htmlDecodeTest("&#;","&#;","&#;");
	htmlDecodeTest("&#x;","&#x;","&#x;");
	htmlDecodeTest("&#X;","&#X;","&#X;");
	htmlDecodeTest("&#","&#","&#");
	htmlDecodeTest("&","&","&");
	htmlDecodeTest("a & b","a & b","a & b");
	htmlDecodeTest("&#6a;","&#6a;","&#6a;");
	htmlDecodeTest("&#x41g;","&#x41g;","&#x41g;");
	htmlDecodeTest("&# 65;","&# 65;","&# 65;");
	htmlDecodeTest("&#+65;","&#+65;","&#+65;");
	htmlDecodeTest("&#-65;","&#-65;","&#-65;");
	stdoutput.printf("\n");


	// html decode unknown entities
	stdoutput.printf("html-decode unknown entities...\n");
	htmlDecodeTest("&AMP;","&AMP;","&AMP;");
	htmlDecodeTest("&Lt;","&Lt;","&Lt;");
	htmlDecodeTest("&APOS;","&APOS;","&APOS;");
	htmlDecodeTest("&nbsp;","&nbsp;","&nbsp;");
	htmlDecodeTest("&copy;","&copy;","&copy;");
	htmlDecodeTest("&am;","&am;","&am;");
	htmlDecodeTest("&foo;","&foo;","&foo;");
	stdoutput.printf("\n");


	// html decode single pass
	stdoutput.printf("html-decode single pass...\n");
	htmlDecodeTest("&amp;lt;","&amp;lt;","&lt;");
	htmlDecodeTest("&amp;#65;","&amp;#65;","&#65;");
	htmlDecodeTest("&&amp;","&&amp;","&&");
	htmlDecodeTest("&#&amp;","&#&amp;","&#&");
	htmlDecodeTest("&#65&#66;","&#65&#66;","&#65B");
	htmlDecodeTest("&lt&gt;","&lt&gt;","&lt>");
	stdoutput.printf("\n");


	// html round trip
	stdoutput.printf("html round trip...\n");
	htmlRoundTripTest("empty","");
	htmlRoundTripTest("plain","plain");
	htmlRoundTripTest("&<>\"'","&<>\"'");
	htmlRoundTripTest("<b>Tom & Jerry's \"x\"</b>",
				"<b>Tom & Jerry's \"x\"</b>");
	htmlRoundTripTest("&amp;","&amp;");
	htmlRoundTripTest("caf\\xC3\\xA9","caf\xC3\xA9");
	stdoutput.printf("\n");


	// html overloads, called directly
	stdoutput.printf("html overloads...\n");
	const char	*htmlin="<a & b>";
	uint64_t	htmlinlen=charstring::getLength(htmlin);
	const char	*htmlout="&lt;a &amp; b&gt;";
	uint64_t	htmloutlen=charstring::getLength(htmlout);
	const char	*htmlnull=NULL;
	char		*htmlresult;
	uint64_t	htmlresultlen;
	htmlresult=charstring::htmlEncode(htmlin,htmlinlen);
	test("encode 1b",!charstring::compare(htmlresult,htmlout));
	delete[] htmlresult;
	charstring::htmlEncode(htmlin,&htmlresult,&htmlresultlen);
	test("encode 2a",htmlresultlen==htmloutlen &&
			!charstring::compare(htmlresult,htmlout));
	delete[] htmlresult;
	charstring::htmlEncode(htmlin,htmlinlen,&htmlresult,&htmlresultlen);
	test("encode 2b",htmlresultlen==htmloutlen &&
			!charstring::compare(htmlresult,htmlout));
	delete[] htmlresult;
	stringbuffer	htmlsb;
	charstring::htmlEncode(htmlin,&htmlsb);
	test("encode 3a",htmlsb.getSize()==htmloutlen &&
			!charstring::compare(htmlsb.getString(),htmlout));
	htmlsb.clear();
	charstring::htmlEncode(htmlin,htmlinlen,&htmlsb);
	test("encode 3b",htmlsb.getSize()==htmloutlen &&
			!charstring::compare(htmlsb.getString(),htmlout));
	htmlsb.clear();
	htmlresult=charstring::htmlDecode(htmlout,htmloutlen);
	test("decode 1b",!charstring::compare(htmlresult,htmlin));
	delete[] htmlresult;
	charstring::htmlDecode(htmlout,&htmlresult,&htmlresultlen);
	test("decode 2a",htmlresultlen==htmlinlen &&
			!charstring::compare(htmlresult,htmlin));
	delete[] htmlresult;
	charstring::htmlDecode(htmlout,htmloutlen,&htmlresult,&htmlresultlen);
	test("decode 2b",htmlresultlen==htmlinlen &&
			!charstring::compare(htmlresult,htmlin));
	delete[] htmlresult;
	charstring::htmlDecode(htmlout,&htmlsb);
	test("decode 3a",htmlsb.getSize()==htmlinlen &&
			!charstring::compare(htmlsb.getString(),htmlin));
	htmlsb.clear();
	charstring::htmlDecode(htmlout,htmloutlen,&htmlsb);
	test("decode 3b",htmlsb.getSize()==htmlinlen &&
			!charstring::compare(htmlsb.getString(),htmlin));
	htmlsb.clear();
	htmlresult=charstring::htmlEncode(htmlnull,htmlinlen);
	test("encode 1b NULL",htmlresult && !htmlresult[0]);
	delete[] htmlresult;
	charstring::htmlEncode(htmlnull,&htmlresult,&htmlresultlen);
	test("encode 2a NULL",htmlresult && !htmlresult[0] && !htmlresultlen);
	delete[] htmlresult;
	charstring::htmlEncode(htmlnull,htmlinlen,&htmlresult,&htmlresultlen);
	test("encode 2b NULL",htmlresult && !htmlresult[0] && !htmlresultlen);
	delete[] htmlresult;
	htmlresult=charstring::htmlDecode(htmlnull,htmloutlen);
	test("decode 1b NULL",htmlresult && !htmlresult[0]);
	delete[] htmlresult;
	charstring::htmlDecode(htmlnull,&htmlresult,&htmlresultlen);
	test("decode 2a NULL",htmlresult && !htmlresult[0] && !htmlresultlen);
	delete[] htmlresult;
	charstring::htmlDecode(htmlnull,htmloutlen,&htmlresult,&htmlresultlen);
	test("decode 2b NULL",htmlresult && !htmlresult[0] && !htmlresultlen);
	delete[] htmlresult;
	charstring::htmlEncode(htmlin,(output *)NULL);
	charstring::htmlEncode(htmlin,htmlinlen,(output *)NULL);
	charstring::htmlDecode(htmlout,(output *)NULL);
	charstring::htmlDecode(htmlout,htmloutlen,(output *)NULL);
	charstring::htmlEncode(htmlnull,&htmlsb);
	charstring::htmlEncode(htmlnull,htmlinlen,&htmlsb);
	charstring::htmlDecode(htmlnull,&htmlsb);
	charstring::htmlDecode(htmlnull,htmloutlen,&htmlsb);
	test("3a/3b NULL input",!htmlsb.getSize());
	stdoutput.printf("\n");


	// html encode
	stdoutput.printf("html-encode overloads...\n");
	htmlEncodeSizeTest("NULL",NULL,zero,"",0);
	htmlEncodeSizeTest("NULL, size 3",NULL,3,"",0);
	htmlEncodeSizeTest("empty","",zero,"",0);
	htmlEncodeSizeTest("&<>, size 0","&<>",zero,"",0);
	htmlEncodeSizeTest("plain","plain",5,"plain",5);
	htmlEncodeSizeTest("&<>\"'","&<>\"'",5,"&amp;&lt;&gt;&quot;&#39;",24);
	htmlEncodeSizeTest("a&b, size 2","a&b",2,"a&amp;",6);
	htmlEncodeSizeTest("<>, size 1","<>",1,"&lt;",4);
	htmlEncodeSizeTest("\\xff&\\x80","\xff&\x80",3,"\xff&amp;\x80",7);
	htmlEncodeSizeTest("\\xc3\\xa9\\xff\\x80","\xc3\xa9\xff\x80",4,
						"\xc3\xa9\xff\x80",4);
	stdoutput.printf("\n");


	// html encode treats a NULL within the length as data
	stdoutput.printf("html-encode embedded NULL...\n");
	htmlEncodeSizeTest("a\\0&","a\0&",3,"a\0&amp;",7);
	htmlEncodeSizeTest("&\\0","&\0",2,"&amp;\0",6);
	htmlEncodeSizeTest("\\0","\0",1,"\0",1);
	stdoutput.printf("\n");


	// html decode
	stdoutput.printf("html-decode overloads...\n");
	htmlDecodeSizeTest("NULL",NULL,zero,"",0);
	htmlDecodeSizeTest("NULL, size 3",NULL,3,"",0);
	htmlDecodeSizeTest("empty","",zero,"",0);
	htmlDecodeSizeTest("&amp;, size 0","&amp;",zero,"",0);
	htmlDecodeSizeTest("plain","plain",5,"plain",5);
	htmlDecodeSizeTest("&amp;&lt;&gt;&quot;&#39;&apos;",
				"&amp;&lt;&gt;&quot;&#39;&apos;",30,
				"&<>\"''",6);
	htmlDecodeSizeTest("&#x1F600;","&#x1F600;",9,"\xF0\x9F\x98\x80",4);
	htmlDecodeSizeTest("\\xff&amp;\\x80","\xff&amp;\x80",7,
						"\xff&\x80",3);
	htmlDecodeSizeTest("\\xc3\\xa9&#233;","\xc3\xa9&#233;",8,
						"\xc3\xa9\xc3\xa9",4);
	htmlDecodeSizeTest("&\\xff;","&\xff;",3,"&\xff;",3);
	htmlDecodeSizeTest("&#\\xff;","&#\xff;",4,"&#\xff;",4);
	htmlDecodeSizeTest("&#x\\xe9;","&#x\xe9;",5,"&#x\xe9;",5);
	stdoutput.printf("\n");


	// html decode only decodes an entity that ends within the length
	stdoutput.printf("html-decode entity cut off by the length...\n");
	htmlDecodeSizeTest("&amp;amp;, size 3","&amp;amp;",3,"&am",3);
	htmlDecodeSizeTest("&amp;amp;, size 4","&amp;amp;",4,"&amp",4);
	htmlDecodeSizeTest("&amp;amp;, size 5","&amp;amp;",5,"&",1);
	htmlDecodeSizeTest("&amp;amp;, size 9","&amp;amp;",9,"&amp;",5);
	htmlDecodeSizeTest("&#65;, size 4","&#65;",4,"&#65",4);
	htmlDecodeSizeTest("&#65;, size 5","&#65;",5,"A",1);
	htmlDecodeSizeTest("&#x41;, size 5","&#x41;",5,"&#x41",5);
	htmlDecodeSizeTest("&#x41;, size 3","&#x41;",3,"&#x",3);
	htmlDecodeSizeTest("&#x41;, size 2","&#x41;",2,"&#",2);
	htmlDecodeSizeTest("&#x41;, size 1","&#x41;",1,"&",1);
	htmlDecodeSizeTest("&quot;, size 5","&quot;",5,"&quot",5);
	htmlDecodeSizeTest("&#x1F600;, size 8","&#x1F600;",8,"&#x1F600",8);
	htmlDecodeSizeTest("a&lt;, size 4","a&lt;",4,"a&lt",4);
	htmlDecodeSizeTest("&lt;&gt;, size 7","&lt;&gt;",7,"<&gt",4);
	stdoutput.printf("\n");


	// html decode treats a NULL within the length as data, and no entity
	// contains one
	stdoutput.printf("html-decode embedded NULL...\n");
	htmlDecodeSizeTest("&amp;\\0&lt;","&amp;\0&lt;",10,"&\0<",3);
	htmlDecodeSizeTest("\\0&gt;","\0&gt;",5,"\0>",2);
	htmlDecodeSizeTest("&#65;\\0","&#65;\0",6,"A\0",2);
	htmlDecodeSizeTest("&\\0amp;","&\0amp;",6,"&\0amp;",6);
	htmlDecodeSizeTest("&am\\0p;","&am\0p;",6,"&am\0p;",6);
	htmlDecodeSizeTest("&#6\\05;","&#6\0" "5;",6,"&#6\0" "5;",6);
	htmlDecodeSizeTest("&#x\\041;","&#x\0" "41;",7,"&#x\0" "41;",7);
	stdoutput.printf("\n");


	// html encode and decode entities that straddle a flush of the
	// output * overloads
	stdoutput.printf("html entities across flushes...\n");
	stringbuffer	htmlplain;
	stringbuffer	htmlencoded;
	appendRepeated(&htmlplain,"a",254);
	htmlplain.append("&b");
	appendRepeated(&htmlencoded,"a",254);
	htmlencoded.append("&amp;b");
	htmlEncodeSizeTest("& at 254",
			htmlplain.getString(),htmlplain.getSize(),
			htmlencoded.getString(),htmlencoded.getSize());
	htmlDecodeSizeTest("&amp; at 254",
			htmlencoded.getString(),htmlencoded.getSize(),
			htmlplain.getString(),htmlplain.getSize());
	htmlplain.clear();
	htmlencoded.clear();
	appendRepeated(&htmlplain,"a",255);
	htmlplain.append("\xE2\x82\xAC" "b");
	appendRepeated(&htmlencoded,"a",255);
	htmlencoded.append("&#x20AC;b");
	htmlDecodeSizeTest("&#x20AC; at 255",
			htmlencoded.getString(),htmlencoded.getSize(),
			htmlplain.getString(),htmlplain.getSize());
	htmlplain.clear();
	htmlencoded.clear();
	appendRepeated(&htmlplain,"x&<\xC3\xA9\xF0\x9F\x98\x80\"'<>&",300);
	appendRepeated(&htmlencoded,
			"x&amp;&lt;\xC3\xA9\xF0\x9F\x98\x80"
			"&quot;&#39;&lt;&gt;&amp;",300);
	htmlEncodeSizeTest("mixed, 300 times",
			htmlplain.getString(),htmlplain.getSize(),
			htmlencoded.getString(),htmlencoded.getSize());
	htmlDecodeSizeTest("mixed, 300 times",
			htmlencoded.getString(),htmlencoded.getSize(),
			htmlplain.getString(),htmlplain.getSize());
	htmlencoded.clear();
	appendRepeated(&htmlencoded,
			"x&amp;&lt;&#233;&#x1F600;&quot;&apos;&#60;&#x3E;&amp;",
			300);
	htmlDecodeSizeTest("mixed references, 300 times",
			htmlencoded.getString(),htmlencoded.getSize(),
			htmlplain.getString(),htmlplain.getSize());
	stdoutput.printf("\n");


	// html encode and decode large input, through several chunks of the
	// output * overloads
	stdoutput.printf("html large input...\n");
	char	*htmlbig=new char[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		htmlbig[bi]=(char)(bi%255+1);
	}
	charsToCharsConsistencyTest("encode",
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			htmlbig,bigsize);
	charstring::htmlEncode(htmlbig,bigsize,&encodedbytes,&encodedlen);
	charsToCharsConsistencyTest("decode",
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			encodedbytes,encodedlen);
	charstring::htmlDecode(encodedbytes,&htmlsb);
	test("decode 3a round trip",
		htmlsb.getSize()==bigsize &&
		!bytestring::compare(htmlsb.getBuffer(),htmlbig,bigsize));
	htmlsb.clear();
	stdoutput.printf("\n");


	// html encode and decode output * overloads stop at the first failed
	// or short write
	stdoutput.printf("html failed writes...\n");
	limitedbuffer	htmlshortenc(300,false);
	charstring::htmlEncode(htmlbig,bigsize,&htmlshortenc);
	test("encode short write",
		htmlshortenc.charwrites==2 && !htmlshortenc.bytewrites &&
		htmlshortenc.getSize()==300 &&
		!bytestring::compare(htmlshortenc.getBuffer(),
							encodedbytes,300));
	limitedbuffer	htmlerrorenc(300,true);
	charstring::htmlEncode(htmlbig,bigsize,&htmlerrorenc);
	test("encode error",
		htmlerrorenc.charwrites==2 && !htmlerrorenc.bytewrites &&
		htmlerrorenc.getSize()==256 &&
		!bytestring::compare(htmlerrorenc.getBuffer(),
							encodedbytes,256));
	limitedbuffer	htmlfirstenc(0,true);
	charstring::htmlEncode(htmlbig,bigsize,&htmlfirstenc);
	test("encode error on first write",
		htmlfirstenc.charwrites==1 && !htmlfirstenc.bytewrites &&
		!htmlfirstenc.getSize());
	limitedbuffer	htmlshortdec(300,false);
	charstring::htmlDecode(encodedbytes,encodedlen,&htmlshortdec);
	test("decode short write",
		htmlshortdec.charwrites==2 && !htmlshortdec.bytewrites &&
		htmlshortdec.getSize()==300 &&
		!bytestring::compare(htmlshortdec.getBuffer(),htmlbig,300));
	limitedbuffer	htmlerrordec(300,true);
	charstring::htmlDecode(encodedbytes,encodedlen,&htmlerrordec);
	test("decode error",
		htmlerrordec.charwrites==2 && !htmlerrordec.bytewrites &&
		htmlerrordec.getSize()==256 &&
		!bytestring::compare(htmlerrordec.getBuffer(),htmlbig,256));
	limitedbuffer	htmlfirstdec(0,true);
	charstring::htmlDecode(encodedbytes,encodedlen,&htmlfirstdec);
	test("decode error on first write",
		htmlfirstdec.charwrites==1 && !htmlfirstdec.bytewrites &&
		!htmlfirstdec.getSize());
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// html encode and decode large input with NULLs, through the
	// input-and-size overloads
	stdoutput.printf("html large input with NULLs...\n");
	for (uint64_t bi=0; bi<bigsize; bi++) {
		htmlbig[bi]=(char)(bi%256);
	}
	charsToCharsConsistencyTest("encode",
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			charstring::htmlEncode,charstring::htmlEncode,
			htmlbig,bigsize);
	charstring::htmlEncode(htmlbig,bigsize,&encodedbytes,&encodedlen);
	charsToCharsConsistencyTest("decode",
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			charstring::htmlDecode,charstring::htmlDecode,
			encodedbytes,encodedlen);
	charstring::htmlDecode(encodedbytes,encodedlen,&htmlsb);
	test("decode 3b round trip",
		htmlsb.getSize()==bigsize &&
		!bytestring::compare(htmlsb.getBuffer(),htmlbig,bigsize));
	htmlsb.clear();
	delete[] encodedbytes;
	delete[] htmlbig;
	stdoutput.printf("\n");


	// json encode
	stdoutput.printf("json-encode...\n");
	jsonEncodeTest("NULL",NULL,"null");
	jsonEncodeTest("empty","","\"\"");
	jsonEncodeTest("hello world","hello world","\"hello world\"");
	jsonEncodeTest("\"","\"","\"\\\"\"");
	jsonEncodeTest("\\","\\","\"\\\\\"");
	jsonEncodeTest("/","/","\"/\"");
	jsonEncodeTest("a\"b\\c/d","a\"b\\c/d","\"a\\\"b\\\\c/d\"");
	jsonEncodeTest("space"," ","\" \"");
	jsonEncodeTest("DEL","\x7f","\"\x7f\"");
	jsonEncodeTest("\\x80","\x80","\"\x80\"");
	jsonEncodeTest("\\xff","\xff","\"\xff\"");
	jsonEncodeTest("caf\\xc3\\xa9","caf\xc3\xa9","\"caf\xc3\xa9\"");
	jsonEncodeTest("\\xf0\\x9f\\x98\\x80","\xf0\x9f\x98\x80",
					"\"\xf0\x9f\x98\x80\"");
	jsonEncodeTest("invalid UTF-8","\xc3","\"\xc3\"");
	jsonEncodeTest("null","null","\"null\"");
	jsonEncodeTest("\\u0041","\\u0041","\"\\\\u0041\"");
	jsonEncodeTest("line1\\nline2\\ttab\\r",
			"line1\x0a" "line2\x09" "tab\x0d",
			"\"line1\\u000aline2\\u0009tab\\u000d\"");
	jsonEncodeTest("\\b\\f","\x08\x0c","\"\\u0008\\u000c\"");
	jsonEncodeTest("<b>&amp;</b>'","<b>&amp;</b>'","\"<b>&amp;</b>'\"");
	jsonEncodeTest("two quotes","\"\"","\"\\\"\\\"\"");
	jsonEncodeTest("two backslashes","\\\\","\"\\\\\\\\\"");
	jsonEncodeTest("boundaries","\x1f \x7f\x80","\"\\u001f \x7f\x80\"");
	stdoutput.printf("\n");


	// json encode every control byte, and every other byte
	stdoutput.printf("json-encode every byte...\n");
	jsonEncodeTest("0x01","\x01","\"\\u0001\"");
	jsonEncodeTest("0x0a","\x0a","\"\\u000a\"");
	jsonEncodeTest("0x1f","\x1f","\"\\u001f\"");
	for (uint64_t bi=0; bi<256; bi++) {
		char		label[16];
		char		in=(char)bi;
		stringbuffer	expected;
		charstring::printf(label,sizeof(label),"byte 0x%02x",
								(uint32_t)bi);
		jsonReference(&expected,&in,1);
		jsonEncodeSizeTest(label,&in,1,
				expected.getString(),expected.getSize());
	}
	stdoutput.printf("\n");


	// json encode overloads
	stdoutput.printf("json-encode overloads...\n");
	jsonEncodeSizeTest("NULL",NULL,zero,"null",4);
	jsonEncodeSizeTest("NULL, size 3",NULL,3,"null",4);
	jsonEncodeSizeTest("empty","",zero,"\"\"",2);
	jsonEncodeSizeTest("abc, size 0","abc",zero,"\"\"",2);
	jsonEncodeSizeTest("hello world","hello world",11,
					"\"hello world\"",13);
	jsonEncodeSizeTest("a\"b, size 2","a\"b",2,"\"a\\\"\"",5);
	jsonEncodeSizeTest("\\xc3\\xa9, size 1","\xc3\xa9",1,"\"\xc3\"",3);
	jsonEncodeSizeTest("line1\\n, size 6","line1\x0a" "x",6,
					"\"line1\\u000a\"",13);
	stdoutput.printf("\n");


	// json encode treats a NULL within the length as data
	stdoutput.printf("json-encode embedded NULL...\n");
	jsonEncodeSizeTest("\\0","\0",1,"\"\\u0000\"",8);
	jsonEncodeSizeTest("a\\0b","a\0b",3,"\"a\\u0000b\"",10);
	jsonEncodeSizeTest("\\0\\0","\0\0",2,"\"\\u0000\\u0000\"",14);
	jsonEncodeSizeTest("a\\0\"","a\0\"",3,"\"a\\u0000\\\"\"",11);
	stdoutput.printf("\n");


	// json decode gives NULL for NULL and for exactly null
	stdoutput.printf("json-decode NULL and null...\n");
	jsonDecodeTest("NULL",NULL,NULL,0);
	jsonDecodeSizeTest("NULL, size 3",NULL,3,NULL,0);
	jsonDecodeTest("null","null",NULL,0);
	jsonDecodeSizeTest("nullx, size 4","nullx",4,NULL,0);
	jsonDecodeSizeTest("null, size 3","null",3,"nul",3);
	jsonDecodeSizeTest("null\\0","null\0",5,"null\0",5);
	jsonDecodeTest(" null"," null"," null",5);
	jsonDecodeTest("null ","null ","null ",5);
	jsonDecodeTest("NULL text","NULL","NULL",4);
	jsonDecodeTest("nul","nul","nul",3);
	jsonDecodeTest("\"null\"","\"null\"","null",4);
	stdoutput.printf("\n");


	// json decode quoted and unquoted input
	stdoutput.printf("json-decode quotes...\n");
	jsonDecodeTest("empty","","",0);
	jsonDecodeSizeTest("abc, size 0","abc",zero,"",0);
	jsonDecodeTest("lone quote","\"","",0);
	jsonDecodeTest("two quotes","\"\"","",0);
	jsonDecodeTest("\"abc\"","\"abc\"","abc",3);
	jsonDecodeTest("abc","abc","abc",3);
	jsonDecodeTest("\"abc","\"abc","abc",3);
	jsonDecodeTest("\"abc\"def","\"abc\"def","abc",3);
	jsonDecodeTest("\"abc\" ","\"abc\" ","abc",3);
	jsonDecodeTest("\"a\"b\"c\"","\"a\"b\"c\"","a",1);
	jsonDecodeTest(" \"abc\""," \"abc\""," \"abc\"",6);
	jsonDecodeTest("abc\"","abc\"","abc\"",4);
	jsonDecodeTest("a\"b\"c","a\"b\"c","a\"b\"c",5);
	jsonDecodeTest("\"a\\\"b\"","\"a\\\"b\"","a\"b",3);
	jsonDecodeTest("\"a\\\\\"b\"","\"a\\\\\"b\"","a\\",2);
	jsonDecodeTest("\"a\\\\\"","\"a\\\\\"","a\\",2);
	jsonDecodeTest("\"\\\"","\"\\\"","\"",1);
	jsonDecodeTest("\"ab\\\"","\"ab\\\"","ab\"",3);
	jsonDecodeTest("\"\\\\\"","\"\\\\\"","\\",1);
	jsonDecodeTest("\"\\\\\\\"\"","\"\\\\\\\"\"","\\\"",2);
	jsonDecodeTest("\" abc \"","\" abc \""," abc ",5);
	stdoutput.printf("\n");


	// json decode short escapes
	stdoutput.printf("json-decode short escapes...\n");
	jsonDecodeTest("\\\"","\\\"","\"",1);
	jsonDecodeTest("\\\\","\\\\","\\",1);
	jsonDecodeTest("\\/","\\/","/",1);
	jsonDecodeTest("\\b","\\b","\x08",1);
	jsonDecodeTest("\\f","\\f","\x0c",1);
	jsonDecodeTest("\\n","\\n","\x0a",1);
	jsonDecodeTest("\\r","\\r","\x0d",1);
	jsonDecodeTest("\\t","\\t","\x09",1);
	jsonDecodeTest("every short escape",
			"\"\\b\\f\\n\\r\\t\\/\\\\\\\"\"",
			"\x08\x0c\x0a\x0d\x09/\\\"",8);
	jsonDecodeTest("/","/","/",1);
	stdoutput.printf("\n");


	// json decode \u escapes, written as UTF-8, and never rescanned
	stdoutput.printf("json-decode \\u escapes...\n");
	jsonDecodeTest("\\u0041","\\u0041","A",1);
	jsonDecodeTest("\\u00e9","\\u00e9","\xc3\xa9",2);
	jsonDecodeTest("\\u00E9","\\u00E9","\xc3\xa9",2);
	jsonDecodeTest("\\u00Ff","\\u00Ff","\xc3\xbf",2);
	jsonDecodeTest("\\u007f","\\u007f","\x7f",1);
	jsonDecodeTest("\\u0080","\\u0080","\xc2\x80",2);
	jsonDecodeTest("\\u07ff","\\u07ff","\xdf\xbf",2);
	jsonDecodeTest("\\u0800","\\u0800","\xe0\xa0\x80",3);
	jsonDecodeTest("\\u20ac","\\u20ac","\xe2\x82\xac",3);
	jsonDecodeTest("\\u20AC","\\u20AC","\xe2\x82\xac",3);
	jsonDecodeTest("\\ud7ff","\\ud7ff","\xed\x9f\xbf",3);
	jsonDecodeTest("\\ue000","\\ue000","\xee\x80\x80",3);
	jsonDecodeTest("\\uffff","\\uffff","\xef\xbf\xbf",3);
	jsonDecodeTest("\\u000a","\\u000a","\x0a",1);
	jsonDecodeTest("\\u001f","\\u001f","\x1f",1);
	jsonDecodeTest("\\u0022","\\u0022","\"",1);
	jsonDecodeTest("\\u005c","\\u005c","\\",1);
	jsonDecodeTest("\\u005cn","\\u005cn","\\n",2);
	jsonDecodeTest("\\u005c\\u005c","\\u005c\\u005c","\\\\",2);
	jsonDecodeTest("\\u005cu0041","\\u005cu0041","\\u0041",6);
	jsonDecodeTest("\\\\n","\\\\n","\\n",2);
	jsonDecodeTest("\\\\u0041","\\\\u0041","\\u0041",6);
	jsonDecodeTest("\\\\\\\\","\\\\\\\\","\\\\",2);
	jsonDecodeTest("\"\\u00e9t\\u00e9\"","\"\\u00e9t\\u00e9\"",
						"\xc3\xa9t\xc3\xa9",5);
	for (uint32_t ui=0; ui<0x80; ui++) {
		char	lower[7];
		char	upper[7];
		char	expected=(char)ui;
		charstring::printf(lower,sizeof(lower),"\\u%04x",ui);
		charstring::printf(upper,sizeof(upper),"\\u%04X",ui);
		jsonDecodeSizeTest(lower,lower,6,&expected,1);
		jsonDecodeSizeTest(upper,upper,6,&expected,1);
	}
	stdoutput.printf("\n");


	// json decode \u0000 to a NULL, which ends the string that the
	// NULL-terminated overload returns, but which the lengths count
	stdoutput.printf("json-decode \\u0000...\n");
	jsonDecodeTest("\\u0000","\\u0000","\0",1);
	jsonDecodeTest("a\\u0000b","a\\u0000b","a\0b",3);
	jsonDecodeTest("\"\\u0000\"","\"\\u0000\"","\0",1);
	jsonDecodeTest("\"\\u0000\\u001f\\\"\\\\/\\x7f\"",
			"\"\\u0000\\u001f\\\"\\\\/\x7f\"",
			"\0\x1f\"\\/\x7f",6);
	char		*jsondecoded=charstring::jsonDecode("a\\u0000b");
	uint64_t	jsondecodedlen;
	test("a\\u0000b 1a",jsondecoded &&
			charstring::getLength(jsondecoded)==1 &&
			!bytestring::compare(jsondecoded,"a\0b",4));
	delete[] jsondecoded;
	charstring::jsonDecode("a\\u0000b",&jsondecoded,&jsondecodedlen);
	test("a\\u0000b 2a",jsondecodedlen==3 &&
			!bytestring::compare(jsondecoded,"a\0b",4));
	delete[] jsondecoded;
	stdoutput.printf("\n");


	// json decode surrogate pairs, only when a high surrogate is followed
	// by a low surrogate within the string
	stdoutput.printf("json-decode surrogate pairs...\n");
	jsonDecodeTest("\\ud83d\\ude00","\\ud83d\\ude00","\xf0\x9f\x98\x80",4);
	jsonDecodeTest("\\uD83D\\uDE00","\\uD83D\\uDE00","\xf0\x9f\x98\x80",4);
	jsonDecodeTest("\\uD83d\\udE00","\\uD83d\\udE00","\xf0\x9f\x98\x80",4);
	jsonDecodeTest("\\uD800\\uDC00","\\uD800\\uDC00","\xf0\x90\x80\x80",4);
	jsonDecodeTest("\\uDBFF\\uDFFF","\\uDBFF\\uDFFF","\xf4\x8f\xbf\xbf",4);
	jsonDecodeTest("\"\\ud83d\\ude00\"","\"\\ud83d\\ude00\"",
						"\xf0\x9f\x98\x80",4);
	jsonDecodeTest("x\\ud83d\\ude00y","x\\ud83d\\ude00y",
						"x\xf0\x9f\x98\x80y",6);
	jsonDecodeTest("\\ud83d","\\ud83d","\\ud83d",6);
	jsonDecodeTest("\\ud83dx","\\ud83dx","\\ud83dx",7);
	jsonDecodeTest("\\ude00","\\ude00","\\ude00",6);
	jsonDecodeTest("\\udfff","\\udfff","\\udfff",6);
	jsonDecodeTest("\\ud83d\\u0041","\\ud83d\\u0041","\\ud83dA",7);
	jsonDecodeTest("\\ud83d\\n","\\ud83d\\n","\\ud83d\x0a",7);
	jsonDecodeTest("\\ud83d\\ud83d\\ude00","\\ud83d\\ud83d\\ude00",
					"\\ud83d\xf0\x9f\x98\x80",10);
	jsonDecodeTest("\\ude00\\ud83d","\\ude00\\ud83d","\\ude00\\ud83d",12);
	jsonDecodeTest("\\ud83d\\ude0g","\\ud83d\\ude0g","\\ud83d\\ude0g",12);
	jsonDecodeTest("\\ud83d\\Ude00","\\ud83d\\Ude00","\\ud83d\\Ude00",12);
	jsonDecodeTest("\"\\ud83d\"\\ude00","\"\\ud83d\"\\ude00","\\ud83d",6);
	jsonDecodeTest("\"\\ud83d\\\"\\ude00\"","\"\\ud83d\\\"\\ude00\"",
					"\\ud83d\"\\ude00",13);
	for (uint64_t size=1; size<12; size++) {
		char	label[32];
		charstring::printf(label,sizeof(label),
				"\\ud83d\\ude00, size %lld",(long long)size);
		jsonDecodeSizeTest(label,"\\ud83d\\ude00",size,
						"\\ud83d\\ude00",size);
	}
	stdoutput.printf("\n");


	// json decode copies a malformed escape's backslash as-is, and carries
	// on decoding after it
	stdoutput.printf("json-decode malformed escapes...\n");
	jsonDecodeTest("\\q","\\q","\\q",2);
	jsonDecodeTest("\\x41","\\x41","\\x41",4);
	jsonDecodeTest("\\U0041","\\U0041","\\U0041",6);
	jsonDecodeTest("\\a\\0","\\a\\0","\\a\\0",4);
	jsonDecodeTest("\\u","\\u","\\u",2);
	jsonDecodeTest("\\u4","\\u4","\\u4",3);
	jsonDecodeTest("\\u41","\\u41","\\u41",4);
	jsonDecodeTest("\\u041","\\u041","\\u041",5);
	jsonDecodeTest("\\u004g","\\u004g","\\u004g",6);
	jsonDecodeTest("\\u 041","\\u 041","\\u 041",6);
	jsonDecodeTest("\\u+041","\\u+041","\\u+041",6);
	jsonDecodeSizeTest("\\u0041, size 5","\\u0041",5,"\\u004",5);
	jsonDecodeTest("\\u\\u0041","\\u\\u0041","\\uA",3);
	jsonDecodeTest("\\u004\\u0041","\\u004\\u0041","\\u004A",6);
	jsonDecodeTest("\\q\\n","\\q\\n","\\q\x0a",3);
	jsonDecodeTest("\\","\\","\\",1);
	jsonDecodeTest("abc\\","abc\\","abc\\",4);
	jsonDecodeTest("\"abc\\","\"abc\\","abc\\",4);
	jsonDecodeTest("\\\\\\","\\\\\\","\\\\",2);
	jsonDecodeSizeTest("\\n, size 1","\\n",1,"\\",1);
	stdoutput.printf("\n");


	// json decode copies other characters as-is
	stdoutput.printf("json-decode other characters...\n");
	jsonDecodeTest("\\x01\\x1f","\x01\x1f","\x01\x1f",2);
	jsonDecodeTest("\"\\x0a\"","\"\x0a\"","\x0a",1);
	jsonDecodeTest("\\xff\\xfe","\xff\xfe","\xff\xfe",2);
	jsonDecodeTest("\\xc3","\xc3","\xc3",1);
	jsonDecodeTest("\"caf\\xc3\\xa9\"","\"caf\xc3\xa9\"","caf\xc3\xa9",5);
	stdoutput.printf("\n");


	// json decode only recognizes a closing quote or an escape that ends
	// within the length
	stdoutput.printf("json-decode cut off by the length...\n");
	jsonDecodeSizeTest("\"abc\", size 4","\"abc\"",4,"abc",3);
	jsonDecodeSizeTest("\"abc\", size 1","\"abc\"",1,"",0);
	jsonDecodeSizeTest("\"\\\"\", size 3","\"\\\"\"",3,"\"",1);
	jsonDecodeSizeTest("\"\\\"\", size 2","\"\\\"\"",2,"\\",1);
	jsonDecodeSizeTest("\\u0041x, size 6","\\u0041x",6,"A",1);
	jsonDecodeSizeTest("\"\\n\", size 3","\"\\n\"",3,"\x0a",1);
	stdoutput.printf("\n");


	// json decode treats a NULL within the length as data
	stdoutput.printf("json-decode embedded NULL...\n");
	jsonDecodeSizeTest("\\\\0","\\\0",2,"\\\0",2);
	jsonDecodeSizeTest("\\u00\\041","\\u00\0" "41",7,"\\u00\0" "41",7);
	jsonDecodeSizeTest("a\\0b","a\0b",3,"a\0b",3);
	jsonDecodeSizeTest("\"a\\0b\"","\"a\0b\"",5,"a\0b",3);
	jsonDecodeSizeTest("\"\\0\"","\"\0\"",3,"\0",1);
	jsonDecodeSizeTest("\\0\\u0041","\0\\u0041",7,"\0A",2);
	stdoutput.printf("\n");


	// json round trip
	stdoutput.printf("json round trip...\n");
	jsonRoundTripTest("NULL",NULL,zero,"null",4);
	jsonRoundTripTest("empty","",zero,"\"\"",2);
	jsonRoundTripTest("null","null",4,"\"null\"",6);
	jsonRoundTripTest("\\0","\0",1,"\"\\u0000\"",8);
	jsonRoundTripTest("a\"b\\c/d\\x7f\\x80","a\"b\\c/d\x7f\x80",9,
					"\"a\\\"b\\\\c/d\x7f\x80\"",13);
	jsonRoundTripTest("\\u0041\\ud83d","\\u0041\\ud83d",12,
					"\"\\\\u0041\\\\ud83d\"",16);
	jsonRoundTripTest("\\xf0\\x9f\\x98\\x80\\n","\xf0\x9f\x98\x80\x0a",5,
					"\"\xf0\x9f\x98\x80\\u000a\"",12);
	jsonRoundTripTest("two quotes","\"\"",2,"\"\\\"\\\"\"",6);
	stdoutput.printf("\n");


	// json round trip every byte, and every pair of bytes
	stdoutput.printf("json round trip every byte...\n");
	char	jsonbytes[256];
	for (uint64_t bi=0; bi<256; bi++) {
		jsonbytes[bi]=(char)bi;
	}
	for (uint64_t bi=0; bi<256; bi++) {
		char		label[16];
		stringbuffer	expected;
		charstring::printf(label,sizeof(label),"byte 0x%02x",
								(uint32_t)bi);
		jsonReference(&expected,jsonbytes+bi,1);
		jsonRoundTripTest(label,jsonbytes+bi,1,
				expected.getString(),expected.getSize());
	}
	stringbuffer	jsonallbytes;
	jsonReference(&jsonallbytes,jsonbytes,256);
	jsonRoundTripTest("256 bytes",jsonbytes,256,
			jsonallbytes.getString(),jsonallbytes.getSize());
	char	*jsonnonnull=terminatedCopy(jsonbytes+1,255);
	char	*jsonallencoded=charstring::jsonEncode(jsonnonnull);
	jsondecoded=charstring::jsonDecode(jsonallencoded);
	test("bytes 1 to 255 1a",
			!charstring::compare(jsondecoded,jsonnonnull));
	delete[] jsondecoded;
	delete[] jsonallencoded;
	delete[] jsonnonnull;
	bool	jsonpairsok=true;
	for (uint64_t bi=0; bi<65536 && jsonpairsok; bi++) {
		char		pair[2]={(char)(bi>>8),(char)(bi&0xFF)};
		char		*pairencoded;
		uint64_t	pairencodedlen;
		char		*pairdecoded;
		uint64_t	pairdecodedlen;
		charstring::jsonEncode(pair,2,&pairencoded,&pairencodedlen);
		charstring::jsonDecode(pairencoded,pairencodedlen,
						&pairdecoded,&pairdecodedlen);
		jsonpairsok=(pairdecodedlen==2 &&
				!bytestring::compare(pairdecoded,pair,2));
		delete[] pairencoded;
		delete[] pairdecoded;
	}
	test("every pair of bytes",jsonpairsok);
	stdoutput.printf("\n");


	// json encode overloads, called directly
	stdoutput.printf("json overloads...\n");
	const char	*jsonin="a\"b\\c\x0a/";
	uint64_t	jsoninlen=charstring::getLength(jsonin);
	const char	*jsonout="\"a\\\"b\\\\c\\u000a/\"";
	uint64_t	jsonoutlen=charstring::getLength(jsonout);
	const char	*jsonnull=NULL;
	const char	*jsonnullout="null";
	uint64_t	jsonnulllen=4;
	char		*jsonresult;
	uint64_t	jsonresultlen;
	jsonresult=charstring::jsonEncode(jsonin);
	test("encode 1a",!charstring::compare(jsonresult,jsonout));
	delete[] jsonresult;
	jsonresult=charstring::jsonEncode(jsonin,jsoninlen);
	test("encode 1b",!charstring::compare(jsonresult,jsonout));
	delete[] jsonresult;
	charstring::jsonEncode(jsonin,&jsonresult,&jsonresultlen);
	test("encode 2a",jsonresultlen==jsonoutlen &&
			!charstring::compare(jsonresult,jsonout));
	delete[] jsonresult;
	charstring::jsonEncode(jsonin,jsoninlen,&jsonresult,&jsonresultlen);
	test("encode 2b",jsonresultlen==jsonoutlen &&
			!charstring::compare(jsonresult,jsonout));
	delete[] jsonresult;
	stringbuffer	jsonsb;
	charstring::jsonEncode(jsonin,&jsonsb);
	test("encode 3a",jsonsb.getSize()==jsonoutlen &&
			!charstring::compare(jsonsb.getString(),jsonout));
	jsonsb.clear();
	charstring::jsonEncode(jsonin,jsoninlen,&jsonsb);
	test("encode 3b",jsonsb.getSize()==jsonoutlen &&
			!charstring::compare(jsonsb.getString(),jsonout));
	jsonsb.clear();
	jsonresult=charstring::jsonEncode(jsonnull);
	test("encode 1a NULL",!charstring::compare(jsonresult,jsonnullout));
	delete[] jsonresult;
	jsonresult=charstring::jsonEncode(jsonnull,jsoninlen);
	test("encode 1b NULL",!charstring::compare(jsonresult,jsonnullout));
	delete[] jsonresult;
	charstring::jsonEncode(jsonnull,&jsonresult,&jsonresultlen);
	test("encode 2a NULL",jsonresultlen==jsonnulllen &&
			!charstring::compare(jsonresult,jsonnullout));
	delete[] jsonresult;
	charstring::jsonEncode(jsonnull,jsoninlen,&jsonresult,&jsonresultlen);
	test("encode 2b NULL",jsonresultlen==jsonnulllen &&
			!charstring::compare(jsonresult,jsonnullout));
	delete[] jsonresult;
	charstring::jsonEncode(jsonnull,&jsonsb);
	test("encode 3a NULL",jsonsb.getSize()==jsonnulllen &&
			!bytestring::compare(jsonsb.getBuffer(),
						jsonnullout,jsonnulllen));
	jsonsb.clear();
	charstring::jsonEncode(jsonnull,jsoninlen,&jsonsb);
	test("encode 3b NULL",jsonsb.getSize()==jsonnulllen &&
			!bytestring::compare(jsonsb.getBuffer(),
						jsonnullout,jsonnulllen));
	jsonsb.clear();
	charstring::jsonEncode(jsonin,(output *)NULL);
	charstring::jsonEncode(jsonin,jsoninlen,(output *)NULL);
	charstring::jsonEncode(jsonnull,(output *)NULL);
	charstring::jsonEncode(jsonnull,jsoninlen,(output *)NULL);
	test("3a/3b NULL output",!jsonsb.getSize());
	jsonresult=charstring::jsonDecode(jsonout);
	test("decode 1a",!charstring::compare(jsonresult,jsonin));
	delete[] jsonresult;
	jsonresult=charstring::jsonDecode(jsonout,jsonoutlen);
	test("decode 1b",!charstring::compare(jsonresult,jsonin));
	delete[] jsonresult;
	charstring::jsonDecode(jsonout,&jsonresult,&jsonresultlen);
	test("decode 2a",jsonresultlen==jsoninlen &&
			!charstring::compare(jsonresult,jsonin));
	delete[] jsonresult;
	charstring::jsonDecode(jsonout,jsonoutlen,&jsonresult,&jsonresultlen);
	test("decode 2b",jsonresultlen==jsoninlen &&
			!charstring::compare(jsonresult,jsonin));
	delete[] jsonresult;
	charstring::jsonDecode(jsonout,&jsonsb);
	test("decode 3a",jsonsb.getSize()==jsoninlen &&
			!charstring::compare(jsonsb.getString(),jsonin));
	jsonsb.clear();
	charstring::jsonDecode(jsonout,jsonoutlen,&jsonsb);
	test("decode 3b",jsonsb.getSize()==jsoninlen &&
			!charstring::compare(jsonsb.getString(),jsonin));
	jsonsb.clear();
	jsonresult=charstring::jsonDecode(jsonnull);
	test("decode 1a NULL",!jsonresult);
	jsonresult=charstring::jsonDecode(jsonnull,jsonoutlen);
	test("decode 1b NULL",!jsonresult);
	jsonresultlen=1;
	charstring::jsonDecode(jsonnull,&jsonresult,&jsonresultlen);
	test("decode 2a NULL",!jsonresult && !jsonresultlen);
	jsonresultlen=1;
	charstring::jsonDecode(jsonnull,jsonoutlen,&jsonresult,&jsonresultlen);
	test("decode 2b NULL",!jsonresult && !jsonresultlen);
	jsonresult=charstring::jsonDecode(jsonnullout);
	test("decode 1a null",!jsonresult);
	jsonresult=charstring::jsonDecode(jsonnullout,jsonnulllen);
	test("decode 1b null",!jsonresult);
	jsonresultlen=1;
	charstring::jsonDecode(jsonnullout,&jsonresult,&jsonresultlen);
	test("decode 2a null",!jsonresult && !jsonresultlen);
	jsonresultlen=1;
	charstring::jsonDecode(jsonnullout,jsonnulllen,
					&jsonresult,&jsonresultlen);
	test("decode 2b null",!jsonresult && !jsonresultlen);
	charstring::jsonDecode(jsonnull,&jsonsb);
	charstring::jsonDecode(jsonnull,jsonoutlen,&jsonsb);
	charstring::jsonDecode(jsonnullout,&jsonsb);
	charstring::jsonDecode(jsonnullout,jsonnulllen,&jsonsb);
	test("decode 3a/3b NULL and null input",!jsonsb.getSize());
	charstring::jsonDecode(jsonout,(output *)NULL);
	charstring::jsonDecode(jsonout,jsonoutlen,(output *)NULL);
	charstring::jsonDecode(jsonnull,(output *)NULL);
	charstring::jsonDecode(jsonnull,jsonoutlen,(output *)NULL);
	test("decode 3a/3b NULL output",!jsonsb.getSize());
	stdoutput.printf("\n");


	// json encode escapes that straddle a flush of the output *
	// overloads, the opening quote shifts the output by 1
	stdoutput.printf("json encode across flushes...\n");
	const char	*jsonspecials[]={"\"","\\","\x01","\x1f","\x0a",NULL};
	for (const char **sp=jsonspecials; *sp; sp++) {
		for (uint64_t offset=248; offset<258; offset++) {
			char		label[32];
			stringbuffer	plain;
			stringbuffer	expected;
			appendRepeated(&plain,"a",offset);
			plain.append(*sp);
			plain.append("bc");
			jsonReference(&expected,plain.getString(),
							plain.getSize());
			charstring::printf(label,sizeof(label),
					"0x%02x at %lld",
					(uint32_t)(byte_t)(*sp)[0],
					(long long)offset);
			jsonEncodeSizeTest(label,
				plain.getString(),plain.getSize(),
				expected.getString(),expected.getSize());
		}
	}
	for (uint64_t plainlen=253; plainlen<258; plainlen++) {
		char		label[32];
		stringbuffer	plain;
		stringbuffer	expected;
		appendRepeated(&plain,"a",plainlen);
		jsonReference(&expected,plain.getString(),plain.getSize());
		charstring::printf(label,sizeof(label),"%lld plain",
							(long long)plainlen);
		jsonEncodeSizeTest(label,plain.getString(),plain.getSize(),
				expected.getString(),expected.getSize());
	}
	stringbuffer	jsonplain;
	stringbuffer	jsonencoded;
	appendRepeated(&jsonplain,"x\"\\/\x01\x1f\x7f\xC3\xA9\xF0\x9F\x98\x80 ",
									300);
	jsonReference(&jsonencoded,jsonplain.getString(),jsonplain.getSize());
	jsonEncodeSizeTest("mixed, 300 times",
			jsonplain.getString(),jsonplain.getSize(),
			jsonencoded.getString(),jsonencoded.getSize());
	stdoutput.printf("\n");


	// json decode escapes that straddle a flush of the output *
	// overloads, quoted and unquoted
	stdoutput.printf("json decode across flushes...\n");
	const char	*jsonescapes[]={
		"\\u00e9","\xc3\xa9",
		"\\ud83d\\ude00","\xf0\x9f\x98\x80",
		"\\\"","\"",
		"\\\\","\\",
		"\\n","\x0a",
		"\\q","\\q",
		NULL
	};
	for (const char **ep=jsonescapes; *ep; ep+=2) {
		for (uint64_t offset=248; offset<258; offset++) {
			for (uint64_t quoted=0; quoted<2; quoted++) {
				char		label[48];
				stringbuffer	encoded;
				stringbuffer	expected;
				if (quoted) {
					encoded.append('"');
				}
				appendRepeated(&encoded,"a",offset);
				encoded.append(*ep);
				encoded.append("bc");
				if (quoted) {
					encoded.append('"');
				}
				appendRepeated(&expected,"a",offset);
				expected.append(*(ep+1));
				expected.append("bc");
				charstring::printf(label,sizeof(label),
						"%s%s at %lld",
						(quoted)?"quoted ":"",*ep,
						(long long)offset);
				jsonDecodeSizeTest(label,
					encoded.getString(),
					encoded.getSize(),
					expected.getString(),
					expected.getSize());
			}
		}
	}
	jsonDecodeSizeTest("mixed, 300 times",
			jsonencoded.getString(),jsonencoded.getSize(),
			jsonplain.getString(),jsonplain.getSize());
	jsonencoded.clear();
	appendRepeated(&jsonencoded,
			"x\\\"\\\\\\/\\u0001\\u001F\x7f"
			"\\u00e9\\uD83D\\ude00 ",300);
	jsonDecodeSizeTest("mixed escapes, 300 times",
			jsonencoded.getString(),jsonencoded.getSize(),
			jsonplain.getString(),jsonplain.getSize());
	stdoutput.printf("\n");


	// json encode and decode large input, through several chunks of the
	// output * overloads
	stdoutput.printf("json large input...\n");
	char	*jsonbig=new char[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		jsonbig[bi]=(char)(bi%255+1);
	}
	jsonencoded.clear();
	jsonReference(&jsonencoded,jsonbig,bigsize);
	jsonEncodeSizeTest("encode",jsonbig,bigsize,
			jsonencoded.getString(),jsonencoded.getSize());
	charstring::jsonEncode(jsonbig,bigsize,&encodedbytes,&encodedlen);
	jsonDecodeSizeTest("decode",encodedbytes,encodedlen,jsonbig,bigsize);
	charstring::jsonDecode(encodedbytes,&jsonsb);
	test("decode 3a round trip",
		jsonsb.getSize()==bigsize &&
		!bytestring::compare(jsonsb.getBuffer(),jsonbig,bigsize));
	jsonsb.clear();
	stdoutput.printf("\n");


	// json encode output * overloads stop at the first failed or short
	// write
	stdoutput.printf("json failed writes...\n");
	limitedbuffer	jsonshortenc(300,false);
	charstring::jsonEncode(jsonbig,bigsize,&jsonshortenc);
	test("encode short write",
		jsonshortenc.charwrites==2 && !jsonshortenc.bytewrites &&
		jsonshortenc.getSize()==300 &&
		!bytestring::compare(jsonshortenc.getBuffer(),
							encodedbytes,300));
	limitedbuffer	jsonerrorenc(300,true);
	charstring::jsonEncode(jsonbig,bigsize,&jsonerrorenc);
	test("encode error",
		jsonerrorenc.charwrites==2 && !jsonerrorenc.bytewrites &&
		jsonerrorenc.getSize()==256 &&
		!bytestring::compare(jsonerrorenc.getBuffer(),
							encodedbytes,256));
	limitedbuffer	jsonfirstenc(0,true);
	charstring::jsonEncode(jsonbig,bigsize,&jsonfirstenc);
	test("encode error on first write",
		jsonfirstenc.charwrites==1 && !jsonfirstenc.bytewrites &&
		!jsonfirstenc.getSize());
	limitedbuffer	jsonshortnull(2,false);
	charstring::jsonEncode(jsonnull,&jsonshortnull);
	test("encode NULL short write",
		jsonshortnull.charwrites==1 && !jsonshortnull.bytewrites &&
		jsonshortnull.getSize()==2 &&
		!bytestring::compare(jsonshortnull.getBuffer(),"nu",2));
	limitedbuffer	jsonshortdec(300,false);
	charstring::jsonDecode(encodedbytes,encodedlen,&jsonshortdec);
	test("decode short write",
		jsonshortdec.charwrites==2 && !jsonshortdec.bytewrites &&
		jsonshortdec.getSize()==300 &&
		!bytestring::compare(jsonshortdec.getBuffer(),jsonbig,300));
	limitedbuffer	jsonerrordec(300,true);
	charstring::jsonDecode(encodedbytes,encodedlen,&jsonerrordec);
	test("decode error",
		jsonerrordec.charwrites==2 && !jsonerrordec.bytewrites &&
		jsonerrordec.getSize()==256 &&
		!bytestring::compare(jsonerrordec.getBuffer(),jsonbig,256));
	limitedbuffer	jsonfirstdec(0,true);
	charstring::jsonDecode(encodedbytes,encodedlen,&jsonfirstdec);
	test("decode error on first write",
		jsonfirstdec.charwrites==1 && !jsonfirstdec.bytewrites &&
		!jsonfirstdec.getSize());
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// json encode and decode large input with NULLs, through the
	// input-and-size overloads
	stdoutput.printf("json large input with NULLs...\n");
	for (uint64_t bi=0; bi<bigsize; bi++) {
		jsonbig[bi]=(char)(bi%256);
	}
	jsonencoded.clear();
	jsonReference(&jsonencoded,jsonbig,bigsize);
	jsonEncodeSizeTest("encode",jsonbig,bigsize,
			jsonencoded.getString(),jsonencoded.getSize());
	jsonDecodeSizeTest("decode",
			jsonencoded.getString(),jsonencoded.getSize(),
			jsonbig,bigsize);
	jsonDecodeSizeTest("decode raw",jsonbig,bigsize,jsonbig,bigsize);
	charstring::jsonDecode(jsonencoded.getString(),jsonencoded.getSize(),
								&jsonsb);
	test("decode 3b round trip",
		jsonsb.getSize()==bigsize &&
		!bytestring::compare(jsonsb.getBuffer(),jsonbig,bigsize));
	jsonsb.clear();
	delete[] jsonbig;
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
			sb.printf("=%02x",(byte_t)i);
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
	bytesToCharsConsistencyTest("encoded",
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		bb.getBuffer(),bb.getSize());
	charsToBytesConsistencyTest("decoded",
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		encoded,encodedlength,false);
	delete[] encoded;
	delete[] decoded;
	stdoutput.printf("\n");


	// quoted printable encode
	stdoutput.printf("quoted-printable-encode...\n");
	quotedPrintableEncodeTest("NULL",NULL,zero,NULL);
	quotedPrintableEncodeTest("NULL, size 3",NULL,3,NULL);
	quotedPrintableEncodeTest("empty",(const byte_t *)"",zero,"");
	quotedPrintableEncodeTest("abc, size 0",(const byte_t *)"abc",zero,"");
	quotedPrintableEncodeTest("hello",(const byte_t *)"hello",5,"hello");
	quotedPrintableEncodeTest("a=b\\tc\\xff",
				(const byte_t *)"a=b\tc\xff",6,"a=3db=09c=ff");
	quotedPrintableEncodeTest("a\\0b",(const byte_t *)"a\0b",3,"a=00b");
	quotedPrintableEncodeTest("line1\\r\\nline2",
				(const byte_t *)"line1\r\nline2",12,
				"line1=0d=0aline2");
	quotedPrintableEncodeTest("trailing ",(const byte_t *)"trailing ",9,
								"trailing ");
	quotedPrintableEncodeTest("~ \\x7f",(const byte_t *)"~ \x7f",3,
								"~ =7f");
	stdoutput.printf("\n");


	// quoted printable encode soft line breaks, checked before each byte
	// once a line reaches 77 characters
	stdoutput.printf("quoted-printable-encode soft line breaks...\n");
	stringbuffer	qpin;
	stringbuffer	qpout;
	appendRepeated(&qpin,"a",77);
	quotedPrintableEncodeTest("77 a's",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpin.getString());
	qpin.append("a");
	appendRepeated(&qpout,"a",77);
	qpout.append("=\r\na");
	quotedPrintableEncodeTest("78 a's",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpout.getString());
	qpin.clear();
	qpout.clear();
	appendRepeated(&qpin,"a",76);
	qpin.append("==");
	appendRepeated(&qpout,"a",76);
	qpout.append("=3d=\r\n=3d");
	quotedPrintableEncodeTest("76 a's and ==",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpout.getString());
	qpin.clear();
	qpout.clear();
	appendRepeated(&qpin,"=",59);
	appendRepeated(&qpout,"=3d",26);
	qpout.append("=\r\n");
	appendRepeated(&qpout,"=3d",26);
	qpout.append("=\r\n");
	appendRepeated(&qpout,"=3d",7);
	quotedPrintableEncodeTest("59 ='s",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpout.getString());
	qpin.clear();
	qpout.clear();
	appendRepeated(&qpin,"a",158);
	appendRepeated(&qpout,"a",77);
	qpout.append("=\r\n");
	appendRepeated(&qpout,"a",77);
	qpout.append("=\r\naaaa");
	quotedPrintableEncodeTest("158 a's",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpout.getString());
	stdoutput.printf("\n");


	// quoted printable soft line breaks that straddle the 256-byte
	// chunks of the output * overloads, 15 full lines and then a
	// 78 or 79 character line, so the break starts at 1278 or 1279
	stdoutput.printf("quoted-printable soft line breaks "
					"across chunks...\n");
	for (uint64_t qpa=75; qpa<77; qpa++) {
		qpin.clear();
		qpout.clear();
		appendRepeated(&qpin,"a",77*15+qpa);
		qpin.append("=a");
		for (uint64_t qpl=0; qpl<15; qpl++) {
			appendRepeated(&qpout,"a",77);
			qpout.append("=\r\n");
		}
		appendRepeated(&qpout,"a",qpa);
		qpout.append("=3d=\r\na");
		uint64_t	qpbreak=1200+qpa+3;
		test("break straddles chunks",
			qpout.getSize()>qpbreak+3 &&
			!bytestring::compare(qpout.getString()+qpbreak,
								"=\r\n",3) &&
			qpbreak<1280 && qpbreak+3>1280);
		quotedPrintableEncodeTest("encode",
			(const byte_t *)qpin.getString(),qpin.getSize(),
			qpout.getString());
		quotedPrintableDecodeTest("decode",
			qpout.getString(),qpout.getSize(),
			qpin.getString(),qpin.getSize());
	}
	stdoutput.printf("\n");


	// quoted printable decode
	stdoutput.printf("quoted-printable-decode...\n");
	quotedPrintableDecodeTest("NULL",NULL,zero,NULL,0);
	quotedPrintableDecodeTest("NULL, size 3",NULL,3,NULL,0);
	quotedPrintableDecodeTest("empty","",zero,"",0);
	quotedPrintableDecodeTest("=41, size 0","=41",zero,"",0);
	quotedPrintableDecodeTest("hello","hello",5,"hello",5);
	quotedPrintableDecodeTest("a=3Db=09c=FF","a=3Db=09c=FF",12,
						"a=b\tc\xff",6);
	quotedPrintableDecodeTest("=3d=ff","=3d=ff",6,"=\xff",2);
	quotedPrintableDecodeTest("=a1","=a1",3,"\xa1",1);
	quotedPrintableDecodeTest("a\\r\\nb","a\r\nb",4,"a\r\nb",4);
	quotedPrintableDecodeTest("\\xc3\\xa9","\xc3\xa9",2,"\xc3\xa9",2);
	qpin.clear();
	qpout.clear();
	appendRepeated(&qpin,"a",77);
	qpin.append("=\r\naaa");
	appendRepeated(&qpout,"a",80);
	quotedPrintableDecodeTest("77 a's, soft break, aaa",
			qpin.getString(),qpin.getSize(),
			qpout.getString(),qpout.getSize());
	stdoutput.printf("\n");


	// quoted printable decode soft line breaks
	stdoutput.printf("quoted-printable-decode soft line breaks...\n");
	quotedPrintableDecodeTest("a=\\r\\nb","a=\r\nb",5,"ab",2);
	quotedPrintableDecodeTest("a=\\nb","a=\nb",4,"ab",2);
	quotedPrintableDecodeTest("a=\\rb","a=\rb",4,"ab",2);
	quotedPrintableDecodeTest("a=\\n\\rb","a=\n\rb",5,"ab",2);
	quotedPrintableDecodeTest("a=\\r\\n","a=\r\n",4,"a",1);
	quotedPrintableDecodeTest("a=\\r","a=\r",3,"a",1);
	quotedPrintableDecodeTest("a=\\n","a=\n",3,"a",1);
	quotedPrintableDecodeTest("=\\r\\n=41","=\r\n=41",6,"A",1);
	stdoutput.printf("\n");


	// quoted printable decode malformed sequences, a bad pair is dropped
	// and an = with fewer than 2 characters after it ends decoding
	stdoutput.printf("quoted-printable-decode malformed...\n");
	quotedPrintableDecodeTest("=4","=4",2,"",0);
	quotedPrintableDecodeTest("a=4","a=4",3,"a",1);
	quotedPrintableDecodeTest("=","=",1,"",0);
	quotedPrintableDecodeTest("a=","a=",2,"a",1);
	quotedPrintableDecodeTest("=41=","=41=",4,"A",1);
	quotedPrintableDecodeTest("=ZZ","=ZZ",3,"",0);
	quotedPrintableDecodeTest("a=ZZb","a=ZZb",5,"ab",2);
	quotedPrintableDecodeTest("=4Zb","=4Zb",4,"b",1);
	quotedPrintableDecodeTest("=Z4b","=Z4b",4,"b",1);
	quotedPrintableDecodeTest("a= \\r\\nb","a= \r\nb",6,"a\nb",3);
	stdoutput.printf("\n");


	// quoted printable decode stops at a NULL, even within the length
	stdoutput.printf("quoted-printable-decode embedded NULL...\n");
	quotedPrintableDecodeTest("ab\\0cd=41","ab\0cd=41",8,"ab",2);
	quotedPrintableDecodeTest("=\\0A","=\0A",3,"",0);
	quotedPrintableDecodeTest("=4\\0A","=4\0A",4,"",0);
	quotedPrintableDecodeTest("=\\r\\0b","=\r\0b",4,"",0);
	quotedPrintableDecodeTest("\\0a","\0a",2,"",0);
	stdoutput.printf("\n");


	// quoted printable large input, through several chunks of the
	// output * overloads
	stdoutput.printf("quoted-printable large input...\n");
	bigbytes=new byte_t[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigbytes[bi]=(byte_t)(bi%256);
	}
	bytesToCharsConsistencyTest("encode",
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		charstring::quotedPrintableEncode,
		bigbytes,bigsize);
	charstring::quotedPrintableEncode(bigbytes,bigsize,
						&encodedbytes,&encodedlen);
	charsToBytesConsistencyTest("decode",
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		charstring::quotedPrintableDecode,
		encodedbytes,encodedlen,false);
	stringbuffer	qpsb;
	charstring::quotedPrintableDecode(encodedbytes,&qpsb);
	test("decode 3a round trip",
		qpsb.getSize()==bigsize &&
		!bytestring::compare(qpsb.getBuffer(),bigbytes,bigsize));
	stdoutput.printf("\n");


	// quoted printable output * overloads stop at the first failed or
	// short write
	stdoutput.printf("quoted-printable failed writes...\n");
	limitedbuffer	qpshortenc(300,false);
	charstring::quotedPrintableEncode(bigbytes,bigsize,&qpshortenc);
	test("encode short write",
		qpshortenc.charwrites==2 && !qpshortenc.bytewrites &&
		qpshortenc.getSize()==300 &&
		!bytestring::compare(qpshortenc.getBuffer(),encodedbytes,300));
	limitedbuffer	qperrorenc(300,true);
	charstring::quotedPrintableEncode(bigbytes,bigsize,&qperrorenc);
	test("encode error",
		qperrorenc.charwrites==2 && !qperrorenc.bytewrites &&
		qperrorenc.getSize()==256 &&
		!bytestring::compare(qperrorenc.getBuffer(),encodedbytes,256));
	limitedbuffer	qpfirstenc(0,true);
	charstring::quotedPrintableEncode(bigbytes,bigsize,&qpfirstenc);
	test("encode error on first write",
		qpfirstenc.charwrites==1 && !qpfirstenc.bytewrites &&
		!qpfirstenc.getSize());
	limitedbuffer	qpshortdec(300,false);
	charstring::quotedPrintableDecode(encodedbytes,encodedlen,&qpshortdec);
	test("decode short write",
		qpshortdec.bytewrites==2 && !qpshortdec.charwrites &&
		qpshortdec.getSize()==300 &&
		!bytestring::compare(qpshortdec.getBuffer(),bigbytes,300));
	limitedbuffer	qperrordec(300,true);
	charstring::quotedPrintableDecode(encodedbytes,encodedlen,&qperrordec);
	test("decode error",
		qperrordec.bytewrites==2 && !qperrordec.charwrites &&
		qperrordec.getSize()==256 &&
		!bytestring::compare(qperrordec.getBuffer(),bigbytes,256));
	limitedbuffer	qpfirstdec(0,true);
	charstring::quotedPrintableDecode(encodedbytes,encodedlen,&qpfirstdec);
	test("decode error on first write",
		qpfirstdec.bytewrites==1 && !qpfirstdec.charwrites &&
		!qpfirstdec.getSize());
	delete[] bigbytes;
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// escape overloads, called directly
	stdoutput.printf("escape overloads...\n");
	const char	*escin="it's \"x\"\\";
	uint64_t	escinlen=charstring::getLength(escin);
	const char	*escchars="\"'";
	const char	*escout="it\\'s \\\"x\\\"\\\\";
	uint64_t	escoutlen=charstring::getLength(escout);
	char		*escresult;
	uint64_t	escresultlen;
	escresult=charstring::escape(escin,escchars);
	test("1a",!charstring::compare(escresult,escout));
	delete[] escresult;
	escresult=charstring::escape(escin,escinlen,escchars);
	test("1b",!charstring::compare(escresult,escout));
	delete[] escresult;
	charstring::escape(escin,&escresult,&escresultlen,escchars);
	test("2a",escresultlen==escoutlen &&
			!charstring::compare(escresult,escout));
	delete[] escresult;
	charstring::escape(escin,escinlen,&escresult,&escresultlen,escchars);
	test("2b",escresultlen==escoutlen &&
			!charstring::compare(escresult,escout));
	delete[] escresult;
	stringbuffer	escsb;
	charstring::escape(escin,&escsb,escchars);
	test("3a",escsb.getSize()==escoutlen &&
			!charstring::compare(escsb.getString(),escout));
	escsb.clear();
	charstring::escape(escin,escinlen,&escsb,escchars);
	test("3b",escsb.getSize()==escoutlen &&
			!charstring::compare(escsb.getString(),escout));
	escsb.clear();
	charstring::escape(escin,(output *)NULL,escchars);
	charstring::escape(escin,escinlen,(output *)NULL,escchars);
	charstring::escape((const char *)NULL,&escsb,escchars);
	charstring::escape((const char *)NULL,escinlen,&escsb,escchars);
	test("3a/3b NULL input",!escsb.getSize());
	const char	*escnullout="it's \"x\"\\\\";
	escresult=charstring::escape(escin,escinlen,(const char *)NULL);
	test("1b NULL characters",!charstring::compare(escresult,escnullout));
	delete[] escresult;
	charstring::escape(escin,escinlen,&escsb,(const char *)NULL);
	test("3b NULL characters",
			!charstring::compare(escsb.getString(),escnullout));
	escsb.clear();
	stdoutput.printf("\n");


	// unescape overloads, called directly
	stdoutput.printf("unescape overloads...\n");
	escresult=charstring::unescape(escout);
	test("1a",!charstring::compare(escresult,escin));
	delete[] escresult;
	escresult=charstring::unescape(escout,escoutlen);
	test("1b",!charstring::compare(escresult,escin));
	delete[] escresult;
	charstring::unescape(escout,&escresult,&escresultlen);
	test("2a",escresultlen==escinlen &&
			!charstring::compare(escresult,escin));
	delete[] escresult;
	charstring::unescape(escout,escoutlen,&escresult,&escresultlen);
	test("2b",escresultlen==escinlen &&
			!charstring::compare(escresult,escin));
	delete[] escresult;
	charstring::unescape(escout,&escsb);
	test("3a",escsb.getSize()==escinlen &&
			!charstring::compare(escsb.getString(),escin));
	escsb.clear();
	charstring::unescape(escout,escoutlen,&escsb);
	test("3b",escsb.getSize()==escinlen &&
			!charstring::compare(escsb.getString(),escin));
	escsb.clear();
	charstring::unescape(escout,(output *)NULL);
	charstring::unescape(escout,escoutlen,(output *)NULL);
	charstring::unescape((const char *)NULL,&escsb);
	charstring::unescape((const char *)NULL,escoutlen,&escsb);
	test("3a/3b NULL input",!escsb.getSize());
	stdoutput.printf("\n");


	// escape
	stdoutput.printf("escape...\n");
	escapeTest("NULL",NULL,zero,"x",NULL,0);
	escapeTest("NULL, size 3",NULL,3,"x",NULL,0);
	escapeTest("NULL, NULL characters",NULL,zero,(const char *)NULL,NULL,0);
	escapeTest("empty","",zero,"\"'","",0);
	escapeTest("empty, NULL characters","",zero,(const char *)NULL,"",0);
	escapeTest("abc, size 0","abc",zero,"abc","",0);
	escapeTest("a\"b'c\\d","a\"b'c\\d",7,"\"'","a\\\"b\\'c\\\\d",10);
	escapeTest("abc, characters abc","abc",3,"abc","\\a\\b\\c",6);
	escapeTest("\"\"''","\"\"''",4,"\"'","\\\"\\\"\\'\\'",8);
	escapeTest("\\xe9\\xff, characters \\xe9","\xe9\xff",2,"\xe9",
							"\\\xe9\xff",3);
	stdoutput.printf("\n");


	// escape always escapes a backslash, even with NULL characters, and
	// only once when characters has one too
	stdoutput.printf("escape backslashes...\n");
	escapeTest("a\"b\\, NULL characters","a\"b\\",4,(const char *)NULL,
							"a\"b\\\\",5);
	escapeTest("ab\\","ab\\",3,"x","ab\\\\",4);
	escapeTest("a\\\\b","a\\\\b",4,"x","a\\\\\\\\b",6);
	escapeTest("\\","\\",1,"x","\\\\",2);
	escapeTest("a\\b, characters \\","a\\b",3,"\\","a\\\\b",4);
	stdoutput.printf("\n");


	// escape treats a NULL within the length as data, and escapes it too
	// unless characters is NULL
	stdoutput.printf("escape embedded NULL...\n");
	escapeTest("a\\0b","a\0b",3,"x","a\\\0b",4);
	escapeTest("a\\0b, empty characters","a\0b",3,"","a\\\0b",4);
	escapeTest("a\\0b, NULL characters","a\0b",3,(const char *)NULL,
							"a\0b",3);
	escapeTest("a\\0b\\\\c, NULL characters","a\0b\\c",5,
					(const char *)NULL,"a\0b\\\\c",6);
	escapeTest("\\0","\0",1,"x","\\\0",2);
	escapeTest("ab\\0","ab\0",3,"x","ab\\\0",4);
	stdoutput.printf("\n");


	// unescape
	stdoutput.printf("unescape...\n");
	unescapeTest("NULL",NULL,zero,NULL,0);
	unescapeTest("NULL, size 3",NULL,3,NULL,0);
	unescapeTest("empty","",zero,"",0);
	unescapeTest("a\\\\b, size 0","a\\b",zero,"",0);
	unescapeTest("a\\\"b\\\\c","a\\\"b\\\\c",7,"a\"b\\c",5);
	unescapeTest("abc","abc",3,"abc",3);
	unescapeTest("a\\0b\\n","a\\0b\\n",6,"a0bn",4);
	unescapeTest("\\xe9\\\\\\xff","\xe9\\\xff",3,"\xe9\xff",2);
	stdoutput.printf("\n");


	// unescape drops a trailing lone backslash
	stdoutput.printf("unescape backslashes...\n");
	unescapeTest("ab\\","ab\\",3,"ab",2);
	unescapeTest("a\\\\b","a\\\\b",4,"a\\b",3);
	unescapeTest("\\\\","\\\\",2,"\\",1);
	unescapeTest("\\\\\\","\\\\\\",3,"\\",1);
	unescapeTest("\\","\\",1,"",0);
	stdoutput.printf("\n");


	// unescape treats a NULL within the length as data
	stdoutput.printf("unescape embedded NULL...\n");
	unescapeTest("a\\0b\\\\\\0c","a\0b\\\0c",6,"a\0b\0c",5);
	unescapeTest("\\\\\\0a","\\\0a",3,"\0a",2);
	unescapeTest("a\\0\\\\","a\0\\",3,"a\0",2);
	stdoutput.printf("\n");


	// escape and unescape large input, through several chunks of the
	// output * overloads
	stdoutput.printf("escape/unescape large input...\n");
	char	*bigchars=new char[bigsize];
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigchars[bi]=(char)(bi%255+1);
	}
	charsToCharsWithCharactersConsistencyTest("escape",
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			bigchars,bigsize,escchars);
	charstring::escape(bigchars,bigsize,&encodedbytes,&encodedlen,escchars);
	charsToCharsConsistencyTest("unescape",
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			encodedbytes,encodedlen);
	charstring::unescape(encodedbytes,&escsb);
	test("unescape 3a round trip",
		escsb.getSize()==bigsize &&
		!bytestring::compare(escsb.getBuffer(),bigchars,bigsize));
	escsb.clear();
	stdoutput.printf("\n");


	// escape and unescape output * overloads stop at the first failed or
	// short write
	stdoutput.printf("escape/unescape failed writes...\n");
	limitedbuffer	escshort(300,false);
	charstring::escape(bigchars,bigsize,&escshort,escchars);
	test("escape short write",
		escshort.charwrites==2 && !escshort.bytewrites &&
		escshort.getSize()==300 &&
		!bytestring::compare(escshort.getBuffer(),encodedbytes,300));
	limitedbuffer	escerror(300,true);
	charstring::escape(bigchars,bigsize,&escerror,escchars);
	test("escape error",
		escerror.charwrites==2 && !escerror.bytewrites &&
		escerror.getSize()==256 &&
		!bytestring::compare(escerror.getBuffer(),encodedbytes,256));
	limitedbuffer	escfirst(0,true);
	charstring::escape(bigchars,bigsize,&escfirst,escchars);
	test("escape error on first write",
		escfirst.charwrites==1 && !escfirst.bytewrites &&
		!escfirst.getSize());
	limitedbuffer	unescshort(300,false);
	charstring::unescape(encodedbytes,encodedlen,&unescshort);
	test("unescape short write",
		unescshort.charwrites==2 && !unescshort.bytewrites &&
		unescshort.getSize()==300 &&
		!bytestring::compare(unescshort.getBuffer(),bigchars,300));
	limitedbuffer	unescerror(300,true);
	charstring::unescape(encodedbytes,encodedlen,&unescerror);
	test("unescape error",
		unescerror.charwrites==2 && !unescerror.bytewrites &&
		unescerror.getSize()==256 &&
		!bytestring::compare(unescerror.getBuffer(),bigchars,256));
	limitedbuffer	unescfirst(0,true);
	charstring::unescape(encodedbytes,encodedlen,&unescfirst);
	test("unescape error on first write",
		unescfirst.charwrites==1 && !unescfirst.bytewrites &&
		!unescfirst.getSize());
	delete[] encodedbytes;
	stdoutput.printf("\n");


	// escape and unescape large input with NULLs, through the
	// input-and-size overloads
	stdoutput.printf("escape/unescape large input with NULLs...\n");
	for (uint64_t bi=0; bi<bigsize; bi++) {
		bigchars[bi]=(char)(bi%256);
	}
	charsToCharsWithCharactersConsistencyTest("escape",
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			charstring::escape,charstring::escape,
			bigchars,bigsize,escchars);
	charstring::escape(bigchars,bigsize,&encodedbytes,&encodedlen,escchars);
	charsToCharsConsistencyTest("unescape",
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			charstring::unescape,charstring::unescape,
			encodedbytes,encodedlen);
	charstring::unescape(encodedbytes,encodedlen,&escsb);
	test("unescape 3b round trip",
		escsb.getSize()==bigsize &&
		!bytestring::compare(escsb.getBuffer(),bigchars,bigsize));
	escsb.clear();
	delete[] encodedbytes;
	delete[] bigchars;
	stdoutput.printf("\n");

	// encoders given a length too large for the result to fit in a size_t
	stdoutput.printf("encode oversized input...\n");
	uint64_t	maxlen=(uint64_t)-1;
	oversizedCharsTest("urlEncode -1",
			charstring::urlEncode,charstring::urlEncode,maxlen);
	oversizedCharsTest("urlEncode first failing",
			charstring::urlEncode,charstring::urlEncode,
			(uint64_t)6148914691236517205ULL);
	oversizedBytesTest("hexEncode -1",
			charstring::hexEncode,charstring::hexEncode,maxlen);
	oversizedBytesTest("hexEncode first failing",
			charstring::hexEncode,charstring::hexEncode,
			(((uint64_t)1)<<63));
	oversizedBytesTest("base64Encode -1",
			charstring::base64Encode,charstring::base64Encode,
			maxlen);
	oversizedBytesTest("base64Encode first failing",
			charstring::base64Encode,charstring::base64Encode,
			(uint64_t)13835058055282163710ULL);

	// htmlEncode, jsonEncode, and escape read the input to count the
	// output, after a lower-bound check on the length.  These lengths, the
	// only ones that fail that check, are the only ones that can be tested
	// without reading memory.
	oversizedCharsTest("htmlEncode -1",
			charstring::htmlEncode,charstring::htmlEncode,maxlen);
	oversizedCharsTest("jsonEncode -1",
			charstring::jsonEncode,charstring::jsonEncode,maxlen);
	oversizedCharsTest("jsonEncode -2",
			charstring::jsonEncode,charstring::jsonEncode,
			maxlen-1);
	oversizedCharsTest("jsonEncode -3",
			charstring::jsonEncode,charstring::jsonEncode,
			maxlen-2);
	oversizedEscapeTest("escape -1",maxlen,"\"'");

	// a NULL input is handled before the length is checked
	char		*nullhtml=charstring::htmlEncode(NULL,maxlen);
	test("htmlEncode NULL, -1 1b",nullhtml && !nullhtml[0]);
	delete[] nullhtml;
	char		*nullout=NULL;
	uint64_t	nulllen=1;
	charstring::htmlEncode(NULL,maxlen,&nullout,&nulllen);
	test("htmlEncode NULL, -1 2b",
			nullout && !nullout[0] && !nulllen);
	delete[] nullout;
	char		*nulljson=charstring::jsonEncode(NULL,maxlen);
	test("jsonEncode NULL, -1 1b",
			nulljson && !charstring::compare(nulljson,"null"));
	delete[] nulljson;
	nullout=NULL;
	nulllen=0;
	charstring::jsonEncode(NULL,maxlen,&nullout,&nulllen);
	test("jsonEncode NULL, -1 2b",nullout &&
			!charstring::compare(nullout,"null") && nulllen==4);
	delete[] nullout;
	stdoutput.printf("\n");

	// decoders given a length too large for the buffer size to fit in a
	// size_t, which for all of them is the same as the length+1 not
	// fitting: only (uint64_t)-1 fails and it is the only length that can
	// be tested without reading memory
	stdoutput.printf("decode oversized input...\n");
	oversizedCharsTest("urlDecode -1",
			charstring::urlDecode,charstring::urlDecode,maxlen);
	oversizedCharsTest("htmlDecode -1",
			charstring::htmlDecode,charstring::htmlDecode,maxlen);
	oversizedCharsTest("jsonDecode -1",
			charstring::jsonDecode,charstring::jsonDecode,maxlen);
	oversizedCharsTest("unescape -1",
			charstring::unescape,charstring::unescape,maxlen);

	// the second size of base64Decode and hexDecode cannot fail: n/2+1
	// is smaller than n+1, and groups*3+1 needs a length past 2^64
	oversizedDecodeTest("hexDecode -1",
			charstring::hexDecode,charstring::hexDecode,maxlen);
	oversizedDecodeTest("base64Decode -1",
			charstring::base64Decode,charstring::base64Decode,
			maxlen);
	oversizedDecodeTest("quotedPrintableDecode -1",
			charstring::quotedPrintableDecode,
			charstring::quotedPrintableDecode,maxlen);
	oversizedBytesTest("quotedPrintableEncode -1",
			charstring::quotedPrintableEncode,
			charstring::quotedPrintableEncode,maxlen);

	// a NULL input is handled before the length is checked
	nullCharsTest("urlDecode NULL, -1",
			charstring::urlDecode,charstring::urlDecode,maxlen);
	nullCharsTest("jsonDecode NULL, -1",
			charstring::jsonDecode,charstring::jsonDecode,maxlen);
	nullCharsTest("unescape NULL, -1",
			charstring::unescape,charstring::unescape,maxlen);
	nullDecodeTest("base64Decode NULL, -1",
			charstring::base64Decode,charstring::base64Decode,
			maxlen);
	nullDecodeTest("hexDecode NULL, -1",
			charstring::hexDecode,charstring::hexDecode,maxlen);
	nullDecodeTest("quotedPrintableDecode NULL, -1",
			charstring::quotedPrintableDecode,
			charstring::quotedPrintableDecode,maxlen);
	char		*nullhtmldecode=charstring::htmlDecode(NULL,maxlen);
	test("htmlDecode NULL, -1 1b",nullhtmldecode && !nullhtmldecode[0]);
	delete[] nullhtmldecode;
	nullout=NULL;
	nulllen=1;
	charstring::htmlDecode(NULL,maxlen,&nullout,&nulllen);
	test("htmlDecode NULL, -1 2b",
			nullout && !nullout[0] && !nulllen);
	delete[] nullout;
	stdoutput.printf("\n");


	// pad, insertString, and the length forms of duplicate
	stdoutput.printf("pad/insertString/duplicate length...\n");
	padTest("pad left","abc",'*',-1,6,"***abc");
	padTest("pad right","abc",'*',1,6,"abc***");
	padTest("pad center even","abc",'*',0,7,"**abc**");
	padTest("pad center odd","abc",'*',0,6,"*abc**");
	padTest("pad exact length","abc",'*',-1,3,"abc");
	padTest("pad truncate","abcdef",'*',-1,3,"abc");
	padTest("pad empty left","",'.',-1,4,"....");
	padTest("pad NULL left",NULL,'.',-1,4,"....");
	padTest("pad NULL right",NULL,'.',1,4,"....");
	padTest("pad zero length","abc",'*',1,0,NULL);
	padTest("pad NULL zero length",NULL,'*',1,0,NULL);
	insertStringTest("insertString start","hello","XY",0,"XYhello");
	insertStringTest("insertString middle","hello","XY",2,"heXYllo");
	insertStringTest("insertString end","hello","XY",5,"helloXY");
	insertStringTest("insertString empty dest","","XY",0,"XY");
	insertStringTest("insertString one","hello","-",3,"hel-lo");

	// empty and NULL arguments and out-of-range indices; an alarm turns
	// a hang into a failure
	signalmanager::alarm(60);
	const char	*isdest="abc";
	for (uint64_t i=0; i<=6; i++) {
		stringbuffer	name;
		name.append("insertString empty src, index ");
		name.append(i);
		insertStringExactTest(name.getString(),isdest,"",i,isdest);
		name.clear();
		name.append("insertString src, index ");
		name.append(i);
		const char	*expected[]={
			"XYabc","aXYbc","abXYc","abcXY",
			"abcXY","abcXY","abcXY"
		};
		insertStringExactTest(name.getString(),isdest,"XY",i,
							expected[i]);
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

	// pad and the duplicate forms given a length too large for the
	// buffer size to fit in a size_t, with a zero-byte input buffer
	// (AddressSanitizer reports any read).  This relies on the size check
	// coming before any read or write.
	stdoutput.printf("pad/duplicate oversized length...\n");
	size_t		none=0;
	char		*zin=new char[none];
	wchar_t		*zwin=(wchar_t *)new char[none];
	ucs2_t		*zuin=(ucs2_t *)new char[none];
	size_t		maxsize=(size_t)-1;
	char		*oversized=charstring::pad(zin,'*',-1,maxlen);
	test("pad -1 left",!oversized);
	delete[] oversized;
	oversized=charstring::pad(zin,'*',0,maxlen);
	test("pad -1 center",!oversized);
	delete[] oversized;
	oversized=charstring::pad(zin,'*',1,maxlen);
	test("pad -1 right",!oversized);
	delete[] oversized;
	const char	*nullstr=NULL;
	oversized=charstring::pad(nullstr,'*',1,maxlen);
	test("pad NULL, -1",!oversized);
	delete[] oversized;
	oversized=charstring::duplicate(zin,maxsize);
	test("duplicate -1 char",!oversized);
	delete[] oversized;
	oversized=charstring::duplicate(zwin,maxsize);
	test("duplicate -1 wchar_t",!oversized);
	delete[] oversized;
	oversized=charstring::duplicate(zwin,maxsize,'#');
	test("duplicate -1 wchar_t replacement",!oversized);
	delete[] oversized;
	oversized=charstring::duplicateUcs2(zuin,maxsize);
	test("duplicateUcs2 -1",!oversized);
	delete[] oversized;
	oversized=charstring::duplicateUcs2(zuin,maxsize,true);
	test("duplicateUcs2 -1 bigendian",!oversized);
	delete[] oversized;
	oversized=charstring::duplicateUcs2(zuin,maxsize,'#');
	test("duplicateUcs2 -1 replacement",!oversized);
	delete[] oversized;
	oversized=charstring::duplicateUcs2(zuin,maxsize,'#',false);
	test("duplicateUcs2 -1 replacement bigendian",!oversized);
	delete[] oversized;

	// a NULL input is handled before the length is checked
	const wchar_t	*nullwstr=NULL;
	const ucs2_t	*nullustr=NULL;
	test("duplicate NULL, -1 char",!charstring::duplicate(nullstr,maxsize));
	test("duplicate NULL, -1 wchar_t",
			!charstring::duplicate(nullwstr,maxsize));
	test("duplicateUcs2 NULL, -1",
			!charstring::duplicateUcs2(nullustr,maxsize));
	delete[] zin;
	delete[] (char *)zwin;
	delete[] (char *)zuin;

	// insertString sizes its result from the lengths of real strings, so
	// a size that does not fit in a size_t needs a string larger than the
	// address space and cannot be tested here
	stdoutput.printf("\n");

	const char	*alphabet=
		"aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
	test("lengthContainingSet",
		charstring::getLengthContainingSet(
				alphabet,"gfedcba")==14);
	test("lengthNotContainingSet",
		charstring::getLengthNotContainingSet(
				alphabet,"hijklmnopqrstuvwxyz")==14);
	test("findFirstOfSet",charstring::findFirstOfSet(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd",
			charstring::findFirstOfSetOrEnd(alphabet,"klm")==
								(alphabet+20));
	test("findFirstOfSetOrEnd (end)",
			charstring::findFirstOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::getLength(alphabet)));
	test("findLastOfSetOrEnd",
			charstring::findLastOfSetOrEnd(alphabet,"abc")==
								(alphabet+5));
	test("findLastOfSetOrEnd (end)",
			charstring::findLastOfSetOrEnd(alphabet,"123")==
				(alphabet+charstring::getLength(alphabet)));
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
			charstring::getIntegerLength((int64_t)0)==1);
	test("integerLength: 1",
			charstring::getIntegerLength((int64_t)1)==1);
	test("integerLength: 10",
			charstring::getIntegerLength((int64_t)10)==2);
	test("integerLength: 100",
			charstring::getIntegerLength((int64_t)100)==3);
	test("integerLength: 1000",
			charstring::getIntegerLength((int64_t)1000)==4);
	test("integerLength: -1",
			charstring::getIntegerLength((int64_t)-1)==2);
	test("integerLength: -10",
			charstring::getIntegerLength((int64_t)-10)==3);
	test("integerLength: -100",
			charstring::getIntegerLength((int64_t)-100)==4);
	test("integerLength: -1000",
			charstring::getIntegerLength((int64_t)-1000)==5);
	test("integerLength: 0",
			charstring::getIntegerLength((uint64_t)0)==1);
	test("integerLength: 1",
			charstring::getIntegerLength((uint64_t)1)==1);
	test("integerLength: 10",
			charstring::getIntegerLength((uint64_t)10)==2);
	test("integerLength: 100",
			charstring::getIntegerLength((uint64_t)100)==3);
	test("integerLength: 1000",
			charstring::getIntegerLength((uint64_t)1000)==4);

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

	// the whole match gets replaced, not one of the capture groups
	original="xayb";
	from.setPattern("(a)(b)?");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (capture groups)",
		!charstring::compare(newstr,"xZyb"));
	delete[] newstr;

	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	// \w is a PCRE shorthand class with no POSIX regcomp equivalent,
	// so the POSIX engines can't match it at all
	original="ab cd";
	from.setPattern("(\\w)(\\w)");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (whole match)",
		!charstring::compare(newstr,"Z Z"));
	delete[] newstr;
	#endif

	// backrefs - swap two groups, s/([a-z]+) ([a-z]+)/\2 \1/
	original="hello world";
	from.setPattern("([a-z]+) ([a-z]+)");
	newstr=charstring::replace(original,&from,"\\2 \\1",true);
	test("replace regex/from-to (swap groups)",
		!charstring::compare(newstr,"world hello"));
	delete[] newstr;

	// \0 is the whole match
	original="a1b2";
	from.setPattern("[0-9]");
	newstr=charstring::replace(original,&from,"<\\0>",true);
	test("replace regex/from-to (whole match backref)",
		!charstring::compare(newstr,"a<1>b<2>"));
	delete[] newstr;

	// a backref alongside literal text, more than once each
	original="2026-08-05";
	from.setPattern("([0-9]{4})-([0-9]{2})-([0-9]{2})");
	newstr=charstring::replace(original,&from,"\\2/\\3/\\1 (\\1)",false);
	test("replace regex/from-to (backrefs and literals)",
		!charstring::compare(newstr,"08/05/2026 (2026)"));
	delete[] newstr;

	// a group that didn't participate expands to nothing
	original="xayb";
	from.setPattern("(a)(b)?");
	newstr=charstring::replace(original,&from,"[\\1|\\2]",true);
	test("replace regex/from-to (non-participating group)",
		!charstring::compare(newstr,"x[a|]yb"));
	delete[] newstr;

	// ...and so does a backref past the group count
	original="ab";
	from.setPattern("(a)");
	newstr=charstring::replace(original,&from,"[\\1|\\5]",true);
	test("replace regex/from-to (backref past the count)",
		!charstring::compare(newstr,"[a|]b"));
	delete[] newstr;

	// a doubled backslash is one backslash, and an escape this doesn't
	// know keeps both characters
	original="ab";
	from.setPattern("a");
	newstr=charstring::replace(original,&from,"\\\\\\1\\q\\",true);
	test("replace regex/from-to (backslashes)",
		!charstring::compare(newstr,"\\\\q\\b"));
	delete[] newstr;

	// a replacement with no backrefs at all is untouched
	original="a.b";
	from.setPattern("\\.");
	newstr=charstring::replace(original,&from,"---",true);
	test("replace regex/from-to (no backrefs)",
		!charstring::compare(newstr,"a---b"));
	delete[] newstr;

	// \g<n> names a group by number, just like \n does
	original="hello world";
	from.setPattern("([a-z]+) ([a-z]+)");
	newstr=charstring::replace(original,&from,"\\g<2> \\g<1>",true);
	test("replace regex/from-to (swap groups by number)",
		!charstring::compare(newstr,"world hello"));
	delete[] newstr;

	// ...but it reaches the groups past the ninth, which a single
	// digit can't name
	original="abcdefghijkl";
	from.setPattern("([a-z])([a-z])([a-z])([a-z])([a-z])([a-z])"
			"([a-z])([a-z])([a-z])([a-z])([a-z])([a-z])");
	newstr=charstring::replace(original,&from,
					"\\g<10>\\g<11>\\g<12>",true);
	test("replace regex/from-to (groups past the ninth)",
		!charstring::compare(newstr,"jkl"));
	delete[] newstr;

	// which is why the digit form doesn't get greedy about digits -
	// \12 is still group 1 and then a literal 2, not group 12
	newstr=charstring::replace(original,&from,"\\12",true);
	test("replace regex/from-to (digit form isn't greedy)",
		!charstring::compare(newstr,"a2"));
	delete[] newstr;

	// \g<0> is the whole match
	original="a1b2";
	from.setPattern("[0-9]");
	newstr=charstring::replace(original,&from,"<\\g<0>>",true);
	test("replace regex/from-to (whole match by number)",
		!charstring::compare(newstr,"a<1>b<2>"));
	delete[] newstr;

	// a numbered group past the group count expands to nothing
	original="ab";
	from.setPattern("(a)");
	newstr=charstring::replace(original,&from,"[\\g<1>|\\g<5>]",true);
	test("replace regex/from-to (number past the count)",
		!charstring::compare(newstr,"[a|]b"));
	delete[] newstr;

	// ...and so does a run of digits too long to fit in the index,
	// which stops growing rather than overflowing
	newstr=charstring::replace(original,&from,
					"[\\g<99999999999999>]",true);
	test("replace regex/from-to (long digit run)",
		!charstring::compare(newstr,"[]b"));
	delete[] newstr;

	// a group that didn't participate expands to nothing too
	original="xayb";
	from.setPattern("(a)(b)?");
	newstr=charstring::replace(original,&from,"[\\g<1>|\\g<2>]",true);
	test("replace regex/from-to (non-participating by number)",
		!charstring::compare(newstr,"x[a|]yb"));
	delete[] newstr;

	// an unterminated \g< keeps its characters as they were
	original="ab";
	from.setPattern("(a)");
	newstr=charstring::replace(original,&from,"\\g<12",true);
	test("replace regex/from-to (unterminated)",
		!charstring::compare(newstr,"\\g<12b"));
	delete[] newstr;

	newstr=charstring::replace(original,&from,"\\g<",true);
	test("replace regex/from-to (unterminated, no body)",
		!charstring::compare(newstr,"\\g<b"));
	delete[] newstr;

	// a \g that isn't the start of a \g< is just an escape this
	// doesn't know, so both characters are kept
	newstr=charstring::replace(original,&from,"\\g1",true);
	test("replace regex/from-to (bare backslash-g)",
		!charstring::compare(newstr,"\\g1b"));
	delete[] newstr;

	// an empty body and a body that isn't all digits are both group
	// names, and neither names a group here
	newstr=charstring::replace(original,&from,"[\\g<>|\\g<1a>]",true);
	test("replace regex/from-to (empty and mixed bodies)",
		!charstring::compare(newstr,"[|]b"));
	delete[] newstr;

	#if defined(RUDIMENTS_HAS_PCRE2) || \
		(defined(RUDIMENTS_HAS_PCRE) && \
			defined(RUDIMENTS_HAS_PCRE_GET_STRINGNUMBER))
	// a \g<name> is resolved with getSubstringIndex(), which only PCRE2
	// and a PCRE with pcre_get_stringnumber (4.0+) can answer - the other
	// engines have no named capture groups, so a pattern that declares
	// one won't compile there at all.  (?<name>...) needs PCRE 7.0, but
	// (?P<name>...) works in every engine that has named groups.
	original="2026-08-05";
	regularexpression	probe;
	if (probe.setPattern("(?<n>x)")) {
		from.setPattern("(?<year>[0-9]{4})-(?<month>[0-9]{2})-"
				"(?<day>[0-9]{2})");
	} else {
		stdoutput.printf("\t(?<name>...) syntax: skipped, "
				"using (?P<name>...)\n");
		from.setPattern("(?P<year>[0-9]{4})-(?P<month>[0-9]{2})-"
				"(?P<day>[0-9]{2})");
	}
	newstr=charstring::replace(original,&from,
				"\\g<month>/\\g<day>/\\g<year>",false);
	test("replace regex/from-to (named groups)",
		!charstring::compare(newstr,"08/05/2026"));
	delete[] newstr;

	// a name the pattern doesn't have expands to nothing
	newstr=charstring::replace(original,&from,
					"[\\g<year>|\\g<era>]",false);
	test("replace regex/from-to (unknown name)",
		!charstring::compare(newstr,"[2026|]"));
	delete[] newstr;
	#else
	// the POSIX engines, and a PCRE older than 4.0, have no named groups
	// at all, so every name is an unknown one and expands to nothing
	original="2026-08-05";
	from.setPattern("([0-9]{4})-([0-9]{2})-([0-9]{2})");
	newstr=charstring::replace(original,&from,
					"[\\g<1>|\\g<year>]",false);
	test("replace regex/from-to (unknown name)",
		!charstring::compare(newstr,"[2026|]"));
	delete[] newstr;
	#endif

	// matching from an offset rather than from a pointer into the
	// middle of the subject, so ^ only matches at the real start
	original="aaa";
	from.setPattern("^a");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (anchored)",
		!charstring::compare(newstr,"Zaa"));
	delete[] newstr;

	#if defined(RUDIMENTS_HAS_PCRE2) || defined(RUDIMENTS_HAS_PCRE)
	// ...and a lookbehind can see what precedes the resume point,
	// which only the PCRE engines have
	original="xaya";
	from.setPattern("(?<=x)a");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (lookbehind)",
		!charstring::compare(newstr,"xZya"));
	delete[] newstr;

	// a word boundary is computed with the preceding character in hand
	original="aa";
	from.setPattern("\\ba");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (word boundary)",
		!charstring::compare(newstr,"Za"));
	delete[] newstr;
	#elif defined(__GLIBC__)
	// glibc's regcomp() supports \b as a GNU extension, but computes
	// a word boundary as though the subject began at the resume
	// point, so the second "a" looks like one too
	original="aa";
	from.setPattern("\\ba");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (word boundary)",
		!charstring::compare(newstr,"ZZ"));
	delete[] newstr;
	#else
	// \b is a GNU extension, not part of POSIX ERE at all, and
	// what a non-glibc regcomp() does with a backslash it doesn't
	// recognize is undefined - there's no specific behavior to
	// assert here
	#endif

	// an empty match is skipped rather than replaced, so a pattern
	// that can match nothing still terminates
	original="abc";
	from.setPattern("x*");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (empty match)",
		!charstring::compare(newstr,"abc"));
	delete[] newstr;

	// an empty match can also come back from well past the resume
	// point, and is skipped from where it was found rather than from
	// the resume point
	original="abc";
	from.setPattern("$");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (empty match ahead)",
		!charstring::compare(newstr,"abc"));
	delete[] newstr;

	// degenerate inputs
	original="";
	from.setPattern("a");
	newstr=charstring::replace(original,&from,"Z",true);
	test("replace regex/from-to (empty string)",
		!charstring::compare(newstr,""));
	delete[] newstr;

	test("replace regex/from-to (NULL string)",
		!charstring::replace((const char *)NULL,&from,"Z",true));

	original="ab";
	newstr=charstring::replace(original,&from,NULL,true);
	test("replace regex/from-to (NULL replacement)",
		!charstring::compare(newstr,"b"));
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
	stdoutput.printf("getHumanReadable...\n");
	long double	val=2.1*1024.0*1024.0*1024.0*1024.0*
				1024.0*1024.0*1024.0*1024.0;
	char	*sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1B",!charstring::compare(sval,"2.1B"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1Y",!charstring::compare(sval,"2.1Y"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1Z",!charstring::compare(sval,"2.1Z"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1P",!charstring::compare(sval,"2.1P"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1T",!charstring::compare(sval,"2.1T"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1G",!charstring::compare(sval,"2.1G"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1M",!charstring::compare(sval,"2.1M"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1K",!charstring::compare(sval,"2.1K"));
	delete[] sval;
	val/=1024.0;
	sval=charstring::getHumanReadable(val);
	test("getHumanReadable 2.1",!charstring::compare(sval,"2.1"));
	delete[] sval;

	val=2.1*1000.0*1000.0*1000.0*1000.0*
		1000.0*1000.0*1000.0*1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1B",!charstring::compare(sval,"2.1B"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1Y",!charstring::compare(sval,"2.1Y"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1Z",!charstring::compare(sval,"2.1Z"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1P",!charstring::compare(sval,"2.1P"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1T",!charstring::compare(sval,"2.1T"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1G",!charstring::compare(sval,"2.1G"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1M",!charstring::compare(sval,"2.1M"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1K",!charstring::compare(sval,"2.1K"));
	delete[] sval;
	val/=1000.0;
	sval=charstring::getHumanReadable(val,true);
	test("getHumanReadable 2.1",!charstring::compare(sval,"2.1"));
	delete[] sval;

#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	ival=((uint64_t)3)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024)*
				((uint64_t)1024)*((uint64_t)1024);
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3Z",!charstring::compare(sval,"3Z"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3P",!charstring::compare(sval,"3P"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3T",!charstring::compare(sval,"3T"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3G",!charstring::compare(sval,"3G"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3M",!charstring::compare(sval,"3M"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3K",!charstring::compare(sval,"3K"));
	delete[] sval;
	ival/=1024;
	sval=charstring::getHumanReadable(ival);
	test("getHumanReadable 3",!charstring::compare(sval,"3"));
	delete[] sval;

	int64_t	sival=((int64_t)-3)*
			((int64_t)1000)*((int64_t)1000)*((int64_t)1000)*
			((int64_t)1000)*((int64_t)1000)*((int64_t)1000);
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3Z",!charstring::compare(sval,"-3Z"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3P",!charstring::compare(sval,"-3P"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3T",!charstring::compare(sval,"-3T"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3G",!charstring::compare(sval,"-3G"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3M",!charstring::compare(sval,"-3M"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3K",!charstring::compare(sval,"-3K"));
	delete[] sval;
	sival/=1000;
	sval=charstring::getHumanReadable(sival,true);
	test("getHumanReadable -3",!charstring::compare(sval,"-3"));
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
	char	*c=charstring::duplicate(wchars);
	test("from wchar_t",!charstring::compare(c,chars));
	delete[] c;
	c=charstring::duplicateUcs2(ucs2chars);
	test("from ucs2",!charstring::compare(c,chars));
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
		test(*s,charstring::findEndOfQuotedString(*s,
					charstring::getLength(*s),true,true)==
					*s+charstring::getLength(*s)-4);
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
		test(*s,charstring::findEndOfQuotedString(*s,
					charstring::getLength(*s),true,true)==
					*s+charstring::getLength(*s));
	}
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
	charstring::rightTrim((char *)NULL);
	charstring::rightTrim((char *)NULL,(char)'x');
	char	*empty=rangeMake("",1);
	charstring::rightTrim(empty);
	charstring::rightTrim(empty,(char)'x');
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
			!charstring::isInteger((const char *)NULL,-1) &&
			!charstring::isNumber((const char *)NULL,-1));
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
	char	*r=charstring::convertAmount((int64_t)-1,8);
	test("convertAmount -1, 8 spaces",emptyPatternMatches(r,"$  -0.01"));
	delete[] r;
	r=charstring::convertAmount(numberMin,8);
	test("convertAmount minimum, 8 spaces",
		emptyPatternMatches(r,"$-92233720368547758.08"));
	delete[] r;
	}
	test("getIntegerLength INT16_MIN",
		charstring::getIntegerLength((int16_t)(-32767-1))==6);
	test("getIntegerLength INT16_MIN+1",
		charstring::getIntegerLength((int16_t)-32767)==6);
	test("getIntegerLength INT16_MAX",
		charstring::getIntegerLength((int16_t)32767)==5);
	test("getIntegerLength int16 -1",
		charstring::getIntegerLength((int16_t)-1)==2);
	test("getIntegerLength int16 0",
		charstring::getIntegerLength((int16_t)0)==1);
	test("getIntegerLength INT32_MIN",
		charstring::getIntegerLength((int32_t)(-2147483647-1))==11);
	test("getIntegerLength INT32_MIN+1",
		charstring::getIntegerLength((int32_t)-2147483647)==11);
	test("getIntegerLength INT32_MAX",
		charstring::getIntegerLength((int32_t)2147483647)==10);
	test("getIntegerLength int32 -1",
		charstring::getIntegerLength((int32_t)-1)==2);
	test("getIntegerLength int32 0",
		charstring::getIntegerLength((int32_t)0)==1);
	test("getIntegerLength INT64_MIN",
		charstring::getIntegerLength(numberMin)==20);
	test("getIntegerLength INT64_MIN+1",
		charstring::getIntegerLength(numberMin+1)==20);
	test("getIntegerLength INT64_MAX",
		charstring::getIntegerLength(numberMax)==19);
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
	char	*r=charstring::parseNumber((int32_t)(-2147483647-1));
	test("parseNumber int32 INT32_MIN",
		emptyPatternMatches(r,"-2147483648"));
	delete[] r;
	r=charstring::parseNumber((int16_t)(-32767-1));
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
	// hosttonet/nettohost (why are these here?)
	byte_t	v8=0xC0;
	uint16_t	v16=filedescriptor::convertHostToNet(
						(uint16_t)0xC0C0);
	uint32_t	v32=filedescriptor::convertHostToNet(
						(uint32_t)0xC0C0C0C0);
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	uint64_t	v64=filedescriptor::convertHostToNet(
					(uint64_t)0xC0C0C0C0C0C0C0C0ULL);
	#endif
	char	sv8=-1*0x01;
	int16_t	sv16=filedescriptor::convertHostToNet((uint16_t)
						(int16_t)(-1*0x0101));
	int32_t	sv32=filedescriptor::convertHostToNet((uint32_t)
						(int32_t)(-1*0x01010101));
	#ifdef RUDIMENTS_HAVE_LONG_LONG
	int64_t	sv64=filedescriptor::convertHostToNet((uint64_t)
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
	stdoutput.printf("\n");
#endif

	return 0;
}
