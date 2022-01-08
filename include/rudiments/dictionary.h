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

		/** Creates an instance of the dictionary class
		 *  that is a copy of "a". */
		dictionary(const dictionary<keytype,valuetype> &a);

		/** Creates an instance of the dictionary class
		 *  that is a copy of "a". */
		dictionary(const dictionarycollection<keytype,valuetype> &a);

		/** Makes this instance of the dictionary class
		 *  identical to "a". */
		dictionary<keytype,valuetype>	&operator=(
					const dictionary<keytype,valuetype> &a);

		/** Makes this instance of the dictionary class
		 *  identical to "a". */
		dictionary<keytype,valuetype>	&operator=(
			const dictionarycollection<keytype,valuetype> &a);

		/** Deletes this instance of the dictionary class.
		 *
		 *  The key stored in each pair is only deleted if
		 *  setManageKeys(true) or setManageArrayKeys(true) has
		 *  been called.
		 *
		 *  The value stored in each pair is only deleted if
		 *  setManageValues(true) or setManageArrayValues(true) has
		 *  been called. */
		~dictionary();

		/** Sets whether tracking of the order of key insertion is
		 *  enabled or not.
		 * 
		 *  If "trackinsertionorder" is true then the order of key
		 *  insertion is tracked and getKeys() returns a list of
		 *  keys in the order that they were inserted.
		 *  
		 *  If "trackinsertionorder" is false then the order of key
		 *  insertion is not tracked and getKeys() returns a list of
		 *  keys in ascending order.
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
		 *  false otherwise.
		 * 
		 *  If getTrackInsertionOrder() is true then the order of key
		 *  insertion is tracked and getKeys() returns a list of
		 *  keys in the order that they were inserted.
		 *  
		 *  If getTrackInsertionOrder() is false then the order of key
		 *  insertion is not tracked and getKeys() returns a list of
		 *  keys in ascending order. */
		bool	getTrackInsertionOrder() const;

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
		void	setValues(dictionary<keytype,valuetype> *dict);

		/** Sets "value" to the value associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getValue(keytype key, valuetype *value) const;

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		valuetype getValue(keytype key) const;

		/** Sets "k" to the key associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getKey(keytype key, keytype *k) const;

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		keytype getKey(keytype key) const;

		/** Returns a list of the keys in the dictionary. */
		linkedlist<keytype>	*getKeys() const;

		/** Returns the number of key-value pairs in the dictionary. */
		uint64_t	getLength() const;

		/** Removes the key-value pair associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found.
		 *
		 *  The key stored in the pair is only deleted if
		 *  setManageKeys(true) or setManageArrayKeys(true) has
		 *  been called.
		 *
		 *  The value stored in the pair is only deleted if
		 *  setManageValues(true) or setManageArrayValues(true) has
		 *  been called. */
		bool	remove(keytype key);

		/** Deletes all key-value pairs currently in the dictionary.
		 *
		 *  The key stored in each pair is only deleted if
		 *  setManageValues(true) or setManageArrayValues(true) has
		 *  been called.
		 *
		 *  The key stored in each pair is only deleted if
		 *  setManageKeys(true) or setManageArrayKeys(true) has
		 *  been called.
		 *
		 *  The value stored in each pair is only deleted if
		 *  setManageValues(true) or setManageArrayValues(true) has
		 *  been called. */
		void	clear();

		/** Prints out a representation of the dictionary. */
		void	print() const;

	#include <rudiments/private/dictionary.h>
};

#include <rudiments/private/dictionaryinlines.h>

#endif
