// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROCESS_H
#define RUDIMENTS_PROCESS_H

#include <rudiments/private/processincludes.h>

enum childstatechange {
	EXIT_CHILDSTATECHANGE=0,
	TERMINATED_CHILDSTATECHANGE,
	STOPPED_CHILDSTATECHANGE,
	CONTINUED_CHILDSTATECHANGE
};

/** The process class provides static methods for accessing information about
 *  and controlling processes, including methods for forking, spawning and
 *  executing child processes. */
class RUDIMENTS_DLLSPEC process {
	public:

		/** Checks for filename "filename" and reads the
		 *  process id out of it, if it exists.  Returns
		 *  the process id on success or -1 on failure. */
		static int64_t	checkForPidFile(const char *filename);

		/** Create's file "filename" with permissions
		 *  "permissions" and puts the current process
		 *  id in it.  Note that when you delete this
		 *  file during shutdown you must use the full
		 *  pathname since the process::detach() method
		 *  changes directories to "/".  Returns true on
		 *  success and false on failure. */
		static bool	createPidFile(const char *filename,
						mode_t permissions);

		/** Returns the process id of the current process. */
		static	pid_t	getProcessId();

		/** Returns the process id of the parent process
		 *  of the current process. */
		static	pid_t	getParentProcessId();

		/** Returns the process group id of the
		 *  current process. */
		static	pid_t	getProcessGroupId();

		/** Returns the process group id of the
		 *  process pid. */
		static	pid_t	getProcessGroupId(pid_t pid);

		/** Sets the process group id of the current
		 *  process to the current process id. */
		static	bool	setProcessGroupId();

		/** Sets the process group id of the current process to pgid. */
		static	bool	setProcessGroupId(pid_t pgid);

		/** Sets the process group id of the process pid to pgid. */
		static	bool	setProcessGroupId(pid_t pid, pid_t pgid);

		/** Returns the session id of the current process. */
		static	pid_t	getSessionId();

		/** Returns the session id of the process pid. */
		static	pid_t	getSessionId(pid_t pid);

		/** Creates a new session, sets the calling process to be the
		 *  process group leader and detaches from the controlling
		 *  terminal.  Returns the session id on success or -1 if an
		 *  error occurred. */
		static	pid_t	createNewSession();

		/** Returns the real user id of the current process. */
		static	uid_t	getUserId();

		/** Returns the effective user id of the current process. */
		static	uid_t	getEffectiveUserId();

		/** Sets the effective user id of the current process to uid.
		 *  If the effective user id is root, the real and saved user
		 *  ids are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setUserId(uid_t uid);

		/** Sets the effective user id of the current process to the
 		 *  user id of "username".  If the effective user id is root,
 		 *  the real and saved user ids are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setUser(const char *username);

		/** Sets the effective user id of the current process to uid.
		 *  Does not set the real or saved user ids.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveUserId(uid_t uid);

		/** Sets the effective user id of the current process to the
 		 *  user id of "username".  Does not set the real or saved
 		 *  user ids.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveUser(const char *username);

		/** Sets the real user id of the current process to uid and the
		 *  effective user id of the current process to euid.  If the
		 *  real user id is set or the effective user id is set to a
		 *  value not equal to the previous real user id, the saved
		 *  user id is set to the new effective user id.
		 * 
		 *  Returns true on success and false on failure. */
		static bool	setRealAndEffectiveUserId(uid_t uid,
								uid_t euid);

		/** Returns the real group id of the current process. */
		static	gid_t	getGroupId();

		/** Returns the effective group id of the current process. */
		static	gid_t	getEffectiveGroupId();

		/** Sets the effective group id of the current process to gid.
		 *  If the effective group id is root, the real and saved group
		 *  ids are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setGroupId(gid_t gid);

		/** Sets the effective group id of the current process to the
 		 *  group id of "groupname".  If the effective group id is root,
 		 *  the real and saved group ids are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setGroup(const char *groupname);

		/** Sets the effective group id of the current process to gid.
		 *  Does not set the real or saved group ids.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveGroupId(gid_t gid);

		/** Sets the effective group id of the current process to the
 		 *  group id of "groupname".  Does not set the real or saved
 		 *  group ids.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveGroup(const char *groupname);

		/** Sets the real group id of the current process to gid and
		 *  the effective group id of the current process to egid.  If
		 *  the real group id is set or the effective group id is set
		 *  to a value not equal to the previous real group id, the
		 *  saved group id is set to the new effective group id.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setRealAndEffectiveGroupId(gid_t gid,
								gid_t egid);

		/** Set file/directory creation mode mask to "mask".  Returns
		 *  the mask that was previously in effect. */
		static mode_t	setFileCreationMask(mode_t mask);

		/** Creates a child process.  The child is a duplicate of the
		 *  parent inheriting file descriptors and a copy of the
		 *  parent's address space.  The child does not have access to
		 *  the parent's address space.  In the parent process, the
		 *  process id of the child is returned.  In the child process,
		 *  0 is returned.  -1 is returned if an error occurred and no
		 *  child is forked. */
		static pid_t	fork();

		/** Returns true of this platform supports fork() and false
		 *  otherwise. */
		static bool	supportsFork();

		/** Runs "command" with arguments "args", replacing the current
		 *  running process with this new process.  Whether the new
		 *  process retains the process id of the current process is
		 *  platform-specific.  Note that the first element of array
		 *  "args" should be the name of the command that you want to
		 *  run, typically the same as "command".  "args" should be NULL
		 *  terminated.  Returns false if an error occurred and
		 *  otherwise does not return at all. */
		static bool	exec(const char *command,
					const char * const *args);

		/** Runs "command" with arguments "args" as a new process.
		 *  If "detached" is true then the process is started in
		 *  the background.
		 *  Use this instead of a combinination of fork() and exec()
		 *  as it is more efficient on some platforms.
		 *  Note that the first element of array "args" should be the
		 *  name of the command that you want to run, typically the
		 *  same as "command".  "args" should be NULL terminated.
		 *  Returns the process id of the child process or -1 if an
		 *  error occurred. */
		static pid_t	spawn(const char *command,
					const char * const *args,
					bool detached);

		/** Detaches the current process from the controlling tty,
		 *  creates a new session, changes directory to / and sets the
		 *  file creation mask to 0.  Use this method to "fork your
		 *  process into the background."  Returns true on success and
		 *  false on failure. */
		static bool	detach();

		/** Terminates the calling process and sets the exit status to
		 *  "status". */
		static void	exit(int32_t status);

		/** Terminates the calling process "immediately" (without
		  * calling any functions registered to run at exit) and sets
		  * the exit status to "status". */
		static void	exitImmediately(int32_t status);

		/** Send signal "signum" to process "processid".
		 *  Returns true on success and false on failure. */
		static	bool	sendSignal(pid_t processid, int32_t signum);

		/** Send signal "signum" to self.
		 *  Returns true on success and false on failure. */
		static	bool	raiseSignal(int32_t signum);
		
		/** Calls exitOnShutDown(), exitOnCrash() below. */
		static	void	exitOnCrashOrShutDown();

		/** Sets up a default handler that calls exitImmediately()
		 *  when the process receives with a shutdown signal:
		 *  * SIGINT - interrupt (ctrl-C)
		 *  * SIGTERM - terminate (kill)
		 *  * SIGQUIT - quit (ctrl-\)
		 *  * SIGHUP - hangup (terminal disconnected)
		 *  * SIGXCPU - cpu consumption soft limit exceeded
		 *  * SIGXFZ - file size limit exceeded
		 *  * SIGPWR - ACPI (or similar) power down */
		static void	exitOnShutDown();

		/** Sets up a default handler that calls exitImmediately() if
		 *  the process crashes with a program error signal -
		 *  SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGIOT,
		 *  SIGEMT or SIGSYS. */
		static void	exitOnCrash();
		
		/** Calls setShutDownFlagOnShutDown(),
		 *  setShutDownFlagOnCrash() below. */
		static	void	setShutDownFlagOnCrashOrShutDown();

		/** Sets up a default handler that sets the global shutdown
		 *  flag (see setShutDownFlag()) to true when the process
		 *  receives a termination signal:
		 *  * SIGINT - interrupt (ctrl-C)
		 *  * SIGTERM - terminate (kill)
		 *  * SIGQUIT - quit (ctrl-\)
		 *  * SIGHUP - hangup (terminal disconnected)
		 *  * SIGXCPU - cpu consumption soft limit exceeded
		 *  * SIGXFSZ - file size limit exceeded
		 *  * SIGPWR - ACPI (or similar) power down */
		static void	setShutDownFlagOnShutDown();

		/** Sets up a default handler that sets the global shutdown
		 *  flag (see setShutDownFlag()) to true the process crashes
		 *  with a program error signal - SIGABRT, SIGFPE, SIGILL,
		 *  SIGSEGV, SIGBUS, SIGIOT, SIGEMT or SIGSYS. */
		static void	setShutDownFlagOnCrash();

		/** Allows you to register a function to run when the process
		 *  exits normally.
		 *
		 *  May be called multiple times to register multiple functions
		 *  to be run at exit.  The functions will be called in the
		 *  reverse order of their registration.
		 *
		 *  Note that there is no way to deregister a function from
		 *  running at exit once it has been registered.
		 *
		 *  Returns true on success and false on failure. */
		static bool	registerExitHandler(void (*handler)(void));

		/** Allows you to designate a function to run when the
		 *  process receives a termination signal:
		 *  * SIGINT - interrupt (ctrl-C)
		 *  * SIGTERM - terminate (kill)
		 *  * SIGQUIT - quit (ctrl-\)
		 *  * SIGHUP - hangup (terminal disconnected)
		 *  * SIGXCPU - cpu consumption soft limit exceeded
		 *  * SIGXFZ - file size limit exceeded
		 *  * SIGPWR - ACPI (or similar) power down
		 *
		 *  Each time setShutDownHandler() is called, the shut down
		 *  handler replaces the previously designated handler. */
		static	void	setShutDownHandler(
					void (*shutdownhandler)(int32_t));

		/** Allows you to designate a function to run if the
		 *  process crashes with a program error signal -
		 *  SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGIOT,
		 *  SIGEMT or SIGSYS.
		 *
		 *  Each time setCrashHandler() is called, the crash handler
		 *  replaces the previously designated handler. */
		static	void	setCrashHandler(void (*crashhandler)(int32_t));

		/** Sets a global shutdown flag indicating whether or not the
		 *  process has been asked to shut down.
		 *
		 *  For example...
		 *
		 *  The "shutdownhandler" of setShutDownHandler() may simply
		 *  call setShutDownFlag(true) and then return.
		 *
		 *  The process may then call getShutDownFlag() at strategic
		 *  points to see if a shutdown has been requested and shut
		 *  down gracefully if it has.
		 *
		 *  All rudiments methods that retry interrupted system calls
		 *  (see retryInterrupted*() methods of various classes) exit
		 *  if this flag is set upon return from the system call. */
		static	void	setShutDownFlag(bool shutdownflag);

		/** Returns the value of a global shutdown flag indicating
		 *  whether or not the process has been asked to shut down.
		 *
		 *  For example...
		 *
		 *  The "shutdownhandler" of setShutDownHandler() may simply
		 *  call setShutDownFlag(true) and then return.
		 *
		 *  The process may then call getShutDownFlag() at strategic
		 *  points to see if a shutdown has been requested and shut
		 *  down gracefully if it has.
		 *
		 *  All rudiments methods that retry interrupted system calls
		 *  (see retryInterrupted*() methods of various classes) exit
		 *  if this flag is set upon return from the system call. */
		static	bool	getShutDownFlag();

		/** Sets a global variable indicating which signal was received
		 *  that initiated the process shut down.
		 *
		 *  For example...
		 *
		 *  The "shutdownhandler" of setShutDownHandler() may call
		 *  setShutDownSignal(signum), passing it the value of its
		 *  "signum" parameter.
		 *
		 *  Once the process determines that a shutdown has been
		 *  initiated (perhaps by checking getShutDownFlag(), it may
		 *  then call getShutDownSignal() to query which signal was
		 *  received that initiated the shutdown and respond
		 *  differently to different signals. */
		static	void	setShutDownSignal(int32_t signum);

		/** Returns the value of a global variable indicating which
		 *  signal was received that initiated the process shut down.
		 *
		 *  For example...
		 *
		 *  The "shutdownhandler" of setShutDownHandler() may call
		 *  setShutDownSignal(signum), passing it the value of its
		 *  "signum" parameter.
		 *
		 *  Once the process determines that a shutdown has been
		 *  initiated (perhaps by checking getShutDownFlag(), it may
		 *  then call getShutDownSignal() to query which signal was
		 *  received that initiated the shutdown and respond
		 *  differently to different signals. */
		static	int32_t	getShutDownSignal();

		/** Returns the signalhandler used internally to handle a crash.
		 *  Calls to exitOnCrash(), exitOnCrashOrShutDown(),
		 *  setShutDownFlagOnCrash(), or
		 *  setShutDownFlagOnCrashOrShutDown() configure this set to
		 *  include a standard set of crash signals.
		 *  If you would like to add signals, or change the signal
		 *  handler from the default to a custom handler, then you can
		 *  use this method to get the signalhandler. */
		static	signalhandler	*getCrashSignalHandler();

		/** Returns the signalhandler used internally to handle a shut
		 *  down.
		 *  Calls to exitOnShutDown(), exitOnCrashOrShutDown(),
		 *  setShutDownFlagOnShutDown(), or
		 *  setShutDownFlagOnCrashOrShutDown() configure this set to
		 *  include a standard set of shutdown signals.
		 *  If you would like to add signals, or change the signal
		 *  handler from the default to a custom handler, then you can
		 *  use this method to get the signalhandler. */
		static	signalhandler	*getShutDownSignalHandler();

		/** If "wait" is true then this method causes the process will
		 *  automatically wait on child processes which have exited,
		 *  preventing so-called "zombie" processes from occurring.
		 *
		 *  If "wait" is false then this method causes the process not
		 *  to wait on child processes which have exited.  Ordinarily,
		 *  you'd want to wait on child processes, but that interferes
		 *  with the behavior of WEXITSTATUS() after a call to system()
		 *  (and possibly other calls).  This method allows you to
		 *  disable waiting on child processes.
		 *
		 *  Defaults to false. */
		static	void	setWaitForChildren(bool wait);

		/** This methods causes the process to wait until child
		 *  process "pid" exits.  Returns true on success and false
		 *  on failure. */
		static	bool	wait(pid_t pid);

		/** This methods causes the process to wait until child
		 *  process "pid" exits.  If "exitstatus" is non-null then it
		 *  is populated with the exit status of the child process.
		 *  Returns true on success and false on failure. */
		static	bool	wait(pid_t pid, int32_t *exitstatus);

		/** This method causes the process to get information about
		 *  a change in process state for the specified child process
		 *  id "pid" (ie. whether it was stopped, continued or killed).
		 *
		 *  Returns the process id of the child that changed state, -1
		 *  if an error occurred, and 0 if "wait" is set false and no
		 *  child has changed state.
		 *
		 *  Setting "pid" to -1 causes the method to get information on
		 *  any child of the current process.
		 *
		 *  Waiting on child processes to exit prevents so-called
		 *  "zombie" processes from occurring.  However, this method
		 *  only waits when called.  To configure a process to
		 *  automatically wait on and respond when any child process
		 *  exits, use waitForChildren().
		 *
		 *  If "wait" is set true then the method waits until a child
		 *  state-change occurs.  Otherwise the method returns
		 *  immediately.
		 *
		 *  If "ignorestop" is set true then the method ignores when
		 *  a child process has been stopped.
		 *
		 *  If "ignorecontinue" is set true then the method ignores when
		 *  a child process has been continued.
		 *
		 *  On exit, if "newstate" is non-NULL then it is populated with
		 *  one of the members of the childstatechange enum, indicating
		 *  the new state of the child process.
		 *
		 *  If "newstate" is EXIT_CHILDSTATECHANGE and "exitstatus" is
		 *  non-null then "exitstatus" is populated with the exit status
		 *  of the child process.
		 *
		 *  If "newstate" is TERMINATED_CHILDSTATECHANGE or
		 *  STOPPED_CHILDSTATECHANGE and "signum" is non-null then
		 *  "signum" is populated with the signum that terminated or
		 *  stopped the child process.
		 *
		 *  If "newstate" is TERMINATED_CHILDSTATECHANGE and
		 *  "coredump" is non-null then "coredump" is set true if a
		 *  core dump was produced and false otherwise. */
		static pid_t	getChildStateChange(pid_t pid,
						bool wait,
						bool ignorestop,
						bool ignorecontinue,
						childstatechange *newstate,
						int32_t	*exitstatus,
						int32_t *signum,
						bool *coredump);

		/** Returns true if the platform supports waiting for a child
		 *  process to exit or change state and false otherwise. */
		static bool	supportsGetChildStateChange();

		/** If "retry" is true then fork() calls will be automatically
		 *  retried if they fail eg. because of insufficient system
		 *  resources.  If "retry" is set false then, if a fork() fails,
		 *  the system error is set to EAGAIN and fork() returns -1.
		 *
		 *  Defaults to true.*/
		static void	setRetryFailedFork(bool retry);

		/** Returns true if failed fork() calls will be retried and
		 *  false otherwise. */
		static bool	getRetryFailedFork();

		/** Writes the backtrace for the current thread to "buffer".
		 *  
		 *  "maxframes" indicates the maximum number of stack frames
		 *  to include in the backtrace.
		 *
		 *  (Not supported on all platforms.) */
		static void	writeBacktrace(output *out, uint32_t maxframes);

		/** Writes the backtrace for the current thread to "buffer".
		 *  
		 *  A maximum of 128 stack frames will be included in the
		 *  backtrace.
		 *  
		 *  (Not supported on all platforms.) */
		static void	writeBacktrace(output *out);

		/** Appends the backtrace for the current thread to "filename".
		 *  
		 *  If "filename" doesn't already exist then it will be created
		 *  with rw------- permissions.
		 *  
		 *  A maximum of 128 stack frames will be included in the
		 *  backtrace.
		 *  
		 *  (Not supported on all platforms.) */
		static void	writeBacktrace(const char *filename);

		/** Appends the backtrace for the current thread to "filename".
		 *  
		 *  If "filename" doesn't already exist then it will be created
		 *  with "perms" permissions.
		 *
		 *  A maximum of "maxframes" stack frames will be included in
		 *  the backtrace.
		 *
		 *  (Not supported on all platforms.) */
		static void	writeBacktrace(const char *filename,
						mode_t perms,
						uint32_t maxframes);

	#include <rudiments/private/process.h>
};

#endif
