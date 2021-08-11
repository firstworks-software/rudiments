// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COLLECTION_H
#define RUDIMENTS_COLLECTION_H

#include <rudiments/object.h>

/** The collection class is the parent class for all rudiments collections. */
class RUDIMENTS_DLLSPEC collection : public object {
	public:

		/** Creates an instance of the collection class. */
		collection();

		/** Creates an instance of the collection class that is a copy
		 *  of "c". */
		collection(const collection &c);

		/** Deletes this instance of the collection class. */
		virtual	~collection();

		/** Indicates whether or not this instance of the collection
		 *  class should delete the values that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Setting "manage" to true causes the instance to delete the
		 *  values that are stored at each location in the collection
		 *  when a clear() or remove() method is called.
		 *
		 *  Setting "manage" to false causes the instance not to delete
		 *  the values that are stored at each location in the
		 *  collection when a clear() or remove() method is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageArrayValues(false); */
		virtual	void	setManageValues(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to delete the values that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageValues();

		/** Indicates whether or not this instance of the collection
		 *  class should array-delete the values that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Setting "manage" to true causes the instance to
		 *  array-delete the values that are stored at each location in
		 *  the collection when a clear() or remove() method is called.
		 *
		 *  Setting "manage" to false causes the instance not to
		 *  array-delete the values that are stored at each location in
		 *  the collection when a clear() or remove() method is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageValues(false); */
		virtual	void	setManageArrayValues(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to array-delete the values that are stored at
		 *  each location in the collection when a clear()/remove()
		 *  method is called.
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageArrayValues();

		/** Indicates whether or not this instance of the collection
		 *  class should delete the keys that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Setting "manage" to true causes the instance to delete the
		 *  keys that are stored at each location in the collection
		 *  when a clear() or remove() method is called.
		 *
		 *  Setting "manage" to false causes the instance not to delete
		 *  the keys that are stored at each location in the
		 *  collection when a clear() or remove() method is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageArrayValues(false); */
		virtual	void	setManageKeys(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to delete the keys that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageKeys();

		/** Indicates whether or not this instance of the collection
		 *  class should array-delete the keys that are stored at each
		 *  location in the collection when a clear()/remove() method
		 *  is called.
		 *
		 *  Setting "manage" to true causes the instance to
		 *  array-delete the keys that are stored at each location in
		 *  the collection when a clear() or remove() method is called.
		 *
		 *  Setting "manage" to false causes the instance not to
		 *  array-delete the keys that are stored at each location in
		 *  the collection when a clear() or remove() method is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageValues(false); */
		virtual	void	setManageArrayKeys(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to array-delete the keys that are stored at
		 *  each location in the collection when a clear()/remove()
		 *  method is called.
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageArrayKeys();

	#include <rudiments/private/collection.h>
};

#include <rudiments/private/collectioninlines.h>

#endif
