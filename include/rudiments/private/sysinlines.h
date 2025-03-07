// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
//#include <limits.h>

inline
size_t sys::getMinSizeT() {
	return 0;
}

inline
size_t sys::getMaxSizeT() {
	return (size_t)-1;
}

inline
ssize_t sys::getMinSSizeT() {
	size_t	s=(size_t)-1;
	s=s/2*-1-1;
	return (ssize_t)s;
}

inline
ssize_t sys::getMaxSSizeT() {
	size_t	s=(size_t)-1;
	s/=2;
	return (ssize_t)s;
}

inline
off64_t sys::getMinOff64T() {
	switch (sizeof(off64_t)) {
		case 1:
			{
			uint8_t		s=(uint8_t)-1;
			s=s/2*-1-1;
			return (off64_t)s;
			}
		case 2:
			{
			uint16_t	s=(uint16_t)-1;
			s=s/2*-1-1;
			return (off64_t)s;
			}
		case 4:
			{
			uint32_t	s=(uint32_t)-1;
			s=s/2*-1-1;
			return (off64_t)s;
			}
		default:
			{
			uint64_t	s=(uint64_t)-1;
			s=s/2*-1-1;
			return (off64_t)s;
			}
	}
}

inline
off64_t sys::getMaxOff64T() {
	switch (sizeof(off64_t)) {
		case 1:
			{
			uint8_t		s=(uint8_t)-1;
			s/=2;
			return (off64_t)s;
			}
		case 2:
			{
			uint16_t	s=(uint16_t)-1;
			s/=2;
			return (off64_t)s;
			}
		case 4:
			{
			uint32_t	s=(uint32_t)-1;
			s/=2;
			return (off64_t)s;
			}
		default:
			{
			uint64_t	s=(uint64_t)-1;
			s/=2;
			return (off64_t)s;
			}
	}
}
