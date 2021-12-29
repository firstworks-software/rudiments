// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMPARATOR_H
#define RUDIMENTS_COMPARATOR_H

#include <rudiments/private/inttypes.h>

/** The comparator class provides a set of compare() methods, typically used by
 *  collections to find and sort nodes. */
class comparator {
	public:
		/** Deletes this instance of the comparator class. */
		virtual	~comparator();

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(char *value1, char *value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(const char *value1, const char *value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(unsigned char *value1,
						unsigned char *value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(const unsigned char *value1,
						const unsigned char *value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(char value1, char value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(int16_t value1, int16_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(int32_t value1, int32_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(int64_t value1, int64_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(unsigned char value1,
						unsigned char value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(uint16_t value1, uint16_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(uint32_t value1, uint32_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(uint64_t value1, uint64_t value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(float value1, float value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(double value1, double value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(long double value1, long double value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(object *value1, object *value2);

		/** Returns an integer less than, equal to, or greater than 0
		 *  if value1 is, respectively, less than, equal to, or greater
		 *  than value2. */
		virtual	int32_t compare(void *value1, void *value2);
};

#include <rudiments/private/comparatorinlines.h>

#endif
