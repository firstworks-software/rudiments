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
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#include <stdio.h>

bool wcharacter::isAlphanumeric(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswalnum(c)!=0;
	#else
		return false;
	#endif
}
 
bool wcharacter::isAlphabetical(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswalpha(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isAlphabeticalExtended(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		// FIXME...
		return isAlphabetical(c);
	#else
		return false;
	#endif
}

bool wcharacter::isControlCharacter(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswcntrl(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isDigit(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswdigit(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isLowerCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswlower(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isLowerCaseExtended(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		// FIXME...
		return isLowerCase(c);
	#else
		return false;
	#endif
}

bool wcharacter::isPrintableNonSpace(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswgraph(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isPrintable(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswprint(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isPunctuation(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswpunct(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isUpperCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswupper(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isUpperCaseExtended(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		// FIXME...
		return isUpperCase(c);
	#else
		return false;
	#endif
}

bool wcharacter::isHexDigit(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswxdigit(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isBlank(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		#ifdef RUDIMENTS_HAVE_ISWBLANK
			return iswblank(c)!=0;
		#else
			return character::isBlank(c);
		#endif
	#else
		return false;
	#endif
}

bool wcharacter::isWhitespace(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return iswspace(c)!=0;
	#else
		return false;
	#endif
}

bool wcharacter::isAscii(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		#ifdef RUDIMENTS_HAVE_ISWASCII
			return iswascii(c)!=0;
		#else
			return character::isAscii(c);
		#endif
	#else
		return false;
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
		return 0;
	#endif
}

int32_t wcharacter::toLowerCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towlower(c);
	#else
		return 0;
	#endif
}

int32_t wcharacter::toAscii(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return character::toAscii(c);
	#else
		return 0;
	#endif
}

bool wcharacter::inSet(wchar_t c, const wchar_t *set) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		for (uint16_t i=0; set[i]; i++) {
			if (set[i]==c) {
				return true;
			}
		}
		return false;
	#else
		return false;
	#endif
}

wchar_t wcharacter::duplicate(char c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		wchar_t	retval;
		size_t	s;
		// To convert a char to a wchar_t, we can pretend that
		// "c" is a string of multi-byte characters, and use
		// mbrtowc/mbtowc to convert it.  If we pass a 1 to these
		// methods, then it will (safely) only read 1 byte from "c".
		#if defined(RUDIMENTS_HAVE_MBRTOWC)
			mbstate_t	st;
			bytestring::zero(&st,sizeof(st));
			s=mbrtowc(&retval,&c,1,&st);
		#elif defined(RUDIMENTS_HAVE_MBTOWC)
			wchar_t		retval;
			s=mbtowc(&retval,&c,1);
		#else
			#error no mbrtowc or anything like it
		#endif
		if (s==(size_t)-1 || s==(size_t)-2) {
			return (wchar_t)0;
		}
		return retval;
	#else
		return (wchar_t)0;
	#endif
}

bool wcharacter::duplicateFromCharacterNeedsMutex() {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		#if defined(RUDIMENTS_HAVE_WCRTOMB)
			return false;
		#elif defined(RUDIMENTS_HAVE_WCTOMB)
			return true;
		#else
			#error no wcrtomb or anything like it
		#endif
		return false;
	#endif
}

bool wcharacter::supported() {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return true;
	#else
		return false;
	#endif
}
