// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/bytestring.h>
#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
	#include <rudiments/process.h>
	#include <rudiments/file.h>
#endif
#include <rudiments/error.h>
#include <rudiments/sys.h>

#ifdef _WIN32
	#include <rudiments/wcharstring.h>
#endif

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
extern "C" int __vsnprintf(ucs2_t *str, size_t size,
				const ucs2_t *format, va_list ap);
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

const ucs2_t *ucs2charstring::findLast(const ucs2_t *haystack,
						const ucs2_t *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const ucs2_t	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compare(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findLastIgnoringCase(const ucs2_t *haystack,
						const ucs2_t *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const ucs2_t	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

void ucs2charstring::upper(ucs2_t *str) {
	if (str) {
		for (ucs2_t *ch=str; *ch; ch++) {
			*ch=(ucs2_t)ucs2character::upper(*ch);
		}
	}
}

void ucs2charstring::lower(ucs2_t *str) {
	if (str) {
		for (ucs2_t *ch=str; *ch; ch++) {
			*ch=(ucs2_t)ucs2character::lower(*ch);
		}
	}
}

void ucs2charstring::capitalize(ucs2_t *str) {
	ucs2_t	delims[]={
		(ucs2_t)' ',
		(ucs2_t)'\'',
		(ucs2_t)'"',
		(ucs2_t)'-',
		(ucs2_t)'(',
		(ucs2_t)'"',
		(ucs2_t)'\0'
	};
	if (str) {
		bool	cap=true;
		for (ucs2_t *ch=str; *ch; ch++) {
			if (cap) {
				*ch=(ucs2_t)ucs2character::upper(*ch);
				cap=false;
			} else {
				*ch=(ucs2_t)ucs2character::lower(*ch);
			}
			if (ucs2character::isInSet(*ch,delims)) {
				cap=true;
			}
		}
	}
}

void ucs2charstring::rightTrim(ucs2_t *str, ucs2_t character) {

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
		*str=(ucs2_t)'\0';
	}
}

void ucs2charstring::leftTrim(ucs2_t *str, ucs2_t character) {

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
		str[j]=(ucs2_t)'\0';
	}
}

bool ucs2charstring::strip(ucs2_t *str, ucs2_t character) {

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
	str[index-total]=(ucs2_t)'\0';
	return retval;
}

bool ucs2charstring::strip(ucs2_t *str1, const ucs2_t *str2) {

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
	str1[index-total]=(ucs2_t)'\0';
	return retval;
}

bool ucs2charstring::stripSet(ucs2_t *str, const ucs2_t *set) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (ucs2character::isInSet(str[index],set)) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]=(ucs2_t)'\0';
	return retval;
}

void ucs2charstring::replace(ucs2_t *str, ucs2_t oldchar, ucs2_t newchar) {
	if (str) {
		for (ucs2_t *ptr=str; *ptr; ptr++) {
			if (*ptr==oldchar) {
				*ptr=newchar;
			}
		}
	}
}

void ucs2charstring::replace(ucs2_t *str,
				const ucs2_t *oldchars, ucs2_t newchar) {
	if (str) {
		for (ucs2_t *ptr=str; *ptr; ptr++) {
			if (ucs2character::isInSet(*ptr,oldchars)) {
				*ptr=newchar;
			}
		}
	}
}

ucs2_t *ucs2charstring::replace(const ucs2_t *str,
					const ucs2_t *oldstr,
					const ucs2_t *newstr) {
	if (!str) {
		return NULL;
	}
	ucs2stringbuffer	newstring;
	size_t		oldstrlen=getLength(oldstr);
	const ucs2_t	*ptr=str;
	const ucs2_t	*start=ptr;
	while (*ptr) {
		if (!compare(ptr,oldstr,oldstrlen)) {
			newstring.appendUcs2(start,ptr-start);
			newstring.appendUcs2(newstr);
			ptr+=oldstrlen;
			start=ptr;
		} else {
			ptr++;
		}
	}
	newstring.appendUcs2(start,ptr-start);
	return newstring.detachString();
}

ucs2_t *ucs2charstring::replace(const ucs2_t *str,
					const ucs2_t * const *oldstrset,
					const ucs2_t * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// count members of oldstrset
	uint64_t	i=0;
	for (const ucs2_t * const *o=oldstrset; *o; o++) {
		i++;
	}

	// create oldstrlen
	size_t	*oldstrlen=new size_t[i];
	i=0;
	for (const ucs2_t * const *o=oldstrset; *o; o++) {
		oldstrlen[i]=getLength(*o);
		i++;
	}

	// replace
	ucs2_t	*result=replace(str,oldstrset,oldstrlen,newstrset);

	// clean up
	delete[] oldstrlen;

	return result;
}

ucs2_t *ucs2charstring::replace(const ucs2_t *str,
					const ucs2_t * const *oldstrset,
					size_t *oldstrlen,
					const ucs2_t * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// search and replace
	ucs2stringbuffer	newstring;
	const ucs2_t	*ptr=str;
	const ucs2_t	*start=ptr;
	while (*ptr) {
		bool	found=false;
		uint64_t i=0;
		for (const ucs2_t * const *oldptr=oldstrset;
							*oldptr; oldptr++) {
			if (!compare(ptr,oldstrset[i],oldstrlen[i])) {
				newstring.appendUcs2(start,ptr-start);
				newstring.appendUcs2(newstrset[i]);
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
	newstring.appendUcs2(start,ptr-start);

	return newstring.detachString();
}

bool ucs2charstring::isInteger(const ucs2_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const ucs2_t	*ptr=str;
	for (; *ptr; ptr++) {
		if (((*ptr>(ucs2_t)'9' || *ptr<(ucs2_t)'0') &&
			*ptr!=(ucs2_t)'-') || 
			(ptr>str && *ptr==(ucs2_t)'-')) {
			return false;
		}
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!=(ucs2_t)'-');
}

bool ucs2charstring::isInteger(const ucs2_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const ucs2_t	*ptr=str;
	for (int32_t index=0; index<len; index++) {
		if (((*ptr>(ucs2_t)'9' || *ptr<(ucs2_t)'0') &&
			*ptr!=(ucs2_t)'-') || 
			(ptr>str && *ptr==(ucs2_t)'-')) {
			return false;
		}
		ptr++;
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!=(ucs2_t)'-');
}

bool ucs2charstring::isNumber(const ucs2_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const ucs2_t	*ptr=str;
	int32_t		decimal=0;
	for (; *ptr; ptr++) {
		if (((*ptr>(ucs2_t)'9' || *ptr<(ucs2_t)'0') &&
			*ptr!=(ucs2_t)'-' && *ptr!=(ucs2_t)'.') || 
			(ptr>str && *ptr==(ucs2_t)'-') ||
			(decimal && *ptr==(ucs2_t)'.')) {
			return false;
		}
		if (*ptr==(ucs2_t)'.') {
			decimal=1;
		}
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!=(ucs2_t)'-' && *(ptr-1)!=(ucs2_t)'.');
}

bool ucs2charstring::isNumber(const ucs2_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const ucs2_t	*ptr=str;
	int32_t		decimal=0;
	for (int32_t index=0; index<len; index++) {
		if (((*ptr>(ucs2_t)'9' || *ptr<(ucs2_t)'0') &&
			*ptr!=(ucs2_t)'-' && *ptr!=(ucs2_t)'.') || 
			(ptr>str && *ptr==(ucs2_t)'-') ||
			(decimal && *ptr==(ucs2_t)'.')) {
			return false;
		}
		if (*ptr==(ucs2_t)'.') {
			decimal=1;
		}
		ptr++;
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!=(ucs2_t)'-' && *(ptr-1)!=(ucs2_t)'.');
}

int64_t ucs2charstring::convertAmount(const ucs2_t *amount) {
	if (!amount) {
		return 0;
	}
	const ucs2_t	*dollarsstr=findFirst(amount,(ucs2_t)'$');
	dollarsstr=(dollarsstr)?dollarsstr+1:amount;
	uint64_t	dollars=toUnsignedInteger(dollarsstr);
	const ucs2_t	*centsstr=findFirst(amount,(ucs2_t)'.');
	uint64_t	cents=(centsstr)?toUnsignedInteger(centsstr+1):0;
	return (dollars*100+cents);
}

ucs2_t *ucs2charstring::convertAmount(int64_t amount) {
	uint16_t	len=integerLength(amount)+4;
	if (len<6) {
		len=6;
	}
	ucs2_t	negative[2];
	if (amount<0) {
		negative[0]=(ucs2_t)'-';
	} else {
		negative[0]=(ucs2_t)'\0';
	}
	negative[1]=(ucs2_t)'\0';
	ucs2_t	*amountstr=new ucs2_t[len];
	int64_t		amt;
	#ifdef RUDIMENTS_HAVE_LLABS
		amt=llabs((long long)amount);
	#else
		amt=abs((long)amount);
	#endif
	ucs2_t	*format=duplicate("$%s%lld.%02lld");
	printf(amountstr,len,
			format,negative,
			amt/100,amt-(amt/100*100));
	delete[] format;
	return amountstr;
}

ucs2_t *ucs2charstring::convertAmount(int64_t amount, uint16_t spaces) {
	ucs2_t	*amt=convertAmount(amount);
	ssize_t	amtlen=getLength(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	ucs2_t	*buffer=new ucs2_t[realspaces+1];
	buffer[realspaces]=(ucs2_t)'\0';
	bytestring::set(buffer,(ucs2_t)' ',realspaces);
	bytestring::copy(buffer+realspaces-amtlen,amt+1,amtlen);
	if (buffer[0]==(ucs2_t)' ') {
		buffer[0]=(ucs2_t)'$';
	}
	delete[] amt;
	return buffer;
}

ucs2_t *ucs2charstring::escape(const ucs2_t *input,
					const ucs2_t *characters) {
	ucs2_t		*output;
	uint64_t	outputlength;
	escape(input,getLength(input),&output,&outputlength,characters);
	return output;
}

void ucs2charstring::escape(const ucs2_t *input, uint64_t inputlength,
				ucs2_t **output, uint64_t *outputlength,
						const ucs2_t *characters) {

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
					input[inputindex]==(ucs2_t)'\\') {
				if (pass==0) {
					(*outputlength)++;
				} else {
					(*output)[outputindex]=(ucs2_t)'\\';
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
			(*output)=new ucs2_t[(*outputlength)+1];
			(*output)[(*outputlength)]=(ucs2_t)'\0';
		}
	}
}

ucs2_t *ucs2charstring::unescape(const ucs2_t *input) {
	ucs2_t		*output;
	uint64_t	outputsize;
	unescape(input,getLength(input),&output,&outputsize);
	return output;
}

void ucs2charstring::unescape(const ucs2_t *input, uint64_t inputlength,
				ucs2_t **output, uint64_t *outputlength) {

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
			if (!escaped && input[inputindex]==(ucs2_t)'\\') {
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
			(*output)=new ucs2_t[(*outputlength)+1];
			(*output)[(*outputlength)]=(ucs2_t)'\0';
		}
	}
}

void ucs2charstring::leftJustify(ucs2_t *str, int32_t len) {

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
		str[index]=(ucs2_t)' ';
	}
}

void ucs2charstring::rightJustify(ucs2_t *str, int32_t len) {

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
		str[index]=(ucs2_t)' ';
	}
}

void ucs2charstring::center(ucs2_t *str, int32_t len) {

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
			str[index]=(ucs2_t)' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int32_t	difference=leadingspaces-leftpad;
		int32_t	index;
		for (index=0; index<len-difference; index++) {
			str[index]=str[index+difference];
		}
		for (; index<len; index++) {
			str[index]=(ucs2_t)' ';
		}
	}
}

int32_t ucs2charstring::countLeadingSpaces(const ucs2_t *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	leadingspaces=0;
	for (int32_t index=0;
			str[index]==(ucs2_t)' ' && index<len;
			index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int32_t ucs2charstring::countTrailingSpaces(const ucs2_t *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	trailingspaces=0;
	for (int32_t index=len-1;
			str[index]==(ucs2_t)' ' && index>-1;
			index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

ucs2_t *ucs2charstring::parseNumber(int16_t number) {
	return parseNumber((int64_t)number,1);
}

ucs2_t *ucs2charstring::parseNumber(int16_t number,
				uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

ucs2_t *ucs2charstring::parseNumber(uint16_t number) {
	return parseNumber((uint64_t)number,1);
}

ucs2_t *ucs2charstring::parseNumber(uint16_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

ucs2_t *ucs2charstring::parseNumber(int32_t number) {
	return parseNumber((int64_t)number,1);
}

ucs2_t *ucs2charstring::parseNumber(int32_t number, uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

ucs2_t *ucs2charstring::parseNumber(uint32_t number) {
	return parseNumber((uint64_t)number,1);
}

ucs2_t *ucs2charstring::parseNumber(uint32_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

ucs2_t *ucs2charstring::parseNumber(int64_t number) {
	return parseNumber(number,1);
}

ucs2_t *ucs2charstring::parseNumber(int64_t number, uint16_t zeropadding) {
	if (number>=0) {
		return parseNumber((uint64_t)number,zeropadding);
	}
	uint16_t	len=integerLength(number);
	uint16_t	strlen=((zeropadding>len)?zeropadding:len);
	ucs2_t	*ptr=new ucs2_t[strlen+1];
	*ptr=(ucs2_t)'-';
	ptr+=strlen;
	*ptr=(ucs2_t)'\0';
	number*=-1;
	strlen--;
	while (strlen) {
		ptr--;
		*ptr=(ucs2_t)('0'+number%10);
		number/=10;
		strlen--;
	}
	ptr--;
	return ptr;
}

ucs2_t *ucs2charstring::parseNumber(uint64_t number) {
	return parseNumber(number,1);
}

ucs2_t *ucs2charstring::parseNumber(uint64_t number, uint16_t zeropadding) {
	uint16_t	len=integerLength(number);
	uint16_t	strlen=((zeropadding>len)?zeropadding:len);
	ucs2_t	*ptr=new ucs2_t[strlen+1];
	ptr+=strlen;
	*ptr=(ucs2_t)'\0';
	while (strlen) {
		ptr--;
		*ptr=(ucs2_t)('0'+number%10);
		number/=10;
		strlen--;
	}
	return ptr;
}

ucs2_t *ucs2charstring::parseNumber(float number) {
	return parseNumber((long double)number);
}

ucs2_t *ucs2charstring::parseNumber(float number, uint16_t scale) {
	return parseNumber((long double)number,scale);
}

ucs2_t *ucs2charstring::parseNumber(float number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

ucs2_t *ucs2charstring::parseNumber(double number) {
	return parseNumber((long double)number);
}

ucs2_t *ucs2charstring::parseNumber(double number,
				uint16_t scale) {
	return parseNumber((long double)number,scale);
}

ucs2_t *ucs2charstring::parseNumber(double number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

ucs2_t *ucs2charstring::parseNumber(long double number) {
	char		*str=new char[22];
	charstring::printf(str,22,"%Lf",number);
	ucs2_t	*retval=duplicate(str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::parseNumber(long double number, uint16_t scale) {
	char		*str=new char[22];
	charstring::printf(str,22,"%.*Lf",scale,number);
	ucs2_t	*retval=duplicate(str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::parseNumber(long double number,
				uint16_t precision, uint16_t scale) {
	size_t		strlen=precision+3;
	char		*str=new char[strlen];
	charstring::printf(str,strlen,"%*.*Lf",precision,scale,number);
	ucs2_t	*retval=duplicate(str);
	delete[] str;
	return retval;
}

void ucs2charstring::bothTrim(ucs2_t *string, ucs2_t character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

uint16_t ucs2charstring::getIntegerLength(int16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int16_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::getIntegerLength(int32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int32_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::getIntegerLength(int64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int64_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::getIntegerLength(uint16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint16_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::getIntegerLength(uint32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint32_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::getIntegerLength(uint64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint64_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

size_t ucs2charstring::getLength(const ucs2_t *string) {
	size_t	len=0;
	if (string) {
		for (const ucs2_t *s=string; *s; s++) {
			len++;
		}
	}
	return len;
}

size_t ucs2charstring::getSize(const ucs2_t *string) {
	return (string)?(getLength(string)+1)*sizeof(ucs2_t):0;
}

bool ucs2charstring::isNullOrEmpty(const ucs2_t *string) {
	return !(string && string[0]);
}

bool ucs2charstring::isYes(const ucs2_t *string) {
	ucs2_t	yes[]={
		(ucs2_t)'y',
		(ucs2_t)'e',
		(ucs2_t)'s',
		(ucs2_t)'\0'
	};
	ucs2_t	truestr[]={
		(ucs2_t)'t',
		(ucs2_t)'r',
		(ucs2_t)'u',
		(ucs2_t)'e',
		(ucs2_t)'\0'
	};
	ucs2_t	on[]={
		(ucs2_t)'o',
		(ucs2_t)'n',
		(ucs2_t)'\0'
	};
	ucs2_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,yes,3)) {
		next=string[3];
	} else if (!compareIgnoringCase(string,truestr,4)) {
		next=string[4];
	} else if (!compareIgnoringCase(string,on,2)) {
		next=string[2];
	} else if (string[0]==(ucs2_t)'y' || string[0]==(ucs2_t)'Y' ||
			string[0]==(ucs2_t)'t' || string[0]==(ucs2_t)'T' ||
			string[0]==(ucs2_t)'1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || !ucs2character::isAlphanumeric(next));
}

bool ucs2charstring::isNo(const ucs2_t *string) {
	ucs2_t	no[]={
		(ucs2_t)'n',
		(ucs2_t)'o',
		(ucs2_t)'\0'
	};
	ucs2_t	falsestr[]={
		(ucs2_t)'f',
		(ucs2_t)'a',
		(ucs2_t)'l',
		(ucs2_t)'s',
		(ucs2_t)'e',
		(ucs2_t)'\0'
	};
	ucs2_t	off[]={
		(ucs2_t)'o',
		(ucs2_t)'f',
		(ucs2_t)'f',
		(ucs2_t)'\0'
	};
	ucs2_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,no,2)) {
		next=string[2];
	} else if (!compareIgnoringCase(string,falsestr,5)) {
		next=string[5];
	} else if (!compareIgnoringCase(string,off,3)) {
		next=string[3];
	} else if (string[0]==(ucs2_t)'n' || string[0]==(ucs2_t)'N' ||
			string[0]==(ucs2_t)'f' || string[0]==(ucs2_t)'F' ||
			string[0]==(ucs2_t)'0') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || !ucs2character::isAlphanumeric(next));
}

void ucs2charstring::zero(ucs2_t *str, size_t len) {
	bytestring::set(str,0,len*sizeof(ucs2_t));
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, const ucs2_t *source) {
	return append(dest,source,getLength(source)+1);
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, const ucs2_t *source, size_t len) {
	return copy(dest+getLength(dest),source,len);
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, int64_t number) {
	ucs2_t	*str=parseNumber(number);
	ucs2_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, uint64_t number) {
	ucs2_t	*str=parseNumber(number);
	ucs2_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, double number) {
	ucs2_t	*str=parseNumber(number);
	ucs2_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, double number,
							uint16_t scale) {
	ucs2_t	*str=parseNumber(number,scale);
	ucs2_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, double number,
							uint16_t precision,
							uint16_t scale) {
	ucs2_t	*str=parseNumber(number,precision,scale);
	ucs2_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

ucs2_t *ucs2charstring::copy(ucs2_t *dest, const ucs2_t *source) {
	return copy(dest,source,getLength(source)+1);
}

ucs2_t *ucs2charstring::copy(ucs2_t *dest, const ucs2_t *source,
								size_t len) {
	if (!dest || !source) {
		return dest;
	}
	while (len && *source) {
		*dest=*source;
		source++;
		dest++;
		len--;
	}
	if (len) {
		*dest=(ucs2_t)'\0';
	}
	return dest;
}

ucs2_t *ucs2charstring::copy(ucs2_t *dest, size_t location,
						const ucs2_t *source) {
	return copy(dest+location,source);
}

ucs2_t *ucs2charstring::copy(ucs2_t *dest, size_t location,
					const ucs2_t *source, size_t len) {
	return copy(dest+location,source,len);
}

ucs2_t *ucs2charstring::safeCopy(ucs2_t *dest, size_t destlen,
						const ucs2_t *source) {
	return safeCopy(dest,destlen,source,getLength(source)+1);
}

ucs2_t *ucs2charstring::safeCopy(ucs2_t *dest, size_t destlen,
				const ucs2_t *source, size_t sourcelen) {
	return copy(dest,source,(sourcelen>destlen)?destlen:sourcelen);
}

int32_t ucs2charstring::compare(const ucs2_t *str1, const ucs2_t *str2) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2) {
		diff=(*str1-*str2);
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
	}
	return *str1-*str2;
}

int32_t ucs2charstring::compare(const ucs2_t *str1, const ucs2_t *str2,
								size_t len) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2 && len) {
		diff=(*str1-*str2);
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
		len--;
	}
	return (len)?*str1-*str2:(ucs2_t)'\0';
}

int32_t ucs2charstring::compareIgnoringCase(const ucs2_t *str1,
							const ucs2_t *str2) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2) {
		diff=(ucs2character::upper(*str1)-
			ucs2character::upper(*str2));
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
	}
	return ucs2character::upper(*str1)-
			ucs2character::upper(*str2);
}

int32_t ucs2charstring::compareIgnoringCase(const ucs2_t *str1,
						const ucs2_t *str2,
						size_t len) {
	if (!str1 && !str2) {
		return 0;
	}
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2 && len) {
		diff=(ucs2character::upper(*str1)-
			ucs2character::upper(*str2));
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
		len--;
	}
	return (len)?ucs2character::upper(*str1)-
			ucs2character::upper(*str2):0;
}

int32_t ucs2charstring::compareNatural(const ucs2_t *str1,
					const ucs2_t *str2) {
	ucs2_t	dot[]={
		(ucs2_t)'.',
		(ucs2_t)'\0'
	};
	return compareNatural(str1,str2,dot);
}

int32_t ucs2charstring::compareNatural(const ucs2_t *str1,
					const ucs2_t *str2,
					const ucs2_t *delimiters) {
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
	const ucs2_t	*start1=NULL;
	const ucs2_t	*start2=NULL;
	ucs2_t		*num1=NULL;
	ucs2_t		*num2=NULL;
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
		bool	isdigit1=ucs2character::isDigit(*str1);
		bool	isdigit2=ucs2character::isDigit(*str2);
		difference+=isdigit2-isdigit1;

		// if the difference is non-zero then return it
		if (difference) {
			return difference;
		}

		if (isdigit1 && isdigit2) {

			// move to after the number in both strings
			start1=str1;
			while (*str1 && (ucs2character::isDigit(*str1) ||
				ucs2character::isInSet(*str1,delimiters))) {
				str1++;
			}
			start2=str2;
			while (*str2 && (ucs2character::isDigit(*str2) ||
				ucs2character::isInSet(*str2,delimiters))) {
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
			difference+=(int64_t)((*str1)-(*str2));

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

int32_t ucs2charstring::compareVersions(const ucs2_t *str1,
					const ucs2_t *str2) {
	ucs2_t	dot[]={
		(ucs2_t)'.',
		(ucs2_t)'\0'
	};
	return compareVersions(str1,str2,dot);
}

int32_t ucs2charstring::compareVersions(const ucs2_t *str1,
					const ucs2_t *str2,
					const ucs2_t *delimiters) {
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
		difference+=(toInteger(str1)-toInteger(str2));

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

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					size_t stringlength,
					const ucs2_t *pattern,
					size_t patternlength,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {

	// handle degenerate case
	if (!string && !pattern) {
		return true;
	}

	const ucs2_t	*stringend=string+stringlength;
	const ucs2_t	*patternend=pattern+patternlength;

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

		// if we encountered a multi-ucs2_t wildcard...
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

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					const ucs2_t *pattern,
					size_t patternlength,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {
	return compareWithWildcards(string,getLength(string),
					pattern,patternlength,
					singlewildcard,multiwildcard);
}

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					size_t stringlength,
					const ucs2_t *pattern,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {
	return compareWithWildcards(string,stringlength,
					pattern,getLength(pattern),
					singlewildcard,multiwildcard);
}

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					const ucs2_t *pattern,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {
	return compareWithWildcards(string,getLength(string),
					pattern,getLength(pattern),
					singlewildcard,multiwildcard);
}

bool ucs2charstring::isInSet(const ucs2_t *str, const ucs2_t * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const ucs2_t * const *s=set; *s; s++) {
		if (!compare(str,*s)) {
			return true;
		}
	}
	return false;
}

bool ucs2charstring::isInSetIgnoringCase(const ucs2_t *str,
					const ucs2_t * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const ucs2_t * const *s=set; *s; s++) {
		if (!compareIgnoringCase(str,*s)) {
			return true;
		}
	}
	return false;
}

bool ucs2charstring::contains(const ucs2_t *haystack,
					const ucs2_t *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool ucs2charstring::containsIgnoringCase(const ucs2_t *haystack,
						const ucs2_t *needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool ucs2charstring::contains(const ucs2_t *haystack, ucs2_t needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool ucs2charstring::containsIgnoringCase(const ucs2_t *haystack,
						ucs2_t needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool ucs2charstring::startsWith(const ucs2_t *haystack,
					const ucs2_t *needle) {
	return !compare(haystack,needle,getLength(needle));
}

bool ucs2charstring::endsWith(const ucs2_t *haystack,
					const ucs2_t *needle) {
	size_t	needlelen=getLength(needle);
	size_t	haystacklen=getLength(haystack);
	return (haystacklen>=needlelen &&
		!compare(haystack+haystacklen-needlelen,needle));
}

const ucs2_t *ucs2charstring::findFirst(const ucs2_t *haystack,
						const ucs2_t *needle) {
	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	for (const ucs2_t *ptr=haystack;
			ptr<=haystack+haystacklen-needlelen;
			ptr++) {
		if (!compare(ptr,needle,needlelen)) {
			return ptr;
		}
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findFirstIgnoringCase(const ucs2_t *haystack,
							const ucs2_t *needle) {
	size_t	haystacklen=getLength(haystack);
	size_t	needlelen=getLength(needle);
	for (const ucs2_t *ptr=haystack;
			ptr<=haystack+haystacklen-needlelen;
			ptr++) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findFirst(const ucs2_t *haystack,
							ucs2_t needle) {
	size_t	haystacklen=getLength(haystack);
	for (const ucs2_t *ptr=haystack;
			ptr<haystack+haystacklen;
			ptr++) {
		if (*ptr==needle) {
			return ptr;
		}
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findFirstIgnoringCase(const ucs2_t *haystack,
							ucs2_t needle) {
	size_t	haystacklen=getLength(haystack);
	needle=(ucs2_t)ucs2character::lower(needle);
	for (const ucs2_t *ptr=haystack;
			ptr<haystack+haystacklen;
			ptr++) {
		if ((ucs2_t)ucs2character::lower(*ptr)==needle) {
			return ptr;
		}
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findFirstOrEnd(const ucs2_t *haystack,
							ucs2_t needle) {
	if (!haystack) {
		return NULL;
	}
	const ucs2_t	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

const ucs2_t *ucs2charstring::findFirstOrEndIgnoringCase(
						const ucs2_t *haystack,
						ucs2_t needle) {
	if (!haystack) {
		return NULL;
	}
	const ucs2_t	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

const ucs2_t *ucs2charstring::findFirstOrEnd(const ucs2_t *haystack,
						const ucs2_t *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const ucs2_t	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

const ucs2_t *ucs2charstring::findFirstOrEndIgnoringCase(
						const ucs2_t *haystack,
						const ucs2_t *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const ucs2_t	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+getLength(haystack);
	}
	return retval;
}

ucs2_t *ucs2charstring::findFirstOrEnd(ucs2_t *haystack,
						const ucs2_t *needle) {
	return (ucs2_t *)(findFirstOrEnd(
				(const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findFirstOrEndIgnoringCase(ucs2_t *haystack,
						const ucs2_t *needle) {
	return (ucs2_t *)(findFirstOrEndIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

const ucs2_t *ucs2charstring::findLast(const ucs2_t *haystack,
							ucs2_t needle) {
	if (!haystack) {
		return NULL;
	}
	const ucs2_t	*c=haystack;
	while (*c) {
		c++;
	}
	while (c>haystack) {
		if (*c==needle) {
			return c;
		}
		c--;
	}
	if (*c==needle) {
		return c;
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findLastIgnoringCase(const ucs2_t *haystack,
							ucs2_t needle) {
	return (ucs2_t *)(findFirstOrEndIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findFirst(ucs2_t *haystack,
					const ucs2_t *needle) {
	return (ucs2_t *)(findFirst((const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findFirstIgnoringCase(ucs2_t *haystack,
						const ucs2_t *needle) {
	return (ucs2_t *)(findFirstIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findFirst(ucs2_t *haystack, ucs2_t needle) {
	return (ucs2_t *)(findFirst((const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findFirstIgnoringCase(ucs2_t *haystack,
							ucs2_t needle) {
	return (ucs2_t *)(findFirstIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findLast(ucs2_t *haystack, const ucs2_t *needle) {
	return (ucs2_t *)(findLast((const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findLastIgnoringCase(ucs2_t *haystack,
						const ucs2_t *needle) {
	return (ucs2_t *)(findLastIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findLast(ucs2_t *haystack, ucs2_t needle) {
	return (ucs2_t *)(findLast((const ucs2_t *)(haystack),needle));
}

ucs2_t *ucs2charstring::findLastIgnoringCase(ucs2_t *haystack,
							ucs2_t needle) {
	return (ucs2_t *)(findLastIgnoringCase(
					(const ucs2_t *)(haystack),needle));
}

const ucs2_t *ucs2charstring::findFirstOfSet(const ucs2_t *haystack,
							const ucs2_t *set) {
	return (ucs2_t *)(findFirstOfSet((ucs2_t *)(haystack),set));
}

ucs2_t *ucs2charstring::findFirstOfSet(ucs2_t *haystack,
							const ucs2_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	ucs2_t	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return NULL;
}

const ucs2_t *ucs2charstring::findFirstOfSetOrEnd(const ucs2_t *haystack,
							const ucs2_t *set) {
	return (ucs2_t *)(findFirstOfSetOrEnd((ucs2_t *)(haystack),set));
}

ucs2_t *ucs2charstring::findFirstOfSetOrEnd(ucs2_t *haystack,
							const ucs2_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	ucs2_t	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return retval;
}

const ucs2_t *ucs2charstring::findLastOfSet(const ucs2_t *haystack,
							const ucs2_t *set) {
	return (ucs2_t *)(findLastOfSet((ucs2_t *)(haystack),set));
}

ucs2_t *ucs2charstring::findLastOfSet(ucs2_t *haystack,
						const ucs2_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	ucs2_t	*retval=haystack+ucs2charstring::getLength(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return NULL;
}

const ucs2_t *ucs2charstring::findLastOfSetOrEnd(const ucs2_t *haystack,
							const ucs2_t *set) {
	return (ucs2_t *)(findLastOfSetOrEnd((ucs2_t *)(haystack),set));
}

ucs2_t *ucs2charstring::findLastOfSetOrEnd(ucs2_t *haystack,
						const ucs2_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	ucs2_t	*retval=haystack+getLength(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+getLength(haystack);
}

size_t ucs2charstring::getLengthContainingSet(const ucs2_t *haystack,
							const ucs2_t *set) {
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (contains(set,haystack[index])) {
		index++;
	}
	return index;
}

size_t ucs2charstring::getLengthNotContainingSet(const ucs2_t *haystack,
							const ucs2_t *set) {
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (!contains(set,haystack[index])) {
		index++;
	}
	return index;
}

ucs2_t *ucs2charstring::isBefore(const ucs2_t *str,
					const ucs2_t *delimiter) {
	return isBetween(str,NULL,delimiter);
}

ucs2_t *ucs2charstring::isBetween(const ucs2_t *str,
					const ucs2_t *start,
					const ucs2_t *end) {

	// find the start (or use beginning of "str" if "start" is empty/NULL)
	const ucs2_t	*s=NULL;
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
	const ucs2_t	*e=NULL;
	if (isNullOrEmpty(end)) {
		for (e=s; *e; e++) {}
	} else {
		e=findFirst(s,end);
	}
	if (!e) {
		return NULL;
	}

	// copy out the string between s and e
	return ucs2charstring::duplicate(s,e-s);
}

ucs2_t *ucs2charstring::isAfter(const ucs2_t *str,
					const ucs2_t *delimiter) {
	return isBetween(str,delimiter,NULL);
}

ucs2_t *ucs2charstring::duplicate(const char *str) {
	return duplicate(str,charstring::getLength(str),sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const char *str, size_t len) {
	return duplicate(str,len,sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const char *str, bool bigendian) {
	return duplicate(str,charstring::getLength(str),bigendian);
}

ucs2_t *ucs2charstring::duplicate(const char *str, size_t len, bool bigendian) {
	// FIXME: use iconvert directly
	if (!str) {
		return NULL;
	}
	ucs2_t	*buffer=new ucs2_t[len+1];
	for (size_t i=0; i<len; i++) {
		buffer[i]=ucs2character::duplicate(str[i],bigendian);
	}
	buffer[len]=(ucs2_t)'\0';
	return buffer;
}

ucs2_t *ucs2charstring::duplicate(const ucs2_t *str) {
	return duplicate(str,getLength(str));
}

ucs2_t *ucs2charstring::duplicate(const ucs2_t *str, size_t len) {
	if (!str) {
		return NULL;
	}
	ucs2_t	*buffer=new ucs2_t[len+1];
	copy(buffer,str,len);
	buffer[len]=(ucs2_t)'\0';
	return buffer;
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string) {
	return duplicate(string,wcharstring::getLength(string),
				(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, size_t len) {
	return duplicate(string,len,(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, bool bigendian) {
	return duplicate(string,wcharstring::getLength(string),
				(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, size_t len,
							bool bigendian) {
	return duplicate(string,len,(ucs2_t)'?',bigendian);
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string,
					ucs2_t replacement) {
	return duplicate(string,wcharstring::getLength(string),
				replacement,sys::getIsBigEndian());
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, size_t len,
							ucs2_t replacement,
							bool bigendian) {
	// FIXME: use iconvert directly
	if (!string) {
		return NULL;
	}
	ucs2_t	*retval=new ucs2_t[len+1];
	for (size_t i=0; i<len; i++) {
		retval[i]=ucs2character::duplicate(string[i],replacement,
								bigendian);
	}
	retval[len]=(ucs2_t)'\0';
	return retval;
}

void ucs2charstring::rightTrim(ucs2_t *str) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (ucs2character::isWhitespace(*str)) { 
			str--;
		}
		str++;

		// terminate the string there
		*str=(ucs2_t)'\0';
	}
}

void ucs2charstring::leftTrim(ucs2_t *str) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
		while (ucs2character::isWhitespace(str[i])) {
			i++;
		}
	
		// swap chars to front of string
		while (str[i]) {
			str[j]=str[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		str[j]=(ucs2_t)'\0';
	}
}

void ucs2charstring::bothTrim(ucs2_t *string) {
	leftTrim(string);
	rightTrim(string);
}

int64_t ucs2charstring::convertToInteger(const ucs2_t *string) {
	return toInteger(string,NULL,10);
}

int64_t ucs2charstring::convertToInteger(const ucs2_t *string,
					const ucs2_t **endptr) {
	return toInteger(string,endptr,10);
}

int64_t ucs2charstring::convertToInteger(const ucs2_t *string, int32_t base) {
	return toInteger(string,NULL,base);
}

int64_t ucs2charstring::convertToInteger(const ucs2_t *string,
				const ucs2_t **endptr, int32_t base) {
	char	*str=charstring::duplicateUcs2(string);
	int64_t	retval=charstring::convertToInteger(str,NULL,base);
	delete[] str;
	if (endptr) {
		while (ucs2character::isDigit(*string)) {
			string++;
		}
		*endptr=string;
	}
	return retval;
}

uint64_t ucs2charstring::convertToUnsignedInteger(const ucs2_t *string) {
	return toUnsignedInteger(string,NULL,10);
}

uint64_t ucs2charstring::convertToUnsignedInteger(const ucs2_t *string,
					const ucs2_t **endptr) {
	return toUnsignedInteger(string,endptr,10);
}

uint64_t ucs2charstring::convertToUnsignedInteger(const ucs2_t *string,
							int32_t base) {
	return toUnsignedInteger(string,NULL,base);
}

uint64_t ucs2charstring::convertToUnsignedInteger(const ucs2_t *string,
						const ucs2_t **endptr,
						int32_t base) {
	char	*str=charstring::duplicateUcs2(string);
	int64_t	retval=charstring::convertToUnsignedInteger(str,NULL,base);
	delete[] str;
	if (endptr) {
		while (ucs2character::isDigit(*string)) {
			string++;
		}
		*endptr=string;
	}
	return retval;
}

long double ucs2charstring::convertToFloat(const ucs2_t *string) {
	return toFloat(string,NULL);
}

long double ucs2charstring::convertToFloatC(const ucs2_t *string) {

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
	ucs2_t	stringinlocale[256];
	const ucs2_t	*decimalpointlocation;
	struct	lconv	*currentlconv=localeconv();
	if ((currentlconv!=NULL) &&
		(currentlconv->decimal_point!=NULL) &&
		(currentlconv->decimal_point[0]!=0) &&
		(currentlconv->decimal_point[0]!=(ucs2_t)'.') &&
		(currentlconv->decimal_point[1]==0) &&
		((decimalpointlocation=findFirst(string,(ucs2_t)'.'))!=NULL) &&
		(len<sizeof(stringinlocale))) {

		bytestring::copy(stringinlocale,string,len+1);

		stringinlocale[decimalpointlocation-string]=
					(ucs2_t)currentlconv->decimal_point[0];

		return toFloat(stringinlocale,NULL);
	}
#endif
	return toFloat(string,NULL);
}

long double ucs2charstring::convertToFloat(const ucs2_t *string,
					const ucs2_t **endptr) {
	char		*str=charstring::duplicateUcs2(string);
	long double	retval=charstring::convertToFloat(str,NULL);
	delete[] str;
	if (endptr) {
		bool	found=false;
		for (;;) {
			if (ucs2character::isDigit(*string)) {
				string++;
			// FIXME: this test should really use the
			// delimiter from the locale instead of just '.'
			} else if (!found && *string==(ucs2_t)'.') {
				string++;
				found=true;
			} else {
				break;
			}
		}
		*endptr=string;
	}
	return retval;
}

void ucs2charstring::split(const ucs2_t *string,
				const ucs2_t *delimiter,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {
	split(string,getLength(string),
			delimiter,getLength(delimiter),
			collapse,list,listlength);
}

void ucs2charstring::split(const ucs2_t *string,
				size_t stringlength,
				const ucs2_t *delimiter,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {
	split(string,stringlength,
			delimiter,getLength(delimiter),
			collapse,list,listlength);
}

void ucs2charstring::split(const ucs2_t *string, 
				const ucs2_t *delimiter,
				size_t delimiterlength,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {
	split(string,getLength(string),
			delimiter,delimiterlength,
			collapse,list,listlength);
}

void ucs2charstring::split(const ucs2_t *string,
				size_t stringlength,
				const ucs2_t *delimiter,
				size_t delimiterlength,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {

	// handle degenerate cases
	if (!list && !listlength) {
		return;
	}
	if (isNullOrEmpty(string) || !stringlength ||
		isNullOrEmpty(delimiter) || !delimiterlength) {
		if (list) {
			(*list)=NULL;
		}
		if (listlength) {
			(*listlength)=0;
		}
		return;
	}

	// declare local list length
	uint64_t	ll=0;

	// 2 passes,
	// 1 to count the number of chunks to split the string into,
	// 1 to actually split the string
	for (int32_t pass=0; pass<2; pass++) {

		// set pointers to the beginning and end of the string
		const ucs2_t	*start=string;
		const ucs2_t	*end=string+stringlength;

		// initialize the list length
		ll=0;

		// loop through the string...
		const ucs2_t	*current=start;
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
						(*list)[ll]=
							duplicate(start,
								current-start);
					}

					// increment the counter
					ll++;
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
			(*list)=new ucs2_t *[ll];
		}
	}

	// copy out the listlength
	if (listlength) {
		(*listlength)=ll;
	}
}

ucs2_t *ucs2charstring::getSubString(const ucs2_t *str,
					size_t start,
					size_t end) {

	// handle end<start
	if (end<start) {
		size_t	temp=end;
		end=start;
		start=temp;
	}
	return duplicate(str+start,end-start+1);
}

ucs2_t *ucs2charstring::getSubString(const ucs2_t *str, size_t start) {
	return subString(str,start,getLength(str)-1);
}

ucs2_t *ucs2charstring::insertString(const ucs2_t *dest,
					const ucs2_t *src,
					uint64_t index) {

	uint64_t	srcsize=getLength(src);
	uint64_t	size=getLength(dest)+srcsize+1;
	ucs2_t		*retval=new ucs2_t[size];
	for (uint64_t i=0,j=0; i<size;) {
		if (i==index) {
			for (uint64_t k=0; k<srcsize; k++) {
				retval[i++]=src[k];
			}
		} else {
			retval[i++]=dest[j++];
		}
	}
	retval[size-1]=(ucs2_t)'\0';
	return retval;
}

ucs2_t *ucs2charstring::pad(const ucs2_t *str, ucs2_t padchar,
			int16_t direction, uint64_t totallength) {

	if (totallength==0) {
		return NULL;
	}

	uint64_t	strlen=((str==NULL)?0:getLength(str));
	ucs2_t	*newstring=NULL;

	newstring=new ucs2_t[totallength+1];
	if (strlen>=totallength) {
		copy(newstring,str,totallength);
		newstring[totallength]=(ucs2_t)'\0';
		return newstring;
	}

	bytestring::set(newstring,padchar,totallength);
	newstring[totallength]=(ucs2_t)'\0';

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

ucs2_t *ucs2charstring::getHumanReadable(int64_t number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::getHumanReadable(int64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

ucs2_t *ucs2charstring::getHumanReadable(uint64_t number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::getHumanReadable(uint64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

ucs2_t *ucs2charstring::getHumanReadable(long double number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::getHumanReadable(long double number, bool onethousand) {

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
	charstring::printf(&buf,"%0.1Lf%c",number/size,suffixes[i]);
	char	*subbed=charstring::replace(buf,".0","");
	delete[] buf;
	ucs2_t	*converted=duplicate(subbed);
	delete[] subbed;
	return converted;
}

ssize_t ucs2charstring::printf(ucs2_t *buffer, size_t len,
					const ucs2_t *format, ...) {
	va_list	args;
	va_start(args,format);
	size_t	result=printf(buffer,len,format,&args);
	va_end(args);
	return result;
}

ssize_t ucs2charstring::printf(ucs2_t *buffer, size_t len,
					const ucs2_t *format, va_list *argp) {
	#ifdef _WIN32
		// on windows, wchar_t's are encoded as UCS-2,
		// so we can piggyback
		return wcharstring::printf((wchar_t *)buffer,len,
						(const wchar_t *)format,argp);
	#else
		// on non-windows, it's trickier...

		// FIXME: implement this...
		// make sure that %lc and %ls are interpreted as ucs2_t
		// characters/strings rather than wide characters/strings
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

ssize_t ucs2charstring::printf(ucs2_t **buffer, const ucs2_t *format, ...) {
	va_list	args;
	va_start(args,format);
	// the printf() below will call va_end(args)
	return printf(buffer,format,&args);
}

ssize_t ucs2charstring::printf(ucs2_t **buffer,
				const ucs2_t *format,
				va_list *argp) {

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
		*buffer=new ucs2_t[size+1];
		size=printf(*buffer,size+1,format,&argp1);
	}
	va_end(argp1);
	return size;
}

bool ucs2charstring::supportsPrintf() {
	// FIXME: set this true eventually
	return false;
}
