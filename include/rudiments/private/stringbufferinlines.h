// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/character.h>

inline
stringbuffer::stringbuffer() : bytebuffer() {
}

inline
stringbuffer::stringbuffer(size_t initialsize) : bytebuffer(initialsize) {
}

inline
stringbuffer::stringbuffer(char *initialcontents, size_t initialsize) :
		bytebuffer((unsigned char *)initialcontents,initialsize) {
}

inline
stringbuffer::stringbuffer(const stringbuffer &s) : bytebuffer(s) {
}

inline
stringbuffer &stringbuffer::operator=(const stringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

inline
stringbuffer::~stringbuffer() {
}

inline
void stringbuffer::setPosition(size_t pos) {
	bytebuffer::setPosition(pos);
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
void stringbuffer::clear() {
	bytebuffer::clear();
}

inline
void stringbuffer::clear(size_t initialsize) {
	bytebuffer::clear(initialsize);
}

inline
void stringbuffer::clear(char *initialcontents, size_t initialsize) {
	bytebuffer::clear((unsigned char *)initialcontents,initialsize);
}

inline
stringbuffer *stringbuffer::append(const unsigned char *string) {
	return (stringbuffer *)bytebuffer::append(string,
				charstring::length((const char *)string));
}

inline
stringbuffer *stringbuffer::append(const unsigned char *string, size_t size) {
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
stringbuffer *stringbuffer::append(const wchar_t *string) {
	return append(string,wcharstring::length(string));
}

inline
stringbuffer *stringbuffer::append(const wchar_t *string, char replacement) {
	return append(string,wcharstring::length(string),replacement);
}

inline
stringbuffer *stringbuffer::append(const wchar_t *string, size_t length) {
	char	*s=charstring::duplicate(string,length);
	stringbuffer	*retval=append(s,length);
	delete[] s;
	return retval;
}

inline
stringbuffer *stringbuffer::append(const wchar_t *string, size_t length,
							char replacement) {
	char	*s=charstring::duplicate(string,length,replacement);
	stringbuffer	*retval=append(s,length);
	delete[] s;
	return retval;
}

inline
stringbuffer *stringbuffer::append(wchar_t character) {
	return append(character::duplicate(character));
}

inline
stringbuffer *stringbuffer::append(wchar_t character, char replacement) {
	return append(character::duplicate(character,replacement));
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
stringbuffer *stringbuffer::append(unsigned char character) {
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
	return (stringbuffer *)appendFormatted("%.*f",scale,number);
}

inline
stringbuffer *stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)appendFormatted("%*.*f",precision,scale,number);
}

inline
stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

inline
stringbuffer *stringbuffer::append(double number, uint16_t scale) {
	return (stringbuffer *)appendFormatted("%.*f",scale,number);
}

inline
stringbuffer *stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)appendFormatted("%*.*f",precision,scale,number);
}

inline
ssize_t stringbuffer::write(const unsigned char *string) {
	return bytebuffer::write(string,
				charstring::length((const char *)string));
}

inline
ssize_t stringbuffer::write(const unsigned char *string, size_t size) {
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
ssize_t stringbuffer::write(const wchar_t *string) {
	return write(string,wcharstring::length(string));
}

inline
ssize_t stringbuffer::write(const wchar_t *string, char replacement) {
	return write(string,wcharstring::length(string),replacement);
}

inline
ssize_t stringbuffer::write(const wchar_t *string, size_t length) {
	char	*s=charstring::duplicate(string,length);
	ssize_t	retval=write(s,length);
	delete[] s;
	return retval;
}

inline
ssize_t stringbuffer::write(const wchar_t *string, size_t length,
							char replacement) {
	char	*s=charstring::duplicate(string,length,replacement);
	ssize_t	retval=write(s,length);
	delete[] s;
	return retval;
}

inline
ssize_t stringbuffer::write(wchar_t character) {
	return bytebuffer::write(character::duplicate(character));
}

inline
ssize_t stringbuffer::write(wchar_t character, char replacement) {
	return bytebuffer::write(character::duplicate(character,replacement));
}

inline
ssize_t stringbuffer::write(int16_t number) {
	return writeFormatted("%hd",number);
}

inline
ssize_t stringbuffer::write(int32_t number) {
	return writeFormatted("%d",number);
}

inline
ssize_t stringbuffer::write(int64_t number) {
	return writeFormatted("%lld",number);
}

inline
ssize_t stringbuffer::write(unsigned char character) {
	return bytebuffer::write(character);
}

inline
ssize_t stringbuffer::write(uint16_t number) {
	return writeFormatted("%hd",number);
}

inline
ssize_t stringbuffer::write(uint32_t number) {
	return writeFormatted("%d",number);
}

inline
ssize_t stringbuffer::write(uint64_t number) {
	return bytebuffer::writeFormatted("%lld",number);
}

inline
ssize_t stringbuffer::write(float number) {
	return write(number,4);
}

inline
ssize_t stringbuffer::write(float number, uint16_t scale) {
	return writeFormatted("%.*f",scale,number);
}

inline
ssize_t stringbuffer::write(float number, uint16_t precision, uint16_t scale) {
	return writeFormatted("%*.*f",precision,scale,number);
}

inline
ssize_t stringbuffer::write(double number) {
	return write(number,4);
}

inline
ssize_t stringbuffer::write(double number, uint16_t scale) {
	return writeFormatted("%.*f",scale,number);
}

inline
ssize_t stringbuffer::write(double number, uint16_t precision, uint16_t scale) {
	return writeFormatted("%*.*f",precision,scale,number);
}

inline
void stringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

inline
void stringbuffer::truncate() {
	bytebuffer::truncate();
}
