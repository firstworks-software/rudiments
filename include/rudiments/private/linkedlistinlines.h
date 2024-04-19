// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
linkedlist<valuetype>::linkedlist() :
	listcollection<valuetype>(),
	first(NULL),
	last(NULL),
	count(0) {
#ifdef DARWIN_GCC_2952_HACKS
	return;

	// On Darwin platforms, when using gcc 2.95.2 (and possibly other
	// versions), if the calling app uses linkedlist, but doesn't happen to
	// directly use linkedlistnode, even if it uses listnode, then the code
	// for linkedlistnode doesn't get pulled in, and various undefined
	// symbols result.  Adding a call here, after the return causes the
	// code to be included and the symbols to be defined.  It's called
	// here, after the return so that it never actually gets executed.
	// The old compiler doesn't complain about that.
	first=(linkedlistnode<valuetype> *)getFirst();
#endif
}

template <class valuetype>
inline
linkedlist<valuetype>::linkedlist(linkedlist<valuetype> &a) :
					listcollection<valuetype>(a) {
	clone(&a);
#ifdef DARWIN_GCC_2952_HACKS
	return;

	// see note above
	first=(linkedlistnode<valuetype> *)getFirst();
#endif
}

template <class valuetype>
inline
linkedlist<valuetype>::linkedlist(nodecollection<valuetype> &a) :
					listcollection<valuetype>(a) {
	clone(&a);
#ifdef DARWIN_GCC_2952_HACKS
	return;

	// see note above
	first=(linkedlistnode<valuetype> *)getFirst();
#endif
}

template <class valuetype>
inline
linkedlist<valuetype> &linkedlist<valuetype>::operator=(
					linkedlist<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
linkedlist<valuetype> &linkedlist<valuetype>::operator=(
					nodecollection<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void linkedlist<valuetype>::clone(nodecollection<valuetype> *coll) {
	first=NULL;
	last=NULL;
	count=0;
	bool	managevalues=this->getManageValues();
	bool	managearrayvalues=this->getManageArrayValues();
	for (nodecollectionnode<valuetype> *node=coll->getFirst();
						node; node=node->getNext()) {
		append(node_duplicate_value(&(node->getReference()),
					managevalues,managearrayvalues));
	}
}

template <class valuetype>
inline
linkedlist<valuetype>::~linkedlist() {
	clear();
}

template <class valuetype>
inline
void linkedlist<valuetype>::prepend(valuetype value) {
	prepend(new linkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void linkedlist<valuetype>::prepend(listnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (first) {
		first->setPrevious(node);
		node->setNext(first);
		first=node;
	} else {
		first=node;
		last=first;
	}
	count++;
}

template <class valuetype>
inline
void linkedlist<valuetype>::append(valuetype value) {
	append(new linkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void linkedlist<valuetype>::append(listnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (last) {
		last->setNext(node);
		node->setPrevious(last);
		last=node;
	} else {
		first=node;
		last=first;
	}
	count++;
}

template <class valuetype>
inline
void linkedlist<valuetype>::insertBefore(listnode<valuetype> *node,
							valuetype value) {
	insertBefore(node,new linkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void linkedlist<valuetype>::insertBefore(listnode<valuetype> *node,
						listnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==first) {
		prepend(newnode);
	} else {
		newnode->setNext(node);
		newnode->setPrevious(node->getPrevious());
		node->getPrevious()->setNext(newnode);
		node->setPrevious(newnode);
		count++;
	}
}

template <class valuetype>
inline
void linkedlist<valuetype>::insertAfter(listnode<valuetype> *node,
							valuetype value) {
	insertAfter(node,new linkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void linkedlist<valuetype>::insertAfter(listnode<valuetype> *node,
					listnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==last) {
		append(newnode);
	} else {
		newnode->setNext(node->getNext());
		newnode->setPrevious(node);
		node->getNext()->setPrevious(newnode);
		node->setNext(newnode);
		count++;
	}
}

template <class valuetype>
inline
void linkedlist<valuetype>::moveBefore(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove) {
	move(node,nodetomove,true);
}

template <class valuetype>
inline
void linkedlist<valuetype>::moveAfter(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove) {
	move(node,nodetomove,false);
}

template <class valuetype>
inline
void linkedlist<valuetype>::move(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove,
					bool before) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	detach(nodetomove);
	if (before) {
		insertBefore(node,nodetomove);
	} else {
		insertAfter(node,nodetomove);
	}
}

template <class valuetype>
inline
void linkedlist<valuetype>::detach(listnode<valuetype> *node) {

	if (node==first) {
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	node->setNext(NULL);
	node->setPrevious(NULL);
	count--;
}

template <class valuetype>
inline
bool linkedlist<valuetype>::remove(valuetype value) {
	listnode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

template <class valuetype>
inline
bool linkedlist<valuetype>::removeAll(valuetype value) {

	listnode<valuetype>	*current=first;
	listnode<valuetype>	*next;
	while (current) {
		next=current->getNext();
		if (!this->getComparator()->compare(
					current->getValue(),value) &&
					!remove(current)) {
			return false;
		}
		current=next;
	}
	return true;
}

template <class valuetype>
inline
bool linkedlist<valuetype>::remove(listnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node==first) {
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	node_delete_value(&(node->getReference()),
				this->getManageValues(),
				this->getManageArrayValues());
	delete node;
	count--;
	return true;
}

template <class valuetype>
inline
uint64_t linkedlist<valuetype>::getCount() {
	return count;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::getFirst() {
	return first;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::getLast() {
	return last;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::getPrevious(
					listnode<valuetype> *node) {
	return (node)?node->getPrevious():NULL;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::getNext(
					listnode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::find(valuetype value) {
	return find(first,value);
}

template <class valuetype>
inline
listnode<valuetype> *linkedlist<valuetype>::find(
					listnode<valuetype> *startnode,
					valuetype value) {
	for (listnode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!this->getComparator()->compare(
					current->getValue(),value)) {
			return current;
		}
	}
	return NULL;
}

template <class valuetype>
inline
void linkedlist<valuetype>::sortInexpensively() {

	// insertion sort with a few optimizations...

	// if there are 0 or 1 items in the list then it's already sorted
	if (count<2) {
		return;
	}

	// first and last pointers for the new list
	listnode<valuetype>	*newfirst=NULL;
	listnode<valuetype>	*newlast=NULL;

	// pointer for iterating through the new list
	listnode<valuetype>	*currentfromfirst=NULL;
	listnode<valuetype>	*currentfromlast=NULL;

	// iterate through the current list, building a new one as we go
	listnode<valuetype>	*node=first;
	listnode<valuetype>	*next=NULL;
	while (node) {

		// get the next node so we can move on later
		next=node->getNext();

		// if the new list is empty...
		if (!newfirst) {
			node->setPrevious(NULL);
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else

		// if the node belongs at the beginning of the new list
		// (optimization for lists that are already largely forwards)
		if (this->getComparator()->compare(newfirst->getValue(),
							node->getValue())>0) {
			node->setNext(newfirst);
			node->setPrevious(NULL);
			newfirst->setPrevious(node);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (this->getComparator()->compare(newlast->getValue(),
							node->getValue())<=0) {
			node->setPrevious(newlast);
			node->setNext(NULL);
			newlast->setNext(node);
			newlast=node;
		} else

		// if the node belongs somewhere in the middle of the new list
		{

			// search from both ends toward the middle...
			// (optimization for data that is more random)
			currentfromfirst=newfirst->getNext();
			currentfromlast=newlast->getPrevious();
			while (currentfromfirst) {

				// if the current node (from the left)
				// is greater than...
				if (this->getComparator()->compare(
						currentfromfirst->getValue(),
						node->getValue())>0) {

					// insert before
					node->setNext(currentfromfirst);
					node->setPrevious(currentfromfirst->
								getPrevious());
					currentfromfirst->
						getPrevious()->setNext(node);
					currentfromfirst->
						setPrevious(node);
					break;

				} else

				// if the current node (from the right)
				// is less than or equal to...
				if (this->getComparator()->compare(
						currentfromlast->getValue(),
						node->getValue())<=0) {

					// insert after
					node->setPrevious(currentfromlast);
					node->setNext(currentfromlast->
								getNext());
					currentfromlast->
						getNext()->setPrevious(node);
					currentfromlast->
						setNext(node);
					break;
				}

				// move on
				currentfromfirst=currentfromfirst->getNext();
				currentfromlast=currentfromlast->getPrevious();
			}
		}

		// move on
		node=next;
	}

	// make the new list the current list
	first=newfirst;
	last=newlast;
}

template <class valuetype>
inline
void linkedlist<valuetype>::sortQuickly() {

	// if there are 0 or 1 items in the list then it's already sorted
	if (count<2) {
		return;
	}

	// build heap as a binary tree mapped to an array:
	//	parentindex = floor((childindex-1)/2)
	//	leftchildindex = parent*2+1
	//	rightchildindex = parent*2+2
	listnode<valuetype>	**heap=new listnode<valuetype> *[count];
	listnode<valuetype>	*temp=NULL;
	uint64_t		heapend=0;
	for (listnode<valuetype> *node=first; node; node=node->getNext()) {

		// insert node into heap
		heap[heapend]=node;

		// walk up the tree, maintaining the heap property
		// (higher values higher up in the tree)
		uint64_t	child=heapend;
		while (child) {

			// get the parent index
			uint64_t	parent=(child-1)/2;

			// swap nodes if necessary
			if (this->getComparator()->compare(
						heap[parent]->getValue(),
						heap[child]->getValue())<0) {
				temp=heap[parent];
				heap[parent]=heap[child];
				heap[child]=temp;
			}

			// move up
			child=parent;
		}
		
		// move on
		heapend++;
	}

	// reset the heap end index
	heapend--;

	// Build a new list from the heap by swapping the root and last leaf
	// node (index 0 is the root and the last index is the last leaf),
	// pulling the value off of the last leaf node, and sifting the tree to
	// maintain the heap property (higher values higher up in the tree),
	// over and over again.  We'll shortcut the swap and pull-off part a
	// bit...

	// first and last pointers for the new list
	listnode<valuetype>	*newfirst=NULL;
	listnode<valuetype>	*newlast=NULL;

	// extract values from the heap...
	for (;;) {

		// pull off the highest value (which is always at the root
		// of the tree, index 0 in the array) and prepend it to the
		// new array
		listnode<valuetype>	*node=heap[0];
		if (!newfirst) {
			node->setPrevious(NULL);
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else {
			node->setPrevious(NULL);
			node->setNext(newfirst);
			newfirst->setPrevious(node);
			newfirst=node;
		}

		// when the tree is empty, we're done
		if (!heapend) {

			// make the new list the current list
			first=newfirst;
			last=newlast;

			// clean up
			delete[] heap;
			return;
		}

		// move the value at the last leaf node (end of the array)
		// to the root node (index 0 of the array)
		heap[0]=heap[heapend];
		heapend--;

		// sift the tree to maintain the heap property
		// (higher values higher up in the tree)
		uint64_t parent=0;
		for (;;) {

			// make sure there's at least a left child
			uint64_t	leftchild=parent*2+1;
			if (leftchild>heapend) {
				break;
			}

			// is the left child greater?
			uint64_t	greater=parent;
			if (this->getComparator()->compare(
					heap[parent]->getValue(),
					heap[leftchild]->getValue())<0) {
				greater=leftchild;
			}

			// is the right child greater?
			uint64_t	rightchild=leftchild+1;
			if (rightchild<=heapend &&
				this->getComparator()->compare(
						heap[rightchild]->getValue(),
						heap[greater]->getValue())>0) {
				greater=rightchild;
			}

			// if the parent was greater than each child then
			// we don't need to continue sifting
			if (greater==parent) {
				break;
			}

			// if one of the children was greater than the parent
			// then swap them and continue down the tree in the
			// direction of the child that was swapped
			temp=heap[parent];
			heap[parent]=heap[greater];
			heap[greater]=temp;
			parent=greater;
		}
	}
}

template <class valuetype>
inline
bool linkedlist<valuetype>::clear() {
	listnode<valuetype>	*next;
	listnode<valuetype>	*current=first;
	bool	managevalues=this->getManageValues();
	bool	managearrayvalues=this->getManageArrayValues();
	while (current) {
		next=current->getNext();
		node_delete_value(&(current->getReference()),
					managevalues,managearrayvalues);
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	count=0;
	return true;
}

template <class valuetype>
inline
linkedlistnode<valuetype>::linkedlistnode(valuetype value) :
	listnode<valuetype>(),
	value(value),
	next(NULL),
	previous(NULL) {
}

template <class valuetype>
inline
linkedlistnode<valuetype>::~linkedlistnode() {
}

template <class valuetype>
inline
void linkedlistnode<valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class valuetype>
inline
valuetype linkedlistnode<valuetype>::getValue() {
	return value;
}

template <class valuetype>
inline
valuetype &linkedlistnode<valuetype>::getReference() {
	return value;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlistnode<valuetype>::getPrevious() {
	return previous;
}

template <class valuetype>
inline
listnode<valuetype> *linkedlistnode<valuetype>::getNext() {
	return next;
}

template <class valuetype>
inline
void linkedlistnode<valuetype>::setPrevious(listnode<valuetype> *previous) {
	this->previous=previous;
}

template <class valuetype>
inline
void linkedlistnode<valuetype>::setNext(listnode<valuetype> *next) {
	this->next=next;
}
