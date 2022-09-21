// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMCLIENT_H
#define RUDIMENTS_MODEMCLIENT_H

#include <rudiments/private/modemclientincludes.h>

/** The modemclient implements methods for connecting to and communicating with
 *  a remote host using a modem. */
class RUDIMENTS_DLLSPEC modemclient : public client, public modemutil {
	public:

		/** Creates an instance of the modemclient class. */
		modemclient();

		/** Deletes this instance of the modemclient class. */
		virtual		~modemclient();

		/** Queries "cd" for "device", "baud", "connectscript",
		 *  "phonenumber", "disconnectscript", "retrywait" and "tries"
		 *  and configures this instance to use the results when
		 *  connect() is called. */
		void	setParameters(
				dictionary<const char *, const char *> *cd);

		/** Sets the connect script (usually a set of AT commands) that 
		 *  connect() will send to the modem during a connect attempt.
		 *  The connect script may contain $(customatcommands) and
		 *  $(phonenumber) substitution variables, which will be
		 *  replaced with whatever values are set by
		 *  setCustomAtCommands() and setPhoneNumber(). */
		void	setConnectScript(const char *connectscript);

		/** Sets a set of custom AT commands that will replace the
		 *  $(customatcommands) substitution variable in the connect 
		 *  script, if the connect script contains this variable. */
		void	setCustomAtCommands(const char *customatcommands);

		/** Sets a phone number that will replace the $(phonenumber)
		 *  substitution variable in the connect script, if the connect
		 *  script contains this variable. */
		void	setPhoneNumber(const char *phonenumber);

		/** Sets the disconnect script (usually a set of AT commands)
		 *  that close() will send to the modem during close. */
		void	setDisconnectScript(const char *disconnectscript);

		/** Returns the connect script (usually a set of AT commands)
		 *  that connect() will send to the modem during a connect
		 *  attempt. */
		const char	*getConnectScript();

		/** Returns the set of custom AT commands that will replace the
		 *  $(customatcommands) substitution variable in the connect 
		 *  script, if the connect script contains this variable. */
		const char	*getCustomAtCommands();

		/** Returns the phone number that will replace the
		 *  $(phonenumber) substitution variable in the connect script,
		 *  if the connect script contains this variable. */
		const char	*getPhoneNumber();

		/** Returns the disconnect script (usually a set of AT commands)
		 *  that close() will send to the modem during close. */
		const char	*getDisconnectScript();

		/** Dials the modem and attempt to establish a connection.
		 *  If the connection fails, it will retry, for a total of
		 *  "tries" attempts, waiting "retrywait" seconds between
		 *  each attempt.
		 * 
		 *  Setting "tries" to 0 will cause it to try to 
		 *  connect indefinitely.  Setting "retrywait" to 0
		 *  will cause it to try to connect over and over
		 *  as fast as possible (not recommended).
		 * 
		 *  Each attempt to connect will wait "timeoutsec"
		 *  seconds and "timeoutusec" microseconds for the
		 *  connect to succeed.  Specifying -1 for either
		 *  parameter will cause the attempt to wait until the
		 *  underlying protocol times out which may be up to 2
		 *  minutes.
		 * 
		 *  Returns RESULT_SUCCESS on success and RESULT_ERROR
		 *  on failure. */
		int32_t	connect();

		/** Hangs up the modem and closes the device.
		 * 
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/modemclient.h>
};

#endif
