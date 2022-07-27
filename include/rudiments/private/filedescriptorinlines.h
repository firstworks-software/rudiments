// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>


inline
ssize_t filedescriptor::write(float number) {
	return highLevelWrite((const unsigned char *)&number,sizeof(float));
}

inline
ssize_t filedescriptor::write(double number) {
	return highLevelWrite((const unsigned char *)&number,sizeof(double));
}

inline
ssize_t filedescriptor::write(unsigned char character) {
	return highLevelWrite(&character,sizeof(unsigned char));
}

inline
ssize_t filedescriptor::write(char character) {
	return highLevelWrite((const unsigned char *)&character,
							sizeof(char));
}

inline
ssize_t filedescriptor::write(wchar_t character) {
	return highLevelWrite((const unsigned char *)&character,
							sizeof(wchar_t));
}

inline
ssize_t filedescriptor::write(bool value) {
	return highLevelWrite((const unsigned char *)&value,sizeof(bool));
}

inline
ssize_t filedescriptor::write(const unsigned char *string) {
	return highLevelWrite(string,charstring::length((const char *)string));
}

inline
ssize_t filedescriptor::write(const char *string) {
	return highLevelWrite((const unsigned char *)string,
					charstring::length(string));
}

inline
ssize_t filedescriptor::write(const wchar_t *string) {
	return highLevelWrite((const unsigned char *)string,
				wcharstring::length(string)*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::write(const unsigned char *string, size_t size) {
	return highLevelWrite((const unsigned char *)string,size);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length) {
	return highLevelWrite((const unsigned char *)string,length);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length) {
	return highLevelWrite((const unsigned char *)string,
					length*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size) {
	return highLevelWrite((const unsigned char *)buffer,size);
}
inline
ssize_t filedescriptor::write(float number, int32_t sec,int32_t usec) {
	return highLevelWrite((const unsigned char *)&number,
					sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::write(double number, int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)&number,
					sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::write(unsigned char character,
					int32_t sec, int32_t usec) {
	return highLevelWrite(&character,sizeof(unsigned char),sec,usec);
}

inline
ssize_t filedescriptor::write(char character, int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)&character,
					sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::write(wchar_t character, int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)&character,
					sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(bool value, int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)&value,
					sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::write(const unsigned char *string, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelWrite(string,size,sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)string,length,sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)string,
					length*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const unsigned char *string,
						int32_t sec, int32_t usec) {
	return highLevelWrite(string,
			charstring::length((const char *)string),sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)string,
					charstring::length(string),sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string,
					int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)string,
			wcharstring::length(string)*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const unsigned char *)buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(float *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(float));
}

inline
ssize_t filedescriptor::read(double *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(double));
}

inline
ssize_t filedescriptor::read(unsigned char *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(unsigned char));
}

inline
ssize_t filedescriptor::read(char *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(char));
}

inline
ssize_t filedescriptor::read(wchar_t *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(wchar_t));
}

inline
ssize_t filedescriptor::read(bool *buffer) {
	return highLevelRead((unsigned char *)buffer,sizeof(bool));
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return highLevelRead(buffer,size);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t length) {
	return highLevelRead((unsigned char *)buffer,length);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t length) {
	return highLevelRead((unsigned char *)buffer,length*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size) {
	return highLevelRead((unsigned char *)buffer,size);
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
inline
ssize_t filedescriptor::read(float *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::read(double *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, int32_t sec, int32_t usec) {
	return highLevelRead(buffer,sizeof(unsigned char),sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::read(bool *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::read(unsigned char *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelRead(buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,length,sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,
				length*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelRead((unsigned char *)buffer,size,sec,usec);
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
