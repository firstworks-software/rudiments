// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/bytestring.h>
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_G_CONFIG_H
	#include <_G_config.h>
	// _G_config.h undefs NULL for some reason
	#ifndef NULL
		#define NULL	0
	#endif
#endif

#ifdef RUDIMENTS_MUST_DEFINE_SWAB
extern "C" void swab(const void *from, void *to, ssize_t n);
#endif

void *bytestring::duplicate(const void *src, size_t size) {
	if (!src || !size) {
		return NULL;
	}
	byte_t	*buffer=new byte_t[size];
	return copy((void *)buffer,src,size);
}

void *bytestring::copy(void *dest, const void *src, size_t size) {
	return (dest && src)?memcpy(dest,src,size):NULL;
}

void *bytestring::copyWithOverlap(void *dest, const void *src, size_t size) {
	return (dest && src)?memmove(dest,src,size):NULL;
}

void *bytestring::copyUntil(void *dest, const void *src,
					byte_t character, size_t size) {
	#if defined(RUDIMENTS_HAVE__MEMCCPY)
		return (dest && src)?_memccpy(dest,src,
						(int)character,size):NULL;
	#elif defined(RUDIMENTS_HAVE_MEMCCPY_CHAR)
		return (dest && src)?(void *)memccpy((char *)dest,
						(const char *)src,
						(int)character,size):NULL;
	#else
		return (dest && src)?memccpy(dest,src,
						(int)character,size):NULL;
	#endif
}

void *bytestring::copySwapBytes(void *dest, const void *src, size_t size) {
	if (dest && src) {
		#if defined(RUDIMENTS_HAVE__SWAB)
			_swab((char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CONST_CHAR)
			swab((const char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CHAR)
			swab((char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CONST_VOID)
			swab((const void *)src,(void *)dest,size);
		#else
			swab(src,dest,size);
		#endif
	}
	return dest;
}

void *bytestring::set(void *dest, byte_t character, size_t size) {
	return (dest)?memset(dest,(int)character,size):NULL;
}

void *bytestring::zero(void *dest, size_t size) {
	return set(dest,0,size);
}

int32_t bytestring::compare(const void *s1, const void *s2, size_t size) {
	if (!s1 && !s2) {
		return 0;
	}
	if (!s2) {
		return 1;
	}
	if (!s1) {
		return -1;
	}
	return memcmp(s1,s2,size);
}

const void *bytestring::findFirst(const void *haystack,
					byte_t needle, size_t size) {
	return (haystack)?memchr(haystack,needle,size):NULL;
}

const void *bytestring::findLast(const void *haystack,
					byte_t needle, size_t size) {
	#ifdef RUDIMENTS_HAVE_MEMRCHR
		return (haystack)?
			memrchr(haystack,needle,size):NULL;
	#else
		if (haystack) {
			byte_t	realneedle=needle;
			for (const byte_t *ptr=((const byte_t *)haystack)+size;
						ptr>=(const byte_t *)haystack;
						ptr--) {
				if (*ptr==realneedle) {
					return (const void *)ptr;
				}
			}
		}
		return NULL;
	#endif
}

const void *bytestring::findFirst(const void *haystack, size_t haystacksize,
					const void *needle, size_t needlesize) {

	// Linux libc has memmem() but the calling order is backwards, and
	// also it just doesn't work!  So prefer our implementation to it...

	#if defined(RUDIMENTS_HAVE_MEMMEM) && !defined(_LINUX_C_LIB_VERSION)
		return (haystack && needle)?
			memmem(haystack,haystacksize,needle,needlesize):NULL;
	#else
		if (haystack && needle) {

			const byte_t	*endptr=(const byte_t *)haystack+
							haystacksize-needlesize;

			for (const byte_t *ptr=(const byte_t *)haystack;
							ptr<endptr; ptr++) {

				if (!memcmp(ptr,needle,needlesize)) {
					return (const void *)ptr;
				}
			}
		}
		return NULL;
	#endif
}

const void *bytestring::findLast(const void *haystack, size_t haystacksize,
					const void *needle, size_t needlesize) {

	if (haystack && needle) {
		for (const byte_t *ptr=(const byte_t *)haystack+
						haystacksize-needlesize;
					ptr>=(const byte_t *)haystack;
					ptr--) {

			if (!memcmp(ptr,needle,needlesize)) {
				return (void *)ptr;
			}
		}
	}
	return NULL;
}

void *bytestring::findFirst(void *haystack, byte_t needle, size_t size) {
	return (void *)findFirst((const void *)haystack,needle,size);
}

void *bytestring::findLast(void *haystack, byte_t needle, size_t size) {
	return (void *)findLast((const void *)haystack,needle,size);
}

void *bytestring::findFirst(void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	return (void *)findFirst((const void *)haystack,
					haystacksize,needle,needlesize);
}

void *bytestring::findLast(void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	return (void *)findLast((const void *)haystack,
					haystacksize,needle,needlesize);
}
