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

		/*template <class valuetype>
		void	write(valuetype &value) const;*/

		template <class valuetype>
		void	write(const valuetype &value) const;



		/** Writes "value" to "output". */
		virtual void	write(output *out, const char *value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, const wchar_t *value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, char value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, wchar_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, int16_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, int32_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, int64_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out,
					const unsigned char *value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, unsigned char value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, uint16_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, uint32_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, uint64_t value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, float value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, double value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, long double value) const;

		/** Writes "value" to "output". */
		virtual void	write(output *out, const void *value) const;

		/*template <class valuetype>
		void	write(output *out, valuetype &value) const;*/

		template <class valuetype>
		void	write(output *out, const valuetype &value) const;
};

#include <rudiments/private/writerinlines.h>

#endif
