// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TREECOLLECTION_H
#define RUDIMENTS_TREECOLLECTION_H

#include <rudiments/nodecollection.h>

// FIXME: add a treenode class and use it here instead of nodecollectionnode,
// and in avltree too, instead of avltreenode

/** The treecollection class is the parent class for all rudiments tree
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC treecollection : public nodecollection {
	public:

		/** Creates an instance of the treecollection class. */
		treecollection() : nodecollection() {};

		/** Creates an instance of the treecollection class. */
		treecollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the treecollection class. */
		virtual	~treecollection() {};

		/** Returns the number of nodes in the tree. */
		virtual uint64_t	getLength() const=0;

		/** Returns the top-most node in the treecollection. */
		virtual nodecollectionnode<valuetype>	*getTop()=0;

		/** Returns the first node in the treecollection (in an
		 *  in-order, depth-first traversal). */
		virtual nodecollectionnode<valuetype>	*getFirst()=0;

		/** Returns the last node in the treecollection (in an in-order,
		 *  depth-first traversal). */
		virtual nodecollectionnode<valuetype>	*getLast()=0;

#if 0
		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the tree (in an in-order, depth-first
		 *  traversal).  "node" is presumed to be in the tree. */
		virtual nodecollectionnode<valuetype>	*getPrevious(
					nodecollectionnode<valuetype> *node)=0;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the tree (in an in-order, depth-first
		 *  traversal). "node" is presumed to be in the tree. */
		virtual nodecollectionnode<valuetype>	*getNext(
					nodecollectionnode<valuetype> *node)=0;
#endif

		/** Returns a pointer to the first nodecollectionnode containing
		 *  "value" or NULL if "value" was not found. */
		virtual nodecollectionnode<valuetype>	*find(
							valuetype value)=0;

#if 0
		/** Returns a pointer to the first nodecollectionnode below
		 *  "startnode" containing "value" or NULL if "value" was not
		 *  found. */
		virtual nodecollectionnode<valuetype>
			*find(nodecollectionnode<valuetype> *startnode,
							valuetype value)=0;
#endif

		/** Deletes all nodecollectionnodes currently in the
		 *  treecollection.
		 *
		 *  The value stored in each nodecollectionnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		virtual void	clear()=0;

		/** Prints out an xml-style representation of the
		 *  treecollection. */
		virtual void	print() const=0;
};

#endif
