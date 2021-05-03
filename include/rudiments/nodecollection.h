// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NODECOLLECTION_H
#define RUDIMENTS_NODECOLLECTION_H

#include <rudiments/collection.h>

/** The node class is the parent class for the node contained by all rudiments
 *  node-based collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC nodecollectionnode : public object {
	public:

		/** Creates an instance of the node class. */
		nodecollectionnode() : object() {};

		/** Creates an instance of the node class. */
		nodecollectionnode(const collection &c) : object(c) {};

		/** Deletes this instance of the node class. */
		virtual	~nodecollectionnode() {};

		virtual	void		setValue(valuetype value)=0;
		virtual	valuetype	getValue() const=0;
		virtual	int32_t	compare(valuetype value) const=0;
		virtual	void	print() const=0;
};

/** The collection class is the parent class for all rudiments node-based
 *  collections. */
class RUDIMENTS_DLLSPEC nodecollection : public collection {
	public:

		/** Creates an instance of the nodecollection class. */
		nodecollection() : collection() {};

		/** Creates an instance of the nodecollection class. */
		nodecollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the nodecollection class. */
		virtual	~nodecollection() {};
};

#endif
