// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wcharacter.h>

#ifdef RUDIMENTS_HAVE_WCTYPE_H
	#include <wctype.h>
#endif
#ifdef RUDIMENTS_HAVE_WCHAR_H
	#include <wchar.h>
#endif

#include <stdio.h>

bool wcharacter::isAlphanumeric(int32_t c) {
	return iswalnum(c)!=0;
}
 
bool wcharacter::isAlphabetical(int32_t c) {
	return iswalpha(c)!=0;
}

bool wcharacter::isAlphabeticalExtended(int32_t c) {
	// FIXME...
	return isAlphabetical(c);
}

bool wcharacter::isControlCharacter(int32_t c) {
	return iswcntrl(c)!=0;
}

bool wcharacter::isDigit(int32_t c) {
	return iswdigit(c)!=0;
}

bool wcharacter::isLowerCase(int32_t c) {
	return iswlower(c)!=0;
}

bool wcharacter::isLowerCaseExtended(int32_t c) {
	// FIXME...
	return isLowerCase(c);
}

bool wcharacter::isPrintableNonSpace(int32_t c) {
	return iswgraph(c)!=0;
}

bool wcharacter::isPrintable(int32_t c) {
	return iswprint(c)!=0;
}

bool wcharacter::isPunctuation(int32_t c) {
	return iswpunct(c)!=0;
}

bool wcharacter::isUpperCase(int32_t c) {
	return iswupper(c)!=0;
}

bool wcharacter::isUpperCaseExtended(int32_t c) {
	// FIXME...
	return isUpperCase(c);
}

bool wcharacter::isHexDigit(int32_t c) {
	return iswxdigit(c)!=0;
}

bool wcharacter::isBlank(int32_t c) {
	// don't be tempted to use isblank() here, it's too challenging to get
	// it to work on every platform without a bunch of confusing #defines
	// before including ctype.h
	// FIXME...
	return (c==' ' || c=='	');
}

bool wcharacter::isWhitespace(int32_t c) {
	return iswspace(c)!=0;
}

bool wcharacter::isAscii(int32_t c) {
	// FIXME...
	//return isascii(c)!=0;
	return false;
}

int32_t wcharacter::toUpperCase(int32_t c) {
	return towupper(c);
}

int32_t wcharacter::toLowerCase(int32_t c) {
	return towlower(c);
}

int32_t wcharacter::toAscii(int32_t c) {
	// FIXME...
	//return toascii(c);
	return false;
}

bool wcharacter::inSet(wchar_t c, const wchar_t *set) {
	for (uint16_t i=0; set[i]; i++) {
		if (set[i]==c) {
			return true;
		}
	}
	return false;
}

wchar_t wcharacter::duplicate(char c) {
	wchar_t		retval;
	mbstate_t	st;
	size_t	s=mbrtowc(&retval,&c,1,&st);
	if (s==(size_t)-1 || s==(size_t)-2) {
		return (wchar_t)0;
	}
	return retval;
}
