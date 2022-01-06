// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TREECOLLECTION_H
#define RUDIMENTS_TREECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The treenode class is the parent class for the node contained by all
 *  rudiments treenode-based collections. */
template <class valuetype>
class treenode : public nodecollectionnode<valuetype> {
	public:
#if 0
		/** Creates an instance of the treenode class that
		 *  stores value "value". */
		treenode() : nodecollectionnode<valuetype>() {};
#endif

		/** Deletes this instance of the treenode class.
		 *  Note however, that the value stored in the treenode
		 *  is not deleted by this call. */
		virtual ~treenode() {};

		/** Sets the value stored in the node to "value". */
		virtual void	setValue(valuetype value)=0;

		/** Return the value stored in the node. */
		virtual valuetype	getValue() const=0;

		/** Returns the parent node in the tree or NULL
		 *  if this node is the top-most node in the tree. */
		virtual treenode<valuetype>	*getParent()=0;

		/** Returns the left child of this node in the tree
		 *  or NULL if this node has no left child. */
		virtual treenode<valuetype>	*getLeftChild()=0;

		/** Returns the right child of this node in the tree
		 *  or NULL if this node has no right child. */
		virtual treenode<valuetype>	*getRightChild()=0;

		/** Returns the left height of this node in the tree. */
		virtual uint8_t	getLeftHeight()=0;

		/** Returns the right height of this node in the tree. */
		virtual uint8_t	getRightHeight()=0;

		/** Returns the previous node in the tree (in an in-order,
 		 *  depth-first traversal) or NULL if this node is the first
 		 *  node in the tree. */
		virtual treenode<valuetype>	*getPrevious()=0;

		/** Returns the next node in the tree (in an in-order,
		 *  depth-first traversal) or NULL if this node is the last
		 *  node in the tree. */
		virtual treenode<valuetype>	*getNext()=0;

		/** Prints the value stored in the node. */
		virtual void	print() const=0;

	#include <rudiments/private/treenode.h>
};

/** The treecollection class is the parent class for all rudiments tree
 *  collections. */
template <class valuetype>
class treecollection : public nodecollection {
	public:

#if 0
		/** Creates an instance of the treecollection class. */
		treecollection() : nodecollection() {};

		/** Creates an instance of the treecollection class. */
		treecollection(const collection &c) : nodecollection(c) {};
#endif

		/** Deletes this instance of the treecollection class. */
		virtual	~treecollection() {};

		/** Returns the number of nodes in the tree. */
		virtual uint64_t	getLength() const=0;

		/** Returns the top-most node in the treecollection. */
		virtual treenode<valuetype>	*getTop()=0;

		/** Returns the first node in the treecollection (in an
		 *  in-order, depth-first traversal). */
		virtual treenode<valuetype>	*getFirst()=0;

		/** Returns the last node in the treecollection (in an in-order,
		 *  depth-first traversal). */
		virtual treenode<valuetype>	*getLast()=0;

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the tree (in an in-order, depth-first
		 *  traversal).  "node" is presumed to be in the tree. */
		virtual treenode<valuetype>	*getPrevious(
					treenode<valuetype> *node)=0;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the tree (in an in-order, depth-first
		 *  traversal). "node" is presumed to be in the tree. */
		virtual treenode<valuetype>	*getNext(
					treenode<valuetype> *node)=0;

		/** Returns a pointer to the first treenode containing
		 *  "value" or NULL if "value" was not found. */
		virtual treenode<valuetype>	*find(valuetype value)=0;

		/** Returns a pointer to the first treenode below
		 *  "startnode" containing "value" or NULL if "value" was not
		 *  found. */
		virtual treenode<valuetype>
			*find(treenode<valuetype> *startnode,
							valuetype value)=0;

		/** Deletes all treenodes currently in the
		 *  treecollection.
		 *
		 *  The value stored in each treenode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		virtual void	clear()=0;

		/** Prints out an xml-style representation of the
		 *  treecollection. */
		virtual void	print() const=0;
};

#endif
