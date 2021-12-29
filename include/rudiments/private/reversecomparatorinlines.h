// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

inline
reversecomparator::~reversecomparator() {
}

inline
int32_t reversecomparator::compare(char *value1, char *value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(const char *value1, const char *value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(unsigned char *value1, unsigned char *value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(const unsigned char *value1,
				const unsigned char *value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(char value1, char value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(int16_t value1, int16_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(int32_t value1, int32_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(int64_t value1, int64_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(unsigned char value1, unsigned char value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(uint16_t value1, uint16_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(uint32_t value1, uint32_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(uint64_t value1, uint64_t value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(float value1, float value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(double value1, double value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(long double value1, long double value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(object *value1, object *value2) {
	return comparator::compare(value1,value2)*-1;
}

inline
int32_t reversecomparator::compare(void *value1, void *value2) {
	return comparator::compare(value1,value2)*-1;
}
