// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/wcharstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/wcharacter.h>
#include <rudiments/process.h>
#include <rudiments/file.h>
#include <rudiments/wstringbuffer.h>
#include <rudiments/error.h>

#ifndef RUDIMENTS_HAVE_VSWPRINTF
	#include <rudiments/charstring.h>
	#include <rudiments/character.h>
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

// for vswprintf
#include <stdio.h>

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
#ifdef RUDIMENTS_HAVE_WCSTR_H
	#include <wcstr.h>
#endif

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

const wchar_t *wcharstring::findLast(const wchar_t *haystack,
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
	size_t		oldstrlen=length(oldstr);
	const wchar_t	*ptr=str;
	const wchar_t	*start=ptr;
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

wchar_t *wcharstring::replace(const wchar_t *str,
					const wchar_t * const *oldstrset,
					size_t *oldstrlen,
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

bool wcharstring::isInteger(const wchar_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const wchar_t	*ptr=str;
	for (int32_t index=0; index<len; index++) {
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

bool wcharstring::isNumber(const wchar_t *str, int32_t len) {

	if (!str || !len) {
		return false;
	}

	const wchar_t	*ptr=str;
	int32_t		decimal=0;
	for (int32_t index=0; index<len; index++) {
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
	wchar_t	*amountstr=new wchar_t[length];
	#ifdef RUDIMENTS_HAVE_SWPRINTF
		wchar_t	negative[2];
		if (amount<0) {
			negative[0]=L'-';
		} else {
			negative[0]=L'\0';
		}
		negative[1]=L'\0';
		int64_t	amt;
		#ifdef RUDIMENTS_HAVE_LLABS
			amt=llabs((long long)amount);
		#else
			amt=abs((long)amount);
		#endif
		swprintf(amountstr,length,
			L"$%s%lld.%02lld",negative,
			amt/100,amt-(amt/100*100));
	#else
		char	*temp=charstring::convertAmount(amount);
		copy(amountstr,temp,length);
		delete[] temp;
	#endif
	return amountstr;
}

wchar_t *wcharstring::convertAmount(int64_t amount, uint16_t spaces) {
	wchar_t	*amt=convertAmount(amount);
	size_t	amtlen=length(amt+1);
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
	uint64_t	outputlen;
	escape(input,length(input),&output,&outputlen,characters);
	return output;
}

void wcharstring::escape(const wchar_t *input, uint64_t inputlen,
				wchar_t **output, uint64_t *outputlen,
						const wchar_t *characters) {

	(*output)=NULL;
	(*outputlen)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputlen;
				inputindex++) {
			if (contains(characters,input[inputindex]) ||
						input[inputindex]==L'\\') {
				if (pass==0) {
					(*outputlen)++;
				} else {
					(*output)[outputindex]=L'\\';
					outputindex++;
				}
			}
			if (pass==0) {
				(*outputlen)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
		}
		if (pass==0) {
			(*output)=new wchar_t[(*outputlen)+1];
			(*output)[(*outputlen)]=L'\0';
		}
	}
}

wchar_t *wcharstring::unescape(const wchar_t *input) {
	wchar_t		*output;
	uint64_t	outputlen;
	unescape(input,length(input),&output,&outputlen);
	return output;
}

void wcharstring::unescape(const wchar_t *input, uint64_t inputlen,
				wchar_t **output, uint64_t *outputlen) {

	(*output)=NULL;
	(*outputlen)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		bool		escaped=false;
		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputlen;
				inputindex++) {
			if (!escaped && input[inputindex]==L'\\') {
				escaped=true;
				continue;
			}
			if (pass==0) {
				(*outputlen)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
			escaped=false;
		}
		if (pass==0) {
			(*output)=new wchar_t[(*outputlen)+1];
			(*output)[(*outputlen)]=L'\0';
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
	#ifdef RUDIMENTS_HAVE_SWPRINTF
		swprintf(str,22,L"%Lf",number);
	#else
		char	*temp=charstring::parseNumber(number);
		copy(str,temp,22);
		delete[] temp;
	#endif
	return str;
}

wchar_t *wcharstring::parseNumber(long double number, uint16_t scale) {
	wchar_t	*str=new wchar_t[22];
	#ifdef RUDIMENTS_HAVE_SWPRINTF
		swprintf(str,22,L"%.*Lf",scale,number);
	#else
		char	*temp=charstring::parseNumber(number,scale);
		copy(str,temp,22);
		delete[] temp;
	#endif
	return str;
}

wchar_t *wcharstring::parseNumber(long double number,
				uint16_t precision, uint16_t scale) {
	size_t	strlength=precision+3;
	wchar_t	*str=new wchar_t[strlength];
	#ifdef RUDIMENTS_HAVE_SWPRINTF
		swprintf(str,strlength,L"%*.*Lf",precision,scale,number);
	#else
		char	*temp=charstring::parseNumber(number,precision,scale);
		copy(str,temp,strlength);
		delete[] temp;
	#endif
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
	if (!string) {
		return 0;
	}
	#ifdef RUDIMENTS_HAVE_WCSLEN
		return wcslen(string);
	#else
		// FIXME: only works with UCS-2/UCS-4
		size_t	len=0;
		for (const wchar_t *c=string; *c; c++) {
			len++;
		}
		return len;
	#endif
}

size_t wcharstring::size(const wchar_t *string) {
	// FIXME: only works with UCS-2/UCS-4
	return (string)?((length(string)+1)*sizeof(wchar_t)):0;
}

bool wcharstring::isNullOrEmpty(const wchar_t *string) {
	return !(string && string[0]);
}

bool wcharstring::isYes(const wchar_t *string) {
	wchar_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,L"yes",3)) {
		next=string[3];
	} else if (!compareIgnoringCase(string,L"true",4)) {
		next=string[4];
	} else if (!compareIgnoringCase(string,L"on",2)) {
		next=string[2];
	} else if (string[0]==L'y' || string[0]==L'Y' ||
			string[0]==L't' || string[0]==L'T' ||
			string[0]==L'1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || !wcharacter::isAlphanumeric(next));
}

bool wcharstring::isNo(const wchar_t *string) {
	wchar_t	next;
	if (!string) {
		return false;
	} else if (!compareIgnoringCase(string,L"no",2)) {
		next=string[2];
	} else if (!compareIgnoringCase(string,L"false",5)) {
		next=string[5];
	} else if (!compareIgnoringCase(string,L"off",3)) {
		next=string[3];
	} else if (string[0]==L'n' || string[0]==L'N' ||
			string[0]==L'f' || string[0]==L'F' ||
			string[0]==L'0') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || !wcharacter::isAlphanumeric(next));
}

void wcharstring::zero(wchar_t *str, size_t length) {
	bytestring::set(str,0,length*sizeof(wchar_t));
}

wchar_t *wcharstring::append(wchar_t *dest, const wchar_t *source) {
	return append(dest,source,length(source)+1);
}

wchar_t *wcharstring::append(wchar_t *dest,
				const wchar_t *source, size_t len) {
	return copy(dest+length(dest),source,len);
}

wchar_t *wcharstring::append(wchar_t *dest, int64_t number) {
	wchar_t	*str=parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, uint64_t number) {
	wchar_t	*str=parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number) {
	wchar_t	*str=parseNumber(number);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number, uint16_t scale) {
	wchar_t	*str=parseNumber(number,scale);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::append(wchar_t *dest, double number,
					uint16_t precision, uint16_t scale) {
	wchar_t	*str=parseNumber(number,precision,scale);
	wchar_t	*retval=append(dest,str);
	delete[] str;
	return retval;
}

wchar_t *wcharstring::copy(wchar_t *dest, const wchar_t *source) {
	if (dest && source) {
		while (*source) {
			*dest=*source;
			dest++;
			source++;
		}
		*dest=L'\0';
	}
	return dest;
}

wchar_t *wcharstring::copy(wchar_t *dest, const char *source) {
	if (dest && source) {
		while (*source) {
			*dest=wcharacter::duplicate(*source);
			dest++;
			source++;
		}
		*dest=L'\0';
	}
	return dest;
}

wchar_t *wcharstring::copy(wchar_t *dest, const wchar_t *source, size_t len) {
	if (!dest || !source) {
		return dest;
	}
	#ifdef RUDIMENTS_HAVE_WCSNCPY
		return wcsncpy(dest,source,len);
	#else
		while (*source && len) {
			*dest=*source;
			dest++;
			source++;
			len--;
		}
		if (len) {
			*dest=L'\0';
		}
		return dest;
	#endif
}

wchar_t *wcharstring::copy(wchar_t *dest, const char *source, size_t len) {
	if (!dest || !source) {
		return dest;
	}
	while (*source && len) {
		*dest=wcharacter::duplicate(*source);
		dest++;
		source++;
		len--;
	}
	if (len) {
		*dest=L'\0';
	}
	return dest;
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
						const wchar_t *source) {
	return copy(dest+location,source);
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
						const char *source) {
	return copy(dest+location,source);
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
					const wchar_t *source, size_t len) {
	return copy(dest+location,source,len);
}

wchar_t *wcharstring::copy(wchar_t *dest, size_t location,
					const char *source, size_t len) {
	return copy(dest+location,source,len);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destlen,
						const wchar_t *source) {
	return safeCopy(dest,destlen,source,length(source)+1);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destlen,
						const char *source) {
	return safeCopy(dest,destlen,source,charstring::length(source)+1);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destlen,
				const wchar_t *source, size_t sourcelen) {
	return copy(dest,source,(sourcelen>destlen)?destlen:sourcelen);
}

wchar_t *wcharstring::safeCopy(wchar_t *dest, size_t destlen,
				const char *source, size_t sourcelen) {
	return copy(dest,source,(sourcelen>destlen)?destlen:sourcelen);
}

int32_t wcharstring::compare(const wchar_t *str1, const wchar_t *str2) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_WCSCMP
		return wcscmp(str1,str2);
	#else
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
	#endif
}

int32_t wcharstring::compare(const wchar_t *str1, const wchar_t *str2,
							size_t len) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_WCSNCMP
		return wcsncmp(str1,str2,len);
	#else
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
	#endif
}

int32_t wcharstring::compareIgnoringCase(const wchar_t *str1,
						const wchar_t *str2) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_WCSCASECMP
		return wcscasecmp(str1,str2);
	#else
		int32_t	diff=0;
		while (*str1 && *str2) {
			diff=(wcharacter::toUpperCase(*str1)-
				wcharacter::toUpperCase(*str2));
			if (diff) {
				return diff;
			}
			str1++;
			str2++;
		}
		return wcharacter::toUpperCase(*str1)-
				wcharacter::toUpperCase(*str2);
	#endif
}

int32_t wcharstring::compareIgnoringCase(const wchar_t *str1,
						const wchar_t *str2,
						size_t len) {
	if (!str2) {
		return 1;
	}
	if (!str1) {
		return -1;
	}
	#ifdef RUDIMENTS_HAVE_WCSNCASECMP
		return wcsncasecmp(str1,str2,len);
	#else
		int32_t	diff=0;
		while (*str1 && *str2 && len) {
			diff=(wcharacter::toUpperCase(*str1)-
				wcharacter::toUpperCase(*str2));
			if (diff) {
				return diff;
			}
			str1++;
			str2++;
			len--;
		}
		return (len)?wcharacter::toUpperCase(*str1)-
				wcharacter::toUpperCase(*str2):0;
	#endif
}

int32_t wcharstring::compareNatural(const wchar_t *str1, const wchar_t *str2) {
	return compareNatural(str1,str2,L".");
}

int32_t wcharstring::compareNatural(const wchar_t *str1,
					const wchar_t *str2,
					const wchar_t *delimiters) {

	int64_t		difference=0;
	const wchar_t	*start1=NULL;
	const wchar_t	*start2=NULL;
	wchar_t		*num1=NULL;
	wchar_t		*num2=NULL;
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
		bool	isdigit1=wcharacter::isDigit(*str1);
		bool	isdigit2=wcharacter::isDigit(*str2);
		difference+=isdigit2-isdigit1;

		// if the difference is non-zero then return it
		if (difference) {
			return difference;
		}

		if (isdigit1 && isdigit2) {

			// move to after the number in both strings
			start1=str1;
			while (*str1 && (wcharacter::isDigit(*str1) ||
					wcharacter::inSet(*str1,delimiters))) {
				str1++;
			}
			start2=str2;
			while (*str2 && (wcharacter::isDigit(*str2) ||
					wcharacter::inSet(*str2,delimiters))) {
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

int32_t wcharstring::compareVersions(const wchar_t *str1, const wchar_t *str2) {
	return compareVersions(str1,str2,L".");
}

int32_t wcharstring::compareVersions(const wchar_t *str1,
					const wchar_t *str2,
					const wchar_t *delimiters) {

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

bool wcharstring::compareWithWildcards(const wchar_t *string,
					size_t stringlength,
					const wchar_t *pattern,
					size_t patternlength,
					wchar_t singlewildcard,
					wchar_t multiwildcard) {

	// handle degenerate case
	if (!string && !pattern) {
		return true;
	}

	const wchar_t	*stringend=string+stringlength;
	const wchar_t	*patternend=pattern+patternlength;

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

bool wcharstring::compareWithWildcards(const wchar_t *string,
					const wchar_t *pattern,
					size_t patternlength,
					wchar_t singlewildcard,
					wchar_t multiwildcard) {
	return compareWithWildcards(string,length(string),
					pattern,patternlength,
					singlewildcard,multiwildcard);
}

bool wcharstring::compareWithWildcards(const wchar_t *string,
					size_t stringlength,
					const wchar_t *pattern,
					wchar_t singlewildcard,
					wchar_t multiwildcard) {
	return compareWithWildcards(string,stringlength,
					pattern,length(pattern),
					singlewildcard,multiwildcard);
}

bool wcharstring::compareWithWildcards(const wchar_t *string,
					const wchar_t *pattern,
					wchar_t singlewildcard,
					wchar_t multiwildcard) {
	return compareWithWildcards(string,length(string),
					pattern,length(pattern),
					singlewildcard,multiwildcard);
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
	return !compare(haystack,needle,length(needle));
}

bool wcharstring::endsWith(const wchar_t *haystack, const wchar_t *needle) {
	size_t	needlelen=length(needle);
	size_t	haystacklen=length(haystack);
	return (haystacklen>=needlelen &&
			!compare(haystack+haystacklen-needlelen,needle));
}

const wchar_t *wcharstring::findFirst(const wchar_t *haystack,
					const wchar_t *needle) {
	#ifdef RUDIMENTS_HAVE_WCSSTR
		return (haystack && needle)?wcsstr(haystack,needle):NULL;
	#else
		size_t	haystacklen=length(haystack);
		size_t	needlelen=length(needle);
		for (const wchar_t *ptr=haystack;
				ptr<=haystack+haystacklen-needlelen;
				ptr++) {
			if (!compare(ptr,needle,needlelen)) {
				return ptr;
			}
		}
		return NULL;
	#endif
}

const wchar_t *wcharstring::findFirstIgnoringCase(const wchar_t *haystack,
							const wchar_t *needle) {
	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
	for (const wchar_t *ptr=haystack;
			ptr<=haystack+haystacklen-needlelen;
			ptr++) {
		if (!compareIgnoringCase(ptr,needle,needlelen)) {
			return ptr;
		}
	}
	return NULL;
}

const wchar_t *wcharstring::findFirst(const wchar_t *haystack,
							wchar_t needle) {
	#ifdef RUDIMENTS_HAVE_WCSCHR
		return (haystack)?wcschr(haystack,needle):NULL;
	#else
		size_t	haystacklen=length(haystack);
		for (const wchar_t *ptr=haystack;
				ptr<haystack+haystacklen;
				ptr++) {
			if (*ptr==needle) {
				return ptr;
			}
		}
		return NULL;
	#endif
}

const wchar_t *wcharstring::findFirstIgnoringCase(const wchar_t *haystack,
							wchar_t needle) {
	size_t	haystacklen=length(haystack);
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
			retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
		retval=haystack+length(haystack);
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
	#ifdef RUDIMENTS_HAVE_WCSRCHR
		return (haystack)?wcsrchr(haystack,needle):NULL;
	#else
		if (!haystack) {
			return NULL;
		}
		const wchar_t	*c=haystack;
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
	#endif
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
	wchar_t	*retval=haystack+length(haystack);
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

wchar_t *wcharstring::findLastOfSetOrEnd(wchar_t *haystack,
						const wchar_t *set) {
	if (!haystack || !set) {
		return NULL;
	}
	wchar_t	*retval=haystack+length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+length(haystack);
}

size_t wcharstring::lengthContainingSet(const wchar_t *haystack,
						const wchar_t *set) {
	if (!haystack || !set) {
		return 0;
	}
	#ifdef RUDIMENTS_HAVE_WCSSPN
		return wcsspn(haystack,set);
	#else
		size_t	index=0;
		while (contains(set,haystack[index])) {
			index++;
		}
		return index;
	#endif
}

size_t wcharstring::lengthNotContainingSet(const wchar_t *haystack,
						const wchar_t *set) {
	if (!haystack || !set) {
		return 0;
	}
	#ifdef RUDIMENTS_HAVE_WCSCSPN
		return wcscspn(haystack,set);
	#else
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
	const wchar_t	*e=NULL;
	if (isNullOrEmpty(end)) {
		for (e=s; *e; e++) {}
	} else {
		e=findFirst(s,end);
	}
	if (!e) {
		return NULL;
	}

	// copy out the string between s and e
	return duplicate(s,e-s);
}

wchar_t *wcharstring::after(const wchar_t *str, const wchar_t *delimiter) {
	return between(str,delimiter,NULL);
}

wchar_t *wcharstring::duplicate(const wchar_t *str) {
	return duplicate(str,length(str));
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

wchar_t	*wcharstring::duplicate(const char *string) {
	return duplicate(string,charstring::length(string));
}

wchar_t	*wcharstring::duplicate(const char *string, size_t length) {
	// FIXME: use iconvert directly
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


wchar_t *wcharstring::duplicate(const ucs2_t *string) {
	return duplicate(string,ucs2charstring::length(string),'?');
}

wchar_t *wcharstring::duplicate(const ucs2_t *string, size_t len) {
	return duplicate(string,len,'?');
}

wchar_t *wcharstring::duplicate(const ucs2_t *string, wchar_t replacement) {
	return duplicate(string,ucs2charstring::length(string),replacement);
}

wchar_t *wcharstring::duplicate(const ucs2_t *string, size_t len,
							wchar_t replacement) {
	// FIXME: use iconvert directly
	if (!string) {
		return NULL;
	}
	wchar_t	*retval=new wchar_t[len+1];
	for (size_t i=0; i<len; i++) {
		retval[i]=wcharacter::duplicate(string[i],replacement);
	}
	retval[len]='\0';
	return retval;
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
	if (!string) {
		return 0;
	}
	#if defined(RUDIMENTS_HAVE_WCSTOLL)
		return wcstoll(string,(wchar_t **)endptr,base);
	#elif defined(RUDIMENTS_HAVE_WCSTOL)
		return wcstol(string,(wchar_t **)endptr,base);
	#else
		char	*str=charstring::duplicate(string);
		int64_t	retval=charstring::toInteger(str,NULL,base);
		delete[] str;
		if (endptr) {
			while (wcharacter::isDigit(*string)) {
				string++;
			}
			*endptr=string;
		}
		return retval;
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
	if (!string) {
		return 0;
	}
	#if defined(RUDIMENTS_HAVE_WCSTOULL)
		return wcstoull(string,(wchar_t **)endptr,base);
	#elif defined(RUDIMENTS_HAVE_WCSTOUL)
		return wcstoul(string,(wchar_t **)endptr,base);
	#else
		char	*str=charstring::duplicate(string);
		int64_t	retval=charstring::toUnsignedInteger(str,NULL,base);
		delete[] str;
		if (endptr) {
			while (wcharacter::isDigit(*string)) {
				string++;
			}
			*endptr=string;
		}
		return retval;
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
	if (!string) {
		return 0.0;
	}
	#if defined(RUDIMENTS_HAVE_WCSTOLD)
		return wcstold(string,(wchar_t **)endptr);
	#elif defined(RUDIMENTS_HAVE_WCSTOD)
		return (long double)(wcstod(string,(wchar_t **)endptr));
	#else
		char		*str=charstring::duplicate(string);
		long double	retval=charstring::toFloat(str,NULL);
		delete[] str;
		if (endptr) {
			bool	found=false;
			for (;;) {
				if (wcharacter::isDigit(*string)) {
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
	#endif
}

void wcharstring::split(const wchar_t *string, const wchar_t *delimiter,
				bool collapse,
				wchar_t ***list, uint64_t *listlength) {
	split(string,length(string),
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string, size_t stringlength,
				const wchar_t *delimiter, bool collapse,
				wchar_t ***list, uint64_t *listlength) {
	split(string,stringlength,
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string, 
				const wchar_t *delimiter,
				size_t delimiterlength,
				bool collapse,
				wchar_t ***list,
				uint64_t *listlength) {
	split(string,length(string),
			delimiter,delimiterlength,
			collapse,list,listlength);
}

void wcharstring::split(const wchar_t *string,
				size_t stringlength,
				const wchar_t *delimiter,
				size_t delimiterlength,
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

wchar_t *wcharstring::humanReadable(int64_t number) {
	return humanReadable(number,false);
}

wchar_t *wcharstring::humanReadable(int64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

wchar_t *wcharstring::humanReadable(uint64_t number) {
	return humanReadable(number,false);
}

wchar_t *wcharstring::humanReadable(uint64_t number, bool onethousand) {
	return humanReadable((long double)number,onethousand);
}

wchar_t *wcharstring::humanReadable(long double number) {
	return humanReadable(number,false);
}

wchar_t *wcharstring::humanReadable(long double number, bool onethousand) {

	long double	k=(onethousand)?1000.0:1024.0;
	wchar_t		suffixes[]={
		L'\0',L'K',L'M',L'G',L'T',L'P',L'Z',L'Y',L'B'
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
	
	#ifdef RUDIMENTS_HAVE_VSWPRINTF
		wchar_t	*buf=NULL;
		printf(&buf,L"%0.1Lf%c",number/size,suffixes[i]);
	#else
		char	*tmp=NULL;
		charstring::printf(&tmp,"%0.1Lf%c",number/size,
					character::duplicate(suffixes[i]));
		wchar_t	*buf=duplicate(tmp);
		delete[] tmp;
	#endif

	wchar_t	*subbed=replace(buf,L".0",L"");
	delete[] buf;
	return subbed;
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
	#ifdef RUDIMENTS_HAVE_VSWPRINTF

		// On most platforms, vswprintf attempts to write formatted
		// data into "buffer" and returns the number of bytes that were
		// written (excluding the null terminator) if it's possible to
		// fit them into the buffer.
		//
		// If an error occurs, then vswprintf returns -1 and sets errno.
		//
		// Platforms differ on what happens if trucation occurs though.
		// Most platforms just return -1 and don't set errno.  Some
		// platforms (freebsd) return -1 and set errno to EOVERFLOW.
		// Some platforms (solaris 7) just return the number of bytes
		// that were able to be written.
		//
		// We don't want any of these behaviors though.  In the case
		// of truncation, we want to write truncated data to "buffer"
		// and return the number of bytes that would have been written
		// (excluding the NULL terminator) if truncation hadn't
		// occurred, similar to the behavior of vsnprintf on
		// well-behaved systems.
		//
		// So, we'll write to ever-larger buffers until we achieve
		// success, copy out what we can to the original buffer, and
		// return the number of bytes that would have been written
		// (excluding the NULL terminator) if truncation hadn't
		// occurred.

		size_t	buflen=length;
		size_t	inc=16;
		ssize_t	size=-1;
		error::clearError();
		do {

			wchar_t	*buf=new wchar_t[buflen];

			size=vswprintf(buf,buflen,format,*argp);

			// On most platforms, size-written equaling -1
			// indicates truncation.
			// On solaris-7-style platforms, size-written exactly
			// equaling the size of the buffer size indicates
			// truncation.
			// If neither of those happened, then we have success.
			if (size>-1 && (size_t)size!=buflen) {
				if ((size_t)(size+1)>length) {
					// just copy out what we can
					copy(buffer,buf,length);
				} else {
					// copy out everything, including
					// the NULL terminator
					copy(buffer,buf,size+1);
				}
				delete[] buf;
				break;
			}
			#if defined(EINVAL) || \
				defined(EOVERFLOW) || \
				defined(EILSEQ)
			// On freebsd-style platforms EINVAL is set if
			// buflen==0 and EOVERFLOW or EILSEQ can be set when
			// truncation occurs.  Ignore all of these.
			else if (size==-1 &&
				(error::getErrorNumber()==EINVAL ||
				error::getErrorNumber()==EOVERFLOW ||
				error::getErrorNumber()==EILSEQ)) {
				error::clearError();
			}
			#endif

			delete[] buf;
			buflen+=inc;

			// adjust how quickly the buffer grows
			// (this can certainly be optimized further)
			inc*=2;
			if (inc>1024) {
				inc=1024;
			}
		} while (!error::getErrorNumber());
		return size;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
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
	ssize_t	size=printf(*buffer,0,format,argp);
	va_end(*argp);
	if (size!=-1) {
		*buffer=new wchar_t[size+1];
		size=printf(*buffer,size+1,format,&argp1);
	}
	va_end(argp1);
	return size;
}

bool wcharstring::supportsPrintf() {
	#ifdef RUDIMENTS_HAVE_VSWPRINTF
		return true;
	#else
		return false;
	#endif
}
