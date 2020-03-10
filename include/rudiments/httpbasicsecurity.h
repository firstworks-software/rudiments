// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_HTTPBASICSECURITY_H
#define RUDIMENTS_HTTPBASICSECURITY_H 1

#include <rudiments/private/httpbasicsecurityincludes.h>

class httpbasicsecurity : public mvcsecurity {
	public:
		httpbasicsecurity();
		virtual ~httpbasicsecurity();

		void	setRequest(httprequest *req);
		httprequest	*getRequest();
		const char	*getUserName();

	#include <rudiments/private/httpbasicsecurity.h>
};

#endif
