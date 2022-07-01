// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
avltree<valuetype>::avltree() :
	treecollection<valuetype>(),
	top(NULL),
	first(NULL),
	last(NULL),
	length(0) {
}

template <class valuetype>
inline
avltree<valuetype>::avltree(const avltree<valuetype> &a) :
					treecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
avltree<valuetype>::avltree(const treecollection<valuetype> &a) :
					treecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
avltree<valuetype>::avltree(const nodecollection<valuetype> &a) :
					treecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
avltree<valuetype> &avltree<valuetype>::operator=(
					const avltree<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
avltree<valuetype> &avltree<valuetype>::operator=(
					const treecollection<valuetype> &a) {
	if (this!=&a) {
		clear();
		nodecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
avltree<valuetype> &avltree<valuetype>::operator=(
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
void avltree<valuetype>::clone(const treecollection<valuetype> *tree) {
	top=NULL;
	first=NULL;
	last=NULL;
	length=tree->getLength();
		
	if (tree->getTop()) {

		// clone the tree
		top=cloneNode(tree->getTop());

		// update first
		for (first=top;
			first->getLeftChild();
			first=first->getLeftChild()) {}

		// update last
		for (last=top;
			last->getRightChild();
			last=last->getRightChild()) {}
	}
}

template <class valuetype>
inline
void avltree<valuetype>::clone(const nodecollection<valuetype> *coll) {
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;

	if (coll->getFirst()) {

		// clone the collection...
		for (nodecollectionnode<valuetype> *node=coll->getFirst();
						node; node=node->getNext()) {
			insert(node_duplicate_value(node->getValue(),
						this->getManageValues(),
						this->getManageArrayValues()));
		}
	
		// update first
		for (first=top;
			first->getLeftChild();
			first=first->getLeftChild()) {}

		// update last
		for (last=top;
			last->getRightChild();
			last=last->getRightChild()) {}
	}
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::cloneNode(
					treenode<valuetype> *node) {

	// create a new node
	avltreenode<valuetype>	*newnode=new avltreenode<valuetype>(NULL);

	// copy the value
	newnode->setValue(node_duplicate_value(node->getValue(),
						this->getManageValues(),
						this->getManageArrayValues()));

	// clone the left side
	if (node->getLeftChild()) {
		treenode<valuetype>	*newleft=
					cloneNode(node->getLeftChild());
		newleft->setParent(newnode);
		newnode->setLeftChild(newleft);
	}
	newnode->setLeftHeight(node->getLeftHeight());

	// clone the right side
	if (node->getRightChild()) {
		treenode<valuetype>	*newright=
					cloneNode(node->getRightChild());
		newright->setParent(newnode);
		newnode->setRightChild(newright);
	}
	newnode->setRightHeight(node->getRightHeight());

	return newnode;
}

template <class valuetype>
inline
avltree<valuetype>::~avltree() {
	clear();
}

template <class valuetype>
inline
void avltree<valuetype>::insert(valuetype value) {
	insert(new avltreenode<valuetype>(value));
}

template <class valuetype>
inline
void avltree<valuetype>::insert(treenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return;
	}

	if (top) {

		// insert the node, optionally replacing the top of the tree
		insert(top,node,&top);

		// update first
		for (first=top;
			first->getLeftChild();
			first=first->getLeftChild()) {}

		// update last
		for (last=top;
			last->getRightChild();
			last=last->getRightChild()) {}
	} else {

		// if there was no top node, then this is the
		// first node inserted into the entire tree
		top=node;
		first=node;
		last=node;
	}

	// increment length
	length++;
}

template <class valuetype>
inline
void avltree<valuetype>::insert(treenode<valuetype> *top,
				treenode<valuetype> *node,
				treenode<valuetype> **treetop) {

	// degenerate case
	if (!node) {
		return;
	}

	// find a location to insert the node (should always be a leaf node)
	treenode<valuetype>	*location=top;
	for (;;) {

		if (this->getComparator()->compare(node->getValue(),
						location->getValue())<=0) {

			if (location->getLeftChild()) {
				location=location->getLeftChild();
			} else {
				location->setLeftChild(node);
				break;
			}

		} else if (this->getComparator()->compare(
						node->getValue(),
						location->getValue())>0) {

			if (location->getRightChild()) {
				location=location->getRightChild();
			} else {
				location->setRightChild(node);
				break;
			}
		}
	}

	node->setParent(location);

	// update heights up the tree
	top->adjustParentHeights(node);

	// balance the tree
	node->getParent()->balance(treetop);
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::detach(treenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return NULL;
	}

	// update first
	if (first==node) {
		first=node->getNext();
	}

	// update last
	if (last==node) {
		last=node->getPrevious();
	}

	// detach the node
	node->detach(&top);

	// decrement length
	length--;

	return node;
}

template <class valuetype>
inline
bool avltree<valuetype>::remove(valuetype value) {
	treenode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

template <class valuetype>
inline
bool avltree<valuetype>::removeAll(valuetype value) {
	bool	removed=false;
	while (remove(value)) {
		removed=true;
	}
	return removed;
}

template <class valuetype>
inline
bool avltree<valuetype>::remove(treenode<valuetype> *node) {
	treenode<valuetype> *detachednode=detach(node);
	node_delete_value(&(detachednode->getValue()),
					this->getManageValues(),
					this->getManageArrayValues());
	delete detachednode;
	return true;
}

template <class valuetype>
inline
uint64_t avltree<valuetype>::getLength() const {
	return length;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::getTop() const {
	return top;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::getFirst() const {
	return first;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::getLast() const {
	return last;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::getPrevious(
					treenode<valuetype> *node) const {
	return (node)?node->getPrevious():NULL;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::getNext(
					treenode<valuetype> *node) const {
	return (node)?node->getNext():NULL;
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::find(valuetype value) const {
	return find((treenode<valuetype> *)top,value);
}

template <class valuetype>
inline
treenode<valuetype> *avltree<valuetype>::find(
					treenode<valuetype> *startnode,
					valuetype value) const {

	// descend the tree until we find the value or run off of the bottom
	treenode<valuetype> *current=startnode;
	while (current) {

		int32_t	result=this->getComparator()->compare(
						current->getValue(),value);

		if (result<0) {
			current=current->getRightChild();
		} else if (result==0) {
			break;
		} else if (result>0) {
			current=current->getLeftChild();
		}
	}

	return current;
}

template <class valuetype>
inline
void avltree<valuetype>::clear() {

	// start at the top
	treenode<valuetype>	*node=top;
	while (node) {

		// go left as far as possible
		while (node->getLeftChild()) {
			node=node->getLeftChild();
		}

		// go right as far as possible
		while (node->getRightChild()) {
			node=node->getRightChild();
		}

		// remove this node from its parent
		treenode<valuetype>	*p=node->getParent();
		if (p) {
			if (p->getLeftChild()==node) {
				p->setLeftChild(NULL);
			} else {
				p->setRightChild(NULL);
			}
		}

		// delete the value in the node
		node_delete_value(&(node->getValue()),
					this->getManageValues(),
					this->getManageArrayValues());

		// delete the node itself
		delete node;

		// continue with the parent...
		node=p;
	}
	
	// clear pointers and length
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

template <class valuetype>
inline
ssize_t avltree<valuetype>::write(output *out) const {
	uint16_t	indentlevel=0;
	return ((avltreenode<valuetype> *)top)->
			write(out,this,"top",&indentlevel);
}

template <class valuetype>
inline
avltreenode<valuetype>::avltreenode(valuetype value) :
	treenode<valuetype>(),
	value(value),
	parent(NULL),
	left(NULL),
	right(NULL),
	leftheight(0),
	rightheight(0) {
}

template <class valuetype>
inline
avltreenode<valuetype>::~avltreenode() {
}

template <class valuetype>
inline
void avltreenode<valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class valuetype>
inline
valuetype avltreenode<valuetype>::getValue() const {
	return value;
}

template <class valuetype>
inline
valuetype &avltreenode<valuetype>::getValue() {
	return value;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::getParent() const {
	return parent;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::getLeftChild() const {
	return left;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::getRightChild() const {
	return right;
}

template <class valuetype>
inline
uint8_t avltreenode<valuetype>::getLeftHeight() const {
	return leftheight;
}

template <class valuetype>
inline
uint8_t avltreenode<valuetype>::getRightHeight() const {
	return rightheight;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::getPrevious() const {

	// reverse in-order, depth-first traversal...

	if (left) {

		// if we have a left child then its rightmost descendent
		// contains the next lowest value...

		// go left
		treenode<valuetype>	*node=left;

		// go as far right as possible
		while (node->getRightChild()) {
			node=node->getRightChild();
		}
		return node;

	} else if (parent) {

		// if we're the right child of our parent,
		// then our parent contains the next lowest value
		//
		// Why the weird cast?  I really don't know.  gcc 2.95.2 on
		// some platforms (os 10.0, 10.1, and mklinux) require it, but
		// oddly don't in other cases of comparing left/right child to
		// this.
		if ((const treenode<valuetype> *)parent->getRightChild()==
					(const treenode<valuetype> *)this) {
			return parent;
		}

		// If we're the left child of our parent, then we have to
		// move up until we find an acestor that's the right child of
		// its parent.  That node contains the next lowest value.
		treenode<valuetype>	*node=parent;
		while (node) {
			if (!node->getParent()) {
				break;
			}
			if (node->getParent()->getRightChild()==node) {
				return node->getParent();
			}
			node=node->getParent();
		}
	}
	return NULL;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::getNext() const {

	// in-order, depth-first traversal...

	if (right) {

		// if we have a right child then its leftmost descendent
		// contains the next highest value...

		// go right
		treenode<valuetype>	*node=right;

		// go as far left as possible
		while (node->getLeftChild()) {
			node=node->getLeftChild();
		}
		return node;

	} else if (parent) {

		// if we're the left child of our parent,
		// then our parent contains the next highest value
		//
		// Why the weird cast?  I really don't know.  gcc 2.95.2 on
		// some platforms (os 10.0, 10.1, and mklinux) require it, but
		// oddly don't in other cases of comparing left/right child to
		// this.
		if ((const treenode<valuetype> *)parent->getLeftChild()==
					(const treenode<valuetype> *)this) {
			return parent;
		}

		// If we're the right child of our parent, then we have to
		// move up until we find an acestor that's the left child of
		// its parent.  That node contains the next highest value.
		treenode<valuetype>	*node=parent;
		while (node) {
			if (!node->getParent()) {
				break;
			}
			if (node->getParent()->getLeftChild()==node) {
				return node->getParent();
			}
			node=node->getParent();
		}
	}
	return NULL;
}

template <class valuetype>
inline
ssize_t avltreenode<valuetype>::write(output *out,
					const treecollection<valuetype> *tree,
					const char *name,
					uint16_t *indentlevel) const {
	ssize_t	retval=0;

	// print an xml-style representation of the node and its descendents
	for (uint16_t i=0; i<*indentlevel; i++) {
		retval+=out->write(' ');
	}
	retval+=out->printf("<%s value=\"",name);
	retval+=tree->writeDelegate(out,value);
	retval+=out->printf("\" lh=\"%d\" rh=\"%d\" bf=\"%d\"",
			leftheight,rightheight,leftheight-rightheight);
	if (!left && !right) {
		retval+=out->write("/>\n");
	} else {
		retval+=out->write(">\n");
		(*indentlevel)++;
		if (left) {
			retval+=((avltreenode<valuetype> *)left)->
					write(out,tree,"left ",indentlevel);
		}
		if (right) {
			retval+=((avltreenode<valuetype> *)right)->
					write(out,tree,"right",indentlevel);
		}
		(*indentlevel)--;
		for (uint16_t i=0; i<*indentlevel; i++) {
			retval+=out->write(' ');
		}
		retval+=out->printf("</%s>\n",name);
	}
	return retval;
}

template <class valuetype>
inline
void avltreenode<valuetype>::setParent(treenode<valuetype> *node) {
	parent=node;
}

template <class valuetype>
inline
void avltreenode<valuetype>::setLeftChild(treenode<valuetype> *node) {
	left=node;
}

template <class valuetype>
inline
void avltreenode<valuetype>::setRightChild(treenode<valuetype> *node) {
	right=node;
}

template <class valuetype>
inline
void avltreenode<valuetype>::setLeftHeight(uint8_t height) {
	leftheight=height;
}

template <class valuetype>
inline
void avltreenode<valuetype>::setRightHeight(uint8_t height) {
	rightheight=height;
}

template <class valuetype>
inline
void avltreenode<valuetype>::detach(treenode<valuetype> **treetop) {

	if (left && right) {

		// node with left and right children...

		// get this node's successor...
		//
		// (eg. if the tree contains values 5, 7, 10, 12, 15, and 18,
		// and this node is 10, then find the node with 12 in it)
		//
		// following the rules from our in-order, depth-first traversal
		// above, since we have a right child, we must go right one,
		// then go left as far as possible
		treenode<valuetype>	*successor=right;
		while (successor->getLeftChild()) {
			successor=successor->getLeftChild();
		}

		// if the successor was the immediate right child of this node
		// then we need to handle a few things differently later
		bool	successorwasimmediaterightchild=(right==successor);


		// swap this node with the successor...

		// get a copy of the successor
		avltreenode<valuetype>	temp(
				*((avltreenode<valuetype> *)successor));

		// re-parent the successor
		successor->setParent(parent);
		if (successor->getParent()) {
			if (successor->getParent()->getLeftChild()==this) {
				successor->getParent()->
						setLeftChild(successor);
			} else {
				successor->getParent()->
						setRightChild(successor);
			}
		} else {
			*treetop=successor;
		}

		// replace the successor's children
		// with those of this node
		successor->setLeftChild(left);
		if (successor->getLeftChild()) {
			successor->getLeftChild()->setParent(successor);
		}
		if (successorwasimmediaterightchild) {
			successor->setRightChild(this);
			successor->getRightChild()->setParent(successor);
		} else {
			successor->setRightChild(right);
			if (successor->getRightChild()) {
				successor->getRightChild()->
						setParent(successor);
			}

			// re-parent this node
			parent=temp.parent;
			if (parent->getLeftChild()==successor) {
				parent->setLeftChild(this);
			} else {
				parent->setRightChild(this);
			}
		}

		// replace the successor's heights
		// with those of this node
		successor->setLeftHeight(leftheight);
		successor->setRightHeight(rightheight);


		// replace this node's children
		// with those of the successor
		left=temp.left;
		if (left) {
			left->setParent(this);
		}
		right=temp.right;
		if (right) {
			right->setParent(this);
		}

		// replace this node's heights
		// with those of the successor
		leftheight=temp.getLeftHeight();
		rightheight=temp.getRightHeight();

		// fall through to the code below because now
		// the node should have one or zero children...
	}

	// node with one or zero children...

	// decide which child the node has
	// NOTE: If the node has no children then this will implicitly
	// set child=NULL (which is what we want in that case) because
	// right=NULL.
	treenode<valuetype>	*child=(left)?left:right;

	if (parent) {

		// disconnect this node from its children
		left=NULL;
		right=NULL;

		// connect the parent to the child
		// (or to NULL if the node has no children)
		// decrement the appropriate height of parent
		if (parent->getLeftChild()==this) {
			parent->setLeftChild(child);
			parent->setLeftHeight(parent->getLeftHeight()-1);
		} else {
			parent->setRightChild(child);
			parent->setRightHeight(parent->getRightHeight()-1);
		}

		// connect the child to the parent
		if (child) {
			child->setParent(parent);
		}

		// disconnect this node from its parent
		// (but keep track of the parent so we
		// can use it to balance)
		treenode<valuetype>	*p=parent;
		parent=NULL;

		// update heights up the tree
		adjustParentHeights(p);

		// balance the tree
		p->balance(treetop);

	} else {

		// disconnect this node's child from it
		if (child) {
			child->setParent(NULL);
		}

		// disconnect this node from its children
		left=NULL;
		right=NULL;

		// NOTE: If the node has no children, then this will
		// implicitly (re)set treetop=NULL, which is what
		// we want in that case.
		*treetop=child;
	}
}

template <class valuetype>
inline
void avltreenode<valuetype>::adjustParentHeights(treenode<valuetype> *node) {

	// move up the tree, starting with the parent of "node"...
	while (node->getParent()) {

		// calculate the new height of the parent
		uint8_t	height=((node->getLeftHeight()>
					node->getRightHeight())?
						node->getLeftHeight():
						node->getRightHeight())+1;

		// If "node" is the left child of the parent, then adjust the
		// parent's left height.  
		// If "node" is the right child of the parent, then adjust the
		// parent's right height.  
		// In either case, bail if the height is already the same as we
		// calculated.
		if (node->getParent()->getLeftChild()==node) {
			if (node->getParent()->getLeftHeight()==height) {
				return;
			}
			node->getParent()->setLeftHeight(height);
		} else {
			if (node->getParent()->getRightHeight()==height) {
				return;
			}
			node->getParent()->setRightHeight(height);
		}

		// move up
		node=node->getParent();
	}
}

template <class valuetype>
inline
void avltreenode<valuetype>::balance(treenode<valuetype> **treetop) {

	// AVL balance...

	// start balancing with the current node
	treenode<valuetype> *node=this;
	while (node) {

		// there's an imbalance if the left and right
		// tree heights differ by more than 1
		if ((node->getLeftHeight()>node->getRightHeight() &&
			node->getLeftHeight()-node->getRightHeight()>1) ||
			(node->getRightHeight()>node->getLeftHeight() &&
			node->getRightHeight()-node->getLeftHeight()>1)) {

			// apply the appropriate rotation to restore balance
			// and let the rotation method tell us whch node to
			// process next
			if (node->getRightHeight()>node->getLeftHeight()) {
				if (node->getRightChild()->getRightHeight()>
					node->getRightChild()->getLeftHeight()) {
					node=node->leftRotate(treetop);
				} else {
					node=node->rightLeftRotate(treetop);
				}
			} else if (node->getLeftHeight()>
						node->getRightHeight()) {
				if (node->getLeftChild()->getLeftHeight()>
						node->getLeftChild()->
							getRightHeight()) {
					node=node->rightRotate(treetop);
				} else {
					node=node->leftRightRotate(treetop);
				}
			}

		} else {

			// if there's no imbalance then the next node we need
			// to process is the parent of the current node
			node=node->getParent();
		}
	}
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::leftRotate(
					treenode<valuetype> **treetop) {

	/* one of these: (eg: insert order a,b,c)
 	 *
 	 *  \
 	 *   a
 	 *  / \            \
 	 *     b    ->      b
 	 *    / \         /   \
 	 *   *   c       a     c
 	 *      / \     / \   / \
 	 *                 *
 	 * needs left rotation */

	// get a, b, and "star"
	treenode<valuetype>	*a=this;
	treenode<valuetype>	*b=a->getRightChild();
	treenode<valuetype>	*star=b->getLeftChild();
	uint8_t			starheight=b->getLeftHeight();

	// move b
	treenode<valuetype>	*p=a->getParent();
	if (p) {
		if (p->getRightChild()==a) {
			p->setRightChild(b);
		} else {
			p->setLeftChild(b);
		}
	} else {
		*treetop=b;
	}
	b->setParent(a->getParent());
	b->setLeftChild(a);

	// move a
	a->setParent(b);
	a->setRightChild(star);
	a->setRightHeight(starheight);

	// move "star"
	if (star) {
		star->setParent(a);
	}

	// update heights up the tree
	adjustParentHeights(a);

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at a.
	return a;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::rightLeftRotate(
					treenode<valuetype> **treetop) {

	/* one of these: (eg: insert order a,c,b)
	 *
	 *  \         \
	 *   a         a
	 *  / \       / \          \
	 *     c  ->     b    ->    b
	 *    / \       / \        / \
	 *   b             c      a   c
	 *  / \           / \    / \ / \
	 *     *         *
	 *
	 * needs right-left rotation */

	// do the right part of the right-left rotation...

	// get a, c, b, and "star"
	treenode<valuetype>	*a=this;
	treenode<valuetype>	*c=a->getRightChild();
	treenode<valuetype>	*b=c->getLeftChild();
	treenode<valuetype>	*star=b->getRightChild();
	uint8_t			starheight=b->getRightHeight();

	// move b
	a->setRightChild(b);
	b->setParent(a);
	b->setRightChild(c);

	// move c
	c->setParent(b);
	c->setLeftChild(star);
	c->setLeftHeight(starheight);

	// move "star"
	if (star) {
		star->setParent(c);
	}

	// update heights up the tree
	adjustParentHeights(c);

	// do the left part of the right-left rotation
	leftRotate(treetop);

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at c.
	return c;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::rightRotate(
					treenode<valuetype> **treetop) {

	/* one of these: (insert order c,b,a)
	 *
	 *     \
	 *      c
	 *     / \       \
	 *    b    ->     b
	 *   / \        /   \
	 *  a   *      a     c 
	 * / \        / \   / \
	 *                 *
	 * needs right rotation */

	// get c, b, and "star"
	treenode<valuetype>	*c=this;
	treenode<valuetype>	*b=c->getLeftChild();
	treenode<valuetype>	*star=b->getRightChild();
	uint8_t			starheight=b->getRightHeight();

	// move b
	treenode<valuetype>	*p=c->getParent();
	if (p) {
		if (p->getRightChild()==c) {
			p->setRightChild(b);
		} else {
			p->setLeftChild(b);
		}
	} else {
		*treetop=b;
	}
	b->setParent(c->getParent());
	b->setRightChild(c);

	// move c
	c->setParent(b);
	c->setLeftChild(star);
	c->setLeftHeight(starheight);

	// move "star"
	if (star) {
		star->setParent(c);
	}

	// update heights up the tree
	adjustParentHeights(c);

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at c.
	return c;
}

template <class valuetype>
inline
treenode<valuetype> *avltreenode<valuetype>::leftRightRotate(
					treenode<valuetype> **treetop) {

	/* one of these: (insert order c,a,b)
	 *
	 *   \           \
	 *    c           c
	 *   / \         / \      \
	 *  a     ->    b    ->    b
	 * / \         / \        / \
	 *    b       a          a   c
	 *   / \     / \        /     \
	 *  *           *
	 *
	 * needs left-right rotation */

	// do the left part of the left-right rotation...

	// get c, a, b, and "star"
	treenode<valuetype>	*c=this;
	treenode<valuetype>	*a=c->getLeftChild();
	treenode<valuetype>	*b=a->getRightChild();
	treenode<valuetype>	*star=b->getLeftChild();
	uint8_t			starheight=b->getLeftHeight();

	// move b
	c->setLeftChild(b);
	b->setParent(c);
	b->setLeftChild(a);

	// move a
	a->setParent(b);
	a->setRightChild(star);
	a->setRightHeight(starheight);

	// move "star"
	if (star) {
		star->setParent(a);
	}

	// update heights up the tree
	adjustParentHeights(a);

	// do the right part of the left-right rotation
	rightRotate(treetop);

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at a.
	return a;
}
