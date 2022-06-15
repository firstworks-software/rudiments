// Copyright (c) 1999-2018 David Muse
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
		 *  that is a copy of "a". */
		scalar(const scalar<valuetype> &a);

		/** Creates an instance of the scalar class
		 *  that is a copy of "a". */
		scalar(const scalarcollection<valuetype> &a);

		/** Makes this instance of the scalar class
		 *  identical to "a". */
		scalar<valuetype>	&operator=(
					const scalar<valuetype> &a);

		/** Makes this instance of the scalar class
		 *  identical to "a". */
		scalar<valuetype>	&operator=(
					const scalarcollection<valuetype> &a);

		/** Deletes this instance of the scalar class. */
		~scalar();

		/** Sets the value stored in this instance to "v". */
		void		setValue(valuetype v);

		/** Returns the value stored in this intance. Returns NULL or
		 *  0 if no value has been stored. */
		valuetype	getValue() const;

		/** Removes the value currently stored in this instance, such
		 *  that getValue() will return NULL or 0.
		 *
		 *  The value is only deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		void	clear();

	#include <rudiments/private/scalar.h>
};

#include <rudiments/private/scalarinlines.h>

#endif
