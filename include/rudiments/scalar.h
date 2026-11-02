// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SCALAR_H
#define RUDIMENTS_SCALAR_H

#include <rudiments/private/scalarincludes.h>

/** The scalar class allows you to store a single value. */
template <class valuetype>
class scalar : public scalarcollection<valuetype> {
	public:

		/** Creates an instance of the scalar class. */
		scalar();

		/** Creates an instance of the scalar class
		 *  that stores "v".
		 *
		 *  The value is not copied, and is not deleted when this
		 *  instance is deleted. */
		scalar(valuetype v);

		/** Creates an instance of the scalar class
		 *  that stores "v".
		 *
		 *  The value is not copied.  If "managevalues" or
		 *  "managearrayvalues" is true, then this instance takes
		 *  ownership of the value, and deletes it when the value is
		 *  cleared or this instance is deleted.
		 *
		 *  If both are true, then "managearrayvalues" overrides
		 *  "managevalues".
		 *
		 *  A managed value must be allocated to match the flag: with
		 *  new for "managevalues" and with new[] for
		 *  "managearrayvalues".  A char * or const char * value is
		 *  only deleted if "managearrayvalues" is true. */
		scalar(valuetype v, bool managevalues, bool managearrayvalues);

		/** Creates an instance of the scalar class
		 *  that is a copy of "a".
		 *
		 *  The copy takes the manage flags from "a".  If no manage
		 *  flag is set on "a", then the copy gets the same pointer as
		 *  "a", and it is valid only while the data that it points to
		 *  exists.  See the collection class description.
		 *
		 *  If "a" manages array values, a char, wchar_t, or byte_t
		 *  array is duplicated, up to the first zero byte for a
		 *  byte_t array.  An array of any other type can't be
		 *  duplicated without its length, so the copy gets NULL. */
		scalar(scalar<valuetype> &a);

		/** Creates an instance of the scalar class
		 *  that is a copy of "a".
		 *
		 *  The copy takes the manage flags from "a".  If no manage
		 *  flag is set on "a", then the copy gets the same pointer as
		 *  "a", and it is valid only while the data that it points to
		 *  exists.  See the collection class description.
		 *
		 *  If "a" manages array values, a char, wchar_t, or byte_t
		 *  array is duplicated, up to the first zero byte for a
		 *  byte_t array.  An array of any other type can't be
		 *  duplicated without its length, so the copy gets NULL. */
		scalar(scalarcollection<valuetype> &a);

		/** Makes this instance of the scalar class
		 *  identical to "a".
		 *
		 *  This instance takes the manage flags from "a", and flags
		 *  that were set on this instance have no effect on the copy.
		 *  If no manage flag is set on "a", then this instance gets
		 *  the same pointer as "a", and it is valid only while the
		 *  data that it points to exists.  See the collection class
		 *  description.
		 *
		 *  If "a" manages array values, a char, wchar_t, or byte_t
		 *  array is duplicated, up to the first zero byte for a
		 *  byte_t array.  An array of any other type can't be
		 *  duplicated without its length, so the copy gets NULL. */
		scalar<valuetype>	&operator=(
					scalar<valuetype> &a);

		/** Makes this instance of the scalar class
		 *  identical to "a".
		 *
		 *  This instance takes the manage flags from "a", and flags
		 *  that were set on this instance have no effect on the copy.
		 *  If no manage flag is set on "a", then this instance gets
		 *  the same pointer as "a", and it is valid only while the
		 *  data that it points to exists.  See the collection class
		 *  description.
		 *
		 *  If "a" manages array values, a char, wchar_t, or byte_t
		 *  array is duplicated, up to the first zero byte for a
		 *  byte_t array.  An array of any other type can't be
		 *  duplicated without its length, so the copy gets NULL. */
		scalar<valuetype>	&operator=(
					scalarcollection<valuetype> &a);

		/** Deletes this instance of the scalar class. */
		~scalar();

		/** Sets the value stored in this instance to "v". */
		void		setValue(valuetype v);

		/** Returns the value stored in this instance. Returns NULL or
		 *  0 if no value has been stored. */
		valuetype	getValue();

		/** Returns a reference to the value stored in this instance. */
		valuetype	&getReference();

		/** Removes the value currently stored in this instance, such
		 *  that getValue() will return NULL or 0.
		 *
		 *  The value is only deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 *
		 *  Always returns true. */
		bool	clear();

	#include <rudiments/private/scalar.h>
};

#include <rudiments/private/scalarinlines.h>

#endif
