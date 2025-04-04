// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TREECOLLECTION_H
#define RUDIMENTS_TREECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The treenode class is the parent class for the node contained by all
 *  rudiments treenode-based collections. */
template <class valuetype>
class treenode : public nodecollectionnode<valuetype> {
	public:
		/** Deletes this instance of treenode. */
		virtual ~treenode() {};

		/** Sets the value stored in the node to "value". */
		virtual void	setValue(valuetype value)=0;

		/** Returns the value stored in the node. */
		virtual valuetype	getValue()=0;

		/** Returns a reference to the value stored in the node. */
		virtual valuetype	&getReference()=0;

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

	#include <rudiments/private/treenode.h>
};

/** The treecollection class is the parent class for all rudiments tree
 *  collections. */
template <class valuetype>
class treecollection : public nodecollection<valuetype> {
	public:
		/** Creates an empty instance of the treecollection class. */
		treecollection();

		/** Creates an instance of the treecollection class
		 *  that is a copy of "a". */
		treecollection(nodecollection<valuetype> &n);

		/** Returns "tree". */
		virtual const char	*getType();

		/** Creates a new treenode containing "value" and
		 *  prepends it to the avltree. */
		virtual void	insert(valuetype value)=0;

		/** Creates new treenodes containing "count" members of
		 *  array "value" and inserts them into the treecollection. */
		virtual void	insert(valuetype *value, uint64_t count);

		/** Inserts already created treenode "node" to the
		 *  treecollection. */
		virtual void	insert(treenode<valuetype> *node)=0;

		/** Detaches "node" from the tree. */
		virtual treenode<valuetype>
			*detach(treenode<valuetype> *node)=0;

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
		virtual bool	remove(valuetype value)=0;

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
		virtual bool	removeAll(valuetype value)=0;

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
		virtual bool	remove(treenode<valuetype> *node)=0;

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

		/** Writes a representation of the treecollection to
		 *  standard output. */
		virtual	ssize_t	write();

		/** Writes a representation of the treecollection to "out". */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the treecollection to
		 *  standard output. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the treecollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the treecollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the treecollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent);

		/** Writes an XML representation of the treecollection to
		 *  standard output. */
		virtual	ssize_t	writeXml();

		/** Writes an XML representation of the treecollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeXml(bool indent);

		/** Writes an XML representation of the treecollection to
		 *  "out". */
		virtual	ssize_t	writeXml(output *out);

		/** Writes an XML representation of the treecollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeXml(output *out, bool indent);

	#include <rudiments/private/treecollection.h>
};

#include <rudiments/private/treecollectioninlines.h>

#endif
