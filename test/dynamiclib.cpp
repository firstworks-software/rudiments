// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/dynamiclib.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("dynamiclib");

// not supported by linux libc
#ifndef RUDIMENTS_HAVE_G_CONFIG_H

#ifdef _WIN32
	// windows
	char	*f=charstring::duplicate(
		"C:\\Windows\\System32\\msvcrt.dll");
#else
	// decide on a file to use
	const char	*dirs[]={
		"/lib64","/usr/lib64","/lib","/usr/lib",
		// FIXME: other architectures
		"/lib/x86_64-linux-gnu",
		"/usr/lib/x86_64-linux-gnu",
		NULL
	};
	char	*f=NULL;
	for (const char * const *dir=dirs; *dir; dir++) {
		directory	d;
		if (d.open(*dir)) {
			d.rewind();
			for (;;) {
				char	*file=d.read();
				if (!file) {
					break;
				}
				if ((!charstring::compare(file,"libc-",5) &&
					(!charstring::compare(
						charstring::findLast(
								file,".so"),
						".so") ||
					!charstring::compare(
						charstring::findLast(
								file,".dylib"),
						".dylib"))) ||
					!charstring::compare(
						file,"libc.so.",8) ||
					!charstring::compare(
						file,"libc.dylib",10)) {
					f=file;
					break;
				}
				delete[] file;
			}
		}
		if (f) {
			break;
		}
	}
#endif
	test("file exists",f);

	// open valid file and get a symbol
	dynamiclib	d;
	test("open: valid file",d.open(f,false,false));
	void	*valid=d.getSymbol("strcmp");
	test("getSymbol: valid symbol",valid);
	test("getError: valid symbol",!d.getError());
	valid=d.getSymbol("strcmpbad");
	test("getSymbol: invalid symbol",!valid);
	test("getError: invalid symbol",d.getError());
	test("close",d.close());

	// open invalid file
	test("open: invalid file",!d.open("badfile",false,false));
	test("getError: invalid file",d.getError()!=NULL);

	delete[] f;
#endif

	stdoutput.printf("\n");
}
