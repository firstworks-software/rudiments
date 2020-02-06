// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME envvars

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


	// environment variables...

	// display an individual environment variable
	resp.write("DOCUMENT_ROOT=");
	resp.write(req.environmentVariable("DOCUMENT_ROOT"));
	resp.write("<br><br>\n");

	// list all environment variables
	resp.write("All Environment Variables:<br>\n");
	const char * const *vars=req.environmentVariables();
	const char * const *vals=req.environmentValues();
	for (uint64_t index=0; index<req.environmentVariableCount(); index++) {
		resp.write("	");
		resp.write(vars[index]);
		resp.write("=");
		resp.write(vals[index]);
		resp.write("<br>\n");
	}
	resp.write("<br>\n");

	// add an environment variable
	req.newEnvironmentVariable("NEW_VARIABLE","newvalue");
	resp.write("New Environment Variable: ");
	resp.write(req.environmentVariable("NEW_VARIABLE"));
	resp.write("<br>\n");

	// change an environment variable
	req.changeEnvironmentVariable("NEW_VARIABLE","newervalue");
	resp.write("Changed Environment Variable: ");
	resp.write(req.environmentVariable("NEW_VARIABLE"));
	resp.write("<br>\n");

	return true;
}
