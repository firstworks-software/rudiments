// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

// necessary to get swab on redhat 7.3
//#define _XOPEN_SOURCE 600

#include <sys/types.h>

#ifdef RUDIMENTS_HAVE_WCTYPE_H
	#include <wctype.h>
#endif
