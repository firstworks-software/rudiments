// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPSERVERMAIN_H
#define RUDIMENTS_HTTPSERVERMAIN_H

// Include this file in the .cpp file that contains your functions
// httpModuleInit(), httpModuleMain(), and httpModuleExit().
//
// Define HTTP_MODULE_CGI to compile your program as a CGI.
// Define HTTP_MODULE_APACHE to compile your program as an Apache module.
//
// To enable an Apache module, add directives like the following to the
// Apache configuration:
//
//	LoadModule	example_module modules/mod_example.so
//	AddHandler	example ".mvc"
//
// In this example, mod_example.so handles urls where the "page" extension is
// ".mvc", such as http://site/example.mvc or http://site/example.mvc/path

#if defined(HTTP_MODULE_CGI)
	#include <rudiments/private/cgimodule.h>
#elif defined(HTTP_MODULE_APACHE)
	#include <rudiments/private/apachemodule.h>
#endif

#endif
