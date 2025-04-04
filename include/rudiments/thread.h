// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_THREAD_H
#define RUDIMENTS_THREAD_H

#include <rudiments/private/threadincludes.h>

/** The thread class provides a simple thread implementation. */
class RUDIMENTS_DLLSPEC thread : virtual public object {
	public:
		/** Creates an instance of the thread class. */
		thread();

		/** Deletes this instance of the thread class. */
		virtual	~thread();

		/** Sets this thread's stack size to "stacksize".  Returns true
		 *  on success and false if an error occurred. */
		bool	setStackSize(size_t stacksize);

		/** Sets "stacksize" to this thread's stack size.  Returns true
		 *  on success and false if an error occurred. */
		bool	getStackSize(size_t *stacksize);

		/** Starts a new thread by running whatever function was set
		 *  by the setFunction() method and passing that function
		 *  whatever argument was set by the setArgument() method.
		 *  Returns true on success and false if an error occurred. */
		bool	spawn(void *(*function)(void *),
					void *arg, bool detached);

		/** Causes a thread to exit with the value stored in the
		 *  location pointed to by "status".  Unless the calling
		 *  thread is detached, if another thread is waiting on this
		 *  thread using wait(), then that thread's wait() method will
		 *  fall through.  */
		void	exit(int32_t *status);

		/** Waits for the spawn()ed function to call exit() or return.
		 *  This method should be called by a second thread.
		 *  If non-NULL, "status" will be set to the exit status of the
		 *  thread.  Returns true on success and false if an error
		 *  occurred. */
		bool	wait(int32_t *status);

		/** Detaches the thread so that it may exit without calling
		 *  exit() and without another thread needing to call wait().
		 *  Returns true on success and false if an error occurred. */
		bool	detach();

		/** Sends signal "signum" to the thread. */
		bool	raiseSignal(int32_t signum);

		/** If "retry" is true then spawn() calls will be automatically
		 *  retried if they fail eg. because of insufficient system
		 *  resources.  If "retry" is set false then, if a spawn()
		 *  fails, the system error is set to EAGAIN and spawn()
		 *  returns false.
		 *
		 *  Defaults to true. */
		void	setRetryFailedSpawn(bool retry);

		/** Returns true if failed spawn() calls will be retried and
		 *  false otherwise. */
		bool	getRetryFailedSpawn();

		/** Returns true if the platform supports threads and rudiments
		 *  was built with thread support and false otherwise.
		 *
		 *  See also: isReliable(). */
		static bool	isSupported();

		/** Returns true if the platform supports threads, and
		 *  rudiments was built with thread support, and this
		 *  platform's thread implementation is bellieved to be
		 *  reliable.
		 *
		 *  Returns false if the platform doesn't support threads,
		 *  rudiments was not built with thread support, or this
		 *  platform's thread implementation is bellieved to be
		 *  unreliable. */
		static bool	isReliable();

	#include <rudiments/private/thread.h>
};

#endif
