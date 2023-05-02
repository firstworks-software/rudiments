// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/bytestring.h>

#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_HAVE_VASPRINTF
	#include <stdio.h>
	#ifdef RUDIMENTS_HAVE_STDLIB_H
		#include <stdlib.h>
	#endif
#endif

#define DEFAULT_INITIALSIZE 32

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
	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	pvt=new bytebufferprivate;
	if (initialcontents) {
		pvt->_buffer=initialcontents;
		pvt->_size=initialsize;
		pvt->_pos=initialsize;
	} else {
		pvt->_buffer=new byte_t[initialsize];
		pvt->_size=0;
		pvt->_pos=0;
	}
	pvt->_actualsize=initialsize;
	pvt->_initialsize=initialsize;
}

bytebuffer::bytebuffer(bytebuffer &v) {
	clone(v);
}

bytebuffer &bytebuffer::operator=(bytebuffer &v) {
	if (this!=&v) {
		delete[] pvt->_buffer;
		delete pvt;
		clone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	delete[] pvt->_buffer;
	delete pvt;
}

void bytebuffer::clone(bytebuffer &v) {
	pvt=new bytebufferprivate;
	pvt->_buffer=(byte_t *)bytestring::duplicate(
					v.pvt->_buffer,v.pvt->_size);
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
	size_t	finalpos=pvt->_pos+size;
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

	do {
		pvt->_actualsize=pvt->_actualsize*3/2;
	} while (requiredsize>pvt->_actualsize);

	// FIXME: we need a growth cap

	byte_t	*newbuffer=new byte_t[pvt->_actualsize];
	memcpy(newbuffer,pvt->_buffer,pvt->_size);
	delete[] pvt->_buffer;
	pvt->_buffer=newbuffer;
}

ssize_t bytebuffer::write(const char *string, size_t length) {
	return write((const byte_t *)string,length);
}

ssize_t bytebuffer::write(const char *string) {
	return write((const byte_t *)string,charstring::getLength(string));
}

ssize_t bytebuffer::write(const wchar_t *string, size_t length) {
	return write((const byte_t *)string,length*sizeof(wchar_t));
}

ssize_t bytebuffer::write(const wchar_t *string) {
	return write((const byte_t *)string,
				wcharstring::getLength(string)*sizeof(wchar_t));
}

ssize_t bytebuffer::writeUcs2(const ucs2_t *string, size_t length) {
	return write((const byte_t *)string,length*sizeof(ucs2_t));
}

ssize_t bytebuffer::writeUcs2(const ucs2_t *string) {
	return write((const byte_t *)string,
			ucs2charstring::getLength(string)*sizeof(ucs2_t));
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

		// extend the list of buffers to accommodate
		// "size" bytes beyond the current position
		if (pvt->_pos+size>pvt->_actualsize) {
			extend(pvt->_pos+size);
		}

		// write the buffer
		write(buffer,size);
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

		// extend the list of buffers to accommodate
		// "size" bytes beyond the current position
		if (pvt->_pos+size>pvt->_actualsize) {
			extend(pvt->_pos+size);
		}

		// write the buffer
		write(buffer,size);
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

		// extend the list of buffers to accommodate
		// "size" bytes beyond the current position
		if (pvt->_pos+size>pvt->_actualsize) {
			extend(pvt->_pos+size);
		}

		// write the buffer
		writeUcs2(buffer,size);
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
	delete[] pvt->_buffer;
	pvt->_buffer=new byte_t[initialsize];
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
	if (pvt->_size+size>pvt->_actualsize) {
		extend(pvt->_size+size);
	}
	memcpy(pvt->_buffer+pvt->_size,data,size);
	pvt->_size+=size;
	pvt->_pos=pvt->_size;
	return this;
}

bytebuffer *bytebuffer::append(const char *data, size_t length) {
	if (pvt->_size+length>pvt->_actualsize) {
		extend(pvt->_size+length);
	}
	memcpy(pvt->_buffer+pvt->_size,data,length);
	pvt->_size+=length;
	pvt->_pos=pvt->_size;
	return this;
}

bytebuffer *bytebuffer::append(const char *string) {
	return append((const byte_t *)string,charstring::getLength(string));
}

bytebuffer *bytebuffer::append(const wchar_t *string, size_t length) {
	return append((const byte_t *)string,length*sizeof(wchar_t));
}

bytebuffer *bytebuffer::append(const wchar_t *string) {
	return append((const byte_t *)string,
				wcharstring::getLength(string)*sizeof(wchar_t));
}

bytebuffer *bytebuffer::appendUcs2(const ucs2_t *string, size_t length) {
	return append((const byte_t *)string,length*sizeof(ucs2_t));
}

bytebuffer *bytebuffer::appendUcs2(const ucs2_t *string) {
	return append((const byte_t *)string,
			ucs2charstring::getLength(string)*sizeof(ucs2_t));
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
	pvt->_size=pos;
}

void bytebuffer::truncate() {
	pvt->_size=pvt->_pos;
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
