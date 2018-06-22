// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/dom.h>
#include <rudiments/charstring.h>
#include <rudiments/dictionary.h>
#include <rudiments/filesystem.h>
#include <rudiments/file.h>
#include <rudiments/sys.h>

class domprivate {
	friend class dom;
	private:
		domnode		*_nullnode;
		domnode		*_rootnode;

		bool				_stringcacheenabled;
		dictionary< char *, uint64_t >	_strcache;
};

dom::dom() {
	init(true);
}

dom::dom(bool stringcacheenabled) {
	init(stringcacheenabled);
}

dom::dom(const dom &x) {
	init(x.pvt->_stringcacheenabled);
	pvt->_rootnode=x.pvt->_rootnode->clone(this);
}

dom &dom::operator=(const dom &x) {
	if (this!=&x) {
		reset();
		pvt->_rootnode=x.pvt->_rootnode->clone(this);
	}
	return *this;
}

void dom::init(bool stringcacheenabled) {
	pvt=new domprivate;
	pvt->_nullnode=domnode::createNullNode(this);
	pvt->_rootnode=pvt->_nullnode;
	pvt->_stringcacheenabled=stringcacheenabled;
}

dom::~dom() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
	}
	delete pvt->_nullnode;
	pvt->_strcache.clearAndArrayDeleteKeys();
	delete pvt;
}

void dom::reset() {
	if (!pvt->_rootnode->isNullNode()) {
		pvt->_rootnode->cascadeOnDelete();
		delete pvt->_rootnode;
		pvt->_rootnode=pvt->_nullnode;
	}
	pvt->_strcache.clearAndArrayDeleteKeys();
}

void dom::createRootNode() {
	pvt->_rootnode=new domnode(this,pvt->_nullnode,
					ROOT_DOMNODETYPE,
					NULL,"document",NULL);
}

domnode *dom::getRootNode() const {
	return (pvt->_rootnode)?pvt->_rootnode:pvt->_nullnode;
}

domnode *dom::getNullNode() const {
	return pvt->_nullnode;
}

bool dom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.open(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=sys::getPageSize();
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);
	bool	retval=write(&fl);
	fl.flushWriteBuffer(-1,-1);
	if (!fl.close()) {
		retval=false;
	}
	return retval;
}

bool dom::write(output *out) const {
	return write(out,false);
}

bool dom::write(output *out, bool indent) const {
	getRootNode()->write(out,indent);
	return true;
}

bool dom::stringCacheEnabled() {
	return pvt->_stringcacheenabled;
}

const char *dom::cacheString(const char *string) {
	if (!string) {
		return NULL;
	}
	dictionarynode< char *, uint64_t > 	*node=
				pvt->_strcache.getNode((char *)string);
	if (node) {
		node->setValue(node->getValue()+1);
		return node->getKey();
	}
	char	*copy=charstring::duplicate(string);
	pvt->_strcache.setValue(copy,1);
	return copy;
}

void dom::unCacheString(const char *string) {
	if (!string) {
		return;
	}
	dictionarynode< char *, uint64_t > 	*node=
				pvt->_strcache.getNode((char *)string);
	if (node) {
		node->setValue(node->getValue()-1);
		if (!node->getValue()) {
			char	*data=node->getKey();
			pvt->_strcache.remove(node);
			delete[] data;
		}
	}
}
