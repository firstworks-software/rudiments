// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/wcharacter.h>
#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
	#include <rudiments/process.h>
	#include <rudiments/file.h>
#endif
#include <rudiments/ucs2stringbuffer.h>
#include <rudiments/error.h>

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

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
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

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
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
			*ch=ucs2character::toUpperCase(*ch);
		}
	}
}

void ucs2charstring::lower(ucs2_t *str) {
	if (str) {
		for (ucs2_t *ch=str; *ch; ch++) {
			*ch=ucs2character::toLowerCase(*ch);
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
		0
	};
	if (str) {
		bool	cap=true;
		for (ucs2_t *ch=str; *ch; ch++) {
			if (cap) {
				*ch=ucs2character::toUpperCase(*ch);
				cap=false;
			} else {
				*ch=ucs2character::toLowerCase(*ch);
			}
			if (ucs2character::inSet(*ch,delims)) {
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
		*str='\0';
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
		str[j]='\0';
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
	str[index-total]='\0';
	return retval;
}

bool ucs2charstring::strip(ucs2_t *str1, const ucs2_t *str2) {

	if (!str1 || !str2) {
		return false;
	}

	int32_t	str2len=length(str2);
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

bool ucs2charstring::stripSet(ucs2_t *str, const ucs2_t *set) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (ucs2character::inSet(str[index],set)) {
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
			if (ucs2character::inSet(*ptr,oldchars)) {
				*ptr=newchar;
			}
		}
	}
}

ucs2_t *ucs2charstring::replace(const ucs2_t *str, const ucs2_t *oldstr,
						const ucs2_t *newstr) {
	if (!str) {
		return NULL;
	}
	ucs2stringbuffer	newstring;
	size_t		oldstrlen=length(oldstr);
	const ucs2_t	*ptr=str;
	const ucs2_t	*start=ptr;
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
		oldstrlen[i]=length(*o);
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

bool ucs2charstring::isInteger(const ucs2_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const ucs2_t	*ptr=str;
	for (; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!='-');
}

bool ucs2charstring::isInteger(const ucs2_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const ucs2_t	*ptr=str;
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

bool ucs2charstring::isNumber(const ucs2_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const ucs2_t	*ptr=str;
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

bool ucs2charstring::isNumber(const ucs2_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const ucs2_t	*ptr=str;
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

int64_t ucs2charstring::convertAmount(const ucs2_t *amount) {
	if (!amount) {
		return 0;
	}
	const ucs2_t	*dollarsstr=findFirst(amount,'$');
	dollarsstr=(dollarsstr)?dollarsstr+1:amount;
	uint64_t	dollars=toUnsignedInteger(dollarsstr);
	const ucs2_t	*centsstr=findFirst(amount,'.');
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
		negative[0]='-';
	} else {
		negative[0]='\0';
	}
	negative[1]='\0';
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
	ssize_t	amtlen=length(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	ucs2_t	*buffer=new ucs2_t[realspaces+1];
	buffer[realspaces]='\0';
	bytestring::set(buffer,' ',realspaces);
	bytestring::copy(buffer+realspaces-amtlen,amt+1,amtlen);
	if (buffer[0]==' ') {
		buffer[0]='$';
	}
	delete[] amt;
	return buffer;
}

ucs2_t *ucs2charstring::escape(const ucs2_t *input,
					const ucs2_t *characters) {
	ucs2_t		*output;
	uint64_t	outputlength;
	escape(input,length(input),&output,&outputlength,characters);
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
			(*output)=new ucs2_t[(*outputlength)+1];
			(*output)[(*outputlength)]='\0';
		}
	}
}

ucs2_t *ucs2charstring::unescape(const ucs2_t *input) {
	ucs2_t		*output;
	uint64_t	outputsize;
	unescape(input,length(input),&output,&outputsize);
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
			(*output)=new ucs2_t[(*outputlength)+1];
			(*output)[(*outputlength)]='\0';
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
		str[index]=' ';
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
		str[index]=' ';
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

int32_t ucs2charstring::countLeadingSpaces(const ucs2_t *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	leadingspaces=0;
	for (int32_t index=0; str[index]==' ' && index<len; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int32_t ucs2charstring::countTrailingSpaces(const ucs2_t *str, int32_t len) {

	if (!str) {
		return 0;
	}

	int32_t	trailingspaces=0;
	for (int32_t index=len-1; str[index]==' ' && index>-1; index--) {
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

ucs2_t *ucs2charstring::parseNumber(uint64_t number) {
	return parseNumber(number,1);
}

ucs2_t *ucs2charstring::parseNumber(uint64_t number, uint16_t zeropadding) {
	uint16_t	len=integerLength(number);
	uint16_t	strlen=((zeropadding>len)?zeropadding:len);
	ucs2_t	*ptr=new ucs2_t[strlen+1];
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

uint16_t ucs2charstring::integerLength(int16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int16_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::integerLength(int32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int32_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::integerLength(int64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (int64_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::integerLength(uint16_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint16_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::integerLength(uint32_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint32_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

uint16_t ucs2charstring::integerLength(uint64_t number) {
	uint16_t	len=(number>0)?0:1;
	for (uint64_t num=number; num>0; num=num/10) {
		len++;
	}
	return len;
}

size_t ucs2charstring::length(const ucs2_t *string) {
	size_t	len=0;
	if (string) {
		for (const ucs2_t *s=string; *s; s++) {
			len++;
		}
	}
	return len;
}

size_t ucs2charstring::size(const ucs2_t *string) {
	return (string)?(length(string)+1)*sizeof(ucs2_t):0;
}

bool ucs2charstring::isNullOrEmpty(const ucs2_t *string) {
	return !(string && string[0]);
}

bool ucs2charstring::isYes(const ucs2_t *string) {
	const ucs2_t	yes[]={'y','e','s',0};
	const ucs2_t	truestr[]={'t','r','u','e',0};
	const ucs2_t	on[]={'o','n',0};
	ucs2_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,yes,3)) {
		next=string[3];
	} else if (!compareIgnoringCase(string,truestr,4)) {
		next=string[4];
	} else if (!compareIgnoringCase(string,on,2)) {
		next=string[2];
	} else if (string[0]=='y' || string[0]=='Y' ||
			string[0]=='t' || string[0]=='T' ||
			string[0]=='1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || !ucs2character::isAlphanumeric(next));
}

bool ucs2charstring::isNo(const ucs2_t *string) {
	const ucs2_t	no[]={'n','o',0};
	const ucs2_t	falsestr[]={'f','a','l','s','e',0};
	const ucs2_t	off[]={'o','f','f',0};
	ucs2_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,no,2)) {
		next=string[2];
	} else if (!compareIgnoringCase(string,falsestr,5)) {
		next=string[5];
	} else if (!compareIgnoringCase(string,off,3)) {
		next=string[3];
	} else if (string[0]=='n' || string[0]=='N' ||
			string[0]=='f' || string[0]=='F' ||
			string[0]=='0') {
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
	return append(dest,source,length(source)+1);
}

ucs2_t *ucs2charstring::append(ucs2_t *dest, const ucs2_t *source, size_t len) {
	return copy(dest+length(dest),source,len);
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
	return copy(dest,source,length(source)+1);
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
		*dest='\0';
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
	return safeCopy(dest,destlen,source,length(source)+1);
}

ucs2_t *ucs2charstring::safeCopy(ucs2_t *dest, size_t destlen,
				const ucs2_t *source, size_t sourcelen) {
	return copy(dest,source,(sourcelen>destlen)?destlen:sourcelen);
}

int32_t ucs2charstring::compare(const ucs2_t *str1, const ucs2_t *str2) {
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
	return (len)?*str1-*str2:0;
}

int32_t ucs2charstring::compareIgnoringCase(const ucs2_t *str1,
							const ucs2_t *str2) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2) {
		diff=(ucs2character::toUpperCase(*str1)-
			ucs2character::toUpperCase(*str2));
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
	}
	return ucs2character::toUpperCase(*str1)-
			ucs2character::toUpperCase(*str2);
}

int32_t ucs2charstring::compareIgnoringCase(const ucs2_t *str1,
						const ucs2_t *str2,
						size_t len) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	int32_t	diff=0;
	while (*str1 && *str2 && len) {
		diff=(ucs2character::toUpperCase(*str1)-
			ucs2character::toUpperCase(*str2));
		if (diff) {
			return diff;
		}
		str1++;
		str2++;
		len--;
	}
	return (len)?ucs2character::toUpperCase(*str1)-
			ucs2character::toUpperCase(*str2):0;
}

int32_t ucs2charstring::compareNatural(const ucs2_t *str1,
					const ucs2_t *str2) {
	const ucs2_t	dot[]={'.',0};
	return compareNatural(str1,str2,dot);
}

int32_t ucs2charstring::compareNatural(const ucs2_t *str1,
					const ucs2_t *str2,
					const ucs2_t *delimiters) {

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
				ucs2character::inSet(*str1,delimiters))) {
				str1++;
			}
			start2=str2;
			while (*str2 && (ucs2character::isDigit(*str2) ||
				ucs2character::inSet(*str2,delimiters))) {
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

int32_t ucs2charstring::compareVersions(const ucs2_t *str1,
					const ucs2_t *str2) {
	const ucs2_t	dot[]={'.',0};
	return compareVersions(str1,str2,dot);
}

int32_t ucs2charstring::compareVersions(const ucs2_t *str1,
					const ucs2_t *str2,
					const ucs2_t *delimiters) {

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
	return compareWithWildcards(string,length(string),
					pattern,patternlength,
					singlewildcard,multiwildcard);
}

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					size_t stringlength,
					const ucs2_t *pattern,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {
	return compareWithWildcards(string,stringlength,
					pattern,length(pattern),
					singlewildcard,multiwildcard);
}

bool ucs2charstring::compareWithWildcards(const ucs2_t *string,
					const ucs2_t *pattern,
					ucs2_t singlewildcard,
					ucs2_t multiwildcard) {
	return compareWithWildcards(string,length(string),
					pattern,length(pattern),
					singlewildcard,multiwildcard);
}

bool ucs2charstring::inSet(const ucs2_t *str, const ucs2_t * const *set) {
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

bool ucs2charstring::inSetIgnoringCase(const ucs2_t *str,
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
	return !compare(haystack,needle,length(needle));
}

bool ucs2charstring::endsWith(const ucs2_t *haystack,
					const ucs2_t *needle) {
	size_t	needlelen=length(needle);
	size_t	haystacklen=length(haystack);
	return (haystacklen>=needlelen &&
		!compare(haystack+haystacklen-needlelen,needle));
}

const ucs2_t *ucs2charstring::findFirst(const ucs2_t *haystack,
						const ucs2_t *needle) {
	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
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
	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
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
	size_t	haystacklen=length(haystack);
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
	size_t	haystacklen=length(haystack);
	needle=ucs2character::toLowerCase(needle);
	for (const ucs2_t *ptr=haystack;
			ptr<haystack+haystacklen;
			ptr++) {
		if (ucs2character::toLowerCase(*ptr)==needle) {
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
		retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
	ucs2_t	*retval=haystack+ucs2charstring::length(haystack);
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
	ucs2_t	*retval=haystack+length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+length(haystack);
}

size_t ucs2charstring::lengthContainingSet(const ucs2_t *haystack,
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

size_t ucs2charstring::lengthNotContainingSet(const ucs2_t *haystack,
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

ucs2_t *ucs2charstring::before(const ucs2_t *str,
					const ucs2_t *delimiter) {
	return between(str,NULL,delimiter);
}

ucs2_t *ucs2charstring::between(const ucs2_t *str,
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
		s+=length(start);
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

ucs2_t *ucs2charstring::after(const ucs2_t *str,
					const ucs2_t *delimiter) {
	return between(str,delimiter,NULL);
}

ucs2_t *ucs2charstring::duplicate(const char *str) {
	if (!str) {
		return NULL;
	}
	return duplicate(str,charstring::length(str));
}

ucs2_t *ucs2charstring::duplicate(const char *str, size_t len) {
	if (!str) {
		return NULL;
	}
	ucs2_t	*buffer=new ucs2_t[len+1];
	for (size_t i=0; i<len; i++) {
		buffer[i]=ucs2character::duplicate(str[i]);
	}
	buffer[len]='\0';
	return buffer;
}

ucs2_t *ucs2charstring::duplicate(const ucs2_t *str) {
	if (!str) {
		return NULL;
	}
	return duplicate(str,length(str));
}

ucs2_t *ucs2charstring::duplicate(const ucs2_t *str, size_t len) {
	if (!str) {
		return NULL;
	}
	ucs2_t	*buffer=new ucs2_t[len+1];
	copy(buffer,str,len);
	buffer[len]='\0';
	return buffer;
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string) {
	return duplicate(string,wcharstring::length(string),'?');
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, size_t len) {
	return duplicate(string,len,'?');
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string,
					ucs2_t replacement) {
	return duplicate(string,wcharstring::length(string),replacement);
}

ucs2_t *ucs2charstring::duplicate(const wchar_t *string, size_t len,
							ucs2_t replacement) {
	if (!string) {
		return NULL;
	}
	ucs2_t	*retval=new ucs2_t[len+1];
	for (size_t i=0; i<len; i++) {
		retval[i]=ucs2character::duplicate(string[i],replacement);
	}
	retval[len]='\0';
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
		*str='\0';
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
		str[j]='\0';
	}
}

void ucs2charstring::bothTrim(ucs2_t *string) {
	leftTrim(string);
	rightTrim(string);
}

int64_t ucs2charstring::toInteger(const ucs2_t *string) {
	return toInteger(string,NULL,10);
}

int64_t ucs2charstring::toInteger(const ucs2_t *string,
					const ucs2_t **endptr) {
	return toInteger(string,endptr,10);
}

int64_t ucs2charstring::toInteger(const ucs2_t *string, int32_t base) {
	return toInteger(string,NULL,base);
}

int64_t ucs2charstring::toInteger(const ucs2_t *string,
				const ucs2_t **endptr, int32_t base) {
	char	*str=charstring::duplicate(string);
	int64_t	retval=charstring::toInteger(str,NULL,base);
	delete[] str;
	if (endptr) {
		while (ucs2character::isDigit(*string)) {
			string++;
		}
		*endptr=string;
	}
	return retval;
}

uint64_t ucs2charstring::toUnsignedInteger(const ucs2_t *string) {
	return toUnsignedInteger(string,NULL,10);
}

uint64_t ucs2charstring::toUnsignedInteger(const ucs2_t *string,
					const ucs2_t **endptr) {
	return toUnsignedInteger(string,endptr,10);
}

uint64_t ucs2charstring::toUnsignedInteger(const ucs2_t *string,
							int32_t base) {
	return toUnsignedInteger(string,NULL,base);
}

uint64_t ucs2charstring::toUnsignedInteger(const ucs2_t *string,
						const ucs2_t **endptr,
						int32_t base) {
	char	*str=charstring::duplicate(string);
	int64_t	retval=charstring::toUnsignedInteger(str,NULL,base);
	delete[] str;
	if (endptr) {
		while (ucs2character::isDigit(*string)) {
			string++;
		}
		*endptr=string;
	}
	return retval;
}

long double ucs2charstring::toFloat(const ucs2_t *string) {
	return toFloat(string,NULL);
}

long double ucs2charstring::toFloatC(const ucs2_t *string) {

	/* This method is needed when the locale of the client is different
	 * from the C/POSIX locale, but we still need to convert a string
	 * formatted in C/POSIX locale format to a float. Perhaps the string
	 * was supplied to us by a host which is using the C/POSIX locale).
	 * Because there are no standard, let alone portable api, for string
	 * conversion functions that take a locale as argument, this code
	 * implements a workaround of converting the string from C locale
	 * representation to one appropriate for the current locale. */

#ifdef RUDIMENTS_HAVE_LOCALE_H
	size_t		len=length(string);
	ucs2_t	stringinlocale[256];
	const ucs2_t	*decimalpointlocation;
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

		return toFloat(stringinlocale,NULL);
	}
#endif
	return toFloat(string,NULL);
}

long double ucs2charstring::toFloat(const ucs2_t *string,
					const ucs2_t **endptr) {
	char		*str=charstring::duplicate(string);
	long double	retval=charstring::toFloat(str,NULL);
	delete[] str;
	if (endptr) {
		bool	found=false;
		for (;;) {
			if (ucs2character::isDigit(*string)) {
				string++;
			// FIXME: this test should really use the
			// delimiter from the locale instead of just '.'
			} else if (!found && *string=='.') {
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
	split(string,length(string),
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void ucs2charstring::split(const ucs2_t *string,
				size_t stringlength,
				const ucs2_t *delimiter,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {
	split(string,stringlength,
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void ucs2charstring::split(const ucs2_t *string, 
				const ucs2_t *delimiter,
				size_t delimiterlength,
				bool collapse,
				ucs2_t ***list,
				uint64_t *listlength) {
	split(string,length(string),
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

ucs2_t *ucs2charstring::subString(const ucs2_t *str,
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

ucs2_t *ucs2charstring::subString(const ucs2_t *str, size_t start) {
	return subString(str,start,length(str)-1);
}

ucs2_t *ucs2charstring::insertString(const ucs2_t *dest,
					const ucs2_t *src,
					uint64_t index) {

	uint64_t	srcsize=length(src);
	uint64_t	size=length(dest)+srcsize+1;
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
	retval[size-1]='\0';
	return retval;
}

ucs2_t *ucs2charstring::pad(const ucs2_t *str, ucs2_t padchar,
			int16_t direction, uint64_t totallength) {

	if (totallength==0) {
		return NULL;
	}

	uint64_t	strlen=((str==NULL)?0:length(str));
	ucs2_t	*newstring=NULL;

	newstring=new ucs2_t[totallength+1];
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

ucs2_t *ucs2charstring::humanReadable(int64_t number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::humanReadable(int64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

ucs2_t *ucs2charstring::humanReadable(uint64_t number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::humanReadable(uint64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

ucs2_t *ucs2charstring::humanReadable(long double number) {
	return humanReadable(number,false);
}

ucs2_t *ucs2charstring::humanReadable(long double number, bool onethousand) {

	long double	k=(onethousand)?1000.0:1024.0;
	ucs2_t		suffixes[]={
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
	ucs2_t	*buf2=duplicate(buf);
	delete[] buf;
	const ucs2_t	dotzero[]={'.','0',0};
	const ucs2_t	empty[]={0};
	ucs2_t	*subbed=replace(buf2,dotzero,empty);
	delete[] buf2;
	return subbed;
}

ssize_t ucs2charstring::printf(ucs2_t *buffer, size_t len,
					const ucs2_t *format, ...) {
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

static ucs2_t	*scratchfile=NULL;
static FILE	*scratch=NULL;

static void removeScratch() {
	file::remove(scratchfile);
	if (scratch) {
		fclose(scratch);
		delete[] scratchfile;
	}
}

static ssize_t vsnprintf(ucs2_t *buffer, size_t len,
				const ucs2_t *format, va_list argp) {

	// open a scratch file if it's not already open
	if (!scratch) {

		// first try the null device
		scratchfile=ucs2charstring::duplicate(
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
			ucs2charstring::copy(scratchfile,"/tmp/scratch.");
			ucs2charstring::append(scratchfile,
					(uint64_t)process::getProcessId());
			scratch=fopen(scratchfile,"w+");
			if (scratch) {
				process::atExit((void (*)(void))removeScratch);
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
	ucs2_t	*safebuffer=new ucs2_t[safebuffersize+1];

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
	ucs2charstring::copy(buffer,safebuffer,bytestocopy);

	// clean up
	delete[] safebuffer;

	// return the number of bytes we would like to have copied
	// (except for the NULL terminator)
	return safebuffersize;
}

// now we can say that we have vsnprintf
#define RUDIMENTS_HAVE_VSNPRINTF 1

#endif

ssize_t ucs2charstring::printf(ucs2_t *buffer, size_t len,
					const ucs2_t *format, va_list *argp) {
	// FIXME: implement this...
	return 0;
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
