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

#define DEFAULT_INITIALSIZE 128
#define DEFAULT_INCREMENT 32

class bytebufferprivate {
	friend class bytebuffer;
	private:
		// the list of extents
		linkedlist< unsigned char * >		_extents;

		// the current extent
		linkedlistnode< unsigned char * >	*_currentextent;

		// the index of the current extent
		size_t		_currentextentindex;

		// the original size of the initial extent
		size_t		_initialextentsize;

		// the current size of the initial extent
		// (which is usually _initialextentsize,
		// but gets expanded during a coalesce())
		size_t		_initextsize;

		// the size of each of the other extents
		size_t		_incrementsize;

		// the position that the next read or write will occur at
		size_t		_pos;

		// the position just past the last byte of valid data
		// (an append will set _pos to this)
		size_t		_end;

		// the total number of bytes in all extents
		size_t		_total;
};

bytebuffer::bytebuffer() {
	init(NULL,0,0);
}

bytebuffer::bytebuffer(size_t initialsize, size_t incrementsize) {
	init(NULL,initialsize,incrementsize);
}

bytebuffer::bytebuffer(unsigned char *initialcontents,
			size_t initialsize, size_t incrementsize) {
	init(initialcontents,initialsize,incrementsize);
}

void bytebuffer::init(unsigned char *initialcontents,
			size_t initialsize, size_t incrementsize) {
	if (initialsize==0) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (incrementsize==0) {
		incrementsize=DEFAULT_INCREMENT;
	}
	pvt=new bytebufferprivate;
	pvt->_initialextentsize=initialsize;
	pvt->_initextsize=initialsize;
	pvt->_incrementsize=incrementsize;
	if (initialcontents) {
		pvt->_extents.append(initialcontents);
		pvt->_pos=initialsize;
		pvt->_end=initialsize;
	} else {
		pvt->_extents.append(new unsigned char[initialsize]);
		pvt->_pos=0;
		pvt->_end=0;
	}
	pvt->_total=initialsize;
	pvt->_currentextent=pvt->_extents.getFirst();
	pvt->_currentextentindex=0;
}

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		clearExtentList();
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	clearExtentList();
	delete pvt;
}

void bytebuffer::clearExtentList() {
	pvt->_currentextent=pvt->_extents.getFirst();
	while (pvt->_currentextent) {
		linkedlistnode< unsigned char * > *next=
					pvt->_currentextent->getNext();
		delete[] pvt->_currentextent->getValue();
		pvt->_extents.remove(pvt->_currentextent);
 		pvt->_currentextent=next;
	}
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	
	pvt=new bytebufferprivate;

	// clone the extents
	for (linkedlistnode< unsigned char * > *curext=
				v.pvt->_extents.getFirst();
				curext; curext=curext->getNext()) {

		unsigned char	*ext=curext->getValue();

		size_t	size=(curext==v.pvt->_extents.getFirst())?
				v.pvt->_initextsize:v.pvt->_incrementsize;
		unsigned char	*newext=(unsigned char *)
					bytestring::duplicate(ext,size);
		pvt->_extents.append(newext);

		if (curext==v.pvt->_currentextent) {
			pvt->_currentextent=pvt->_extents.getLast();
		}
	}

	// clone sizes and positions
	pvt->_currentextentindex=v.pvt->_currentextentindex;
	pvt->_initialextentsize=v.pvt->_initialextentsize;
	pvt->_initextsize=v.pvt->_initextsize;
	pvt->_incrementsize=v.pvt->_incrementsize;
	pvt->_pos=v.pvt->_pos;
	pvt->_end=v.pvt->_end;
	pvt->_total=v.pvt->_total;
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {
	size_t	bytescopied;
	copy(data,size,false,&bytescopied);
	return bytescopied;
}

ssize_t bytebuffer::write(const unsigned char *data, size_t size) {
	size_t	bytescopied;
	copy((unsigned char *)data,size,true,&bytescopied);
	return bytescopied;
}

bytebuffer *bytebuffer::copy(unsigned char *data, size_t size,
					bool copyin, size_t *bytescopied) {

	// FIXME: simplest case:
	// * copyin
	// * writing to current extent
	// * size < remaining in current extent
	// maybe checking for this here, writing and bailing
	// will improve performance

	// bail if we're not actually writing anything
	// or, if copying out, if the current position
	// is set to or set beyond the end
	if (!size || (!copyin && pvt->_pos>=pvt->_end)) {
		*bytescopied=0;
		return this;
	}

	// if we're copying in, extend the list of buffers
	// to accommodate "size" bytes beyond the current position
	if (copyin) {
		extend(size);
	}

	// move to the extent that contains the current position
	if (pvt->_pos<pvt->_initextsize) {
		pvt->_currentextent=pvt->_extents.getFirst();
		pvt->_currentextentindex=0;
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
	}

	// copy-in "size" bytes, updating the current extent and
	// number of bytes available in that extent as we go
	size_t	remaintocopy=size;
	*bytescopied=0;
	while (remaintocopy) {

		// calculate how many bytes to copy
		size_t	epos=0;
		size_t	esize=pvt->_initextsize;
		if (pvt->_currentextentindex) {
			epos=pvt->_initextsize+pvt->_incrementsize*
					(pvt->_currentextentindex-1);
			esize=pvt->_incrementsize;
		}
		size_t	eoff=(remaintocopy==size)?pvt->_pos-epos:0;
		size_t	erest=esize-eoff;
		if (!copyin && epos<=pvt->_end && epos+esize>=pvt->_end) {
			erest=erest-(epos+esize-pvt->_end);
		}
		size_t	bytestocopy=(erest<remaintocopy)?erest:remaintocopy;

		// copy bytes
		unsigned char	*ext=pvt->_currentextent->getValue()+eoff;
		if (copyin) {
			bytestring::copy(ext,data,bytestocopy);
		} else {
			bytestring::copy(data,ext,bytestocopy);
		}

		// update counters and position
		remaintocopy=remaintocopy-bytestocopy;
		*bytescopied=*bytescopied+bytestocopy;
		data=data+bytestocopy;
		if (remaintocopy) {
			if (!copyin && !pvt->_currentextent->getNext()) {
				break;
			}
			pvt->_currentextent=pvt->_currentextent->getNext();
			pvt->_currentextentindex++;
		}
	}

	// update the current position and, if we're copying in, end of buffer
	pvt->_pos=pvt->_pos+*bytescopied;
	if (copyin && pvt->_pos>pvt->_end) {
		pvt->_end=pvt->_pos;
	}

	return this;
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
	if (initialsize==0) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (incrementsize==0) {
		incrementsize=DEFAULT_INCREMENT;
	}
	if (resetveryinitialsize) {
		pvt->_initialextentsize=initialsize;
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
}

void bytebuffer::extend(size_t bytestowrite) {

	// figure out how many bytes are available between the current
	// position and the end of all extents...
	// if the position is already set past the end of all extents then
	// handle that by pretending that we just have more bytes to write...
	size_t	totalavail=0;
	if (pvt->_pos>=pvt->_total) {
		bytestowrite=bytestowrite+pvt->_pos-pvt->_total;
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
	pvt->_total=pvt->_total+pvt->_incrementsize*newextents;
}

const unsigned char *bytebuffer::getBuffer() {
	return coalesce(false);
}

unsigned char *bytebuffer::detachBuffer() {
	return coalesce(true);
}

unsigned char *bytebuffer::coalesce(bool detach) {

	// combine the data from all extents into a single buffer and place
	// that buffer in the first extent...

	// if we're not detaching and there is only 1
	// extent then we don't need to do anything
	if (!detach && pvt->_extents.getLength()==1) {
		return pvt->_currentextent->getValue();
	}

	// allocate a buffer to copy the data into
	// (since we're probably going to replace the initial extent with this
	// buffer later, make sure the buffer is at least as large as the size
	// specified for the initial segment)
	size_t		coalescedbuffersize=
				(pvt->_initialextentsize>pvt->_end)?
					pvt->_initialextentsize:pvt->_end;
	unsigned char	*coalescedbuffer=new unsigned char[coalescedbuffersize];

	// copy data into the buffer
	size_t	pos=0;
	for (pvt->_currentextent=pvt->_extents.getFirst();
			pvt->_currentextent;
			pvt->_currentextent=pvt->_currentextent->getNext()) {
	
		size_t	bytestocopy=
			(pvt->_currentextent==pvt->_extents.getFirst())?
					pvt->_initextsize:pvt->_incrementsize;
		if (pvt->_end-pos<bytestocopy) {
			bytestocopy=pvt->_end-pos;
		}
		bytestring::copy(coalescedbuffer+pos,
				pvt->_currentextent->getValue(),bytestocopy);
		pos=pos+bytestocopy;
	}

	// clear all but the first extent
	// (but preserve position and end if we're not detaching)
	clear(detach,false,NULL,pvt->_initextsize,pvt->_incrementsize);

	if (!detach) {

		// replace the first extent's buffer with the coalesced buffer
		pvt->_currentextent=pvt->_extents.getFirst();
		delete[] pvt->_currentextent->getValue();
		pvt->_currentextent->setValue(coalescedbuffer);
		pvt->_currentextentindex=0;
		pvt->_initextsize=coalescedbuffersize;
		pvt->_total=coalescedbuffersize;
	}

	return coalescedbuffer;
}

size_t bytebuffer::getSize() {
	return pvt->_end;
}

size_t bytebuffer::getPosition() {
	return pvt->_pos;
}

size_t bytebuffer::getEnd() {
	return pvt->_end;
}

size_t bytebuffer::getActualSize() {
	return pvt->_total;
}

size_t bytebuffer::getInitialSize() {
	return pvt->_initialextentsize;
}

size_t bytebuffer::getIncrementSize() {
	return pvt->_incrementsize;
}

void bytebuffer::setPosition(size_t pos) {
	pvt->_pos=pos;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	pvt->_pos=pvt->_end;
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
	pvt->_pos=pvt->_end;
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
	pvt->_pos=pos;
}
