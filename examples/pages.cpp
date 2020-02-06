// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME pages

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>
#include <rudiments/templateengine.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>

// handler for login.html
bool loginhtml(request *req, response *resp,
			stringbuffer *container, const char *filename,
			void *data) {

	// send an http header
	resp->textHtml();

	// parse page contents
	return templateengine::parse(req,resp,NULL,req->pagePath(),
							NULL,NULL,NULL);
}


// handler for addressbook.html
bool addressbookhtml(request *req, response *resp,
			stringbuffer *container, const char *filename,
			void *data) {

	// validate login
	// normally you'd look this data up in a database, but for this
	// example, it's hardcoded into the program
	if (charstring::compare(req->parameter("login"),"myuser") ||
		charstring::compare(req->parameter("password"),"mypassword")) {
		return false;
	}

	// send an http header
	resp->textHtml();

	// parse page contents
	return templateengine::parse(req,resp,NULL,req->pagePath(),
							NULL,NULL,NULL);
}

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	// associate pages with page handling functions
	pagehandler	ph[]={
		{"addressbook","login.html",loginhtml,NULL},
		{"addressbook","addressbook.html",addressbookhtml,NULL},
		{NULL,NULL,NULL,NULL}
	};

	// handle the page
	if (templateengine::handlePage(&req,&resp,ph)) {
		return true;
	}

	// if an error occurred, display the error page
	stringbuffer	filename;
	filename.append(req.skinPath())->append("/addressbook/error.html");
	resp.textHtml();
	templateengine::parse(&req,&resp,NULL,filename.getString(),
							NULL,NULL,NULL);
	return true;
}
