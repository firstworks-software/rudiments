// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>

// NOTE: these methods are defined out-of-order from their declarations because
// some compilers (eg. gcc 2.7.x) prefer for inline methods to be defined before
// they are used.

inline
ssize_t filedescriptor::write(float number, int32_t sec,int32_t usec) {
	return bufferedWrite((const unsigned char *)&number,
					sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::write(double number, int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)&number,
					sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::write(unsigned char character,
					int32_t sec, int32_t usec) {
	return bufferedWrite(&character,sizeof(unsigned char),sec,usec);
}

inline
ssize_t filedescriptor::write(char character, int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)&character,
					sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::write(wchar_t character, int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)&character,
					sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(bool value, int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)&value,
					sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::write(const unsigned char *string, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedWrite(string,size,sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length,
						int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)string,length,sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length,
						int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)string,
					length*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const unsigned char *string,
						int32_t sec, int32_t usec) {
	return bufferedWrite(string,
			charstring::length((const char *)string),sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)string,
					charstring::length(string),sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string,
					int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)string,
			wcharstring::length(string)*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedWrite((const unsigned char *)buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::write(uint16_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(uint32_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(uint64_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(int16_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(int32_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(int64_t number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(float number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(double number) {
	return write(number,-1,-1);
}

inline
ssize_t filedescriptor::write(unsigned char character) {
	return write(character,-1,-1);
}

inline
ssize_t filedescriptor::write(char character) {
	return write(character,-1,-1);
}

inline
ssize_t filedescriptor::write(wchar_t character) {
	return write(character,-1,-1);
}

inline
ssize_t filedescriptor::write(bool value) {
	return write(value,-1,-1);
}

inline
ssize_t filedescriptor::write(const unsigned char *string) {
	return write(string,charstring::length((const char *)string),-1,-1);
}

inline
ssize_t filedescriptor::write(const char *string) {
	return write(string,charstring::length(string),-1,-1);
}

inline
ssize_t filedescriptor::write(const wchar_t *string) {
	return write(string,wcharstring::length(string),-1,-1);
}

inline
ssize_t filedescriptor::write(const unsigned char *string, size_t size) {
	return write(string,size,-1,-1);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length) {
	return write(string,length,-1,-1);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length) {
	return write(string,length,-1,-1);
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size) {
	return write(buffer,size,-1,-1);
}

inline
ssize_t filedescriptor::read(float *buffer, int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::read(double *buffer, int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, int32_t sec, int32_t usec) {
	return bufferedRead(buffer,sizeof(unsigned char),sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::read(bool *buffer, int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedRead(buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,
				size*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return bufferedRead((unsigned char *)buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(char **buffer, const char *terminator,
						int32_t sec, int32_t usec) {
	return input::read(buffer,terminator,0,'\0',sec,usec);
}

inline
ssize_t filedescriptor::read(char **buffer, const char *terminator,
				size_t maxbytes, int32_t sec, int32_t usec) {
	return input::read(buffer,terminator,maxbytes,'\0',sec,usec);
}

inline
ssize_t	filedescriptor::read(char **buffer,
				const char *terminator, size_t maxbytes,
				char escapechar, int32_t sec, int32_t usec) {
	return input::read(buffer,terminator,maxbytes,escapechar,sec,usec);
}

inline
ssize_t filedescriptor::read(uint16_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(uint32_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(uint64_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(int16_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(int32_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(int64_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(float *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(double *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(char *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(bool *buffer) {
	return read(buffer,-1,-1);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return read(buffer,size,-1,-1);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t length) {
	return read(buffer,length,-1,-1);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t length) {
	return read(buffer,length,-1,-1);
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size) {
	return read(buffer,size,-1,-1);
}

inline
ssize_t filedescriptor::read(char **buffer, const char *terminator) {
	return input::read(buffer,terminator,0,'\0',-1,-1);
}

inline
ssize_t filedescriptor::read(char **buffer,
				const char *terminator, size_t maxbytes) {
	return input::read(buffer,terminator,maxbytes,'\0',-1,-1);
}
