// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICARRAY_H
#define RUDIMENTS_DYNAMICARRAY_H

#include <rudiments/private/dynamicarrayincludes.h>

/** The dynamicarray class allows you to store an arbitrary number of values
 *  and access them with array-like syntax.
 *
 *  Internally, the class stores these values in a series of extents.  The
 *  size of the initial and incremental extents are defined in the constructor.
 *
 *  "valuetype" can be any scalar type, including a pointer, but cannot be
 *  an array.
 *
 *  Ie. these are legal:
 * 	dynamicarray<int> d;
 * 	dynamicarray<myclass> d;
 * 	dynamicarray<myclass *> d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	dynamicarray<int[100]> d;
 * 	dynamicarray<myclass[100]> d;
 * 	dynamicarray<myclass *[100]> d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  dynamicarrays and/or staticarrays, like:
 *
 * 	dynamicarray<dynamicarray<int> > d;
 * 	dynamicarray<dynamicarray<myclass> > d;
 * 	dynamicarray<dynamicarray<myclass *> > d;
 * 	dynamicarray<staticarray<int> > d;
 * 	dynamicarray<staticarray<myclass> > d;
 * 	dynamicarray<staticarray<myclass *> > d;
 *
 *
 *  NOTE: If "valuetype" is a pointer and either of
 *  collection::setManageValues(true) or collection::setManageArrayValue(true)
 *  are called, then all elements of the dynamicarray prior to the highest set
 *  element must also be set to something.
 *
 *  For example, if element 9 is set to some value then elements 0-8 must also
 *  be set to some value.  They may be set to 0 or NULL, but they must be set.
 *
 *  Otherwise attempts to delete the uninitialized elements during calls to
 *  clear() or the destructor will likely cause the program to crash.
 */
template <class valuetype>
class dynamicarray : public arraycollection<valuetype> {
	public:
		/** Creates an empty instance of the dynamicarray class. */
		dynamicarray();

		/** Creates an empty instance of the dynamicarray class.
		 *  The initial extent will be created with "initiallength"
		 *  members and each successive extent will be created with
		 *  "incrementlength" members. */
		dynamicarray(uint64_t initiallength, uint64_t incrementlength);

		/** Creates an instance of the dynamicarray class
		 *  that is a copy of "v". */
		dynamicarray(dynamicarray<valuetype> &v);

		/** Creates an instance of the dynamicarray class
		 *  that is a copy of "v". */
		dynamicarray(arraycollection<valuetype> &v);

		/** Makes this instance of the dynamicarray class
		 *  identical to "v". */
		dynamicarray<valuetype>	&operator=(
					dynamicarray<valuetype> &v);

		/** Makes this instance of the dynamicarray class
		 *  identical to "v". */
		dynamicarray<valuetype>	&operator=(
					arraycollection<valuetype> &v);

		/** Deletes this instance of the dynamicarray class and all of
		 *  its values. */
		~dynamicarray();

		/** Sets all bytes of the first "count" elements of the
		 *  dynamicarray to 0. */
		void	zero(uint64_t count);

		/** Sets all bytes of "count" elements of the dynamicarray,
		 *  starting at "start", to 0. */
		void	zero(uint64_t start, uint64_t count);

		/** Sets all bytes of the first "count" elements of the
 		 *  dynamicarray to "value". */
		void	set(byte_t value, uint64_t count);

		/** Sets all bytes of "count" elements of the dynamicarray,
		 *  starting at "start", to "value". */
		void	set(byte_t value, uint64_t start, uint64_t count);

		/** Provides read/write access to the "index"th element of the
		 *  dynamicarray. */
		valuetype	&operator[](uint64_t index);

		/*  Returns the number of elements in the initial extent. */
		uint64_t	getInitialCount();

		/*  Returns the number of elements in each incremental
		 *  extent. */
		uint64_t	getIncrementCount();

		/** Returns the number of elements in the array. */
		uint64_t	getCount();

		/** Clears the array.
		 *
		 *  Always returns true. */
		bool	clear();

		/** Clears the array and resets the lengths of the initial and
 		 *  incremental extents. */
		bool	clear(uint64_t initiallength, uint64_t incrementlength);

	#include <rudiments/private/dynamicarray.h>
};

#include <rudiments/private/dynamicarrayinlines.h>

#endif
