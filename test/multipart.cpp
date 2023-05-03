// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define HTTP_MODULE_NAME headers
#define HTTP_MODULE_CGI

#include <rudiments/httpserverapi.h>
#include <rudiments/httpresponse.h>
#include <rudiments/httpserverapimain.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/file.h>
#include <rudiments/snooze.h>

static bool httpModuleInit(httpserverapi *sapi) {
	return true;
}

static bool httpModuleMain(httpserverapi *sapi) {

	httpresponse	resp(sapi);

	// send http header
	resp.sendContentTypeHeader("mixed","x-mixed-replace",
					NULL,resp.getBoundaryString());

	// loop, sending the series of images 5 times
	for (uint16_t i=0; i<5; i++) {
		for (uint16_t j=0; j<10; j++) {

			// send the a boundary string
			resp.sendMultiPartBoundary(NULL);

			if (i>0) {
				snooze::macrosnooze(1);
			}

			resp.sendContentTypeHeader("image","gif");
			resp.sendCrLf();

			// send the image
			stringbuffer	imagename;
			imagename.append("image")->append(j);
			imagename.append(".gif");
			file		imagefile;
			imagefile.open(imagename.getString(),O_RDONLY);
			resp.write(&imagefile);
			imagefile.close();
		}
	}

	// send the final boundary string
	resp.sendFinalMultiPartBoundary(NULL);

	return true;
}

static bool httpModuleExit(httpserverapi *sapi) {
	return true;
}
