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
		collection(collection &c);

		/** Makes this instance of the collection class
		 *  identical to "c". */
		collection &operator=(collection &c);

		/** Deletes this instance of the collection class. */
		virtual	~collection();

		/** Returns a string representing the base type of the
		 *  collection, eg. list, dictionary, tree, etc. */
		virtual const char	*getType()=0;

		/** Returns the number of elementes in the collection. */
		virtual uint64_t	getCount()=0;

		/** Returns the comparator used internally by the class.
		 *  Returns whatever was previously set by setComparator() or
		 *  an instance of the comparator class by default. */
		comparator	*getComparator();

		/** Sets the comparator used by the class.  Reverts to the
		 *  default comparator if "newcomp" is NULL. */
		void	setComparator(comparator *newcomp);

		/** Returns true for read-only implementations and false for
		 *  read-write implementations.
		 *
		 *  Returns false by default. */
		virtual bool		isReadOnly();

		/** Returns true for block-based implementations and false for
		 *  monolithic implementations.
		 *
		 *  Returns false by default. */
		virtual bool		isBlockBased();

		/** Returns the block size for block-based implementations and
		 *  0 for monolithic implementations.
		 *
		 *  Returns 0 by default. */
		virtual uint64_t	getBlockSize();

		/** Returns true for sequential-access implementations and
		 *  false for random-access implementations.
		 *
		 *  Returns false by default. */
		virtual bool		isSequentialAccess();

		/** Indicates whether or not this instance should "manage" the
		 *  values that are stored at each location in the collection.
		 *
		 *  "manage" should only be set true if the values stored in
		 *  the collections are pointers to objects.
		 *
		 *  Defaults to "false".
		 *
		 *  If "manage" is set to true then:
		 *  * When remove() is called, the object stored as the value
		 *    at that location in the collection will be deleted.
		 *  * When clear() is called, the object stored as the value at
		 *    each location in the collection will be deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    attempt to create a copy of the object stored as the value
		 *    in each location of the rvalue instance.  Note that this
		 *    can crash, fail, or even fail to compile if the objects
		 *    being stored don't have properly implemented copy
		 *    constructors.
		 *  * When the copy constructor is called, the new instance
		 *    will attempt to create a copy of the object stored as the
		 *    value in each location of the instance that is being
		 *    copied.  Note that this can crash, fail, or even fail to
		 *    compile if the objects being stored don't have properly
		 *    implemented copy constructors.
		 *
		 *  If "manage" is set to false then:
		 *  * When remove() is called, the object stored as the value
		 *    at that location in the collection will not be deleted.
		 *  * When clear() is called, the object stored as the value a
		 *    each location in the collection will not be deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    not attempt to create a copy of the object stored as the
		 *    value in each location of the rvalue instance, rather it
		 *    will just copy the value or pointer.
		 *  * When the copy constructor is called, the new instance
		 *    will not attempt to create a copy of the object stored as
		 *    the value in each location of the instance that is being
		 *    copied, rather it will just copy the value or pointer.
		 *
		 *  Note that setting this true implies
		 *  setManageArrayValues(false); */
		virtual	void	setManageValues(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to manage the objects that are stored as the
		 *  values at each location in the collection.
		 *
		 *  See setManageValues().
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageValues();

		/** Indicates whether or not this instance should "manage" the
		 *  values that are stored at each location in the collection
		 *  as arrays.
		 *
		 *  "manage" should only be set true if the values stored in
		 *  the collections are pointers to arrays of primitive types.
		 *
		 *  Defaults to "false".
		 *
		 *  If "manage" is set to true then:
		 *  * When remove() is called, the array stored as the value at
		 *    that location in the collection will be array-deleted.
		 *  * When clear() is called, the array stored as the value at
		 *    each location in the collection will be array-deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    attempt to create a copy of the array stored as the value
		 *    in each location of the rvalue instance.
		 *  * When the copy constructor is called, the new instance
		 *    will attempt to create a copy of the array stored as the
		 *    value in each location of the instance that is being
		 *    copied.
		 *
		 *  If "manage" is set to false then:
		 *  * When remove() is called, the array stored as the value at
		 *    that location in the collection will not be array-deleted.
		 *  * When clear() is called, the array stored as the value at
		 *    each location in the collection will not be array-deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    not attempt to create a copy of the array stored as the
		 *    value in each location of the rvalue instance, rather it
		 *    will just copy the value or pointer.
		 *  * When the copy constructor is called, the new instance
		 *    will not attempt to create a copy of the array stored as
		 *    the value in each location of the instance that is being
		 *    copied, rather it will just copy the value or pointer.
		 *
		 *  To store managed arrays of non-primitive types as values,
		 *  consider using a managed collection of managed staticarrays.
		 *  For example:
		 *
		 *  	linkedlist< staticarray< myobject * > *>	l;
		 *  	l.setManageValues(true);
		 *
		 *  	staticarray< myobject * >	*s1=
		 *  			new staticarray< myobject * >();
		 *  	s1.setManageValues(true);
		 *  	s1[0]=new myobject(...);
		 *  	s1[1]=new myobject(...);
		 *  	s1[2]=new myobject(...);
		 *  	l.append(s1);
		 *
		 *  	staticarray< myobject * >	*s2=
		 *  			new staticarray< myobject * >();
		 *  	s2.setManageValues(true);
		 *  	s2[0]=new myobject(...);
		 *  	s2[1]=new myobject(...);
		 *  	s2[2]=new myobject(...);
		 *  	l.append(s2);
		 *
		 *  	...
		 *
		 *  Note that setting this true implies
		 *  setManageValues(false); */
		virtual	void	setManageArrayValues(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to manage the arrays that are stored as the
		 *  values at each location in the collection.
		 *
		 *  See setManageArrayValues().
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageArrayValues();

		/** Indicates whether or not this instance should "manage" the
		 *  keys that are stored at each location in the collection.
		 *
		 *  "manage" should only be set true if the keys stored in
		 *  the collections are pointers to objects.
		 *
		 *  Defaults to "false".
		 *
		 *  If "manage" is set to true then:
		 *  * When remove() is called, the object stored as the key
		 *    at that location in the collection will be deleted.
		 *  * When clear() is called, the object stored as the key at
		 *    each location in the collection will be deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    attempt to create a copy of the object stored as the key
		 *    in each location of the rvalue instance.  Note that this
		 *    can crash, fail, or even fail to compile if the objects
		 *    being stored don't have properly implemented copy
		 *    constructors.
		 *  * When the copy constructor is called, the new instance
		 *    will attempt to create a copy of the object stored as the
		 *    key in each location of the instance that is being
		 *    copied.  Note that this can crash, fail, or even fail to
		 *    compile if the objects being stored don't have properly
		 *    implemented copy constructors.
		 *
		 *  If "manage" is set to false then:
		 *  * When remove() is called, the object stored as the key
		 *    at that location in the collection will not be deleted.
		 *  * When clear() is called, the object stored as the key a
		 *    each location in the collection will not be deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    not attempt to create a copy of the object stored as the
		 *    key in each location of the rvalue instance, rather it
		 *    will just copy the value or pointer.
		 *  * When the copy constructor is called, the new instance
		 *    will not attempt to create a copy of the object stored as
		 *    the key in each location of the instance that is being
		 *    copied, rather it will just copy the value or pointer.
		 *
		 *  Note that setting this true implies
		 *  setManageArrayKeys(false); */
		virtual	void	setManageKeys(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to manage the objects that are stored as the
		 *  keys at each location in the collection.
		 *
		 *  See setManageKeys().
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageKeys();

		/** Indicates whether or not this instance should "manage" the
		 *  keys that are stored at each location in the collection
		 *  as arrays.
		 *
		 *  "manage" should only be set true if the keys stored in
		 *  the collections are pointers to arrays of primitive types.
		 *
		 *  Defaults to "false".
		 *
		 *  If "manage" is set to true then:
		 *  * When remove() is called, the array stored as the key at
		 *    that location in the collection will be array-deleted.
		 *  * When clear() is called, the array stored as the key at
		 *    each location in the collection will be array-deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    attempt to create a copy of the array stored as the key
		 *    in each location of the rvalue instance.
		 *  * When the copy constructor is called, the new instance
		 *    will attempt to create a copy of the array stored as the
		 *    key in each location of the instance that is being
		 *    copied.
		 *
		 *  If "manage" is set to false then:
		 *  * When remove() is called, the array stored as the key at
		 *    that location in the collection will not be array-deleted.
		 *  * When clear() is called, the array stored as the key at
		 *    each location in the collection will not be array-deleted.
		 *  * When the = operator is called, the lvalue instance will
		 *    not attempt to create a copy of the array stored as the
		 *    key in each location of the rvalue instance, rather it
		 *    will just copy the value or pointer.
		 *  * When the copy constructor is called, the new instance
		 *    will not attempt to create a copy of the array stored as
		 *    the key in each location of the instance that is being
		 *    copied, rather it will just copy the value or pointer.
		 *
		 *  To store managed arrays of non-primitive types as keys,
		 *  consider using a managed collection of managed staticarrays.
		 *  For example:
		 *
		 *  	dictionary<staticarray< myobject * > *,
		 *  					uint32_t >	d;
		 *  	d.setManageKeys(true);
		 *
		 *  	staticarray< myobject * >	*s1=
		 *  			new staticarray< myobject * >();
		 *  	s1.setManageKeys(true);
		 *  	s1[0]=new myobject(...);
		 *  	s1[1]=new myobject(...);
		 *  	s1[2]=new myobject(...);
		 *  	d.setValue(s1,1);
		 *
		 *  	staticarray< myobject * >	*s2=
		 *  			new staticarray< myobject * >();
		 *  	s2.setManageKeys(true);
		 *  	s2[0]=new myobject(...);
		 *  	s2[1]=new myobject(...);
		 *  	s2[2]=new myobject(...);
		 *  	d.setValue(s2,2);
		 *
		 *  	...
		 *
		 *  Note that setting this true implies
		 *  setManageKeys(false); */
		virtual	void	setManageArrayKeys(bool manage);

		/** Returns whether or not this instance of the collection class
		 *  is configured to manage the arrays that are stored as the
		 *  keys at each location in the collection.
		 *
		 *  See setManageArrayValues().
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getManageArrayKeys();

		/** Empties the collection. */
		virtual	bool	clear()=0;

		/** Writes a representation of the collection to standard
		 *  output. */
		virtual	ssize_t	write()=0;

		/** Writes a representation of the collection to "out". */
		virtual	ssize_t	write(output *out)=0;

		/** Writes a JSON representation of the collection to standard
		 *  output. */
		virtual	ssize_t	writeJson()=0;

		/** Writes a JSON representation of the collection to standard
		 *  output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent)=0;

		/** Writes a JSON representation of the collection to "out". */
		virtual	ssize_t	writeJson(output *out)=0;

		/** Writes a JSON representation of the collection to "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent)=0;

	#include <rudiments/private/collection.h>
};

#include <rudiments/private/collectioninlines.h>

#endif
