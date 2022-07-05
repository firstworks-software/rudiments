// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WRITER_H
#define RUDIMENTS_WRITER_H

#include <rudiments/private/inttypes.h>

/** The writer class provides a set of write() methods, typically used by
 *  collections to write out representations of themselves. */
class writer {
	public:
		/** Deletes this instance of the writer class. */
		virtual	~writer();

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const char *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(char *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const wchar_t *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(wchar_t *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(char value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(wchar_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(int16_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(int32_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(int64_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const unsigned char *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(unsigned char value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(uint16_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(uint32_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(uint64_t value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(float value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(double value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(long double value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const void *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(void *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const object *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(object *value) const;

		/** Writes "value" to standard output. */
		virtual ssize_t	write(const object &value) const;



		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, const char *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, char *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, const wchar_t *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, wchar_t *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, char value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, wchar_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, int16_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, int32_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, int64_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out,
					const unsigned char *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, unsigned char value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, uint16_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, uint32_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, uint64_t value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, float value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, double value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, long double value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, const void *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, void *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, const object *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, object *value) const;

		/** Writes "value" to "output". */
		virtual ssize_t	write(output *out, const object &value) const;
};

#include <rudiments/private/writerinlines.h>

#endif
