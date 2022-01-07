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
		/** Sets the value stored in the node to "value". */
		virtual void	setValue(valuetype value)=0;

		/** Return the value stored in the node. */
		virtual valuetype	getValue() const=0;

		/** Returns the parent node in the tree or NULL
		 *  if this node is the top-most node in the tree. */
		virtual treenode<valuetype>	*getParent() const=0;

		/** Returns the left child of this node in the tree
		 *  or NULL if this node has no left child. */
		virtual treenode<valuetype>	*getLeftChild() const=0;

		/** Returns the right child of this node in the tree
		 *  or NULL if this node has no right child. */
		virtual treenode<valuetype>	*getRightChild() const=0;

		/** Returns the left height of this node in the tree. */
		virtual uint8_t	getLeftHeight() const=0;

		/** Returns the right height of this node in the tree. */
		virtual uint8_t	getRightHeight() const=0;

		/** Returns the previous node in the tree (in an in-order,
 		 *  depth-first traversal) or NULL if this node is the first
 		 *  node in the tree. */
		virtual treenode<valuetype>	*getPrevious() const=0;

		/** Returns the next node in the tree (in an in-order,
		 *  depth-first traversal) or NULL if this node is the last
		 *  node in the tree. */
		virtual treenode<valuetype>	*getNext() const=0;

		/** Prints the value stored in the node. */
		virtual void	print() const=0;

	#include <rudiments/private/treenode.h>
};

/** The treecollection class is the parent class for all rudiments tree
 *  collections. */
template <class valuetype>
class treecollection : public nodecollection<valuetype> {
	public:
		/** Creates an empty instance of the treecollection class. */
		treecollection() : nodecollection<valuetype>() {}

		/** Creates an instance of the treecollection class
		 *  that is a copy of "a". */
		treecollection(const nodecollection<valuetype> &a) :
					nodecollection<valuetype>(a) {}

		/** Returns the number of nodes in the tree. */
		virtual uint64_t	getLength() const=0;

		/** Returns the top-most node in the treecollection. */
		virtual treenode<valuetype>	*getTop() const=0;

		/** Returns the first node in the treecollection (in an
		 *  in-order, depth-first traversal). */
		virtual treenode<valuetype>	*getFirst() const=0;

		/** Returns the last node in the treecollection (in an in-order,
		 *  depth-first traversal). */
		virtual treenode<valuetype>	*getLast() const=0;

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the tree (in an in-order, depth-first
		 *  traversal).  "node" is presumed to be in the tree. */
		virtual treenode<valuetype>	*getPrevious(
					treenode<valuetype> *node) const=0;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the tree (in an in-order, depth-first
		 *  traversal). "node" is presumed to be in the tree. */
		virtual treenode<valuetype>	*getNext(
					treenode<valuetype> *node) const=0;

		/** Returns a pointer to the first treenode containing
		 *  "value" or NULL if "value" was not found. */
		virtual treenode<valuetype>	*find(valuetype value) const=0;

		/** Returns a pointer to the first treenode below
		 *  "startnode" containing "value" or NULL if "value" was not
		 *  found. */
		virtual treenode<valuetype>
			*find(treenode<valuetype> *startnode,
						valuetype value) const=0;

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
