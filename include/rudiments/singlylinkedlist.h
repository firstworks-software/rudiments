// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SINGLYLINKEDLIST_H
#define RUDIMENTS_SINGLYLINKEDLIST_H

#include <rudiments/private/singlylinkedlistincludes.h>

/** The singlylinkedlistnode class stores the values that compose a
 *  singlylinkedlist. */
template <class valuetype>
class singlylinkedlistnode : public listnode<valuetype> {
	public:
		/** Creates an instance of the singlylinkedlistnode class that
		 *  stores value "value". */
		singlylinkedlistnode(valuetype value);

		/** Deletes this instance of the singlylinkedlistnode class. */
		~singlylinkedlistnode();

		/** Set the value stored in the node to "value". */
		void		setValue(valuetype value);

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Always returns NULL. */
		listnode<valuetype>	*getPrevious() const;

		/** Returns the next node in the singlylinkedlist or NULL
		 * if this node is the last node in the list. */
		listnode<valuetype>	*getNext() const;

	#include <rudiments/private/singlylinkedlistnode.h>
};

/** The singlylinkedlist class allows you to store an arbitrary number of
 *  values in a singly-linked list.  Since the singlylinkedlist class is
 *  template-based, you can store arbitrary types of values.
 * 
 *  Each singlylinkedlist is composed of a series of singlylinkedlistnodes.
 *  Each singlylinkedlistnode contains a value.
 *
 *  This class is similar to the linkedlist class but uses less memory and
 *  many of its operations are faster.
 *
 *  However, the move, detach and remove operations are much slower.  If your
 *  application must run these operations regularly, you should consider using
 *  the linkedlist class instead.  */
template <class valuetype>
class singlylinkedlist : public listcollection<valuetype> {
	public:
		/** Creates an empty instance of the singlylinkedlist class. */
		singlylinkedlist();

		/** Creates an instance of the singlylinkedlist class
		 *  that is a copy of "a". */
		singlylinkedlist(const singlylinkedlist<valuetype> &a);

		/** Creates an instance of the singlylinkedlist class
		 *  that is a copy of "a". */
		singlylinkedlist(const listcollection<valuetype> &a);

		/** Makes this instance of the singlylinkedlist class
		 *  identical to "a". */
		singlylinkedlist<valuetype>	&operator=(
					const singlylinkedlist<valuetype> &a);

		/** Makes this instance of the singlylinkedlist class
		 *  identical to "a". */
		singlylinkedlist<valuetype>	&operator=(
					const nodecollection<valuetype> &a);

		/** Deletes this instance of the singlylinkedlist class and all
		 *  of its singlylinkedlistnodes.
		 *
		 *  The value stored in each singlylinkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		~singlylinkedlist();

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  prepends it to the singlylinkedlist. */
		void	prepend(valuetype value);

		/** Prepends already created singlylinkedlistnode "node" to the
		 *  singlylinkedlist. */
		void	prepend(listnode<valuetype> *node);

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  appends it to the singlylinkedlist. */
		void	append(valuetype value);

		/** Appends already created singlylinkedlistnode "node" to the
		 *  singlylinkedlist. */
		void	append(listnode<valuetype> *node);

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  inserts it into the singlylinkedlist after "node". */
		void	insertAfter(listnode<valuetype> *node, valuetype value);

		/** Inserts already created singlylinkedlistnode "node" into the
		 *  singlylinkedlist after "node". */
		void	insertAfter(listnode<valuetype> *node,
					listnode<valuetype> *newnode);

		/** Moves node "nodetomove" to the position after "node" in
		 *  the singlylinkedlist.
		 *
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.  Consider using the
		 *  linkedlist class. */
		void	moveAfter(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove);

		/** Detaches "node" from the list.
		 *
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.  Consider using the
		 *  linkedlist class. */
		void	detach(listnode<valuetype> *node);

		/** Deletes the first singlylinkedlistnode containing "value".
		 *
		 *  The value stored in the singlylinkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 *
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.  Consider using the
		 *  linkedlist class.
		 * 
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all singlylinkedlistnodes containing "value".
		 *
		 *  The value stored in each singlylinkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 *
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.  Consider using the
		 *  linkedlist class.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removes singlylinkedlistnode "node" from the
		 *  singlylinkedlist.
		 *
		 *  The value stored in the singlylinkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.
		 *
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.  Consider using the
		 *  linkedlist class.
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(listnode<valuetype> *node);

		/** Returns the number of nodes in the singlylinkedlist. */
		uint64_t	getLength() const;

		/** Returns the first node in the singlylinkedlist. */
		listnode<valuetype>	*getFirst() const;

		/** Returns the last node in the singlylinkedlist. */
		listnode<valuetype>	*getLast() const;

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		listnode<valuetype>	*getNext(
					listnode<valuetype> *node) const;

		/** Returns a pointer to the first singlylinkedlistnode
		 *  containing "value" or NULL if "value" was not found. */
		listnode<valuetype>	*find(valuetype value) const;

		/** Returns a pointer to the first singlylinkedlistnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		listnode<valuetype> *find(listnode<valuetype> *startnode,
							valuetype value) const;

		/** Sorts the singlylinkedlist in ascending order using a
		 *  modified insertion sort algorithm.  This sort is slower
		 *  than heapSort() but uses no additional memory. */
		void	insertionSort();

		/** Sorts the singlylinkedlist in ascending order using a heap
 		 *  sort algorithm.  This sort is faster than heapSort() but
 		 *  uses additional memory in proportion to the size of the
 		 *  list. */
		void	heapSort();

		/** Deletes all singlylinkedlistnodes currently in the
		 *  singlylinkedlist.
		 *
		 *  The value stored in each singlylinkedlistnode is only
		 *  deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		void	clear();

		/** Writes out a representation of the linkedlist to "out". */
		bool	write(output *out) const;

	#include <rudiments/private/singlylinkedlist.h>
};


#include <rudiments/private/singlylinkedlistinlines.h>

#endif
