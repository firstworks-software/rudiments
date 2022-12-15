// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/ucs2character.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>

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

bool ucs2character::inSet(char16_t c, const char16_t *set) {
	for (const char16_t *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

char16_t ucs2character::duplicate(char c) {
	// FIXME: implement this
	return 0;
}

char16_t ucs2character::duplicate(wchar_t c) {
	return duplicate(c,'?');
}

char16_t ucs2character::duplicate(wchar_t c, char16_t replacement) {
	// FIXME: implement this
	return 0;
}

bool ucs2character::duplicateFromWideCharacterNeedsMutex() {
	// FIXME: implement this
	return false;
}
