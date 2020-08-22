// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVCSECURITY_H
#define RUDIMENTS_MVCSECURITY_H 1

#include <rudiments/private/dll.h>

class RUDIMENTS_DLLSPEC mvcsecurity {
	public:
		mvcsecurity() {};
		virtual	~mvcsecurity() {};
		virtual const char	*getUserName()=0;
};

#endif
