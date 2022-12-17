// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ICONVERT_H
#define RUDIMENTS_ICONVERT_H

#include <rudiments/private/iconvertincludes.h>

/** The iconvert class implements character encoding converter. */
class RUDIMENTS_DLLSPEC iconvert : public object {
	public:

		/** Creates a new instance of the iconvert class. */
		iconvert();

		iconvert(iconvert &i);

		iconvert	&operator=(iconvert &i);

		/** Deletes this instance of the iconvert class. */
		virtual	~iconvert();

		void	setFromEncoding(const char *fromencoding);

		const char	*getFromEncoding();

		void	setToEncoding(const char *fromencoding);

		const char	*getToEncoding();

		void	setFromBuffer(const unsigned char *buffer);

		const unsigned char	*getFromBuffer();

		void	setFromBufferSize(size_t buffersize);

		size_t	getFromBufferSize();

		void	setToBuffer(unsigned char *buffer);

		unsigned char	*getToBuffer();

		void	setToBufferSize(size_t buffersize);

		size_t	getToBufferSize();

		bool	convert();

		const unsigned char	*getFromBufferPosition();

		size_t	getFromBufferRemaining();

		const unsigned char	*getToBufferPosition();

		size_t	getToBufferRemaining();

		bool	close();

		bool	reset();

		static	uint16_t	maxMultiByteSize();

		static	bool	convertNeedsMutex();

	#include <rudiments/private/iconvert.h>
};

#endif
