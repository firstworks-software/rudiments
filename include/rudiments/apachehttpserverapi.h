// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_APACHEHTTPSERVERAPI_H
#define RUDIMENTS_APACHEHTTPSERVERAPI_H

#include <rudiments/private/apachehttpserverapiincludes.h>

class RUDIMENTS_DLLSPEC apachehttpserverapi : public httpserverapi {
	public:
		apachehttpserverapi(void *apistruct);
		~apachehttpserverapi();

		uint64_t	getThreadsPerProcess();

	#include <rudiments/private/apachehttpserverapi.h>
};

#endif
