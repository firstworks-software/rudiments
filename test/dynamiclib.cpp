// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/charstring.h>
#include <rudiments/dynamiclib.h>
#include <rudiments/sys.h>
#include <rudiments/process.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("dynamiclib");

	// switch time zones
	char	*os=sys::getOperatingSystemName();
	char	*rel=sys::getOperatingSystemRelease();
	double	ver=charstring::toFloat(rel);
	// FIXME: not supported on linux libc, however it's possible that
	// there's a distro with a pre 2.0 kernel that doesn't use libc
	bool	notsupported=(!charstring::compare(os,"Linux",5) && ver<2.0);
	delete[] os;
	delete[] rel;
			
	if (notsupported) {
		stdoutput.printf("	not supported\n\n");
		process::exit(0);
	}

	char	*f=NULL;
	if (!charstring::compare(os,"Windows")) {
		char	*f=charstring::duplicate(
				"C:\\Windows\\System32\\msvcrt.dll");
	} else {
		// decide on a file to use
		const char	*dirs[]={
			"/lib64","/usr/lib64","/lib","/usr/lib",
			// FIXME: other architectures
			"/lib/x86_64-linux-gnu",
			"/usr/lib/x86_64-linux-gnu",
			NULL
		};
		for (const char * const *dir=dirs; *dir; dir++) {
			directory	d;
			if (d.open(*dir)) {
				d.rewind();
				for (;;) {
					char	*file=d.read();
					if (!file) {
						break;
					}
					if ((!charstring::compare(
							file,"libc-",5) &&
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
	}
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

	stdoutput.printf("\n");
}
