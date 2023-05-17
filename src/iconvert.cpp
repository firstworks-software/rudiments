// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/iconvert.h>
#include <rudiments/error.h>
#include <rudiments/sys.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

#ifdef RUDIMENTS_HAVE_ICONV_H
	#include <iconv.h>
#endif

// for testing
//#undef RUDIMENTS_HAVE_ICONV
//#undef RUDIMENTS_HAVE_WCRTOMB
//#undef RUDIMENTS_HAVE_WCTOMB
//#undef RUDIMENTS_HAVE_MBRTOWC
//#undef RUDIMENTS_HAVE_MBTOWC


#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>
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

// use EINVAL on platforms that don't have EILSEQ
#ifndef EILSEQ
	#ifdef EINVAL
		#define EILSEQ EINVAL
	#else
		#error no EILSEQ, EINVAL, or anything like it...
	#endif
#endif

class iconvertprivate {
	friend class iconvert;
	private:
		#ifdef RUDIMENTS_HAVE_ICONV
			iconv_t		_i;
		#endif
		bool		_open;
		bool		_dirty;

		const char	*_fromencoding;
		const byte_t	*_frombuffer;
		const byte_t	*_frombufferptr;
		size_t		_frombuffersize;
		size_t		_frombufferremaining;

		const char	*_toencoding;
		byte_t		*_tobuffer;
		byte_t		*_tobufferptr;
		size_t		_tobuffersize;
		size_t		_tobufferremaining;
		
};

iconvert::iconvert() : object() {
	construct();
}

iconvert::iconvert(iconvert &i) : object() {
	construct();
	setFromEncoding(i.pvt->_fromencoding);
	setToEncoding(i.pvt->_toencoding);
}

void iconvert::construct() {
	pvt=new iconvertprivate;

	#ifdef RUDIMENTS_HAVE_ICONV
		pvt->_i=0;
	#endif
	pvt->_open=false;
	pvt->_dirty=true;

	pvt->_fromencoding="";
	pvt->_frombuffer=NULL;
	pvt->_frombufferptr=NULL;
	pvt->_frombuffersize=0;
	pvt->_frombufferremaining=0;

	pvt->_toencoding="";
	pvt->_tobuffer=NULL;
	pvt->_tobufferptr=NULL;
	pvt->_tobuffersize=0;
	pvt->_tobufferremaining=0;
}

iconvert &iconvert::operator=(iconvert &i) {
	if (this!=&i) {
		close();
		setFromEncoding(i.pvt->_fromencoding);
		pvt->_frombuffer=NULL;
		pvt->_frombufferptr=NULL;
		pvt->_frombuffersize=0;
		pvt->_frombufferremaining=0;

		setToEncoding(i.pvt->_toencoding);
		pvt->_tobuffer=NULL;
		pvt->_tobufferptr=NULL;
		pvt->_tobuffersize=0;
		pvt->_tobufferremaining=0;
	}
	return *this;
}

iconvert::~iconvert() {
	close();
	delete pvt;
}

void iconvert::setFromEncoding(const char *fromencoding) {
	pvt->_fromencoding=(fromencoding)?fromencoding:"";
	pvt->_dirty=true;
}

const char *iconvert::getFromEncoding() {
	return pvt->_fromencoding;
}

void iconvert::setToEncoding(const char *toencoding) {
	pvt->_toencoding=(toencoding)?toencoding:"";
	pvt->_dirty=true;
}

const char *iconvert::getToEncoding() {
	return pvt->_toencoding;
}

void iconvert::setFromBuffer(const byte_t *buffer) {
	pvt->_frombuffer=buffer;
}

const byte_t *iconvert::getFromBuffer() {
	return pvt->_frombuffer;
}

void iconvert::setFromBufferSize(size_t buffersize) {
	pvt->_frombuffersize=buffersize;
}

size_t iconvert::getFromBufferSize() {
	return pvt->_frombuffersize;
}

void iconvert::setToBuffer(byte_t *buffer) {
	pvt->_tobuffer=buffer;
}

byte_t *iconvert::getToBuffer() {
	return pvt->_tobuffer;
}

void iconvert::setToBufferSize(size_t buffersize) {
	pvt->_tobuffersize=buffersize;
}

size_t iconvert::getToBufferSize() {
	return pvt->_tobuffersize;
}

bool iconvert::convert() {

	// if we changed the to/from encodings since the last
	// call to convert() then close and force a re-open
	if (pvt->_dirty) {
		close();
		pvt->_dirty=false;
	}

	#ifdef RUDIMENTS_HAVE_ICONV
		size_t result=0;

		// open, if we're not already open
		if (!pvt->_open) {

			error::clearError();

			pvt->_i=iconv_open(pvt->_toencoding,pvt->_fromencoding);

			// If iconv_open() returns -1 then it ought to set
			// errno.  When Solaris 8 sparc (but not x86, oddly)
			// doesn't support a conversion, it returns -1 but
			// doesn't set the errno.  Catch that and fake EINVAL.
			if (pvt->_i==(iconv_t)-1 && !error::getErrorNumber()) {
				error::setErrorNumber(EINVAL);
			}

			// if the requested conversion isn't supported, then we
			// should fall back to other methods
			//
			// Eg.
			//
			// SCO UW 7.0.1 iconv doesn't convert to/from
			// wchar_t but its wctomb()/mbtowc() do
			//
			// Solaris 11 iconv doesn't appear to convert to/from
			// UCS-2
			if (error::getErrorNumber()==EINVAL) {

				debugPrintf("iconv_open(\"%s\",\"%s\") "
						"failed with error %d "
						"- attempting other "
						"methods...\n",
						pvt->_toencoding,
						pvt->_fromencoding,
						error::getErrorNumber());
				

				// Solaris 11 misbehaves when it doesn't support
				// a conversion.  Rather than returning -1, it
				// returns a valid descriptor but still sets
				// errno to EINVAL.  Lets close the descriptor.
				if (pvt->_i!=(iconv_t)-1) {
					iconv_close(pvt->_i);
				}

				pvt->_i=0;
				error::clearError();

				goto fallback;
			}

			// if iconv_open() failed for some reason other than
			// not supporting the conversion, then bail
			if (pvt->_i==(iconv_t)-1) {
				debugPrintf("iconv_open(\"%s\",\"%s\") "
						"failed with error %d "
						"- not attempting other "
						"methods.\n",
						pvt->_toencoding,
						pvt->_fromencoding,
						error::getErrorNumber());
				pvt->_i=0;
				return false;
			}

			// success
			pvt->_open=true;
			pvt->_frombufferptr=pvt->_frombuffer;
			pvt->_frombufferremaining=pvt->_frombuffersize;
			pvt->_tobufferptr=pvt->_tobuffer;
			pvt->_tobufferremaining=pvt->_tobuffersize;
		}

		// convert a character
		result=iconv(pvt->_i,
				#ifdef RUDIMENTS_HAVE_ICONV_WITH_NONCONST_INBUF
				(char **)
				#else
				(const char **)
				#endif
				&(pvt->_frombufferptr),
				&(pvt->_frombufferremaining),
				(char **)&(pvt->_tobufferptr),
				&(pvt->_tobufferremaining));
		if (result==(size_t)-1 ||
			(result==0 && error::getErrorNumber())) {
			debugPrintf("iconv() failed with result %d "
						"and error %d\n",
						result,error::getErrorNumber());
			return false;
		}
		return true;

fallback:
	#endif

	// open, if we're not already open
	if (!pvt->_open) {
		pvt->_open=true;
		pvt->_frombufferptr=pvt->_frombuffer;
		pvt->_frombufferremaining=pvt->_frombuffersize;
		pvt->_tobufferptr=pvt->_tobuffer;
		pvt->_tobufferremaining=pvt->_tobuffersize;
	}

	// sanity check on from buffer
	if (!pvt->_frombufferptr) {
		debugPrintf("no from buffer\n");
		error::setErrorNumber(EILSEQ);
		return false;
	}

	// without iconv() and friends, we can still convert to and from
	// the current locale, wchar_t, and ucs-2be/le...

	// determine from and to encodings
	const char	*fromenc=pvt->_fromencoding;
	const char	*toenc=pvt->_toencoding;
	#ifdef _WIN32
		// on windows, wchar_t's are encoded as UCS-2LE,
		// so we can support UCS-2LE by piggybacking
		if (!charstring::compare(fromenc,"UCS-2LE")) {
			fromenc="WCHAR_T";
		}
		if (!charstring::compare(toenc,"UCS-2LE")) {
			toenc="WCHAR_T";
		}
	#endif

	// initialize sizes and return value
	size_t	fromsize=0;
	size_t	tosize=0;

	if (!charstring::compare(fromenc,"WCHAR_T") &&
				!charstring::compare(toenc,"")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(wchar_t) ||
			pvt->_tobufferremaining<getMaxMultiByteSize()) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(wchar_t);
		wchar_t	from=*((const wchar_t *)pvt->_frombufferptr);

		// set up "to"
		char	*to=(char *)pvt->_tobufferptr;
		bytestring::zero(to,getMaxMultiByteSize());

		// convert...
		#if defined(RUDIMENTS_HAVE_WCRTOMB)
			debugPrintf("wcrtomb()... ");
			mbstate_t	st;
			bytestring::zero(&st,sizeof(st));
			tosize=wcrtomb(to,from,&st);
		#elif defined(RUDIMENTS_HAVE_WCTOMB)
			debugPrintf("wctomb()... ");
			tosize=wctomb(to,from);
		#else
			debugPrintf("direct conversion... ");
			// FIXME: Arguably we should verify that the
			// character set of the current locale is ASCII
			// or some kind of extended ASCII.  This is
			// likely the case on platforms that don't
			// provide wctomb/wcrtomb, but not guaranteed.
			if (from<128) {
				tosize=sizeof(char);
				if (to) {
					*to=(char)from;
				}
			} else {
				tosize=(size_t)-1;
				error::setErrorNumber(EILSEQ);
			}
		#endif

		if (tosize==(size_t)-1) {
			debugPrintf("failed\n");
			return false;
		}

	} else if (!charstring::compare(fromenc,"") &&
			!charstring::compare(toenc,"WCHAR_T")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(char) ||
			pvt->_tobufferremaining<sizeof(wchar_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		const char	*from=(const char *)pvt->_frombufferptr;

		// set up "to"
		tosize=sizeof(wchar_t);
		wchar_t	*to=(wchar_t *)pvt->_tobufferptr;
		bytestring::zero(to,sizeof(wchar_t));

		// convert...
		#if defined(RUDIMENTS_HAVE_WCRTOMB)
			debugPrintf("mbrtowc()... ");
			mbstate_t	st;
			bytestring::zero(&st,sizeof(st));
			fromsize=mbrtowc(to,from,
					pvt->_frombufferremaining,
					&st);
		#elif defined(RUDIMENTS_HAVE_WCTOMB)
			debugPrintf("mbtowc()... ");
			// mbtowc() doesn't like being passed '\0' on
			// some platforms (redhat 4.2 with libc5)
			if (from) {
				fromsize=mbtowc(to,from,
					pvt->_frombufferremaining);
			} else {
				fromsize=sizeof(char);
				if (to) {
					*to=(wchar_t)0;
				}
			}
		#else
			debugPrintf("direct conversion... ");
			// FIXME: Arguably we should verify that the
			// character set of the current locale is ASCII
			// or some kind of extended ASCII.  This is
			// likely the case on platforms that don't
			if ((unsigned char)*from<128) {
				fromsize=sizeof(char);
				if (to) {
					*to=(wchar_t)*from;
				}
			} else {
				fromsize=(size_t)-1;
				error::setErrorNumber(EILSEQ);
			}
		#endif

		// bail on error
		if (fromsize==(size_t)-1 || fromsize==(size_t)-2) {
			debugPrintf("failed\n");
			return false;
		}

	} else if (!charstring::compare(fromenc,"UCS-2",5) &&
				!charstring::compare(toenc,"")) {

		// FIXME: verify that the character set of the current
		// locale is ASCII or some kind of extended ASCII

		// set up "from"
		fromsize=sizeof(ucs2_t);
		ucs2_t	from=*((const ucs2_t *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(char);
		char	*to=(char *)pvt->_tobufferptr;

		// convert
		debugPrintf("direct conversion... ");
		if (from<128) {
			if (to) {
				*to=(char)byteswap(fromenc,from);
			}
		} else {
			debugPrintf("failed - invalid sequence\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

	} else if (!charstring::compare(fromenc,"") &&
				!charstring::compare(toenc,"UCS-2",5)) {

		// FIXME: verify that the character set of the current
		// locale is ASCII or some kind of extended ASCII

		// set up "from"
		fromsize=sizeof(char);
		char	from=*((const char *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(ucs2_t);
		ucs2_t	*to=(ucs2_t *)pvt->_tobufferptr;

		// convert
		debugPrintf("direct conversion... ");
		if ((unsigned char)from<128) {
			if (to) {
				*to=byteswap(toenc,(ucs2_t)from);
			}
		} else {
			debugPrintf("failed - invalid sequence\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

	} else if (!charstring::compare(fromenc,"UCS-2",5) &&
				!charstring::compare(toenc,"WCHAR_T")) {
		
		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(ucs2_t) ||
			pvt->_tobufferremaining<sizeof(wchar_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(ucs2_t);
		ucs2_t	from=*((const ucs2_t *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(wchar_t);
		wchar_t	*to=(wchar_t *)pvt->_tobufferptr;

		// FIXME: use mbrtoc16/mbtoc16 if available

		// convert
		// FIXME: this implementation is incorrect for
		// chars >= 128 and for platforms where the wchar_t
		// format is not the same as UCS-2 (Solaris 9-)
		debugPrintf("direct conversion... ");
		if (from<128) {
			if (to) {
				*to=(wchar_t)byteswap(fromenc,from);
			}
		} else {
			debugPrintf("failed - invalid sequence\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

	} else if (!charstring::compare(fromenc,"WCHAR_T") &&
				!charstring::compare(toenc,"UCS-2",5)) {
		
		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(wchar_t) ||
			pvt->_tobufferremaining<sizeof(ucs2_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(wchar_t);
		wchar_t	from=*((const wchar_t *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(ucs2_t);
		ucs2_t	*to=(ucs2_t *)pvt->_tobufferptr;
		bytestring::zero(to,tosize);

		// FIXME: use c16rtomb/c16tomb if available

		// convert
		// FIXME: this implementation is incorrect for
		// chars >= 128 and for platforms where the wchar_t
		// format is not the same as UCS-2 (Solaris 9-)
		debugPrintf("direct conversion... ");
		if (from<128) {
			if (to) {
				*to=byteswap(toenc,(ucs2_t)from);
			}
		} else {
			debugPrintf("failed - invalid sequence\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

	} else if ((!charstring::compare(fromenc,"UCS-2LE") &&
				!charstring::compare(toenc,"UCS-2LE")) ||
			(!charstring::compare(fromenc,"UCS-2BE") &&
				!charstring::compare(toenc,"UCS-2BE"))) {
		
		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(ucs2_t) ||
			pvt->_tobufferremaining<sizeof(ucs2_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(ucs2_t);

		// set up "to"
		tosize=sizeof(ucs2_t);

		// copy
		debugPrintf("direct conversion... ");
		if (pvt->_tobufferptr) {
			bytestring::copy(pvt->_tobufferptr,
						pvt->_frombufferptr,
						tosize);
		}

	} else if (!charstring::compare(fromenc,"UCS-2LE") &&
				!charstring::compare(toenc,"UCS-2BE")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(wchar_t) ||
			pvt->_tobufferremaining<sizeof(ucs2_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(ucs2_t);
		ucs2_t	from=*((const ucs2_t *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(ucs2_t);
		ucs2_t	*to=(ucs2_t *)pvt->_tobufferptr;

		// convert
		debugPrintf("direct conversion... ");
		if (to) {
			*to=(ucs2_t)filedescriptor::convertHostToNet(
				filedescriptor::convertLittleEndianToHost(
							(uint16_t)from));
		}

	} else if (!charstring::compare(fromenc,"UCS-2BE") &&
				!charstring::compare(toenc,"UCS-2LE")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(wchar_t) ||
			pvt->_tobufferremaining<sizeof(ucs2_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(ucs2_t);
		ucs2_t	from=*((const ucs2_t *)pvt->_frombufferptr);

		// set up "to"
		tosize=sizeof(ucs2_t);
		ucs2_t	*to=(ucs2_t *)pvt->_tobufferptr;

		// convert
		debugPrintf("direct conversion... ");
		if (to) {
			*to=(ucs2_t)filedescriptor::convertHostToLittleEndian(
				filedescriptor::convertNetToHost((uint16_t)from));
		}

	} else if (!charstring::compare(fromenc,"WCHAR_T") &&
				!charstring::compare(toenc,"WCHAR_T")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(wchar_t) ||
			pvt->_tobufferremaining<sizeof(wchar_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		fromsize=sizeof(wchar_t);

		// set up "to"
		tosize=sizeof(wchar_t);

		// copy
		debugPrintf("direct conversion... ");
		if (pvt->_tobufferptr) {
			bytestring::copy(pvt->_tobufferptr,
						pvt->_frombufferptr,
						tosize);
		}

	} else if (!charstring::compare(fromenc,"") &&
				!charstring::compare(toenc,"")) {

		// sanity check on buffers
		if (pvt->_frombufferremaining<sizeof(char) ||
			pvt->_tobufferremaining<sizeof(wchar_t)) {
			debugPrintf("buffer check failed\n");
			error::setErrorNumber(EILSEQ);
			return false;
		}

		// set up "from"
		const char	*from=(const char *)pvt->_frombufferptr;

		// set up "to"
		char		*to=(char *)pvt->_tobufferptr;

		// get the number of bytes to copy
		#if defined(RUDIMENTS_HAVE_WCRTOMB)
			debugPrintf("mbrtowc()... ");
			mbstate_t	st;
			bytestring::zero(&st,sizeof(st));
			fromsize=mbrtowc(NULL,from,
					pvt->_frombufferremaining,
					&st);
		#elif defined(RUDIMENTS_HAVE_WCTOMB)
			debugPrintf("mbtowc()... ");
			// mbtowc() doesn't like being passed '\0' on
			// some platforms (redhat 4.2 with libc5)
			if (from) {
				fromsize=mbtowc(NULL,from,
					pvt->_frombufferremaining);
			} else {
				fromsize=sizeof(char);
			}
		#else
			debugPrintf("direct conversion... ");
			// FIXME: verify that the character set of the
			// current locale is single-byte
			fromsize=sizeof(char);
		#endif

		// bail on error
		if (fromsize==(size_t)-1 || fromsize==(size_t)-2) {
			debugPrintf("failed\n");
			return false;
		}

		// copy the bytes
		if (to) {
			bytestring::copy(to,from,fromsize);
		}

	} else {

		debugPrintf("unsupported conversion\n");
		error::setErrorNumber(ENOSYS);
		return false;
	}

	// bump buffer pointers and remaining counts
	if ((ssize_t)fromsize>-1) {
		pvt->_frombufferptr+=fromsize;
		pvt->_frombufferremaining-=fromsize;
		if (pvt->_tobufferptr) {
			pvt->_tobufferptr+=tosize;
			pvt->_tobufferremaining-=tosize;
		}
	}

	debugPrintf("success\n");
	return true;
}

ucs2_t iconvert::byteswap(const char *enc, ucs2_t value) {
	if (sys::isBigEndian()) {
		if (!charstring::compare(enc,"UCS-2LE")) {
			return (ucs2_t)filedescriptor::convertLittleEndianToHost(
							(uint16_t)value);
		}
	} else {
		if (!charstring::compare(enc,"UCS-2BE")) {
			return (ucs2_t)filedescriptor::convertNetToHost(
							(uint16_t)value);
		}
	}
	return value;
}

const byte_t *iconvert::getFromBufferPosition() {
	return pvt->_frombufferptr;
}

size_t iconvert::getFromBufferRemaining() {
	return pvt->_frombufferremaining;
}

const byte_t *iconvert::getToBufferPosition() {
	return pvt->_tobufferptr;
}

size_t iconvert::getToBufferRemaining() {
	return pvt->_tobufferremaining;
}

void iconvert::close() {
	if (pvt->_open) {
		#ifdef RUDIMENTS_HAVE_ICONV
			// NOTE: some platforms (SCO UW 7.0.1) crash if
			// pvt->_i=-1.  If iconv_open() returned -1 then code
			// above should have set pvt->_i=0, and also pvt->_open
			// should be set to false, so iconv_close(-1) should
			// never be called, but it's worth mentioning in case
			// any of this code gets changed in the future.
			iconv_close(pvt->_i);
			pvt->_i=0;
		#endif
		pvt->_open=false;
	}
}

uint16_t iconvert::getMaxMultiByteSize() {
	#ifdef MB_CUR_MAX
		// NOTE: MB_CUR_MAX is not a constant, but rather a macro that
		// expands to an integer expression.  It also varies with the
		// locale.
		return MB_CUR_MAX;
	#else
		return 4;
	#endif
}

bool iconvert::getNeedsMutex() {
	#ifdef RUDIMENTS_HAVE_WCRTOMB
		return false;
	#endif
	return true;
}
