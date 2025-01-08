// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPRESPONSE_H
#define RUDIMENTS_HTTPRESPONSE_H

#include <rudiments/private/httpresponseincludes.h>

/** The httpresponse class provides methods for generating http responses. */

class RUDIMENTS_DLLSPEC httpresponse : public output {
	public:
		/** Creates an instance of the httpresponse class. */
		httpresponse(httpserverapi *sapi);

		/** Deletes this instance of the httpresponse class. */
		virtual	~httpresponse();

		/** Writes HTTP header:
		 *  Content-type: text/html
		 *  followed by two carriage-returns to the browser. */
		void	writeTextHtmlHeader();

		/** Writes HTTP header:
		 *  Content-type: text/plain
		 *  followed by two carriage-returns to the browser. */
		void	writeTextPlainHeader();

		/** Writes HTTP header:
		 *  Content-type: "type"/"subtype"
		 *  to the browser.
		 *
		 *  Does not write carriage-returns. */
		void	writeContentTypeHeader(const char *type,
						const char *subtype);

		/** Writes HTTP header:
		 *  Content-type: "type"/"subtype"; charset="charset"
		 *  to the browser.
		 *
		 *  If charset is NULL or an empty string then it will be
		 *  omitted.
		 *
		 *  Does not write carriage-returns. */
		void	writeContentTypeHeader(const char *type,
						const char *subtype,
						const char *charset);

		/** Writes HTTP header:
		 *  Content-type: "type"/"subtype"; charset="charset";
		 *  boundary="boundary"
		 *  to the browser.
		 *
		 *  If either of charset or boundary are NULL or empty strings
		 *  then it will be omitted.
		 *
		 *  Does not write carriage-returns. */
		void	writeContentTypeHeader(const char *type,
						const char *subtype,
						const char *charset,
						const char *boundary);

		/** Writes carriage-return and line-feed (/r/n) to the
		 *  browser. */
		void	writeCrLf();
		
		/** Writes Set-Cookie header to the browser. */
		void	writeSetCookie(const char *name, const char *value,
					const char *domain, const char *path,
					const char *expires, bool secure);

		/** Returns a boundary string that is useful in building
		 *  multipart responses. */
		const char	*getBoundaryString();

		/** Writes a multipart boundary string. */
		void	writeMultiPartBoundary(output *out);

		/** Writes a final multipart boundary string. */
		void	writeFinalMultiPartBoundary(output *out);

		/** Writes an http status header. */
		httpresponse	*writeStatusHeader(const char *status);

		/** Writes an http header. */
		httpresponse	*writeHeader(const char *header);

		/** Writes an http header. */
		httpresponse	*writeHeader(const char *header,
						const char *value);

		/** Writes "size" bytes of "string" to the browser.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const byte_t *string, size_t size);

		/** Writes NULL-terminated "string" to the browser.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const char *string);

		/** Writes "length" characters of "string" to the browser.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.  Note that it is
 		 *  possible to write beyond the string's NULL terminator using
 		 *  this method.  */
		ssize_t	write(const char *string, size_t length);

		/** Writes "character" to the browser.  Returns the number of
 		 *  bytes that were successfully written or RESULT_ERROR if an
 		 *  error occurred. */
		ssize_t	write(char character);

		/** Writes NULL-terminated "string" to the browser.  Returns
		 *  the number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(const wchar_t *string);

		/** Writes "length" characters of "string" to the browser.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred.  Note that it is
		 *  possible to write beyond the string's NULL terminator using
		 *  this method.  */
		ssize_t	write(const wchar_t *string, size_t length);

		/** Writes "character" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(wchar_t character);

		/** Writes NULL-terminated "string" to the browser.  Returns
		 *  the number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	writeUcs2(const ucs2_t *string);

		/** Writes "length" characters of "string" to the browser.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred.  Note that it is
		 *  possible to write beyond the string's NULL terminator using
		 *  this method.  */
		ssize_t	writeUcs2(const ucs2_t *string, size_t length);

		/** Writes "character" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	writeUcs2(ucs2_t character);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(int16_t number);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(int32_t number);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(int64_t number);

		/** Writes "character" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(byte_t character);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(uint16_t number);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(uint32_t number);

		/** Writes "number" to the browser.  Returns the number of
		 *   bytes that were successfully written or RESULT_ERROR if an
		 *   error occurred. */
		ssize_t	write(uint64_t number);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(float number);

		/** Writes "number" to the browser.  Returns the number of
		 *  bytes that were successfully written or RESULT_ERROR if an
		 *  error occurred. */
		ssize_t	write(double number);

		/** Writes "filebuffer" to the browser.*/
		ssize_t	write(file *filebuffer);

	#include <rudiments/private/httpresponse.h>

};

#endif
