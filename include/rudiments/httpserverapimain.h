// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPSERVERMAIN_H
#define RUDIMENTS_HTTPSERVERMAIN_H

// Include this file in the .cpp file that contains your httpModuleMain().
//
// Define HTTP_MODULE_CGI to compile your program as a CGI.
// Define HTTP_MODULE_APACHE to compile your program as an Apache module.

#if defined(HTTP_MODULE_CGI)
	#include <rudiments/private/cgimodule.h>
#elif defined(HTTP_MODULE_APACHE)
	#include <rudiments/private/apachemodule.h>
#endif

#endif
