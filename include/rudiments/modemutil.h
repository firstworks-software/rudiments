// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/private/modemutilincludes.h>

class RUDIMENTS_DLLSPEC modemutil {
	public:
		modemutil();
		virtual	~modemutil();

		void	setDevice(const char *device);
		void	setBaud(const char *baud);

		const char	*getDevice();
		const char	*getBaud();

	#include <rudiments/private/modemutil.h>
};

#endif
