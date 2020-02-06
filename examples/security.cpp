// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME security

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	// send an http header
	resp.textPlain();

	// method allowed

	// ip allowed

	// referer allowed

	// required parameters

	return true;
}
