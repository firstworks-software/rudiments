// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_CGIHTTPSERVERAPI_H
#define RUDIMENTS_CGIHTTPSERVERAPI_H

#include <rudiments/private/cgihttpserverapiincludes.h>

class RUDIMENTS_DLLSPEC cgihttpserverapi : public httpserverapi {
	public:
		cgihttpserverapi(void *apistruct);
		~cgihttpserverapi();

	#include <rudiments/private/cgihttpserverapi.h>
};

#endif
