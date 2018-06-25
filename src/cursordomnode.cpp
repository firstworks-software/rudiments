// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#include <rudiments/cursordomnode.h>

class cursordomnodeprivate {
	friend class cursordomnode;
	private:
};

cursordomnode::cursordomnode(dom *dom, domnode *nullnode) :
						domnode(dom,nullnode) {
	pvt=new cursordomnodeprivate;
}

cursordomnode::cursordomnode(dom *dom, domnode *nullnode, const char *ns) :
							domnode(dom,nullnode) {
	pvt=new cursordomnodeprivate;
	setNamespace(ns);
}

cursordomnode::~cursordomnode() {
	delete pvt;
}

domnodetype cursordomnode::getType() const {
	return NULL_DOMNODETYPE;
}

const char *cursordomnode::getName() const {
	return NULL;
}

const char *cursordomnode::getValue() const {
	return NULL;
}

domnode *cursordomnode::getParent() const {
	return getNullNode();
}

uint64_t cursordomnode::getPosition() const {
	return 0;
}

domnode *cursordomnode::getPreviousSibling() const {
	return getNullNode();
}

domnode *cursordomnode::getNextSibling() const {
	return getNullNode();
}

uint64_t cursordomnode::getChildCount() const {
	return 0;
}

domnode *cursordomnode::getFirstChild() const  {
	return getNullNode();
}

uint64_t cursordomnode::getAttributeCount() const {
	return 0;
}

domnode *cursordomnode::getAttribute(const char *name) const {
	return getNullNode();
}

domnode *cursordomnode::getAttributeIgnoringCase(const char *name) const {
	return getNullNode();
}

domnode *cursordomnode::getAttribute(uint64_t position) const {
	return getNullNode();
}

const char *cursordomnode::getAttributeValue(const char *name) const {
	return NULL;
}

const char *cursordomnode::getAttributeValue(uint64_t position) const {
	return NULL;
}

bool cursordomnode::isNullNode() const {
	return true;
}

void cursordomnode::setType(domnodetype type) {
	// do nothing
}

void cursordomnode::setName(const char *name) {
	// do nothing
}

void cursordomnode::setValue(const char *value) {
	// do nothing
}

domnode *cursordomnode::clone() {
	return getNullNode();
}

domnode *cursordomnode::clone(dom *dom) {
	return (dom!=getTree())?domnode::clone():getNullNode();
}

bool cursordomnode::insertNode(domnode *node,
					uint64_t position,
					domnodetype type,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return false;
}

bool cursordomnode::deleteNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return false;
}

domnode *cursordomnode::unlinkNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return NULL;
}
