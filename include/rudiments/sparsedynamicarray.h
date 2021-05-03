// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SPARSEDYNAMICARRAY_H
#define RUDIMENTS_SPARSEDYNAMICARRAY_H

#include <rudiments/private/dynamicarrayincludes.h>

/** The sparsedynamicarray class allows you to store an arbitrary number of
 *  values and access them with array-like syntax.
 *
 *  Internally, the class stores these values such that memory is conserved
 *  when the array is sparsely populated.
 *
 *  "valuetype" can be any scalar type, including a pointer, but cannot be
 *  an array.
 *
 *  Ie. these are legal:
 * 	sparsedynamicarray<int> d;
 * 	sparsedynamicarray<myclass> d;
 * 	sparsedynamicarray<myclass *> d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	sparsedynamicarray<int[100]> d;
 * 	sparsedynamicarray<myclass[100]> d;
 * 	sparsedynamicarray<myclass *[100]> d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  sparsedynamicarrays and/or staticarrays, like:
 *
 * 	sparsedynamicarray<sparsedynamicarray<int> > d;
 * 	sparsedynamicarray<sparsedynamicarray<myclass> > d;
 * 	sparsedynamicarray<sparsedynamicarray<myclass *> > d;
 * 	sparsedynamicarray<staticarray<int> > d;
 * 	sparsedynamicarray<staticarray<myclass> > d;
 * 	sparsedynamicarray<staticarray<myclass *> > d;
 */
template <class valuetype>
class sparsedynamicarray : public arraycollection<valuetype> {
	public:
		/** Creates an empty instance of the sparsedynamicarray class. */
		sparsedynamicarray();

		/** Creates an instance of the sparsedynamicarray class
		 *  that is a copy of "v". */
		//sparsedynamicarray(const sparsedynamicarray<valuetype> &v);

		/** Makes this instance of the sparsedynamicarray class
		 *  identical to "v". */
		//sparsedynamicarray<valuetype>	&operator=(
				//const sparsedynamicarray<valuetype> &v);

		/** Deletes this instance of the dynamicarray class and all of
		 *  its values. */
		~dynamicarray();

		/** Provides access to the "index"th element of the
		 *  dynamicarray. */
		valuetype	&operator[](uint64_t index);

		/** Returns the number of elements in the array. */
		uint64_t	getLength() const;

		/** Clears the array, deleting all of its values. */
		void	clear();

	#include <rudiments/private/sparsedynamicarray.h>
};

#include <rudiments/private/sparsedynamicarrayinlines.h>

#endif
