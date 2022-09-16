// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/output.h>

output::output() {
}

output::~output() {
}

ssize_t output::write(const unsigned char *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(const char *string, int32_t sec, int32_t usec) {
	return write(string);
}

ssize_t output::write(const char *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(char character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::write(const wchar_t *string, int32_t sec, int32_t usec) {
	return write(string);
}

ssize_t output::write(const wchar_t *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(wchar_t character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::write(int16_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(int32_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(int64_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(unsigned char character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::write(uint16_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(uint32_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(uint64_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(float number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(double number, int32_t sec, int32_t usec) {
	return write(number);
}
