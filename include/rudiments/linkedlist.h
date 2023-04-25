// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_H
#define RUDIMENTS_LINKEDLIST_H

#include <rudiments/private/linkedlistincludes.h>

/** The linkedlistnode class stores the values that compose a linkedlist. */
template <class valuetype>
class linkedlistnode : public listnode<valuetype> {
	public:
		/** Creates an instance of the linkedlistnode class that
		 *  stores value "value". */
		linkedlistnode(valuetype value);

		/** Deletes this instance of the linkedlistnode class. */
		~linkedlistnode();

		/** Set the value stored in the node to "value". */
		void		setValue(valuetype value);

		/** Returns the value stored in the node. */
		valuetype	getValue();

		/** Returns a reference to the value stored in the node. */
		valuetype	&getReference();

		/** Returns the previous node in the linkedlist or NULL
		 *  if this node is the first node in the list. */
		listnode<valuetype>	*getPrevious();

		/** Returns the next node in the linkedlist or NULL
		 * if this node is the last node in the list. */
		listnode<valuetype>	*getNext();

	#include <rudiments/private/linkedlistnode.h>
};

/** The linkedlist class allows you to store an arbitrary number of values in a
 *  doubly-linked list.  Since the linkedlist class is template-based, you can
 *  store arbitrary types of values.
 * 
 *  Each linkedlist is composed of a series of linkedlistnodes.  Each
 *  linkedlistnode contains a value. */
template <class valuetype>
class linkedlist : public listcollection<valuetype> {
	public:
		/** Creates an empty instance of the linkedlist class. */
		linkedlist();

		/** Creates an instance of the linkedlist class
		 *  that is a copy of "a". */
		linkedlist(linkedlist<valuetype> &a);

		/** Creates an instance of the linkedlist class
		 *  that is a copy of "a". */
		linkedlist(nodecollection<valuetype> &a);

		/** Makes this instance of the linkedlist class
		 *  identical to "a". */
		linkedlist<valuetype>	&operator=(
					linkedlist<valuetype> &a);

		/** Makes this instance of the linkedlist class
		 *  identical to "a". */
		linkedlist<valuetype>	&operator=(
					nodecollection<valuetype> &a);

		/** Deletes this instance of the linkedlist class and all of
		 *  its linkedlistnodes.
		 *
		 *  The value stored in each linkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		~linkedlist();

		/** Creates a new linkedlistnode containing "value" and
		 *  prepends it to the linkedlist. */
		void	prepend(valuetype value);

		/** Prepends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	prepend(listnode<valuetype> *node);

		/** Creates a new linkedlistnode containing "value" and
		 *  appends it to the linkedlist. */
		void	append(valuetype value);

		/** Appends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	append(listnode<valuetype> *node);

		/** Creates a new linkedlistnode containing "value" and
		 *  inserts it into the linkedlist before "node". */
		void	insertBefore(listnode<valuetype> *node,
							valuetype value);

		/** Inserts already created linkedlistnode "newnode" into the
		 *  linkedlist before "node". */
		void	insertBefore(listnode<valuetype> *node,
					listnode<valuetype> *newnode);

		/** Creates a new linkedlistnode containing "value" and
		 *  inserts it into the linkedlist after "node". */
		void	insertAfter(listnode<valuetype> *node,
							valuetype value);

		/** Inserts already created linkedlistnode "newnode" into the
		 *  linkedlist after "node". */
		void	insertAfter(listnode<valuetype> *node,
					listnode<valuetype> *newnode);

		/** Moves node "nodetomove" to the position before "node" in
		 *  the linkedlist. */
		void	moveBefore(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove);

		/** Moves node "nodetomove" to the position after "node" in
		 *  the linkedlist. */
		void	moveAfter(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove);

		/** Detaches "node" from the list. */
		void	detach(listnode<valuetype> *node);

		/** Deletes the first linkedlistnode containing "value".
		 *
		 *  The value stored in the linkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all linkedlistnodes containing "value".
		 *
		 *  The value stored in each linkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removes linkedlistnode "node" from the linkedlist.
		 *
		 *  The value stored in the linkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 * 
		 *  Note that this operation does not require a search and is
		 *  far less expensive than the remove(value) operation and
		 *  removeAll().
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(listnode<valuetype> *node);

		/** Returns the number of nodes in the linkedlist. */
		uint64_t	getLength();

		/** Returns the first node in the linkedlist. */
		listnode<valuetype>	*getFirst();

		/** Returns the last node in the linkedlist. */
		listnode<valuetype>	*getLast();

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the list.  "node" is presumed to be in
		 *  the list. */
		listnode<valuetype>	*getPrevious(
					listnode<valuetype> *node);

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		listnode<valuetype>	*getNext(
					listnode<valuetype> *node);

		/** Returns a pointer to the first linkedlistnode
		 *  containing "value" or NULL if "value" was not found. */
		listnode<valuetype>	*find(valuetype value);

		/** Returns a pointer to the first linkedlistnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		listnode<valuetype>	*find(listnode<valuetype> *startnode,
							valuetype value);

		/** Sorts the listcollection.
		 *
		 *  This sort is potentially much slower than sortQuickly() but
		 *  uses no additional memory.
		 *
		 *  The order that the items are sorted into depends on the
		 *  comparator that is being used, and how that comparator is
		 *  configured.  The default comparator, in its default
		 *  configuration causes the list to be sorted in ascending
		 *  order.
		 *
		 *  See collection::setComparator() and the
		 *  comparator class for more detail. */
		void	sortInexpensively();

		/** Sorts the listcollection.
		 *
		 *  This sort is potentially much faster than
		 *  sortInexpensively() but uses additional memory in
		 *  proportion to the size of the list.
		 *
		 *  The order that the items are sorted into depends on the
		 *  comparator that is being used, and how that comparator is
		 *  configured.  The default comparator, in its default
		 *  configuration causes the list to be sorted in ascending
		 *  order.
		 *
		 *  See collection::setComparator() and the
		 *  comparator class for more detail. */
		void	sortQuickly();

		/** Deletes all linkedlistnodes currently in the linkedlist.
		 *
		 *  The value stored in each linkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 *
		 *  Always returns true. */
		bool	clear();

	#include <rudiments/private/linkedlist.h>
};

#include <rudiments/private/linkedlistinlines.h>

#endif
