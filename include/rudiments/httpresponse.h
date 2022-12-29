// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPRESPONSE_H
#define RUDIMENTS_HTTPRESPONSE_H

#include <rudiments/private/httpresponseincludes.h>

/** The httpresponse class provides methods for generating http responses. */

class RUDIMENTS_DLLSPEC httpresponse : public output {
	public:
		httpresponse(httpserverapi *sapi);
		virtual	~httpresponse();

		/** Send HTTP header:
		 *  Content-type: text/html
		 *  followed by two carriage-returns. */
		void	textHtml();

		/** Send HTTP header:
		 *  Content-type: text/plain
		 *  followed by two carriage-returns. */
		void	textPlain();

		/** Send HTTP header:
		 *  Content-type: "type"/"subtype"
		 *
		 *  Does not send carriage-returns. */
		void	contentType(const char *type, const char *subtype);

		/** Send HTTP header:
		 *  Content-type: "type"/"subtype"; charset="charset"
		 *
		 *  If charset is NULL or an empty string then it will be
		 *  omitted.
		 *
		 *  Does not send carriage-returns. */
		void	contentType(const char *type, const char *subtype,
							const char *charset);

		/** Send HTTP header:
		 *  Content-type: "type"/"subtype"; charset="charset";
		 *  boundary="boundary"
		 *
		 *  If either of charset or boundary are NULL or empty strings
		 *  then it will be omitted.
		 *
		 *  Does not send carriage-returns. */
		void	contentType(const char *type, const char *subtype,
							const char *charset,
							const char *boundary);

		/** Send carriage-return. */
		void	cr();
		
		/** Send Set-Cookie header. */
		void	setCookie(const char *name, const char *value,
					const char *domain, const char *path,
					const char *expires, bool secure);

		/** Returns a boundary string that is useful in building
		 *  multipart responses. */
		const char	*boundaryString();

		/** Sends a starting boundary string. */
		void	multiPartBoundary(output *out);

		/** Sends a ending boundary string. */
		void	multiPartEnd(output *out);

		/** Sends an http status header. */
		httpresponse	*status(const char *status);

		/** Sends an http header. */
		httpresponse	*header(const char *header);

		/** Sends an http header. */
		httpresponse	*header(const char *header,
						const char *value);

		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string);
		ssize_t	write(const char *string, size_t length);
		ssize_t	write(char character);
		ssize_t	write(const wchar_t *string);
		ssize_t	write(const wchar_t *string, size_t length);
		ssize_t	write(wchar_t character);
		ssize_t	writeUcs2(const ucs2_t *string);
		ssize_t	writeUcs2(const ucs2_t *string, size_t length);
		ssize_t	writeUcs2(ucs2_t character);
		ssize_t	write(int16_t number);
		ssize_t	write(int32_t number);
		ssize_t	write(int64_t number);
		ssize_t	write(byte_t character);
		ssize_t	write(uint16_t number);
		ssize_t	write(uint32_t number);
		ssize_t	write(uint64_t number);
		ssize_t	write(float number);
		ssize_t	write(double number);

		/** Writes "filebuffer" to the browser.*/
		ssize_t	write(file *filebuffer);

	#include <rudiments/private/httpresponse.h>

};

#endif
