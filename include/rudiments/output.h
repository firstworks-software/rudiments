// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OUTPUT_H
#define RUDIMENTS_OUTPUT_H

#include <rudiments/private/outputincludes.h>

class RUDIMENTS_DLLSPEC output {
	public:
		virtual ~output();

		virtual	ssize_t	write(const unsigned char *string,
							size_t size)=0;
		virtual	ssize_t	write(const char *string)=0;
		virtual	ssize_t	write(const char *string, size_t size)=0;
		virtual	ssize_t	write(char character)=0;
		virtual	ssize_t	write(int16_t number)=0;
		virtual	ssize_t	write(int32_t number)=0;
		virtual	ssize_t	write(int64_t number)=0;
		virtual	ssize_t	write(unsigned char character)=0;
		virtual	ssize_t	write(uint16_t number)=0;
		virtual	ssize_t	write(uint32_t number)=0;
		virtual	ssize_t	write(uint64_t number)=0;
		virtual	ssize_t	write(float number)=0;
		virtual	ssize_t	write(double number)=0;

		virtual	ssize_t	write(const unsigned char *string, size_t size,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const char *string,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const char *string, size_t size,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(char character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int16_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int32_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int64_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(unsigned char character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint16_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint32_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint64_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(float number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(double number,
						int32_t sec, int32_t usec);
};

#endif
