// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTCOLLECTION_H
#define RUDIMENTS_LISTCOLLECTION_H

#include <rudiments/nodecollection.h>

/** The listnode class is the parent class for the node contained by all
 *  rudiments listnode-based collections. */
template <class valuetype>
class listnode : public nodecollectionnode<valuetype> {
	public:
		/** Set the value stored in the node to "value". */
		virtual	void		setValue(valuetype value)=0;

		/** Returns the value stored in the node. */
		virtual	valuetype	getValue() const=0;

		/** Returns the previous node in the listcollection or NULL
		 *  if this node is the first node in the list. */
		virtual	listnode<valuetype>	*getPrevious() const=0;

		/** Returns the next node in the listcollection or NULL
		 * if this node is the last node in the list. */
		virtual	listnode<valuetype>	*getNext() const=0;

	#include <rudiments/private/listnode.h>
};

/** The listcollection class is the parent class for all rudiments list
 *  collections. */
template <class valuetype>
class listcollection : public nodecollection<valuetype> {
	public:
		/** Creates an empty instance of the listcollection class. */
		listcollection() : nodecollection<valuetype>() {}

		/** Creates an instance of the listcollection class
		 *  that is a copy of "a". */
		listcollection(const nodecollection<valuetype> &a) :
					nodecollection<valuetype>(a) {}

		/** Creates a new listnode containing "value" and
		 *  prepends it to the listcollection. */
		virtual void	prepend(valuetype value)=0;

		/** Creates new listnodes containing "count" members of
		 *  array "value" and prepends them to the listcollection. */
		virtual void	prepend(valuetype *values, uint64_t count);

		/** Prepends already created listnode "node" to the
		 *  listcollection. */
		virtual void	prepend(listnode<valuetype> *node)=0;

		/** Creates a new listnode containing "value" and
		 *  appends it to the listcollection. */
		virtual void	append(valuetype value)=0;

		/** Creates new listnodes containing "count" members of
		 *  array "value" and appends them to the listcollection. */
		virtual void	append(valuetype *values, uint64_t count);

		/** Appends already created listnode "node" to the
		 *  listcollection. */
		virtual void	append(listnode<valuetype> *node)=0;

		/** Creates a new listnode containing "value" and
		 *  inserts it into the listcollection before "node". */
		virtual void	insertBefore(listnode<valuetype> *node,
							valuetype value)=0;

		/** Creates new listnodes containing "count" members of
		 *  array "value" and inserts them into the listcollection
		 *  before "node". */
		virtual void	insertBefore(listnode<valuetype> *node,
					valuetype *values, uint64_t count);

		/** Inserts already created listnode "newnode" into the
		 *  listcollection before "node". */
		virtual void	insertBefore(listnode<valuetype> *node,
					listnode<valuetype> *newnode)=0;

		/** Creates a new listnode containing "value" and
		 *  inserts it into the listcollection after "node". */
		virtual void	insertAfter(listnode<valuetype> *node,
							valuetype value)=0;

		/** Creates new listnodes containing "count" members of
		 *  array "value" and inserts them into the listcollection
		 *  after "node". */
		virtual void	insertAfter(listnode<valuetype> *node,
					valuetype *values, uint64_t count);

		/** Inserts already created listnode "newnode" into the
		 *  listcollection after "node". */
		virtual void	insertAfter(listnode<valuetype> *node,
					listnode<valuetype> *newnode)=0;

		/** Moves node "nodetomove" to the position before "node" in
		 *  the listcollection. */
		virtual void	moveBefore(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove)=0;

		/** Moves node "nodetomove" to the position after "node" in
		 *  the listcollection. */
		virtual void	moveAfter(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove)=0;

		/** Detaches "node" from the list. */
		virtual void	detach(listnode<valuetype> *node)=0;

		/** Deletes the first listnode containing "value".
		 *
		 *  The value stored in the listnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	remove(valuetype value)=0;

		/** Deletes all listnodes containing "value".
		 *
		 *  The value stored in each listnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 * 
		 *  Returns true on success and false on failure. */
		virtual bool	removeAll(valuetype value)=0;

		/** Removes listnode "node" from the listcollection.
		 *
		 *  The value stored in the listnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation does not require a search and is
		 *  far less expensive than the remove(value) operation and
		 *  removeAll().
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	remove(listnode<valuetype> *node)=0;

		/** Returns the number of nodes in the listcollection. */
		virtual	uint64_t	getLength() const=0;

		/** Returns the first node in the listcollection. */
		virtual	listnode<valuetype>	*getFirst() const=0;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		virtual	listnode<valuetype>	*getNext(
					listnode<valuetype> *node) const=0;

		/** Returns a pointer to the first listnode
		 *  containing "value" or NULL if "value" was not found. */
		virtual	listnode<valuetype>	*find(valuetype value) const=0;

		/** Returns a pointer to the first listnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		virtual	listnode<valuetype>	*find(
						listnode<valuetype> *startnode,
						valuetype value) const=0;

		/** Sorts the listcollection in ascending order using a modified
		 *  insertion sort algorithm.  This sort is slower than
		 *  heapSort() but uses no additional memory. */
		virtual void	insertionSort()=0;

		/** Sorts the listcollection in ascending order using a heap
		 *  sort algorithm.  This sort is faster than heapSort() but
		 *  uses additional memory in proportion to the size of the
		 *  list. */
		virtual void	heapSort()=0;

		/** Writes a representation of the listcollection to stdout. */
		virtual	ssize_t	write() const;

		/** Writes a representation of the listcollection to "out". */
		virtual	ssize_t	write(output *out) const;
};

#include <rudiments/private/listcollectioninlines.h>

#endif
