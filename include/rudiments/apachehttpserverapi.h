// Copyright (c) David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_APACHEHTTPSERVERAPI_H
#define RUDIMENTS_APACHEHTTPSERVERAPI_H

#include <rudiments/private/apachehttpserverapiincludes.h>


/** The apachehttpserverapi class implements the httpserverapi, input, and
 *  output interfaces for the Apache web server, enabling web based
 *  applications to be developed as Apache modules.
 *
 *  Note that this class is not intended to be instantiated directly.
 *
 *  Rather, an application should define HTTP_MODULE_APACHE, include
 *  <rudiments/httpserverapimain.h>, and then implement the method
 *  httpModuleMain(httpserverapi *sapi) as the main body of the application.
 *  The methods of "sapi" can then be used directly. */
class RUDIMENTS_DLLSPEC apachehttpserverapi : public httpserverapi {
	public:

		/** Creates an instance of the apachehttpserverapi class. */
		apachehttpserverapi(void *apistruct);

		/** Deletes this instance of the apachehttpserverapi class. */
		~apachehttpserverapi();

		/** Returns "apache1" if Rudiments was compiled against
		 *  Apache 1.x or "apache2" if Rudiments was compiled against
		 *  Apache 2.x. */
		const char	*getType();

	#include <rudiments/private/apachehttpserverapi.h>
};

#endif
