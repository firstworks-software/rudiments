// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTTYPES_H
#define RUDIMENTS_INTTYPES_H

#include <rudiments/private/config.h>

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
	#include <inttypes.h>
#endif

#if defined(RUDIMENTS_HAVE_UCHAR_H)
	#include <uchar.h>
#endif

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

// define char(8|16|32)_t
#ifndef RUDIMENTS_HAVE_CHAR8_T
	typedef	uint8_t		char8_t;
#endif
#if !defined(RUDIMENTS_HAVE_CHAR16_T) && \
	defined(__cplusplus) && (__cplusplus<201103L)
	typedef	uint16_t	char16_t;
#endif
#ifndef RUDIMENTS_HAVE_CHAR32_T
	typedef	uint32_t	char32_t;
#endif

// define ucs2_t
#if 0
class ucs2_t {
	public:
		ucs2_t() {
		}
		ucs2_t(const ucs2_t &v) {
			value=v.value;
		}
		ucs2_t(const bool &v) {
			value=v;
		}
		ucs2_t(const long long &v) {
			value=v;
		}
		ucs2_t(const long &v) {
			value=v;
		}
		ucs2_t(const int &v) {
			value=v;
		}
		ucs2_t(const short &v) {
			value=v;
		}
		ucs2_t(const char &v) {
			value=v;
		}
		ucs2_t(const unsigned long long &v) {
			value=v;
		}
		ucs2_t(const unsigned long &v) {
			value=v;
		}
		ucs2_t(const unsigned int &v) {
			value=v;
		}
		ucs2_t(const unsigned short &v) {
			value=v;
		}
		ucs2_t(const unsigned char &v) {
			value=v;
		}

		ucs2_t &operator=(const ucs2_t &v) {
			value=v.value;
			return *this;
		}
		ucs2_t &operator=(const bool &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const long long &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const long &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const int &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const short &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const char &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const unsigned long long &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const unsigned long &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const unsigned int &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const unsigned short &v) {
			value=v;
			return *this;
		}
		ucs2_t &operator=(const unsigned char &v) {
			value=v;
			return *this;
		}

		operator bool() const {
			return value;
		}
		operator long long() const {
			return value;
		}
		operator long() const {
			return value;
		}
		operator int() const {
			return value;
		}
		operator short() const {
			return value;
		}
		operator char() const {
			return value;
		}
		operator unsigned long long() const {
			return value;
		}
		operator unsigned long() const {
			return value;
		}
		operator unsigned int() const {
			return value;
		}
		operator unsigned short() const {
			return value;
		}
		operator unsigned char() const {
			return value;
		}

		int operator!() {
			return !value;
		}

		int operator==(const ucs2_t &v) const {
			return value==v.value;
		}
		int operator==(bool b) const {
			return value==b;
		}
		int operator==(long long b) const {
			return value==b;
		}
		int operator==(long b) const {
			return value==b;
		}
		int operator==(int b) const {
			return value==b;
		}
		int operator==(short b) const {
			return value==b;
		}
		int operator==(char b) const {
			return value==b;
		}
		int operator==(unsigned long long b) const {
			return value==b;
		}
		int operator==(unsigned long b) const {
			return value==b;
		}
		int operator==(unsigned int b) const {
			return value==b;
		}
		int operator==(unsigned short b) const {
			return value==b;
		}
		int operator==(unsigned char b) const {
			return value==b;
		}

		int operator!=(const ucs2_t &v) const {
			return value!=v.value;
		}
		int operator!=(bool v) const {
			return value!=v;
		}
		int operator!=(long long v) const {
			return value!=v;
		}
		int operator!=(long v) const {
			return value!=v;
		}
		int operator!=(int v) const {
			return value!=v;
		}
		int operator!=(short v) const {
			return value!=v;
		}
		int operator!=(char v) const {
			return value!=v;
		}
		int operator!=(unsigned long long v) const {
			return value!=v;
		}
		int operator!=(unsigned long v) const {
			return value!=v;
		}
		int operator!=(unsigned int v) const {
			return value!=v;
		}
		int operator!=(unsigned short v) const {
			return value!=v;
		}
		int operator!=(unsigned char v) const {
			return value!=v;
		}

		int operator>(const ucs2_t &v) const {
			return value>v.value;
		}
		int operator>(bool v) const {
			return value>v;
		}
		int operator>(long long v) const {
			return value>v;
		}
		int operator>(long v) const {
			return value>v;
		}
		int operator>(int v) const {
			return value>v;
		}
		int operator>(short v) const {
			return value>v;
		}
		int operator>(char v) const {
			return value>v;
		}
		int operator>(unsigned long long v) const {
			return value>v;
		}
		int operator>(unsigned long v) const {
			return value>v;
		}
		int operator>(unsigned int v) const {
			return value>v;
		}
		int operator>(unsigned short v) const {
			return value>v;
		}
		int operator>(unsigned char v) const {
			return value>v;
		}

		int operator<(const ucs2_t &v) const {
			return value<v.value;
		}
		int operator<(bool v) const {
			return value<v;
		}
		int operator<(long long v) const {
			return value<v;
		}
		int operator<(long v) const {
			return value<v;
		}
		int operator<(int v) const {
			return value<v;
		}
		int operator<(short v) const {
			return value<v;
		}
		int operator<(char v) const {
			return value<v;
		}
		int operator<(unsigned long long v) const {
			return value<v;
		}
		int operator<(unsigned long v) const {
			return value<v;
		}
		int operator<(unsigned int v) const {
			return value<v;
		}
		int operator<(unsigned short v) const {
			return value<v;
		}
		int operator<(unsigned char v) const {
			return value<v;
		}

		int operator>=(const ucs2_t &v) const {
			return value>=v.value;
		}
		int operator>=(bool v) const {
			return value>=v;
		}
		int operator>=(long long v) const {
			return value>=v;
		}
		int operator>=(long v) const {
			return value>=v;
		}
		int operator>=(int v) const {
			return value>=v;
		}
		int operator>=(short v) const {
			return value>=v;
		}
		int operator>=(char v) const {
			return value>=v;
		}
		int operator>=(unsigned long long v) const {
			return value>=v;
		}
		int operator>=(unsigned long v) const {
			return value>=v;
		}
		int operator>=(unsigned int v) const {
			return value>=v;
		}
		int operator>=(unsigned short v) const {
			return value>=v;
		}
		int operator>=(unsigned char v) const {
			return value>=v;
		}

		int operator<=(const ucs2_t &v) const {
			return value<=v.value;
		}
		int operator<=(bool v) const {
			return value<=v;
		}
		int operator<=(long long v) const {
			return value<=v;
		}
		int operator<=(long v) const {
			return value<=v;
		}
		int operator<=(int v) const {
			return value<=v;
		}
		int operator<=(short v) const {
			return value<=v;
		}
		int operator<=(char v) const {
			return value<=v;
		}
		int operator<=(unsigned long long v) const {
			return value<=v;
		}
		int operator<=(unsigned long v) const {
			return value<=v;
		}
		int operator<=(unsigned int v) const {
			return value<=v;
		}
		int operator<=(unsigned short v) const {
			return value<=v;
		}
		int operator<=(unsigned char v) const {
			return value<=v;
		}

		ucs2_t operator-(const ucs2_t &v) const {
			ucs2_t	u;
			u.value=value-v.value;
			return u;
		}

		ucs2_t operator+(const ucs2_t &v) const {
			ucs2_t	u;
			u.value=value+v.value;
			return u;
		}
	private:
		unsigned short	value;
};
#else
typedef char16_t	ucs2_t;
#endif

// define ucs4_t, and utf(8|16)_t
typedef char32_t	ucs4_t;
typedef char8_t		utf8_t;
typedef char16_t	utf16_t;

#endif
