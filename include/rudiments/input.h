// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INPUT_H
#define RUDIMENTS_INPUT_H

#include <rudiments/private/inputincludes.h>

class RUDIMENTS_DLLSPEC input : virtual public object {
	public:
		virtual	~input();

		virtual	ssize_t	read(unsigned char *buffer, size_t size)=0;
		virtual	ssize_t	read(char *buffer, size_t length)=0;
		virtual	ssize_t	read(char *character)=0;
		virtual	ssize_t	read(wchar_t *buffer, size_t length)=0;
		virtual	ssize_t	read(wchar_t *character)=0;
		virtual	ssize_t	read(int16_t *number)=0;
		virtual	ssize_t	read(int32_t *number)=0;
		virtual	ssize_t	read(int64_t *number)=0;
		virtual	ssize_t	read(unsigned char *character)=0;
		virtual	ssize_t	read(uint16_t *number)=0;
		virtual	ssize_t	read(uint32_t *number)=0;
		virtual	ssize_t	read(uint64_t *number)=0;
		virtual	ssize_t	read(float *number)=0;
		virtual	ssize_t	read(double *number)=0;

		virtual	ssize_t	read(unsigned char *buffer, size_t size,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char *buffer, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(wchar_t *buffer, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(wchar_t *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int16_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int32_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int64_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(unsigned char *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint16_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint32_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint64_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(float *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(double *number,
						int32_t sec, int32_t usec);
};

#endif
