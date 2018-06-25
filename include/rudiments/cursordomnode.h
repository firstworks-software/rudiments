// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CURSORDOMNODE_H
#define RUDIMENTS_CURSORDOMNODE_H

#include <rudiments/private/cursordomnodeincludes.h>

class cursordomnodeprivate;

/** The cursordomnode class provides a base class for a special kind of domnode
 *  which allows data from a backend data store to be embedded in a dom tree
 *  without having to actually copy the data into memory.
 *
 *
 *  A normal domnode has child nodes, which can be accessed via methods like
 *  getFirstChild(), getFirstTagChild(), getChild(n), etc.  Once you have
 *  accessed a child node, you can access its children in a similar manner,
 *  or step between its siblings using methods like getNextSibling(),
 *  getNextTagSibling(), getPreviousSibling(), getPreviousTagSibling(), etc.
 *  Each node returned by one of the above mentioned methods is distinct from
 *  the node that the method was called on.
 *
 *  A cursordomnode provides this same interface to an arbitrary backend data
 *  source, via a single domnode, by stepping a "cursor" through the data
 *  source.
 *
 *  For example, a sql result might have a structure similar to:
 *
 *     resultset {
 *             columns {
 *                     column: "user_id"
 *                     column: "name"
 *                     column: "description"
 *             }
 *             rows {
 *                     row {
 *                             field: "0"
 *                             field: "car"
 *                             field: "A small vehicle for driving around."
 *                     }
 *                     row {
 *                             field: "0"
 *                             field: "truck"
 *                             field: "A large vehicle for hauling cargo."
 *                     }
 *             }
 *     }
 *
 *  A child class of cursordomnode that implements the cursordomnode interface
 *  for a sql result set would step a virtual "cursor" through the different
 *  parts of the result set and calls to other domnode methods would return the
 *  data at that location.
 *
 *  For example, the cursor would be initially positioned on "resultset".
 *
 *  -> resultset {
 *             columns {
 *                     ...
 *             }
 *             rows {
 *                     ...
 *             }
 *     }
 *
 *  Calling getFirstChild() would position the cursor on "columns".
 *
 *     resultset {
 *          -> columns {
 *                     ...
 *             }
 *             rows {
 *                     ...
 *             }
 *     }
 *
 *  Calling getNextSibling() would position the cursor on "rows".
 *
 *     resultset {
 *             columns {
 *                     ...
 *             }
 *          -> rows {
 *                     ...
 *             }
 *     }
 *
 *  Calling getParent() would position the cursor back on "resultset",
 *  and so on.
 *
 *  If the cursor is positioned on a "column" or "field" then
 *  getAttributeValue() might be implemented to return the column name
 *  or field value.
 *
 *  It is important to note that since this is all being implemented by a
 *  single domnode, which just steps a virtual cursor around a backend data
 *  store, that each of the getParent(), getNext/PreviousChild(), etc. calls
 *  all return the SAME domnode.
 *
 *  For example, in the code:
 *  	domnode *node=...some cursordomnode...;
 *  	...
 *  	domnode	*child=node->getFirstChild();
 *  	domnode	*next=child->getNextSibling();
 *  	domnode *prev=child->getPreviousSibling();
 *
 *  "node", "child", "next", and "prev" would all point to the same node.
 *  The calls just cause the domnode to move its cursor around the backend
 *  data store.  As such, it isn't possible to access two parts of the data
 *  store simultaneously, without copying them out.
 *
 *  There are two caveats to this behavior, but they should be intiutive:
 *
 *  * If the cursor is positioned on the top-level virtual node, then calls to
 *    getParent(), getNextSibling(), getPreviousSibling(), etc. will return
 *    legitimate parent or sibling nodes.
 *
 *  * If the cursor is stepped off of the end or beginning of something (eg. if
 *    it is positioned on the last row and getNextSibling() is called) then the
 *    call will return the nullnode.
 *
 *  Since most of the data is provided by a backend data store, a
 *  possibly-unintuitive consequence is that the cursordomnode is generally
 *  read-only.  The namespace can be set, but any attempt to set the type, name,
 *  or value, to set attribute values, or to insert, delete, or move nodes will
 *  fail.
 *
 *
 *  So, how is this useful?
 *
 *  Dom trees are traditionally stored entirely in-memory.  As mentioned above,
 *  the cursordomnode provides a framework for embedding data from a backend
 *  data store in a dom tree without having to actually copy the data into
 *  memory.
 *
 *  An example use case would be to embed an instance of a child of
 *  cursordomnode that represents a sql result set into a larger dom and then
 *  call dom->write(socketfd) to send the entire dom to a web browser across a
 *  socket.  Without a cursordomnode, the entire result set would have to be
 *  copied into the dom (and this into memory) before it could be sent.  With a
 *  cursordomnode, each value in the sql result set would be fetched and sent,
 *  on-demand, without having to copy anything in the dom.
 *
 *
 *  Again, it is important to note that the cursordomnode class itself is just
 *  a base class.  The default implementations of its members do nothing and
 *  are intended to be overriden in a data-source-specific manner by a child
 *  class.
 *
 * */
class RUDIMENTS_DLLSPEC cursordomnode : public domnode {
	public:
		/** Creates a new node and intializes its
		 *  member variables to NULL. */
		cursordomnode(dom *dom, domnode *nullnode);

		/** Creates a new node and intializes its
		 *  member variables to NULL. */
		cursordomnode(dom *dom, domnode *nullnode, const char *ns);

		/** Deletes the node, all attribute nodes and
		 *  optionally all child nodes, recursively. */
		virtual	~cursordomnode();

		/** Returns the type of the node. */
		virtual	domnodetype	getType() const;

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

		/** Returns true if this node is the special
		 *  nullnode and false otherwise. */
		virtual	bool	isNullNode() const;

	#include <rudiments/private/cursordomnode.h>
};

#endif
