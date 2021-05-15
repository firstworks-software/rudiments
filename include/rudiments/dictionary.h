// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/dictionaryincludes.h>

/** The dictionary class allows you to store arbitrary numbers of key-value
 *  pairs. */
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

		/** Deletes this instance of the dictionary class.  Note
		 *  however, that neither the keys nor values stored in the
		 *  dictionary are deleted by this call. */
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
		 *  Note, this can only be changed when there are no key-value
		 *  pairs in the dictionary.  Eg. before the first call to
		 *  setValue() or after a call to clear().
		 *
		 *  Returns true if the call succeeded and false if it failed
		 *  (eg. because the dictionary contained key-value pairs). */
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

		/** Returns the number of key-value pairs in the dictionary. */
		uint64_t	getLength();

		/** Removes the key-value pair associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	remove(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the key and value stored in the key-value pair as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDelete(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the key and value stored in the key-value pair as well,
		 *  which are both presumed to be arrays.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDelete(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the key stored in the key-value pair as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteKey(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the key stored in the key-value pair as well, which is
		 *  presumed to be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteKey(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the value stored in the key-value pair as well.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndDeleteValue(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the value stored in the key-value pair as well, which is
		 *  presumed to be an array.
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeAndArrayDeleteValue(keytype key);

		/** Removes the key-value pair associated with "key", deleting
		 *  the key and value stored in the key-value pair as well.
		 *  The value is presumed to be an array.*/
		bool	removeAndDeleteKeyAndArrayDeleteValue(keytype key);

		/** Deletes the key-value pairs associated with "key",
		 *  deleting the key and value stored in the key-value pair
		 *  as well.  The key is presumed to be an array.*/
		bool	removeAndArrayDeleteKeyAndDeleteValue(keytype key);

		/** Deletes all key-value pairs currently in the dictionary.
		 *  Note however, that the key and value stored in each
		 *  key-value pair are not deleted by this call. */
		void	clear();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key and value stored in each key-value pair
		 *  as well. */
		void	clearAndDelete();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key and value stored in each key-value pair
		 *  as well, which are both presumed to be arrays. */
		void	clearAndArrayDelete();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key (but not the value) stored in each
		 *  key-value pair as well. */
		void	clearAndDeleteKeys();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key (but not the value) stored in each
		 *  key-value pair as well, which is presumed to be an array. */
		void	clearAndArrayDeleteKeys();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the value (but not the key) stored in each
		 *  key-value pair as well. */
		void	clearAndDeleteValues();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the value (but not the key) stored in each
		 *  key-value pair as well, which is presumed to be an array. */
		void	clearAndArrayDeleteValues();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key and value stored in each key-value pair
		 *  as well.  Each value is presumed to be an array.*/
		void	clearAndDeleteKeysAndArrayDeleteValues();

		/** Deletes all key-value pairs currently in the dictionary,
		 *  deleting the key and value stored in each key-value pair
		 *  as well.  Each key is presumed to be an array.*/
		void	clearAndArrayDeleteKeysAndDeleteValues();

		/** Prints out a representation of the dictionary. */
		void	print();

	#include <rudiments/private/dictionary.h>
};

#include <rudiments/private/dictionaryinlines.h>

#endif
