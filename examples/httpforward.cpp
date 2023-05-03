// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME httpforward

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	resp.sendHeader("Location","html://www.mysite.com/errors/error.html");
	resp.sendCrLf()();
	resp.sendCrLf()();

	return true;
}
