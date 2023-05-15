// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOCALE_H
#define RUDIMENTS_LOCALE_H

#include <rudiments/private/localeincludes.h>

/** The locale class provides static methods for accessing and manipulating
 *  locale categories. */
class RUDIMENTS_DLLSPEC locale {
	public:
		/** Returns the value of "category". */
		static	const char	*getValue(const char *category);

		/** Sets the value of "category" to "value", overwriting any
		 *  value that "category" previously had.
		 *
		 *  "category" may be any LC_* category supported by the
		 *  system.  Most systems support:
		 *
		 *  LC_ALL
		 *  LC_COLLATE
		 *  LC_CTYPE
		 *  LC_MESSAGES
		 *  LC_MONETARY
		 *  LC_NUMERIC
		 *  LC_TIME
		 *
		 *  Some systems also support:
		 *
		 *  LC_ADDRESS
		 *  LC_IDENTIFICATION
		 *  LC_MEASUREMENT
		 *  LC_NAME
		 *  LC_PAPER
		 *  LC_TELEPHONE
		 *
		 *  Your system may support additional locale categories.
		 *
		 *  Returns true on success and false on failure. */
		static	bool	setValue(const char *category,
						const char *value);

		/** Returns a NULL terminated list of supported locale
		 *  categories. */
		static	const char * const	*getCategories();

		/** Returns true if the platform supports locales and
		 *  false otherwise. */
		static bool	isSupported();

	#include <rudiments/private/locale.h>
};

#endif
