// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>


inline
ssize_t filedescriptor::write(float number) {
	return highLevelWrite((const byte_t *)&number,sizeof(float));
}

inline
ssize_t filedescriptor::write(double number) {
	return highLevelWrite((const byte_t *)&number,sizeof(double));
}

inline
ssize_t filedescriptor::write(byte_t character) {
	return highLevelWrite(&character,sizeof(byte_t));
}

inline
ssize_t filedescriptor::write(char character) {
	return highLevelWrite((const byte_t *)&character,sizeof(char));
}

inline
ssize_t filedescriptor::write(wchar_t character) {
	return highLevelWrite((const byte_t *)&character,sizeof(wchar_t));
}

inline
ssize_t filedescriptor::writeUcs2(ucs2_t character) {
	return highLevelWrite((const byte_t *)&character,sizeof(ucs2_t));
}

inline
ssize_t filedescriptor::write(bool value) {
	return highLevelWrite((const byte_t *)&value,sizeof(bool));
}

inline
ssize_t filedescriptor::write(const byte_t *string) {
	return highLevelWrite(string,
			charstring::getLength((const char *)string));
}

inline
ssize_t filedescriptor::write(const char *string) {
	return highLevelWrite((const byte_t *)string,
				charstring::getLength(string));
}

inline
ssize_t filedescriptor::write(const wchar_t *string) {
	return highLevelWrite((const byte_t *)string,
			wcharstring::getLength(string)*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::writeUcs2(const ucs2_t *string) {
	return highLevelWrite((const byte_t *)string,
			ucs2charstring::getLength(string)*sizeof(ucs2_t));
}

inline
ssize_t filedescriptor::write(const byte_t *string, size_t size) {
	return highLevelWrite((const byte_t *)string,size);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length) {
	return highLevelWrite((const byte_t *)string,length);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length) {
	return highLevelWrite((const byte_t *)string,length*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::writeUcs2(const ucs2_t *string, size_t length) {
	return highLevelWrite((const byte_t *)string,length*sizeof(ucs2_t));
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size) {
	return highLevelWrite((const byte_t *)buffer,size);
}
inline
ssize_t filedescriptor::write(float number, int32_t sec,int32_t usec) {
	return highLevelWrite((const byte_t *)&number,sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::write(double number, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)&number,sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::write(byte_t character, int32_t sec, int32_t usec) {
	return highLevelWrite(&character,sizeof(byte_t),sec,usec);
}

inline
ssize_t filedescriptor::write(char character, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)&character,
					sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::write(wchar_t character, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)&character,
					sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::writeUcs2(ucs2_t character, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)&character,
					sizeof(ucs2_t),sec,usec);
}

inline
ssize_t filedescriptor::write(bool value, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)&value,
					sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::write(const byte_t *string, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelWrite(string,size,sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,length,sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,
					length*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::writeUcs2(const ucs2_t *string, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,
					length*sizeof(ucs2_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const byte_t *string, int32_t sec, int32_t usec) {
	return highLevelWrite(string,
			charstring::getLength((const char *)string),sec,usec);
}

inline
ssize_t filedescriptor::write(const char *string, int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,
			charstring::getLength(string),sec,usec);
}

inline
ssize_t filedescriptor::write(const wchar_t *string,
					int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,
		wcharstring::getLength(string)*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::writeUcs2(const ucs2_t *string,
					int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)string,
		ucs2charstring::getLength(string)*sizeof(ucs2_t),sec,usec);
}

inline
ssize_t filedescriptor::write(const void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelWrite((const byte_t *)buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(float *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(float));
}

inline
ssize_t filedescriptor::read(double *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(double));
}

inline
ssize_t filedescriptor::read(byte_t *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(byte_t));
}

inline
ssize_t filedescriptor::read(char *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(char));
}

inline
ssize_t filedescriptor::read(wchar_t *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(wchar_t));
}

inline
ssize_t filedescriptor::readUcs2(ucs2_t *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(ucs2_t));
}

inline
ssize_t filedescriptor::read(bool *buffer) {
	return highLevelRead((byte_t *)buffer,sizeof(bool));
}

inline
ssize_t filedescriptor::read(byte_t *buffer, size_t size) {
	return highLevelRead(buffer,size);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t length) {
	return highLevelRead((byte_t *)buffer,length);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t length) {
	return highLevelRead((byte_t *)buffer,length*sizeof(wchar_t));
}

inline
ssize_t filedescriptor::readUcs2(ucs2_t *buffer, size_t length) {
	return highLevelRead((byte_t *)buffer,length*sizeof(ucs2_t));
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size) {
	return highLevelRead((byte_t *)buffer,size);
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
	return highLevelRead((byte_t *)buffer,sizeof(float),sec,usec);
}

inline
ssize_t filedescriptor::read(double *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,sizeof(double),sec,usec);
}

inline
ssize_t filedescriptor::read(byte_t *buffer, int32_t sec, int32_t usec) {
	return highLevelRead(buffer,sizeof(byte_t),sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,sizeof(char),sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::readUcs2(ucs2_t *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,sizeof(ucs2_t),sec,usec);
}

inline
ssize_t filedescriptor::read(bool *buffer, int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,sizeof(bool),sec,usec);
}

inline
ssize_t filedescriptor::read(byte_t *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelRead(buffer,size,sec,usec);
}

inline
ssize_t filedescriptor::read(char *buffer, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,length,sec,usec);
}

inline
ssize_t filedescriptor::read(wchar_t *buffer, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,
				length*sizeof(wchar_t),sec,usec);
}

inline
ssize_t filedescriptor::readUcs2(ucs2_t *buffer, size_t length,
						int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,
				length*sizeof(ucs2_t),sec,usec);
}

inline
ssize_t filedescriptor::read(void *buffer, size_t size,
						int32_t sec, int32_t usec) {
	return highLevelRead((byte_t *)buffer,size,sec,usec);
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
