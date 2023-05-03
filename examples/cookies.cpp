// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME cookies

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	// send an http header
	resp.sendTextPlainHeader();

	// cookies...

	// display an individual cookie
	resp.write("cookie1=");
	resp.write(req.cookie("cookie1"));
	resp.write("<br><br>\n");

	// list all cookies
	resp.write("All Cookies:<br>\n");
	const char * const *vars=req.cookieVariables();
	const char * const *vals=req.cookieValues();
	for (uint64_t index=0; index<req.cookieCount(); index++) {
		resp.write("	");
		resp.write(vars[index]);
		resp.write("=");
		resp.write(vals[index]);
		resp.write("<br>\n");
	}
	resp.write("<br>\n");

	// add a cookie
	req.newCookie("newentry","newvalue");
	resp.write("New Cookie: ");
	resp.write(req.cookie("newentry"));
	resp.write("<br>\n");

	// change a cookie
	req.changeCookie("newentry","newervalue");
	resp.write("Changed Cookie: ");
	resp.write(req.cookie("newentry"));
	resp.write("<br>\n");
	
	return true;
}
