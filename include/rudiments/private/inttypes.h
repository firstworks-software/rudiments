// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTTYPES_H
#define RUDIMENTS_INTTYPES_H

#include <rudiments/private/config.h>

// HP-UX has a function named memorymap() that prints out the contents of the
// memory allocator for 32-bit HP-UX systems.  It's depreceated in 11.11 and
// obsolete in 11.21+.  However, its existence confuses code that uses the
// memorymap class.  It's defined in malloc.h, and one of the includes below
// ends up pulling it in.  Since this header gets included by any code that
// uses rudiments, we'll use the trick below to effectively rename memorymap()
// to _memorymap(), then undef it after the includes.  Hopefully no code
// that uses rudiments actually tries to call memorymap().
#define memorymap _memorymap

// define NULL...

// NULL is typically defined in stddef.h
#include <stddef.h>

// Certain versions of gcc define NULL as ((void *)0) and then complain when
// you set a const pointer to it.  Work around that.
#ifdef RUDIMENTS_REDEFINE_NULL
#undef NULL
#define NULL 0
#endif

// some platforms define types like char16_t in their new or new.h
// (some firstworks C code uses inttypes.h to make sure types are defined
// though, and we don't want that code to include new.h)
#ifdef __cplusplus
	#include <rudiments/private/new.h>
#endif

#if defined(RUDIMENTS_HAVE_STDINT_H)
	#include <stdint.h>
#elif defined(RUDIMENTS_HAVE_SYS_BITYPES_H)
	// Tru64 needs __arch64__ for int64_t and uint64_t typedefs
	#ifndef __arch64__
		#define __arch64__
	#endif
	#include <sys/bitypes.h>
#elif defined(RUDIMENTS_HAVE_INTTYPES_H)
	#if defined(RUDIMENTS_HAVE_VARARGS_H) && \
		!defined(RUDIMENTS_HAVE_STDARG_H)
		#include <varargs.h>
	#endif
	#include <inttypes.h>
#endif

#if defined(RUDIMENTS_HAVE_UCHAR_H)
	#include <uchar.h>
#endif

// see HP-UX note above for why we're doing this
#undef memorymap

// define bool and true/false
#ifndef RUDIMENTS_HAVE_BOOL
	class bool {
		public:
			bool(const bool &b) {
				value=b.value;
			}
			bool(const long long &b) {
				value=b;
			}
			bool(const long &b) {
				value=b;
			}
			bool(const int &b) {
				value=b;
			}
			bool(const short &b) {
				value=b;
			}
			bool(const char &b) {
				value=b;
			}
			bool(const unsigned long long &b) {
				value=b;
			}
			bool(const unsigned long &b) {
				value=b;
			}
			bool(const unsigned int &b) {
				value=b;
			}
			bool(const unsigned short &b) {
				value=b;
			}
			bool(const unsigned char &b) {
				value=b;
			}
			bool &operator=(const bool &b) {
				value=b.value;
				return *this;
			}
			bool &operator=(const long long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const int &b) {
				value=b;
				return *this;
			}
			bool &operator=(const short &b) {
				value=b;
				return *this;
			}
			bool &operator=(const char &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned long long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned int &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned short &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned char &b) {
				value=b;
				return *this;
			}
			operator long() {
				return value;
			}
			int operator!() {
				return !value;
			}
			int operator==(const bool &b) {
				return value==b.value;
			}
			int operator!=(const bool &b) {
				return value!=b.value;
			}
		private:
			long	value;
	};
#endif
#ifndef RUDIMENTS_HAVE_TRUE_FALSE
	#define true 1
	#define false 0
#endif

// define [u]int(8|16|32|64)_t...
#ifndef RUDIMENTS_HAVE_INT8_T
	typedef	signed char		int8_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT8_T
	typedef	unsigned char		uint8_t;
#endif
#ifndef RUDIMENTS_HAVE_INT16_T
	typedef	signed short		int16_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT16_T
	typedef	unsigned short		uint16_t;
#endif
#ifndef RUDIMENTS_HAVE_INT32_T
	typedef	signed int		int32_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT32_T
	typedef	unsigned int		uint32_t;
	// older versions of solaris require this to prevent a pthreads conflict
	#define	_UINT32_T 1
#endif
#ifndef RUDIMENTS_HAVE_INT64_T
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		typedef	signed long long	int64_t;
	#else
		typedef	signed long		int64_t;
	#endif
#endif
#ifndef RUDIMENTS_HAVE_UINT64_T
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		typedef	unsigned long long	uint64_t;
	#else
		typedef	unsigned long		uint64_t;
	#endif
#endif

// define byte_t
#ifndef RUDIMENTS_HAVE_BYTE_T
	typedef uint8_t	byte_t;
#endif

// define char(8|32)_t
#ifndef RUDIMENTS_HAVE_CHAR8_T
	typedef	uint8_t		char8_t;
#endif
#ifndef RUDIMENTS_HAVE_CHAR32_T
	typedef	uint32_t	char32_t;
#endif

// define ucs2_t and utf(8|16)_t
typedef uint16_t ucs2_t;
typedef uint8_t utf8_t;
typedef uint16_t utf16_t;

#endif
