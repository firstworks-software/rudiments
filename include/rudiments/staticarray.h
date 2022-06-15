// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STATICARRAY_H
#define RUDIMENTS_STATICARRAY_H

#include <rudiments/arraycollection.h>

/** The staticarray class allows you to store a pre-defined number of values
 *  and access them with array-like syntax.
 *
 *  "valuetype" can be any scalar type, including a pointer, but cannot be
 *  an array.
 *
 *  Ie. these are legal:
 * 	staticarray<int,10> d;
 * 	staticarray<myclass,10> d;
 * 	staticarray<myclass *,10> d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	staticarray<int[100],10> d;
 * 	staticarray<myclass[100],10> d;
 * 	staticarray<myclass *[100],10> d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  staticarrays or dynamicarrays, like:
 *
 * 	staticarray<staticarray<in >,10> d;
 * 	staticarray<staticarray<myclass>,10> d;
 * 	staticarray<staticarray<myclass *>,10> d;
 * 	staticarray<dynamicarray<int>,10> d;
 * 	staticarray<dynamicarray<myclass>,10> d;
 * 	staticarray<dynamicarray<myclass *>,10> d;
 *
 *
 *  NOTE: If "valuetype" is a pointer and either of
 *  collection::setManageValues(true) or collection::setManageArrayValue(true)
 *  are called, then all elements of the staticarray must be set to something.
 *
 *  For example, if the staticarray has length 10 then elements 0-9 must be set
 *  to some value.  They may be set to 0 or NULL, but they must be set.
 *
 *  Otherwise attempts to delete the uninitialized elements during calls to
 *  clear() or the destructor will likely cause the program to crash.
 */
template <class valuetype, uint64_t length>
class staticarray : public arraycollection<valuetype> {
	public:
		/** Creates an empty instance of the staticarray class with
		 *  "length" elements.  "length" is given in the template
		 *  definition. */
		staticarray();

		/** Creates an instance of the staticarray class
		 *  that is a copy of "v". */
		staticarray(const staticarray<valuetype,length> &v);

		/** Creates an instance of the staticarray class
		 *  that is a copy of "v". */
		staticarray(const arraycollection<valuetype> &v);

		/** Makes this instance of the staticarray class
		 *  identical to "v". */
		staticarray<valuetype,length>	&operator=(
				const staticarray<valuetype,length> &v);

		/** Makes this instance of the staticarray class
		 *  identical to "v". */
		staticarray<valuetype,length>	&operator=(
				const arraycollection<valuetype> &v);

		/** Deletes this instance of the staticarray class. */
		~staticarray();

		/** Provides read/write access to the "index"th element of the
		 *  staticarray. */
		valuetype	&operator[](uint64_t index);

		/** Provides read-only access to the "index"th element of the
		 *  staticarray. */
		valuetype	operator[](uint64_t index) const;

		/** Returns the number of elements in the array. */
		uint64_t	getLength() const;

		/** Clears the array. */
		void	clear();

		/** Writes a representation of the staticarray to "out". */
		bool	write(output *out) const;

	#include <rudiments/private/staticarray.h>
};

#include <rudiments/private/staticarrayinlines.h>

#endif
