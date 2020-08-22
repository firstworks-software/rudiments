// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVCVIEW_H
#define RUDIMENTS_MVCVIEW_H 1

#include <rudiments/private/mvcviewincludes.h>

class RUDIMENTS_DLLSPEC mvcview : public mvctier {
	public:
		mvcview();
		virtual ~mvcview();

		void	setRequest(httprequest *req);
		httprequest	*getRequest();

		void	setResponse(httpresponse *resp);
		httpresponse	*getResponse();

	#include <rudiments/private/mvcview.h>
};

#endif
