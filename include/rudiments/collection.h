// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COLLECTION_H
#define RUDIMENTS_COLLECTION_H

#include <rudiments/object.h>
#include <rudiments/comparator.h>

/** The collection class is the parent class for all rudiments collections. */
class collection : virtual public object {
	public:

		/** Creates an instance of the collection class. */
		collection();

		/** Creates an instance of the collection class that is a copy
		 *  of "c". */
		collection(const collection &c);

		/** Makes this instance of the collection class
		 *  identical to "c". */
		collection &operator=(const collection &c);

		/** Deletes this instance of the collection class. */
		virtual	~collection();

		/** Returns a string representing the base type of the
		 *  collection, eg. list, dictionary, tree, etc. */
		virtual const char	*getType() const=0;

		/** Returns the comparator used internally by the class.
		 *  Returns whatever was previously set by setComparator() or
		 *  an instance of the comparator class by default. */
		comparator	*getComparator() const;

		/** Sets the comparator used by the class.  Reverts to the
		 *  default comparator if "newcomp" is NULL. */
		void	setComparator(comparator *newcomp);

		/** Returns true for read-only implementations and false for
		 *  read-write implementations.
		 *
		 *  Returns false by default. */
		virtual bool		getIsReadOnly() const;

		/** Returns true for block-based implementations and false for
		 *  monolithic implementations.
		 *
		 *  Returns false by default. */
		virtual bool		getIsBlockBased() const;

		/** Returns the block size for block-based implementations and
		 *  0 for monolithic implementations.
		 *
		 *  Returns 0 by default. */
		virtual uint64_t	getBlockSize() const;

		/** Returns true for sequential-access implementations and
		 *  false for random-access implementations.
		 *
		 *  Returns false by default. */
		virtual bool		getIsSequentialAccess() const;

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
		virtual	bool	getManageValues() const;

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
		virtual	bool	getManageArrayValues() const;

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
		virtual	bool	getManageKeys() const;

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
		virtual	bool	getManageArrayKeys() const;

		/** Emptyies the collection. */
		virtual	void	clear()=0;

		/** Writes a representation of the collection to standard
		 *  output. */
		virtual	ssize_t	write() const=0;

		/** Writes a representation of the collection to "out". */
		virtual	ssize_t	write(output *out) const=0;

		/** Writes a JSON representation of the collection to standard
		 *  output. */
		virtual	ssize_t	writeJson() const=0;

		/** Writes a JSON representation of the collection to standard
		 *  output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent) const=0;

		/** Writes a JSON representation of the collection to "out". */
		virtual	ssize_t	writeJson(output *out) const=0;

		/** Writes a JSON representation of the collection to "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent) const=0;

	#include <rudiments/private/collection.h>
};

#include <rudiments/private/collectioninlines.h>

#endif
