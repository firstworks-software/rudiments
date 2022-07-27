// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_APACHEINCLUDES_H
#define RUDIMENTS_APACHEINCLUDES_H

extern "C" {
	#ifndef APACHE_2
		// apache 1.x uses table instead of apr_table_t, which
		// collides with our table class, so use some trickery
		// to work around that
		#define table apr_table_t
	#endif
	#ifdef _DARWIN
		#define MAC_OS 1
	#endif
	#include "httpd.h"
	#include "http_config.h"
	#include "http_protocol.h"
	#include "ap_config.h"
	#ifndef APACHE_2
		#undef table
	#endif
}

#endif
