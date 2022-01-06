// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NODECOLLECTION_H
#define RUDIMENTS_NODECOLLECTION_H

#include <rudiments/collection.h>

/** The node class is the parent class for the node contained by all rudiments
 *  node-based collections. */
template <class valuetype>
class nodecollectionnode : virtual public object {
	public:

		/** Deletes this instance of the node class. */
		virtual	~nodecollectionnode() {};

		/** Set the value stored in the node to "value". */
		virtual	void		setValue(valuetype value)=0;

		/** Return the value stored in the node. */
		virtual	valuetype	getValue() const=0;

		/** Prints out a representation of the node. */
		virtual	void	print() const=0;
};

/** The collection class is the parent class for all rudiments node-based
 *  collections. */
class nodecollection : public collection {
	public:

		/** Deletes this instance of the nodecollection class. */
		virtual	~nodecollection() {};
};

#endif
