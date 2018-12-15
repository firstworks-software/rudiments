// Copyright (c) 1999-2018 David Muse
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

void dom::write(const domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) const {

	// NOTE: this method is written a little strangely
	// to work correctly with cursordomnodes

	domnode	*current;
	if (dn->getType()==ROOT_DOMNODETYPE) {
		current=dn->getFirstChild();
		while (!current->isNullNode()) {
			dom::write(current,out,indent,indentlevel);
			current=current->getNextSibling();
		}
	} else if (dn->getType()==TAG_DOMNODETYPE) {
		if (indent && indentlevel) {
			writeIndent(out,*indentlevel);
		}
		out->write("<");
		if (dn->getNamespace()) {
			safeWrite(out,dn->getNamespace());
			out->write(":");
		}
		safeWrite(out,dn->getName());
		current=dn->getAttribute((uint64_t)0);
		while (current && !current->isNullNode()) {
			out->write(" ");
			dom::write(current,out,indent,indentlevel);
			current=current->getNextSibling();
		}
		current=dn->getFirstChild();
		if (!current->isNullNode()) {
			out->write(">");
			if (indent && indentlevel) {
				if (current->getType()!=TEXT_DOMNODETYPE &&
					current->getType()!=CDATA_DOMNODETYPE) {
					out->write("\n");
				}
				*indentlevel=*indentlevel+2;
			}
			domnodetype	prevtype=current->getType();
			while (!current->isNullNode()) {
				dom::write(current,out,indent,indentlevel);
				prevtype=current->getType();
				current=current->getNextSibling();
			}
			if (indent && indentlevel) {
				*indentlevel=*indentlevel-2;
				if (prevtype!=TEXT_DOMNODETYPE &&
					prevtype!=CDATA_DOMNODETYPE) {
					writeIndent(out,*indentlevel);
				}
			}
			out->write("</");
			if (dn->getNamespace()) {
				safeWrite(out,dn->getNamespace());
				out->write(":");
			}
			safeWrite(out,dn->getName());
			out->write(">");
			if (indent && indentlevel) {
				out->write("\n");
			}
		} else {
			if (dn->getName()[0]=='?') {
				out->write("?>");
			} else if (dn->getName()[0]=='!') {
				out->write(">");
			} else {
				out->write("/>");
			}
			if (indent && indentlevel) {
				out->write("\n");
			}
		}
	} else if (dn->getType()==TEXT_DOMNODETYPE) {
		safeWrite(out,dn->getValue());
	} else if (dn->getType()==ATTRIBUTE_DOMNODETYPE) {
		if (dn->getParent()->getName()[0]=='!') {
			out->write("\"");
			safeWrite(out,dn->getValue());
			out->write("\"");
		} else {
			safeWrite(out,dn->getName());
			out->write("=\"");
			safeWrite(out,dn->getValue());
			out->write("\"");
		}
	} else if (dn->getType()==COMMENT_DOMNODETYPE) {
		out->write("<!--");
		safeWrite(out,dn->getValue());
		out->write("-->");
	} else if (dn->getType()==CDATA_DOMNODETYPE) {
		out->write("<![CDATA[");
		safeWrite(out,dn->getValue());
		out->write("]]>");
	}
}

void dom::writeIndent(output *out, uint16_t indent) const {
	for (uint16_t i=0; i<indent; i++) {
		out->write(" ");
	}
}

void dom::safeWrite(output *out, const char *str) const {

	if (!str) {
		return;
	}

	const char	*start=str;
	const char	*ch=start;
	const char	*entity=NULL;
	uint16_t	num=0;

	for (; *ch; ch++) {
		if (*ch=='&') {
			entity="&amp;";
		} else if (*ch=='<') {
			entity="&lt;";
		} else if (*ch=='>') {
			entity="&gt;";
		} else if (*ch=='\'') {
			entity="&apos;";
		} else if (*ch=='"') {
			entity="&quot;";
		} else if ((signed char)*ch<0) {
			num=static_cast<uint16_t>(
					static_cast<unsigned char>(*ch));
		}
		if (entity || num) {
			out->write(start,ch-start);
			if (entity) {
				out->write(entity);
				entity=NULL;
			} else {
				out->write("&#");
				char	*numstr=charstring::parseNumber(num);
				out->write(numstr);
				delete[] numstr;
				out->write(";");
				num=0;
			}
			start=ch+1;
		}
	}
	out->write(start,ch-start);
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
