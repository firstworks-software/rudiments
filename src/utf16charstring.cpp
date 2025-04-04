// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/utf16charstring.h>
#include <rudiments/utf16character.h>

bool utf16charstring::isByteOrderMark(const utf16_t *str) {
	const byte_t	*s=(const byte_t *)str;
	return s && *s && *(s+1) &&
		((*s==0xFE && *(s+1)==0xFF) ||
		(*s==0xFF && *(s+1)==0xFE));
}

bool utf16charstring::isBigEndian(const utf16_t *str) {
	const byte_t	*s=(const byte_t *)str;
	return s && *s && *(s+1) && *s==0xFE && *(s+1)==0xFF;
}

size_t utf16charstring::getLength(const utf16_t *string, bool bigendian) {
	if (!string) {
		return 0;
	}
	size_t	offset=(bigendian)?1:0;
	size_t	len=0;
	const byte_t	*ptr=(const byte_t *)string;
	while (*ptr || *(ptr+1)) {
		len++;
		if (*(ptr+offset)>=0xD8 && *(ptr+offset)<=0xDF) {
			ptr+=4;
		} else {
			ptr+=2;
		}
	}
	return len;
}

size_t utf16charstring::getSize(const utf16_t *string, bool bigendian) {
	if (!string) {
		return 0;
	}
	size_t	offset=(bigendian)?1:0;
	size_t	sz=0;
	const byte_t	*ptr=(const byte_t *)string;
	while (*ptr || *(ptr+1)) {
		if (*(ptr+offset)>=0xD8 && *(ptr+offset)<=0xDF) {
			sz+=4;
			ptr+=4;
		} else {
			sz+=2;
			ptr+=2;
		}
	}
	return sz+utf16character::getNullSize();
}
