// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROTOCOLENTRY_H
#define RUDIMENTS_PROTOCOLENTRY_H

#include <rudiments/private/protocolentryincludes.h>

/** The protocolentry class provides methods for retrieving entries from the
 *  protocol file (/etc/protocols on unix). */
class RUDIMENTS_DLLSPEC protocolentry : public object {
	public:

		/** Creates an instance of the protocolentry class. */
		protocolentry();

		/** Deletes this instance of the protocolentry class. */
		~protocolentry();

		/** Opens a protocol entry by name.
		 *  Returns true on success and false on failure. */
		bool	open(const char *protocolname);

		/** Opens a protocol entry by number.
		 *  Returns true on success and false on failure. */
		bool	open(int32_t number);

		/** Returns the name of the protocol entry. */
		const char		*getName();

		/** Returns a NULL-terminated list of aliases for the protocol
		 *  entry. */
		const char * const	*getAliasList();

		/** Returns the number associated with the protocol entry. */
		int32_t			getNumber();

		/** Convenience method.
		 *  Returns the number associated with "protocolname".
		 *
		 *  Returns -1 if an error occurred or if "protocolname"
		 *  is invalid. */
		static	int32_t	getNumber(const char *protocolname);

		/** Convenience method.
		 *  Returns the name of the protocol associated with "number".
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or if "number" is invalid.
		 *  */
		static	char	*getName(int32_t number);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	getNeedsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see getNeedsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/protocolentry.h>
};

#endif
