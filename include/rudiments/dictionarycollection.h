// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARYCOLLECTION_H
#define RUDIMENTS_DICTIONARYCOLLECTION_H

#include <rudiments/collection.h>
#include <rudiments/linkedlist.h>

/** The dictionarycollection class is the parent class for all rudiments
 *  dictionary collections. */
template<class keytype, class valuetype>
class dictionarycollection : public collection {
	public:
		/** Returns "dictionary". */
		virtual const char	*getType();

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
		virtual bool	getTrackInsertionOrder()=0;

		/** Sets the value associated with "key" to "value".
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		virtual	void	setValue(keytype key, valuetype value)=0;

		/** Sets the value associated with each key in the "keys"
		 *  array to the corresponding value in the "values" array.
		 *  Assumes that keys is NULL terminated.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		virtual	void	setValues(keytype *key, valuetype *value)=0;

		/** Sets the value associated with each key in the "keys"
		 *  array to the corresponding value in the "values" array.
		 *  Assumes that keys is NULL terminated.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		virtual	void	setValues(keytype const *key,
						valuetype const *value)=0;

		/** Sets the value associated with each of "count" keys
		 *  in the "keys" array to the corresponding value in the
		 *  "values" array.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		virtual	void	setValues(keytype *key,
						valuetype *value,
						uint64_t count)=0;

		/** Sets the value associated with each of "count" keys
		 *  in the "keys" array to the corresponding value in the
		 *  "values" array.
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		virtual	void	setValues(keytype const *key,
						valuetype const *value,
						uint64_t count)=0;

		/** Sets "value" to the value associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		virtual	bool	getValue(keytype key, valuetype *value)=0;

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		virtual	valuetype	getValue(keytype key)=0;

		/** Sets "k" to the key associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		virtual	bool	getKey(keytype key, keytype *k)=0;

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		virtual	keytype getKey(keytype key)=0;

		/** Returns a list of the keys in the dictionary. */
		virtual	linkedlist<keytype>	*getKeys()=0;

		/** Removes the key-value pair associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		virtual	bool		remove(keytype key)=0;

		/** Writes a representation of the dictionarycollection to
		 *  standard output. */
		virtual	ssize_t	write();

		/** Writes a representation of the dictionarycollection to
		 *  "out". */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the dictionarycollection to
		 *  standard output. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the dictionarycollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the dictionarycollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the dictionarycollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent);
};

#include <rudiments/private/dictionarycollectioninlines.h>

#endif
