// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#ifndef _WIN32
	#include <rudiments/iconvert.h>
#endif

#ifdef RUDIMENTS_HAVE_CTYPE_H
	#include <ctype.h>
#endif
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

bool character::isAlphanumeric(int32_t c) {
	return isalnum(c)!=0;
}
 
bool character::isAlphabetical(int32_t c) {
	return isalpha(c)!=0;
}

bool character::isAlphabeticalExtended(int32_t c) {
	// see http://www.theasciicode.com.ar/
	return isAlphabetical(c) ||
		(c>=128 && c<=154) ||
		(c>=160 && c<=165) ||
		(c>=181 && c<=183) ||
		(c>=198 && c<=199) ||
		(c>=208 && c<=216) ||
		(c>=224 && c<=237);
}

bool character::isControlCharacter(int32_t c) {
	return iscntrl(c)!=0;
}

bool character::isDigit(int32_t c) {
	return isdigit(c)!=0;
}

bool character::isLowerCase(int32_t c) {
	return islower(c)!=0;
}

bool character::isLowerCaseExtended(int32_t c) {
	// see http://www.theasciicode.com.ar/
	return isLowerCase(c) ||
		(c>=129 && c<=141) ||
		(c==145) ||
		(c>=147 && c<=152) ||
		(c>=160 && c<=164) ||
		(c==198) ||
		(c==208) ||
		(c==213) ||
		(c==228) ||
		(c>=230 && c<=231) ||
		(c==236);
}

bool character::isPrintableNonSpace(int32_t c) {
	return isgraph(c)!=0;
}

bool character::isPrintable(int32_t c) {
	return isprint(c)!=0;
}

bool character::isPunctuation(int32_t c) {
	return ispunct(c)!=0;
}

bool character::isUpperCase(int32_t c) {
	return isupper(c)!=0;
}

bool character::isUpperCaseExtended(int32_t c) {
	// see http://www.theasciicode.com.ar/
	return isUpperCase(c) ||
		(c==128) ||
		(c>=142 && c<=144) ||
		(c==146) ||
		(c>=153 && c<=154) ||
		(c==165) ||
		(c>=181 && c<=183) ||
		(c==199) ||
		(c>=209 && c<=212) ||
		(c>=214 && c<=216) ||
		(c>=224 && c<=227) ||
		(c>=232 && c<=235) ||
		(c==237);
}

bool character::isHexDigit(int32_t c) {
	return isxdigit(c)!=0;
}

bool character::isBlank(int32_t c) {
	// don't be tempted to use isblank() here, it's too challenging to get
	// it to work on every platform without a bunch of confusing #defines
	// before including ctype.h
	return (c==' ' || c=='	');
}

bool character::isWhitespace(int32_t c) {
	return isspace(c)!=0;
}

bool character::isAscii(int32_t c) {
	return isascii(c)!=0;
}

int32_t character::toUpperCase(int32_t c) {
	return toupper(c);
}

int32_t character::toLowerCase(int32_t c) {
	return tolower(c);
}

int32_t character::toAscii(int32_t c) {
	return toascii(c);
}

bool character::inSet(char c, const char *set) {
	for (const char *s=set; *s; s++) {
		if (*s==c) {
			return true;
		}
	}
	return false;
}

char character::duplicate(wchar_t c) {
	return duplicate(c,'?');
}

char character::duplicate(wchar_t c, char replacement) {

	char		*mb= new char[iconvert::maxMultiByteSize()];
	iconvert	i;
	i.setFromEncoding("WCHAR_T");
	i.setFromBuffer((byte_t *)&c);
	i.setFromBufferSize(sizeof(wchar_t));
	i.setToBuffer((byte_t *)mb);
	i.setToBufferSize(iconvert::maxMultiByteSize());
	if (!i.convert()) {
		delete[] mb;
		return replacement;
	}
	// We're attempting to convert a wide character to a regular
	// character, but it's possible that the wide character was
	// only representable by a multi-byte character in the current
	// locale.  If that ends up being the case, then just return
	// the replacement character.
	if (i.getToBufferPosition()-i.getToBuffer()>1) {
		delete[] mb;
		return replacement;
	}
	// otherwise, return the character
	char	retval=mb[0];
	delete[] mb;
	return retval;
}

char character::duplicate(ucs2_t c) {
	return duplicate(c,'?');
}

char character::duplicate(ucs2_t c, char replacement) {
	#ifdef _WIN32
		// on windows, wchar_t's are encoded as UCS-2,
		// so we can piggyback
		return character::duplicate((wchar_t)c,replacement);
	#else
		// on non-windows, use iconvert
		char		*mb= new char[iconvert::maxMultiByteSize()];
		iconvert	i;
		i.setFromEncoding("UCS-2");
		i.setFromBuffer((byte_t *)&c);
		i.setFromBufferSize(sizeof(ucs2_t));
		i.setToBuffer((byte_t *)mb);
		i.setToBufferSize(iconvert::maxMultiByteSize());
		if (!i.convert()) {
			delete[] mb;
			return replacement;
		}
		// We're attempting to convert a UCS-2 character to a regular
		// character, but it's possible that the UCS-2 character was
		// only representable by a multi-byte character in the current
		// locale.  If that ends up being the case, then just return
		// the replacement character.
		if (i.getToBufferPosition()-i.getToBuffer()>1) {
			delete[] mb;
			return replacement;
		}
		// otherwise, return the character
		char	retval=mb[0];
		delete[] mb;
		return retval;
	#endif
}
