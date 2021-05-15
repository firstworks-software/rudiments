// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/dictionaryincludes.h>

/** The dictionarynode class stores the key/value pairs that compose a
 *  dictionary. */
template <class keytype, class valuetype>
class dictionarynode : public object {
	public:
		/** Creates an instance of the dictionary node class 
		 *  with key "key" and value "value". */
		dictionarynode(keytype key, valuetype value);

		/** Deletes this instance of the dictionarynode class.
		 *  Note however, that neither the key nor value stored in the
		 *  dictionarynode are deleted by this call. */
		virtual	~dictionarynode();

		/** Sets the key stored in the node to "key". */
		void	setKey(keytype key);

		/** Sets the value stored in the node to "value". */
		void	setValue(valuetype value);

		/** Returns the key stored in the node. */
		keytype		getKey() const;

		/** Returns the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(keytype testkey) const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(
			dictionarynode<keytype,valuetype> *testnode) const;

		/** Prints a representation of the key and
		 *  value stored in the node. */
		void	print() const;

	#include <rudiments/private/dictionarynode.h>
};

/** The dictionary class allows you to store arbitrary numbers of key/value
 *  pairs.
 * 
 *  Each dictionary is composed of a set of dictionarynodes.  Each
 *  dictionarynode contains the key and value.
 *
 *  Internally, the dictionary class uses a linkedlist to store the values
 *  though this is potentially inefficient though, and may change in a future
 *  version. */
template <class keytype, class valuetype>
class dictionary : public dictionarycollection<keytype,valuetype> {
	public:
		/** Creates an empty instance of the dictionary class. */
		dictionary();

		/** Creates an empty instance of the dictionary class.
		 *  
		 *  If "trackinsertionorder" is true then the order of key
		 *  insertion is tracked and getKeys() and getList() below
		 *  return a list of key-value pairs in the order that they
		 *  were inserted.
		 *  
		 *  If "trackinsertionorder" is false then the order of key
		 *  insertion is not tracked and getList() and getList()
		 *  below return alist of key-value pairs in ascending order. */
		dictionary(bool trackinsertionorder);

		/** Deletes this instance of the dictionary class and all
		 *  of its dictionarynodes.  Note however, that neither the
		 *  key nor value stored in each dictionarynode are deleted
		 *  by this call. */
		virtual ~dictionary();

		/** Sets whether tracking of the order of key insertion is
		 *  enabled or not.
		 * 
		 *  If "trackinsertionorder" is true then the order of key
		 *  insertion is tracked and getKeys() and getList() below
		 *  return a list of key-value pairs in the order that they
		 *  were inserted.
		 *  
		 *  If "trackinsertionorder" is false then the order of key
		 *  insertion is not tracked and getList() and getList()
		 *  below return alist of key-value pairs in ascending order.
		 *
		 *  Defaults to "true".
		 *
		 *  Note, this can only be changed when there are no nodes
		 *  in the dictionary.  Eg. before the first call to setValue()
		 *  or after a call to clear().
		 *
		 *  Returns true if the call succeeded and false if it failed
		 *  (eg. because the dictionary contained nodes). */
		bool	setTrackInsertionOrder(bool trackinsertionorder);

		/** Returns "true" if insertion order tracking is enabled and
		 *  false otherwise. */
		bool	getTrackInsertionOrder();

		/** Sets the value associated with "key" to "value".
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValue(keytype key, valuetype value);

		/** Sets the value associated with each key in the "keys"
		 *  array to the corresponding value in the "values" array.
		 *  Assumes that keys is NULL terminated.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValues(keytype *keys, valuetype *values);

		/** Sets the value associated with each key in the "keys"
		 *  array to the corresponding value in the "values" array.
		 *  Assumes that keys is NULL terminated.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValues(keytype const *keys, valuetype const *values);

		/** Sets the value associated with each of "count" keys
		 *  in the "keys" array to the corresponding value in the
		 *  "values" array.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValues(keytype *keys, valuetype *values,
							uint64_t count);

		/** Sets the value associated with each of "count" keys
		 *  in the "keys" array to the corresponding value in the
		 *  "values" array.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValues(keytype const *keys, valuetype const *values,
								uint64_t count);

		/** Sets the value associated with each key in "dict" to the
		 *  corresponding value from "dict".
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValues(dictionary<keytype, valuetype> *dict);

		/** Sets "value" to the value associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getValue(keytype key, valuetype *value);

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		valuetype getValue(keytype key);

		/** Sets "k" to the key associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getKey(keytype key, keytype *k);

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		keytype getKey(keytype key);

		/** Returns a list of the keys in the dictionary. */
		linkedlist<keytype>	*getKeys();

		/** Returns the number of key/value pairs in the dictionary. */
		uint64_t	getLength();

		/** Removes the dictionarynode associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	remove(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the key and value stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDelete(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the key and value stored in the dictionarynode as well,
		 *  which are both presumed to be arrays.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDelete(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the key stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteKey(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the key stored in the dictionarynode as well, which is
		 *  presumed to be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteKey(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the value stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteValue(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the value stored in the dictionarynode as well, which is
		 *  presumed to be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteValue(keytype key);

		/** Removes the dictionarynode associated with "key", deleting
		 *  the key and value stored in the dictionarynode as well.
		 *  The value is presumed to be an array.*/
		bool	removeAndDeleteKeyAndArrayDeleteValue(keytype key);

		/** Deletes the dictionarynodes associated with "key",
		 *  deleting the key and value stored in the dictionarynode
		 *  as well.  The key is presumed to be an array.*/
		bool	removeAndArrayDeleteKeyAndDeleteValue(keytype key);

		/** Removes the specified dictionarynode.
		 *  Returns true on success or false if "node" was NULL. */
		bool	remove(dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the key and
		 *  value stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDelete(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the key and
		 *  value stored in the dictionarynode as well, which are both
		 *  presumed to be arrays.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDelete(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the key
		 *  stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteKey(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the key
		 *  stored in the dictionarynode as well, which is presumed to
		 *  be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteKey(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the value
		 *  stored in the dictionarynode as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteValue(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the value
		 *  stored in the dictionarynode as well, which is presumed to
		 *  be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node);

		/** Removes the specified dictionarynode, deleting the key and
		 *  value stored in the dictionarynode as well.
		 *  The value is presumed to be an array.*/
		bool	removeAndDeleteKeyAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node);

		/** Deletes the specified dictionarynode, deleting the key and
		 *  value stored in the dictionarynode as well.  The key is
		 *  presumed to be an array.*/
		bool	removeAndArrayDeleteKeyAndDeleteValue(
				dictionarynode<keytype,valuetype> *node);

		/** Deletes all dictionarynodes currently in the dictionary.
		 *  Note however, that the key and value stored in each
		 *  dictionarynode are not deleted by this call. */
		void	clear();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key and value stored in each dictionarynode
		 *  as well. */
		void	clearAndDelete();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key and value stored in each dictionarynode
		 *  as well, which are both presumed to be arrays. */
		void	clearAndArrayDelete();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key (but not the value) stored in each
		 *  dictionarynode as well. */
		void	clearAndDeleteKeys();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key (but not the value) stored in each
		 *  dictionarynode as well, which is presumed to be an array. */
		void	clearAndArrayDeleteKeys();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the value (but not the key) stored in each
		 *  dictionarynode as well. */
		void	clearAndDeleteValues();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the value (but not the key) stored in each
		 *  dictionarynode as well, which is presumed to be an array. */
		void	clearAndArrayDeleteValues();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key and value stored in each dictionarynode
		 *  as well.  Each value is presumed to be an array.*/
		void	clearAndDeleteKeysAndArrayDeleteValues();

		/** Deletes all dictionarynodes currently in the dictionary,
		 *  deleting the key and value stored in each dictionarynode
		 *  as well.  Each key is presumed to be an array.*/
		void	clearAndArrayDeleteKeysAndDeleteValues();

		/** Prints out a representation of the dictionary. */
		void	print();

	#include <rudiments/private/dictionary.h>
};


// ideally I'd use typdefs for these but older compilers can't handle them
#define namevaluepairsnode	dictionarynode<char *,char *>
#define namevaluepairs		dictionary<char *,char *>
#define constnamevaluepairsnode	dictionarynode<const char *,const char *>
#define constnamevaluepairs	dictionary<const char *,const char *>

#include <rudiments/private/dictionaryinlines.h>

#endif
