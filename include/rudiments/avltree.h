// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_AVLTREE_H
#define RUDIMENTS_AVLTREE_H

#include <rudiments/private/avltreeincludes.h>

/** The avltreenode class stores the values that compose a avltree. */
template <class valuetype>
class avltreenode : public treenode<valuetype> {
	public:
		/** Creates an instance of the avltreenode class that
		 *  stores value "value". */
		avltreenode(valuetype value);

		/** Deletes this instance of the avltreenode class.
		 *  Note however, that the value stored in the avltreenode
		 *  is not deleted by this call. */
		~avltreenode();

		/** Sets the value stored in the node to "value". */
		void	setValue(valuetype value);

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than "value". */
		virtual int32_t	compare(valuetype value) const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than the value stored in
		 *  "peer". */
		int32_t	compare(treenode<valuetype> *peer) const;

		/** Returns the parent node in the avltree or NULL
		 *  if this node is the top-most node in the tree. */
		treenode<valuetype>	*getParent();

		/** Returns the left child of this node in the avltree
		 *  or NULL if this node has no left child. */
		treenode<valuetype>	*getLeftChild();

		/** Returns the right child of this node in the avltree
		 *  or NULL if this node has no right child. */
		treenode<valuetype>	*getRightChild();

		/** Returns the left height of this node in the avltree. */
		uint8_t	getLeftHeight();

		/** Returns the right height of this node in the avltree. */
		uint8_t	getRightHeight();

		/** Returns the previous node in the avltree (in an in-order,
 		 *  depth-first traversal) or NULL if this node is the first
 		 *  node in the tree. */
		treenode<valuetype>	*getPrevious();

		/** Returns the next node in the avltree (in an in-order,
		 *  depth-first traversal) or NULL if this node is the last
		 *  node in the tree. */
		treenode<valuetype>	*getNext();

		/** Prints the value stored in the node. */
		void	print() const;

	#include <rudiments/private/avltreenode.h>
};

/** The avltree class allows you to store an arbitrary number of values in a
 *  self-sorting, self-balancing binary tree.  Since the avltree class is
 *  template-based, you can store arbitrary types of values.
 * 
 *  Each avltree is composed of a set of avltreenodes.  Each avltreenode
 *  contains a value. */
template <class valuetype>
class avltree : public treecollection<valuetype> {
	public:
		/** Creates an empty instance of the avltree class. */
		avltree();

		/** Deletes this instance of the avltree class and all of
		 *  its avltreenodes.  Note however, that the value stored
		 *  in each avltreenode is not deleted by this call. */
		~avltree();

		/** Creates a new avltreenode containing "value" and
		 *  prepends it to the avltree. */
		void	insert(valuetype value);

		/** Inserts already created avltreenode "node" to the
		 *  avltree. */
		void	insert(treenode<valuetype> *node);

		/** Detaches "node" from the tree. */
		treenode<valuetype>
			*detach(treenode<valuetype> *node);

		/** Deletes the first avltreenode containing "value".
		 *
		 *  The value stored in the avltreenode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all avltreenodes containing "value".
		 *
		 *  The value stored in each avltreenode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removed avltreenode "node" from the avltree.
		 *
		 *  The value stored in the avltreenode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation does not require a search and is
		 *  far less expensive than the remove(value) operation and
		 *  removeAll().
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(treenode<valuetype> *node);

		/** Returns the number of nodes in the avltree. */
		uint64_t	getLength() const;

		/** Returns the top-most node in the avltree. */
		treenode<valuetype>	*getTop();

		/** Returns the first node in the avltree (in an in-order,
		 *  depth-first traversal). */
		treenode<valuetype>	*getFirst();

		/** Returns the last node in the avltree (in an in-order,
		 *  depth-first traversal). */
		treenode<valuetype>	*getLast();

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the tree (in an in-order, depth-first
		 *  traversal).  "node" is presumed to be in the tree. */
		treenode<valuetype>	*getPrevious(
					treenode<valuetype> *node);

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the tree (in an in-order, depth-first
		 *  traversal). "node" is presumed to be in the tree. */
		treenode<valuetype>	*getNext(
					treenode<valuetype> *node);

		/** Returns a pointer to the first avltreenode containing
		 *  "value" or NULL if "value" was not found. */
		treenode<valuetype>	*find(valuetype value);

		/** Returns a pointer to the first avltreenode below
		 *  "startnode" containing "value" or NULL if "value" was not
		 *  found. */
		treenode<valuetype>
			*find(treenode<valuetype> *startnode,
							valuetype value);

		/** Deletes all avltreenodes currently in the avltree.
		 *
		 *  The value stored in each avltreenode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		void	clear();

		/** Prints out an xml-style representation of the avltree. */
		void	print() const;

	#include <rudiments/private/avltree.h>
};

#include <rudiments/private/avltreeinlines.h>

#endif
