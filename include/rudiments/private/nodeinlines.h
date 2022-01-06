// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_NODE_H
#define RUDIMENTS_NODE_H

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.

inline
void node_print(const char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(const wchar_t *value) {
	stdoutput.printf(L"%s",value);
}

inline
void node_print(wchar_t *value) {
	stdoutput.printf(L"%s",value);
}

inline
void node_print(char value) {
	stdoutput.printf("%c",value);
}

inline
void node_print(wchar_t value) {
	stdoutput.printf(L"%c",value);
}

inline
void node_print(int16_t value) {
	stdoutput.printf("%hd",value);
}

inline
void node_print(int32_t value) {
	stdoutput.printf("%d",(int)value);
}

inline
void node_print(int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(long long)value);
	#else
		stdoutput.printf("%ld",(long)value);
	#endif
}

inline
void node_print(const unsigned char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(unsigned char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(unsigned char value) {
	stdoutput.printf("%c",value);
}

inline
void node_print(uint16_t value) {
	stdoutput.printf("%hd",value);
}

inline
void node_print(uint32_t value) {
	stdoutput.printf("%d",(unsigned int)value);
}

inline
void node_print(uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(unsigned long long)value);
	#else
		stdoutput.printf("%ld",(unsigned long)value);
	#endif
}

inline
void node_print(float value) {
	stdoutput.printf("%f",value);
}

inline
void node_print(double value) {
	stdoutput.printf("%f",value);
}

inline
void node_print(long double value) {
	stdoutput.printf("%Lf",value);
}

inline
void node_print(void *value) {
	stdoutput.printf("%08x",value);
}

inline
void node_delete_value(const char *value) {
}

inline
void node_delete_value(char *value) {
}

inline
void node_delete_value(const wchar_t *value) {
}

inline
void node_delete_value(wchar_t *value) {
}

inline
void node_delete_value(char value) {
}

inline
void node_delete_value(wchar_t value) {
}

inline
void node_delete_value(int16_t value) {
}

inline
void node_delete_value(int32_t value) {
}

inline
void node_delete_value(int64_t value) {
}

inline
void node_delete_value(const unsigned char *value) {
}

inline
void node_delete_value(unsigned char *value) {
}

inline
void node_delete_value(unsigned char value) {
}

inline
void node_delete_value(uint16_t value) {
}

inline
void node_delete_value(uint32_t value) {
}

inline
void node_delete_value(uint64_t value) {
}

inline
void node_delete_value(float value) {
}

inline
void node_delete_value(double value) {
}

inline
void node_delete_value(long double value) {
}

template <class valuetype>
inline
void node_delete_value(valuetype *value) {
	delete value;
}

inline
void node_delete_array_value(const char *value) {
	delete[] (char *)value;
}

inline
void node_delete_array_value(char *value) {
	delete[] value;
}

inline
void node_delete_array_value(const wchar_t *value) {
	delete[] (wchar_t *)value;
}

inline
void node_delete_array_value(wchar_t *value) {
	delete[] value;
}

inline
void node_delete_array_value(char value) {
}

inline
void node_delete_array_value(int16_t value) {
}

inline
void node_delete_array_value(int32_t value) {
}

inline
void node_delete_array_value(int64_t value) {
}

inline
void node_delete_array_value(const unsigned char *value) {
	delete[] (unsigned char *)value;
}

inline
void node_delete_array_value(unsigned char *value) {
	delete[] value;
}

inline
void node_delete_array_value(unsigned char value) {
}

inline
void node_delete_array_value(uint16_t value) {
}

inline
void node_delete_array_value(uint32_t value) {
}

inline
void node_delete_array_value(uint64_t value) {
}

inline
void node_delete_array_value(float value) {
}

inline
void node_delete_array_value(double value) {
}

inline
void node_delete_array_value(long double value) {
}

template <class valuetype>
inline
void node_delete_array_value(valuetype *value) {
	delete[] value;
}

inline
const char *node_duplicate_value(const char *value) {
	return value;
}

inline
char *node_duplicate_value(char *value) {
	return value;
}

inline
const wchar_t *node_duplicate_value(const wchar_t *value) {
	return value;
}

inline
wchar_t *node_duplicate_value(wchar_t *value) {
	return value;
}

inline
char node_duplicate_value(char value) {
	return value;
}

inline
wchar_t node_duplicate_value(wchar_t value) {
	return value;
}

inline
int16_t node_duplicate_value(int16_t value) {
	return value;
}

inline
int32_t node_duplicate_value(int32_t value) {
	return value;
}

inline
int64_t node_duplicate_value(int64_t value) {
	return value;
}

inline
const unsigned char *node_duplicate_value(const unsigned char *value) {
	return value;
}

inline
unsigned char *node_duplicate_value(unsigned char *value) {
	return value;
}

inline
unsigned char node_duplicate_value(unsigned char value) {
	return value;
}

inline
uint16_t node_duplicate_value(uint16_t value) {
	return value;
}

inline
uint32_t node_duplicate_value(uint32_t value) {
	return value;
}

inline
uint64_t node_duplicate_value(uint64_t value) {
	return value;
}

inline
float node_duplicate_value(float value) {
	return value;
}

inline
double node_duplicate_value(double value) {
	return value;
}

inline
long double node_duplicate_value(long double value) {
	return value;
}

template <class valuetype>
inline
valuetype *node_duplicate_value(valuetype *value) {
	return new valuetype(*value);
}

inline
const char *node_duplicate_array_value(const char *value) {
	return charstring::duplicate(value);
}

inline
char *node_duplicate_array_value(char *value) {
	return charstring::duplicate(value);
}

inline
const wchar_t *node_duplicate_array_value(const wchar_t *value) {
	return wcharstring::duplicate(value);
}

inline
wchar_t *node_duplicate_array_value(wchar_t *value) {
	return wcharstring::duplicate(value);
}

inline
char node_duplicate_array_value(char value) {
	return value;
}

inline
wchar_t node_duplicate_array_value(wchar_t value) {
	return value;
}

inline
int16_t node_duplicate_array_value(int16_t value) {
	return value;
}

inline
int32_t node_duplicate_array_value(int32_t value) {
	return value;
}

inline
int64_t node_duplicate_array_value(int64_t value) {
	return value;
}

inline
const unsigned char *node_duplicate_array_value(const unsigned char *value) {
	// this isn't a true duplicate, but it's
	// the best we can do without a size
	return (const unsigned char *)charstring::duplicate((const char *)value);
}

inline
unsigned char *node_duplicate_array_value(unsigned char *value) {
	// this isn't a true duplicate, but it's
	// the best we can do without a size
	return (unsigned char *)charstring::duplicate((char *)value);
}

inline
unsigned char node_duplicate_array_value(unsigned char value) {
	return value;
}

inline
uint16_t node_duplicate_array_value(uint16_t value) {
	return value;
}

inline
uint32_t node_duplicate_array_value(uint32_t value) {
	return value;
}

inline
uint64_t node_duplicate_array_value(uint64_t value) {
	return value;
}

inline
float node_duplicate_array_value(float value) {
	return value;
}

inline
double node_duplicate_array_value(double value) {
	return value;
}

inline
long double node_duplicate_array_value(long double value) {
	return value;
}

template <class valuetype>
inline
valuetype *node_duplicate_array_value(valuetype *value) {
	// no way to do this without a length
	return NULL;
}

#endif
