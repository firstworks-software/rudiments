// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/locale.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include <rudiments/sys.h>
#include "test.cpp"

static const char *categories[]={
	"LC_ALL",
	"LC_COLLATE",
	"LC_CTYPE",
	"LC_MESSAGES",
	"LC_MONETARY",
	"LC_NUMERIC",
	"LC_TIME",
	"LC_ADDRESS",
	"LC_IDENTIFICATION",
	"LC_MEASUREMENT",
	"LC_NAME",
	"LC_PAPER",
	"LC_TELEPHONE",
	NULL
};

const char	*l="en_US.UTF-8";

int main(int argc, const char **argv) {

	header("locale");

	// bail if not supported
	if (!locale::isSupported()) {
		stdoutput.printf("not supported\n");
		return 0;
	}

	// NetBSD has some strangeness
	char	*osname=sys::getOperatingSystemName();
	bool	isnetbsd=!charstring::compare(osname,"NetBSD");
	delete[] osname;

	stringbuffer	title;

	// default locale
	stdoutput.printf("default locale (C):\n");
	for (const char * const *cat=categories; *cat; cat++) {
		if (!charstring::isInSet(*cat,locale::getCategories())) {
			continue;
		}
		title.append("get ")->append(*cat)->append("=C");
		test(title.getString(),
			!charstring::compare(locale::getValue(*cat),"C"));
		title.clear();
	}

	// set LC_ALL to another locale
	stdoutput.printf("set LC_ALL to %s locale:\n",l);
	title.append("set LC_ALL=")->append(l);
	test(title.getString(),locale::setValue("LC_ALL",l));
	title.clear();
	for (const char * const *cat=categories; *cat; cat++) {

		// on NetBSD, LC_ALL isn't just a single value, but rather
		// a /-delimited list of the values for the other categories,
		// eg. C/en_US.UTF-8/en_US.UTF-8/...
		// also, LC_COLLATE is always set to C
		// so, skip both of those on NetBSD
		if (isnetbsd && (!charstring::compare(*cat,"LC_ALL") ||
				!charstring::compare(*cat,"LC_COLLATE"))) {
			continue;
		}

		if (!charstring::isInSet(*cat,locale::getCategories())) {
			continue;
		}
		title.append("get ")->append(*cat)->append("=")->append(l);
		test(title.getString(),
			!charstring::compare(locale::getValue(*cat),l));
		title.clear();
	}

	// set individual categories back to C
	stdoutput.printf("individuals back to C:\n");
	for (const char * const *cat=categories+1; *cat; cat++) {

		if (!charstring::isInSet(*cat,locale::getCategories())) {
			continue;
		}

		title.append("set ")->append(*cat)->append("=C");
		test(title.getString(),locale::setValue(*cat,"C"));
		title.clear();

		title.append("get ")->append(*cat)->append("=C");
		test(title.getString(),
			!charstring::compare(locale::getValue(*cat),"C"));
		title.clear();

		bool	notequalsc=true;
		for (const char * const *ocat=cat+1; *ocat; ocat++) {
			if (!charstring::isInSet(*cat,
					locale::getCategories())) {
				continue;
			}
			if (!charstring::compare(locale::getValue(*ocat),"C")) {
				notequalsc=false;
			}
		}
		test("others!=C",notequalsc);
	}

	return 0;
}
