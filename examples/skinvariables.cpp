// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME skinvariables

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	resp.textHtml();

	resp.write("Skin Variables: <br><br>\n");

	resp.write("Individually: <br>\n");
	resp.write("instance: ");
	resp.write(req.skinVariable("instance"));
	resp.write("<br>\n");
	resp.write("username: ");
	resp.write(req.skinVariable("username"));
	resp.write("<br>\n");
	resp.write("password: ");
	resp.write(req.skinVariable("password"));
	resp.write("<br>\n");

	resp.write("From Array: <br>\n");
	resp.write("\n<br><br>\n");
	for (uint64_t i=0; i<req.skinVariableCount(); i++) {
		resp.write(req.skinVariables()[i]);
		resp.write(": ");
		resp.write(req.skinValues()[i]);
		resp.write("<br>\n");
	}
	
	return true;
}
