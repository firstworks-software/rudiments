// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/iconvert.h>

#ifdef RUDIMENTS_HAVE_ICONV_H
	#include <iconv.h>
#endif

class iconvertprivate {
	friend class iconvert;
	private:
		iconv_t		_i;

		const char		*_fromencoding;
		const unsigned char	*_frombuffer;
		const unsigned char	*_frombufferptr;
		size_t			_frombuffersize;
		size_t			_frombufferremaining;

		const char	*_toencoding;
		unsigned char	*_tobuffer;
		unsigned char	*_tobufferptr;
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

	pvt->_i=0;

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
		setToEncoding(i.pvt->_toencoding);
	}
	return *this;
}

iconvert::~iconvert() {
	close();
	delete pvt;
}

void iconvert::setFromEncoding(const char *fromencoding) {
	pvt->_fromencoding=(fromencoding)?fromencoding:"";
}

const char *iconvert::getFromEncoding() {
	return pvt->_fromencoding;
}

void iconvert::setToEncoding(const char *toencoding) {
	pvt->_toencoding=(toencoding)?toencoding:"";
}

const char *iconvert::getToEncoding() {
	return pvt->_toencoding;
}

void iconvert::setFromBuffer(const unsigned char *buffer) {
	pvt->_frombuffer=buffer;
}

const unsigned char *iconvert::getFromBuffer() {
	return pvt->_frombuffer;
}

void iconvert::setFromBufferSize(size_t buffersize) {
	pvt->_frombuffersize=buffersize;
}

size_t iconvert::getFromBufferSize() {
	return pvt->_frombuffersize;
}

void iconvert::setToBuffer(unsigned char *buffer) {
	pvt->_tobuffer=buffer;
}

unsigned char *iconvert::getToBuffer() {
	return pvt->_tobuffer;
}

void iconvert::setToBufferSize(size_t buffersize) {
	pvt->_tobuffersize=buffersize;
}

size_t iconvert::getToBufferSize() {
	return pvt->_tobuffersize;
}

bool iconvert::convert() {

	#ifdef RUDIMENTS_HAVE_ICONV

		// open, if we haven't already
		if (!pvt->_i) {
			pvt->_i=iconv_open(pvt->_toencoding,pvt->_fromencoding);
			if (pvt->_i==(iconv_t)-1) {
				return false;
			}
			pvt->_frombufferptr=pvt->_frombuffer;
			pvt->_frombufferremaining=pvt->_frombuffersize;
			pvt->_tobufferptr=pvt->_tobuffer;
			pvt->_tobufferremaining=pvt->_tobuffersize;
		}

		// convert a character
		return iconv(pvt->_i,
				(char **)&(pvt->_frombufferptr),
				&(pvt->_frombufferremaining),
				(char **)&(pvt->_tobufferptr),
				&(pvt->_tobufferremaining))!=(size_t)-1;
	#else
		#error no iconv or anything like it
	#endif
}

const unsigned char *iconvert::getFromBufferPosition() {
	return pvt->_frombufferptr;
}

size_t iconvert::getFromBufferRemaining() {
	return pvt->_frombufferremaining;
}

const unsigned char *iconvert::getToBufferPosition() {
	return pvt->_tobufferptr;
}

size_t iconvert::getToBufferRemaining() {
	return pvt->_tobufferremaining;
}

bool iconvert::close() {
	bool	result=!iconv_close(pvt->_i);
	pvt->_i=0;

	pvt->_frombuffer=NULL;
	pvt->_frombufferptr=NULL;
	pvt->_frombuffersize=0;
	pvt->_frombufferremaining=0;

	pvt->_tobuffer=NULL;
	pvt->_tobufferptr=NULL;
	pvt->_tobuffersize=0;
	pvt->_tobufferremaining=0;
	return result;
}

bool iconvert::reset() {
	bool	result=close();
	pvt->_fromencoding="";
	pvt->_toencoding="";
	return result;
}
