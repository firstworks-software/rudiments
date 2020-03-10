// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/httpbasicsecurity.h>

class httpbasicsecurityprivate {
	friend class httpbasicsecurity;
	private:
		httprequest	*req;
};

httpbasicsecurity::httpbasicsecurity() : mvcsecurity() {
	pvt=new httpbasicsecurityprivate;
	pvt->req=NULL;
}

httpbasicsecurity::~httpbasicsecurity() {
	delete pvt;
}

void httpbasicsecurity::setRequest(httprequest *req) {
	pvt->req=req;
}

httprequest *httpbasicsecurity::getRequest() {
	return pvt->req;
}

const char *httpbasicsecurity::getUserName() {
	return pvt->req->getEnvironmentVariable("REMOTE_USER");
}
