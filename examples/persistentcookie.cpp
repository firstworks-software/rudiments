// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME persistentcookie

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	resp.contentType("text","html",NULL);
	resp.setCookie("password","abcd1234",NULL,NULL,
			"Sun, 01-Mar-2020 00:00:00 GMT",false);
	resp.cr();
	resp.cr();

	resp.write("Cookie set!\n");

	return true;
}
