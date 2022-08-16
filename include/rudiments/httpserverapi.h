// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_HTTPSERVERAPI_H
#define RUDIMENTS_HTTPSERVERAPI_H

#include <rudiments/private/httpserverapiincludes.h>

class RUDIMENTS_DLLSPEC httpserverapi : public input, public output {
	public:

		virtual const char	*getType()=0;

	#include <rudiments/private/httpserverapi.h>
};

#endif
