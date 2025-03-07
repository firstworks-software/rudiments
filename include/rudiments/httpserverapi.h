// Copyright (c) David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPSERVERAPI_H
#define RUDIMENTS_HTTPSERVERAPI_H

#include <rudiments/private/httpserverapiincludes.h>


/** The httpserverapi class provides a base class for developing web based
 *  applications using http server APIs.
 *
 *  Note that this class and its parenet classes define an interface, and
 *  cannot be instantiated directly.  Its children are also not intended to be
 *  instantiated directly.
 *
 *  Rather, an application should define one of HTTP_MODULE_CGI,
 *  HTTP_MODULE_APACHE, etc., include <rudiments/httpserverapimain.h>, and
 *  then implement the method httpModuleMain(httpserverapi *sapi) as the main
 *  body of the application.  The methods of "sapi" can then be used
 *  directly. */
class RUDIMENTS_DLLSPEC httpserverapi : public input, public output {
	public:

		/** Returns the type of httpserverapi, eg. "apache1",
		 *  "apache2", "cgi", etc. */
		virtual const char	*getType()=0;

	#include <rudiments/private/httpserverapi.h>
};

#endif
