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

	resp.textHtml();

	resp.write("URL Parts: <br><br>\n");

	resp.write("Individually: <br>\n");
	resp.write("application: ");
	resp.write(req.application());
	resp.write("<br>\n");
	resp.write("applicationPath: ");
	resp.write(req.applicationPath());
	resp.write("<br>\n");
	resp.write("applicationUrl: ");
	resp.write(req.applicationUrl());
	resp.write("<br>\n");
	resp.write("program: ");
	resp.write(req.program());
	resp.write("<br>\n");
	resp.write("programPath: ");
	resp.write(req.programPath());
	resp.write("<br>\n");
	resp.write("programUrl: ");
	resp.write(req.programUrl());
	resp.write("<br>\n");
	resp.write("skin: ");
	resp.write(req.skin());
	resp.write("<br>\n");
	resp.write("skinPath: ");
	resp.write(req.skinPath());
	resp.write("<br>\n");
	resp.write("skinUrl: ");
	resp.write(req.skinUrl());
	resp.write("<br>\n");
	resp.write("module: ");
	resp.write(req.module());
	resp.write("<br>\n");
	resp.write("modulePath: ");
	resp.write(req.modulePath());
	resp.write("<br>\n");
	resp.write("moduleUrl: ");
	resp.write(req.moduleUrl());
	resp.write("<br>\n");
	resp.write("page: ");
	resp.write(req.page());
	resp.write("<br>\n");
	resp.write("pagePath: ");
	resp.write(req.pagePath());
	resp.write("<br>\n");
	resp.write("pageUrl: ");
	resp.write(req.pageUrl());
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
