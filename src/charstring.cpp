// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/character.h>
#include <rudiments/wcharacter.h>
#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
	#include <rudiments/process.h>
	#include <rudiments/file.h>
#endif
#include <rudiments/stringbuffer.h>
#include <rudiments/error.h>
#include <rudiments/sys.h>

// for strtold and for strchrnul
#ifndef __USE_GNU
	#define __USE_GNU
#endif
#ifndef __USE_ISOC9X
	#define __USE_ISOC9X
#endif

// Redhat 6.2 needs _GNU_SOURCE
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

// include this after the above defines or Haiku can't find strchrnul
#include <rudiments/stdio.h>

// for vsnprintf/vsnprintf_s
#include <stdio.h>

#ifdef RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF
extern "C" int __vsnprintf(char *str, size_t size,
				const char *format, va_list ap);
#endif

// MSVC 2010- doesn't define va_copy
#if defined(_MSC_VER) && (_MSC_VER <= 1700)
	#define va_copy(a,b) ((a)=(b))
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STRINGS_H
	#include <strings.h>
#endif

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

const char *charstring::findLast(const char *haystack, const char *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const char	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compare(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

const char *charstring::findLastIgnoringCase(const char *haystack,
						const char *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const char	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

void charstring::upper(char *str) {
	if (str) {
		for (char *ch=str; *ch; ch++) {
			*ch=character::upper(*ch);
		}
	}
}

void charstring::lower(char *str) {
	if (str) {
		for (char *ch=str; *ch; ch++) {
			*ch=character::lower(*ch);
		}
	}
}

void charstring::capitalize(char *str) {
	if (str) {
		bool	cap=true;
		for (char *ch=str; *ch; ch++) {
			if (cap) {
				*ch=character::upper(*ch);
				cap=false;
			} else {
				*ch=character::lower(*ch);
			}
			if (character::isInSet(*ch," '\"-(")) {
				cap=true;
			}
		}
	}
}

void charstring::rightTrim(char *str, char character) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (*str==character) { 
			str--;
		}
		str++;

		// terminate the string there
		*str='\0';
	}
}

void charstring::leftTrim(char *str, char character) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
		while (str[i]==character) {
			i++;
		}
	
		// swap chars to front of string
		while (str[i]) {
			str[j]=str[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		str[j]='\0';
	}
}

bool charstring::strip(char *str, char character) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (str[index]==character) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]='\0';
	return retval;
}

bool charstring::strip(char *str1, const char *str2) {

	if (!str1 || !str2) {
		return false;
	}

	int32_t	str2len=getLength(str2);
	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str1[index]) {
		if (!compare(str1+index,str2,str2len)) {
			total=total+str2len;
			index=index+str2len;
			retval=true;
		} else {
			if (total) {
				str1[index-total]=str1[index];
			}
			index++;
		}
	}
	str1[index-total]='\0';
	return retval;
}

bool charstring::stripSet(char *str, const char *set) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (character::isInSet(str[index],set)) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]='\0';
	return retval;
}

void charstring::replace(char *str, char oldchar, char newchar) {
	if (str) {
		for (char *ptr=str; *ptr; ptr++) {
			if (*ptr==oldchar) {
				*ptr=newchar;
			}
		}
	}
}

void charstring::replace(char *str, const char *oldchars, char newchar) {
	if (str) {
		for (char *ptr=str; *ptr; ptr++) {
			if (character::isInSet(*ptr,oldchars)) {
				*ptr=newchar;
			}
		}
	}
}

char *charstring::replace(const char *str, const char *oldstr,
						const char *newstr) {
	if (!str) {
		return NULL;
	}
	stringbuffer	newstring;
	size_t		oldstrlen=getLength(oldstr);
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		if (!compare(ptr,oldstr,oldstrlen)) {
			newstring.append(start,ptr-start);
			newstring.append(newstr);
			ptr+=oldstrlen;
			start=ptr;
		} else {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);
	return newstring.detachString();
}

char *charstring::replace(const char *str, const char * const *oldstrset,
						const char * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// count members of oldstrset
	uint64_t	i=0;
	for (const char * const *o=oldstrset; *o; o++) {
		i++;
	}

	// create oldstrlen
	size_t	*oldstrlen=new size_t[i];
	i=0;
	for (const char * const *o=oldstrset; *o; o++) {
		oldstrlen[i]=getLength(*o);
		i++;
	}

	// replace
	char	*result=replace(str,oldstrset,oldstrlen,newstrset);

	// clean up
	delete[] oldstrlen;

	return result;
}

char *charstring::replace(const char *str, const char * const *oldstrset,
						size_t *oldstrlen,
						const char * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// search and replace
	stringbuffer	newstring;
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		bool	found=false;
		uint64_t i=0;
		for (const char * const *oldptr=oldstrset; *oldptr; oldptr++) {
			if (!compare(ptr,oldstrset[i],oldstrlen[i])) {
				newstring.append(start,ptr-start);
				newstring.append(newstrset[i]);
				ptr+=oldstrlen[i];
				start=ptr;
				found=true;
				break;
			}
			i++;
		}
		if (!found) {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);

	return newstring.detachString();
}

void charstring::replaceIgnoringCase(char *str, char oldchar, char newchar) {
	if (str) {
		for (char *ptr=str; *ptr; ptr++) {
			if (character::lower(*ptr)==character::lower(oldchar)) {
				*ptr=newchar;
			}
		}
	}
}

void charstring::replaceIgnoringCase(char *str,
					const char *oldchars,
					char newchar) {
	if (str) {
		for (char *ptr=str; *ptr; ptr++) {
			if (character::isInSetIgnoringCase(*ptr,oldchars)) {
				*ptr=newchar;
			}
		}
	}
}

char *charstring::replaceIgnoringCase(const char *str,
						const char *oldstr,
						const char *newstr) {
	if (!str) {
		return NULL;
	}
	stringbuffer	newstring;
	size_t		oldstrlen=getLength(oldstr);
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		if (!compareIgnoringCase(ptr,oldstr,oldstrlen)) {
			newstring.append(start,ptr-start);
			newstring.append(newstr);
			ptr+=oldstrlen;
			start=ptr;
		} else {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);
	return newstring.detachString();
}

char *charstring::replaceIgnoringCase(const char *str,
					const char * const *oldstrset,
					const char * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// count members of oldstrset
	uint64_t	i=0;
	for (const char * const *o=oldstrset; *o; o++) {
		i++;
	}

	// create oldstrlen
	size_t	*oldstrlen=new size_t[i];
	i=0;
	for (const char * const *o=oldstrset; *o; o++) {
		oldstrlen[i]=getLength(*o);
		i++;
	}

	// replace
	char	*result=replaceIgnoringCase(str,oldstrset,oldstrlen,newstrset);

	// clean up
	delete[] oldstrlen;

	return result;
}

char *charstring::replaceIgnoringCase(const char *str,
					const char * const *oldstrset,
					size_t *oldstrlen,
					const char * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// search and replace
	stringbuffer	newstring;
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		bool	found=false;
		uint64_t i=0;
		for (const char * const *oldptr=oldstrset; *oldptr; oldptr++) {
			if (!compareIgnoringCase(ptr,oldstrset[i],
							oldstrlen[i])) {
				newstring.append(start,ptr-start);
				newstring.append(newstrset[i]);
				ptr+=oldstrlen[i];
				start=ptr;
				found=true;
				break;
			}
			i++;
		}
		if (!found) {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);

	return newstring.detachString();
}

// append the substring a group matched, if any
static void appendSubstring(stringbuffer *newstring,
					regularexpression *from,
					int32_t index) {
	const char	*substart=from->getSubstringStart(index);
	if (substart) {
		newstring->append(substart,
				from->getSubstringEnd(index)-substart);
	}
}

// resolve a \g<...> backref body to a group index
static int32_t resolveGroupIndex(regularexpression *from,
					const char *start,
					const char *end) {

	bool	number=(end>start);
	for (const char *d=start; number && d<end; d++) {
		number=character::isDigit((unsigned char)*d);
	}

	if (!number) {

		// the name lookup needs it null-terminated
		char	*name=charstring::duplicate(start,end-start);
		int32_t	index=from->getSubstringIndex(name);
		delete[] name;
		return index;
	}

	// clamp index growth once it exceeds the group count
	int32_t	count=from->getSubstringCount();
	int32_t	index=0;
	for (const char *d=start; d<end && index<count; d++) {
		index=index*10+(*d-'0');
	}
	return index;
}

// expand backrefs in "to" and append the result to "newstring"
static void appendExpandingBackrefs(stringbuffer *newstring,
					regularexpression *from,
					const char *to) {

	if (!to) {
		return;
	}

	for (const char *ptr=to; *ptr; ptr++) {

		if (*ptr!='\\') {
			newstring->append(*ptr);
			continue;
		}

		// a trailing backslash is just a backslash
		if (!*(ptr+1)) {
			newstring->append(*ptr);
			continue;
		}

		ptr++;

		if (*ptr=='\\') {
			newstring->append('\\');
			continue;
		}

		// handle a \g<...> named or numbered backref
		if (*ptr=='g' && *(ptr+1)=='<') {

			// find the closing >
			const char	*bodystart=ptr+2;
			const char	*bodyend=bodystart;
			while (*bodyend && *bodyend!='>') {
				bodyend++;
			}

			// leave an unterminated form as it was
			if (!*bodyend) {
				newstring->append('\\')->append(*ptr);
				continue;
			}

			appendSubstring(newstring,from,
					resolveGroupIndex(from,
							bodystart,bodyend));

			// step onto the >, which the loop steps past
			ptr=bodyend;
			continue;
		}

		// leave an unknown escape as it was
		if (!character::isDigit((unsigned char)*ptr)) {
			newstring->append('\\')->append(*ptr);
			continue;
		}

		appendSubstring(newstring,from,*ptr-'0');
	}
}

char *charstring::replace(const char *str,
				regularexpression *from,
				const char *to,
				bool global) {
	if (!str) {
		return NULL;
	}

	// declare buffer for new string
	stringbuffer	newstring;

	// Resume with matchNext() rather than a manual offset loop, so
	// anchors like ^, \b, and lookbehind still see the whole subject.
	int32_t	start=0;
	for (bool matched=from->match(str,getLength(str));
			matched; matched=from->matchNext()) {

		// get the bounds of the matching chunk
		int32_t	fromstart=from->getSubstringStartOffset(0);
		int32_t	fromend=from->getSubstringEndOffset(0);

		// pcre1 can report a \K inside a lookahead as a match that
		// ends before it starts; treat that as empty too, and skip it.
		if (fromend<=fromstart) {
			continue;
		}

		// append the previous, non-matching part of the chunk
		newstring.append(str+start,fromstart-start);

		// append the replacement part
		appendExpandingBackrefs(&newstring,from,to);

		// move the start forward in the matching chunk
		start=fromend;

		// bail if we're not replacing globally
		if (!global) {
			break;
		}
	}

	// append the rest of the chunk
	newstring.append(str+start);

	// return the string that contains the replacements
	return newstring.detachString();
}

bool charstring::isInteger(const char *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const char	*ptr=str;
	for (; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!='-');
}

bool charstring::isInteger(const char *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const char	*ptr=str;
	for (int32_t index=0; index<len; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
		ptr++;
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!='-');
}

bool charstring::isNumber(const char *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const char	*ptr=str;
	int32_t		decimal=0;
	for (; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!='-' && *(ptr-1)!='.');
}

bool charstring::isNumber(const char *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const char	*ptr=str;
	int32_t		decimal=0;
	for (int32_t index=0; index<len; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
		ptr++;
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!='-' && *(ptr-1)!='.');
}

int64_t charstring::convertAmount(const char *amount) {
	if (!amount) {
		return 0;
	}
	const char	*dollarsstr=findFirst(amount,'$');
	dollarsstr=(dollarsstr)?dollarsstr+1:amount;
	uint64_t	dollars=convertToUnsignedInteger(dollarsstr);
	const char	*centsstr=findFirst(amount,'.');
	uint64_t	cents=(centsstr)?convertToUnsignedInteger(centsstr+1):0;
	return (dollars*100+cents);
}

char *charstring::convertAmount(int64_t amount) {
	uint16_t	len=getIntegerLength(amount)+4;
	if (len<6) {
		len=6;
	}
	char	negative[2];
	if (amount<0) {
		negative[0]='-';
	} else {
		negative[0]='\0';
	}
	negative[1]='\0';
	char	*amountstr=new char[len];
	int64_t	amt;
	#ifdef RUDIMENTS_HAVE_LLABS
		amt=llabs((long long)amount);
	#else
		amt=abs((long)amount);
	#endif
	printf(amountstr,len,
			"$%s%lld.%02lld",negative,
			amt/100,amt-(amt/100*100));
	return amountstr;
}

char *charstring::convertAmount(int64_t amount, uint16_t spaces) {
	char	*amt=convertAmount(amount);
	ssize_t	amtlen=getLength(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	char	*buffer=new char[realspaces+1];
	buffer[realspaces]='\0';
	bytestring::set(buffer,' ',realspaces);
	bytestring::copy(buffer+realspaces-amtlen,amt+1,amtlen);
	if (buffer[0]==' ') {
		buffer[0]='$';
	}
	delete[] amt;
	return buffer;
}

// computes count*multiplier+addend, false if it exceeds limit
static bool bufferSizeWithin(uint64_t count, uint64_t multiplier,
				uint64_t addend, size_t limit, size_t *size) {
	if (addend>limit || (multiplier && count>(limit-addend)/multiplier)) {
		return false;
	}
	*size=count*multiplier+addend;
	return true;
}

// computes count*multiplier+addend, false if it does not fit in a size_t
static bool bufferSize(uint64_t count, uint64_t multiplier,
				uint64_t addend, size_t *size) {
	return bufferSizeWithin(count,multiplier,addend,(size_t)-1,size);
}

// buffers output * writes in chunks, discarding all after the first failure
struct chunkedwriter {
	output	*out;
	bool	binary;
	bool	failed;
	size_t	used;
	char	buffer[256];
};

static void chunkedWriterInit(chunkedwriter *cw, output *out, bool binary) {
	cw->out=out;
	cw->binary=binary;
	cw->failed=false;
	cw->used=0;
}

static void chunkedWriterFlush(chunkedwriter *cw) {
	if (cw->used && !cw->failed) {
		ssize_t	result=(cw->binary)?
				cw->out->write((const byte_t *)cw->buffer,
								cw->used):
				cw->out->write(cw->buffer,cw->used);
		if (result!=(ssize_t)cw->used) {
			cw->failed=true;
		}
	}
	cw->used=0;
}

static void chunkedWriterAppend(chunkedwriter *cw,
					const void *data, size_t size) {
	const char	*ptr=(const char *)data;
	while (size && !cw->failed) {
		size_t	chunk=sizeof(cw->buffer)-cw->used;
		if (chunk>size) {
			chunk=size;
		}
		bytestring::copy(cw->buffer+cw->used,ptr,chunk);
		cw->used+=chunk;
		ptr+=chunk;
		size-=chunk;
		if (cw->used==sizeof(cw->buffer)) {
			chunkedWriterFlush(cw);
		}
	}
}

static const char	hexdigits[]="0123456789abcdef";

// writes 2 lowercase hex digits to "out"
static void hexEncodeByte(byte_t b, char *out) {
	out[0]=hexdigits[b>>4];
	out[1]=hexdigits[b&0x0F];
}

// returns false and sets "value" to 0 if "c" is not a hex digit
static bool hexDigitValue(char c, byte_t *value) {
	if (c>='A' && c<='F') {
		*value=c-'A'+10;
	} else if (c>='a' && c<='f') {
		*value=c-'a'+10;
	} else if (c>='0' && c<='9') {
		*value=c-'0';
	} else {
		*value=0;
		return false;
	}
	return true;
}

// decodes the 2 characters at "in", counting a non-hex digit as 0
static byte_t hexDecodePair(const char *in) {
	byte_t	sixteens;
	byte_t	ones;
	hexDigitValue(in[0],&sixteens);
	hexDigitValue(in[1],&ones);
	return sixteens*16+ones;
}

static const char	upperhexdigits[]="0123456789ABCDEF";

// url-encodes "b" at "out", returns the characters written, 1 or 3
static size_t urlEncodeByte(byte_t b, char *out) {
	if (b==' ') {
		out[0]='+';
		return 1;
	}
	if (character::isAlphanumeric(b) ||
			character::isInSet(b,"$-_.!*'(),")) {
		out[0]=b;
		return 1;
	}
	out[0]='%';
	out[1]=upperhexdigits[b>>4];
	out[2]=upperhexdigits[b&0x0F];
	return 3;
}

// url-decodes one character or %XX at "in" to "out", returns the characters
// consumed, a malformed % is copied as-is
static size_t urlDecodeSequence(const char *in, const char *end, char *out) {
	byte_t	sixteens;
	byte_t	ones;
	if (*in=='+') {
		*out=' ';
		return 1;
	}
	if (*in=='%' && end-in>=3 &&
			hexDigitValue(in[1],&sixteens) &&
			hexDigitValue(in[2],&ones)) {
		*out=sixteens*16+ones;
		return 3;
	}
	*out=*in;
	return 1;
}

char *charstring::urlEncode(const char *input) {
	return urlEncode(input,getLength(input));
}

char *charstring::urlEncode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	urlEncode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::urlEncode(const char *input,
				char **output, uint64_t *outputlen) {
	urlEncode(input,getLength(input),output,outputlen);
}

void charstring::urlEncode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	(*output)=NULL;
	(*outputlen)=0;
	if (!input) {
		return;
	}

	// size the output
	size_t	size;
	if (!bufferSize(inputlen,3,1,&size)) {
		return;
	}

	// encode
	(*output)=new char[size];
	char		*oi=*output;
	const char	*end=input+inputlen;
	for (const char *c=input; c!=end; c++) {
		oi+=urlEncodeByte(*c,oi);
	}
	*oi='\0';
	(*outputlen)=oi-(*output);
}

void charstring::urlEncode(const char *input, output *out) {
	urlEncode(input,getLength(input),out);
}

void charstring::urlEncode(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	char		encoded[3];
	for (const char *c=input; c!=end && !cw.failed; c++) {
		chunkedWriterAppend(&cw,encoded,urlEncodeByte(*c,encoded));
	}
	chunkedWriterFlush(&cw);
}

char *charstring::urlDecode(const char *input) {
	return urlDecode(input,getLength(input));
}

char *charstring::urlDecode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	urlDecode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::urlDecode(const char *input,
				char **output, uint64_t *outputlen) {
	urlDecode(input,getLength(input),output,outputlen);
}

void charstring::urlDecode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	(*output)=NULL;
	(*outputlen)=0;
	if (!input) {
		return;
	}

	// size the output
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		return;
	}

	// decode
	(*output)=new char[size];
	char		*oi=*output;
	const char	*end=input+inputlen;
	for (const char *c=input; c!=end; oi++) {
		c+=urlDecodeSequence(c,end,oi);
	}
	*oi='\0';
	(*outputlen)=oi-(*output);
}

void charstring::urlDecode(const char *input, output *out) {
	urlDecode(input,getLength(input),out);
}

void charstring::urlDecode(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	char		decoded;
	for (const char *c=input; c!=end && !cw.failed; ) {
		c+=urlDecodeSequence(c,end,&decoded);
		chunkedWriterAppend(&cw,&decoded,1);
	}
	chunkedWriterFlush(&cw);
}

// returns the entity for "c" and its length, or NULL if "c" is copied as-is
static const char *htmlEncodeChar(char c, size_t *len) {
	switch (c) {
		case '&':
			*len=5;
			return "&amp;";
		case '<':
			*len=4;
			return "&lt;";
		case '>':
			*len=4;
			return "&gt;";
		case '"':
			*len=6;
			return "&quot;";
		case '\'':
			*len=5;
			return "&#39;";
	}
	*len=1;
	return NULL;
}

char *charstring::htmlEncode(const char *input) {
	return htmlEncode(input,getLength(input));
}

char *charstring::htmlEncode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	htmlEncode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::htmlEncode(const char *input,
				char **output, uint64_t *outputlen) {
	htmlEncode(input,getLength(input),output,outputlen);
}

void charstring::htmlEncode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	if (!input) {
		input="";
		inputlen=0;
	}

	// check the size before reading the input
	size_t	size;
	(*output)=NULL;
	(*outputlen)=0;
	if (!bufferSize(inputlen,1,1,&size)) {
		return;
	}

	// size the output
	const char	*end=input+inputlen;
	size_t		replen;
	for (const char *c=input; c!=end; c++) {
		htmlEncodeChar(*c,&replen);
		(*outputlen)+=replen;
	}
	if (!bufferSize(*outputlen,1,1,&size)) {
		(*outputlen)=0;
		return;
	}

	// encode
	(*output)=new char[size];
	char	*oi=*output;
	for (const char *c=input; c!=end; c++) {
		const char	*rep=htmlEncodeChar(*c,&replen);
		if (rep) {
			bytestring::copy(oi,rep,replen);
		} else {
			*oi=*c;
		}
		oi+=replen;
	}
	*oi='\0';
}

void charstring::htmlEncode(const char *input, output *out) {
	htmlEncode(input,getLength(input),out);
}

void charstring::htmlEncode(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	size_t		replen;
	for (const char *c=input; c!=end && !cw.failed; c++) {
		const char	*rep=htmlEncodeChar(*c,&replen);
		chunkedWriterAppend(&cw,(rep)?rep:c,replen);
	}
	chunkedWriterFlush(&cw);
}

static char *writeUtf8(char *outptr, uint32_t cp) {
	if (cp<0x80) {
		*outptr++=(char)cp;
	} else if (cp<0x800) {
		*outptr++=(char)(0xC0|(cp>>6));
		*outptr++=(char)(0x80|(cp&0x3F));
	} else if (cp<0x10000) {
		*outptr++=(char)(0xE0|(cp>>12));
		*outptr++=(char)(0x80|((cp>>6)&0x3F));
		*outptr++=(char)(0x80|(cp&0x3F));
	} else {
		*outptr++=(char)(0xF0|(cp>>18));
		*outptr++=(char)(0x80|((cp>>12)&0x3F));
		*outptr++=(char)(0x80|((cp>>6)&0x3F));
		*outptr++=(char)(0x80|(cp&0x3F));
	}
	return outptr;
}

static const char	*htmlentities[]={
	"&amp;","&","&lt;","<","&gt;",">","&quot;","\"","&apos;","'",NULL
};

// html-decodes one character or entity at "in" to "out", returns the
// characters consumed, an unknown or malformed entity copies only its &
static size_t htmlDecodeSequence(const char *in, const char *end,
						char *out, size_t *outlen) {

	if (*in=='&') {

		// named entities
		size_t	avail=end-in;
		for (const char **e=htmlentities; *e; e+=2) {
			size_t	len=charstring::getLength(*e);
			if (len<=avail && !bytestring::compare(in,*e,len)) {
				*out=**(e+1);
				*outlen=1;
				return len;
			}
		}

		// numeric references
		if (avail>1 && in[1]=='#') {
			const char	*p=in+2;
			bool		hex=(p!=end && (*p=='x' || *p=='X'));
			if (hex) {
				p++;
			}
			const char	*digits=p;
			uint32_t	cp=0;
			bool		valid=true;
			for (; p!=end; p++) {
				unsigned char	ch=(unsigned char)*p;
				uint32_t	d;
				if (character::isDigit(ch)) {
					d=ch-'0';
				} else if (hex && character::isHexDigit(ch)) {
					d=(ch|0x20)-'a'+10;
				} else {
					break;
				}

				// stop accumulating past the largest
				// code point, so the value can't wrap
				if (valid) {
					cp=cp*((hex)?16:10)+d;
					if (cp>0x10FFFF) {
						valid=false;
					}
				}
			}
			if (p>digits && p!=end && *p==';' && valid && cp &&
					(cp<0xD800 || cp>0xDFFF)) {
				*outlen=writeUtf8(out,cp)-out;
				return p+1-in;
			}
		}
	}

	// leave anything else as typed
	*out=*in;
	*outlen=1;
	return 1;
}

char *charstring::htmlDecode(const char *input) {
	return htmlDecode(input,getLength(input));
}

char *charstring::htmlDecode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	htmlDecode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::htmlDecode(const char *input,
				char **output, uint64_t *outputlen) {
	htmlDecode(input,getLength(input),output,outputlen);
}

void charstring::htmlDecode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	if (!input) {
		input="";
		inputlen=0;
	}

	// size the output, no entity decodes to more bytes than it occupies
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		(*output)=NULL;
		(*outputlen)=0;
		return;
	}
	(*output)=new char[size];

	// decode
	char		*oi=*output;
	const char	*end=input+inputlen;
	size_t		decodedlen;
	for (const char *c=input; c!=end; oi+=decodedlen) {
		c+=htmlDecodeSequence(c,end,oi,&decodedlen);
	}
	*oi='\0';
	(*outputlen)=oi-(*output);
}

void charstring::htmlDecode(const char *input, output *out) {
	htmlDecode(input,getLength(input),out);
}

void charstring::htmlDecode(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	char		decoded[4];
	size_t		decodedlen;
	for (const char *c=input; c!=end && !cw.failed; ) {
		c+=htmlDecodeSequence(c,end,decoded,&decodedlen);
		chunkedWriterAppend(&cw,decoded,decodedlen);
	}
	chunkedWriterFlush(&cw);
}

// json-encodes "c" at "out", returns the characters written, 1, 2, or 6
static size_t jsonEncodeChar(char c, char *out) {
	if (c=='"' || c=='\\') {
		out[0]='\\';
		out[1]=c;
		return 2;
	}
	if ((byte_t)c<0x20) {
		out[0]='\\';
		out[1]='u';
		out[2]='0';
		out[3]='0';
		hexEncodeByte((byte_t)c,out+4);
		return 6;
	}
	out[0]=c;
	return 1;
}

char *charstring::jsonEncode(const char *input) {
	return jsonEncode(input,getLength(input));
}

char *charstring::jsonEncode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	jsonEncode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::jsonEncode(const char *input,
				char **output, uint64_t *outputlen) {
	jsonEncode(input,getLength(input),output,outputlen);
}

void charstring::jsonEncode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	if (!input) {
		(*output)=duplicate("null",4);
		(*outputlen)=4;
		return;
	}

	// check the size before reading the input
	size_t	size;
	(*output)=NULL;
	(*outputlen)=0;
	if (!bufferSize(inputlen,1,3,&size)) {
		return;
	}

	// size the output, including the quotes
	const char	*end=input+inputlen;
	char		encoded[6];
	(*outputlen)=2;
	for (const char *c=input; c!=end; c++) {
		(*outputlen)+=jsonEncodeChar(*c,encoded);
	}
	if (!bufferSize(*outputlen,1,1,&size)) {
		(*outputlen)=0;
		return;
	}

	// encode
	(*output)=new char[size];
	char	*oi=*output;
	*oi++='"';
	for (const char *c=input; c!=end; c++) {
		oi+=jsonEncodeChar(*c,oi);
	}
	*oi++='"';
	*oi='\0';
}

void charstring::jsonEncode(const char *input, output *out) {
	jsonEncode(input,getLength(input),out);
}

void charstring::jsonEncode(const char *input, uint64_t inputlen, output *out) {

	// handle null output
	if (!out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);

	// handle null input
	if (!input) {
		chunkedWriterAppend(&cw,"null",4);
		chunkedWriterFlush(&cw);
		return;
	}

	// encode
	const char	*end=input+inputlen;
	char		encoded[6];
	chunkedWriterAppend(&cw,"\"",1);
	for (const char *c=input; c!=end && !cw.failed; c++) {
		chunkedWriterAppend(&cw,encoded,jsonEncodeChar(*c,encoded));
	}
	chunkedWriterAppend(&cw,"\"",1);
	chunkedWriterFlush(&cw);
}

// decodes the 4 hex digits at "in", false if one is not a hex digit or they
// do not all come before "end"
static bool jsonDecodeHex4(const char *in, const char *end, uint32_t *value) {
	if (end-in<4) {
		return false;
	}
	*value=0;
	for (uint8_t i=0; i<4; i++) {
		byte_t	digit;
		if (!hexDigitValue(in[i],&digit)) {
			return false;
		}
		*value=((*value)<<4)|digit;
	}
	return true;
}

// json-decodes one character or escape at "in" to "out", returns the
// characters consumed, a malformed escape copies only its backslash
static size_t jsonDecodeSequence(const char *in, const char *end,
						char *out, size_t *outlen) {

	*outlen=1;

	// copy a character, or a lone backslash at the end, as-is
	if (*in!='\\' || end-in<2) {
		*out=*in;
		return 1;
	}

	// short escapes
	switch (in[1]) {
		case '"':
		case '\\':
		case '/':
			*out=in[1];
			return 2;
		case 'b':
			*out='\b';
			return 2;
		case 'f':
			*out='\f';
			return 2;
		case 'n':
			*out='\n';
			return 2;
		case 'r':
			*out='\r';
			return 2;
		case 't':
			*out='\t';
			return 2;
	}

	// \uXXXX, or a surrogate pair
	uint32_t	cp;
	if (in[1]=='u' && jsonDecodeHex4(in+2,end,&cp)) {
		if (cp>=0xD800 && cp<=0xDBFF) {
			uint32_t	low;
			if (end-in>=12 && in[6]=='\\' && in[7]=='u' &&
					jsonDecodeHex4(in+8,end,&low) &&
					low>=0xDC00 && low<=0xDFFF) {
				cp=0x10000+((cp-0xD800)<<10)+(low-0xDC00);
				*outlen=writeUtf8(out,cp)-out;
				return 12;
			}
		} else if (cp<0xDC00 || cp>0xDFFF) {
			*outlen=writeUtf8(out,cp)-out;
			return 6;
		}
	}

	// leave a malformed escape or lone surrogate as typed
	*out='\\';
	return 1;
}

// finds the string to decode in "input", false if "input" is NULL or exactly
// null
static bool jsonDecodeStart(const char *input, uint64_t inputlen,
				const char **start, const char **end) {

	// handle null input
	if (!input || (inputlen==4 && !bytestring::compare(input,"null",4))) {
		return false;
	}

	// handle unquoted input
	*start=input;
	*end=input+inputlen;
	if (!inputlen || *input!='"') {
		return true;
	}

	// find the closing quote, stepping over escaped characters
	(*start)++;
	const char	*c=*start;
	while (c!=*end && *c!='"') {
		c+=(*c=='\\' && *end-c>=2)?2:1;
	}
	*end=c;
	return true;
}

char *charstring::jsonDecode(const char *input) {
	return jsonDecode(input,getLength(input));
}

char *charstring::jsonDecode(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputlen;
	jsonDecode(input,inputlen,&output,&outputlen);
	return output;
}

void charstring::jsonDecode(const char *input,
				char **output, uint64_t *outputlen) {
	jsonDecode(input,getLength(input),output,outputlen);
}

void charstring::jsonDecode(const char *input, uint64_t inputlen,
				char **output, uint64_t *outputlen) {

	// handle null input
	(*output)=NULL;
	(*outputlen)=0;
	if (!input) {
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		return;
	}

	// find the string to decode
	const char	*start;
	const char	*end;
	if (!jsonDecodeStart(input,inputlen,&start,&end)) {
		return;
	}

	// no escape decodes to more bytes than it occupies
	(*output)=new char[size];

	// decode
	char	*oi=*output;
	size_t	decodedlen;
	for (const char *c=start; c!=end; oi+=decodedlen) {
		c+=jsonDecodeSequence(c,end,oi,&decodedlen);
	}
	*oi='\0';
	(*outputlen)=oi-(*output);
}

void charstring::jsonDecode(const char *input, output *out) {
	jsonDecode(input,getLength(input),out);
}

void charstring::jsonDecode(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	const char	*start;
	const char	*end;
	if (!out || !jsonDecodeStart(input,inputlen,&start,&end)) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	char		decoded[4];
	size_t		decodedlen;
	for (const char *c=start; c!=end && !cw.failed; ) {
		c+=jsonDecodeSequence(c,end,decoded,&decodedlen);
		chunkedWriterAppend(&cw,decoded,decodedlen);
	}
	chunkedWriterFlush(&cw);
}

// true if escape() puts a backslash before "c" (contains() matches a NULL "c"
// when "characters" is non-NULL, so that is escaped too)
static bool escapeNeedsBackslash(char c, const char *characters) {
	return (c=='\\' || charstring::contains(characters,c));
}

// true if unescape() writes "c", tracks whether a backslash precedes it
static bool unescapeChar(char c, bool *escaped) {
	if (!*escaped && c=='\\') {
		*escaped=true;
		return false;
	}
	*escaped=false;
	return true;
}

char *charstring::escape(const char *input, const char *characters) {
	return escape(input,getLength(input),characters);
}

char *charstring::escape(const char *input, uint64_t inputlen,
						const char *characters) {
	char		*output;
	uint64_t	outputlength;
	escape(input,inputlen,&output,&outputlength,characters);
	return output;
}

void charstring::escape(const char *input,
				char **output, uint64_t *outputlen,
				const char *characters) {
	escape(input,getLength(input),output,outputlen,characters);
}

void charstring::escape(const char *input, uint64_t inputlength,
				char **output, uint64_t *outputlength,
						const char *characters) {

	// handle null input
	(*output)=NULL;
	(*outputlength)=0;
	if (!input) {
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlength,1,1,&size)) {
		return;
	}

	// size the output
	const char	*end=input+inputlength;
	(*outputlength)=inputlength;
	for (const char *c=input; c!=end; c++) {
		if (escapeNeedsBackslash(*c,characters)) {
			(*outputlength)++;
		}
	}
	if (!bufferSize(*outputlength,1,1,&size)) {
		(*outputlength)=0;
		return;
	}

	// escape
	(*output)=new char[size];
	char	*oi=*output;
	for (const char *c=input; c!=end; c++) {
		if (escapeNeedsBackslash(*c,characters)) {
			*oi='\\';
			oi++;
		}
		*oi=*c;
		oi++;
	}
	*oi='\0';
}

void charstring::escape(const char *input, output *out,
					const char *characters) {
	escape(input,getLength(input),out,characters);
}

void charstring::escape(const char *input, uint64_t inputlen,
				output *out, const char *characters) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	for (const char *c=input; c!=end && !cw.failed; c++) {
		if (escapeNeedsBackslash(*c,characters)) {
			chunkedWriterAppend(&cw,"\\",1);
		}
		chunkedWriterAppend(&cw,c,1);
	}
	chunkedWriterFlush(&cw);
}

char *charstring::unescape(const char *input) {
	return unescape(input,getLength(input));
}

char *charstring::unescape(const char *input, uint64_t inputlen) {
	char		*output;
	uint64_t	outputsize;
	unescape(input,inputlen,&output,&outputsize);
	return output;
}

void charstring::unescape(const char *input,
				char **output, uint64_t *outputlen) {
	unescape(input,getLength(input),output,outputlen);
}

void charstring::unescape(const char *input, uint64_t inputlength,
				char **output, uint64_t *outputlength) {

	// handle null input
	(*output)=NULL;
	(*outputlength)=0;
	if (!input) {
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlength,1,1,&size)) {
		return;
	}

	// size the output
	const char	*end=input+inputlength;
	bool		escaped=false;
	for (const char *c=input; c!=end; c++) {
		if (unescapeChar(*c,&escaped)) {
			(*outputlength)++;
		}
	}
	// cannot fail, the count is at most the input length
	if (!bufferSize(*outputlength,1,1,&size)) {
		(*outputlength)=0;
		return;
	}

	// unescape
	(*output)=new char[size];
	char	*oi=*output;
	escaped=false;
	for (const char *c=input; c!=end; c++) {
		if (unescapeChar(*c,&escaped)) {
			*oi=*c;
			oi++;
		}
	}
	*oi='\0';
}

void charstring::unescape(const char *input, output *out) {
	unescape(input,getLength(input),out);
}

void charstring::unescape(const char *input, uint64_t inputlen, output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	const char	*end=input+inputlen;
	bool		escaped=false;
	for (const char *c=input; c!=end && !cw.failed; c++) {
		if (unescapeChar(*c,&escaped)) {
			chunkedWriterAppend(&cw,c,1);
		}
	}
	chunkedWriterFlush(&cw);
}

void charstring::leftJustify(char *str, int32_t len) {

	if (!str) {
		return;
	}

	// count leading spaces
	int32_t	spaces=countLeadingSpaces(str,len);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=len-spaces;
	for (index=0; index<stop; index++) {
		str[index]=str[index+spaces];
	}

	// right-pad with spaces
	for (; index<len; index++) {
		str[index]=' ';
	}
}

void charstring::rightJustify(char *str, int32_t len) {

	if (!str) {
		return;
	}
	
	// count trailing spaces
	int32_t	spaces=countTrailingSpaces(str,len);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=spaces-1;
	for (index=len-1; index>stop; index--) {
		str[index]=str[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		str[index]=' ';
	}
}

void charstring::center(char *str, int32_t len) {

	if (!str) {
		return;
	}

	int32_t	leadingspaces=countLeadingSpaces(str,len);
	int32_t	trailingspaces=countTrailingSpaces(str,len);

	int32_t	leftpad=(leadingspaces+trailingspaces)/2;

	if (leftpad>leadingspaces) {
		// shift everything right
		int32_t	difference=leftpad-leadingspaces;
		int32_t	index;
		for (index=len-1; index>difference-1; index--) {
			str[index]=str[index-difference];
		}
		for (; index>-1; index--) {
			str[index]=' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int32_t	difference=leadingspaces-leftpad;
		int32_t	index;
		for (index=0; index<len-difference; index++) {
			str[index]=str[index+difference];
		}
		for (; index<len; index++) {
			str[index]=' ';
		}
	}
}

int32_t charstring::countLeadingSpaces(const char *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	leadingspaces=0;
	for (int32_t index=0; str[index]==' ' && index<len; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int32_t charstring::countTrailingSpaces(const char *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	trailingspaces=0;
	for (int32_t index=len-1; str[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *charstring::parseNumber(int16_t number) {
	return parseNumber((int64_t)number,1);
}

char *charstring::parseNumber(int16_t number,
				uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

char *charstring::parseNumber(uint16_t number) {
	return parseNumber((uint64_t)number,1);
}

char *charstring::parseNumber(uint16_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

char *charstring::parseNumber(int32_t number) {
	return parseNumber((int64_t)number,1);
}

char *charstring::parseNumber(int32_t number, uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

char *charstring::parseNumber(uint32_t number) {
	return parseNumber((uint64_t)number,1);
}

char *charstring::parseNumber(uint32_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

char *charstring::parseNumber(int64_t number) {
	return parseNumber(number,1);
}

char *charstring::parseNumber(int64_t number, uint16_t zeropadding) {
	if (number>=0) {
		return parseNumber((uint64_t)number,zeropadding);
	}
	uint16_t	len=getIntegerLength(number);
	uint16_t	strlen=((zeropadding>len)?zeropadding:len);
	char		*ptr=new char[strlen+1];
	*ptr='-';
	ptr+=strlen;
	*ptr='\0';
	number*=-1;
	strlen--;
	while (strlen) {
		ptr--;
		*ptr='0'+number%10;
		number/=10;
		strlen--;
	}
	ptr--;
	return ptr;
}

char *charstring::parseNumber(uint64_t number) {
	return parseNumber(number,1);
}

char *charstring::parseNumber(uint64_t number, uint16_t zeropadding) {
	uint16_t	len=getIntegerLength(number);
	uint16_t	strlen=((zeropadding>len)?zeropadding:len);
	char		*ptr=new char[strlen+1];
	ptr+=strlen;
	*ptr='\0';
	while (strlen) {
		ptr--;
		*ptr='0'+number%10;
		number/=10;
		strlen--;
	}
	return ptr;
}

char *charstring::parseNumber(float number) {
	return parseNumber((long double)number);
}

char *charstring::parseNumber(float number, uint16_t scale) {
	return parseNumber((long double)number,scale);
}

char *charstring::parseNumber(float number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

char *charstring::parseNumber(double number) {
	return parseNumber((long double)number);
}

char *charstring::parseNumber(double number,
				uint16_t scale) {
	return parseNumber((long double)number,scale);
}

char *charstring::parseNumber(double number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

char *charstring::parseNumber(long double number) {
	char	*str=new char[22];
	printf(str,22,
		#ifdef _HPUX
		"%lf",
		#else
		"%Lf",
		#endif
		number);
	return str;
}

char *charstring::parseNumber(long double number, uint16_t scale) {
	char	*str=new char[22];
	printf(str,22,
		#ifdef _HPUX
		"%.*lf",
		#else
		"%.*Lf",
		#endif
		scale,number);
	return str;
}

char *charstring::parseNumber(long double number,
				uint16_t precision, uint16_t scale) {
	size_t	strlen=precision+3;
	char	*str=new char[strlen];
	printf(str,strlen,
		#ifdef _HPUX
		"%*.*lf",
		#else
		"%*.*Lf",
		#endif
		precision,scale,number);
	return str;
}

void charstring::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

uint16_t charstring::getIntegerLength(int16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int16_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t charstring::getIntegerLength(int32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int32_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t charstring::getIntegerLength(int64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int64_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t charstring::getIntegerLength(uint16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint16_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t charstring::getIntegerLength(uint32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint32_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t charstring::getIntegerLength(uint64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint64_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

size_t charstring::getLength(const char *string) {
	return (string)?strlen(string):0;
}

size_t charstring::getSize(const char *string) {
	return (string)?strlen(string)+character::getNullSize():0;
}

bool charstring::isNullOrEmpty(const char *string) {
	return !(string && string[0]);
}

bool charstring::isYes(const char *string) {
	char	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,"yes",3)) {
		next=string[3];
	} else if (!compareIgnoringCase(string,"true",4)) {
		next=string[4];
	} else if (!compareIgnoringCase(string,"on",2)) {
		next=string[2];
	} else if (string[0]=='y' || string[0]=='Y' ||
			string[0]=='t' || string[0]=='T' ||
			string[0]=='1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!character::isAlphanumeric(next) && 
				!character::isAlphabeticalExtended(next)));
}

bool charstring::isNo(const char *string) {
	char	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,"no",2)) {
		next=string[2];
	} else if (!compareIgnoringCase(string,"false",5)) {
		next=string[5];
	} else if (!compareIgnoringCase(string,"off",3)) {
		next=string[3];
	} else if (string[0]=='n' || string[0]=='N' ||
			string[0]=='f' || string[0]=='F' ||
			string[0]=='0') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!character::isAlphanumeric(next) && 
				!character::isAlphabeticalExtended(next)));
}

void charstring::zero(char *str, size_t len) {
	bytestring::set(str,0,len);
}

char *charstring::append(char *dest, const char *source) {
	return append(dest,source,getLength(source)+1);
}

char *charstring::append(char *dest, const char *source, size_t len) {
	return copy(dest+getLength(dest),source,len);
}

char *charstring::append(char *dest, int64_t number) {
	char	*str=parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, uint64_t number) {
	char	*str=parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number) {
	char	*str=parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number, uint16_t scale) {
	char	*str=parseNumber(number,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number, uint16_t precision,
							uint16_t scale) {
	char	*str=parseNumber(number,precision,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::copy(char *dest, const char *source) {
	return copy(dest,source,getLength(source)+1);
}

char *charstring::copy(char *dest, const char *source, size_t len) {
	if (!dest || !source) {
		return dest;
	}
	return strncpy(dest,source,len);
}

char *charstring::copy(char *dest, size_t location, const char *source) {
	return copy(dest+location,source);
}

char *charstring::copy(char *dest, size_t location,
					const char *source, size_t len) {
	return copy(dest+location,source,len);
}

char *charstring::safeCopy(char *dest, size_t destlen, const char *source) {
	return safeCopy(dest,destlen,source,getLength(source)+1);
}

char *charstring::safeCopy(char *dest, size_t destlen,
				const char *source, size_t sourcelen) {
	return copy(dest,source,(sourcelen>destlen)?destlen:sourcelen);
}

int32_t charstring::compare(const char *str1, const char *str2) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	return strcmp(str1,str2);
}

int32_t charstring::compare(const char *str1, const char *str2, size_t len) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	return strncmp(str1,str2,len);
}

int32_t charstring::compareIgnoringCase(const char *str1, const char *str2) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_STRCASECMP
		return strcasecmp(str1,str2);
	#else
		int32_t	diff=0;
		while (*str1 && *str2) {
			diff=(character::upper(*str1)-
				character::upper(*str2));
			if (diff) {
				return diff;
			}
			str1++;
			str2++;
		}
		return character::upper(*str1)-
				character::upper(*str2);
	#endif
}

int32_t charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t len) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_STRNCASECMP
		return strncasecmp(str1,str2,len);
	#else
		int32_t	diff=0;
		while (*str1 && *str2 && len) {
			diff=(character::upper(*str1)-
				character::upper(*str2));
			if (diff) {
				return diff;
			}
			str1++;
			str2++;
			len--;
		}
		return (len)?character::upper(*str1)-
				character::upper(*str2):0;
	#endif
}

int32_t charstring::compareNatural(const char *str1, const char *str2) {
	return compareNatural(str1,str2,".");
}

int32_t charstring::compareNatural(const char *str1,
					const char *str2,
					const char *delimiters) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}

	int64_t		difference=0;
	const char	*start1=NULL;
	const char	*start2=NULL;
	char		*num1=NULL;
	char		*num2=NULL;
	for (;;) {

		// handle end-of-string cases...
		if (!*str1 && !*str2) {
			return difference;
		}

		// str1 has more chars than str2 - str1 > str2
		if (*str1 && !*str2) {
			difference++;
			return difference;
		}

		// str2 has more chars than str1 - str2 > str1
		if (!*str1 && *str2) {
			difference--;
			return difference;
		}

		// subtract the "is a digit" status of the next character of
		// str1 from the "is a digit" status of the next character of
		// str2 and add the result to the running difference
		//
		// (if str1 contains a non-number and str2 contains a number
		// then the difference will be positive, indicating that
		// str1 > str2, which is what we want)
		bool	isdigit1=character::isDigit(*str1);
		bool	isdigit2=character::isDigit(*str2);
		difference+=isdigit2-isdigit1;

		// if the difference is non-zero then return it
		if (difference) {
			return difference;
		}

		if (isdigit1 && isdigit2) {

			// move to after the number in both strings
			start1=str1;
			while (*str1 && (character::isDigit(*str1) ||
					character::isInSet(*str1,delimiters))) {
				str1++;
			}
			start2=str2;
			while (*str2 && (character::isDigit(*str2) ||
					character::isInSet(*str2,delimiters))) {
				str2++;
			}

			// copy out the numbers
			num1=duplicate(start1,str1-start1);
			num2=duplicate(start2,str2-start2);

			// version-compare the numbers and add that
			// to the running difference
			difference+=compareVersions(num1,num2,delimiters);

			// clean up
			delete[] num1;
			delete[] num2;

		} else {

			// subtract the next character of str2 from the next
			// character of str1 and add the result to the running
			// difference
			difference+=(*str1)-(*str2);

			// move on
			str1++;
			str2++;
		}

		// if the running difference is non-zero then return it
		if (difference) {
			return difference;
		}
	}
}

int32_t charstring::compareVersions(const char *str1, const char *str2) {
	return compareVersions(str1,str2,".");
}

int32_t charstring::compareVersions(const char *str1,
					const char *str2,
					const char *delimiters) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}

	int64_t	difference=0;
	for (;;) {

		// get the next integers from the strings, subtract them, and
		// add the result to the running difference
		difference+=(convertToInteger(str1)-convertToInteger(str2));

		// if the difference is non-zero then return it
		if (difference) {
			return difference;
		}

		// skip past the next .
		str1=findFirstOfSet(str1,delimiters);
		if (str1) {
			str1++;
		}
		str2=findFirstOfSet(str2,delimiters);
		if (str2) {
			str2++;
		}

		// bail if we're at the end of both strings
		if ((!str1 || !*str1) && (!str2 || !*str2)) {
			return difference;
		}

		// str1 has more parts than str2 - str1 > str2
		if (str1 && *str1 && (!str2 || !*str2)) {
			difference++;
			return difference;
		}

		// str2 has more parts than str1 - str2 > str1
		if (str2 && *str2 && (!str1 || !*str1)) {
			difference--;
			return difference;
		}
	}
}

bool charstring::compareWithWildcards(const char *string,
					size_t stringlength,
					const char *pattern,
					size_t patternlength,
					char singlewildcard,
					char multiwildcard) {

	// handle degenerate case
	if (!string && !pattern) {
		return true;
	}

	const char	*stringend=string+stringlength;
	const char	*patternend=pattern+patternlength;

	for (;;) {

		// if we encountered the end of the string...
		if (string==stringend) {

			// if we're also at the end of the pattern,
			// then they match
			if (pattern==patternend) {
				return true;
			}

			// if we're not also at the end of the pattern,
			// then they don't match
			return false;
		}

		// if we encountered the end of the pattern
		// (but not the end of the string) then they don't match
		if (pattern==patternend) {
			return false;
		}

		// if we encountered a multi-char wildcard...
		if (multiwildcard && *pattern==multiwildcard) {

			// skip to the next non-multi-character wildcarad
			// in the pattern
			while (*pattern==multiwildcard) {

				pattern++;
				patternlength--;

				// if we hit the end of the pattern then
				// we have a match
				if (pattern==patternend) {
					return true;
				}
			}

			// Compare the rest of the string to the rest of the
			// pattern.  If that fails, move to the next character
			// of the string and try again.  If any of these
			// succeed, then we have a match.  If they all fail
			// then the string and pattern don't match.
			for (;;) {
				if (compareWithWildcards(string,stringlength,
							pattern,patternlength,
							singlewildcard,
							multiwildcard)) {
					return true;
				}

				// move on...
				string++;
				if (string==stringend) {
					return false;
				}
				stringlength--;
			}
		}

		// bail if the characters don't match
		// (unless we encountered a single-character wildcard)
		if (*string!=*pattern &&
			(!singlewildcard || *pattern!=singlewildcard)) {
			return false;
		}

		// move on...
		string++;
		stringlength--;
		pattern++;
		patternlength--;
	}
}

bool charstring::compareWithWildcards(const char *string,
					const char *pattern,
					size_t patternlength,
					char singlewildcard,
					char multiwildcard) {
	return compareWithWildcards(string,getLength(string),
					pattern,patternlength,
					singlewildcard,multiwildcard);
}

bool charstring::compareWithWildcards(const char *string,
					size_t stringlength,
					const char *pattern,
					char singlewildcard,
					char multiwildcard) {
	return compareWithWildcards(string,stringlength,
					pattern,getLength(pattern),
					singlewildcard,multiwildcard);
}

bool charstring::compareWithWildcards(const char *string,
					const char *pattern,
					char singlewildcard,
					char multiwildcard) {
	return compareWithWildcards(string,getLength(string),
					pattern,getLength(pattern),
					singlewildcard,multiwildcard);
}

bool charstring::isInSet(const char *str, const char * const *set) {
	if (!set || !set[0]) {
		return false;
	}
	for (const char * const *s=set; *s; s++) {
		if (!compare(str,*s)) {
			return true;
		}
	}
	return false;
}

bool charstring::isInSetIgnoringCase(const char *str, const char * const *set) {
	if (!set || !set[0]) {
		return false;
	}
	for (const char * const *s=set; *s; s++) {
		if (!compareIgnoringCase(str,*s)) {
			return true;
		}
	}
	return false;
}

bool charstring::contains(const char *haystack, const char *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool charstring::containsIgnoringCase(const char *haystack,
						const char *needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool charstring::contains(const char *haystack, char needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool charstring::containsIgnoringCase(const char *haystack, char needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool charstring::startsWith(const char *haystack, const char *needle) {
	return !compare(haystack,needle,getLength(needle));
}

bool charstring::endsWith(const char *haystack, const char *needle) {
	size_t	needlelen=getLength(needle);
	size_t	haystacklen=getLength(haystack);
	return (haystacklen>=needlelen &&
		!compare(haystack+haystacklen-needlelen,needle));
}

const char *charstring::findFirst(const char *haystack, const char *needle) {
	return (haystack && needle)?strstr(haystack,needle):NULL;
}

const char *charstring::findFirstIgnoringCase(const char *haystack,
							const char *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	for (const char *ptr=haystack;
			ptr<=haystack+haystacklen-needlelen;
			ptr++) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
	}
	return NULL;
}

const char *charstring::findFirst(const char *haystack, char needle) {
	return (haystack)?strchr(haystack,needle):NULL;
}

const char *charstring::findFirstIgnoringCase(const char *haystack,
							char needle) {
	if (!haystack) {
		return NULL;
	}
	size_t	haystacklen=getLength(haystack);
	needle=character::lower(needle);
	for (const char *ptr=haystack;
			ptr<haystack+haystacklen;
			ptr++) {
		if (character::lower(*ptr)==needle) {
			return ptr;
		}
	}
	return NULL;
}

const char *charstring::findFirstOrEnd(const char *haystack, char needle) {
	#ifdef RUDIMENTS_HAVE_STRCHRNUL
	return (haystack)?strchrnul(haystack,needle):NULL;
	#else
	if (!haystack) {
		return NULL;
	}
	const char	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
	#endif
}

const char *charstring::findFirstOrEndIgnoringCase(const char *haystack,
								char needle) {
	if (!haystack) {
		return NULL;
	}
	const char	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

const char *charstring::findFirstOrEnd(const char *haystack,
						const char *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const char	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

const char *charstring::findFirstOrEndIgnoringCase(const char *haystack,
							const char *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const char	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

char *charstring::findFirstOrEnd(char *haystack, const char *needle) {
	return (char *)(findFirstOrEnd((const char *)(haystack),needle));
}

char *charstring::findFirstOrEndIgnoringCase(char *haystack,
						const char *needle) {
	return (char *)(findFirstOrEndIgnoringCase(
					(const char *)(haystack),needle));
}

const char *charstring::findLast(const char *haystack, char needle) {
	return (haystack)?strrchr(haystack,needle):NULL;
}

const char *charstring::findLastIgnoringCase(const char *haystack,
							char needle) {
	return (char *)(findFirstOrEndIgnoringCase(
					(const char *)(haystack),needle));
}

char *charstring::findFirst(char *haystack, const char *needle) {
	return (char *)(findFirst((const char *)(haystack),needle));
}

char *charstring::findFirstIgnoringCase(char *haystack, const char *needle) {
	return (char *)(findFirstIgnoringCase((const char *)(haystack),needle));
}

char *charstring::findFirst(char *haystack, char needle) {
	return (char *)(findFirst((const char *)(haystack),needle));
}

char *charstring::findFirstIgnoringCase(char *haystack, char needle) {
	return (char *)(findFirstIgnoringCase((const char *)(haystack),needle));
}

char *charstring::findLast(char *haystack, const char *needle) {
	return (char *)(findLast((const char *)(haystack),needle));
}

char *charstring::findLastIgnoringCase(char *haystack, const char *needle) {
	return (char *)(findLastIgnoringCase((const char *)(haystack),needle));
}

char *charstring::findLast(char *haystack, char needle) {
	return (char *)(findLast((const char *)(haystack),needle));
}

char *charstring::findLastIgnoringCase(char *haystack, char needle) {
	return (char *)(findLastIgnoringCase((const char *)(haystack),needle));
}

const char *charstring::findFirstOfSet(const char *haystack, const char *set) {
	return (char *)(findFirstOfSet((char *)(haystack),set));
}

char *charstring::findFirstOfSet(char *haystack, const char *set) {
	#ifdef RUDIMENTS_HAVE_STRPBRK
	return (haystack && set)?strpbrk(haystack,set):NULL;
	#else
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return NULL;
	#endif
}

const char *charstring::findFirstOfSetOrEnd(const char *haystack,
							const char *set) {
	return (char *)(findFirstOfSetOrEnd((char *)(haystack),set));
}

char *charstring::findFirstOfSetOrEnd(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return retval;
}

const char *charstring::findLastOfSet(const char *haystack, const char *set) {
	return (char *)(findLastOfSet((char *)(haystack),set));
}

char *charstring::findLastOfSet(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack+charstring::getLength(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return NULL;
}

const char *charstring::findLastOfSetOrEnd(const char *haystack,
							const char *set) {
	return (char *)(findLastOfSetOrEnd((char *)(haystack),set));
}

char *charstring::findLastOfSetOrEnd(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack+getLength(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+getLength(haystack);
}

const char *charstring::findEndOfQuotedString(const char *string,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString((char *)string,
					charstring::getLength(string),
					backslash,doubling);
}

char *charstring::findEndOfQuotedString(char *string,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString(string,
					charstring::getLength(string),
					backslash,doubling);
}

const char *charstring::findEndOfQuotedString(const char *string,
						size_t stringlen,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString((char *)string,stringlen,
						backslash,doubling);
}

char *charstring::findEndOfQuotedString(char *string,
						size_t stringlen,
						bool backslash,
						bool doubling) {

	// handle degenerate cases
	if (!string) {
		return NULL;
	}
	if (!stringlen) {
		return string;
	}

	// auto-detect the quoting style
	char	quote=*string;

	// for bracket-quoting, the closing bracket is different
	// and we need to disable backslash-escaping and doubling
	if (quote=='[') {
		quote=']';
		backslash=false;
		doubling=false;
	}

	// find end of string
	return findEndOfQuotedString(string,stringlen,
					quote,backslash,doubling);
}

const char *charstring::findEndOfQuotedString(const char *string,
						char quote,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString((char *)string,
					charstring::getLength(string),
					quote,backslash,doubling);
}

char *charstring::findEndOfQuotedString(char *string,
						char quote,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString(string,
					charstring::getLength(string),
					quote,backslash,doubling);
}

const char *charstring::findEndOfQuotedString(const char *string,
						size_t stringlen,
						char quote,
						bool backslash,
						bool doubling) {
	return findEndOfQuotedString((char *)string,stringlen,quote,
						backslash,doubling);
}

char *charstring::findEndOfQuotedString(char *string,
						size_t stringlen,
						char quote,
						bool backslash,
						bool doubling) {

	// handle degenerate cases
	if (!string) {
		return NULL;
	}
	if (!stringlen) {
		return string;
	}

	// find the end
	char	*end=string+stringlen;

	// run through the string
	char	*ch=string+1;
	for (;;) {

		if (backslash && *ch=='\\') {

			// we support backslash-escaping and
			// found a backslash...

			if (ch+1==end) {
				// apparently the string ended in a backslash
				return end;
			} else {
				// skip the escaped sequence
				ch+=2;
			}

		} else if (*ch==quote) {
	
			// we found a quote...

			// if the next char was a quote...
			if (doubling && *(ch+1)==quote) {
				
				// skip the escaped sequence
				ch+=2;
			} else {

				// this is the terminating quote
				return ch+1;
			}

		} else if (ch==end) {

			// we ran off the end of the string...
			return end;

		} else {

			// move on
			ch++;
		}
	}
}

size_t charstring::getLengthContainingSet(const char *haystack,
						const char *set) {
	#ifdef RUDIMENTS_HAVE_STRSPN
	return (haystack && set)?strspn(haystack,set):0;
	#else
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (contains(set,haystack[index])) {
		index++;
	}
	return index;
	#endif
}

size_t charstring::getLengthNotContainingSet(const char *haystack,
						const char *set) {
	#ifdef RUDIMENTS_HAVE_STRCSPN
	return (haystack && set)?strcspn(haystack,set):0;
	#else
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (!contains(set,haystack[index])) {
		index++;
	}
	return index;
	#endif
}

char *charstring::isBefore(const char *str, const char *delimiter) {
	return isBetween(str,NULL,delimiter);
}

char *charstring::isBetween(const char *str,
				const char *start, const char *end) {

	// find the start (or use beginning of "str" if "start" is empty/NULL)
	const char	*s=NULL;
	if (isNullOrEmpty(start)) {
		s=str;
	} else {
		s=findFirst(str,start);
	}
	if (!s) {
		return NULL;
	}

	// bump past the start
	if (s!=str) {
		s+=getLength(start);
	}

	// find the end (or use end of "str" if "end" is empty/NULL)
	const char	*e=NULL;
	if (isNullOrEmpty(end)) {
		for (e=s; *e; e++) {}
	} else {
		e=findFirst(s,end);
	}
	if (!e) {
		return NULL;
	}

	// copy out the string between s and e
	return charstring::duplicate(s,e-s);
}

char *charstring::isAfter(const char *str, const char *delimiter) {
	return isBetween(str,delimiter,NULL);
}

char *charstring::duplicate(const char *str) {
	return duplicate(str,getLength(str));
}

char *charstring::duplicate(const char *str, size_t len) {
	if (!str) {
		return NULL;
	}
	size_t	size;
	if (!bufferSize(len,1,1,&size)) {
		return NULL;
	}
	char	*buffer=new char[size];
	copy(buffer,str,len);
	buffer[len]='\0';
	return buffer;
}

char *charstring::duplicate(const wchar_t *string) {
	return duplicate(string,wcharstring::getLength(string),'?');
}

char *charstring::duplicate(const wchar_t *string, size_t len) {
	return duplicate(string,len,'?');
}

char *charstring::duplicate(const wchar_t *string, char replacement) {
	return duplicate(string,wcharstring::getLength(string),replacement);
}

char *charstring::duplicate(const wchar_t *string, size_t len,
							char replacement) {
	if (!string) {
		return NULL;
	}
	size_t	size;
	if (!bufferSize(len,1,1,&size)) {
		return NULL;
	}
	char	*retval=new char[size];
	for (size_t i=0; i<len; i++) {
		retval[i]=character::duplicate(string[i],replacement);
	}
	retval[len]='\0';
	return retval;
}

char *charstring::duplicateUcs2(const ucs2_t *string) {
	return duplicateUcs2(string,ucs2charstring::getLength(string),
						'?',sys::isBigEndian());
}

char *charstring::duplicateUcs2(const ucs2_t *string, size_t len) {
	return duplicateUcs2(string,len,'?',sys::isBigEndian());
}

char *charstring::duplicateUcs2(const ucs2_t *string, bool bigendian) {
	return duplicateUcs2(string,ucs2charstring::getLength(string),
							'?',bigendian);
}

char *charstring::duplicateUcs2(const ucs2_t *string, size_t len,
							bool bigendian) {
	return duplicateUcs2(string,len,'?',bigendian);
}

char *charstring::duplicateUcs2(const ucs2_t *string, char replacement) {
	return duplicateUcs2(string,ucs2charstring::getLength(string),
					replacement,sys::isBigEndian());
}

char *charstring::duplicateUcs2(const ucs2_t *string, size_t len,
							char replacement) {
	return duplicateUcs2(string,len,replacement,sys::isBigEndian());
}

char *charstring::duplicateUcs2(const ucs2_t *string, char replacement,
							bool bigendian) {
	return duplicateUcs2(string,ucs2charstring::getLength(string),
						replacement,bigendian);
}

char *charstring::duplicateUcs2(const ucs2_t *string, size_t len,
					char replacement, bool bigendian) {
	if (!string) {
		return NULL;
	}
	size_t	size;
	if (!bufferSize(len,1,1,&size)) {
		return NULL;
	}
	char	*retval=new char[size];
	for (size_t i=0; i<len; i++) {
		retval[i]=character::duplicateUcs2(string[i],
							replacement,
							bigendian);
	}
	retval[len]='\0';
	return retval;
}

void charstring::rightTrim(char *str) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (character::isWhitespace(*str)) { 
			str--;
		}
		str++;

		// terminate the string there
		*str='\0';
	}
}

void charstring::leftTrim(char *str) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
		while (character::isWhitespace(str[i])) {
			i++;
		}
	
		// swap chars to front of string
		while (str[i]) {
			str[j]=str[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		str[j]='\0';
	}
}

void charstring::bothTrim(char *string) {
	leftTrim(string);
	rightTrim(string);
}

int64_t charstring::convertToInteger(const char *string) {
	return convertToInteger(string,NULL,10);
}

int64_t charstring::convertToInteger(const char *string, const char **endptr) {
	return convertToInteger(string,endptr,10);
}

int64_t charstring::convertToInteger(const char *string, int32_t base) {
	return convertToInteger(string,NULL,base);
}

int64_t charstring::convertToInteger(const char *string,
				const char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOLL
	return (string)?strtoll(string,(char **)endptr,base):0;
	#else
	return (string)?strtol(string,(char **)endptr,base):0;
	#endif
}

uint64_t charstring::convertToUnsignedInteger(const char *string) {
	return convertToUnsignedInteger(string,NULL,10);
}

uint64_t charstring::convertToUnsignedInteger(const char *string,
						const char **endptr) {
	return convertToUnsignedInteger(string,endptr,10);
}

uint64_t charstring::convertToUnsignedInteger(const char *string,
							int32_t base) {
	return convertToUnsignedInteger(string,NULL,base);
}

uint64_t charstring::convertToUnsignedInteger(const char *string,
					const char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOULL
	return (string)?strtoull(string,(char **)endptr,base):0;
	#else
	return (string)?strtoul(string,(char **)endptr,base):0;
	#endif
}

long double charstring::convertToFloat(const char *string) {
	return convertToFloat(string,NULL);
}

long double charstring::convertToFloatC(const char *string) {

	/* This method is needed when the locale of the client is different
	 * from the C/POSIX locale, but we still need to convert a string
	 * formatted in C/POSIX locale format to a float. Perhaps the string
	 * was supplied to us by a host which is using the C/POSIX locale).
	 * Because there are no standard, let alone portable api, for string
	 * conversion functions that take a locale as argument, this code
	 * implements a workaround of converting the string from C locale
	 * representation to one appropriate for the current locale. */

#ifdef RUDIMENTS_HAVE_LOCALE_H
	size_t		len=getLength(string);
	char		stringinlocale[256];
	const char	*decimalpointlocation;
	struct	lconv	*currentlconv=localeconv();
	if ((currentlconv!=NULL) &&
		(currentlconv->decimal_point!=NULL) &&
		(currentlconv->decimal_point[0]!=0) &&
		(currentlconv->decimal_point[0]!='.') &&
		(currentlconv->decimal_point[1]==0) &&
		((decimalpointlocation=findFirst(string,'.'))!=NULL) &&
		(len<sizeof(stringinlocale))) {

		bytestring::copy(stringinlocale,string,len+1);

		stringinlocale[decimalpointlocation-string]=
					currentlconv->decimal_point[0];

		return convertToFloat(stringinlocale,NULL);
	}
#endif
	return convertToFloat(string,NULL);
}

long double charstring::convertToFloat(const char *string,
					const char **endptr) {
	#ifdef _HPUX
		// g++ can't seem to compile code that uses strtold() on HPUX.
		// sscanf() code compiles and works correctly though, except
		// that %Lf doesn't work as it should.  Instead, you have to
		// use %lf.  At least on HP-UX 11.11.
		if (endptr) {
			*endptr=NULL;
		}
		if (isNullOrEmpty(string)) {
			return 0.0;
		}
		long double	retval;
		int		endoflongdouble;
		int		result=sscanf(string,
						"%lf%n",
						&retval,
						&endoflongdouble);
		if (result==EOF) {
			return 0.0;
		}
		if (result==2) {
			*endptr=string+endoflongdouble;
		}
		return retval;
	#else
		#ifdef RUDIMENTS_HAVE_STRTOLD
			return (string)?
				strtold(string,(char **)endptr):
				0.0;
		#else
			return (string)?
				(long double)(strtod(string,(char **)endptr)):
				0.0;
		#endif
	#endif
}

void charstring::split(const char *string, const char *delimiter,
				bool collapse,
				char ***list, uint64_t *listcount) {
	split(string,getLength(string),
			delimiter,getLength(delimiter),
			collapse,list,listcount);
}

void charstring::split(const char *string, size_t stringlength,
				const char *delimiter, bool collapse,
				char ***list, uint64_t *listcount) {
	split(string,stringlength,
			delimiter,getLength(delimiter),
			collapse,list,listcount);
}

void charstring::split(const char *string, 
				const char *delimiter, size_t delimiterlength,
				bool collapse,
				char ***list, uint64_t *listcount) {
	split(string,getLength(string),
			delimiter,delimiterlength,
			collapse,list,listcount);
}

void charstring::split(const char *string, size_t stringlength,
				const char *delimiter, size_t delimiterlength,
				bool collapse,
				char ***list, uint64_t *listcount) {

	// handle degenerate cases
	if (!list && !listcount) {
		return;
	}
	if (isNullOrEmpty(string) || !stringlength ||
		isNullOrEmpty(delimiter) || !delimiterlength) {
		if (list) {
			(*list)=NULL;
		}
		if (listcount) {
			(*listcount)=0;
		}
		return;
	}

	// declare local list count
	uint64_t	lc=0;

	// 2 passes,
	// 1 to count the number of chunks to split the string into,
	// 1 to actually split the string
	for (int32_t pass=0; pass<2; pass++) {

		// set pointers to the beginning and end of the string
		const char	*start=string;
		const char	*end=string+stringlength;

		// initialize the list count
		lc=0;

		// loop through the string...
		const char	*current=start;
		for (;;) {

			// if there's not enough room left in the string for
			// another delimiter, then move the current position
			// to the end
			if (((size_t)(end-current))<delimiterlength) {
				current=end;
			}

			// if we found a delimiter or ran into the end of
			// the string...
			if (current==end ||
				!compare(current,delimiter,delimiterlength)) {

				// handle cases of multiple delimiters in a row
				if (current!=start || !collapse) {

					// if we're on the second pass...
					if (pass && list) {

						// make a copy of the string
						// between the last delimiter
						// and here
						(*list)[lc]=
							duplicate(start,
								current-start);
					}

					// increment the counter
					lc++;
				}

				if (current==end) {
					// if we're at the end of the string,
					// then we're done
					break;
				} else {
					// move the current and start pointers
					current=current+delimiterlength;
					start=current;
				}
			} else {
				current++;
			}
		}

		// if we're done with the first pass,
		// create the list and reset the counter
		if (!pass && list) {
			(*list)=new char *[lc];
		}
	}

	// copy out the listcount
	if (listcount) {
		(*listcount)=lc;
	}
}

char *charstring::getSubString(const char *str, size_t start, size_t end) {

	// handle end<start
	if (end<start) {
		size_t	temp=end;
		end=start;
		start=temp;
	}
	return duplicate(str+start,end-start+1);
}

char *charstring::getSubString(const char *str, size_t start) {
	return getSubString(str,start,getLength(str)-1);
}

char *charstring::base64Encode(const byte_t *input) {
	return base64Encode(input,getLength((const char *)input));
}

char *charstring::base64Encode(const byte_t *input, uint64_t inputsize) {
	char		*retval=NULL;
	uint64_t	retvalsize=0;
	base64Encode(input,inputsize,&retval,&retvalsize);
	return retval;
}

static char	b64code[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz0123456789+/";

// Since we're setting a lot of the elements of this array to -1, it must
// explicitly be set to type signed char for platforms like ARM where char
// (without the signed qualifier) are actually unsigned.
static signed char	b64dcode[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// 0-9
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// 10-19
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// etc.
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,62,-1,-1,-1,63,52,53,
					54,55,56,57,58,59,60,61,-1,-1,
					-1,0,-1,-1,-1,0,1,2,3,4,
					5,6,7,8,9,10,11,12,13,14,
					15,16,17,18,19,20,21,22,23,24,
					25,-1,-1,-1,-1,-1,-1,26,27,28,
					29,30,31,32,33,34,35,36,37,38,
					39,40,41,42,43,44,45,46,47,48,
					49,50,51,-1,-1,-1,-1,-1};

// encodes 1-3 bytes as 4 characters, padded with =
static void base64EncodeGroup(const byte_t *in, size_t size, char *out) {
	byte_t	data[3];
	bytestring::copy(data,in,size);
	bytestring::zero(data+size,3-size);
	out[0]=b64code[data[0]>>2];
	out[1]=b64code[(data[0]<<4|data[1]>>4)&0x3F];
	out[2]=(size>1)?b64code[(data[1]<<2|data[2]>>6)&0x3F]:'=';
	out[3]=(size>2)?b64code[data[2]&0x3F]:'=';
}

// decodes 1-4 characters to 3 bytes, a missing character counts as = and
// an invalid one (including bytes >= 0x80) as all one bits
static void base64DecodeGroup(const char *in, size_t size, byte_t *out) {
	byte_t	data[4];
	for (size_t i=0; i<4; i++) {
		byte_t	c=(i<size)?(byte_t)in[i]:(byte_t)'=';
		data[i]=(c<sizeof(b64dcode))?(byte_t)b64dcode[c]:0xFF;
	}
	out[0]=data[0]<<2|data[1]>>4;
	out[1]=(data[1]&0x0F)<<4|data[2]>>2;
	out[2]=(data[2]&0x03)<<6|data[3];
}

void charstring::base64Encode(const byte_t *input,
					char **output, uint64_t *outputlen) {
	base64Encode(input,getLength((const char *)input),output,outputlen);
}

void charstring::base64Encode(const byte_t *input, uint64_t inputsize,
					char **output, uint64_t *outputlen) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputlen=0;
		return;
	}

	// handle 0-length input
	if (!inputsize) {
		*output=duplicate("");
		*outputlen=0;
		return;
	}

	// size the output, 4 characters per group of up to 3 bytes
	uint64_t	groups=inputsize/3+((inputsize%3)?1:0);
	size_t		size;
	if (!bufferSize(groups,4,1,&size)) {
		*output=NULL;
		*outputlen=0;
		return;
	}
	*outputlen=groups*4;
	*output=new char[size];

	char	*oi=*output;
	for (uint64_t ii=0; ii<inputsize; ii+=3) {
		uint64_t	remaining=inputsize-ii;
		base64EncodeGroup(input+ii,(remaining<3)?remaining:3,oi);
		oi+=4;
	}
	*oi='\0';
}

void charstring::base64Encode(const byte_t *input, output *out) {
	base64Encode(input,getLength((const char *)input),out);
}

void charstring::base64Encode(const byte_t *input, uint64_t inputsize,
							output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	char	group[4];
	for (uint64_t ii=0; ii<inputsize && !cw.failed; ii+=3) {
		uint64_t	remaining=inputsize-ii;
		base64EncodeGroup(input+ii,(remaining<3)?remaining:3,group);
		chunkedWriterAppend(&cw,group,4);
	}
	chunkedWriterFlush(&cw);
}

byte_t *charstring::base64Decode(const char *input) {
	return base64Decode(input,getLength(input));
}

byte_t *charstring::base64Decode(const char *input, uint64_t inputlen) {
	byte_t		*retval=NULL;
	uint64_t	retvalsize=0;
	base64Decode(input,inputlen,&retval,&retvalsize);
	return retval;
}

void charstring::base64Decode(const char *input,
				byte_t **output, uint64_t *outputsize) {
	base64Decode(input,getLength(input),output,outputsize);
}

void charstring::base64Decode(const char *input, uint64_t inputlen,
				byte_t **output, uint64_t *outputsize) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle 0-length input
	if (!inputlen) {
		*output=(byte_t *)duplicate("");
		*outputsize=0;
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// size the output, 3 bytes per group of up to 4 characters, which
	// cannot fail once the length above fits
	uint64_t	groups=inputlen/4+((inputlen%4)?1:0);
	bufferSize(groups,3,1,&size);
	*output=new byte_t[size];
	uint64_t	outputindex=0;
	uint64_t	inputindex=0;
	while (inputindex<inputlen) {

		// ignore whitespace
		if (character::isWhitespace((byte_t)input[inputindex])) {
			inputindex++;
			continue;
		}

		uint64_t	remaining=inputlen-inputindex;
		if (remaining>4) {
			remaining=4;
		}
		base64DecodeGroup(input+inputindex,remaining,
						(*output)+outputindex);
		inputindex+=remaining;
		outputindex+=3;
	}
	(*output)[outputindex]='\0';

	// reset the outputsize
	(*outputsize)=outputindex+1;

	// NOTE: The below seemed correct at one point, but doesn't appear to
	// be now that we're skipping whitespace in the loop above.  Lets keep
	// it around just in case though.

	// the output could contain some trailing \0's that are artifacts of
	// the ='s in the encoded data, no big deal if the decoded data is
	// supposed to be a string, but not ok for binary data.  Reduce
	// outputsize accordingly.

	// there can be at most 2 trailing ='s, each equal represents an
	// additional trailing NULL, reduce outputsize accordingly
	/*(*outputsize)-=(input[inputlen-1]=='=')+
				(input[inputlen-2]=='=');*/
}

void charstring::base64Decode(const char *input, output *out) {
	base64Decode(input,getLength(input),out);
}

void charstring::base64Decode(const char *input, uint64_t inputlen,
							output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,true);
	byte_t		group[3];
	uint64_t	inputindex=0;
	while (inputindex<inputlen && !cw.failed) {

		// ignore whitespace
		if (character::isWhitespace((byte_t)input[inputindex])) {
			inputindex++;
			continue;
		}

		uint64_t	remaining=inputlen-inputindex;
		if (remaining>4) {
			remaining=4;
		}
		base64DecodeGroup(input+inputindex,remaining,group);
		chunkedWriterAppend(&cw,group,3);
		inputindex+=remaining;
	}
	chunkedWriterFlush(&cw);
}

// quoted-printable-encodes "b", returns the characters written, at most 6
// (the soft line break comes first once "*column" reaches 77)
static size_t quotedPrintableEncodeByte(byte_t b, uint16_t *column,
								char *out) {
	size_t	size=0;

	// soft line break
	if (*column>=77) {
		out[size++]='=';
		out[size++]='\r';
		out[size++]='\n';
		*column=0;
	}

	// hex-encode control characters, non-ascii bytes, and =
	if (b<' ' || b>'~' || b=='=') {
		out[size++]='=';
		hexEncodeByte(b,out+size);
		size+=2;
		(*column)+=3;
	} else {
		out[size++]=(char)b;
		(*column)++;
	}
	return size;
}

// decodes the = sequence at "*c" and advances it, returns 1 for a byte,
// 0 for a soft line break or dropped invalid pair, -1 if the input ends first
static int8_t quotedPrintableDecodeSequence(const char **c,
						const char *end, byte_t *b) {

	// skip the =
	const char	*ptr=(*c)+1;
	if (ptr==end || !*ptr) {
		*c=ptr;
		return -1;
	}

	// get the 1 or 2 characters after it
	char	char1=*ptr;
	char	char2='\0';
	ptr++;
	if (ptr!=end && *ptr) {
		char2=*ptr;
	} else if (char1!='\r' && char1!='\n') {
		*c=ptr;
		return -1;
	}

	// handle soft line breaks
	if (char1=='\n' || char1=='\r') {
		if ((char1=='\n' && char2=='\r') ||
			(char1=='\r' && char2=='\n')) {
			ptr++;
		}
		*c=ptr;
		return 0;
	}

	// handle hex pairs
	*c=ptr+1;
	byte_t	sixteens;
	byte_t	ones;
	if (!hexDigitValue(char1,&sixteens) || !hexDigitValue(char2,&ones)) {
		return 0;
	}
	*b=sixteens*16+ones;
	return 1;
}

char *charstring::quotedPrintableEncode(const byte_t *input) {
	return quotedPrintableEncode(input,getLength((const char *)input));
}

char *charstring::quotedPrintableEncode(const byte_t *input,
						uint64_t inputsize) {
	char		*retval=NULL;
	uint64_t	retvalsize=0;
	quotedPrintableEncode(input,inputsize,&retval,&retvalsize);
	return retval;
}

void charstring::quotedPrintableEncode(const byte_t *input,
						char **output,
						uint64_t *outputlen) {
	quotedPrintableEncode(input,getLength((const char *)input),
							output,outputlen);
}

void charstring::quotedPrintableEncode(const byte_t *input,
						uint64_t inputsize,
						char **output,
						uint64_t *outputlen) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputlen=0;
		return;
	}

	// handle 0-length input
	if (!inputsize) {
		*output=duplicate("");
		*outputlen=0;
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputsize,1,1,&size)) {
		*output=NULL;
		*outputlen=0;
		return;
	}

	// handle real input...
	stringbuffer	out;
	uint16_t	column=0;
	char		encoded[6];
	const byte_t	*end=input+inputsize;
	for (const byte_t *c=input; c<end; c++) {
		out.append(encoded,
			quotedPrintableEncodeByte(*c,&column,encoded));
	}

	*outputlen=out.getSize();
	*output=out.detachString();
}

void charstring::quotedPrintableEncode(const byte_t *input, output *out) {
	quotedPrintableEncode(input,getLength((const char *)input),out);
}

void charstring::quotedPrintableEncode(const byte_t *input,
						uint64_t inputsize,
						output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	uint16_t	column=0;
	char		encoded[6];
	const byte_t	*end=input+inputsize;
	for (const byte_t *c=input; c<end && !cw.failed; c++) {
		chunkedWriterAppend(&cw,encoded,
			quotedPrintableEncodeByte(*c,&column,encoded));
	}
	chunkedWriterFlush(&cw);
}

byte_t *charstring::quotedPrintableDecode(const char *input) {
	return quotedPrintableDecode(input,getLength(input));
}

byte_t *charstring::quotedPrintableDecode(const char *input,
						uint64_t inputlen) {
	byte_t		*retval=NULL;
	uint64_t	retvalsize=0;
	quotedPrintableDecode(input,inputlen,&retval,&retvalsize);
	return retval;
}

void charstring::quotedPrintableDecode(const char *input,
					byte_t **output,
					uint64_t *outputsize) {
	quotedPrintableDecode(input,getLength(input),output,outputsize);
}

void charstring::quotedPrintableDecode(const char *input,
					uint64_t inputlen,
					byte_t **output,
					uint64_t *outputsize) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle 0-length input
	if (!inputlen) {
		*output=(byte_t *)duplicate("");
		*outputsize=0;
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle real input...
	bytebuffer	out;
	const char	*end=input+inputlen;
	const char	*c=input;
	byte_t		b;
	while (c!=end && *c) {
		if (*c=='=') {
			int8_t	result=
				quotedPrintableDecodeSequence(&c,end,&b);
			if (result==-1) {
				break;
			}
			if (result) {
				out.append(b);
			}
		} else {
			out.append(*c);
			c++;
		}
	}

	// null-terminate, in case it's a string, but don't include the
	// null-terminator in the output size
	out.append('\0');

	*outputsize=out.getSize()-1;
	*output=out.detachBuffer();
}

void charstring::quotedPrintableDecode(const char *input, output *out) {
	quotedPrintableDecode(input,getLength(input),out);
}

void charstring::quotedPrintableDecode(const char *input,
					uint64_t inputlen,
					output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,true);
	const char	*end=input+inputlen;
	const char	*c=input;
	byte_t		b;
	while (c!=end && *c && !cw.failed) {
		if (*c=='=') {
			int8_t	result=
				quotedPrintableDecodeSequence(&c,end,&b);
			if (result==-1) {
				break;
			}
			if (result) {
				chunkedWriterAppend(&cw,&b,1);
			}
		} else {
			chunkedWriterAppend(&cw,c,1);
			c++;
		}
	}
	chunkedWriterFlush(&cw);
}

char *charstring::hexEncode(const byte_t *input) {
	return hexEncode(input,getLength((const char *)input));
}

char *charstring::hexEncode(const byte_t *input, uint64_t inputsize) {
	char		*retval=NULL;
	uint64_t	retvalsize=0;
	hexEncode(input,inputsize,&retval,&retvalsize);
	return retval;
}

void charstring::hexEncode(const byte_t *input,
					char **output, uint64_t *outputlen) {
	hexEncode(input,getLength((const char *)input),output,outputlen);
}

void charstring::hexEncode(const byte_t *input, uint64_t inputsize,
					char **output, uint64_t *outputlen) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputlen=0;
		return;
	}

	// handle 0-length input
	if (!inputsize) {
		*output=duplicate("");
		*outputlen=0;
		return;
	}

	// size the output
	size_t	size;
	if (!bufferSize(inputsize,2,1,&size)) {
		*output=NULL;
		*outputlen=0;
		return;
	}

	*outputlen=inputsize*2;
	*output=new char[size];
	char	*oi=*output;
	for (uint64_t ii=0; ii<inputsize; ii++) {
		hexEncodeByte(input[ii],oi);
		oi+=2;
	}
	*oi='\0';
}

void charstring::hexEncode(const byte_t *input, output *out) {
	hexEncode(input,getLength((const char *)input),out);
}

void charstring::hexEncode(const byte_t *input, uint64_t inputsize,
							output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,false);
	char	hex[2];
	for (uint64_t ii=0; ii<inputsize && !cw.failed; ii++) {
		hexEncodeByte(input[ii],hex);
		chunkedWriterAppend(&cw,hex,2);
	}
	chunkedWriterFlush(&cw);
}

byte_t *charstring::hexDecode(const char *input) {
	return hexDecode(input,getLength(input));
}

byte_t *charstring::hexDecode(const char *input, uint64_t inputlen) {
	byte_t		*retval=NULL;
	uint64_t	retvalsize=0;
	hexDecode(input,inputlen,&retval,&retvalsize);
	return retval;
}

void charstring::hexDecode(const char *input,
				byte_t **output, uint64_t *outputsize) {
	hexDecode(input,getLength(input),output,outputsize);
}

void charstring::hexDecode(const char *input, uint64_t inputlen,
				byte_t **output, uint64_t *outputsize) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle 0-length input
	if (!inputlen) {
		*output=(byte_t *)duplicate("");
		*outputsize=0;
		return;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(inputlen,1,1,&size)) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// deal with invalid input lengths
	if (inputlen%2) {
		inputlen--;
	}

	// create output buffer and init output size, which cannot fail
	// once the length above fits
	bufferSize(inputlen/2,1,1,&size);
	*output=new byte_t[size];
	*outputsize=0;

	// convert...
	const char	*end=input+inputlen;
	while (input!=end) {
		(*output)[*outputsize]=hexDecodePair(input);
		(*outputsize)++;
		input+=2;
	}
	(*output)[*outputsize]='\0';
}

void charstring::hexDecode(const char *input, output *out) {
	hexDecode(input,getLength(input),out);
}

void charstring::hexDecode(const char *input, uint64_t inputlen,
							output *out) {

	// handle null input or output
	if (!input || !out) {
		return;
	}

	// deal with invalid input lengths
	if (inputlen%2) {
		inputlen--;
	}

	// convert...
	chunkedwriter	cw;
	chunkedWriterInit(&cw,out,true);
	const char	*end=input+inputlen;
	while (input!=end && !cw.failed) {
		byte_t	b=hexDecodePair(input);
		chunkedWriterAppend(&cw,&b,1);
		input+=2;
	}
	chunkedWriterFlush(&cw);
}

char *charstring::insertString(const char *dest,
				const char *src, uint64_t index) {

	uint64_t	srcsize=getLength(src);
	uint64_t	size=getLength(dest)+srcsize+1;
	size_t		bufsize;
	if (!bufferSize(size,1,0,&bufsize)) {
		return NULL;
	}
	char		*retval=new char[bufsize];
	for (uint64_t i=0,j=0; i<size;) {
		if (i==index) {
			for (uint64_t k=0; k<srcsize; k++) {
				retval[i++]=src[k];
			}
		} else {
			retval[i++]=dest[j++];
		}
	}
	retval[size-1]='\0';
	return retval;
}

void charstring::obfuscate(char *str) {
	for (char *ch=str; *ch; ch++) {
		*ch=(*ch)+128;
	}
}

void charstring::deobfuscate(char *str) {
	for (char *ch=str; *ch; ch++) {
		*ch=(*ch)-128;
	}
}

char *charstring::pad(const char *str, char padchar,
			int16_t direction, uint64_t totallength) {

	if (totallength==0) {
		return NULL;
	}

	// check the size before reading the input
	size_t	size;
	if (!bufferSize(totallength,1,1,&size)) {
		return NULL;
	}

	uint64_t	strlen=((str==NULL)?0:getLength(str));
	char		*newstring=NULL;

	newstring=new char[size];
	if (strlen>=totallength) {
		copy(newstring,str,totallength);
		newstring[totallength]=0;
		return newstring;
	}

	bytestring::set(newstring,padchar,totallength);
	newstring[totallength]=0;

	if (direction<0) {
		// pad left
		copy(&newstring[totallength-strlen],str,strlen);
	} else if (direction>0) {
		// pad right
		copy(newstring,str,strlen);
	} else {
		// pad center
		copy(&newstring[(totallength-strlen)/2],str,strlen);
	}

	return newstring;
}

char *charstring::getHumanReadable(int64_t number) {
	return getHumanReadable(number,false);
}

char *charstring::getHumanReadable(int64_t number, bool onethousand) {
	return getHumanReadable((long double)number,onethousand);
}

char *charstring::getHumanReadable(uint64_t number) {
	return getHumanReadable(number,false);
}

char *charstring::getHumanReadable(uint64_t number, bool onethousand) {
	return getHumanReadable((long double)number,onethousand);
}

char *charstring::getHumanReadable(long double number) {
	return getHumanReadable(number,false);
}

char *charstring::getHumanReadable(long double number, bool onethousand) {

	long double	k=(onethousand)?1000.0:1024.0;
	char		suffixes[]={
		'\0','K','M','G','T','P','Z','Y','B'
	};
	long double	num=(number>=0)?number:(number*-1.0);
	long double	size=k*k*k*k*k*k*k*k;
	uint8_t i=8;
	do {
		if (num>=size) {
			break;
		}
		size/=k;
		i--;
	} while (i>0);
	
	char	*buf=NULL;
	printf(&buf,
		#ifdef _HPUX
		"%0.1lf%c",
		#else
		"%0.1Lf%c",
		#endif
		number/size,suffixes[i]);
	char	*subbed=replace(buf,".0","");
	delete[] buf;
	return subbed;
}

ssize_t charstring::printf(char *buffer, size_t len,
					const char *format, ...) {
	va_list	args;
	va_start(args,format);
	size_t	result=printf(buffer,len,format,&args);
	va_end(args);
	return result;
}

#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE__VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED_VSNPRINTF_S)
// This is quite a hack...
//
// Old enough systems (like linux libc4) don't provide vsnprintf but do provide
// vsprintf.  There's no safe way to use vsprintf though, especially the way
// the bytebuffer class would like to use it.
//
// I could grab a vsnprintf implementation from any of the other
// LGPL-compatible libraries.  Tried it.  Too much work.
//
// vsnprintf could be implemented using vsprintf but a large enough buffer to
// vsprintf to safely must be created.  What is "large enough" though?
//
// I could implement a format string parser that calculates the buffer
// size.  Tried it.  Also too much work.
// 
// Short of that, the only safe thing to do is vfprintf to a file, find out how
// many characters were written and then do the same to a string.
// 
// That's taking the long way around for sure.
//
// The null device is the obvious choice, so we'll try that first.  But, some
// implementations of vsprintf return 0 when writing to the null device.  Or,
// maybe some implementations of the null device return 0 when anything is
// written to them.  Either way, if using the null device fails, we fall back
// to a scratch file.
//
// I'm not even going to benchmark to find out how poorly this performs.
// Hopefully disk-caching will help it out.  Also, if you happen to be using
// a ram-based temporary directory then that will help too.  Systems old enough
// to need this probably aren't though.
//
// There are, of course, security concerns with the scratch file.  Anyone with
// the right permissions can read the scratch file.  Someone could delete it,
// and on some systems that could cause problems.
//
// The scratch file uses the PID of the current process for uniqueness.  This
// could cause race conditions in multi-threaded programs, but chances are if
// your system doesn't have vsnprintf, then it doesn't have thread support
// either.
//
// While terribly inefficient, the scratch file process should be clean.  It
// only creates one scratch file per process and cleans it up at exit, unless
// the program crashes or is killed with -9.

static char	*scratchfile=NULL;
static FILE	*scratch=NULL;

static void removeScratch() {
	file::remove(scratchfile);
	if (scratch) {
		fclose(scratch);
		delete[] scratchfile;
	}
}

static ssize_t vsnprintf(char *buffer, size_t len,
				const char *format, va_list argp) {

	// open a scratch file if it's not already open
	if (!scratch) {

		// first try the null device
		scratchfile=charstring::duplicate(
					#if defined(_WIN32)
						"\Device\Null"
					#elif defined(__VMS)
						"NLA0:"
					#else
						"/dev/null"
					#endif
						);
		scratch=fopen(scratchfile,"w");
		if (scratch) {
			// writing to the null device returns
			// 0 or -1 on some platforms
			if (fprintf(scratch,"test")!=4) {
				fclose(scratch);
				scratch=NULL;
				delete[] scratchfile;
			}
		} else {
			delete[] scratchfile;
		}

		// if that fails then try /tmp/scratch.pid
		if (!scratch) {
			scratchfile=new char[20];
			charstring::copy(scratchfile,"/tmp/scratch.");
			charstring::append(scratchfile,
					(uint64_t)process::getProcessId());
			scratch=fopen(scratchfile,"w+");
			if (scratch) {
				process::registerExitHandler((void (*)(void))removeScratch);
				rewind(scratch);
			} else {
				delete[] scratchfile;
				return -1;
			}
		}
	}


	// write to the scratch file so we can
	// figure out how much space we need
	//
	// argp gets consumed by va_arg() here and again below by vsprintf(),
	// and on platforms where va_list is itself an array type (so this
	// by-value parameter is really a reference to the caller's va_list),
	// reusing it as-is for both calls is undefined behavior - see the
	// va_copy() comment above charstring::printf()'s vsnprintf() calls
	// for the mechanism.  Each call gets its own copy instead.
	va_list	argp1;
	va_copy(argp1,argp);
	ssize_t	safebuffersize=vfprintf(scratch,format,argp1);
	va_end(argp1);

	// create a big enough buffer for that
	char	*safebuffer=new char[safebuffersize+1];

	// vsprintf to safebuffer
	va_list	argp2;
	va_copy(argp2,argp);
	ssize_t byteswritten=vsprintf(safebuffer,format,argp2);
	va_end(argp2);

	// bail on error
	if (byteswritten==-1) {
		delete[] safebuffer;
		return -1;
	}

	// figure out how many bytes we can copy back to "buffer"
	// (the +1 is because vsprintf returns the number of bytes written
	// minus the NULL terminator)
	size_t	bytestocopy=((size_t)byteswritten+1<len)?
					byteswritten+1:len;

	// copy what we can back to "buffer"
	charstring::copy(buffer,safebuffer,bytestocopy);

	// clean up
	delete[] safebuffer;

	// return the number of bytes we would like to have copied
	// (except for the NULL terminator)
	return safebuffersize;
}

// now we can say that we have vsnprintf
#define RUDIMENTS_HAVE_VSNPRINTF 1

#endif

ssize_t charstring::printf(char *buffer, size_t len,
					const char *format, va_list *argp) {

	// vsnprintf should write whatever will fit into "buffer" and
	// either return the number of bytes that were written or the
	// number of bytes that would have been written if truncation
	// hadn't occurred.

	// Attempt to write directly to the provided buffer...
	char	*buf=buffer;
	size_t	buflen=len;
	// Some implementations (like linux libc5) crash if "buffer" is NULL
	// and corrupt memory if "buffer" is only 1 character.  Use a buffer
	// of at least two characters to manage those cases.
	char	b[2];
	if (!buf || buflen<2) {
		buf=b;
		buflen=2;
	}
	error::clearError();
	// vsnprintf() consumes *argp via va_arg(), leaving it indeterminate
	// afterward, but this function may need to call vsnprintf() more than
	// once below (once here, then again per retry iteration further down)
	// if the platform's vsnprintf() doesn't report truncation the way we
	// expect.  Reusing *argp itself across those calls is undefined
	// behavior - on some ABIs it hands a later call a stale argument
	// pointer instead of the real one, which is exactly the kind of bug
	// that sends a %s conversion scanning off into unrelated memory
	// looking for a NUL.  Take a fresh va_copy() for each individual
	// vsnprintf() call instead, so *argp itself is never directly
	// consumed and stays valid for as many copies as we need.
	va_list	argp1;
	va_copy(argp1,*argp);
	#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
		ssize_t	size=vsnprintf_s(buf,buflen,_TRUNCATE,format,argp1);
	#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,argp1);
	#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
		ssize_t	size=_vsnprintf(buf,buflen,format,argp1);
	#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
		ssize_t	size=vsnprintf(buf,buflen,format,argp1);
	#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,argp1);
		// Solaris 2.5.1 (and maybe others) return buflen-1 if
		// truncation occurs.  In that case, simulate systems
		// that return -1 if truncation occurs.
		if (size==(ssize_t)buflen-1) {
			size=-1;
		}
	#else
		#error no vsnprintf or anything like it
	#endif
	va_end(argp1);

	// Return "size" if either:
	// * "size" > -1 (no error occurred)
	// * "size" <= -1 and there was an error
	if (size>-1) {
		// if we had to use that 2-byte buffer hack above,
		// then copy out from it to the provided buffer
		if (buf==b) {
			if ((size_t)size>len) {
				// just copy out what we can
				copy(buffer,buf,len);
			} else {
				// copy out everything, including
				// the NULL terminator
				copy(buffer,buf,size+1);
			}
		}
		return size;
	} else if (error::getErrorNumber()) {
		return size;
	}

	// If "size" <= -1 and there was no error though...

	// Some implementations (SCO OSR6, Redhat 5.2, probably others) return
	// -1 if truncation occurred and don't write anything to "buffer".  This
	// must be one of those systems.

	// To simulate the expected behavior of writing truncated data to
	// "buffer" and returning the number of bytes that would have been
	// written if truncation had not occured, we need to loop, attempting
	// to write to ever-larger buffers until we achieve success, copy out
	// what we can to the original buffer, and return the number of bytes
	// that would have been written if truncation hadn't occurred.
	buflen=len;
	size_t	inc=16;
	error::clearError();
	do {
		buflen+=inc;
		buf=new char[buflen+1];

		// each retry needs its own fresh copy too - see comment
		// on the first attempt above
		va_list	argp2;
		va_copy(argp2,*argp);
		#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
			size=vsnprintf_s(buf,buflen,_TRUNCATE,format,argp2);
		#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,argp2);
		#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
			size=_vsnprintf(buf,buflen,format,argp2);
		#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
			size=vsnprintf(buf,buflen,format,argp2);
		#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,argp2);
			// Solaris 2.5.1 (and maybe others) return buflen if
			// truncation occurs.  In that case, simulate systems
			// that return -1 if truncation occurs.
			if (size==(ssize_t)buflen-1) {
				size=-1;
			}
		#else
			#error no vsnprintf or anything like it
		#endif
		va_end(argp2);
		if (size>-1) {
			if ((size_t)(size+1)>len) {
				// just copy out what we can
				copy(buffer,buf,len);
			} else {
				// copy out everything, including
				// the NULL terminator
				copy(buffer,buf,size+1);
			}
			delete[] buf;
			break;
		}

		// clean up
		delete[] buf;

		// adjust how quickly the buffer grows
		// (this can certainly be optimized further)
		inc*=2;
		if (inc>1024) {
			inc=1024;
		}
	} while (!error::getErrorNumber());
	return size;
}

ssize_t charstring::printf(char **buffer, const char *format, ...) {
	va_list	args;
	va_start(args,format);
	// the printf() below will call va_end(args)
	return printf(buffer,format,&args);
}

ssize_t charstring::printf(char **buffer, const char *format, va_list *argp) {

	// sanity check
	if (!buffer) {
		return 0;
	}

	// initialize *buffer
	*buffer=NULL;

	// Ideally we'd use vasprintf() if it's available, but we want to be
	// able to delete[] buffer, and if we use vasprintf() to create it then
	// we'd have to free(buffer).

	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// whether it's the global function printf() or one that you've defined
	// yourself.  Using *buffer here works around that.
	va_list	argp1;
	va_copy(argp1,*argp);
	ssize_t	size=printf(*buffer,0,format,argp);
	va_end(*argp);
	if (size!=-1) {
		*buffer=new char[size+1];
		size=printf(*buffer,size+1,format,&argp1);
	}
	va_end(argp1);
	return size;
}
