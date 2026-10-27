// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/wcharstring.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharacter.h>

inline
wstringbuffer::wstringbuffer() : bytebuffer() {
}

inline
wstringbuffer::wstringbuffer(size_t initialsize) : bytebuffer(initialsize) {
}

inline
wstringbuffer::wstringbuffer(wchar_t *initialcontents, size_t initialsize) :
		bytebuffer((byte_t *)initialcontents,
				(initialsize>((size_t)-1>>1)/sizeof(wchar_t))?
				0:initialsize*sizeof(wchar_t)) {
}

inline
wstringbuffer::wstringbuffer(wstringbuffer &s) : bytebuffer(s) {
}

inline
wstringbuffer &wstringbuffer::operator=(wstringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

inline
wstringbuffer::~wstringbuffer() {
}

inline
off64_t wstringbuffer::setPositionRelativeToBeginning(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t,
	// or a position before the start
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(wchar_t) || offset<0) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToBeginning(
			offset*(off64_t)sizeof(wchar_t))/
			(off64_t)sizeof(wchar_t);
}

inline
off64_t wstringbuffer::setPositionRelativeToCurrent(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(wchar_t) ||
		offset<(-max-1)/(off64_t)sizeof(wchar_t)) {
		return -1;
	}

	// refuse a position before the start
	if (offset<0 &&
		(uint64_t)(-offset)*sizeof(wchar_t)>(uint64_t)_position()) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToCurrent(
			offset*(off64_t)sizeof(wchar_t))/
			(off64_t)sizeof(wchar_t);
}

inline
off64_t wstringbuffer::setPositionRelativeToEnd(off64_t offset) {

	// refuse an offset whose byte count does not fit an off64_t
	const off64_t	max=(((off64_t)1<<(sizeof(off64_t)*8-2))-1)*2+1;
	if (offset>max/(off64_t)sizeof(wchar_t) ||
		offset<(-max-1)/(off64_t)sizeof(wchar_t)) {
		return -1;
	}

	// refuse a position before the start
	if (offset<0 &&
		(uint64_t)(-offset)*sizeof(wchar_t)>(uint64_t)_size()) {
		return -1;
	}

	// the byte position divided down to characters
	return bytebuffer::setPositionRelativeToEnd(
			offset*(off64_t)sizeof(wchar_t))/
			(off64_t)sizeof(wchar_t);
}

inline
const wchar_t *wstringbuffer::getString() {
	bytebuffer::append(L'\0');
	const wchar_t	*retval=(const wchar_t *)getBuffer();
	bytebuffer::setPositionRelativeToBeginning(
					_position()-sizeof(wchar_t));
	_size(_size()-sizeof(wchar_t));
	return retval;
}

inline
size_t wstringbuffer::getStringLength() {
	return wcharstring::getLength(getString());
}

inline
wchar_t *wstringbuffer::detachString() {
	bytebuffer::append(L'\0');
	return (wchar_t *)detachBuffer();
}

inline
off64_t wstringbuffer::getPosition() {
	return bytebuffer::getPosition()/sizeof(wchar_t);
}

inline
bool wstringbuffer::clear() {
	return bytebuffer::clear();
}

inline
bool wstringbuffer::clear(size_t initialsize) {
	return bytebuffer::clear(initialsize);
}

inline
bool wstringbuffer::clear(wchar_t *initialcontents, size_t initialsize) {
	return bytebuffer::clear((byte_t *)initialcontents,
				(initialsize>((size_t)-1>>1)/sizeof(wchar_t))?
				0:initialsize*sizeof(wchar_t));
}

inline
wstringbuffer *wstringbuffer::append(const wchar_t *string) {
	return append(string,wcharstring::getLength(string));
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
	printf(L"%.*f",scale,number);
	return this;
}

inline
wstringbuffer *wstringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	printf(L"%*.*f",precision,scale,number);
	return this;
}

inline
wstringbuffer *wstringbuffer::append(double number) {
	return append(number,4);
}

inline
wstringbuffer *wstringbuffer::append(double number, uint16_t scale) {
	printf(L"%.*f",scale,number);
	return this;
}

inline
wstringbuffer *wstringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	printf(L"%*.*f",precision,scale,number);
	return this;
}

inline
ssize_t wstringbuffer::write(const wchar_t *string) {
	return write(string,wcharstring::getLength(string));
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
	if (wcharstring::supportsPrintf()) {
		return printf(L"%hd",number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(int32_t number) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%d",number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(int64_t number) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%lld",(long long)number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(uint16_t number) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%hu",(unsigned int)number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(uint32_t number) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%u",(unsigned int)number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(uint64_t number) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%llu",(unsigned long long)number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(float number) {
	if (wcharstring::supportsPrintf()) {
		return write(number,4);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(float number, uint16_t scale) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%.*f",scale,number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number,scale);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(float number, uint16_t precision, uint16_t scale) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%*.*f",precision,scale,number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number,precision,scale);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(double number) {
	if (wcharstring::supportsPrintf()) {
		return write(number,4);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(double number, uint16_t scale) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%.*f",scale,number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number,scale);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
ssize_t wstringbuffer::write(double number,
				 uint16_t precision, uint16_t scale) {
	if (wcharstring::supportsPrintf()) {
		return printf(L"%*.*f",precision,scale,number);
	} else {
		wchar_t	*s=wcharstring::parseNumber(number,precision,scale);
		ssize_t	retval=write(s);
		delete[] s;
		return retval;
	}
}

inline
void wstringbuffer::truncate(size_t pos) {

	// a position too large for a byte count truncates nothing
	if (pos>(size_t)-1/sizeof(wchar_t)) {
		return;
	}
	bytebuffer::truncate(pos*sizeof(wchar_t));
}

inline
void wstringbuffer::truncate() {

	// The inherited truncate() would call the virtual truncate(size_t),
	// which takes characters, with a byte count.
	bytebuffer::truncate(_position());
}
