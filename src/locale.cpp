// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/locale.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

const char *locale::getValue(const char *category) {
	return setLocale(category,NULL);
}

bool locale::setValue(const char *category, const char *value) {
	return setLocale(category,value);
}

const char *locale::setLocale(const char *category, const char *value) {
	#ifdef RUDIMENTS_HAVE_SETLOCALE
		int	cat;
		bool	found=false;
		#ifdef LC_ALL
			if (!charstring::compare(category,"LC_ALL")) {
				cat=LC_ALL;
				found=true;
			}
		#endif
		#ifdef LC_COLLATE
			if (!charstring::compare(category,"LC_COLLATE")) {
				cat=LC_COLLATE;
				found=true;
			}
		#endif
		#ifdef LC_CTYPE
			if (!charstring::compare(category,"LC_CTYPE")) {
				cat=LC_CTYPE;
				found=true;
			}
		#endif
		#ifdef LC_MESSAGES
			if (!charstring::compare(category,"LC_MESSAGES")) {
				cat=LC_MESSAGES;
				found=true;
			}
		#endif
		#ifdef LC_MONETARY
			if (!charstring::compare(category,"LC_MONETARY")) {
				cat=LC_MONETARY;
				found=true;
			}
		#endif
		#ifdef LC_NUMERIC
			if (!charstring::compare(category,"LC_NUMERIC")) {
				cat=LC_NUMERIC;
				found=true;
			}
		#endif
		#ifdef LC_TIME
			if (!charstring::compare(category,"LC_TIME")) {
				cat=LC_TIME;
				found=true;
			}
		#endif
		#ifdef LC_ADDRESS
			if (!charstring::compare(category,"LC_ADDRESS")) {
				cat=LC_ADDRESS;
				found=true;
			}
		#endif
		#ifdef LC_IDENTIFICATION
			if (!charstring::compare(category,
						"LC_IDENTIFICATION")) {
				cat=LC_IDENTIFICATION;
				found=true;
			}
		#endif
		#ifdef LC_MEASUREMENT
			if (!charstring::compare(category,"LC_MEASUREMENT")) {
				cat=LC_MEASUREMENT;
				found=true;
			}
		#endif
		#ifdef LC_NAME
			if (!charstring::compare(category,"LC_NAME")) {
				cat=LC_NAME;
				found=true;
			}
		#endif
		#ifdef LC_PAPER
			if (!charstring::compare(category,"LC_PAPER")) {
				cat=LC_PAPER;
				found=true;
			}
		#endif
		#ifdef LC_TELEPHONE
			if (!charstring::compare(category,"LC_TELEPHONE")) {
				cat=LC_TELEPHONE;
				found=true;
			}
		#endif
		if (!found) {
			error::setErrorNumber(ENOTSUP);
			return NULL;
		}
		return setlocale(cat,value);
	#else
		error::setErrorNumber(ENOTSUP);
		return NULL;
	#endif
}

static const char *categories[]={
	#ifdef LC_ALL
		"LC_ALL",
	#endif
	#ifdef LC_COLLATE
		"LC_COLLATE",
	#endif
	#ifdef LC_CTYPE
		"LC_CTYPE",
	#endif
	#ifdef LC_MESSAGES
		"LC_MESSAGES",
	#endif
	#ifdef LC_MONETARY
		"LC_MONETARY",
	#endif
	#ifdef LC_NUMERIC
		"LC_NUMERIC",
	#endif
	#ifdef LC_TIME
		"LC_TIME",
	#endif
	#ifdef LC_ADDRESS
		"LC_ADDRESS",
	#endif
	#ifdef LC_IDENTIFICATION
		"LC_IDENTIFICATION",
	#endif
	#ifdef LC_MEASUREMENT
		"LC_MEASUREMENT",
	#endif
	#ifdef LC_NAME
		"LC_NAME",
	#endif
	#ifdef LC_PAPER
		"LC_PAPER",
	#endif
	#ifdef LC_TELEPHONE
		"LC_TELEPHONE",
	#endif
	NULL
};

const char * const *locale::getCategories() {
	return categories;
}

bool locale::isSupported() {
	#ifdef RUDIMENTS_HAVE_SETLOCALE
		return true;
	#else
		return false;
	#endif
}
