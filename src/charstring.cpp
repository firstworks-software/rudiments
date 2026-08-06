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

// appends the "index"'th substring that "from" matched to "newstring".  A
// group that didn't participate in the match, or that the pattern doesn't
// have at all, contributes nothing.
static void appendSubstring(stringbuffer *newstring,
					regularexpression *from,
					int32_t index) {
	const char	*substart=from->getSubstringStart(index);
	if (substart) {
		newstring->append(substart,
				from->getSubstringEnd(index)-substart);
	}
}

// resolves the body of a \g<...> backref - a non-empty, all-digits body is a
// group number, anything else is a group name - to a group index, or to -1 if
// the pattern has no group by that name
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

	// A number past the group count is out of range no matter what digits
	// follow it, so the scan stops growing the index there.  That also
	// keeps a long run of digits from overflowing it.
	int32_t	count=from->getSubstringCount();
	int32_t	index=0;
	for (const char *d=start; d<end && index<count; d++) {
		index=index*10+(*d-'0');
	}
	return index;
}

// appends "to" to "newstring", expanding \0 through \9, \g<number>, and
// \g<name> into the substrings that "from" matched, and \\ into a single
// backslash
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

		// \g<...> refers to a group by number or by name, reaching
		// the groups past the ninth, which a single digit can't name
		if (*ptr=='g' && *(ptr+1)=='<') {

			// find the closing >
			const char	*bodystart=ptr+2;
			const char	*bodyend=bodystart;
			while (*bodyend && *bodyend!='>') {
				bodyend++;
			}

			// an unterminated form is kept as it was, like any
			// other escape this method doesn't know.  The rest
			// of it is appended by the normal path.
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

		// anything but a backslash or a digit isn't an escape this
		// method knows, so both characters are kept as they were
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

	// Walk every match with the regularexpression class rather than
	// resuming by hand.  It knows where the next match starts, it keeps
	// the engine from thinking each resume point is the start of the
	// subject - so ^, \b, and lookbehind still work - and it doesn't
	// re-copy the subject at every step the way a loop over
	// match(str,length,offset) does on the platforms without PCRE.
	int32_t	start=0;
	for (bool matched=from->match(str,getLength(str));
			matched; matched=from->matchNext()) {

		// get the bounds of the matching chunk
		int32_t	fromstart=from->getSubstringStartOffset(0);
		int32_t	fromend=from->getSubstringEndOffset(0);

		// Skip the match if it's empty.  matchNext() reports empty
		// matches, but replacing one would insert "to" between every
		// pair of characters, so a substitution steps over them.  A
		// match can also end before it starts - pcre1 reports that
		// for a \K inside a lookahead - which counts as empty too.
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

char *charstring::urlEncode(const char *input) {

	if (!input) {
		return NULL;
	}

	size_t		outputlen=getLength(input)*3+1;
	char		*output=new char[outputlen];
	char		*outptr=output;
	const byte_t	*ptr=(byte_t *)input;
	
	while (*ptr) {
		if (*ptr==' ') {
			(*outptr)='+';
		} else if (character::isAlphanumeric(*ptr) ||
				character::isInSet(*ptr,"$-_.!*'(),")) {
			(*outptr)=*ptr;
		} else {
			(*outptr)='%';
			outptr++;
			outputlen--;
			byte_t	upper=(*ptr)>>4;
			if (upper<10) {
				*outptr=upper+'0';
			} else {
				*outptr=upper-10+'A';
			}
			outptr++;
			outputlen--;
			byte_t	lower=(*ptr)&0x0F;
			if (lower<10) {
				*outptr=lower+'0';
			} else {
				*outptr=lower-10+'A';
			}
		}
		outptr++;
		outputlen--;
		ptr++;
	}
	(*outptr)='\0';

	return output;
}

char *charstring::urlDecode(const char *input) {

	if (!input) {
		return NULL;
	}

	char		*output=new char[getLength(input)+1];
	char		*outptr=output;
	const char	*ptr=input;
	
	while (*ptr) {
		if (*ptr=='+') {
			(*outptr)=' ';
		} else if (*ptr=='%') {
			ptr++;
			char	hex[5];
			hex[0]='0';
			hex[2]='x';
			if (*ptr) {
				hex[0]=*ptr;
				ptr++;
				if (*ptr) {
					hex[1]=*ptr;
				} else {
					(*outptr)='%';
					outptr++;
					(*outptr)=*ptr;
					outptr++;
					break;
				}
				hex[2]='\0';
				char	ch=convertToInteger(hex,16);
				(*outptr)=ch;
			} else {
				(*outptr)='%';
				outptr++;
				break;
			}
		} else {
			(*outptr)=*ptr;
		}
		outptr++;
		ptr++;
	}
	(*outptr)='\0';

	return output;
}

char *charstring::escape(const char *input, const char *characters) {
	char		*output;
	uint64_t	outputlength;
	escape(input,getLength(input),&output,&outputlength,characters);
	return output;
}

void charstring::escape(const char *input, uint64_t inputlength,
				char **output, uint64_t *outputlength,
						const char *characters) {

	(*output)=NULL;
	(*outputlength)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputlength;
				inputindex++) {
			if (contains(characters,input[inputindex]) ||
						input[inputindex]=='\\') {
				if (pass==0) {
					(*outputlength)++;
				} else {
					(*output)[outputindex]='\\';
					outputindex++;
				}
			}
			if (pass==0) {
				(*outputlength)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
		}
		if (pass==0) {
			(*output)=new char[(*outputlength)+1];
			(*output)[(*outputlength)]='\0';
		}
	}
}

char *charstring::unescape(const char *input) {
	char		*output;
	uint64_t	outputsize;
	unescape(input,getLength(input),&output,&outputsize);
	return output;
}

void charstring::unescape(const char *input, uint64_t inputlength,
				char **output, uint64_t *outputlength) {

	(*output)=NULL;
	(*outputlength)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		bool		escaped=false;
		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputlength;
				inputindex++) {
			if (!escaped && input[inputindex]=='\\') {
				escaped=true;
				continue;
			}
			if (pass==0) {
				(*outputlength)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
			escaped=false;
		}
		if (pass==0) {
			(*output)=new char[(*outputlength)+1];
			(*output)[(*outputlength)]='\0';
		}
	}
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
	char	*buffer=new char[len+1];
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
	char	*retval=new char[len+1];
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
	char	*retval=new char[len+1];
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

	// handle real input...

	// figure out the output length:
	//
	// let x = inputsize
	// let y = outputlen
	//
	// (I know this sequence is true by
	//  observation of input vs output sizes)
	// x   y
	// -----
	// 9   12 = (x*4/3)+0
	// 8   12 = (x*4/3)+2
	// 7   12 = (x*4/3)+3
	// 6   8  = (x*4/3)+0
	// 5   8  = (x*4/3)+2
	// 4   8  = (x*4/3)+3
	// 3   4  = (x*4/3)+0
	// 2   4  = (x*4/3)+2
	// 1   4  = (x*4/3)+3
	//
	// y=((x*4/3)+z)
	//
	// x   z
	// -----
	// 9   0 = 9-x+0
	// 8   2 = 9-x+1
	// 7   3 = 9-x+1
	// 6   0 = 6-x+0
	// 5   2 = 6-x+1
	// 4   3 = 6-x+1
	// 3   0 = 3-x+0
	// 2   2 = 3-x+1
	// 1   3 = 3-x+1
	//
	// z=(a-x+b)
	//
	// x   a
	// -----
	// 9   9 = ((x+2)/3)*3
	// 8   9 = ((x+2)/3)*3
	// 7   9 = ((x+2)/3)*3
	// 6   6 = ((x+2)/3)*3
	// 5   6 = ((x+2)/3)*3
	// 4   6 = ((x+2)/3)*3
	// 3   3 = ((x+2)/3)*3
	// 2   3 = ((x+2)/3)*3
	// 1   3 = ((x+2)/3)*3
	//
	// a=(((x+2)/3)*3)
	//
	// x   b
	// -----
	// 9   0 = 1-((x+2)%3/2)
	// 8   1 = 1-((x+2)%3/2)
	// 7   1 = 1-((x+2)%3/2)
	// 6   0 = 1-((x+2)%3/2)
	// 5   1 = 1-((x+2)%3/2)
	// 4   1 = 1-((x+2)%3/2)
	// 3   0 = 1-((x+2)%3/2)
	// 2   1 = 1-((x+2)%3/2)
	// 1   1 = 1-((x+2)%3/2)
	//
	// b=(1-((x+2)%3/2))
	//
	// z=(a-x+b)
	// z=(a-x+(1-((x+2)%3/2)))
	// a=(((x+2)/3)*3)
	// z=((((x+2)/3)*3)-x+(1-((x+2)%3/2)))
	// y=((x*4/3)+z)
	// y=((x*4/3)+((((x+2)/3)*3)-x+(1-((x+2)%3/2))))
	//
	// yay!
	*outputlen=((inputsize*4/3)+((((inputsize+2)/3)*3)-
				inputsize+(1-((inputsize+2)%3/2))));
	*output=new char [(*outputlen)+1];

	uint64_t	outputindex=0;
	byte_t		data[3];
	uint64_t	bytesremaining=0;
	for (uint64_t inputindex=0;
			inputindex<inputsize;
			inputindex=inputindex+3) {

		bytesremaining=inputsize-inputindex;
		if (bytesremaining>=3) {
			bytestring::copy(data,input+inputindex,3);
		} else {
			bytestring::copy(data,input+inputindex,bytesremaining);
			bytestring::zero(data+bytesremaining,3-(bytesremaining));
		}

		(*output)[outputindex++]=b64code[data[0]>>2];
		(*output)[outputindex++]=b64code[(data[0]<<4|data[1]>>4)&0x3F];
		(*output)[outputindex++]=b64code[(data[1]<<2|data[2]>>6)&0x3F];
		(*output)[outputindex++]=b64code[data[2]&0x3F];
	}
	(*output)[outputindex]='\0';

	// convert any excess A's to ='s
	uint64_t	excess=3-bytesremaining;
	for (uint64_t i=0; i<excess; i++) {
		if ((*output)[--outputindex]=='A') {
			(*output)[outputindex]='=';
		}
	}
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

	// handle real input...
	*outputsize=inputlen*3/4;
	*output=new byte_t [(*outputsize)+1];
	uint64_t	outputindex=0;
	byte_t		data[4];
	uint64_t	inputindex=0;
	while (inputindex<inputlen) {

		// ignore whitespace
		if (character::isWhitespace(input[inputindex])) {
			inputindex++;
			continue;
		}

		data[0]=(byte_t)b64dcode[(int32_t)input[inputindex++]];
		data[1]=(byte_t)b64dcode[(int32_t)input[inputindex++]];
		data[2]=(byte_t)b64dcode[(int32_t)input[inputindex++]];
		data[3]=(byte_t)b64dcode[(int32_t)input[inputindex++]];

		(*output)[outputindex++]=data[0]<<2|data[1]>>4;
		(*output)[outputindex++]=(data[1]&0x0F)<<4|data[2]>>2;
		(*output)[outputindex++]=(data[2]&0x03)<<6|data[3];
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

	// handle real input...
	stringbuffer	out;
	uint16_t	index=0;
	const byte_t	*end=input+inputsize;
	for (const byte_t *c=input; c<end; c++) {

		if (index>=77) {
			// append soft-eol
			out.append("=\r\n");
			index=0;
		}

		if (*c<' ' || *c>'~' || *c=='=') {
			// hex encode
			out.printf("=%02x",*c);
			index+=3;
		} else {
			out.append(*c);
			index++;
		}
	}

	*outputlen=out.getSize();
	*output=out.detachString();
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

	// handle real input...
	bytebuffer	out;
	const char	*end=input+inputlen;
	const char *c=input;
	while (*c && c!=end) {

		if (*c=='=') {

			char	char1='\0';
			char	char2='\0';

			// move on
			c++;

			// bail if we hit the EOL
			if (c==end || *c=='\0') {
				break;
			}

			// get the char
			char1=*c;

			// move on
			c++;

			// if we hit EOL..
			if (c==end || *c=='\0') {
 
				// The only potentially valid case where the
				// second character past the = is the end of
				// line is a soft-eol.  Bail if we don't find
				// that.
				if (char1!='\r' && char1!='\n') {
					break;
				}

			} else {

				// get the char
				char2=*c;
			}

			// handle soft eol
			if (char1=='\n') {
				if (char2=='\r') {
					c++;
				} else {
					// don't increment c
				}

			} else if (char1=='\r') {
				if (char2=='\n') {
					c++;
				} else {
					// don't increment c
				}

			// handle hex chars
			} else {

				bool	good=true;

				byte_t	sixteens=character::upper(char1);
				if (sixteens>='A' && sixteens<='F') {
					sixteens=sixteens-'A'+10;
				} else if (sixteens>='0' && sixteens<='9') {
					sixteens-='0';
				} else {
					good=false;
				}

				byte_t	ones=character::upper(char2);
				if (ones>='A' && ones<='F') {
					ones=ones-'A'+10;
				} else if (ones>='0' && ones<='9') {
					ones-='0';
				} else {
					good=false;
				}

				// if both were valid hex chars then
				// append the hex value, otherwise ignore
				// the encoding altogether
				if (good) {
					out.append((byte_t)(sixteens*16+ones));
				}

				c++;
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

char *charstring::hexEncode(const byte_t *input) {
	return hexEncode(input,getLength((const char *)input));
}

char *charstring::hexEncode(const byte_t *input, uint64_t inputsize) {
	char		*retval=NULL;
	uint64_t	retvalsize=0;
	hexEncode(input,inputsize,&retval,&retvalsize);
	return retval;
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

	*outputlen=inputsize*2;
	*output=new char[*outputlen+1];
	char	*oi=*output;
	for (uint16_t ii=0; ii<inputsize; ii++) {
		printf(oi,3,"%02x",input[ii]);
		oi+=2;
	}
	*oi='\0';
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

	// deal with invalid input lengths
	if (inputlen%2) {
		inputlen--;
	}

	// create output buffer and init output size
	*output=new byte_t[inputlen/2+1];
	*outputsize=0;

	// convert...
	const char	*end=input+inputlen;
	while (input!=end) {
		byte_t	sixteens=*input;
		if (sixteens>='A' && sixteens<='F') {
			sixteens=sixteens-'A'+10;
		} else if (sixteens>='a' && sixteens<='f') {
			sixteens=sixteens-'a'+10;
		} else if (sixteens>='0' && sixteens<='9') {
			sixteens-='0';
		} else {
			sixteens=0;
		}
		input++;
		byte_t	ones=*input;
		if (ones>='A' && ones<='F') {
			ones=ones-'A'+10;
		} else if (ones>='a' && ones<='f') {
			ones=ones-'a'+10;
		} else if (ones>='0' && ones<='9') {
			ones-='0';
		} else {
			ones=0;
		}
		input++;
		(*output)[*outputsize]=sixteens*16+ones;
		(*outputsize)++;
	}
	(*output)[*outputsize]='\0';
}

char *charstring::insertString(const char *dest,
				const char *src, uint64_t index) {

	uint64_t	srcsize=getLength(src);
	uint64_t	size=getLength(dest)+srcsize+1;
	char		*retval=new char[size];
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

	uint64_t	strlen=((str==NULL)?0:getLength(str));
	char		*newstring=NULL;

	newstring=new char[totallength+1];
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
	ssize_t	safebuffersize=vfprintf(scratch,format,argp);

	// create a big enough buffer for that
	char	*safebuffer=new char[safebuffersize+1];

	// vsprintf to safebuffer
	ssize_t byteswritten=vsprintf(safebuffer,format,argp);

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
	#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
		ssize_t	size=vsnprintf_s(buf,buflen,_TRUNCATE,format,*argp);
	#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
		ssize_t	size=_vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
		ssize_t	size=vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,*argp);
		// Solaris 2.5.1 (and maybe others) return buflen-1 if
		// truncation occurs.  In that case, simulate systems
		// that return -1 if truncation occurs.
		if (size==(ssize_t)buflen-1) {
			size=-1;
		}
	#else
		#error no vsnprintf or anything like it
	#endif

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

		#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
			size=vsnprintf_s(buf,buflen,_TRUNCATE,format,*argp);
		#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
			size=_vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
			size=vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,*argp);
			// Solaris 2.5.1 (and maybe others) return buflen if
			// truncation occurs.  In that case, simulate systems
			// that return -1 if truncation occurs.
			if (size==(ssize_t)buflen-1) {
				size=-1;
			}
		#else
			#error no vsnprintf or anything like it
		#endif
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
