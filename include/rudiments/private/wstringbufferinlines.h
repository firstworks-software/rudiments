// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/wcharstring.h>
#include <rudiments/wcharacter.h>

inline
wstringbuffer::wstringbuffer() : bytebuffer() {
}

inline
wstringbuffer::wstringbuffer(size_t initialsize) : bytebuffer(initialsize) {
}

inline
wstringbuffer::wstringbuffer(wchar_t *initialcontents, size_t initialsize) :
		bytebuffer((unsigned char *)initialcontents,
					initialsize*sizeof(wchar_t)) {
}

inline
wstringbuffer::wstringbuffer(const wstringbuffer &s) : bytebuffer(s) {
}

inline
wstringbuffer &wstringbuffer::operator=(const wstringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

inline
wstringbuffer::~wstringbuffer() {
}

inline
void wstringbuffer::setPosition(size_t pos) {
	bytebuffer::setPosition(pos*sizeof(wchar_t));
}

inline
const wchar_t *wstringbuffer::getString() {
	bytebuffer::append(L'\0');
	const wchar_t	*retval=(const wchar_t *)getBuffer();
	_position(_position()-1);
	_size(_size()-1);
	return retval;
}

inline
size_t wstringbuffer::getStringLength() {
	return wcharstring::length(getString());
}

inline
wchar_t *wstringbuffer::detachString() {
	bytebuffer::append(L'\0');
	return (wchar_t *)detachBuffer();
}

inline
size_t wstringbuffer::getPosition() {
	return bytebuffer::getPosition()/sizeof(wchar_t);
}

inline
void wstringbuffer::clear() {
	bytebuffer::clear();
}

inline
void wstringbuffer::clear(size_t initialsize) {
	bytebuffer::clear(initialsize);
}

inline
void wstringbuffer::clear(wchar_t *initialcontents, size_t initialsize) {
	bytebuffer::clear((unsigned char *)initialcontents,
					initialsize*sizeof(wchar_t));
}

inline
wstringbuffer *wstringbuffer::append(const char *string) {
	return append(string,charstring::length(string));
}

inline
wstringbuffer *wstringbuffer::append(const char *string, size_t length) {
	wchar_t	*s=wcharstring::duplicate(string,length);
	wstringbuffer	*retval=append(s,length);
	delete[] s;
	return retval;
}

inline
wstringbuffer *wstringbuffer::append(char character) {
	return (wstringbuffer *)bytebuffer::append(
				wcharacter::duplicate(character));
}

inline
wstringbuffer *wstringbuffer::append(const wchar_t *string) {
	return append(string,wcharstring::length(string));
}

inline
wstringbuffer *wstringbuffer::append(const wchar_t *string, size_t length) {
	return (wstringbuffer *)bytebuffer::append(string,length);
}

inline
wstringbuffer *wstringbuffer::append(wchar_t character) {
	return (wstringbuffer *)bytebuffer::append(character);
}

inline
wstringbuffer *wstringbuffer::append(int16_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(int32_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(int64_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(int16_t number, uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
wstringbuffer *wstringbuffer::append(int32_t number, uint16_t zeropadding) {
	return append((int64_t)number,zeropadding);
}

inline
wstringbuffer *wstringbuffer::append(int64_t number, uint16_t zeropadding) {
	wchar_t	*num=wcharstring::parseNumber(number,zeropadding);
	append(num);
	delete[] num;
	return this;
}

inline
wstringbuffer *wstringbuffer::append(uint16_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(uint32_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(uint64_t number) {
	return append(number,1);
}

inline
wstringbuffer *wstringbuffer::append(uint16_t number, uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
wstringbuffer *wstringbuffer::append(uint32_t number, uint16_t zeropadding) {
	return append((uint64_t)number,zeropadding);
}

inline
wstringbuffer *wstringbuffer::append(uint64_t number, uint16_t zeropadding) {
	wchar_t	*num=wcharstring::parseNumber(number,zeropadding);
	append(num);
	delete[] num;
	return this;
}

inline
wstringbuffer *wstringbuffer::append(float number) {
	return append(number,4);
}

inline
wstringbuffer *wstringbuffer::append(float number, uint16_t scale) {
	return (wstringbuffer *)appendFormatted(L"%.*f",
						scale,number);
}

inline
wstringbuffer *wstringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	return (wstringbuffer *)appendFormatted(L"%*.*f",
						precision,scale,number);
}

inline
wstringbuffer *wstringbuffer::append(double number) {
	return append(number,4);
}

inline
wstringbuffer *wstringbuffer::append(double number, uint16_t scale) {
	return (wstringbuffer *)appendFormatted(L"%.*f",scale,number);
}

inline
wstringbuffer *wstringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	return (wstringbuffer *)appendFormatted(L"%*.*f",
						precision,scale,number);
}

inline
ssize_t wstringbuffer::write(const char *string) {
	return write(string,charstring::length(string));
}

inline
ssize_t wstringbuffer::write(const char *string, size_t length) {
	wchar_t	*s=wcharstring::duplicate(string,length);
	ssize_t	retval=write(s,length);
	delete[] s;
	return retval;
}

inline
ssize_t wstringbuffer::write(char character) {
	return bytebuffer::write(wcharacter::duplicate(character));
}

inline
ssize_t wstringbuffer::write(const wchar_t *string) {
	return write(string,wcharstring::length(string));
}

inline
ssize_t wstringbuffer::write(const wchar_t *string, size_t length) {
	return bytebuffer::write(string,length);
}

inline
ssize_t wstringbuffer::write(wchar_t character) {
	return bytebuffer::write(character);
}

inline
ssize_t wstringbuffer::write(int16_t number) {
	return writeFormatted(L"%hd",number);
}

inline
ssize_t wstringbuffer::write(int32_t number) {
	return writeFormatted(L"%d",number);
}

inline
ssize_t wstringbuffer::write(int64_t number) {
	return writeFormatted(L"%lld",number);
}

inline
ssize_t wstringbuffer::write(uint16_t number) {
	return writeFormatted(L"%hd",number);
}

inline
ssize_t wstringbuffer::write(uint32_t number) {
	return writeFormatted(L"%d",number);
}

inline
ssize_t wstringbuffer::write(uint64_t number) {
	return bytebuffer::writeFormatted(L"%lld",number);
}

inline
ssize_t wstringbuffer::write(float number) {
	return write(number,4);
}

inline
ssize_t wstringbuffer::write(float number, uint16_t scale) {
	return writeFormatted(L"%.*f",scale,number);
}

inline
ssize_t wstringbuffer::write(float number, uint16_t precision, uint16_t scale) {
	return writeFormatted(L"%*.*f",precision,scale,number);
}

inline
ssize_t wstringbuffer::write(double number) {
	return write(number,4);
}

inline
ssize_t wstringbuffer::write(double number, uint16_t scale) {
	return writeFormatted(L"%.*f",scale,number);
}

inline
ssize_t wstringbuffer::write(double number, uint16_t precision, uint16_t scale) {
	return writeFormatted(L"%*.*f",precision,scale,number);
}

inline
void wstringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

inline
void wstringbuffer::truncate() {
	bytebuffer::truncate();
}

inline
bool wstringbuffer::supported() {
#ifdef RUDIMENTS_HAVE_WCHAR_H
	return true;
#else
	return false;
#endif
}
