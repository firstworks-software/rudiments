// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/ucs2charstring.h>
#include <rudiments/ucs2character.h>

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
	return bytebuffer::setPositionRelativeToBeginning(
						offset*sizeof(ucs2_t));
}

inline
off64_t ucs2stringbuffer::setPositionRelativeToCurrent(off64_t offset) {
	return bytebuffer::setPositionRelativeToCurrent(
						offset*sizeof(ucs2_t));
}

inline
off64_t ucs2stringbuffer::setPositionRelativeToEnd(off64_t offset) {
	return bytebuffer::setPositionRelativeToEnd(
						offset*sizeof(ucs2_t));
}

inline
const ucs2_t *ucs2stringbuffer::getString() {
	bytebuffer::append((ucs2_t)0);
	const ucs2_t	*retval=(const ucs2_t *)getBuffer();
	_position(_position()-1);
	_size(_size()-1);
	return retval;
}

inline
size_t ucs2stringbuffer::getStringLength() {
	return ucs2charstring::length(getString());
}

inline
ucs2_t *ucs2stringbuffer::detachString() {
	bytebuffer::append((ucs2_t)0);
	return (ucs2_t *)detachBuffer();
}

inline
size_t ucs2stringbuffer::getPosition() {
	return bytebuffer::getPosition();
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
			ucs2charstring::length((const ucs2_t *)string));
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(const byte_t *string, size_t size) {
	return (ucs2stringbuffer *)bytebuffer::append(string,size);
}

inline
ucs2stringbuffer *ucs2stringbuffer::appendUcs2(const ucs2_t *string) {
	return appendUcs2(string,ucs2charstring::length(string));
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
	return (ucs2stringbuffer *)appendFormatted("%.*f",scale,number);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	return (ucs2stringbuffer *)appendFormatted(
					"%*.*f",precision,scale,number);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number) {
	return append(number,4);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number, uint16_t scale) {
	return (ucs2stringbuffer *)appendFormatted("%.*f",scale,number);
}

inline
ucs2stringbuffer *ucs2stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	return (ucs2stringbuffer *)appendFormatted(
					"%*.*f",precision,scale,number);
}

inline
ssize_t ucs2stringbuffer::write(const byte_t *string) {
	return bytebuffer::write(string,
			ucs2charstring::length((const ucs2_t *)string));
}

inline
ssize_t ucs2stringbuffer::write(const byte_t *string, size_t size) {
	return bytebuffer::write(string,size);
}

inline
ssize_t ucs2stringbuffer::writeUcs2(const ucs2_t *string) {
	return bytebuffer::writeUcs2(string,ucs2charstring::length(string));
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
	return printf("%hd",number);
}

inline
ssize_t ucs2stringbuffer::write(int32_t number) {
	return printf("%d",number);
}

inline
ssize_t ucs2stringbuffer::write(int64_t number) {
	return printf("%lld",number);
}

inline
ssize_t ucs2stringbuffer::write(byte_t character) {
	return bytebuffer::write(character);
}

inline
ssize_t ucs2stringbuffer::write(uint16_t number) {
	return printf("%hd",number);
}

inline
ssize_t ucs2stringbuffer::write(uint32_t number) {
	return printf("%d",number);
}

inline
ssize_t ucs2stringbuffer::write(uint64_t number) {
	return bytebuffer::printf("%lld",number);
}

inline
ssize_t ucs2stringbuffer::write(float number) {
	return write(number,4);
}

inline
ssize_t ucs2stringbuffer::write(float number, uint16_t scale) {
	return printf("%.*f",scale,number);
}

inline
ssize_t ucs2stringbuffer::write(float number,
				uint16_t precision, uint16_t scale) {
	return printf("%*.*f",precision,scale,number);
}

inline
ssize_t ucs2stringbuffer::write(double number) {
	return write(number,4);
}

inline
ssize_t ucs2stringbuffer::write(double number, uint16_t scale) {
	return printf("%.*f",scale,number);
}

inline
ssize_t ucs2stringbuffer::write(double number,
				uint16_t precision, uint16_t scale) {
	return printf("%*.*f",precision,scale,number);
}

inline
void ucs2stringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

inline
void ucs2stringbuffer::truncate() {
	bytebuffer::truncate();
}
