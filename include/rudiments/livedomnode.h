// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LIVEDOMNODE_H
#define RUDIMENTS_LIVEDOMNODE_H

#include <rudiments/private/livedomnodeincludes.h>

class livedomnodeprivate;

/** The livedomnode class provides a ... */
class RUDIMENTS_DLLSPEC livedomnode : public domnode {
	public:
		/** Creates a new node and intializes its
		 *  member variables to NULL.
		 * 
		 *  Your application should pass in a special
		 *  "nullnode" which may be created by the
		 *  static method createNullNode() below.
		 * 
		 *  This will keep command chaining like this:
		 * 
		 *  mynode->getFirstChild("node1")->
		 *  	getFirstChild("node2")->getName("node3");
		 * 
		 *  from causing the program to crash trying to
		 *  dereference a NULL pointer if, for example,
		 *  "node2" doesn't exist. */
		livedomnode(dom *dom, domnode *nullnode);

		/** Deletes the node, all attribute nodes and
		 *  optionally all child nodes, recursively. */
		virtual	~livedomnode();

		/** Returns the type of the node. */
		virtual	domnodetype	getType() const;

		/** Returns the namespace of the node. */
		virtual	const char	*getNamespace() const;

		/** Returns the name of the node. */
		virtual	const char	*getName() const;

		/** Returns the value of node. */
		virtual	const char	*getValue() const;

		/** Returns a pointer to the parent node or the
		 *  nullnode if none exists. */
		virtual	domnode	*getParent() const;

		/** Returns the position of the node, relative to its
		 *  siblings. */
		virtual	uint64_t	getPosition() const;

		/** Returns a pointer to the previous sibling
		 *  node or the nullnode if none exists. */
		virtual	domnode	*getPreviousSibling() const;

		/** Returns a pointer to the next sibling node
		 *  or the nullnode if none exists. */
		virtual	domnode	*getNextSibling() const;

		/** Returns the number of immediate child nodes. */
		virtual	uint64_t	getChildCount() const;

		/** Returns the first child node or the nullnode
		 *  if no children are found. */
		virtual	domnode	*getFirstChild() const;

		/** Returns the number of attributes. */
		virtual	uint64_t	getAttributeCount() const;

		/** Returns the attribute named "name"
		 *  or the nullnode if not found. */
		virtual	domnode	*getAttribute(const char *name) const;

		/** Returns the attribute named "name" (ignoring case)
		 *  or the nullnode if not found. */
		virtual	domnode	*getAttributeIgnoringCase(
						const char *name) const;

		/** Returns the attribute node at index
		 *  "position" or the nullnode if not found. */
		virtual	domnode	*getAttribute(uint64_t position) const;

		/** Returns the value of the attribute named
		 *  "name" or the nullnode if not found. */
		virtual	const char	*getAttributeValue(
						const char *name) const;

		/** Returns the value of the attribute node at
		 *  index "position" or the nullnode if not
		 *  found. */
		virtual	const char	*getAttributeValue(
						uint64_t position) const;

		/** Returns true if this node is the special
		 *  nullnode and false otherwise. */
		virtual	bool	isNullNode() const;

	#include <rudiments/private/livedomnode.h>
};

#endif
