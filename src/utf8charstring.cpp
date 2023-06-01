// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/utf8charstring.h>
#include <rudiments/utf8character.h>

bool utf8charstring::isByteOrderMark(const utf8_t *str) {
	const byte_t	*s=(const byte_t *)str;
	return s && *s && *(s+1) && *(s+2) &&
		*s==0xEF && *(s+1)==0xBB && *(s+2)==0xBF;
}

size_t utf8charstring::getLength(const utf8_t *string) {
	if (!string) {
		return 0;
	}
	size_t	len=0;
	const byte_t	*ptr=(const byte_t *)string;
	while (*ptr) {
		len++;
		if (*ptr<192) {
			ptr++;
		} else if (*ptr<224) {
			ptr+=2;
		} else if (*ptr<240) {
			ptr+=3;
		} else {
			ptr+=4;
		}
	}
	return len;
}

size_t utf8charstring::getSize(const utf8_t *string) {
	if (!string) {
		return 0;
	}
	size_t	sz=0;
	const byte_t	*ptr=(const byte_t *)string;
	while (*ptr) {
		if (*ptr<192) {
			sz++;
			ptr++;
		} else if (*ptr<224) {
			sz+=2;
			ptr+=2;
		} else if (*ptr<240) {
			sz+=3;
			ptr+=3;
		} else {
			sz+=4;
			ptr+=4;
		}
	}
	return sz+utf8character::getNullSize();
}
