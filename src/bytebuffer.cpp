// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/resultcodes.h>

#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_HAVE_VASPRINTF
	#include <stdio.h>
	#ifdef RUDIMENTS_HAVE_STDLIB_H
		#include <stdlib.h>
	#endif
#endif

#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif

// if SSIZE_MAX is undefined, choose the largest value a size_t can hold
// that is also positive when stored in an ssize_t
#ifndef SSIZE_MAX
	#define SSIZE_MAX ((size_t)-1>>1)
#endif

#define DEFAULT_INITIALSIZE 32

// the largest size a buffer can have, the largest value write() can return
#define BUFFER_LIMIT ((size_t)SSIZE_MAX)

// computes count*multiplier+addend, false if it exceeds limit
static bool bufferSizeWithin(uint64_t count, uint64_t multiplier,
				uint64_t addend, size_t limit, size_t *size) {
	if (addend>limit || (multiplier && count>(limit-addend)/multiplier)) {
		return false;
	}
	*size=count*multiplier+addend;
	return true;
}

// computes count*multiplier+addend, false if it exceeds the buffer limit
static bool bufferSize(uint64_t count, uint64_t multiplier,
				uint64_t addend, size_t *size) {
	return bufferSizeWithin(count,multiplier,addend,BUFFER_LIMIT,size);
}

// grows actualsize by half, or by one when it is too small to grow by half,
// until it holds requiredsize
static size_t growSizeWithin(size_t actualsize, size_t requiredsize,
							size_t limit) {
	size_t	size=actualsize;
	while (size<requiredsize) {
		size_t	step=size/2;
		if (!step) {
			step=1;
		}

		// a step past the limit grows to exactly the required size
		if (size>limit || step>limit-size) {
			return requiredsize;
		}
		size+=step;
	}
	return size;
}

// grows actualsize to hold requiredsize, without passing the buffer limit
static size_t growSize(size_t actualsize, size_t requiredsize) {
	return growSizeWithin(actualsize,requiredsize,BUFFER_LIMIT);
}

class bytebufferprivate {
	friend class bytebuffer;
	private:
		byte_t*	_buffer;
		size_t	_size;
		size_t	_actualsize;
		size_t	_pos;
		size_t	_initialsize;
};

bytebuffer::bytebuffer() : object() {
	construct(NULL,DEFAULT_INITIALSIZE);
}

bytebuffer::bytebuffer(size_t initialsize) : object() {
	construct(NULL,initialsize);
}

bytebuffer::bytebuffer(byte_t *initialcontents, size_t initialsize) :
								object() {
	construct(initialcontents,initialsize);
}

void bytebuffer::construct(byte_t *initialcontents, size_t initialsize) {
	pvt=new bytebufferprivate;
	if (initialcontents) {

		// attached contents hold exactly initialsize bytes, even 0
		pvt->_buffer=initialcontents;
		pvt->_size=initialsize;
		pvt->_pos=initialsize;
		pvt->_actualsize=initialsize;
		if (!initialsize) {
			initialsize=DEFAULT_INITIALSIZE;
		}
	} else {
		if (!initialsize) {
			initialsize=DEFAULT_INITIALSIZE;
		}
		pvt->_buffer=new byte_t[initialsize];
		pvt->_size=0;
		pvt->_pos=0;
		pvt->_actualsize=initialsize;
	}
	pvt->_initialsize=initialsize;
}

bytebuffer::bytebuffer(bytebuffer &v) {
	clone(v);
}

bytebuffer &bytebuffer::operator=(bytebuffer &v) {
	if (this!=&v) {

		// copy first so a failed allocation leaves this unchanged
		bytebufferprivate	*oldpvt=pvt;
		clone(v);
		delete[] oldpvt->_buffer;
		delete oldpvt;
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	delete[] pvt->_buffer;
	delete pvt;
}

void bytebuffer::clone(bytebuffer &v) {

	// the copy has the same capacity, but only the used bytes are copied
	byte_t	*buffer=new byte_t[v.pvt->_actualsize];
	if (v.pvt->_size) {
		memcpy(buffer,v.pvt->_buffer,v.pvt->_size);
	}
	pvt=new bytebufferprivate;
	pvt->_buffer=buffer;
	pvt->_size=v.pvt->_size;
	pvt->_actualsize=v.pvt->_actualsize;
	pvt->_pos=v.pvt->_pos;
	pvt->_initialsize=v.pvt->_initialsize;
}

ssize_t bytebuffer::read(byte_t *data, size_t size) {

	if (size>pvt->_size-pvt->_pos) {
		size=pvt->_size-pvt->_pos;
	}

	// bail if we're not actually copying anything or if
	// the current position is set to or set beyond the end
	if (!size || pvt->_pos>=pvt->_size) {
		return 0;
	}

	memcpy(data,pvt->_buffer+pvt->_pos,size);
	pvt->_pos+=size;
	return size;
}

ssize_t bytebuffer::write(const byte_t *data, size_t size) {

	// fail if the data would end past the limit
	size_t	finalpos;
	if (!bufferSize(pvt->_pos,1,size,&finalpos)) {
		return RESULT_ERROR;
	}
	if (finalpos>pvt->_actualsize) {
		extend(finalpos);
	}
	memcpy(pvt->_buffer+pvt->_pos,data,size);
	pvt->_pos=finalpos;
	if (finalpos>pvt->_size) {
		pvt->_size=finalpos;
	}
	return size;
}

void bytebuffer::extend(size_t requiredsize) {

	// if new throws then the buffer is unchanged
	size_t	newsize=growSize(pvt->_actualsize,requiredsize);
	byte_t	*newbuffer=new byte_t[newsize];
	memcpy(newbuffer,pvt->_buffer,pvt->_size);
	delete[] pvt->_buffer;
	pvt->_buffer=newbuffer;
	pvt->_actualsize=newsize;
}

ssize_t bytebuffer::write(const char *string, size_t length) {
	return write((const byte_t *)string,length);
}

ssize_t bytebuffer::write(const char *string) {
	return write((const byte_t *)string,charstring::getLength(string));
}

ssize_t bytebuffer::write(const wchar_t *string, size_t length) {
	size_t	size;
	if (!bufferSize(length,sizeof(wchar_t),0,&size)) {
		return RESULT_ERROR;
	}
	return write((const byte_t *)string,size);
}

ssize_t bytebuffer::write(const wchar_t *string) {
	return write(string,wcharstring::getLength(string));
}

ssize_t bytebuffer::writeUcs2(const ucs2_t *string, size_t length) {
	size_t	size;
	if (!bufferSize(length,sizeof(ucs2_t),0,&size)) {
		return RESULT_ERROR;
	}
	return write((const byte_t *)string,size);
}

ssize_t bytebuffer::writeUcs2(const ucs2_t *string) {
	return writeUcs2(string,ucs2charstring::getLength(string));
}

ssize_t bytebuffer::write(char character) {
	return write((const byte_t *)&character,sizeof(char));
}

ssize_t bytebuffer::write(wchar_t character) {
	return write((const byte_t *)&character,sizeof(wchar_t));
}

ssize_t bytebuffer::writeUcs2(ucs2_t character) {
	return write((const byte_t *)&character,sizeof(ucs2_t));
}

ssize_t bytebuffer::write(int16_t number) {
	return write((const byte_t *)&number,sizeof(int16_t));
}

ssize_t bytebuffer::write(int32_t number) {
	return write((const byte_t *)&number,sizeof(int32_t));
}

ssize_t bytebuffer::write(int64_t number) {
	return write((const byte_t *)&number,sizeof(int64_t));
}

ssize_t bytebuffer::write(byte_t character) {
	return write((const byte_t *)&character,sizeof(byte_t));
}

ssize_t bytebuffer::write(uint16_t number) {
	return write((const byte_t *)&number,sizeof(uint16_t));
}

ssize_t bytebuffer::write(uint32_t number) {
	return write((const byte_t *)&number,sizeof(uint32_t));
}

ssize_t bytebuffer::write(uint64_t number) {
	return write((const byte_t *)&number,sizeof(uint64_t));
}

ssize_t bytebuffer::write(float number) {
	return write((const byte_t *)&number,sizeof(float));
}

ssize_t bytebuffer::write(double number) {
	return write((const byte_t *)&number,sizeof(double));
}

ssize_t bytebuffer::printfDelegate(const char *format, va_list *argp) {

	// write the formatted data to a buffer
	char	*buffer=NULL;
	ssize_t	size=0;
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		size=vasprintf(&buffer,format,*argp);
	#else
		size=charstring::printf(&buffer,format,argp);
	#endif
	if (size!=-1) {

		// write the buffer
		if (write(buffer,size)==RESULT_ERROR) {
			size=RESULT_ERROR;
		}
	}

	// clean up
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		free(buffer);
	#else
		delete[] buffer;
	#endif
	
	return size;
}

ssize_t bytebuffer::printfDelegate(const wchar_t *format, va_list *argp) {

	// write the formatted data to a buffer
	wchar_t	*buffer=NULL;
	ssize_t	size=wcharstring::printf(&buffer,format,argp);
	if (size!=-1) {

		// write the buffer
		if (write(buffer,size)==RESULT_ERROR) {
			size=RESULT_ERROR;
		}
	}

	// clean up
	delete[] buffer;
	
	return size;
}

ssize_t bytebuffer::printfUcs2Delegate(const ucs2_t *format, va_list *argp) {

	// write the formatted data to a buffer
	ucs2_t	*buffer=NULL;
	ssize_t	size=ucs2charstring::printf(&buffer,format,argp);
	if (size!=-1) {

		// write the buffer
		if (writeUcs2(buffer,size)==RESULT_ERROR) {
			size=RESULT_ERROR;
		}
	}

	// clean up
	delete[] buffer;
	
	return size;
}

bool bytebuffer::clear() {
	pvt->_size=0;
	pvt->_pos=0;
	return true;
}

bool bytebuffer::clear(size_t initialsize) {

	// allocate first so a failed allocation leaves this unchanged
	byte_t	*buffer=new byte_t[initialsize];
	delete[] pvt->_buffer;
	pvt->_buffer=buffer;
	pvt->_size=0;
	pvt->_actualsize=initialsize;
	pvt->_pos=0;
	pvt->_initialsize=initialsize;
	return true;
}

bool bytebuffer::clear(byte_t *initialcontents, size_t initialsize) {
	delete[] pvt->_buffer;
	pvt->_buffer=initialcontents;
	pvt->_size=initialsize;
	pvt->_actualsize=initialsize;
	pvt->_pos=initialsize;
	pvt->_initialsize=initialsize;
	return true;
}

const byte_t *bytebuffer::getBuffer() {
	return pvt->_buffer;
}

byte_t *bytebuffer::detachBuffer() {
	byte_t *buffer=pvt->_buffer;
	pvt->_buffer=new byte_t[pvt->_initialsize];
	pvt->_size=0;
	pvt->_actualsize=pvt->_initialsize;
	pvt->_pos=0;
	return buffer;
}

size_t bytebuffer::getSize() {
	return pvt->_size;
}

off64_t bytebuffer::getPosition() {
	return pvt->_pos;
}

size_t bytebuffer::getActualSize() {
	return pvt->_actualsize;
}

size_t bytebuffer::getInitialSize() {
	return pvt->_initialsize;
}

off64_t bytebuffer::setPositionRelativeToBeginning(off64_t offset) {
	pvt->_pos=offset;
	return pvt->_pos;
}

off64_t bytebuffer::setPositionRelativeToCurrent(off64_t offset) {
	pvt->_pos+=offset;
	return pvt->_pos;
}

off64_t bytebuffer::setPositionRelativeToEnd(off64_t offset) {
	pvt->_pos=pvt->_size+offset;
	return pvt->_pos;
}

bytebuffer *bytebuffer::append(const byte_t *data, size_t size) {

	// append nothing if the data would end past the limit
	size_t	finalsize;
	if (!bufferSize(pvt->_size,1,size,&finalsize)) {
		return this;
	}
	if (finalsize>pvt->_actualsize) {
		extend(finalsize);
	}
	memcpy(pvt->_buffer+pvt->_size,data,size);
	pvt->_size=finalsize;
	pvt->_pos=pvt->_size;
	return this;
}

bytebuffer *bytebuffer::append(const char *data, size_t length) {
	return append((const byte_t *)data,length);
}

bytebuffer *bytebuffer::append(const char *string) {
	return append((const byte_t *)string,charstring::getLength(string));
}

bytebuffer *bytebuffer::append(const wchar_t *string, size_t length) {
	size_t	size;
	if (!bufferSize(length,sizeof(wchar_t),0,&size)) {
		return this;
	}
	return append((const byte_t *)string,size);
}

bytebuffer *bytebuffer::append(const wchar_t *string) {
	return append(string,wcharstring::getLength(string));
}

bytebuffer *bytebuffer::appendUcs2(const ucs2_t *string, size_t length) {
	size_t	size;
	if (!bufferSize(length,sizeof(ucs2_t),0,&size)) {
		return this;
	}
	return append((const byte_t *)string,size);
}

bytebuffer *bytebuffer::appendUcs2(const ucs2_t *string) {
	return appendUcs2(string,ucs2charstring::getLength(string));
}

bytebuffer *bytebuffer::append(char character) {
	return append((const byte_t *)&character,sizeof(char));
}

bytebuffer *bytebuffer::append(wchar_t character) {
	return append((const byte_t *)&character,sizeof(wchar_t));
}

bytebuffer *bytebuffer::appendUcs2(ucs2_t character) {
	return append((const byte_t *)&character,sizeof(ucs2_t));
}

bytebuffer *bytebuffer::append(int16_t number) {
	return append((const byte_t *)&number,sizeof(int16_t));
}

bytebuffer *bytebuffer::append(int32_t number) {
	return append((const byte_t *)&number,sizeof(int32_t));
}

bytebuffer *bytebuffer::append(int64_t number) {
	return append((const byte_t *)&number,sizeof(int64_t));
}

bytebuffer *bytebuffer::append(byte_t character) {
	return append((const byte_t *)&character,sizeof(byte_t));
}

bytebuffer *bytebuffer::append(uint16_t number) {
	return append((const byte_t *)&number,sizeof(uint16_t));
}

bytebuffer *bytebuffer::append(uint32_t number) {
	return append((const byte_t *)&number,sizeof(uint32_t));
}

bytebuffer *bytebuffer::append(uint64_t number) {
	return append((const byte_t *)&number,sizeof(uint64_t));
}

bytebuffer *bytebuffer::append(float number) {
	return append((const byte_t *)&number,sizeof(float));
}

bytebuffer *bytebuffer::append(double number) {
	return append((const byte_t *)&number,sizeof(double));
}

void bytebuffer::truncate(size_t pos) {

	// truncating past the end leaves the size alone
	if (pos<pvt->_size) {
		pvt->_size=pos;
	}
}

void bytebuffer::truncate() {
	truncate(pvt->_pos);
}

size_t bytebuffer::_size() {
	return pvt->_size;
}

void bytebuffer::_size(size_t s) {
	pvt->_size=s;
}

size_t bytebuffer::_position() {
	return pvt->_pos;
}

void bytebuffer::_position(size_t pos) {
	setPositionRelativeToBeginning(pos);
}
