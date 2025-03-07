// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/hostentryincludes.h>

/** The hostentry class provides methods for retrieving entries from the host
 *  file (/etc/hosts on unix). */
class RUDIMENTS_DLLSPEC hostentry : public object {
	public:

		/** Creates an instance of the hostentry class. */
		hostentry();

		/** Deletes this instance of the hostentry class. */
		~hostentry();

		/** Opens a host entry by name. */
		bool	open(const char *hostname);

		/** Opens a host entry by address.
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required. */
		bool	open(const char *address, int32_t len, int32_t type);

		/** Returns the primary hostname of the host entry. */
		const char	*getName();

		/** Returns a NULL-terminated list of aliases for the
		 *  host entry. */
		const char * const	*getAliasList();

		/** Returns the address type of the host entry. */
		int32_t		getAddressType();

		/** Returns the address length of the host entry. */
		int32_t		getAddressLength();

		/** Returns a NULL-terminated list of addresses for the
		 *  host entry. */
		const char * const	*getAddressList();

		/** Returns the "index"'th address of the host entry.
		 *
		 *  Note that this method allocated a buffer internally and
		 *  returns it.  The calling program must free the buffer. */
		char	*getAddressString(int32_t index);

		/** Convenience method.
		 *  Returns the address of "hostname".
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "hostname" is invalid.
		 *  */
		static	char	*getAddressString(const char *hostname);

		/** Convenience method.
		 *  Returns the primary hostname of "address" of length
		 *  "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "address" is invalid.
		 *  */
		static	char	*getName(const char *address,
						int32_t len, int32_t type);

		/** Convenience method.
		 *  Returns a string representation of "address" of length
		 *  "len" and type "type".
		 *
		 *  Note that "address" must be a binary representation of the
		 *  address.  This is why "len" and "type" are also required.
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *
		 *  Returns NULL if an error occurred or "address" is invalid.
		 *  */
		static	char	*getAddressString(const char *address,
						int32_t len, int32_t type);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	getNeedsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see getNeedsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/hostentry.h>
};

#endif
