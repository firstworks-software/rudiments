// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {
	if (!sys::halt()) {
		stdoutput.printf("halt failed: %s\n",error::getErrorString());
		process::exit(1);
	}
}
