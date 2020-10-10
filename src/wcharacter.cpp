// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wcharacter.h>
#include <rudiments/character.h>

#ifdef RUDIMENTS_HAVE_WCTYPE_H
	#include <wctype.h>
#endif
#ifdef RUDIMENTS_HAVE_WCHAR_H
	#include <wchar.h>
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
		return iswblank(c)!=0;
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

int32_t wcharacter::toUpperCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towupper(c);
	#else
		return false;
	#endif
}

int32_t wcharacter::toLowerCase(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return towlower(c);
	#else
		return false;
	#endif
}

int32_t wcharacter::toAscii(int32_t c) {
	#ifdef RUDIMENTS_HAVE_WCTYPE_H
		return character::toAscii(c);
	#else
		return false;
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
		wchar_t		retval;
		mbstate_t	st;
		size_t	s=mbrtowc(&retval,&c,1,&st);
		if (s==(size_t)-1 || s==(size_t)-2) {
			return (wchar_t)0;
		}
		return retval;
	#else
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
