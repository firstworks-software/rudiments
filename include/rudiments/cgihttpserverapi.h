// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_CGIHTTPSERVERAPI_H
#define RUDIMENTS_CGIHTTPSERVERAPI_H

#include <rudiments/private/cgihttpserverapiincludes.h>


/** The cgihttpserverapi class implements the httpserverapi, input, and
 *  output interfaces such that web based applications can be developed as CGIs.
 *
 *  Note that this class is not intended to be instantiated directly.
 *
 *  Rather, an application should define HTTP_MODULE_CGI, include
 *  <rudiments/httpserverapimain.h>, and then implement the method
 *  httpModuleMain(httpserverapi *sapi) as the main body of the application.
 *  The methods of "sapi" can then be used directly. */
class RUDIMENTS_DLLSPEC cgihttpserverapi : public httpserverapi {
	public:

		/** Creates an instance of the cgihttpserverapi class. */
		cgihttpserverapi();

		/** Deletes this instance of the cgihttpserverapi class. */
		~cgihttpserverapi();

		/** Returns "cgi". */
		const char	*getType();

	#include <rudiments/private/cgihttpserverapi.h>
};

#endif
