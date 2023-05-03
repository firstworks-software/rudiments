// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <stencil/stencil.h>
#include <stencil/request.h>
#include <stencil/response.h>
#include <rudiments/process.h>

int main(int argc, char **argv) {

	stencil		st(NULL);
	request		req(&st);
	response	resp(&st);

	// send an http header
	resp.sendTextPlainHeader();

	// write hello world
	resp.write("hello world!\n");

	process::exit(0);
}
