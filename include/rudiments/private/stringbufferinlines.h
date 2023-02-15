// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/character.h>

inline
stringbuffer::stringbuffer() : bytebuffer() {
}

inline
stringbuffer::stringbuffer(size_t initialsize) : bytebuffer(initialsize) {
}

inline
stringbuffer::stringbuffer(char *initialcontents, size_t initialsize) :
		bytebuffer((byte_t *)initialcontents,initialsize) {
}

inline
stringbuffer::stringbuffer(stringbuffer &s) : bytebuffer(s) {
}

inline
stringbuffer &stringbuffer::operator=(stringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

inline
stringbuffer::~stringbuffer() {
}

inline
off64_t stringbuffer::setPositionRelativeToBeginning(off64_t offset) {
	return bytebuffer::setPositionRelativeToBeginning(offset);
}

inline
off64_t stringbuffer::setPositionRelativeToCurrent(off64_t offset) {
	return bytebuffer::setPositionRelativeToCurrent(offset);
}

inline
off64_t stringbuffer::setPositionRelativeToEnd(off64_t offset) {
	return bytebuffer::setPositionRelativeToEnd(offset);
}

inline
const char *stringbuffer::getString() {
	bytebuffer::append('\0');
	const char	*retval=(const char *)getBuffer();
	_position(_position()-1);
	_size(_size()-1);
	return retval;
}

inline
size_t stringbuffer::getStringLength() {
	return charstring::length(getString());
}

inline
char *stringbuffer::detachString() {
	bytebuffer::append('\0');
	return (char *)detachBuffer();
}

inline
size_t stringbuffer::getPosition() {
	return bytebuffer::getPosition();
}

inline
bool stringbuffer::clear() {
	return bytebuffer::clear();
}

inline
bool stringbuffer::clear(size_t initialsize) {
	return bytebuffer::clear(initialsize);
}

inline
bool stringbuffer::clear(char *initialcontents, size_t initialsize) {
	return bytebuffer::clear((byte_t *)initialcontents,initialsize);
}

inline
stringbuffer *stringbuffer::append(const byte_t *string) {
	return (stringbuffer *)bytebuffer::append(string,
				charstring::length((const char *)string));
}

inline
stringbuffer *stringbuffer::append(const byte_t *string, size_t size) {
	return (stringbuffer *)bytebuffer::append(string,size);
}

inline
stringbuffer *stringbuffer::append(const char *string) {
	return (stringbuffer *)bytebuffer::
				append(string,charstring::length(string));
}

inline
stringbuffer *stringbuffer::append(const char *string, size_t length) {
	return (stringbuffer *)bytebuffer::append(string,length);
}

inline
stringbuffer *stringbuffer::append(char character) {
	return (stringbuffer *)bytebuffer::append(character);
}

inline
stringbuffer *stringbuffer::append(int16_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(int32_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(int64_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(int16_t number, uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
stringbuffer *stringbuffer::append(int32_t number, uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
stringbuffer *stringbuffer::append(int64_t number, uint16_t zeropadding) {
	char	*num=charstring::parseNumber(number,zeropadding);
	append(num);
	delete[] num;
	return this;
}

inline
stringbuffer *stringbuffer::append(byte_t character) {
	return (stringbuffer *)bytebuffer::append(character);
}

inline
stringbuffer *stringbuffer::append(uint16_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(uint32_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(uint64_t number) {
	return append(number,1);
}

inline
stringbuffer *stringbuffer::append(uint16_t number, uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
stringbuffer *stringbuffer::append(uint32_t number, uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
stringbuffer *stringbuffer::append(uint64_t number, uint16_t zeropadding) {
	char	*num=charstring::parseNumber(number,zeropadding);
	append(num);
	delete[] num;
	return this;
}

inline
stringbuffer *stringbuffer::append(float number) {
	return append(number,4);
}

inline
stringbuffer *stringbuffer::append(float number, uint16_t scale) {
	printf("%.*f",scale,number);
	return this;
}

inline
stringbuffer *stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	printf("%*.*f",precision,scale,number);
	return this;
}

inline
stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

inline
stringbuffer *stringbuffer::append(double number, uint16_t scale) {
	printf("%.*f",scale,number);
	return this;
}

inline
stringbuffer *stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	printf("%*.*f",precision,scale,number);
	return this;
}

inline
ssize_t stringbuffer::write(const byte_t *string) {
	return bytebuffer::write(string,
				charstring::length((const char *)string));
}

inline
ssize_t stringbuffer::write(const byte_t *string, size_t size) {
	return bytebuffer::write(string,size);
}

inline
ssize_t stringbuffer::write(const char *string) {
	return bytebuffer::write(string,charstring::length(string));
}

inline
ssize_t stringbuffer::write(const char *string, size_t length) {
	return bytebuffer::write(string,length);
}

inline
ssize_t stringbuffer::write(char character) {
	return bytebuffer::write(character);
}

inline
ssize_t stringbuffer::write(int16_t number) {
	return printf("%hd",number);
}

inline
ssize_t stringbuffer::write(int32_t number) {
	return printf("%d",number);
}

inline
ssize_t stringbuffer::write(int64_t number) {
	return printf("%lld",number);
}

inline
ssize_t stringbuffer::write(byte_t character) {
	return bytebuffer::write(character);
}

inline
ssize_t stringbuffer::write(uint16_t number) {
	return printf("%hd",number);
}

inline
ssize_t stringbuffer::write(uint32_t number) {
	return printf("%d",number);
}

inline
ssize_t stringbuffer::write(uint64_t number) {
	return bytebuffer::printf("%lld",number);
}

inline
ssize_t stringbuffer::write(float number) {
	return write(number,4);
}

inline
ssize_t stringbuffer::write(float number, uint16_t scale) {
	return printf("%.*f",scale,number);
}

inline
ssize_t stringbuffer::write(float number, uint16_t precision, uint16_t scale) {
	return printf("%*.*f",precision,scale,number);
}

inline
ssize_t stringbuffer::write(double number) {
	return write(number,4);
}

inline
ssize_t stringbuffer::write(double number, uint16_t scale) {
	return printf("%.*f",scale,number);
}

inline
ssize_t stringbuffer::write(double number, uint16_t precision, uint16_t scale) {
	return printf("%*.*f",precision,scale,number);
}

inline
void stringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

inline
void stringbuffer::truncate() {
	bytebuffer::truncate();
}
