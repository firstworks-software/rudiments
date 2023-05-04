// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHAREDMEMORY_H
#define RUDIMENTS_SHAREDMEMORY_H

#include <rudiments/private/sharedmemoryincludes.h>

/** Shared memory segments allow multiple processes to access a common
 *  address space.
 * 
 *  Shared memory can be used for interprocess communication and is generally
 *  faster than using fifo's or unix sockets.
 * 
 *  To synchronize access to shared memory, use the semaphoreset class.
 * 
 *  A shared memory segment is owned by a user and group and has access 
 *  permissions just like a file. */
class RUDIMENTS_DLLSPEC sharedmemory : public object {
	public:
		/** Creates an instance of the sharedmemory class. */
		sharedmemory();

		/** Deletes this instance of the sharedmemory class.
 		 *  Removes the shared memory segment if it was created by
 		 *  this class.  If it was just attached to, it is not
 		 *  removed. */
		~sharedmemory();

		/** Creates a shared memory segment of "size" bytes, 
		 *  identified by "key".  "key" should be generated using the
		 *  ftok function.  "permissions" sets the access permissions
		 *  for the segment. */
		bool	create(key_t key, size_t size, mode_t permissions);

		/** Attaches to a shared memory segment of "size" bytes,
		 *  identified by "key". */
		bool	attach(key_t key, size_t size);

		/** Attempts to create a shared memory segment of "size" bytes,
		 *  identified by "key".  If this fails, it attempts to attach
		 *  to a shared memory segment identified by "key". */
		bool	createOrAttach(key_t key, size_t size,
						mode_t permissions);

		/** By default, the destructor will remove a sharemd memory
		 *  segment if it was created during a call to create() or
		 *  createOrAttach().
		 *
		 *  This method allows you to manually override that behavior.
		 *
		 *  If "remove" is true then the destructor will remove the
		 *  shared memory segment whehter it was created or not.  If
		 *  "remove" is set false then the destructor will not remove
		 *  the shared memory segment, even if it was created.
		 *
		 *  Manually setting "remove" false is useful, for example, if
		 *  an application creates a shared memory segment then forks
		 *  and wants to delete the shred memory segment in the forked
		 *  process but does not want the shared memory segment removed
		 *  from the system.
		 *
		 *  Manually setting "remove" true is useful, for example, if
		 *  you want to create a utility for removing arbitray
		 *  shared memory segments. */
		void	setRemove(bool remove);

		/** Returns true if the shared memory segment will be removed
 		 *  by the destructor and false otherwise. */
		bool	getRemove();

		/** Returns the internal id for the 
		 *  shared memory segment. */
		int32_t	getId();

		/** Returns a pointer to the shared memory
		 *  segment.  Data may be read from or written
		 *  to the segment using this pointer. */
		void	*getPointer();

		/** Makes this shared memory segment owned by
		 *  the user "username".
		 * 
		 *  Note that setUserName() uses the userentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the userentry
		 *  class a mutex.  See userentry.h for more detail. */
		bool	setUserName(const char *username);

		/** Makes this shared memory segment owned by
		 *  the group "groupname".
		 * 
		 *  Note that setGroupName() uses the groupentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the groupentry
		 *  class a mutex.  See groupentry.h for more detail. */
		bool	setGroupName(const char *groupname);

		/** Makes this shared memory segment owned by
		 *  the user identified by uid. */
		bool	setUserId(uid_t uid);

		/** Makes this shared memory segment owned by
		 *  the group identified by gid. */
		bool	setGroupId(gid_t gid);

		/** Sets the access permissions for this
		 *  shared memory segment to "permissions". */
		bool	setPermissions(mode_t permissions);

		/** Returns the name of the user that owns this
		 *  shared memory segment
		 * 
		 *  Note that getUserName() uses the userentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the userentry
		 *  class a mutex.  See userentry.h for more detail. */
		const char	*getUserName();

		/** Returns the name of the group that owns this
		 *  shared memory segment
		 * 
		 *  Note that getGroupName() uses the groupentry class.
		 *  If you are using this method in a multithreaded
		 *  application, you may need to supply the groupentry
		 *  class a mutex.  See groupentry.h for more detail. */
		const char	*getGroupName();

		/** Returns the user id of the user that owns this
		 *  shared memory segment. */
		uid_t	getUserId();

		/** Returns the group id of the group that owns this
		 *  shared memory segment. */
		gid_t	getGroupId();

		/** Returns the access permissions for this
		 *  shared memory segment. */
		mode_t	getPermissions();

		/** Returns true if the platform supports shared memory and
		 *  false otherwise. */
		static bool	isSupported();

	#include <rudiments/private/sharedmemory.h>
};

#endif
