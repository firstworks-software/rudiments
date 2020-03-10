// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVCTIER_H
#define RUDIMENTS_MVCTIER_H 1

#include <rudiments/private/mvctierincludes.h>

class mvctier {
	public:
		mvctier();
		virtual ~mvctier();

		void		setProperties(mvcproperties *prop);
		mvcproperties	*getProperties();

		void		setSecurity(mvcsecurity *sec);
		mvcsecurity	*getSecurity();

	#include <rudiments/private/mvctier.h>
};

#endif
