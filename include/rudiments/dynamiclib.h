// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICLIB_H
#define RUDIMENTS_DYNAMICLIB_H

#include <rudiments/private/dynamiclibincludes.h>

/** The dynamiclib class provides methods for loading, unloading and calling
 *  functions from dynamically linked libraries. */
class RUDIMENTS_DLLSPEC dynamiclib : public object {
	public:

		/** Creates an instance of the dynamiclib class. */
		dynamiclib();

		/** Deletes this instance of the dynamiclib class. */
		~dynamiclib();

		/** Opens library "library".  If "loaddependencies"
		 *  is true, then all libraries required by this library
		 *  are also loaded, if it is false, they are loaded
		 *  later, as needed.  If "global" is true, then the
		 *  symbols defined in the library are made available
		 *  to libraries which are loaded later.
		 * 
		 *  Returns true on success and false on failure. */
		bool	open(const char *library,
				bool loaddependencies,
				bool global);

		/** Opens library "library" in a new, isolated namespace.
		 *  If "loaddependencies" is true, then all libraries required
		 *  by this library are also loaded, if it is false, they are
		 *  loaded later, as needed.  If "global" is true, then the
		 *  symbols defined in the library are made available to
		 *  libraries which are loaded later, into the same namespace.
		 * 
		 *  Returns true on success and false on failure. */
		bool	openInNewNamespace(
				const char *library,
				bool loaddependencies,
				bool global);

		/** Opens library "library" in the isolated namespace "nsid".
		 *  If "loaddependencies" is true, then all libraries required
		 *  by this library are also loaded, if it is false, they are
		 *  loaded later, as needed.  If "global" is true, then the
		 *  symbols defined in the library are made available to
		 *  libraries which are loaded later, into the same namespace.
		 * 
		 *  Returns true on success and false on failure. */
		bool	openInNamespace(
				const char *library,
				uint64_t nsid,
				bool loaddependencies,
				bool global);

		/** Returns the namespace that the library opened by the most
		 * recent call to open(), openInNewNamespace(), or
		 * openInNamespace() was opened into. */
		uint64_t	getNamespace();

		/** Closes and unloads the previously opened library. */
		bool	close();

		/** Returns a handle to "symbol" (exported function or
		 *  variable) in the currently open library or NULL if
		 *  an error occurs or if no library is currently open. */
		void	*getSymbol(const char *symbol);

		/** Returns a handle to the next definition of "symbol" in
		 *  the load order after the caller (the platform's RTLD_NEXT),
		 *  so an interposed function can call through to the real
		 *  one, or NULL if there is no such definition or the
		 *  platform has no equivalent.  No library need be open. */
		static void	*getNextSymbol(const char *symbol);

		/** Returns a human-readable description of the previous
		 *  error that occurred, or NULL if no error has
		 *  occurred.
		 * 
		 *  This method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate
		 *  the buffer. */
		char	*getError();
		
		/** getError() is not reentrant and thus not thread safe.  If
		 *  your application is multi-threaded and you use getError(),
		 *  you must use this method to supply a mutex and ensure thread
		 *  safety.
		 * 
		 *  If you don't supply a mutex, getError() will still work, but
		 *  will not be thread-safe. */
		static	void	setErrorMutex(threadmutex *mtx);


		/** Returns true if rudiments supports namespaces on this
		 *  platform, and false otherwise. */
		static bool	supportsNamespaces();

	#include <rudiments/private/dynamiclib.h>
};

#endif
