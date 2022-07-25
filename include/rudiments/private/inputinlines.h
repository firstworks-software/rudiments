// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

// NOTE: these methods are defined out-of-order from their declarations because
// some compilers (eg. gcc 2.7.x) prefer for inline methods to be defined before
// they are used.

inline
ssize_t input::read(char **buffer, const char *terminator,
						int32_t sec, int32_t usec) {
	return read(buffer,terminator,0,'\0',sec,usec);
}

inline
ssize_t input::read(char **buffer, const char *terminator,
				size_t maxbytes, int32_t sec, int32_t usec) {
	return read(buffer,terminator,maxbytes,'\0',sec,usec);
}

inline
ssize_t input::read(char **buffer, const char *terminator) {
	return read(buffer,terminator,0,'\0',-1,-1);
}

inline
ssize_t input::read(char **buffer, const char *terminator, size_t maxbytes) {
	return read(buffer,terminator,maxbytes,'\0',-1,-1);
}
