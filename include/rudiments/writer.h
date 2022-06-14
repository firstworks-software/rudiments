// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WRITER_H
#define RUDIMENTS_WRITER_H

#include <rudiments/private/inttypes.h>

/** The writer class provides a set of write() methods, typically used by
 *  collections to write out representations of themselves. */
class writer {
	public:
		/** Writes "value" to standard output. */
		virtual void	write(const char *value) const;

		/** Writes "value" to standard output. */
		virtual void	write(const wchar_t *value) const;

		/** Writes "value" to standard output. */
		virtual void	write(char value) const;

		/** Writes "value" to standard output. */
		virtual void	write(wchar_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(int16_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(int32_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(int64_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(const unsigned char *value) const;

		/** Writes "value" to standard output. */
		virtual void	write(unsigned char value) const;

		/** Writes "value" to standard output. */
		virtual void	write(uint16_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(uint32_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(uint64_t value) const;

		/** Writes "value" to standard output. */
		virtual void	write(float value) const;

		/** Writes "value" to standard output. */
		virtual void	write(double value) const;

		/** Writes "value" to standard output. */
		virtual void	write(long double value) const;

		/** Writes "value" to standard output. */
		virtual void	write(const void *value) const;
};

#include <rudiments/private/writerinlines.h>

#endif
