// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME sessioncookie

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	resp.sendContentTypeHeader("text","html",NULL);
	resp.setCookie("password","abcd1234",NULL,NULL,NULL,false);
	resp.sendCrLf()();
	resp.sendCrLf()();

	resp.write("Cookie set!\n");

	return true;
}
