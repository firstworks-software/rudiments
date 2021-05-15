// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTCOLLECTION_H
#define RUDIMENTS_LISTCOLLECTION_H

#include <rudiments/nodecollection.h>

/** The listnode class is the parent class for the node contained by all
 *  rudiments listnode-based collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC listnode : public nodecollectionnode<valuetype> {
	public:

		/** Creates an instance of the listnode class. */
		listnode() : nodecollectionnode<valuetype>() {};

		/** Creates an instance of the listnode class. */
		listnode(const collection &c) :
				nodecollectionnode<valuetype>(c) {};

		/** Deletes this instance of the listnode class. */
		virtual	~listnode() {};

		/** Set the value stored in the node to "value". */
		virtual	void		setValue(valuetype value)=0;

		/** Return the value stored in the node. */
		virtual	valuetype	getValue() const=0;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than "value". */
		virtual	int32_t	compare(valuetype value) const=0;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than the value stored in
		 *  "peer". */
		virtual	int32_t	compare(listnode<valuetype> *peer) const=0;

		/** Returns the previous node in the linkedlist or NULL
		 *  if this node is the first node in the list. */
		virtual	listnode<valuetype>	*getPrevious()=0;

		/** Returns the next node in the linkedlist or NULL
		 * if this node is the last node in the list. */
		virtual	listnode<valuetype>	*getNext()=0;

		virtual	void	setNext(listnode<valuetype> *next)=0;
		virtual	void	setPrevious(listnode<valuetype> *previous)=0;
};

/** The listcollection class is the parent class for all rudiments list
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC listcollection : public nodecollection {
	public:

		/** Creates an instance of the listcollection class. */
		listcollection() : nodecollection() {};

		/** Creates an instance of the listcollection class. */
		listcollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~listcollection() {};

		/** Returns the number of nodes in the linkedlist. */
		virtual	uint64_t	getLength() const=0;

		/** Returns the first node in the linkedlist. */
		virtual	listnode<valuetype>	*getFirst()=0;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		virtual	listnode<valuetype>	*getNext(
						listnode<valuetype> *node)=0;

		/** Returns a pointer to the first linkedlistnode
		 *  containing "value" or NULL if "value" was not found. */
		virtual	listnode<valuetype>	*find(valuetype value)=0;

		/** Returns a pointer to the first linkedlistnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		virtual	listnode<valuetype>	*find(
						listnode<valuetype> *startnode,
						valuetype value)=0;

		/** Deletes all linkedlistnodes currently in the linkedlist.
		 *  Note however, that the value stored in each linkedlistnode
		 *  is not deleted by this call. */
		virtual	void	clear()=0;

		/** Prints out a representation of the linkedlist. */
		virtual	void	print() const=0;

		/** Prints out a representation of the first "count"
		 *  nodes of the linkedlist. */
		virtual	void	print(uint64_t count) const=0;
};

#endif
