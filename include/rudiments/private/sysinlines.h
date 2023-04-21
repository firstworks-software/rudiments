// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>

inline
size_t sys::getMinSizeT() {
	return 0;
}

inline
size_t sys::getMaxSizeT() {
	return -1;
}

inline
ssize_t sys::getMinSSizeT() {
	int64_t	retval=-2;
	uint8_t	exp=(sizeof(ssize_t)*8)-1;
	for (uint8_t i=1; i<exp; i++) {
		retval*=2;
	}
	return retval;
}

inline
ssize_t sys::getMaxSSizeT() {
	int64_t	retval=2;
	uint8_t	exp=(sizeof(ssize_t)*8)-1;
	for (uint8_t i=1; i<exp; i++) {
		retval*=2;
	}
	retval--;
	return retval;
}

inline
off64_t sys::getMinOff64T() {
	int64_t	retval=-2;
	uint8_t	exp=(sizeof(off64_t)*8)-1;
	for (uint8_t i=1; i<exp; i++) {
		retval*=2;
	}
	return retval;
}

inline
off64_t sys::getMaxOff64T() {
	int64_t	retval=2;
	uint8_t	exp=(sizeof(off64_t)*8)-1;
	for (uint8_t i=1; i<exp; i++) {
		retval*=2;
	}
	retval--;
	return retval;
}
