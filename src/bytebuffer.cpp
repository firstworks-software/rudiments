// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
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
		unsigned char *	_buffer;
		size_t		_size;
		size_t		_actualsize;
		size_t		_pos;
		size_t		_initialsize;
};

bytebuffer::bytebuffer() {
	init(NULL,DEFAULT_INITIALSIZE);
}

bytebuffer::bytebuffer(size_t initialsize) {
	init(NULL,initialsize);
}

bytebuffer::bytebuffer(unsigned char *initialcontents, size_t initialsize) {
	init(initialcontents,initialsize);
}

void bytebuffer::init(unsigned char *initialcontents, size_t initialsize) {
	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	pvt=new bytebufferprivate;
	if (initialcontents) {
		pvt->_buffer=initialcontents;
		pvt->_size=initialsize;
		pvt->_pos=initialsize;
	} else {
		pvt->_buffer=new unsigned char[initialsize];
		pvt->_size=0;
		pvt->_pos=0;
	}
	pvt->_actualsize=initialsize;
	pvt->_initialsize=initialsize;
}

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		delete pvt->_buffer;
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	delete[] pvt->_buffer;
	delete pvt;
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	pvt=new bytebufferprivate;
	pvt->_buffer=(unsigned char *)bytestring::duplicate(
					v.pvt->_buffer,v.pvt->_size);
	pvt->_size=v.pvt->_size;
	pvt->_actualsize=v.pvt->_actualsize;
	pvt->_pos=v.pvt->_pos;
	pvt->_initialsize=v.pvt->_initialsize;
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {

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

ssize_t bytebuffer::write(const unsigned char *data, size_t size) {
	size_t	finalpos=pvt->_pos+size;
	extend(finalpos);
	memcpy(pvt->_buffer+pvt->_pos,data,size);
	pvt->_pos=finalpos;
	if (finalpos>pvt->_size) {
		pvt->_size=finalpos;
	}
	return size;
}

inline
void bytebuffer::extend(size_t requiredsize) {
	if (requiredsize>pvt->_actualsize) {

		do {
			pvt->_actualsize=pvt->_actualsize*3/2;
		} while (requiredsize>pvt->_actualsize);

		// FIXME: we need a growth cap

		unsigned char	*newbuffer=new unsigned char[pvt->_actualsize];
		memcpy(newbuffer,pvt->_buffer,pvt->_size);
		delete[] pvt->_buffer;
		pvt->_buffer=newbuffer;
	}
}

ssize_t bytebuffer::write(const char *string, size_t size) {
	return write(reinterpret_cast<const unsigned char *>(string),size);
}

ssize_t bytebuffer::write(const char *string) {
	return write(reinterpret_cast<const unsigned char *>(string),
					charstring::length(string));
}

ssize_t bytebuffer::write(char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

ssize_t bytebuffer::write(int16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

ssize_t bytebuffer::write(int32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

ssize_t bytebuffer::write(int64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

ssize_t bytebuffer::write(unsigned char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

ssize_t bytebuffer::write(uint16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

ssize_t bytebuffer::write(uint32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

ssize_t bytebuffer::write(uint64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

ssize_t bytebuffer::write(float number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

ssize_t bytebuffer::write(double number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

ssize_t bytebuffer::writeFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	ssize_t	retval=writeFormatted(format,&argp);
	va_end(argp);
	return retval;
}

ssize_t bytebuffer::writeFormatted(const char *format, va_list *argp) {

	// write the formatted data to a buffer
	char	*buffer=NULL;
	ssize_t	size=0;
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		size=vasprintf(&buffer,format,*argp);
	#else
		size=charstring::printf(&buffer,format,argp);
	#endif

	// extend the list of buffers to accommodate
	// "size" bytes beyond the current position
	extend(pvt->_pos+size);

	// write the buffer
	write(buffer,size);

	// clean up
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		free(buffer);
	#else
		delete[] buffer;
	#endif
	
	return size;
}

void bytebuffer::clear() {
	pvt->_size=0;
	pvt->_pos=0;
}

void bytebuffer::clear(size_t initialsize) {
	delete[] pvt->_buffer;
	pvt->_buffer=new unsigned char[initialsize];
	pvt->_size=0;
	pvt->_actualsize=initialsize;
	pvt->_pos=0;
	pvt->_initialsize=initialsize;
}

void bytebuffer::clear(unsigned char *initialcontents, size_t initialsize) {
	delete[] pvt->_buffer;
	pvt->_buffer=initialcontents;
	pvt->_size=initialsize;
	pvt->_actualsize=initialsize;
	pvt->_pos=initialsize;
	pvt->_initialsize=initialsize;
}

const unsigned char *bytebuffer::getBuffer() {
	return pvt->_buffer;
}

unsigned char *bytebuffer::detachBuffer() {
	unsigned char *buffer=pvt->_buffer;
	pvt->_buffer=new unsigned char[pvt->_initialsize];
	pvt->_size=0;
	pvt->_actualsize=pvt->_initialsize;
	pvt->_pos=0;
	return buffer;
}

size_t bytebuffer::getSize() {
	return pvt->_size;
}

size_t bytebuffer::getPosition() {
	return pvt->_pos;
}

size_t bytebuffer::getActualSize() {
	return pvt->_actualsize;
}

size_t bytebuffer::getInitialSize() {
	return pvt->_initialsize;
}

void bytebuffer::setPosition(size_t pos) {
	pvt->_pos=pos;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	extend(pvt->_size+size);
	memcpy(pvt->_buffer+pvt->_size,data,size);
	pvt->_size+=size;
	pvt->_pos=pvt->_size;
	return this;
}

bytebuffer *bytebuffer::append(const char *data, size_t size) {
	extend(pvt->_size+size);
	memcpy(pvt->_buffer+pvt->_size,data,size);
	pvt->_size+=size;
	pvt->_pos=pvt->_size;
	return this;
}

bytebuffer *bytebuffer::append(const char *string) {
	return append(reinterpret_cast<const unsigned char *>(string),
						charstring::length(string));
}

bytebuffer *bytebuffer::append(char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

bytebuffer *bytebuffer::append(int16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

bytebuffer *bytebuffer::append(int32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

bytebuffer *bytebuffer::append(int64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

bytebuffer *bytebuffer::append(unsigned char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

bytebuffer *bytebuffer::append(uint16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

bytebuffer *bytebuffer::append(uint32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

bytebuffer *bytebuffer::append(uint64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

bytebuffer *bytebuffer::append(float number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

bytebuffer *bytebuffer::append(double number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

bytebuffer *bytebuffer::appendFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	bytebuffer	*retval=appendFormatted(format,&argp);
	va_end(argp);
	return retval;
}

bytebuffer *bytebuffer::appendFormatted(const char *format, va_list *argp) {
	pvt->_pos=pvt->_size;
	writeFormatted(format,argp);
	return this;
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
	setPosition(pos);
}
