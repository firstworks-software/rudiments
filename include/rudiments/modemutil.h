// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/private/modemutilincludes.h>

/** The modemutil class is just a base class for other classes that use
 *  modems.  It unlikely to need to be used directly. */
class RUDIMENTS_DLLSPEC modemutil {
	public:

		/** Creates an instance of the modemutil class. */
		modemutil();

		/** Deletes this instance of the modemutil class. */
		virtual	~modemutil();

		/** Sets the (serial) device to be used by the class
		 *  Eg. /dev/ttyS0 */
		void	setDevice(const char *device);

		/** Sets the baud rate to be used when talking to the serial
		 *  port.  Eg. 115200  Note that this is not the baud rate
		 *  used by the modem itself.  That must usually be set using
		 *  AT commands or similar. */
		void	setBaud(const char *baud);

		/** Returns the (serial) device name set by the most recent
		 *  call to setDevice(). */
		const char	*getDevice();

		/** Returns the baud rate set by the most recent call to
		 *  setBaud(). */
		const char	*getBaud();

	#include <rudiments/private/modemutil.h>
};

#endif
