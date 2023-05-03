// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME urlparsing

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	request		req(&st);
	response	resp(&st);

	resp.sendTextHtmlHeader();

	resp.write("URL Parts: <br><br>\n");

	resp.write("Individually: <br>\n");
	resp.write("application: ");
	resp.write(req.getApplication());
	resp.write("<br>\n");
	resp.write("application path: ");
	resp.write(req.getApplicationPath());
	resp.write("<br>\n");
	resp.write("application url: ");
	resp.write(req.getApplicationUrl());
	resp.write("<br>\n");
	resp.write("program: ");
	resp.write(req.getProgram());
	resp.write("<br>\n");
	resp.write("program path: ");
	resp.write(req.getProgramPath());
	resp.write("<br>\n");
	resp.write("program url: ");
	resp.write(req.getProgramUrl());
	resp.write("<br>\n");
	resp.write("skin: ");
	resp.write(req.getSkin());
	resp.write("<br>\n");
	resp.write("skin path: ");
	resp.write(req.getSkinPath());
	resp.write("<br>\n");
	resp.write("skin url: ");
	resp.write(req.getSkinUrl());
	resp.write("<br>\n");
	resp.write("module: ");
	resp.write(req.getModule());
	resp.write("<br>\n");
	resp.write("module path: ");
	resp.write(req.getModulePath());
	resp.write("<br>\n");
	resp.write("module url: ");
	resp.write(req.getModuleUrl());
	resp.write("<br>\n");
	resp.write("page: ");
	resp.write(req.getPage());
	resp.write("<br>\n");
	resp.write("page path: ");
	resp.write(req.getPagePath());
	resp.write("<br>\n");
	resp.write("page url: ");
	resp.write(req.getPageUrl());
	resp.write("<br>\n");

	resp.write("From Array: <br>\n");
	resp.write("\n<br><br>\n");
	for (uint64_t i=0; i<req.urlVariableCount(); i++) {
		resp.write(req.urlVariables()[i]);
		resp.write(": ");
		resp.write(req.urlValues()[i]);
		resp.write("<br>\n");
	}
	
	return true;
}
