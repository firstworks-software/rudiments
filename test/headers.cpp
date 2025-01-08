// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define HTTP_MODULE_NAME headers
#define HTTP_MODULE_CGI

#include <rudiments/httpserverapi.h>
#include <rudiments/httpresponse.h>
#include <rudiments/httpserverapimain.h>

static bool httpModuleInit(httpserverapi *sapi) {
	return true;
}

static bool httpModuleMain(httpserverapi *sapi) {

	httpresponse	resp(sapi);
		
	// status line
	resp.writeHeader("HTTP/1.1 200 OK");
	resp.writeCrLf();

	// content types
	resp.writeTextPlainHeader();
	resp.writeTextHtmlHeader();
	resp.writeContentTypeHeader("text","html");
	resp.writeContentTypeHeader("text","html","ISO-8859-4");
	resp.writeContentTypeHeader("mixed","multipart",
					NULL,resp.getBoundaryString());

	// cookies
	resp.writeSetCookie("cookievariable","cookievalue",NULL,NULL,NULL,0);
	resp.writeSetCookie("cookievariable","cookievalue","firstworks.com",
				NULL,NULL,0);
	resp.writeSetCookie("cookievariable","cookievalue","firstworks.com",
				"/stencil",NULL,0);
	resp.writeSetCookie("cookievariable","cookievalue","firstworks.com",
				"/stencil","Sun, 06 Nov 1994 08:49:37 GMT",0);
	resp.writeSetCookie("cookievariable","cookievalue","firstworks.com",
				"/stencil","Sun, 06 Nov 1994 08:49:37 GMT",1);

	resp.writeHeader("Title","Hello, This is a title.");
	resp.writeCrLfs();

	return true;
}

static bool httpModuleExit(httpserverapi *sapi) {
	return true;
}
