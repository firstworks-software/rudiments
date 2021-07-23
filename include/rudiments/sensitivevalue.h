// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SENSITIVEVALUE_H
#define RUDIMENTS_SENSITIVEVALUE_H

#include <rudiments/private/sensitivevalueincludes.h>

/** The sensitivevalue class provides methods for reading sensitive values from
 *  a string, which may optionally redirect to a file. */
class RUDIMENTS_DLLSPEC sensitivevalue {
	public:
		sensitivevalue();
		~sensitivevalue();

		void	setRedirectStart(const char *delimiter);
		void	setRedirectStart(const char *delimiter, uint64_t len);
		void	setRedirectEnd(const char *delimiter);
		void	setRedirectEnd(const char *delimiter, uint64_t len);
		void	setVerbatimIsHex(bool verbatimishex);
		void	setFileIsHex(bool fileishex);
		void	setPath(const char *path);
		void	setPath(const char *path, uint64_t len);
		void	setBinaryExtension(const char *ext);
		void	setBinaryExtension(const char *ext, uint64_t len);
		void	setHexExtension(const char *ext);
		void	setHexExtension(const char *ext, uint64_t len);

		const char	*getRedirectStart();
		uint64_t	getRedirectStartLength();
		const char	*getRedirectEnd();
		uint64_t	getRedirectEndLength();
		bool		getVerbatimIsHex();
		bool		getFileIsHex();
		const char	*getPath();
		uint64_t	getPathLength();
		const char	*getBinaryExtension();
		uint64_t	getBinaryExtensionLength();
		const char	*getHexExtension();
		uint64_t	getHexExtensionLength();

		void	parse(const char *in);
		void	parse(const char *in, uint64_t inlen);
		
		const unsigned char	*getValue();
		uint64_t	getValueSize();

		const char	*getTextValue();
		uint64_t	getTextValueLength();


	#include <rudiments/private/sensitivevalue.h>
};

#endif
