// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/wcharstring.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/wcharacter.h>
#include <rudiments/process.h>
#include <rudiments/file.h>
#include <rudiments/wstringbuffer.h>

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
#ifdef RUDIMENTS_HAVE_WCHAR_H
	#include <wchar.h>
#endif

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

const wchar_t *wcharstring::findLast(const wchar_t *haystack, const wchar_t *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const wchar_t	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compare(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

const wchar_t *wcharstring::findLastIgnoringCase(const wchar_t *haystack,
						const wchar_t *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const wchar_t	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

void wcharstring::upper(wchar_t *str) {
	if (str) {
		for (wchar_t *ch=str; *ch; ch++) {
			*ch=wcharacter::toUpperCase(*ch);
		}
	}
}

void wcharstring::lower(wchar_t *str) {
	if (str) {
		for (wchar_t *ch=str; *ch; ch++) {
			*ch=wcharacter::toLowerCase(*ch);
		}
	}
}

void wcharstring::capitalize(wchar_t *str) {
	if (str) {
		bool	cap=true;
		for (wchar_t *ch=str; *ch; ch++) {
			if (cap) {
				*ch=wcharacter::toUpperCase(*ch);
				cap=false;
			} else {
				*ch=wcharacter::toLowerCase(*ch);
			}
			if (wcharacter::inSet(*ch,L" '\"-(")) {
				cap=true;
			}
		}
	}
}

void wcharstring::rightTrim(wchar_t *str, wchar_t character) {

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
		*str=L'\0';
	}
}

void wcharstring::leftTrim(wchar_t *str, wchar_t character) {

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
		str[j]=L'\0';
	}
}

bool wcharstring::strip(wchar_t *str, wchar_t character) {

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
	str[index-total]=L'\0';
	return retval;
}

bool wcharstring::strip(wchar_t *str1, const wchar_t *str2) {

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
	str1[index-total]=L'\0';
	return retval;
}

bool wcharstring::stripSet(wchar_t *str, const wchar_t *set) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (wcharacter::inSet(str[index],set)) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]=L'\0';
	return retval;
}

void wcharstring::replace(wchar_t *str, wchar_t oldchar, wchar_t newchar) {
	if (str) {
		for (wchar_t *ptr=str; *ptr; ptr++) {
			if (*ptr==oldchar) {
				*ptr=newchar;
			}
		}
	}
}

void wcharstring::replace(wchar_t *str,
				const wchar_t *oldchars,
				wchar_t newchar) {
	if (str) {
		for (wchar_t *ptr=str; *ptr; ptr++) {
			if (wcharacter::inSet(*ptr,oldchars)) {
				*ptr=newchar;
			}
		}
	}
}

wchar_t *wcharstring::replace(const wchar_t *str,
				const wchar_t *oldstr,
				const wchar_t *newstr) {
	if (!str) {
		return NULL;
	}
	wstringbuffer	newstring;
	ssize_t		oldstrlen=wcharstring::length(oldstr);
	const wchar_t	*ptr=str;
	const wchar_t	*start=ptr;
	while (*ptr) {
		if (!wcharstring::compare(ptr,oldstr,oldstrlen)) {
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

wchar_t *wcharstring::replace(const wchar_t *str,
					const wchar_t * const *oldstrset,
					ssize_t *oldstrlen,
					const wchar_t * const *newstrset) {
	if (!str) {
		return NULL;
	}

	// search and replace
	wstringbuffer	newstring;
	const wchar_t	*ptr=str;
	const wchar_t	*start=ptr;
	while (*ptr) {
		bool	found=false;
		uint64_t i=0;
		for (const wchar_t * const *oldptr=oldstrset;
						*oldptr; oldptr++) {
			if (!wcharstring::compare(
					ptr,oldstrset[i],oldstrlen[i])) {
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

bool wcharstring::isInteger(const wchar_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const wchar_t	*ptr=str;
	for (; *ptr; ptr++) {
		if (((*ptr>L'9' || *ptr<L'0') && *ptr!=L'-') || 
			(ptr>str && *ptr==L'-')) {
			return false;
		}
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!=L'-');
}

bool wcharstring::isInteger(const wchar_t *str, int32_t size) {

	if (!str || !size) {
		return false;
	}

	const wchar_t	*ptr=str;
	for (int32_t index=0; index<size; index++) {
		if (((*ptr>L'9' || *ptr<L'0') && *ptr!=L'-') || 
			(ptr>str && *ptr==L'-')) {
			return false;
		}
		ptr++;
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!=L'-');
}

bool wcharstring::isNumber(const wchar_t *str) {

	if (isNullOrEmpty(str)) {
		return false;
	}

	const wchar_t	*ptr=str;
	int32_t		decimal=0;
	for (; *ptr; ptr++) {
		if (((*ptr>L'9' || *ptr<L'0') && *ptr!=L'-' && *ptr!=L'.') || 
			(ptr>str && *ptr==L'-') || (decimal && *ptr==L'.')) {
			return false;
		}
		if (*ptr==L'.') {
			decimal=1;
		}
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!=L'-' && *(ptr-1)!=L'.');
}

bool wcharstring::isNumber(const wchar_t *str, int32_t size) {

	if (!str || !size) {
		return false;
	}

	const wchar_t	*ptr=str;
	int32_t		decimal=0;
	for (int32_t index=0; index<size; index++) {
		if (((*ptr>L'9' || *ptr<L'0') && *ptr!=L'-' && *ptr!=L'.') || 
			(ptr>str && *ptr==L'-') || (decimal && *ptr==L'.')) {
			return false;
		}
		if (*ptr==L'.') {
			decimal=1;
		}
		ptr++;
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!=L'-' && *(ptr-1)!=L'.');
}

int64_t wcharstring::convertAmount(const wchar_t *amount) {
	if (!amount) {
		return 0;
	}
	const wchar_t	*dollarsstr=findFirst(amount,L'$');
	dollarsstr=(dollarsstr)?dollarsstr+1:amount;
	uint64_t	dollars=toUnsignedInteger(dollarsstr);
	const wchar_t	*centsstr=findFirst(amount,L'.');
	uint64_t	cents=(centsstr)?toUnsignedInteger(centsstr+1):0;
	return (dollars*100+cents);
}

wchar_t *wcharstring::convertAmount(int64_t amount) {
	uint16_t	length=integerLength(amount)+4;
	if (length<6) {
		length=6;
	}
	wchar_t	negative[2];
	if (amount<0) {
		negative[0]=L'-';
	} else {
		negative[0]=L'\0';
	}
	negative[1]=L'\0';
	wchar_t	*amountstr=new wchar_t[length];
	int64_t	amt;
	#ifdef RUDIMENTS_HAVE_LLABS
		amt=llabs((long long)amount);
	#else
		amt=abs((long)amount);
	#endif
	wprintf(amountstr,length,
			"$%s%lld.%02lld",negative,
			amt/100,amt-(amt/100*100));
	return amountstr;
}

wchar_t *wcharstring::convertAmount(int64_t amount, uint16_t spaces) {
	wchar_t	*amt=convertAmount(amount);
	ssize_t	amtlen=length(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	wchar_t	*buffer=new wchar_t[realspaces+1];
	buffer[realspaces]=L'\0';
	bytestring::set(buffer,L' ',realspaces);
	bytestring::copy(buffer+realspaces-amtlen,amt+1,amtlen);
	if (buffer[0]==L' ') {
		buffer[0]=L'$';
	}
	delete[] amt;
	return buffer;
}

wchar_t *wcharstring::escape(const wchar_t *input, const wchar_t *characters) {
	wchar_t		*output;
	uint64_t	outputsize;
	escape(input,length(input),&output,&outputsize,characters);
	return output;
}

void wcharstring::escape(const wchar_t *input, uint64_t inputsize,
				wchar_t **output, uint64_t *outputsize,
						const wchar_t *characters) {

	(*output)=NULL;
	(*outputsize)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputsize;
				inputindex++) {
			if (wcharstring::contains(characters,
						input[inputindex]) ||
					input[inputindex]==L'\\') {
				if (pass==0) {
					(*outputsize)++;
				} else {
					(*output)[outputindex]=L'\\';
					outputindex++;
				}
			}
			if (pass==0) {
				(*outputsize)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
		}
		if (pass==0) {
			(*output)=new wchar_t[(*outputsize)+1];
			(*output)[(*outputsize)]=L'\0';
		}
	}
}

wchar_t *wcharstring::unescape(const wchar_t *input) {
	wchar_t		*output;
	uint64_t	outputsize;
	unescape(input,length(input),&output,&outputsize);
	return output;
}

void wcharstring::unescape(const wchar_t *input, uint64_t inputsize,
				wchar_t **output, uint64_t *outputsize) {

	(*output)=NULL;
	(*outputsize)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		bool		escaped=false;
		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputsize;
				inputindex++) {
			if (!escaped && input[inputindex]==L'\\') {
				escaped=true;
				continue;
			}
			if (pass==0) {
				(*outputsize)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
			escaped=false;
		}
		if (pass==0) {
			(*output)=new wchar_t[(*outputsize)+1];
			(*output)[(*outputsize)]=L'\0';
		}
	}
}

void wcharstring::leftJustify(wchar_t *str, int32_t length) {

	if (!str) {
		return;
	}

	// count leading spaces
	int32_t	spaces=countLeadingSpaces(str,length);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=length-spaces;
	for (index=0; index<stop; index++) {
		str[index]=str[index+spaces];
	}

	// right-pad with spaces
	for (; index<length; index++) {
		str[index]=L' ';
	}
}

void wcharstring::rightJustify(wchar_t *str, int32_t length) {

	if (!str) {
		return;
	}
	
	// count trailing spaces
	int32_t	spaces=countTrailingSpaces(str,length);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=spaces-1;
	for (index=length-1; index>stop; index--) {
		str[index]=str[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		str[index]=L' ';
	}
}

void wcharstring::center(wchar_t *str, int32_t length) {

	if (!str) {
		return;
	}

	int32_t	leadingspaces=countLeadingSpaces(str,length);
	int32_t	trailingspaces=countTrailingSpaces(str,length);

	int32_t	leftpad=(leadingspaces+trailingspaces)/2;

	if (leftpad>leadingspaces) {
		// shift everything right
		int32_t	difference=leftpad-leadingspaces;
		int32_t	index;
		for (index=length-1; index>difference-1; index--) {
			str[index]=str[index-difference];
		}
		for (; index>-1; index--) {
			str[index]=L' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int32_t	difference=leadingspaces-leftpad;
		int32_t	index;
		for (index=0; index<length-difference; index++) {
			str[index]=str[index+difference];
		}
		for (; index<length; index++) {
			str[index]=L' ';
		}
	}
}

int32_t wcharstring::countLeadingSpaces(const wchar_t *str, int32_t length) {

	if (!str) {
		return 0;
	}

	int32_t	leadingspaces=0;
	for (int32_t index=0; str[index]==L' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int32_t wcharstring::countTrailingSpaces(const wchar_t *str, int32_t length) {

	if (!str) {
		return 0;
	}

	int32_t	trailingspaces=0;
	for (int32_t index=length-1; str[index]==L' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

wchar_t *wcharstring::parseNumber(int16_t number) {
	return parseNumber((int64_t)number,1);
}

wchar_t *wcharstring::parseNumber(int16_t number,
				uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

wchar_t *wcharstring::parseNumber(uint16_t number) {
	return parseNumber((uint64_t)number,1);
}

wchar_t *wcharstring::parseNumber(uint16_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

wchar_t *wcharstring::parseNumber(int32_t number) {
	return parseNumber((int64_t)number,1);
}

wchar_t *wcharstring::parseNumber(int32_t number, uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

wchar_t *wcharstring::parseNumber(uint32_t number) {
	return parseNumber((uint64_t)number,1);
}

wchar_t *wcharstring::parseNumber(uint32_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

wchar_t *wcharstring::parseNumber(int64_t number) {
	return parseNumber(number,1);
}

wchar_t *wcharstring::parseNumber(int64_t number, uint16_t zeropadding) {
	if (number>=0) {
		return parseNumber((uint64_t)number,zeropadding);
	}
	uint16_t	len=integerLength(number);
	uint16_t	strlength=((zeropadding>len)?zeropadding:len);
	wchar_t		*ptr=new wchar_t[strlength+1];
	*ptr=L'-';
	ptr+=strlength;
	*ptr=L'\0';
	number*=-1;
	strlength--;
	while (strlength) {
		ptr--;
		*ptr=L'0'+number%10;
		number/=10;
		strlength--;
	}
	ptr--;
	return ptr;
}

wchar_t *wcharstring::parseNumber(uint64_t number) {
	return parseNumber(number,1);
}

wchar_t *wcharstring::parseNumber(uint64_t number, uint16_t zeropadding) {
	uint16_t	len=integerLength(number);
	uint16_t	strlength=((zeropadding>len)?zeropadding:len);
	wchar_t		*ptr=new wchar_t[strlength+1];
	ptr+=strlength;
	*ptr=L'\0';
	while (strlength) {
		ptr--;
		*ptr=L'0'+number%10;
		number/=10;
		strlength--;
	}
	return ptr;
}

wchar_t *wcharstring::parseNumber(float number) {
	return parseNumber((long double)number);
}

wchar_t *wcharstring::parseNumber(float number, uint16_t scale) {
	return parseNumber((long double)number,scale);
}

wchar_t *wcharstring::parseNumber(float number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

wchar_t *wcharstring::parseNumber(double number) {
	return parseNumber((long double)number);
}

wchar_t *wcharstring::parseNumber(double number,
				uint16_t scale) {
	return parseNumber((long double)number,scale);
}

wchar_t *wcharstring::parseNumber(double number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

wchar_t *wcharstring::parseNumber(long double number) {
	wchar_t	*str=new wchar_t[22];
	wprintf(str,22,"%Lf",number);
	return str;
}

wchar_t *wcharstring::parseNumber(long double number, uint16_t scale) {
	wchar_t	*str=new wchar_t[22];
	wprintf(str,22,"%.*Lf",scale,number);
	return str;
}

wchar_t *wcharstring::parseNumber(long double number,
				uint16_t precision, uint16_t scale) {
	size_t	strlength=precision+3;
	wchar_t	*str=new wchar_t[strlength];
	wprintf(str,strlength,"%*.*Lf",precision,scale,number);
	return str;
}

void wcharstring::bothTrim(wchar_t *string, wchar_t character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

uint16_t wcharstring::integerLength(int16_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int16_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t wcharstring::integerLength(int32_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int32_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t wcharstring::integerLength(int64_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int64_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t wcharstring::integerLength(uint16_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint16_t num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t wcharstring::integerLength(uint32_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint32_t num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t wcharstring::integerLength(uint64_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint64_t num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

size_t wcharstring::length(const wchar_t *string) {
	return (string)?wcslen(string):0;
}

size_t wcharstring::size(const wchar_t *string) {
	return (string)?wcslen(string)+sizeof(wchar_t):0;
}

bool wcharstring::isNullOrEmpty(const wchar_t *string) {
	return !(string && string[0]);
}

bool wcharstring::isYes(const wchar_t *string) {
	wchar_t	next;
	if (!string) {
		return false;
	} else if (!wcharstring::compareIgnoringCase(string,L"yes",3)) {
		next=string[3];
	} else if (!wcharstring::compareIgnoringCase(string,L"true",4)) {
		next=string[4];
	} else if (!wcharstring::compareIgnoringCase(string,L"on",2)) {
		next=string[2];
	} else if (string[0]==L'y' || string[0]==L'Y' ||
			string[0]==L't' || string[0]==L'T' ||
			string[0]==L'1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!wcharacter::isAlphanumeric(next) && 
				!wcharacter::isAlphabeticalExtended(next)));
}

bool wcharstring::isNo(const wchar_t *string) {
	wchar_t	next;
	if (!string) {
		return false;
	} else if (!wcharstring::compareIgnoringCase(string,L"no",2)) {
		next=string[2];
	} else if (!wcharstring::compareIgnoringCase(string,L"false",5)) {
		next=string[5];
	} else if (!wcharstring::compareIgnoringCase(string,L"off",3)) {
		next=string[3];
	} else if (string[0]==L'n' || string[0]==L'N' ||
			string[0]==L'f' || string[0]==L'F' ||
			string[0]==L'0') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!wcharacter::isAlphanumeric(next) && 
				!wcharacter::isAlphabeticalExtended(next)));
}

void wcharstring::zero(wchar_t *str, size_t length) {
	bytestring::set(str,0,length*sizeof(wchar_t));
}

wchar_t *wcharstring::append(wchar_t *dest, const wchar_t *source) {
	return append(dest,source,length(source)+1);
}

wchar_t *wcharstring::append(wchar_t *dest,
				const wchar_t *source, size_t size) {
	return copy(dest+length(dest),source,size);
}

wchar_t *wcharstring::append(wchar_t *dest, int64_t number) {
	wchar_t	*str=wcharstring::parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, uint64_t number) {
	wchar_t	*str=wcharstring::parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number) {
	wchar_t	*str=wcharstring::parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number, uint16_t scale) {
	wchar_t	*str=wcharstring::parseNumber(number,scale);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number,
					uint16_t precision, uint16_t scale) {
	wchar_t	*str=wcharstring::parseNumber(number,precision,scale);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::copy(wchar_t *dest, const wchar_t *source) {
	return copy(dest,source,length(source)+1);
}

wchar_t *wcharstring::copy(wchar_t *dest, const wchar_t *source, size_t size) {
	if (!dest || !source) {
		return dest;
	}
	return wcsncpy(dest,source,size);
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
						const wchar_t *source) {
	return copy(dest+location,source);
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
					const wchar_t *source, size_t size) {
	return copy(dest+location,source,size);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destsize,
						const wchar_t *source) {
	return safeCopy(dest,destsize,source,length(source)+1);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destsize,
				const wchar_t *source, size_t sourcesize) {
	return copy(dest,source,(sourcesize>destsize)?destsize:sourcesize);
}

int32_t wcharstring::compare(const wchar_t *str1, const wchar_t *str2) {
	// FIXME: use strcoll?
	return (str1 && str2)?wcscmp(str1,str2):(str1!=str2);
}

int32_t wcharstring::compare(const wchar_t *str1, const wchar_t *str2,
							size_t size) {
	return (str1 && str2)?wcsncmp(str1,str2,size):(str1!=str2);
}

int32_t wcharstring::compareIgnoringCase(const wchar_t *str1,
						const wchar_t *str2) {
	#ifdef RUDIMENTS_HAVE_STRCASECMP
		return (str1 && str2)?wcscasecmp(str1,str2):(str1!=str2);
	#else
		if (!str1 || !str2) {
			return (str1!=str2);
		}
		const wchar_t	*ptr1=str1;
		const wchar_t	*ptr2=str2;
		int32_t		diff=0;
		while (*ptr1 && *ptr2) {
			diff=wcharacter::toUpperCase(*ptr1)-
				wcharacter::toUpperCase(*ptr2);
			if (diff) {
				return diff;
			}
			ptr1++;
			ptr2++;
		}
		if (*ptr1) {
			return *ptr1;
		} else if (*ptr2) {
			return -(*ptr2);
		}
		return 0;
	#endif
}

int32_t wcharstring::compareIgnoringCase(const wchar_t *str1,
						const wchar_t *str2,
						size_t size) {
	#ifdef RUDIMENTS_HAVE_STRNCASECMP
		return (str1 && str2)?wcsncasecmp(str1,str2,size):(str1!=str2);
	#else
		if (!str1 || !str2) {
			return (str1!=str2);
		}
		size_t		count=0;
		const wchar_t	*ptr1=str1;
		const wchar_t	*ptr2=str2;
		int32_t		diff=0;
		while (*ptr1 && *ptr2 && count<size) {
			diff=wcharacter::toUpperCase(*ptr1)-
				wcharacter::toUpperCase(*ptr2);
			if (diff) {
				return diff;
			}
			ptr1++;
			ptr2++;
			count++;
		}
		if (count<size) {
			if (*ptr1 && !*ptr2) {
				return *ptr1;
			} else if (*ptr2 && !*ptr1) {
				return -(*ptr2);
			}
		}
		return 0;
	#endif
}

bool wcharstring::inSet(const wchar_t *str, const wchar_t * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const wchar_t * const *s=set; *s; s++) {
		if (!compare(str,*s)) {
			return true;
		}
	}
	return false;
}

bool wcharstring::inSetIgnoringCase(const wchar_t *str,
					const wchar_t * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const wchar_t * const *s=set; *s; s++) {
		if (!compareIgnoringCase(str,*s)) {
			return true;
		}
	}
	return false;
}

bool wcharstring::contains(const wchar_t *haystack, const wchar_t *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool wcharstring::containsIgnoringCase(const wchar_t *haystack,
						const wchar_t *needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool wcharstring::contains(const wchar_t *haystack, wchar_t needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool wcharstring::containsIgnoringCase(const wchar_t *haystack,
							wchar_t needle) {
	return (findFirstIgnoringCase(haystack,needle)!=NULL);
}

bool wcharstring::startsWith(const wchar_t *haystack, const wchar_t *needle) {
	return !wcharstring::compare(haystack,needle,
					wcharstring::length(needle));
}

bool wcharstring::endsWith(const wchar_t *haystack, const wchar_t *needle) {
	size_t	needlelen=wcharstring::length(needle);
	size_t	haystacklen=wcharstring::length(haystack);
	return (haystacklen>=needlelen &&
		!wcharstring::compare(haystack+haystacklen-needlelen,needle));
}

const wchar_t *wcharstring::findFirst(const wchar_t *haystack,
					const wchar_t *needle) {
	return (haystack && needle)?wcsstr(haystack,needle):NULL;
}

const wchar_t *wcharstring::findFirstIgnoringCase(const wchar_t *haystack,
							const wchar_t *needle) {
	size_t	haystacklen=wcharstring::length(haystack);
	size_t	needlelen=wcharstring::length(needle);
	for (const wchar_t *ptr=haystack;
			ptr<=haystack+haystacklen-needlelen;
			ptr++) {
		if (!wcharstring::compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
	}
	return NULL;
}

const wchar_t *wcharstring::findFirst(const wchar_t *haystack,
							wchar_t needle) {
	return (haystack)?wcschr(haystack,needle):NULL;
}

const wchar_t *wcharstring::findFirstIgnoringCase(const wchar_t *haystack,
							wchar_t needle) {
	size_t	haystacklen=wcharstring::length(haystack);
	needle=wcharacter::toLowerCase(needle);
	for (const wchar_t *ptr=haystack;
			ptr<haystack+haystacklen;
			ptr++) {
		if (wcharacter::toLowerCase(*ptr)==needle) {
			return ptr;
		}
	}
	return NULL;
}

const wchar_t *wcharstring::findFirstOrEnd(const wchar_t *haystack,
							wchar_t needle) {
	#ifdef RUDIMENTS_HAVE_WCSCHRNUL
	return (haystack)?wcschrnul(haystack,needle):NULL;
	#else
	if (!haystack) {
		return NULL;
	}
	const wchar_t	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+wcharstring::length(haystack);
	}
	return retval;
	#endif
}

const wchar_t *wcharstring::findFirstOrEndIgnoringCase(
						const wchar_t *haystack,
						wchar_t needle) {
	if (!haystack) {
		return NULL;
	}
	const wchar_t	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+wcharstring::length(haystack);
	}
	return retval;
}

const wchar_t *wcharstring::findFirstOrEnd(const wchar_t *haystack,
						const wchar_t *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const wchar_t	*retval=findFirst(haystack,needle);
	if (!retval) {
		retval=haystack+wcharstring::length(haystack);
	}
	return retval;
}

const wchar_t *wcharstring::findFirstOrEndIgnoringCase(const wchar_t *haystack,
							const wchar_t *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const wchar_t	*retval=findFirstIgnoringCase(haystack,needle);
	if (!retval) {
		retval=haystack+wcharstring::length(haystack);
	}
	return retval;
}

wchar_t *wcharstring::findFirstOrEnd(wchar_t *haystack, const wchar_t *needle) {
	return (wchar_t *)(findFirstOrEnd((const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findFirstOrEndIgnoringCase(wchar_t *haystack,
						const wchar_t *needle) {
	return (wchar_t *)(findFirstOrEndIgnoringCase(
					(const wchar_t *)(haystack),needle));
}

const wchar_t *wcharstring::findLast(const wchar_t *haystack, wchar_t needle) {
	return (haystack)?wcsrchr(haystack,needle):NULL;
}

const wchar_t *wcharstring::findLastIgnoringCase(const wchar_t *haystack,
							wchar_t needle) {
	return (wchar_t *)(findFirstOrEndIgnoringCase(
					(const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findFirst(wchar_t *haystack, const wchar_t *needle) {
	return (wchar_t *)(findFirst((const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findFirstIgnoringCase(
				wchar_t *haystack, const wchar_t *needle) {
	return (wchar_t *)(findFirstIgnoringCase(
				(const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findFirst(wchar_t *haystack, wchar_t needle) {
	return (wchar_t *)(findFirst((const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findFirstIgnoringCase(
				wchar_t *haystack, wchar_t needle) {
	return (wchar_t *)(findFirstIgnoringCase(
				(const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findLast(wchar_t *haystack, const wchar_t *needle) {
	return (wchar_t *)(findLast((const wchar_t *)(haystack),needle));
}

wchar_t *wcharstring::findLast(wchar_t *haystack, wchar_t needle) {
	return (wchar_t *)(findLast((const wchar_t *)(haystack),needle));
}

const wchar_t *wcharstring::findFirstOfSet(const wchar_t *haystack,
							const wchar_t *set) {
	return (wchar_t *)(findFirstOfSet((wchar_t *)(haystack),set));
}

wchar_t *wcharstring::findFirstOfSet(wchar_t *haystack, const wchar_t *set) {
	#ifdef RUDIMENTS_HAVE_WCSPBRK
	return (haystack && set)?wcspbrk(haystack,set):NULL;
	#else
	if (!haystack || !set) {
		return NULL;
	}
	wchar_t	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return NULL;
	#endif
}

const wchar_t *wcharstring::findFirstOfSetOrEnd(const wchar_t *haystack,
							const wchar_t *set) {
	return (wchar_t *)(findFirstOfSetOrEnd((wchar_t *)(haystack),set));
}

wchar_t *wcharstring::findFirstOfSetOrEnd(wchar_t *haystack,
						const wchar_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	wchar_t	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return retval;
}

const wchar_t *wcharstring::findLastOfSet(const wchar_t *haystack,
						const wchar_t *set) {
	return (wchar_t *)(findLastOfSet((wchar_t *)(haystack),set));
}

wchar_t *wcharstring::findLastOfSet(wchar_t *haystack, const wchar_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	wchar_t	*retval=haystack+wcharstring::length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return NULL;
}

const wchar_t *wcharstring::findLastOfSetOrEnd(const wchar_t *haystack,
							const wchar_t *set) {
	return (wchar_t *)(findLastOfSetOrEnd((wchar_t *)(haystack),set));
}

wchar_t *wcharstring::findLastOfSetOrEnd(wchar_t *haystack, const wchar_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	wchar_t	*retval=haystack+wcharstring::length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+wcharstring::length(haystack);
}

size_t wcharstring::lengthContainingSet(const wchar_t *haystack,
						const wchar_t *set) {
	#ifdef RUDIMENTS_HAVE_WCSSPN
	return (haystack && set)?wcsspn(haystack,set):0;
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

size_t wcharstring::lengthNotContainingSet(const wchar_t *haystack,
						const wchar_t *set) {
	#ifdef RUDIMENTS_HAVE_WCSCSPN
	return (haystack && set)?wcscspn(haystack,set):0;
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

wchar_t *wcharstring::before(const wchar_t *str, const wchar_t *delimiter) {
	return between(str,NULL,delimiter);
}

wchar_t *wcharstring::between(const wchar_t *str, const wchar_t *start,
							const wchar_t *end) {

	// find the start (or use beginning of "str" if "start" is empty/NULL)
	const wchar_t	*s=NULL;
	if (wcharstring::isNullOrEmpty(start)) {
		s=str;
	} else {
		s=findFirst(str,start);
	}
	if (!s) {
		return NULL;
	}

	// bump past the start
	if (s!=str) {
		s+=wcharstring::length(start);
	}

	// find the end (or use end of "str" if "end" is empty/NULL)
	const wchar_t	*e=NULL;
	if (wcharstring::isNullOrEmpty(end)) {
		for (e=s; *e; e++) {}
	} else {
		e=findFirst(s,end);
	}
	if (!e) {
		return NULL;
	}

	// copy out the string between s and e
	return wcharstring::duplicate(s,e-s);
}

wchar_t *wcharstring::after(const wchar_t *str, const wchar_t *delimiter) {
	return between(str,delimiter,NULL);
}

wchar_t *wcharstring::duplicate(const wchar_t *str) {
	if (!str) {
		return NULL;
	}
	return duplicate(str,length(str));
}

wchar_t	*wcharstring::duplicate(const char *string, size_t length) {
	if (!string) {
		return NULL;
	}
	wchar_t		*retval=new wchar_t[length+1];
	for (size_t i=0; i<length; i++) {
		retval[i]=wcharacter::duplicate(string[i]);
	}
	retval[length]=L'\0';
	return retval;
}

wchar_t	*wcharstring::duplicate(const char *string) {
	return duplicate(string,charstring::length(string));
}

void wcharstring::rightTrim(wchar_t *str) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (wcharacter::isWhitespace(*str)) { 
			str--;
		}
		str++;

		// terminate the string there
		*str=L'\0';
	}
}

void wcharstring::leftTrim(wchar_t *str) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
		while (wcharacter::isWhitespace(str[i])) {
			i++;
		}
	
		// swap chars to front of string
		while (str[i]) {
			str[j]=str[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		str[j]=L'\0';
	}
}

void wcharstring::bothTrim(wchar_t *string) {
	leftTrim(string);
	rightTrim(string);
}

int64_t wcharstring::toInteger(const wchar_t *string) {
	return toInteger(string,NULL,10);
}

int64_t wcharstring::toInteger(const wchar_t *string, const wchar_t **endptr) {
	return toInteger(string,endptr,10);
}

int64_t wcharstring::toInteger(const wchar_t *string, int32_t base) {
	return toInteger(string,NULL,base);
}

int64_t wcharstring::toInteger(const wchar_t *string,
				const wchar_t **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_WCSTOLL
	return (string)?wcstoll(string,(wchar_t **)endptr,base):0;
	#else
	return (string)?wcstol(string,(wchar_t **)endptr,base):0;
	#endif
}

uint64_t wcharstring::toUnsignedInteger(const wchar_t *string) {
	return toUnsignedInteger(string,NULL,10);
}

uint64_t wcharstring::toUnsignedInteger(const wchar_t *string,
					const wchar_t **endptr) {
	return toUnsignedInteger(string,endptr,10);
}

uint64_t wcharstring::toUnsignedInteger(const wchar_t *string, int32_t base) {
	return toUnsignedInteger(string,NULL,base);
}

uint64_t wcharstring::toUnsignedInteger(const wchar_t *string,
					const wchar_t **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_WCSTOULL
	return (string)?wcstoull(string,(wchar_t **)endptr,base):0;
	#else
	return (string)?wcstoul(string,(wchar_t **)endptr,base):0;
	#endif
}

long double wcharstring::toFloat(const wchar_t *string) {
	return toFloat(string,NULL);
}

long double wcharstring::toFloatC(const wchar_t *string) {

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
	wchar_t		stringinlocale[256];
	const wchar_t	*decimalpointlocation;
	struct	lconv	*currentlconv=localeconv();
	if ((currentlconv!=NULL) &&
		(currentlconv->decimal_point!=NULL) &&
		(currentlconv->decimal_point[0]!=0) &&
		(currentlconv->decimal_point[0]!=L'.') &&
		(currentlconv->decimal_point[1]==0) &&
		((decimalpointlocation=findFirst(string,L'.'))!=NULL) &&
		(len<sizeof(stringinlocale))) {

		bytestring::copy(stringinlocale,string,len+1);

		stringinlocale[decimalpointlocation-string]=
					currentlconv->decimal_point[0];

		return toFloat(stringinlocale,NULL);
	}
#endif
	return toFloat(string,NULL);
}

long double wcharstring::toFloat(const wchar_t *string,
					const wchar_t **endptr) {
	#ifdef RUDIMENTS_HAVE_WCSTOLD
	return (string)?wcstold(string,(wchar_t **)endptr):0.0;
	#else
	return (string)?(long double)(wcstod(string,(wchar_t **)endptr)):0.0;
	#endif
}

wchar_t *wcharstring::duplicate(const wchar_t *str, size_t length) {
	if (!str) {
		return NULL;
	}
	wchar_t	*buffer=new wchar_t[length+1];
	copy(buffer,str,length);
	buffer[length]=L'\0';
	return buffer;
}

void wcharstring::split(const wchar_t *string, const wchar_t *delimiter,
				bool collapse,
				wchar_t ***list, uint64_t *listlength) {
	split(string,length(string),
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string, ssize_t stringlength,
				const wchar_t *delimiter, bool collapse,
				wchar_t ***list, uint64_t *listlength) {
	split(string,stringlength,
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string, 
				const wchar_t *delimiter,
				ssize_t delimiterlength,
				bool collapse,
				wchar_t ***list,
				uint64_t *listlength) {
	split(string,length(string),
			delimiter,delimiterlength,
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string,
				ssize_t stringlength,
				const wchar_t *delimiter,
				ssize_t delimiterlength,
				bool collapse,
				wchar_t ***list,
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
		const wchar_t	*start=string;
		const wchar_t	*end=string+stringlength;

		// initialize the list length
		ll=0;

		// loop through the string...
		const wchar_t	*current=start;
		for (;;) {

			// if there's not enough room left in the string for
			// another delimiter, then move the current position
			// to the end
			if (end-current<delimiterlength) {
				current=end;
			}

			// if we found a delimiter or ran into the end of
			// the string...
			if (current==end ||
				!wcharstring::compare(current,delimiter,
							delimiterlength)) {

				// handle cases of multiple delimiters in a row
				if (current!=start || !collapse) {

					// if we're on the second pass...
					if (pass && list) {

						// make a copy of the string
						// between the last delimiter
						// and here
						(*list)[ll]=
							wcharstring::duplicate(
								start,
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
			(*list)=new wchar_t *[ll];
		}
	}

	// copy out the listlength
	if (listlength) {
		(*listlength)=ll;
	}
}

wchar_t *wcharstring::subString(const wchar_t *str, size_t start, size_t end) {

	// handle end<start
	if (end<start) {
		size_t	temp=end;
		end=start;
		start=temp;
	}
	return duplicate(str+start,end-start+1);
}

wchar_t *wcharstring::subString(const wchar_t *str, size_t start) {
	return subString(str,start,length(str)-1);
}

wchar_t *wcharstring::insertString(const wchar_t *dest,
				const wchar_t *src, uint64_t index) {

	uint64_t	srcsize=length(src);
	uint64_t	size=length(dest)+srcsize+1;
	wchar_t		*retval=new wchar_t[size];
	for (uint64_t i=0,j=0; i<size;) {
		if (i==index) {
			for (uint64_t k=0; k<srcsize; k++) {
				retval[i++]=src[k];
			}
		} else {
			retval[i++]=dest[j++];
		}
	}
	retval[size-1]=L'\0';
	return retval;
}

wchar_t *wcharstring::pad(const wchar_t *str, wchar_t padchar,
			int16_t direction, uint64_t totallength) {

	if (totallength==0) {
		return NULL;
	}

	uint64_t	strlen=((str==NULL)?0:length(str));
	wchar_t		*newstring=NULL;

	newstring=new wchar_t[totallength+1];
	if (strlen>=totallength) {
		wcharstring::copy(newstring,str,totallength);
		newstring[totallength]=0;
		return newstring;
	}

	bytestring::set(newstring,padchar,totallength);
	newstring[totallength]=0;

	if (direction<0) {
		// pad left
		wcharstring::copy(&newstring[totallength-strlen],str,strlen);
	} else if (direction>0) {
		// pad right
		wcharstring::copy(newstring,str,strlen);
	} else {
		// pad center
		wcharstring::copy(&newstring[(totallength-strlen)/2],str,strlen);
	}

	return newstring;
}

ssize_t wcharstring::printf(wchar_t *buffer, size_t length,
					const wchar_t *format, ...) {
	va_list	args;
	va_start(args,format);
	size_t	result=printf(buffer,length,format,&args);
	va_end(args);
	return result;
}

ssize_t wcharstring::printf(wchar_t *buffer, size_t length,
					const wchar_t *format, va_list *argp) {

	// vswprintf should write whatever will fit into "buffer" and
	// either return the number of bytes that were written or -1
	// if truncation occurs.
	//
	// Iterate, expanding the buffer as necessary.
	size_t	buflen=length;
	size_t	inc=16;
	ssize_t	size=-1;
	do {

		wchar_t	*buf=new wchar_t[buflen];

		size=vswprintf(buf,buflen,format,*argp);
		if (size>-1) {
			wcharstring::copy(buffer,buf,length);
		}
		delete[] buf;

		buflen=buflen+inc;

		// adjust how quickly the buffer grows
		// (this can certainly be optimized further)
		inc=inc*2;
		if (inc>1024) {
			inc=1024;
		}
	} while (size==-1);
	return size;
}

ssize_t wcharstring::printf(wchar_t **buffer, const wchar_t *format, ...) {
	va_list	args;
	va_start(args,format);
	// the printf() below will call va_end(args)
	return printf(buffer,format,&args);
}

ssize_t wcharstring::printf(wchar_t **buffer,
				const wchar_t *format, va_list *argp) {

	// sanity check
	if (!buffer) {
		return 0;
	}

	// initialize *buffer
	*buffer=NULL;

	// Ideally we'd use vaswprintf() if it's available, but we want to be
	// able to delete[] buffer, and if we use vasprintf() to create it then
	// we'd have to free(buffer).

	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// whether it's the global function printf() or one that you've defined
	// yourself.  Using *buffer here works around that.
	va_list	argp1;
	va_copy(argp1,*argp);
	ssize_t	size=wcharstring::printf(*buffer,0,format,argp);
	va_end(*argp);
	*buffer=new wchar_t[size+1];
	size=wcharstring::printf(*buffer,size+1,format,&argp1);
	va_end(argp1);
	return size;
}
