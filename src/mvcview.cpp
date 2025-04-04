// Copyright (c) David Muse
// See the COPYING file for more informations.

#include <rudiments/mvc.h>

class mvcviewprivate {
	friend class mvcview;
	private:
		// FIXME: ideally these should be generic request/response
		httprequest	*req;
		httpresponse	*resp;
};

mvcview::mvcview() : mvctier() {
	pvt=new mvcviewprivate;
	pvt->req=NULL;
	pvt->resp=NULL;
}

mvcview::~mvcview() {
	delete pvt;
}

void mvcview::setRequest(httprequest *req) {
	pvt->req=req;
}

httprequest *mvcview::getRequest() {
	return pvt->req;
}

void mvcview::setResponse(httpresponse *resp) {
	pvt->resp=resp;
}

httpresponse *mvcview::getResponse() {
	return pvt->resp;
}
