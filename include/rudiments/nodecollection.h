// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NODECOLLECTION_H
#define RUDIMENTS_NODECOLLECTION_H

#include <rudiments/collection.h>

/** The node class is the parent class for the node contained by all rudiments
 *  node-based collections. */
template <class valuetype>
class nodecollectionnode {
	public:
		/** Deletes this instance of the nodecollectionnode class. */
		virtual	~nodecollectionnode() {};

		/** Set the value stored in the node to "value". */
		virtual	void		setValue(valuetype value)=0;

		/** Returns the value stored in the node. */
		virtual	valuetype	getValue()=0;

		/** Returns a reference to the value stored in the node. */
		virtual	valuetype	&getReference()=0;

		/** Returns the next node in the nodecollection or NULL
		 * if this node is the last node in the collection. */
		virtual	nodecollectionnode<valuetype>	*getNext()=0;
};

/** The collection class is the parent class for all rudiments node-based
 *  collections. */
template <class valuetype>
class nodecollection : public collection {
	public:
		/** Returns the first node in the nodecollection. */
		virtual nodecollectionnode<valuetype>	*getFirst()=0;
};

#endif
