// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/input.h>

input::~input() {
}

ssize_t input::read(unsigned char *string, size_t size,
						int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(char *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(char *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(wchar_t *string, size_t size, int32_t sec, int32_t usec) {
	return read(string,size);
}

ssize_t input::read(wchar_t *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(int16_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(int32_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(int64_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(unsigned char *character, int32_t sec, int32_t usec) {
	return read(character);
}

ssize_t input::read(uint16_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(uint32_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(uint64_t *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(float *number, int32_t sec, int32_t usec) {
	return read(number);
}

ssize_t input::read(double *number, int32_t sec, int32_t usec) {
	return read(number);
}
