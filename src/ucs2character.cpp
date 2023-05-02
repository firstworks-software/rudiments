// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/ucs2character.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#include <rudiments/iconvert.h>
#include <rudiments/sys.h>

bool ucs2character::isAlphanumeric(int32_t c) {
	return character::isAlphanumeric(c);
}
 
bool ucs2character::isAlphabetical(int32_t c) {
	return character::isAlphabetical(c);
}

bool ucs2character::isControlCharacter(int32_t c) {
	return character::isControlCharacter(c);
}

bool ucs2character::isDigit(int32_t c) {
	return character::isDigit(c);
}

bool ucs2character::isLowerCase(int32_t c) {
	return character::isLowerCase(c);
}

bool ucs2character::isPrintableNonSpace(int32_t c) {
	return character::isPrintableNonSpace(c);
}

bool ucs2character::isPrintable(int32_t c) {
	return character::isPrintable(c);
}

bool ucs2character::isPunctuation(int32_t c) {
	return character::isPunctuation(c);
}

bool ucs2character::isUpperCase(int32_t c) {
	return character::isUpperCase(c);
}

bool ucs2character::isHexDigit(int32_t c) {
	return character::isHexDigit(c);
}

bool ucs2character::isBlank(int32_t c) {
	return character::isBlank(c);
}

bool ucs2character::isWhitespace(int32_t c) {
	return character::isWhitespace(c);
}

bool ucs2character::isAscii(int32_t c) {
	return character::isAscii(c);
}

int32_t ucs2character::upper(int32_t c) {
	return character::upper(c);
}

int32_t ucs2character::lower(int32_t c) {
	return character::lower(c);
}

int32_t ucs2character::convertToAscii(int32_t c) {
	return character::convertToAscii(c);
}

bool ucs2character::isInSet(ucs2_t c, const ucs2_t *set) {
	for (const ucs2_t *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

bool ucs2character::isBom(ucs2_t c) {
	return c==0xFEFF || c==0xFFFE;
}

bool ucs2character::isBigEndianBom(ucs2_t c) {
	return sys::getIsBigEndian() && c==0xFEFF;
}

ucs2_t ucs2character::duplicate(char c) {
	return duplicate(c,(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t ucs2character::duplicate(char c, ucs2_t replacement) {
	return duplicate(c,replacement,sys::getIsBigEndian());
}

ucs2_t ucs2character::duplicate(char c, bool bigendian) {
	return duplicate(c,(ucs2_t)'?',bigendian);
}

ucs2_t ucs2character::duplicate(char c, ucs2_t replacement, bool bigendian) {
	ucs2_t		uc;
	iconvert	i;
	i.setFromBuffer((byte_t *)&c);
	i.setFromBufferSize(sizeof(c));
	i.setToEncoding((bigendian)?"UCS-2BE":"UCS-2LE");
	i.setToBuffer((byte_t *)&uc);
	i.setToBufferSize(sizeof(uc));
	return (i.convert())?uc:replacement;
}

ucs2_t ucs2character::duplicate(wchar_t c) {
	return duplicate(c,(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t ucs2character::duplicate(wchar_t c, ucs2_t replacement) {
	return duplicate(c,replacement,sys::getIsBigEndian());
}

ucs2_t ucs2character::duplicate(wchar_t c, bool bigendian) {
	return duplicate(c,(ucs2_t)'?',sys::getIsBigEndian());
}

ucs2_t ucs2character::duplicate(wchar_t c, ucs2_t replacement, bool bigendian) {
	ucs2_t		uc;
	iconvert	i;
	i.setFromEncoding("WCHAR_T");
	i.setFromBuffer((byte_t *)&c);
	i.setFromBufferSize(sizeof(c));
	i.setToEncoding((bigendian)?"UCS-2BE":"UCS-2LE");
	i.setToBuffer((byte_t *)&uc);
	i.setToBufferSize(sizeof(uc));
	return (i.convert())?uc:replacement;
}
