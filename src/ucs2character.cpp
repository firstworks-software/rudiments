// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/ucs2character.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#include <rudiments/iconvert.h>

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

int32_t ucs2character::toUpperCase(int32_t c) {
	return character::toUpperCase(c);
}

int32_t ucs2character::toLowerCase(int32_t c) {
	return character::toLowerCase(c);
}

int32_t ucs2character::toAscii(int32_t c) {
	return character::toAscii(c);
}

bool ucs2character::inSet(ucs2_t c, const ucs2_t *set) {
	for (const ucs2_t *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

ucs2_t ucs2character::duplicate(char c) {
	return duplicate(c,'?');
}

ucs2_t ucs2character::duplicate(char c, ucs2_t replacement) {
	ucs2_t		uc;
	iconvert	i;
	i.setFromBuffer((unsigned char *)&c);
	i.setFromBufferSize(sizeof(c));
	i.setToEncoding("UCS-2");
	i.setToBuffer((unsigned char *)&uc);
	i.setToBufferSize(sizeof(uc));
	return (i.convert())?uc:replacement;
}

ucs2_t ucs2character::duplicate(wchar_t c) {
	return duplicate(c,'?');
}

ucs2_t ucs2character::duplicate(wchar_t c, ucs2_t replacement) {
	#ifdef _WIN32
		// on windows, wchar_t's are encoded as UCS-2
		return (ucs2_t)c;
	#else
		// on non-windows, use iconvert
		ucs2_t		uc;
		iconvert	i;
		i.setFromEncoding("WCHAR_T");
		i.setFromBuffer((unsigned char *)&c);
		i.setFromBufferSize(sizeof(c));
		i.setToEncoding("UCS-2");
		i.setToBuffer((unsigned char *)&uc);
		i.setToBufferSize(sizeof(uc));
		return (i.convert())?uc:replacement;
	#endif
}
