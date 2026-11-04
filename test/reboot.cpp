// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {
	if (!sys::reboot()) {
		char	*err=error::getErrorString();
		stdoutput.printf("reboot failed: %s\n",err);
		delete[] err;
		process::exit(1);
	}
}
