// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wcharacter.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>

#ifdef RUDIMENTS_HAVE_WCTYPE_H
	#include <wctype.h>
#endif
#ifdef RUDIMENTS_HAVE_WCHAR_H
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

int32_t wcharacter::toUpperCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towupper(c);
	#else
		return character::toUpperCase(
				character::duplicate((wchar_t)c));
	#endif
}

int32_t wcharacter::toLowerCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towlower(c);
	#else
		return character::toLowerCase(
				character::duplicate((wchar_t)c));
	#endif
}

int32_t wcharacter::toAscii(int32_t c) {
	return character::toAscii(character::duplicate((wchar_t)c));
}

bool wcharacter::inSet(wchar_t c, const wchar_t *set) {
	for (const wchar_t *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

wchar_t wcharacter::duplicate(char c) {
	wchar_t	retval;
	size_t	s;
	// To convert a char to a wchar_t, we can pretend that "c" is a string
	// of multi-byte characters, and use mbrtowc/mbtowc to convert it.  If
	// we pass a 1 to these methods, then it will (safely) only read 1 byte
	// from "c".
	#if defined(RUDIMENTS_HAVE_MBRTOWC)
		mbstate_t	st;
		bytestring::zero(&st,sizeof(st));
		s=mbrtowc(&retval,&c,1,&st);
	#elif defined(RUDIMENTS_HAVE_MBTOWC)
		// mbtowc() doesn't like being passed '\0' on some platforms
		// (eg. redhat 4.2 with libc5) 
		if (c) {
			s=mbtowc(&retval,&c,1);
		} else {
			s=0;
			retval=(wchar_t)c;
		}
	#else
		// This will only work if the first 256 characters of the
		// source and target character set are equivalent.
		// eg. Latin 1 and UCS-2/UCS-4.  This is usually the case
		// on older paltforms that don't provide mbtowc()/mbrtowc(),
		// but I bet I'll be back here tweaking this some day.
		s=0;
		retval=(wchar_t)c;
	#endif
	if (s==(size_t)-1 || s==(size_t)-2) {
		return (wchar_t)0;
	}
	return retval;
}

wchar_t wcharacter::duplicate(ucs2_t c) {
	return duplicate(c,'?');
}

wchar_t wcharacter::duplicate(ucs2_t c, wchar_t replacement) {
	#ifdef _WIN32
		// on windows, wchar_t's are encoded as UCS-2
		return (wchar_t)ucs2_t;
	#else
		// on non-windows, it's trickier
		// FIXME: implement this...
		return 0;
	#endif
}

bool wcharacter::duplicateFromCharacterNeedsMutex() {
	#if defined(RUDIMENTS_HAVE_WCRTOMB)
		return false;
	#elif defined(RUDIMENTS_HAVE_MBTOWC)
		return true;
	#else
		return false;
	#endif
}
