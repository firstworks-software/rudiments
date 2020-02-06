// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME httpforward

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	resp.header("Location","html://www.mysite.com/errors/error.html");
	resp.cr();
	resp.cr();

	return true;
}
