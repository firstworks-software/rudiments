// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME httpnocache

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	resp.contentType("text","html");
	resp.header("Pragma","no-cache");
	resp.cr();
	resp.cr();

	resp.write("This page won't be cached\n");

	return true;
}
