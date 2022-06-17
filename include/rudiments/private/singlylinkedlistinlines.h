// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
singlylinkedlist<valuetype>::singlylinkedlist() :
	listcollection<valuetype>(),
	first(NULL),
	last(NULL),
	length(0) {
}

template <class valuetype>
inline
singlylinkedlist<valuetype>::singlylinkedlist(
				const singlylinkedlist<valuetype> &a) :
				listcollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
singlylinkedlist<valuetype>::singlylinkedlist(
				const listcollection<valuetype> &a) :
				listcollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
singlylinkedlist<valuetype> &singlylinkedlist<valuetype>::operator=(
					const singlylinkedlist<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
singlylinkedlist<valuetype> &singlylinkedlist<valuetype>::operator=(
					const nodecollection<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::clone(const nodecollection<valuetype> *coll) {

	first=NULL;
	last=NULL;
	length=0;

	for (nodecollectionnode<valuetype> *node=coll->getFirst();
						node; node=node->getNext()) {
		append(node_duplicate_value(node->getValue(),
						this->getManageValues(),
						this->getManageArrayValues()));
	}
}

template <class valuetype>
inline
singlylinkedlist<valuetype>::~singlylinkedlist() {
	clear();
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::prepend(valuetype value) {
	prepend(new singlylinkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::prepend(listnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (first) {
		node->setNext(first);
		first=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::append(valuetype value) {
	append(new singlylinkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::append(listnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (last) {
		last->setNext(node);
		last=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::insertAfter(
					listnode<valuetype> *node,
					valuetype value) {
	insertAfter(node,new singlylinkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::insertAfter(
					listnode<valuetype> *node,
					listnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==last) {
		append(newnode);
	} else {
		newnode->setNext(node->getNext());
		node->setNext(newnode);
		length++;
	}
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::moveAfter(
					listnode<valuetype> *node,
					listnode<valuetype> *nodetomove) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	if (nodetomove==first) {
		first=nodetomove->getNext();
	} else if (nodetomove==last) {
		listnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		listnode<valuetype> *previous=first;
		while (previous->getNext()!=nodetomove) {
			previous=previous->getNext();
		}
		previous->setNext(nodetomove->getNext());
	}

	nodetomove->setNext(node->getNext());
	node->setNext(nodetomove);
	if (node==last) {
		last=nodetomove;
	}
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::detach(listnode<valuetype> *node) {

	if (node==first && node==last) {
		first=NULL;
		last=NULL;
	} else if (node==first) {
		first=node->getNext();
	} else if (node==last) {
		listnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		listnode<valuetype> *previous=first;
		while (previous->getNext()!=node) {
			previous=previous->getNext();
		}
		previous->setNext(node->getNext());
	}
	node->setNext(NULL);
	length--;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::remove(valuetype value) {
	listnode<valuetype> *current=first;
	if (!this->getComparator()->compare(current->getValue(),value)) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		listnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (!this->getComparator()->compare(
					current->getValue(),value)) {
				prev->setNext(current->getNext());
				break;
			}
			prev=current;
			current=current->getNext();
		}
		if (last==current) {
			last=prev;
		}
	}
	if (current) {
		node_delete_value(&(current->getValue()),
					this->getManageValues(),
					this->getManageArrayValues());
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::removeAll(valuetype value) {
	if (!first) {
		return true;
	}
	bool	retval=false;
	listnode<valuetype> *current=first;
	while (!this->getComparator()->compare(current->getValue(),value)) {
		retval=true;
		if (first==last) {
			first=NULL;
			last=NULL;
			node_delete_value(&(current->getValue()),
						this->getManageValues(),
						this->getManageArrayValues());
			delete current;
			length--;
			return true;
		} else {
			first=first->getNext();
			node_delete_value(&(current->getValue()),
						this->getManageValues(),
						this->getManageArrayValues());
			delete current;
			length--;
			current=first;
		}
	}
	listnode<valuetype> *prev=first;
	current=current->getNext();
	while (current) {
		if (!this->getComparator()->compare(
					current->getValue(),value)) {
			retval=true;
			listnode<valuetype> *temp=current->getNext();
			prev->setNext(temp);
			if (last==current) {
				last=prev;
			}
			node_delete_value(&(current->getValue()),
						this->getManageValues(),
						this->getManageArrayValues());
			delete current;
			length--;
			current=temp;
		} else {
			prev=current;
			current=current->getNext();
		}
	}
	return retval;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::remove(listnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	listnode<valuetype> *current=first;
	if (current==node) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		listnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (current==node) {
				prev->setNext(current->getNext());
				break;
			}
			prev=current;
			current=current->getNext();
		}
		if (last==current) {
			last=prev;
		}
	}
	if (current) {
		node_delete_value(&(current->getValue()),
					this->getManageValues(),
					this->getManageArrayValues());
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
uint64_t singlylinkedlist<valuetype>::getLength() const {
	return length;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlist<valuetype>::getFirst() const {
	return first;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlist<valuetype>::getLast() const {
	return last;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlist<valuetype>::getNext(
					listnode<valuetype> *node) const {
	return (node)?node->getNext():NULL;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlist<valuetype>::find(valuetype value) const {
	return find(first,value);
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlist<valuetype>::find(
				listnode<valuetype> *startnode,
				valuetype value) const {
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
void singlylinkedlist<valuetype>::insertionSort() {

	// insertion sort with a few optimization...

	// if there are 0 or 1 items in the list then it's already sorted
	if (length<2) {
		return;
	}

	// first and last pointers for the new list
	listnode<valuetype>	*newfirst=NULL;
	listnode<valuetype>	*newlast=NULL;

	// pointers for iterating through the new list
	listnode<valuetype> *current=NULL;
	listnode<valuetype> *previous=NULL;

	// iterate through the current list, building a new one as we go
	listnode<valuetype>	*node=first;
	listnode<valuetype>	*next=NULL;
	while (node) {

		// get the next node so we can move on later
		next=node->getNext();

		// if the new list is empty
		if (!newfirst) {
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else

		// if the node belongs at the beginning of the new list
		// (optimization for lists that are already largely forwards)
		if (this->getComparator()->compare(newfirst->getValue(),
							node->getValue())>0) {
			node->setNext(newfirst);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (this->getComparator()->compare(newlast->getValue(),
							node->getValue())<=0) {
			node->setNext(NULL);
			newlast->setNext(node);
			newlast=node;
		} else

		// if the node belongs somewhere in the middle of the new list
		{
			// search from the left...
			current=newfirst->getNext();
			previous=newfirst;
			while (current) {

				// if the current node is greater than...
				if (this->getComparator()->compare(
							current->getValue(),
							node->getValue())>0) {

					// insert before
					node->setNext(current);
					previous->setNext(node);
					break;
				}

				// move on
				previous=current;
				current=current->getNext();
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
void singlylinkedlist<valuetype>::heapSort() {

	// if there are 0 or 1 items in the list then it's already sorted
	if (length<2) {
		return;
	}

	// build heap as a binary tree mapped to an array:
	//	parentindex = floor((childindex-1)/2)
	//	leftchildindex = parent*2+1
	//	rightchildindex = parent*2+2
	listnode<valuetype>	**heap=new listnode<valuetype> *[length];
	listnode<valuetype>	*temp=NULL;
	uint64_t		heapend=0;
	for (listnode<valuetype> *node=first;
					node; node=node->getNext()) {

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
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else {
			node->setNext(newfirst);
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
void singlylinkedlist<valuetype>::clear() {
	listnode<valuetype>	*next;
	listnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		node_delete_value(&(current->getValue()),
					this->getManageValues(),
					this->getManageArrayValues());
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype>::singlylinkedlistnode(valuetype value) :
	listnode<valuetype>(),
	value(value),
	next(NULL) {
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype>::~singlylinkedlistnode() {
}

template <class valuetype>
inline
void singlylinkedlistnode<valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class valuetype>
inline
valuetype singlylinkedlistnode<valuetype>::getValue() const {
	return value;
}

template <class valuetype>
inline
valuetype &singlylinkedlistnode<valuetype>::getValue() {
	return value;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlistnode<valuetype>::getPrevious() const {
	return next;
}

template <class valuetype>
inline
listnode<valuetype> *singlylinkedlistnode<valuetype>::getNext() const {
	return next;
}

template <class valuetype>
inline
void singlylinkedlistnode<valuetype>::setNext(listnode<valuetype> *next) {
	this->next=next;
}

template <class valuetype>
inline
void singlylinkedlistnode<valuetype>::setPrevious(listnode<valuetype> *next) {
}
