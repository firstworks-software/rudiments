// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME substitutions

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
	resp->sendTextHtmlHeader();

	// parse page contents
	return templateengine::parse(req,resp,NULL,
					req->pagePath(),NULL,NULL,NULL);
}


// handler for addressentry segment
bool addressentry(request *req, response *resp,
			stringbuffer *container, const char *name,
			const char *segment, uint64_t length,
			void *data) {

	struct addressdata {
		const char	*name;
		const char	*address;
		const char	*city;
		const char	*state;
		const char	*phonenumber;
	};

	// build a list of names and address data
	// normally you'd look this data up in a database, but for this
	// example, it's hardcoded into the program
	addressdata	ad[]={
		{"David","1234 Dave's St.","Cumming","GA","555-1234"},
		{"Mark","2345 Mark's St.","Baton Rouge","LA","555-2345"},
		{"Nick","3456 Nick's Rd.","Some City","NJ","555-3456"},
		{"Mike","4567 Mike's St.","Houston","TX","555-4567"}
	};

	// for each address in the list, parse the segment,
	// substituting in the name and address data
	for (int i=0; i<4; i++) {

		variable	var[]={
			{"name",ad[i].name},
			{"address",ad[i].address},
			{"city",ad[i].city},
			{"state",ad[i].state},
			{"phonenumber",ad[i].phonenumber},
			{NULL,NULL}
		};

		if (!templateengine::parse(req,resp,container,
						segment,length,NULL,NULL,var)) {
			return false;
		}
	}
	return true;
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
	resp->sendTextHtmlHeader();

	// use built-in substitution variables (parameters, cookies, etc.)
	variables vars[]={
		{req->allVariables(),req->allValues()},
		{NULL,NULL}
	};

	// handle the segments in the page
	segmenthandler	sh[]={
		{NULL,"addressentry",addressentry,NULL},
		{NULL,NULL,NULL,NULL}
	};

	// parse page contents
	return templateengine::parse(req,resp,NULL,
					req->pagePath(),sh,vars,NULL);
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
	resp.sendTextHtmlHeader();
	templateengine::parse(&req,&resp,NULL,
				filename.getString(),NULL,NULL,NULL);
	return true;
}
