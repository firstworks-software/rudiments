// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_PROPERTIES_H
#define RUDIMENTS_PROPERTIES_H 1

#include <rudiments/private/mvcpropertiesincludes.h>

class mvcproperties {
	public:
		mvcproperties();
		virtual	~mvcproperties();

		bool		parseFile(const char *filename);
		bool		parseString(const char *string);

		const char	*getValue(const char *parameter);

	#include <rudiments/private/mvcproperties.h>
};

#endif
