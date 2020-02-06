// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME files

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


	// files...

	// display an individual file entry
	resp.write("Specific File Parameter:\n");
	resp.write("file1=");
	resp.write(req.fileParameterFilename("file1"));
	resp.write("\n");
	resp.write("temp file=");
	resp.write(req.fileParameterTempFilename("file1"));
	resp.write("\n");
	resp.write("mime type=");
	resp.write(req.fileParameterMimeType("file1"));
	resp.write("\n");

	// list all files
	resp.write("All File Parameters:\n");
	for (uint64_t index=0; index<req.fileCount(); index++) {
		const char	*name=req.fileNames()[index];
		resp.write("file1=");
		resp.write(req.fileParameterFilename(name));
		resp.write("\n");
		resp.write("temp file=");
		resp.write(req.fileParameterTempFilename(name));
		resp.write("\n");
		resp.write("mime type=");
		resp.write(req.fileParameterMimeType(name));
		resp.write("\n");
	}

	// add a file entry
	req.newFileParameter("newfile","filename","tempfilename","text/html");
	resp.write("New File Parameter:\n");
	resp.write("newfile=");
	resp.write(req.fileParameterFilename("newfile"));
	resp.write("\n");
	resp.write("temp file=");
	resp.write(req.fileParameterTempFilename("newfile"));
	resp.write("\n");
	resp.write("mime type=");
	resp.write(req.fileParameterMimeType("newfile"));
	resp.write("\n");

	return true;
}
