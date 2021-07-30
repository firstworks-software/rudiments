// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SENSITIVEVALUE_H
#define RUDIMENTS_SENSITIVEVALUE_H

#include <rudiments/private/sensitivevalueincludes.h>

/** The sensitivevalue class provides methods for reading sensitive values from
 *  a string, which may optionally redirect to a file. */

enum sensitivevalue_format_t {
	FORMAT_TEXT=0,
	FORMAT_BINARY,
	FORMAT_HEX
};

class RUDIMENTS_DLLSPEC sensitivevalue {
	public:
		sensitivevalue();
		~sensitivevalue();

		void	setRedirectStart(const char *delimiter);
		void	setRedirectStart(const char *delimiter, uint64_t len);

		void	setRedirectEnd(const char *delimiter);
		void	setRedirectEnd(const char *delimiter, uint64_t len);

		void	setVerbatimFormat(sensitivevalue_format_t format);
		void	setFileFormat(sensitivevalue_format_t format);

		void	setChompTextFile(bool chomptextfile);

		void	setPath(const char *path);
		void	setPath(const char *path, uint64_t len);

		void	setTextExtension(const char *ext);
		void	setTextExtension(const char *ext, uint64_t len);

		void	setBinaryExtension(const char *ext);
		void	setBinaryExtension(const char *ext, uint64_t len);

		void	setHexExtension(const char *ext);
		void	setHexExtension(const char *ext, uint64_t len);

		const char	*getRedirectStart();
		uint64_t	getRedirectStartLength();

		const char	*getRedirectEnd();
		uint64_t	getRedirectEndLength();

		sensitivevalue_format_t	getVerbatimFormat();
		sensitivevalue_format_t	getFileFormat();

		bool	getChompTextFile();

		const char	*getPath();
		uint64_t	getPathLength();

		const char	*getTextExtension();
		uint64_t	getTextExtensionLength();

		const char	*getBinaryExtension();
		uint64_t	getBinaryExtensionLength();

		const char	*getHexExtension();
		uint64_t	getHexExtensionLength();

		void	parse(const char *in);
		void	parse(const char *in, uint64_t inlen);
		
		const unsigned char	*getValue();
		unsigned char	*detachValue();
		uint64_t	getValueSize();

		const char	*getTextValue();
		char		*detachTextValue();
		uint64_t	getTextValueLength();

		void	clear();


	#include <rudiments/private/sensitivevalue.h>
};

#endif
