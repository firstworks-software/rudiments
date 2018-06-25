// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#include <rudiments/livedomnode.h>

class livedomnodeprivate {
	friend class livedomnode;
	private:
};

livedomnode::livedomnode(dom *dom, domnode *nullnode) : domnode(dom,nullnode) {
	pvt=new livedomnodeprivate;
}

livedomnode::~livedomnode() {
	delete pvt;
}

domnodetype livedomnode::getType() const {
	return NULL_DOMNODETYPE;
}

const char *livedomnode::getNamespace() const {
	return NULL;
}

const char *livedomnode::getName() const {
	return NULL;
}

const char *livedomnode::getValue() const {
	return NULL;
}

domnode *livedomnode::getParent() const {
	return getNullNode();
}

uint64_t livedomnode::getPosition() const {
	return 0;
}

domnode *livedomnode::getPreviousSibling() const {
	return getNullNode();
}

domnode *livedomnode::getNextSibling() const {
	return getNullNode();
}

uint64_t livedomnode::getChildCount() const {
	return 0;
}

domnode *livedomnode::getFirstChild() const  {
	return getNullNode();
}

uint64_t livedomnode::getAttributeCount() const {
	return 0;
}

domnode *livedomnode::getAttribute(const char *name) const {
	return getNullNode();
}

domnode *livedomnode::getAttributeIgnoringCase(const char *name) const {
	return getNullNode();
}

domnode *livedomnode::getAttribute(uint64_t position) const {
	return getNullNode();
}

const char *livedomnode::getAttributeValue(const char *name) const {
	return NULL;
}

const char *livedomnode::getAttributeValue(uint64_t position) const {
	return NULL;
}

bool livedomnode::isNullNode() const {
	return true;
}

void livedomnode::setType(domnodetype type) {
	// do nothing
}

void livedomnode::setNamespace(const char *ns) {
	// do nothing
}

void livedomnode::setName(const char *name) {
	// do nothing
}

void livedomnode::setValue(const char *value) {
	// do nothing
}

void livedomnode::setParent(domnode *parent) {
	// do nothing
}

void livedomnode::setPreviousSibling(domnode *previous) {
	// do nothing
}

void livedomnode::setNextSibling(domnode *next) {
	// do nothing
}

domnode *livedomnode::clone() {
	return getNullNode();
}

domnode *livedomnode::clone(dom *dom) {
	return (dom!=getTree())?domnode::clone():getNullNode();
}

bool livedomnode::insertNode(domnode *node,
					uint64_t position,
					domnodetype type,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return false;
}

bool livedomnode::deleteNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return false;
}

domnode *livedomnode::unlinkNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count) {
	return NULL;
}
