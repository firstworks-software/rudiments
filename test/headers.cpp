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
	resp.sendHeader("HTTP/1.1 200 OK");

	// content types
	resp.sendTextPlainHeader();
	resp.sendTextHtmlHeader();
	resp.sendContentTypeHeader("text","html");
	resp.sendContentTypeHeader("text","html","ISO-8859-4");
	resp.sendContentTypeHeader("mixed","multipart",
					NULL,resp.getBoundaryString());

	// cookies
	resp.setCookie("cookievariable","cookievalue",NULL,NULL,NULL,0);
	resp.setCookie("cookievariable","cookievalue","firstworks.com",
			NULL,NULL,0);
	resp.setCookie("cookievariable","cookievalue","firstworks.com",
			"/stencil",NULL,0);
	resp.setCookie("cookievariable","cookievalue","firstworks.com",
			"/stencil","Sun, 06 Nov 1994 08:49:37 GMT",0);
	resp.setCookie("cookievariable","cookievalue","firstworks.com",
			"/stencil","Sun, 06 Nov 1994 08:49:37 GMT",1);

	resp.sendHeader("Title","Hello, This is a title.");

	return true;
}

static bool httpModuleExit(httpserverapi *sapi) {
	return true;
}
