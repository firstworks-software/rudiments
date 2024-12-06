// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/object.h>

#include <rudiments/linkedlist.h>
#include <rudiments/file.h>

// provides option, facility, and priority macros
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	#include <syslog.h>
#endif

class syslogdestinationprivate;
class filedestinationprivate;
class loggerprivate;
