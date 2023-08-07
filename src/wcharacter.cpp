// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wcharacter.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#include <rudiments/iconvert.h>
#include <rudiments/sys.h>

#ifdef RUDIMENTS_HAVE_WCTYPE_H
	#ifdef RUDIMENTS_HAVE_VARCHAR_H
		#include <varchar.h>
	#endif
	#include <wctype.h>
#endif
#ifdef RUDIMENTS_HAVE_WCHAR_H
	#ifdef RUDIMENTS_HAVE_VARCHAR_H
		#include <varchar.h>
	#endif
	#include <wchar.h>
#endif
#ifdef RUDIMENTS_HAVE_WCSTR_H
	#include <wcstr.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#include <stdio.h>

bool wcharacter::isAlphanumeric(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswalnum(c)!=0;
	#else
		return character::isAlphanumeric(
				character::duplicate((wchar_t)c));
	#endif
}
 
bool wcharacter::isAlphabetical(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswalpha(c)!=0;
	#else
		return character::isAlphabetical(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isControlCharacter(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswcntrl(c)!=0;
	#else
		return character::isControlCharacter(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isDigit(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswdigit(c)!=0;
	#else
		return character::isDigit(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isLowerCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswlower(c)!=0;
	#else
		return character::isLowerCase(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isPrintableNonSpace(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswgraph(c)!=0;
	#else
		return character::isPrintableNonSpace(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isPrintable(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswprint(c)!=0;
	#else
		return character::isPrintable(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isPunctuation(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswpunct(c)!=0;
	#else
		return character::isPunctuation(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isUpperCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswupper(c)!=0;
	#else
		return character::isUpperCase(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isHexDigit(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswxdigit(c)!=0;
	#else
		return character::isHexDigit(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isBlank(int32_t c) {
	#if defined(RUDIMENTS_HAVE_WCTYPE_H) && \
			defined(RUDIMENTS_HAVE_ISWBLANK)
		return iswblank(c)!=0;
	#else
		return character::isBlank(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isWhitespace(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswspace(c)!=0;
	#else
		return character::isWhitespace(
				character::duplicate((wchar_t)c));
	#endif
}

bool wcharacter::isAscii(int32_t c) {
	#if defined(RUDIMENTS_HAVE_WCTYPE_H) && \
			defined(RUDIMENTS_HAVE_ISWASCII)
		return iswascii(c)!=0;
	#else
		return character::isAscii(
				character::duplicate((wchar_t)c));
	#endif
}

#if defined(RUDIMENTS_HAVE_WCTYPE_H) && \
	defined(RUDIMENTS_HAVE_BROKEN_TOWCTRANS)

static wint_t localtowctrans(wint_t wc, const int *desc) {
	return towctrans(wc,(wctrans_t)desc);
}

#undef towupper
#define towupper(wc) localtowctrans(wc,__ctype_toupper)
#undef towlower
#define towlower(wc) localtowctrans(wc,__ctype_tolower)

#endif

int32_t wcharacter::upper(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towupper(c);
	#else
		return character::upper(character::duplicate((wchar_t)c));
	#endif
}

int32_t wcharacter::lower(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towlower(c);
	#else
		return character::lower(character::duplicate((wchar_t)c));
	#endif
}

int32_t wcharacter::convertToAscii(int32_t c) {
	return character::convertToAscii(character::duplicate((wchar_t)c));
}

bool wcharacter::isInSet(wchar_t c, const wchar_t *set) {
	for (const wchar_t *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

wchar_t wcharacter::duplicate(char c) {
	return duplicate(c,L'?');
}

wchar_t wcharacter::duplicate(char c, wchar_t replacement) {
	wchar_t		wc;
	iconvert	i;
	i.setFromBuffer((byte_t *)&c);
	i.setFromBufferSize(sizeof(char));
	i.setToEncoding("WCHAR_T");
	i.setToBuffer((byte_t *)&wc);
	i.setToBufferSize(sizeof(wchar_t));
	return (i.convert())?wc:replacement;
}

wchar_t wcharacter::duplicateUcs2(ucs2_t c) {
	return duplicateUcs2(c,L'?',sys::isBigEndian());
}

wchar_t wcharacter::duplicateUcs2(ucs2_t c, wchar_t replacement) {
	return duplicateUcs2(c,replacement,sys::isBigEndian());
}

wchar_t wcharacter::duplicateUcs2(ucs2_t c, bool bigendian) {
	return duplicateUcs2(c,L'?',bigendian);
}

wchar_t wcharacter::duplicateUcs2(ucs2_t c, wchar_t replacement,
							bool bigendian) {
	wchar_t		wc;
	iconvert	i;
	i.setFromEncoding((bigendian)?"UCS-2BE":"UCS-2LE");
	i.setFromBuffer((byte_t *)&c);
	i.setFromBufferSize(sizeof(ucs2_t));
	i.setToEncoding("WCHAR_T");
	i.setToBuffer((byte_t *)&wc);
	i.setToBufferSize(sizeof(wchar_t));
	return (i.convert())?wc:replacement;
}

uint8_t wcharacter::getNullSize() {
	return sizeof(wchar_t);
}
