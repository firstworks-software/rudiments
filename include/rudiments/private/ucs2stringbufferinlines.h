// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>
#include <rudiments/charstring.h>

inline
ucs2stringbuffer::ucs2stringbuffer() : bytebuffer() {
}

inline
ucs2stringbuffer::ucs2stringbuffer(size_t initialsize) :
					bytebuffer(initialsize) {
}

inline
ucs2stringbuffer::ucs2stringbuffer(ucs2_t *initialcontents, size_t initialsize) :
		bytebuffer((byte_t *)initialcontents,initialsize) {
}

inline
ucs2stringbuffer::ucs2stringbuffer(ucs2stringbuffer &s) : bytebuffer(s) {
}

inline
ucs2stringbuffer &ucs2stringbuffer::operator=(ucs2stringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

inline
ucs2stringbuffer::~ucs2stringbuffer() {
}

inline
off64_t ucs2stringbuffer::setPositionRelativeToBeginning(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t,
	// or a position before the start
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(ucs2_t) || offset<0) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToBeginning(
			offset*(off64_t)sizeof(ucs2_t))/(off64_t)sizeof(ucs2_t);
}

inline
off64_t ucs2stringbuffer::setPositionRelativeToCurrent(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(ucs2_t) ||
		offset<(-max-1)/(off64_t)sizeof(ucs2_t)) {
		return -1;
	}

	// refuse a position before the start
	if (offset<0 &&
		(uint64_t)(-offset)*sizeof(ucs2_t)>(uint64_t)_position()) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToCurrent(
			offset*(off64_t)sizeof(ucs2_t))/(off64_t)sizeof(ucs2_t);
}

inline
off64_t ucs2stringbuffer::setPositionRelativeToEnd(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(ucs2_t) ||
		offset<(-max-1)/(off64_t)sizeof(ucs2_t)) {
		return -1;
	}

	// refuse a position before the start
	if (offset<0 &&
		(uint64_t)(-offset)*sizeof(ucs2_t)>(uint64_t)_size()) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToEnd(
			offset*(off64_t)sizeof(ucs2_t))/(off64_t)sizeof(ucs2_t);
}

inline
const ucs2_t *ucs2stringbuffer::getString() {
	bytebuffer::append((ucs2_t)0);
	const ucs2_t	*retval=(const ucs2_t *)getBuffer();
	bytebuffer::setPositionRelativeToBeginning(
					_position()-sizeof(ucs2_t));
	_size(_size()-sizeof(ucs2_t));
	return retval;
}

inline
size_t ucs2stringbuffer::getStringLength() {
	return ucs2charstring::getLength(getString());
}

inline
ucs2_t *ucs2stringbuffer::detachString() {
	bytebuffer::append((ucs2_t)0);
	return (ucs2_t *)detachBuffer();
}

inline
off64_t ucs2stringbuffer::getPosition() {
	return bytebuffer::getPosition()/sizeof(ucs2_t);
}

inline
bool ucs2stringbuffer::clear() {
	return bytebuffer::clear();
}

inline
bool ucs2stringbuffer::clear(size_t initialsize) {
	return bytebuffer::clear(initialsize);
}

inline
bool ucs2stringbuffer::clear(ucs2_t *initialcontents, size_t initialsize) {
	return bytebuffer::clear((byte_t *)initialcontents,initialsize);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(const byte_t *string) {
	return (ucs2stringbuffer *)bytebuffer::append(string,
			ucs2charstring::getLength((const ucs2_t *)string));
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(const byte_t *string, size_t size) {
	return (ucs2stringbuffer *)bytebuffer::append(string,size);
}

inline
ucs2stringbuffer *ucs2stringbuffer::appendUcs2(const ucs2_t *string) {
	return appendUcs2(string,ucs2charstring::getLength(string));
}

inline
ucs2stringbuffer *ucs2stringbuffer::appendUcs2(const ucs2_t *string,
							size_t length) {
	return (ucs2stringbuffer *)bytebuffer::appendUcs2(string,length);
}

inline
ucs2stringbuffer *ucs2stringbuffer::appendUcs2(ucs2_t character) {
	return (ucs2stringbuffer *)bytebuffer::appendUcs2(character);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int16_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int32_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int64_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int16_t number,
						uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int32_t number,
						uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(int64_t number,
						uint16_t zeropadding) {
	ucs2_t	*num=ucs2charstring::parseNumber(number,zeropadding);
	appendUcs2(num);
	delete[] num;
	return this;
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(byte_t character) {
	return (ucs2stringbuffer *)bytebuffer::append(character);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint16_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint32_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint64_t number) {
	return append(number,1);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint16_t number,
						uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint32_t number,
						uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(uint64_t number,
						uint16_t zeropadding) {
	ucs2_t	*num=ucs2charstring::parseNumber(number,zeropadding);
	appendUcs2(num);
	delete[] num;
	return this;
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(float number) {
	return append(number,4);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(float number, uint16_t scale) {
	return append((double)number,scale);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	return append((double)number,precision,scale);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number) {
	return append(number,4);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number, uint16_t scale) {

	// a width of 0 is the scale-only format
	return append(number,(uint16_t)0,scale);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {

	// format as narrow text, then widen
	char	*num=NULL;
	if (charstring::printf(&num,"%*.*f",precision,scale,number)!=-1) {
		ucs2_t	*wide=ucs2charstring::duplicate(num);
		appendUcs2(wide);
		delete[] wide;
	}
	delete[] num;
	return this;
}

inline
ssize_t ucs2stringbuffer::write(const byte_t *string) {
	return bytebuffer::write(string,
			ucs2charstring::getLength((const ucs2_t *)string));
}

inline
ssize_t ucs2stringbuffer::write(const byte_t *string, size_t size) {
	return bytebuffer::write(string,size);
}

inline
ssize_t ucs2stringbuffer::writeUcs2(const ucs2_t *string) {
	return bytebuffer::writeUcs2(string,ucs2charstring::getLength(string));
}

inline
ssize_t ucs2stringbuffer::writeUcs2(const ucs2_t *string, size_t length) {
	return bytebuffer::writeUcs2(string,length);
}

inline
ssize_t ucs2stringbuffer::writeUcs2(ucs2_t character) {
	return bytebuffer::writeUcs2(character);
}

inline
ssize_t ucs2stringbuffer::write(int16_t number) {
	return write((int64_t)number);
}

inline
ssize_t ucs2stringbuffer::write(int32_t number) {
	return write((int64_t)number);
}

inline
ssize_t ucs2stringbuffer::write(int64_t number) {
	ucs2_t	*num=ucs2charstring::parseNumber(number);
	ssize_t	retval=writeUcs2(num);
	delete[] num;
	return retval;
}

inline
ssize_t ucs2stringbuffer::write(byte_t character) {
	return bytebuffer::write(character);
}

inline
ssize_t ucs2stringbuffer::write(uint16_t number) {
	return write((uint64_t)number);
}

inline
ssize_t ucs2stringbuffer::write(uint32_t number) {
	return write((uint64_t)number);
}

inline
ssize_t ucs2stringbuffer::write(uint64_t number) {
	ucs2_t	*num=ucs2charstring::parseNumber(number);
	ssize_t	retval=writeUcs2(num);
	delete[] num;
	return retval;
}

inline
ssize_t ucs2stringbuffer::write(float number) {
	return write(number,4);
}

inline
ssize_t ucs2stringbuffer::write(float number, uint16_t scale) {
	return write((double)number,scale);
}

inline
ssize_t ucs2stringbuffer::write(float number,
				uint16_t precision, uint16_t scale) {
	return write((double)number,precision,scale);
}

inline
ssize_t ucs2stringbuffer::write(double number) {
	return write(number,4);
}

inline
ssize_t ucs2stringbuffer::write(double number, uint16_t scale) {

	// a width of 0 is the scale-only format
	return write(number,(uint16_t)0,scale);
}

inline
ssize_t ucs2stringbuffer::write(double number,
				uint16_t precision, uint16_t scale) {

	// format as narrow text, then widen
	char	*num=NULL;
	ssize_t	retval=charstring::printf(&num,"%*.*f",precision,scale,number);
	if (retval!=-1) {
		ucs2_t	*wide=ucs2charstring::duplicate(num);
		retval=writeUcs2(wide);
		delete[] wide;
	}
	delete[] num;
	return retval;
}

inline
void ucs2stringbuffer::truncate(size_t pos) {

	// a position too large for a byte count truncates nothing
	if (pos>(size_t)-1/sizeof(ucs2_t)) {
		return;
	}
	bytebuffer::truncate(pos*sizeof(ucs2_t));
}

inline
void ucs2stringbuffer::truncate() {

	// The inherited truncate() would call the virtual truncate(size_t),
	// which takes characters, with a byte count.
	bytebuffer::truncate(_position());
}
