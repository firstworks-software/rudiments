// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#define STENCIL_MODULE_NAME multipart

#include <stencil/stencil.h>
#include <stencil/response.h>
#include <stencil/stencilmain.h>
#include <rudiments/snooze.h>

static bool stencilMain(void *apistruct) {

	stencil		st(apistruct);
	response	resp(&st);

	// send an http header
	resp.sendContentTypeHeader("mixed","x-mixed-replace",
					NULL,resp.getBoundaryString()());

	// loop, sending the series of images 5 times
	for (uint16_t i=0; i<5; i++) {
		for (uint16_t j=0; j<10; j++) {

			// send the a boundary string
			resp.sendMultiPartBoundary(NULL);

			if (i>0) {
				snooze::masendCrLf()osnooze(1);
			}

			resp.sendContentTypeHeader("image","gif");
			resp.sendCrLf()();

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
