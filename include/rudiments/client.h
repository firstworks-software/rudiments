// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENT_H
#define RUDIMENTS_CLIENT_H

#include <rudiments/private/clientincludes.h>

/** The client class provides a base class for classes that implement the
 *  client side of a client-server communication model. */
class RUDIMENTS_DLLSPEC client : public filedescriptor {
	public:
		/** Creates an instance of the client class. */
		client();

		/** Deletes this instance of the client class. */
		virtual ~client();

		/** Queries "cd" for "timeoutsec", "timeoutusec", "retrywait"
		 *  and "tries" and configures this instancec to use them when
		 *  connect() is called. */
		virtual void	setParameters(
				dictionary<const char *, const char *> *cd);

		/** Sets the number of seconds that a connect() attempt will
		 *  wait before giving up.  If set to -1 then connect() will
		 *  wait forever.
		 *
		 *  Defaults to -1. */
		void	setTimeoutSeconds(int32_t timeoutsec);

		/** Sets the number of microseconds (in addition to the number
		 *  of seconds set by setTimeoutSeconds()) that a connect()
		 *  attempt will wait before giving up.  If set to -1 then
		 *  connect() will wait forever.
		 *
		 *  Defaults to -1. */
		void	setTimeoutMicroseconds(int32_t timeoutusec);

		/** Sets the number of seconds that connect() will wait after
		 *  failing before trying again.
		 *
		 *  Defaults to 0. */
		void	setRetryWait(uint32_t retrywait);

		/** Sets the number of attemps that connect() will make before
		 *  giving up.  If set to 0 then connect() will attempt to
		 *  connect forever.  If set to 1 then connect() will only
		 *  attempt to connect once.
		 *
		 *  Defaults to 0. */
		void	setTries(uint32_t tries);

		/** Sets the verbose connect error.  May be called by a child
		 *  class to set the error that occurred during connect, if it
		 *  is appropriate to give more information that would be given
		 *  by error::getErrorString() or if no system-level error
		 *  occurred. */
		void	setVerboseConnectError(const char *error);

		/** Returns the number of seconds that a connect() attempt will
		 *  wait before giving up. */
		int32_t	getTimeoutSeconds();

		/** Returns the number of microseconds (in addition to the
		 *  number of seconds set by setTimeoutSeconds()) that a
		 *  connect() attempt will wait before giving up. */
		int32_t	getTimeoutMicroseconds();

		/** Returns the number of seconds that connect() will wait after
		 *  failing before trying again. */
		uint32_t	getRetryWait();

		/** Returns the number of attemps that connect() will make
		 *  before giving up. */
		uint32_t	getTries();

		/** Returns the verbose connect error. */
		const char	*getVerboseConnectError();

		/** This method may be implemnted by a child class to open a
		 *  connection to a server, presumably specified in the "cd"
		 *  parameter of the setParameters() method.
		 *
		 *  This method return an int32_t instead of a bool so child
		 *  classes that have non atomic connection procedures (such
		 *  as a modem) can implement return codes other than success
		 *  or failure (such as abort).
		 *
		 *  This implementation just returns RESULT_SUCCESS. */
		virtual int32_t	connect();

	#include <rudiments/private/client.h>
};

#endif
