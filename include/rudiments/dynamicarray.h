// Copyright (c) 1999-2018 David Muse
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
 * 	dynamicarray< int > d;
 * 	dynamicarray< myclass > d;
 * 	dynamicarray< myclass * > d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	dynamicarray< int[100] > d;
 * 	dynamicarray< myclass[100] > d;
 * 	dynamicarray< myclass *[100] > d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  dynamicarrays and/or staticarrays, like:
 *
 * 	dynamicarray< dynamicarray< int > > d;
 * 	dynamicarray< dynamicarray< myclass > > d;
 * 	dynamicarray< dynamicarray< myclass * > > d;
 * 	dynamicarray< staticarray< int > > d;
 * 	dynamicarray< staticarray< myclass > > d;
 * 	dynamicarray< staticarray< myclass * > > d;
 */
template < class valuetype >
class dynamicarray : public arraycollection {
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
		dynamicarray(const dynamicarray<valuetype> &v);

		/** Makes this instance of the dynamicarray class
		 *  identical to "v". */
		dynamicarray<valuetype>	&operator=(
					const dynamicarray<valuetype> &v);

		/** Deletes this instance of the dynamicarray class and all of
		 *  its values. */
		~dynamicarray();

		/** Provides access to the "index"th element of the
		 *  dynamicarray. */
		valuetype	&operator[](uint64_t index);

		/*  Returns the length of the initial extent. */
		uint64_t	getInitialLength() const;

		/*  Returns the length of each incremental extent. */
		uint64_t	getIncrementLength() const;

		/** Returns the number of elements in the array. */
		uint64_t	getLength() const;

		/** Clears the array, deleting all of its values. */
		void	clear();

		/** Clears the array, deleting all of its values and resetting
		 *  the lengths of the initial and incremental extents. */
		void	clear(uint64_t initiallength, uint64_t incrementlength);

	#include <rudiments/private/dynamicarray.h>
};

#include <rudiments/private/dynamicarrayinlines.h>

#endif
