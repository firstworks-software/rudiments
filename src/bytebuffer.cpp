// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_VASPRINTF
	#include <stdio.h>
	#ifdef RUDIMENTS_HAVE_STDLIB_H
		#include <stdlib.h>
	#endif
#endif

#define DEFAULT_INITIALSIZE 32
#define DEFAULT_INCREMENTSIZE 32

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
	init(NULL,DEFAULT_INITIALSIZE,DEFAULT_INCREMENTSIZE);
}

bytebuffer::bytebuffer(size_t initialsize, size_t incrementsize) {
	init(NULL,initialsize,incrementsize);
}

bytebuffer::bytebuffer(unsigned char *initialcontents,
			size_t initialsize, size_t incrementsize) {
	init(initialcontents,initialsize,incrementsize);
}

void bytebuffer::init(unsigned char *initialcontents,
				size_t initialsize,
				size_t incrementsize) {
	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (!incrementsize) {
		incrementsize=DEFAULT_INCREMENTSIZE;
	}
	pvt=new bytebufferprivate;
	pvt->_initialsize=initialsize;
	if (initialcontents) {
		pvt->_extents.append(initialcontents);
		pvt->_pos=initialsize;
	} else {
		pvt->_extents.append(new unsigned char[initialsize]);
		pvt->_pos=0;
	}
	pvt->_size=0;
	pvt->_actualsize=initialsize;
}

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		delete pvt->buffer;
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	delete pvt->buffer;
	delete pvt;
}

void bytebuffer::clearExtentList() {
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	pvt=new bytebufferprivate;
	pvt->_buffer=bytestring::duplicate(v.pvt->_buffer,v.pvt->_size);
	pvt->_size=v.pvt->_size;
	pvt->_actualsize=v.pvt->_actualsize;
	pvt->_pos=v.pvt->_pos;
	pvt->_initialsize=v.pvt->_initialsize;
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {
	size_t	bytescopied;
	copyOut(data,size,&bytescopied);
	return bytescopied;
}

ssize_t bytebuffer::write(const unsigned char *data, size_t size) {
	copyIn((unsigned char *)data,size);
	return size;
}

void bytebuffer::copyIn(unsigned char *data, size_t size) {
	if (!size) {
		return;
	}
	if (size>pvt->_size-pvt->_pos) {
		pvt->_size*=2;
		unsigned char	*newbuffer=new unsigned char[pvt->_size];
		bytestring::copy(newbuffer,pvt->_buffer,pvt->_pos);
		delete[] pvt->_buffer;
		pvt->_buffer=newbuffer;
	}
	bytestring::copy(pvt->_buffer+pvt->_pos,data,size);
}

void bytebuffer::copyOut(unsigned char *data,
					size_t size,
					size_t *bytescopied) {

	// bail if we're not actually copying anything or if
	// the current position is set to or set beyond the end
	if (!size || pvt->_pos>=pvt->_end) {
		*bytescopied=0;
		return;
	}

	findExtent();

	// copy out "size" bytes, updating the current extent and
	// number of bytes available in that extent as we go
	size_t	remaintocopy=size;
	size_t	erest=pvt->_currentextentsize-pvt->_currentextentoff;
	unsigned char	*ext=pvt->_currentextent->getValue()+
						pvt->_currentextentoff;
	*bytescopied=0;
	while (remaintocopy) {

		// calculate how many bytes to copy
		if (pvt->_currentextentstart<=pvt->_end &&
				pvt->_currentextentstart+
					pvt->_currentextentsize>=pvt->_end) {
			erest=erest-(pvt->_currentextentstart+
					pvt->_currentextentsize-pvt->_end);
		}
		size_t	bytestocopy=(erest<remaintocopy)?erest:remaintocopy;

		// copy bytes
		bytestring::copy(data,ext,bytestocopy);

		// update counters and positions
		remaintocopy-=bytestocopy;
		*bytescopied+=bytestocopy;
		data+=bytestocopy;
		if (remaintocopy) {
			if (!pvt->_currentextent->getNext()) {
				break;
			}
			pvt->_currentextent=pvt->_currentextent->getNext();
			pvt->_currentextentindex++;
			pvt->_currentextentstart+=pvt->_currentextentsize;
			pvt->_currentextentsize=pvt->_incrementsize;
			pvt->_currentextentoff=0;
			erest=pvt->_incrementsize;
			ext=pvt->_currentextent->getValue();
		}
	}

	// update counters and positions
	pvt->_pos+=*bytescopied;
	pvt->_currentextentoff=pvt->_pos-pvt->_currentextentstart;
}

ssize_t bytebuffer::write(const char *string) {
	return write(reinterpret_cast<const unsigned char *>(string),
					charstring::length(string));
}

ssize_t bytebuffer::write(const char *string, size_t size) {
	return write(reinterpret_cast<const unsigned char *>(string),size);
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
	extend(size);

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
	clear(true,false,NULL,pvt->_initextsize,pvt->_incrementsize);
}

void bytebuffer::clear(size_t initialsize, size_t incrementsize) {
	clear(true,true,NULL,initialsize,incrementsize);
}

void bytebuffer::clear(unsigned char *initialcontents,
				size_t initialsize, size_t incrementsize) {
	clear(true,true,initialcontents,initialsize,incrementsize);
}

void bytebuffer::clear(bool resetpositions,
				bool resetveryinitialsize,
				unsigned char *initialcontents,
				size_t initialsize, size_t incrementsize) {

	// remove all but the first extent
	pvt->_currentextent=pvt->_extents.getLast();
	while (pvt->_currentextent!=pvt->_extents.getFirst()) {
		linkedlistnode< unsigned char * >
				*prev=pvt->_currentextent->getPrevious();
		delete[] pvt->_currentextent->getValue();
		pvt->_extents.remove(pvt->_currentextent);
		pvt->_currentextent=prev;
	}

	// reset the initial/incremental sizes and first extent
	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (!incrementsize) {
		incrementsize=DEFAULT_INCREMENTSIZE;
	}
	if (resetveryinitialsize) {
		pvt->_originalinitextsize=initialsize;
	}
	if (initialcontents || initialsize!=pvt->_initextsize) {
		delete[] pvt->_currentextent->getValue();
		pvt->_currentextent->setValue((initialcontents)?
						initialcontents:
						new unsigned char[initialsize]);
		pvt->_initextsize=initialsize;
	}
	pvt->_initextsize=initialsize;
	pvt->_incrementsize=incrementsize;

	// reset sizes and positions
	pvt->_total=pvt->_initextsize;
	if (resetpositions) {
		pvt->_pos=0;
		pvt->_end=0;
	}
	pvt->_currentextentindex=0;
	pvt->_currentextentstart=0;
	pvt->_currentextentsize=pvt->_initextsize;
	pvt->_currentextentoff=pvt->_pos;
}

void bytebuffer::extend(size_t bytestowrite) {

	// figure out how many bytes are available between the current
	// position and the end of all extents...
	// if the position is already set past the end of all extents then
	// handle that by pretending that we just have more bytes to write...
	size_t	totalavail=0;
	if (pvt->_pos>=pvt->_total) {
		bytestowrite+=pvt->_pos-pvt->_total;
	} else {
		totalavail=pvt->_total-pvt->_pos;
	}

	// bail if we have enough space to accommodate
	// the specified number of bytes
	if (bytestowrite<=totalavail) {
		return;
	}

	// calculate how many new extents we need
	size_t	newextents=((bytestowrite-totalavail-1)/pvt->_incrementsize)+1;

	// create those extents
	for (size_t i=0; i<newextents; i++) {
		pvt->_extents.append(new unsigned char[pvt->_incrementsize]);
	}

	// update the total number of bytes in all extents
	pvt->_total+=pvt->_incrementsize*newextents;
}

void bytebuffer::findExtent() {

	if (!pvt->_dirtypos) {
		return;
	}

	// move to the extent that contains the current position
	if (pvt->_pos<pvt->_initextsize) {
		pvt->_currentextent=pvt->_extents.getFirst();
		pvt->_currentextentindex=0;
		pvt->_currentextentstart=0;
	} else {
		size_t	targetind=
			(pvt->_pos-pvt->_initextsize+pvt->_incrementsize)/
			pvt->_incrementsize;
		while (pvt->_currentextentindex!=targetind) {
			if (pvt->_currentextentindex>targetind) {
				pvt->_currentextent=
					pvt->_currentextent->getPrevious();
				pvt->_currentextentindex--;
			} else {
				pvt->_currentextent=
					pvt->_currentextent->getNext();
				pvt->_currentextentindex++;
			}
		}
		if (pvt->_currentextentindex) {
			pvt->_currentextentstart=
					pvt->_initextsize+
					pvt->_incrementsize*
					(pvt->_currentextentindex-1);
			pvt->_currentextentsize=pvt->_incrementsize;
		} else {
			pvt->_currentextentstart=0;
			pvt->_currentextentsize=pvt->_initextsize;
		}
	}
	pvt->_currentextentoff=pvt->_pos-pvt->_currentextentstart;
	pvt->_dirtypos=false;
}

const unsigned char *bytebuffer::getBuffer() {
	return pvt->_buffer;
}

unsigned char *bytebuffer::detachBuffer() {
	unsigned char *buffer=pvt->_buffer;
	pvt->_buffer=new unsigned char[pvt->_initialsize];
	pvt->_size=pvt->_initialsize;
	pvt->_pos=0;
	return buffer;
}

unsigned char *bytebuffer::coalesce(bool detach) {
	return null;
}

size_t bytebuffer::getSize() {
	return pvt->_pos;
}

size_t bytebuffer::getPosition() {
	return pvt->_pos;
}

size_t bytebuffer::getEnd() {
	return pvt->_end;
}

size_t bytebuffer::getActualSize() {
	return pvt->_size;
}

size_t bytebuffer::getInitialSize() {
	return pvt->_originalinitextsize;
}

size_t bytebuffer::getIncrementSize() {
	return pvt->_incrementsize;
}

void bytebuffer::setPosition(size_t pos) {
	pvt->_pos=pos;
	pvt->_dirtypos=true;
}

void bytebuffer::goToEnd() {
	if (pvt->_pos==pvt->_end) {
		return;
	}
	pvt->_pos=pvt->_end;
	pvt->_dirtypos=false;
	pvt->_currentextent=pvt->_extents.getLast();
	pvt->_currentextentindex=pvt->_extents.getLength()-1;
	if (pvt->_currentextentindex) {
		pvt->_currentextentstart=
				pvt->_initextsize+
				pvt->_incrementsize*
				(pvt->_currentextentindex-1);
		pvt->_currentextentsize=pvt->_incrementsize;
	} else {
		pvt->_currentextentstart=0;
		pvt->_currentextentsize=pvt->_initextsize;
	}
	pvt->_currentextentoff=pvt->_pos-pvt->_currentextentstart;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	goToEnd();
	write(data,size);
	return this;
}

bytebuffer *bytebuffer::append(const char *string) {
	return append(reinterpret_cast<const unsigned char *>(string),
						charstring::length(string));
}

bytebuffer *bytebuffer::append(const char *string, size_t size) {
	return append(reinterpret_cast<const unsigned char *>(string),size);
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
	goToEnd();
	writeFormatted(format,argp);
	return this;
}

void bytebuffer::truncate(size_t pos) {
	pvt->_end=pos;
}

void bytebuffer::truncate() {
	pvt->_end=pvt->_pos;
}

size_t bytebuffer::_end() {
	return pvt->_end;
}

void bytebuffer::_end(size_t e) {
	pvt->_end=e;
}

size_t bytebuffer::_position() {
	return pvt->_pos;
}

void bytebuffer::_position(size_t pos) {
	setPosition(pos);
}
