// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
singlylinkedlist<valuetype>::singlylinkedlist() : listcollection<valuetype>() {
	first=NULL;
	last=NULL;
	length=0;
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
void singlylinkedlist<valuetype>::prepend(
				singlylinkedlistnode<valuetype> *node) {
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
void singlylinkedlist<valuetype>::append(
				singlylinkedlistnode<valuetype> *node) {
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
				singlylinkedlistnode<valuetype> *node,
				valuetype value) {
	insertAfter(node,new singlylinkedlistnode<valuetype>(value));
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::insertAfter(
				singlylinkedlistnode<valuetype> *node,
				singlylinkedlistnode<valuetype> *newnode) {
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
				singlylinkedlistnode<valuetype> *node,
				singlylinkedlistnode<valuetype> *nodetomove) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	if (nodetomove==first) {
		first=nodetomove->getNext();
	} else if (nodetomove==last) {
		singlylinkedlistnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		singlylinkedlistnode<valuetype> *previous=first;
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
void singlylinkedlist<valuetype>::detach(
				singlylinkedlistnode<valuetype> *node) {

	if (node==first && node==last) {
		first=NULL;
		last=NULL;
	} else if (node==first) {
		first=node->getNext();
	} else if (node==last) {
		singlylinkedlistnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		singlylinkedlistnode<valuetype> *previous=first;
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
	singlylinkedlistnode<valuetype> *current=first;
	if (!current->compare(value)) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (!current->compare(value)) {
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
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::removeAndDelete(valuetype value) {
	singlylinkedlistnode<valuetype> *current=first;
	if (!current->compare(value)) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (!current->compare(value)) {
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
		delete current->getValue();
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::removeAndArrayDelete(valuetype value) {
	singlylinkedlistnode<valuetype> *current=first;
	if (!current->compare(value)) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (!current->compare(value)) {
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
		delete[] current->getValue();
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
	singlylinkedlistnode<valuetype> *current=first;
	while (!current->compare(value)) {
		retval=true;
		if (first==last) {
			first=NULL;
			last=NULL;
			delete current;
			length--;
			return true;
		} else {
			first=first->getNext();
			delete current;
			length--;
			current=first;
		}
	}
	singlylinkedlistnode<valuetype> *prev=first;
	current=current->getNext();
	while (current) {
		if (!current->compare(value)) {
			retval=true;
			singlylinkedlistnode<valuetype> *temp=
							current->getNext();
			prev->setNext(temp);
			if (last==current) {
				last=prev;
			}
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
bool singlylinkedlist<valuetype>::removeAllAndDelete(valuetype value) {
	if (!first) {
		return true;
	}
	bool	retval=false;
	singlylinkedlistnode<valuetype> *current=first;
	while (!current->compare(value)) {
		retval=true;
		if (first==last) {
			first=NULL;
			last=NULL;
			delete current->getValue();
			delete current;
			length--;
			return true;
		} else {
			first=first->getNext();
			delete current->getValue();
			delete current;
			length--;
			current=first;
		}
	}
	singlylinkedlistnode<valuetype> *prev=first;
	current=current->getNext();
	while (current) {
		if (!current->compare(value)) {
			retval=true;
			singlylinkedlistnode<valuetype> *temp=
							current->getNext();
			prev->setNext(temp);
			if (last==current) {
				last=prev;
			}
			delete current->getValue();
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
bool singlylinkedlist<valuetype>::removeAllAndArrayDelete(valuetype value) {
	if (!first) {
		return true;
	}
	bool	retval=false;
	singlylinkedlistnode<valuetype> *current=first;
	while (!current->compare(value)) {
		retval=true;
		if (first==last) {
			first=NULL;
			last=NULL;
			delete[] current->getValue();
			delete current;
			length--;
			return true;
		} else {
			first=first->getNext();
			delete[] current->getValue();
			delete current;
			length--;
			current=first;
		}
	}
	singlylinkedlistnode<valuetype> *prev=first;
	current=current->getNext();
	while (current) {
		if (!current->compare(value)) {
			retval=true;
			singlylinkedlistnode<valuetype> *temp=
							current->getNext();
			prev->setNext(temp);
			if (last==current) {
				last=prev;
			}
			delete[] current->getValue();
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
bool singlylinkedlist<valuetype>::remove(
				singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	singlylinkedlistnode<valuetype> *current=first;
	if (current==node) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
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
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::removeAndDelete(
				singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	singlylinkedlistnode<valuetype> *current=first;
	if (current==node) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
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
		delete current->getValue();
		delete current;
		length--;
		return true;
	}
	return false;
}

template <class valuetype>
inline
bool singlylinkedlist<valuetype>::removeAndArrayDelete(
				singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	singlylinkedlistnode<valuetype> *current=first;
	if (current==node) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
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
		delete[] current->getValue();
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
singlylinkedlistnode<valuetype> *singlylinkedlist<valuetype>::getFirst() {
	return first;
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype> *singlylinkedlist<valuetype>::getLast() {
	return last;
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype> *singlylinkedlist<valuetype>::getNext(
					singlylinkedlistnode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype> *singlylinkedlist<valuetype>::
							find(valuetype value) {
	return find((singlylinkedlistnode<valuetype> *)first,value);
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype> *singlylinkedlist<valuetype>::find(
				singlylinkedlistnode<valuetype> *startnode,
				valuetype value) {
	for (singlylinkedlistnode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
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
	singlylinkedlistnode<valuetype>	*newfirst=NULL;
	singlylinkedlistnode<valuetype>	*newlast=NULL;

	// pointers for iterating through the new list
	singlylinkedlistnode<valuetype> *current=NULL;
	singlylinkedlistnode<valuetype> *previous=NULL;

	// iterate through the current list, building a new one as we go
	singlylinkedlistnode<valuetype>	*node=first;
	singlylinkedlistnode<valuetype>	*next=NULL;
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
		if (newfirst->compare(node)>0) {
			node->setNext(newfirst);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (newlast->compare(node)<=0) {
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
				if (current->compare(node)>0) {

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
	singlylinkedlistnode<valuetype>	**heap=
				new singlylinkedlistnode<valuetype> *[length];
	singlylinkedlistnode<valuetype>	*temp=NULL;
	uint64_t			heapend=0;
	for (singlylinkedlistnode<valuetype> *node=first;
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
			if (heap[parent]->compare(heap[child])<0) {
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
	singlylinkedlistnode<valuetype>	*newfirst=NULL;
	singlylinkedlistnode<valuetype>	*newlast=NULL;

	// extract values from the heap...
	for (;;) {

		// pull off the highest value (which is always at the root
		// of the tree, index 0 in the array) and prepend it to the
		// new array
		singlylinkedlistnode<valuetype>	*node=heap[0];
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
			if (heap[parent]->compare(heap[leftchild])<0) {
				greater=leftchild;
			}

			// is the right child greater?
			uint64_t	rightchild=leftchild+1;
			if (rightchild<=heapend &&
				heap[rightchild]->compare(heap[greater])>0) {
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

// NOTE: Don't collapse the clear methods into a single method, or the compiler
// will attempt to compile calls to:
// 	delete current->getValue();
// 	and
// 	delete[] current->getValue();
// even if the app just calls clear().  This will fail for primitive types or
// when the type has a private destructor.

template <class valuetype>
inline
void singlylinkedlist<valuetype>::clear() {
	singlylinkedlistnode<valuetype>	*next;
	singlylinkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::clearAndDelete() {
	singlylinkedlistnode<valuetype>	*next;
	singlylinkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		delete current->getValue();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::clearAndArrayDelete() {
	singlylinkedlistnode<valuetype>	*next;
	singlylinkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		delete[] current->getValue();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::print() const {
	print(length);
}

template <class valuetype>
inline
void singlylinkedlist<valuetype>::print(uint64_t count) const {
	uint64_t	i=0;
	for (singlylinkedlistnode<valuetype> *current=first;
			current && i<count; current=current->getNext()) {
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			stdoutput.printf("index %lld: ",(long long)i);
		#else
			stdoutput.printf("index %ld: ",(long)i);
		#endif
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

template <class valuetype>
inline
singlylinkedlistnode<valuetype>::singlylinkedlistnode(valuetype value) :
					nodecollectionnode<valuetype>() {
	this->value=value;
	next=NULL;
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
singlylinkedlistnode<valuetype> *singlylinkedlistnode<valuetype>::getNext() {
	return next;
}

template <class valuetype>
inline
int32_t singlylinkedlistnode<valuetype>::compare(valuetype value) const {
	return node_compare(this->value,value);
}

template <class valuetype>
inline
int32_t singlylinkedlistnode<valuetype>::compare(
				singlylinkedlistnode<valuetype> *peer) const {
	return node_compare(this->value,peer->value);
}

template <class valuetype>
inline
void singlylinkedlistnode<valuetype>::print() const {
	node_print(value);
}

template <class valuetype>
inline
void singlylinkedlistnode<valuetype>::setNext(
				singlylinkedlistnode<valuetype> *next) {
	this->next=next;
}
