// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/listener.h>

class modemutilprivate;

class RUDIMENTS_DLLSPEC modemutil {
	public:
			modemutil();
		virtual	~modemutil();

		void	setParameters(const char *device, const char *baud);

	protected:
		const char	*_devicename();
		const char	*_baud();

		bool	configureSerialPort(int32_t fd, const char *baud);

	private:
		modemutilprivate	*pvt;

};

#endif
