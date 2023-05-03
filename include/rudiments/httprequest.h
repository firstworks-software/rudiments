// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPREQUEST_H
#define RUDIMENTS_HTTPREQUEST_H

#include <rudiments/private/httprequestincludes.h>

/** The httprequest class provides methods for accessing components of the
 *  http request. */

class RUDIMENTS_DLLSPEC httprequest : public input {
	public:
		httprequest(httpserverapi *sapi);
		virtual	~httprequest();



		/** Writes all environment variables, parameters,
		 *  file parameters, and cookies to the browser. */
		virtual void	writeVariables();



		/** Returns the value of the environment variable "name". */
		const char	*getEnvironmentVariable(const char *name);

		/** Returns the number of environment variables. */
		uint64_t	getEnvironmentVariableCount();

		/** Returns a null terminated list of environment variables. */
		const char * const *getEnvironmentVariables();

		/** Returns a null terminated list of environment variable
		 *  values. */
		const char * const *getEnvironmentValues();

		/** Creates a new environment variable with name "name" and
		 *  value "value" or updates an existing environment variable
		 *  "name" to have value "value". */
		bool	setEnvironmentVariable(const char *name,
							const char *value);

		/** Writes all environment variables to the browser.  Similar
		 *  to running "env" on the command line. */
		void	writeEnvironment();



		/** Returns the value of parameter "name". */
		const char	*getParameter(const char *name);

		/** Returns all parameters formatted as an escaped get string
		 *  into "container" with the exception of a NULL terminated
		 *  list of exceptions.  If "container" is NULL, the list is
		 *  written to the browser. */
		void	getParametersAsGetString(output *container,
						const char * const *exceptions);

		/** Returns all parameters formatted as a list of hidden
		 *  variables into "container" with the exception of a NULL
		 *  terminated list of exceptions passed in after "container".
		 *  If "container" is NULL, the list is written to the
		 *  browser. */
		void	getParametersAsHiddenVariables(output *container,
						const char * const *excepcions);

		/** Returns the number of parameters. */
		uint64_t	getParameterCount();

		/** Returns a null terminated list of parameter variables.
		 *  The order of the list is the order the variables were
		 *  received in. */
		const char * const *getParameterVariables();

		/** Returns a null terminated list of values corresponding to
		 *  the variables returned from getParameterVariables(). */
		const char * const *getParameterValues();

		/** Creates a new parameter with name "name" and value "value"
		 *  or updates an existing parameter "name" to have value
		 *  "value". */
		bool	setParameter(const char *name, const char *value);



		/** Returns the filename passed in from file parameter
		 *  "name". */
		const char	*getFileParameterFileName(const char *name);

		/** Returns the filename of the local file created when file
		 *  parameter "name" was passed in. */
		const char	*getFileParameterTempFileName(const char *name);

		/** Returns the mime type of the file parameter "name". */
		const char	*getFileParameterMimeType(const char *name);

		/** Returns the number of posted files. */
		uint64_t	getFileCount();

		/** Returns a null terminated list of file names.  The order of
		 *  the list is the order in which the file parameters were
		 *  received. */
		const char * const *getFileNames();

		/** Updates file parameter with name "name", to filename
		 *  "filename", temporary filename "tempfilename" and mime-type
		 *  "mimetype".  If the file parameter doesn't already exist,
		 *  then no action is taken. */
		bool	setFileParameter(const char *name,
						const char *filename, 
						const char *tempfilename,
						const char *mimetype);

		/** Removes all temporary files that were created when parsing
		 *  the request. */
		void	removeTempFiles();



		/** Returns the value of cookie "name". */
		const char	*getCookie(const char *name);

		/** Returns the number of cookies collected. */
		uint64_t	getCookieCount();

		/** Returns a null terminated list of cookie variables.
		 *  The order of the list is the order the variables were
		 *  received in. */
		const char * const *getCookieVariables();

		/** Returns a null terminated list of values corresponding to
		 *  the variables returned from getCookieVariables(). */
		const char * const *getCookieValues();

		/** Creates a new cookie with name "name" and value "value"
		 *  or updates an existing cookie "name" to have value
		 *  "value". */
		bool	setCookie(const char *name, const char *value);



		/** Returns the total number of variables - environment,
		 *  parameters, files, and cookies. */
		uint64_t	getAllVariableCount();

		/** Returns a null terminated list of all variables.
		 *  The order of the list is the order the variables were
		 *  received in. */
		const char * const *getAllVariables();

		/** Returns a null terminated list of the values corresponding
		 *  to the variables returned from getAllVariables(). */
		const char * const *getAllValues();



		/** Reads "size" bytes from the data posted by the client into
		 *  "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(byte_t *buffer, size_t size);

		/** Reads "length" characters from the data posted by the
		 *  client into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(char *buffer, size_t length);

		/** Reads a character from the data posted by the client into
		 *  "character".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(char *character);

		/** Reads "length" characters from the data posted by the
		 *  client into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(wchar_t *buffer, size_t length);

		/** Reads a character from the data posted by the client into
		 *  "character".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(wchar_t *character);

		/** Reads "length" characters from the data posted by the
		 *  client into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	readUcs2(ucs2_t *buffer, size_t length);

		/** Reads a character from the data posted by the client into
		 *  "character".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	readUcs2(ucs2_t *character);

		/** Reads a 16-bit integer from the data posted by the client
		 *  into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(int16_t *number);

		/** Reads a 32-bit integer from the data posted by the client
		 *  into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(int32_t *number);

		/** Reads a 64-bit integer from the data posted by the client
		 *  into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(int64_t *number);

		/** Reads a byte from the data posted by the client into
		 *  "character".  Returns the number of bytes that were
		 *  successfully read or  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(byte_t *character);

		/** Reads a 16-bit unsigned integer from the data posted by the
		 *  client into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(uint16_t *number);

		/** Reads a 32-bit unsigned integer from the data posted by the
		 *  client into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(uint32_t *number);

		/** Reads a 64-bit unsigned integer from the data posted by the
		 *  client into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(uint64_t *number);

		/** Reads a floating point number from the data posted by the
		 *  client into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(float *number);

		/** Reads a double-precision floating point number from the
		 *  data posted by the client into "number".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  Note that all read() methods return 0 (EOF) when the
		 *  REQUEST_METHOD environment vairable is "get" or "head", as
		 *  no data will have been posted, or when the REQUEST_METHOD
		 *  environment variable is "post" and the CONTENT_TYPE
		 *  environment variable is either
		 *  "application/x-www-form-urlencoded" or
		 *  "multipart-form-data", as in those cases, the posted data
		 *  will have been processed internally. */
		ssize_t	read(double *number);



		/** Matches the REQUEST_METHOD environment variable against
		 *  "deniedmethods" and "allowedmethods" (in that order) using
		 *  regular expression syntax.  Returns true if the request
		 *  method used by the client is allowed and false otherwise.
		 * 
		 *  This method may be overriden to provide customized
		 *  request-method-based security. */
		virtual bool	getMethodIsAllowed(
					const char *deniedmethods,
					const char *allowedmethods);

		/** Matches the CONTENT_TYPE environment variable against
		 *  "deniedcontenttypes" and "allowedcontenttypes" (in that
		 *  order) using regular expression syntax.  Returns true if
		 *  the content type of the data posted by the client is
		 *  allowed and false otherwise.
		 * 
		 *  This method may be overriden to provide customized
		 *  content-type-based security. */
		virtual bool	getContentTypeIsAllowed(
					const char *deniedcontenttypes,
					const char *allowedcontenttypes);

		/** Matches the REMOTE_ADDR environment variable against
		 *  "deniedips" and "allowedips" (in that order) using regular
		 *  expression syntax.  Returns true if the client's IP
		 *  address is allowed and false otherwise.
		 * 
		 *  This method may be overriden to provide customized
		 *  ip-based security. */
		virtual	bool	getIpIsAllowed(
					const char *deniedips,
					const char *allowedips);

		/** Matches the HTTP_REFERER environment variable against
		 *  "deniedreferers" and "allowedreferers" (in that order)
		 *  using regular expression syntax.  Returns true if the
		 *  referrer is permitted and false otherwise.
		 * 
		 *  This method may be overriden to provide customized
		 *  referrer-based security. */
		virtual	bool	getRefererIsAllowed(
					const char *deniedreferers,
					const char *allowedreferers);


		struct parameterrequirement {
			const char	*variable;
			bool		allowempty;
		};

		/** Reads through the NULL terminated list of parameter
		 *  requirements.  Returns true if the parameters meet
		 *  the requirements and false if they did not.
		 * 
		 *  Each parameterrequirement contains a parameter
		 *  variable name and a boolean value indicating whether
		 *  that variable can be empty or not.
		 * 
		 *  For example.  The following call requires that
		 *  "first", "last", "address", "city", "state" and
		 *  "phone" be present but allows "address", "city" and
		 *  "state" to be blank.
		 * 
		 *  parameterrequirement	*pr[]={{"first",false},
		 * 				{"last",false},
		 * 				{"address",true},
		 * 				{"city",true},
		 * 				{"state",true},
		 * 				{"phone",false},
		 * 				NULL};
		 *  success=requiredParameters(pr); */
		bool	getRequiredParametersWereProvided(
					parameterrequirement **pr);

	#include <rudiments/private/httprequest.h>
};


#endif
