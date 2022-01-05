// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_URLHTTPREQUEST_H
#define RUDIMENTS_URLHTTPREQUEST_H

#include <rudiments/private/urlhttprequestincludes.h>

/** The urlhttprequest class extends httprequest, parsing the url and
 *  providing methods for accessing its components.
 *
 *  The SCRIPT_NAME part of the url is parsed as follows:
 *
 *      DOCUMENT_ROOT/"application"/"program".cgi
 *
 *      ("application" may span multiple directories)
 *
 *  The PATH_INFO part url is parsed as follows:
 *
 *      /"skin"/"module"/"page".html
 *
 *      ("module" may span multiple directories)
 *
 *  The following directories are presumed to exist:
 *
 *      DOCUMENT_ROOT/"application"/"skin"/"module"
 *
 *  These directories are searched for XML files with .var extensions,
 *  formatted as follows:
 *
 *  	<skin>
 * 	 	<var name="...name...">...value...</var>
 *  	</skin>
 *
 *  These are parsed into "skin variables".  Skin variables defined in deeper
 *  level .var files override skin variables defined in shallower files.
 */
class RUDIMENTS_DLLSPEC urlhttprequest : public httprequest {
	public:
		urlhttprequest(httpserverapi *sapi);
		virtual	~urlhttprequest();



		/** Dumps all environment variable, parameters, file parameter,
		 *  cookies, skin variables, and url variables to the
		 *  browser. */
		void	dumpVariables();



		/** Returns the "application" part of the SCRIPT_NAME
		 *  environment variable.  SCRIPT_NAME is assumed to be
		 *  constructed as follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*application();

		/** Returns the location in the filesystem corresponding to the
		 *  "application" part of the SCRIPT_NAME environment variable.
		 *  SCRIPT_NAME is assumed to be constructed as follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*applicationPath();

		/** Returns a URL corresponding to the "application" part of
		 *  the SCRIPT_NAME environment variable.  SCRIPT_NAME is
		 *  assumed to be constructed as follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*applicationUrl();

		/** Returns the "program" part of the SCRIPT_NAME environment
		 *  variable.  SCRIPT_NAME is assumed to be constructed as
		 *  follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*program();

		/** Returns the location in the filesystem corresponding to the
		 *  "program" part of the SCRIPT_NAME environment variable.
		 *  SCRIPT_NAME is assumed to be constructed as follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*programPath();

		/** Returns a URL corresponding to the "program" part of the
		 *  SCRIPT_NAME environment variable.  SCRIPT_NAME is assumed
		 *  to be constructed as follows:
		 *      DOCUMENT_ROOT/"application"/"program".cgi */
		const char	*programUrl();

		/** Returns the "skin" part of the PATH_INFO environment
		 *  variable.  PATH_INFO is assumed to be constructed as
		 *  follows:
		 *      /"skin"/"module"/"page".html */
		const char	*skin();

		/** Returns the location in the filesystem corresponding to the
		 *  "skin" part of the PATH_INFO environment variable.
		 *  PATH_INFO is assumed to be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*skinPath();

		/** Returns a URL corresponding to the "skin" part of the
		 *  PATH_INFO environment variable.  PATH_INFO is  assumed to
		 *  be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*skinUrl();

		/** Returns the "module" part of the PATH_INFO environment
		 *  variable.  PATH_INFO is assumed to be constructed as
		 *  follows:
		 *      /"skin"/"module"/"page".html */
		const char	*module();

		/** Returns the location in the filesystem corresponding to the
		 *  "module" part of the PATH_INFO environment variable.
		 *  PATH_INFO is assumed to be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*modulePath();

		/** Returns a URL corresponding to the "module" part of the
		 *  PATH_INFO environment variable.  PATH_INFO is  assumed to
		 *  be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*moduleUrl();

		/** Returns the "page" part of the PATH_INFO environment
		 *  variable.  PATH_INFO is assumed to be constructed as
		 *  follows:
		 *      /"skin"/"module"/"page".html */
		const char	*page();

		/** Returns the location in the filesystem corresponding to the
		 *  "page" part of the PATH_INFO environment variable.
		 *  PATH_INFO is assumed to be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*pagePath();

		/** Returns a URL corresponding to the "path" part of the
		 *  PATH_INFO environment variable.  PATH_INFO is  assumed to
		 *  be constructed as follows:
		 *      /"skin"/"module"/"page".html */
		const char	*pageUrl();

		/** Returns the number of URL variables. */
		uint64_t	getUrlVariableCount();

		/** Returns a null terminated list of URL variables. */
		const char * const *getUrlVariables();

		/** Returns a null terminated list of values corresponding
		 *  to the variables returned from getUrlVariables(). */
		const char * const *getUrlValues();



		/** Returns the value of skin variable "name". */
		const char	*getSkinVariable(const char *name);

		/** Returns the number of skin variables. */
		uint64_t	getSkinVariableCount();

		/** Returns a null terminated list of skin variables.  The
		 *  order of the list is the order the variables were received
		 *  in. */
		const char * const *getSkinVariables();

		/** Returns a null terminated list of values corresponding to
		 *  the variables returned from getSkinVariables(). */
		const char * const *getSkinValues();

		/** Creates a new skin variable with name "name" and value
		 *  "value" or updates an existing skin variable "name" to have
		 *  value "value". */
		bool	setSkinVariable(const char *name, const char *value);



		/** Matches the REQUEST_METHOD environment variable
		 *  against the "denied-methods" and "allowed-methods"
		 *  skin variables (in that order) using regular
		 *  expression syntax to determine whether the client's
		 *  IP address is allowed to access this cgi.
		 * 
		 *  This method may be overriden to provide customized
		 *  ip-based security. */
		virtual bool	methodAllowed();

		/** Matches the REMOTE_ADDR environment variable against
		 *  the "denied-ips" and "allowed-ips" skin
		 *  variables (in that order) using regular expression 
		 *  syntax to determine whether the client's IP address
		 *  is allowed to access this cgi.
		 * 
		 *  This method may be overriden to provide customized
		 *  ip-based security. */
		virtual bool	ipAllowed();

		/** Matches the HTTP_REFERER environment variable against
		 *  the "denied-referers" and "allowed-referers" 
		 *  skin variables (in that order) using regular 
		 *  expression syntax to determine whether the client's 
		 *  IP address is allowed to access this cgi.
		 * 
		 *  This method may be overriden to provide customized
		 *  referer-based security. */
		virtual bool	refererAllowed();


	#include <rudiments/private/urlhttprequest.h>
};


#endif
