// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPREQUEST_H
#define RUDIMENTS_HTTPREQUEST_H

#include <rudiments/private/httprequestincludes.h>

/** The httprequest class provides methods for accessing components of the
 *  http request. */

class RUDIMENTS_DLLSPEC httprequest : virtual public object {
	public:
		httprequest(httpserverapi *sapi);
		virtual	~httprequest();



		/** Dumps all environment variables, parameters,
		 *  file parameters, and cookies to the browser. */
		virtual void	dumpVariables();



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
		void	dumpEnvironment();



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
		const char	*getFileParameterFilename(const char *name);

		/** Returns the filename of the local file created when file
		 *  parameter "name" was passed in. */
		const char	*getFileParameterTempFilename(const char *name);

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



		/** Returns the posted json, if the content type of the
 		 *  request was application/json, or an empty string
 		 *  otherwise. */
		const char	*getJson();

		/** Returns the posted xml, if the content type of the
 		 *  request was application/xml, or an empty string
 		 *  otherwise. */
		const char	*getXml();



		/** Matches the REQUEST_METHOD environment variable against
		 *  "deniedmethods" and "allowedmethods" (in that order) using
		 *  regular expression syntax to determine whether the client's
		 *  IP address is allowed to access this application.
		 * 
		 *  This method may be overriden to provide customized
		 *  ip-based security. */
		virtual bool	methodAllowed(const char *deniedmethods,
						const char *allowedmethods);

		/** Matches the REMOTE_ADDR environment variable against
		 *  "deniedips" and "allowedips" (in that order) using regular
		 *  expression syntax to determine whether the client's IP
		 *  address is allowed to access this application.
		 * 
		 *  This method may be overriden to provide customized
		 *  ip-based security. */
		virtual	bool	ipAllowed(const char *deniedips,
						const char *allowedips);

		/** Matches the HTTP_REFERER environment variable against
		 *  "deniedreferers" and "allowedreferers" (in that order)
		 *  using regular expression syntax to determine whether the
		 *  client's IP address is allowed to access this application.
		 * 
		 *  This method may be overriden to provide customized
		 *  referer-based security. */
		virtual	bool	refererAllowed(const char *deniedreferers,
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
		bool	requiredParameters(parameterrequirement **pr);

	#include <rudiments/private/httprequest.h>
};


#endif
