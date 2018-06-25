// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/domnode.h>
#include <rudiments/charstring.h>
#include <rudiments/dom.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class domnodeprivate {
	friend class domnode;
	private:
		dom		*_dom;
		bool		_cascade;
		domnodetype	_type;
		const char	*_namespace;
		const char	*_name;
		const char	*_value;
		domnode		*_parent;
		domnode		*_next;
		domnode		*_nexttag;
		domnode		*_previous;
		uint64_t	_childcount;
		domnode		*_firstchild;
		domnode		*_firsttagchild;
		domnode		*_lastchild;
		uint64_t	_attributecount;
		domnode		*_firstattribute;
		domnode		*_lastattribute;
		domnode		*_nullnode;
		bool		_isnullnode;
		void		*_data;
		void		*_privatedata;
};

domnode::domnode(dom *dom, domnode *nullnode) {
	init(dom,nullnode);
	setType(NULL_DOMNODETYPE);
}

domnode::domnode(dom *dom, domnode *nullnode,
				domnodetype type,
				const char *name,
				const char *value) {
	init(dom,nullnode);
	setType(type);
	setName(name);
	setValue(value);
}

domnode::domnode(dom *dom, domnode *nullnode,
				domnodetype type,
				const char *ns,
				const char *name,
				const char *value) {
	init(dom,nullnode);
	setType(type);
	setNamespace(ns);
	setName(name);
	setValue(value);
}

void domnode::init(dom *dom, domnode *nullnode) {
	pvt=new domnodeprivate;
	pvt->_dom=dom;
	pvt->_nullnode=nullnode;
	pvt->_parent=nullnode;
	pvt->_next=nullnode;
	pvt->_nexttag=nullnode;
	pvt->_previous=nullnode;
	pvt->_firstchild=nullnode;
	pvt->_firsttagchild=nullnode;
	pvt->_lastchild=nullnode;
	pvt->_childcount=0;
	pvt->_firstattribute=NULL;
	pvt->_lastattribute=NULL;
	pvt->_attributecount=0;
	pvt->_cascade=true;
	pvt->_isnullnode=false;
	pvt->_namespace=NULL;
	pvt->_name=NULL;
	pvt->_value=NULL;
	pvt->_data=NULL;
	pvt->_privatedata=NULL;
}

domnode::~domnode() {
	domnode	*current;
	if (pvt->_cascade) {
		// delete child nodes
		current=pvt->_firstchild;
		while (current && !current->isNullNode()) {
			pvt->_lastchild=current->getNextSibling();
			delete current;
			current=pvt->_lastchild;
		}
	}
	// delete attributes
	current=pvt->_firstattribute;
	while (current && !current->isNullNode()) {
		pvt->_lastattribute=current->getNextSibling();
		delete current;
		current=pvt->_lastattribute;
	}
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_namespace);
		pvt->_dom->unCacheString(pvt->_name);
		pvt->_dom->unCacheString(pvt->_value);
	} else {
		delete[] (char *)pvt->_namespace;
		delete[] (char *)pvt->_name;
		delete[] (char *)pvt->_value;
	}
	delete pvt;
}

domnode *domnode::createNullNode(dom *dom) {
	domnode	*nn=new domnode(dom,NULL);
	nn->pvt->_nullnode=nn;
	nn->setParent(nn);
	nn->setNextSibling(nn);
	nn->pvt->_nexttag=nn;
	nn->setPreviousSibling(nn);
	nn->pvt->_firstchild=nn;
	nn->pvt->_firsttagchild=nn;
	nn->pvt->_lastchild=nn;
	nn->pvt->_isnullnode=true;
	return nn;
}

domnode *domnode::getPreviousTagSibling() const {
	domnode	*node=getPreviousSibling();
	while (!node->isNullNode() && node->getType()!=TAG_DOMNODETYPE) {
		node=node->getPreviousSibling();
	}
	return node;
}

domnode *domnode::getPreviousTagSibling(const char *name) const {
	return getPreviousTagSibling(NULL,name,false);
}

domnode *domnode::getPreviousTagSibling(const char *ns,
						const char *name) const {
	return getPreviousTagSibling(ns,name,false);
}

domnode *domnode::getPreviousTagSiblingIgnoringCase(
						const char *name) const {
	return getPreviousTagSibling(NULL,name,true);
}

domnode *domnode::getPreviousTagSiblingIgnoringCase(
						const char *ns,
						const char *name) const {
	return getPreviousTagSibling(ns,name,true);
}

domnode *domnode::getPreviousTagSibling(const char *ns,
					const char *name,
					bool ignorecase) const {
	for (domnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
			current=current->getPreviousTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			return current;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getPreviousTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(NULL,name,
					attributename,attributevalue,false);
}

domnode *domnode::getPreviousTagSibling(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(ns,name,
					attributename,attributevalue,false);
}

domnode *domnode::getPreviousTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(NULL,name,
					attributename,attributevalue,true);
}

domnode *domnode::getPreviousTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(ns,name,
					attributename,attributevalue,true);
}

domnode *domnode::getPreviousTagSibling(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (domnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
			current=current->getPreviousTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getNextTagSibling() const {
	if (getType()==TAG_DOMNODETYPE) {
		return pvt->_nexttag;
	}
	domnode	*node=getNextSibling();
	while (!node->isNullNode() && node->getType()!=TAG_DOMNODETYPE) {
		node=node->getNextSibling();
	}
	return node;
}

domnode *domnode::getNextTagSibling(const char *name) const {
	return getNextTagSibling(NULL,name,false);
}

domnode *domnode::getNextTagSibling(const char *ns,
					const char *name) const {
	return getNextTagSibling(ns,name,false);
}

domnode *domnode::getNextTagSiblingIgnoringCase(
					const char *name) const {
	return getNextTagSibling(NULL,name,true);
}

domnode *domnode::getNextTagSiblingIgnoringCase(
					const char *ns,
					const char *name) const {
	return getNextTagSibling(ns,name,true);
}

domnode *domnode::getNextTagSibling(const char *ns,
					const char *name,
					bool ignorecase) const {
	for (domnode *current=getNextTagSibling();
			current && !current->isNullNode();
			current=current->getNextTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			return current;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getNextTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(NULL,name,
				attributename,attributevalue,false);
}

domnode *domnode::getNextTagSibling(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(ns,name,
				attributename,attributevalue,false);
}

domnode *domnode::getNextTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(NULL,name,
				attributename,attributevalue,true);
}

domnode *domnode::getNextTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(ns,name,
				attributename,attributevalue,true);
}

domnode *domnode::getNextTagSibling(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (domnode *current=getNextTagSibling();
			current && !current->isNullNode();
			current=current->getNextTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getNextTagSiblingInSet(const char * const *set) const {
	return getNextTagSiblingInSet(NULL,set);
}

domnode *domnode::getNextTagSiblingInSet(const char *ns,
						const char * const *set) const {
	for (domnode *child=getNextTagSibling();
			!child->isNullNode(); 
			child=child->getNextTagSibling()) {
		if (match(child,ns,set)) {
			return child;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getFirstTagChild() const {
	return pvt->_firsttagchild;
}

domnode *domnode::getFirstTagChild(const char *name) const {
	return getFirstTagChild(NULL,name,false);
}

domnode *domnode::getFirstTagChild(const char *ns, const char *name) const {
	return getFirstTagChild(ns,name,false);
}

domnode *domnode::getFirstTagChildIgnoringCase(const char *name) const {
	return getFirstTagChild(NULL,name,true);
}

domnode *domnode::getFirstTagChildIgnoringCase(const char *ns,
						const char *name) const {
	return getFirstTagChild(ns,name,true);
}

domnode *domnode::getFirstTagChild(const char *ns,
					const char *name,
					bool ignorecase) const {
	domnode	*node=pvt->_firsttagchild;
	if (node->isNullNode() || match(node,ns,name,ignorecase)) {
		return node;
	}
	return node->getNextTagSibling(ns,name,ignorecase);
}

domnode *domnode::getFirstTagChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(NULL,name,attributename,attributevalue,false);
}

domnode *domnode::getFirstTagChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(ns,name,attributename,attributevalue,false);
}

domnode *domnode::getFirstTagChildIgnoringCase(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(NULL,name,attributename,attributevalue,true);
}

domnode *domnode::getFirstTagChildIgnoringCase(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(ns,name,attributename,attributevalue,true);
}

domnode *domnode::getFirstTagChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	domnode	*node=getFirstChild(ns,name,attributename,
						attributevalue,ignorecase);
	return (node->isNullNode() || node->getType()==TAG_DOMNODETYPE)?
		node:
		node->getNextTagSibling(ns,name,attributename,
					attributevalue,ignorecase);
}

domnode *domnode::getFirstTagChildInSet(const char * const *set) const {
	return getFirstTagChildInSet(NULL,set);
}

domnode *domnode::getFirstTagChildInSet(const char *ns,
						const char * const *set) const {
	domnode	*child=getFirstTagChild();
	if (match(child,ns,set)) {
		return child;
	}
	return child->getNextTagSiblingInSet(ns,set);
}

domnode *domnode::getFirstChild() const {
	return pvt->_firstchild;
}

domnode *domnode::getFirstChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(NULL,name,attributename,attributevalue,false);
}

domnode *domnode::getFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(ns,name,attributename,attributevalue,false);
}

domnode *domnode::getFirstChildIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(NULL,name,attributename,attributevalue,true);
}

domnode *domnode::getFirstChildIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(ns,name,attributename,attributevalue,true);
}

domnode *domnode::getFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (domnode *current=pvt->_firstchild;
			!current->isNullNode();
			current=current->getNextSibling()) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getFirstTagDescendent(const char *name) const {
	return getFirstTagDescendent(NULL,name,false);
}

domnode *domnode::getFirstTagDescendent(const char *ns,
						const char *name) const {
	return getFirstTagDescendent(ns,name,false);
}

domnode *domnode::getFirstTagDescendentIgnoringCase(
						const char *name) const {
	return getFirstTagDescendent(NULL,name,true);
}

domnode *domnode::getFirstTagDescendentIgnoringCase(
						const char *ns,
						const char *name) const {
	return getFirstTagDescendent(ns,name,true);
}

domnode *domnode::getFirstTagDescendent(const char *ns,
						const char *name,
						bool ignorecase) const {
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,name,ignorecase)) {
			return child;
		}

		domnode	*desc=
			child->getFirstTagDescendent(ns,name,ignorecase);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getFirstTagDescendent(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(NULL,name,
					attributename,attributevalue,false);
}

domnode *domnode::getFirstTagDescendent(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(ns,name,
					attributename,attributevalue,false);
}

domnode *domnode::getFirstTagDescendentIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(NULL,name,
					attributename,attributevalue,true);
}

domnode *domnode::getFirstTagDescendentIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(ns,name,
					attributename,attributevalue,true);
}

domnode *domnode::getFirstTagDescendent(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {

	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,name,ignorecase) &&
			((ignorecase)?
			!charstring::compareIgnoringCase(
					child->getAttributeValue(attributename),
					attributevalue):
			!charstring::compare(
					child->getAttributeValue(attributename),
					attributevalue))) {
			return child;
		}

		domnode	*desc=child->getFirstTagDescendent(ns,name,
						attributename,attributevalue,
						ignorecase);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getFirstTagDescendentInSet(
					const char * const *set) const {
	return getFirstTagDescendentInSet(NULL,set);
}

domnode *domnode::getFirstTagDescendentInSet(
					const char *ns,
					const char * const *set) const {
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,set)) {
			return child;
		}

		domnode	*desc=child->getFirstTagDescendentInSet(ns,set);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

domnode *domnode::getNextTag() const {
	return getNextTag(NULL);
}

domnode *domnode::getNextTag(domnode *top) const {

	domnode	*ftc=getFirstTagChild();
	if (!ftc->isNullNode()) {
		return ftc;
	}
	domnode	*nts=getNextTagSibling();
	if (!nts->isNullNode()) {
		return nts;
	}
	const domnode	*ancestor=this;
	for (;;) {
		ancestor=ancestor->getParent();
		if (ancestor->isNullNode() || ancestor==top) {
			return pvt->_nullnode;
		}
		domnode	*ancestorsib=ancestor->getNextTagSibling();
		if (!ancestorsib->isNullNode()) {
			return ancestorsib;
		}
	}
}

domnode *domnode::getPreviousTag() const {
	return getPreviousTag(NULL);
}

domnode *domnode::getPreviousTag(domnode *top) const {

	domnode	*nts=getPreviousTagSibling();
	if (!nts->isNullNode()) {
		return nts;
	}
	const domnode	*ancestor=this;
	for (;;) {
		ancestor=ancestor->getParent();
		if (ancestor->isNullNode() || ancestor==top) {
			return pvt->_nullnode;
		}
		domnode	*ancestorsib=ancestor->getPreviousTagSibling();
		if (!ancestorsib->isNullNode()) {
			return ancestorsib;
		}
	}
}

bool domnode::insertText(const char *value, uint64_t position) {
	domnode	*text=new domnode(pvt->_dom,pvt->_nullnode);
	text->setName("text");
	text->setValue(value);
	return insertNode(text,position,
				TEXT_DOMNODETYPE,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool domnode::insertAttribute(const char *name, const char *value,
							uint64_t position) {
	domnode	*attribute=new domnode(pvt->_dom,pvt->_nullnode);
	attribute->setName(name);
	attribute->setValue(value);
	return insertNode(attribute,position,
				ATTRIBUTE_DOMNODETYPE,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

void domnode::write(output *out, bool indent, uint16_t *indentlevel) const {

	// NOTE: this method is written a little strangely
	// to work correctly with cursordomnodes

	domnode	*current;
	if (getType()==ROOT_DOMNODETYPE) {
		current=getFirstChild();
		while (!current->isNullNode()) {
			current->write(out,indent,indentlevel);
			current=current->getNextSibling();
		}
	} else if (getType()==TAG_DOMNODETYPE) {
		if (indent && indentlevel) {
			for (uint16_t i=0; i<*indentlevel; i++) {
				out->write(" ");
			}
		}
		out->write("<");
		if (getNamespace()) {
			safeWrite(out,getNamespace());
			out->write(":");
		}
		safeWrite(out,getName());
		current=getAttribute((uint64_t)0);
		while (current && !current->isNullNode()) {
			out->write(" ");
			current->write(out,indent,indentlevel);
			current=current->getNextSibling();
		}
		current=getFirstChild();
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
				current->write(out,indent,indentlevel);
				prevtype=current->getType();
				current=current->getNextSibling();
			}
			if (indent && indentlevel) {
				*indentlevel=*indentlevel-2;
				if (prevtype!=TEXT_DOMNODETYPE &&
					prevtype!=CDATA_DOMNODETYPE) {
					for (uint16_t i=0;
						i<*indentlevel; i++) {
						out->write(" ");
					}
				}
			}
			out->write("</");
			if (getNamespace()) {
				safeWrite(out,getNamespace());
				out->write(":");
			}
			safeWrite(out,getName());
			out->write(">");
			if (indent && indentlevel) {
				out->write("\n");
			}
		} else {
			if (getName()[0]=='?') {
				out->write("?>");
			} else if (getName()[0]=='!') {
				out->write(">");
			} else {
				out->write("/>");
			}
			if (indent && indentlevel) {
				out->write("\n");
			}
		}
	} else if (getType()==TEXT_DOMNODETYPE) {
		safeWrite(out,getValue());
	} else if (getType()==ATTRIBUTE_DOMNODETYPE) {
		if (getParent()->getName()[0]=='!') {
			out->write("\"");
			safeWrite(out,getValue());
			out->write("\"");
		} else {
			safeWrite(out,getName());
			out->write("=\"");
			safeWrite(out,getValue());
			out->write("\"");
		}
	} else if (getType()==COMMENT_DOMNODETYPE) {
		out->write("<!--");
		safeWrite(out,getValue());
		out->write("-->");
	} else if (getType()==CDATA_DOMNODETYPE) {
		out->write("<![CDATA[");
		safeWrite(out,getValue());
		out->write("]]>");
	}
}

void domnode::safeWrite(output *out, const char *str) const {

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

domnode *domnode::getNode(domnode *first,
				uint64_t position,
				const char *ns,
				const char *name,
				bool ignorecase,
				uint64_t count) const {
	if (!first || position>=count) {
		return pvt->_nullnode;
	}
	domnode	*current=first;
	if (name) {
		for (uint64_t i=0; i<count; i++) {
			if (match(current,ns,name,ignorecase)) {
				break;
			}
			current=current->getNextSibling();
		}
	} else {
		for (uint64_t i=0; i<position; i++) {
			current=current->getNextSibling();
		}
	}
	return current;
}

bool domnode::insertNode(domnode *node,
				uint64_t position,
				domnodetype type,
				domnode **first, domnode **last,
				uint64_t *count) {
	if (position>(*count)) {
		return false;
	}
	node->setParent(this);
	node->setType(type);
	domnode	*atpos=getNode(*first,position,NULL,NULL,false,*count);
	domnode	*beforepos=getNode(*first,position-1,NULL,NULL,false,*count);
	if (atpos) {
		node->setNextSibling(atpos);
		atpos->setPreviousSibling(node);
	}
	if (beforepos) {
		node->setPreviousSibling(beforepos);
		beforepos->setNextSibling(node);
	}
	if (position==0) {
		(*first)=node;
	}
	if (position==(*count)) {
		(*last)=node;
	}
	if (type==TAG_DOMNODETYPE) {
		domnode	*prevtag=node->getPreviousTagSibling();
		if (prevtag->isNullNode()) {
			pvt->_firsttagchild=node;
		} else {
			prevtag->pvt->_nexttag=node;
		}
		node->pvt->_nexttag=node->getNextSibling();
		while (!node->pvt->_nexttag->isNullNode() &&
			node->pvt->_nexttag->getType()!=TAG_DOMNODETYPE) {
			node->pvt->_nexttag=
				node->pvt->_nexttag->getNextSibling();
		}
	}
	(*count)++;
	return true;
}

bool domnode::deleteNode(domnode *node,
				uint64_t position, const char *name,
				domnode **first, domnode **last,
				uint64_t *count) {
	domnode	*current=unlinkNode(node,position,name,
						first,last,count);
	if (!current) {
		return false;
	}
	delete current;
	return true;
}

domnode *domnode::unlinkNode(domnode *node,
				uint64_t position, const char *name,
				domnode **first, domnode **last,
				uint64_t *count) {

	if (position>(*count)) {
		return NULL;
	}
	domnode	*current=*first;
	if (node || name) {
		while (current && !current->isNullNode() &&
			((name && charstring::compare(
					current->getName(),name)) ||
			(node && current!=node))) {
			current=current->getNextSibling();
		}
	} else {
		for (uint64_t i=0; i<position; i++) {
			current=current->getNextSibling();
		}
	}
	if (current && !current->isNullNode()) {
		if (current->getType()==TAG_DOMNODETYPE) {
			domnode	*prevtag=
					current->getPreviousTagSibling();
			if (!prevtag->isNullNode()) {
				prevtag->pvt->_nexttag=
					current->getNextTagSibling();
			}
			if (current->getParent()->
					pvt->_firsttagchild==current) {
				current->getParent()->
					pvt->_firsttagchild=
						current->getNextTagSibling();
			}
		}
		if (current->getPreviousSibling()) {
			current->getPreviousSibling()->
				setNextSibling(current->getNextSibling());
		}
		if (current->getNextSibling()) {
			current->getNextSibling()->setPreviousSibling(
						current->getPreviousSibling());
		}
		if (current==*first) {
			*first=current->getNextSibling();
		}
		if (current==*last) {
			*last=current->getPreviousSibling();
		}
		(*count)--;
		return current;
	}
	return NULL;
}

bool domnode::moveChild(domnode *child,
				domnode *parent, uint64_t position) {
	if (!parent || parent->isNullNode()) {
		return false;
	}
	domnode *current=unlinkNode(child,0,NULL,
					&pvt->_firstchild,
					&pvt->_lastchild,
					&pvt->_childcount);
	if (!current) {
		return false;
	}
	return parent->insertChild(current,position);
}

bool domnode::appendChild(domnode *child) {
	return insertChild(child,getChildCount());
}

domnode *domnode::appendTag(const char *tag) {
	return insertTag(NULL,tag,getChildCount());
}

domnode *domnode::appendTag(const char *ns, const char *tag) {
	return insertTag(ns,tag,getChildCount());
}

bool domnode::appendText(const char *value) {
	return insertText(value,getChildCount());
}

bool domnode::appendAttribute(domnode *attribute) {
	return insertAttribute(attribute,getAttributeCount());
}

bool domnode::appendAttribute(const char *name, const char *value) {
	return insertAttribute(name,value,getAttributeCount());
}

constnamevaluepairs *domnode::getAttributes() const {

	if (pvt->_isnullnode) {
		return NULL;
	}

	constnamevaluepairs	*nvp=new constnamevaluepairs();
	for (uint64_t i=0; i<getAttributeCount(); i++) {
		nvp->setValue(getAttribute(i)->getName(),
				getAttribute(i)->getValue());
	}
	return nvp;
}

void domnode::setAttributeValue(const char *name, const char *value) {
	if (isNullNode()) {
		return;
	}
	domnode	*attr=getAttribute(name);
	if (!attr->isNullNode()) {
		attr->setValue(value);
	} else {
		appendAttribute(name,value);
	}
}

void domnode::setAttributeValue(const char *name, int64_t value) {
	char	*valuestr=charstring::parseNumber(value);
	setAttributeValue(name,valuestr);
	delete[] valuestr;
}

void domnode::setAttributeValue(const char *name, uint64_t value) {
	char	*valuestr=charstring::parseNumber(value);
	setAttributeValue(name,valuestr);
	delete[] valuestr;
}

void domnode::cascadeOnDelete() {
	pvt->_cascade=true;
}

void domnode::dontCascadeOnDelete() {
	pvt->_cascade=false;
}

domnodetype domnode::getType() const {
	return pvt->_type;
}

const char *domnode::getNamespace() const {
	return pvt->_namespace;
}

const char *domnode::getName() const {
	return pvt->_name;
}

const char *domnode::getValue() const {
	return pvt->_value;
}

dom *domnode::getTree() const {
	return pvt->_dom;
}

domnode *domnode::getParent() const {
	return pvt->_parent;
}

uint64_t domnode::getPosition() const {
	domnode	*current=getParent()->getChild((uint64_t)0);
	if (!current) {
		// shouldn't ever happen
		return 0;
	}
	uint64_t	count=getParent()->getChildCount();
	for (uint64_t i=0; i<count; i++) {
		if (this==current) {
			return i;
		}
		current=current->getNextSibling();
	}
	// shouldn't ever happen
	return 0;
}

domnode *domnode::getPreviousSibling() const {
	return pvt->_previous;
}

domnode *domnode::getNextSibling() const {
	return pvt->_next;
}

domnode *domnode::getNullNode() const {
	return pvt->_nullnode;
}

bool domnode::isNullNode() const {
	return pvt->_isnullnode;
}

uint64_t domnode::getChildCount() const {
	return pvt->_childcount;
}

domnode *domnode::getChild(uint64_t position) const {
	return getNode(pvt->_firstchild,position,
				NULL,NULL,false,getChildCount());
}

domnode *domnode::getFirstChild(const char *name) const {
	return getFirstChild(NULL,name,false);
}

domnode *domnode::getFirstChild(const char *ns,
					const char *name) const {
	return getFirstChild(ns,name,false);
}

domnode *domnode::getFirstChildIgnoringCase(const char *name) const {
	return getFirstChild(NULL,name,true);
}

domnode *domnode::getFirstChildIgnoringCase(const char *ns,
						const char *name) const {
	return getFirstChild(ns,name,true);
}

domnode *domnode::getFirstChild(const char *ns,
					const char *name,
					bool ignorecase) const {
	return getNode(pvt->_firstchild,0,ns,name,ignorecase,getChildCount());
}

uint64_t domnode::getAttributeCount() const {
	return pvt->_attributecount;
}

domnode *domnode::getAttribute(uint64_t position) const {
	return getNode(pvt->_firstattribute,position,
				NULL,NULL,false,getAttributeCount());
}

domnode *domnode::getAttribute(const char *name) const {
	return getAttribute(name,false);
}

domnode *domnode::getAttributeIgnoringCase(const char *name) const {
	return getAttribute(name,true);
}

domnode *domnode::getAttribute(const char *name, bool ignorecase) const {
	return getNode(pvt->_firstattribute,0,
				NULL,name,ignorecase,getAttributeCount());
}

const char *domnode::getAttributeValue(uint64_t position) const {
	return getAttribute(position)->getValue();
}

const char *domnode::getAttributeValue(const char *name) const {
	return getAttribute(name)->getValue();
}

void domnode::setType(domnodetype type) {
	pvt->_type=type;
}

void domnode::setNamespace(const char *ns) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_namespace);
		pvt->_namespace=pvt->_dom->cacheString(ns);
	} else {
		delete[] (char *)pvt->_namespace;
		pvt->_namespace=charstring::duplicate(ns);
	}
}

void domnode::setName(const char *name) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_name);
		pvt->_name=pvt->_dom->cacheString(name);
	} else {
		delete[] (char *)pvt->_name;
		pvt->_name=charstring::duplicate(name);
	}
}

void domnode::setName(const char *ns, const char *name) {
	setNamespace(ns);
	setName(name);
}

void domnode::setValue(const char *value) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_value);
		pvt->_value=pvt->_dom->cacheString(value);
	} else {
		delete[] (char *)pvt->_value;
		pvt->_value=charstring::duplicate(value);
	}
}

void domnode::setParent(domnode *parent) {
	pvt->_parent=parent;
}

void domnode::setPreviousSibling(domnode *previous) {
	pvt->_previous=previous;
}

void domnode::setNextSibling(domnode *next) {
	pvt->_next=next;
}

bool domnode::insertChild(domnode *child, uint64_t position) {
	return insertNode(child,position,
				child->getType(),
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

domnode *domnode::insertTag(const char *tag,
					uint64_t position) {
	return insertTag(NULL,tag,position);
}

domnode *domnode::insertTag(const char *ns,
					const char *tag,
					uint64_t position) {
	domnode	*child=new domnode(pvt->_dom,
					pvt->_nullnode,
					TAG_DOMNODETYPE,
					ns,tag,NULL);
	if (insertNode(child,position,
				TAG_DOMNODETYPE,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount)) {
		return child;
	}
	delete child;
	return pvt->_nullnode;
}

bool domnode::insertAttribute(domnode *attribute, uint64_t position) {
	return insertNode(attribute,position,
				ATTRIBUTE_DOMNODETYPE,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool domnode::deleteChild(uint64_t position) {
	return deleteNode(NULL,position,NULL,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool domnode::deleteChild(domnode *child) {
	return deleteNode(child,0,NULL,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool domnode::deleteFirstChild(const char *name) {
	return deleteFirstChild(NULL,name,false);
}

bool domnode::deleteFirstChild(const char *ns, const char *name) {
	return deleteFirstChild(ns,name,false);
}

bool domnode::deleteFirstChildIgnoringCase(const char *name) {
	return deleteFirstChild(NULL,name,true);
}

bool domnode::deleteFirstChildIgnoringCase(const char *ns,
						const char *name) {
	return deleteFirstChild(ns,name,true);
}

bool domnode::deleteFirstChild(const char *ns,
				const char *name, bool ignorecase) {
	return deleteChild(getFirstChild(ns,name,ignorecase));
}

bool domnode::deleteFirstChild(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChild(name,attributename,attributevalue));
}

bool domnode::deleteFirstChild(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChild(ns,name,attributename,attributevalue));
}

bool domnode::deleteFirstChildIgnoringCase(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChildIgnoringCase(
				name,attributename,attributevalue));
}

bool domnode::deleteFirstChildIgnoringCase(
				const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChildIgnoringCase(
				ns,name,attributename,attributevalue));
}

bool domnode::deleteChildren() {
	while (!pvt->_lastchild->isNullNode()) {
		if (!deleteChild(pvt->_lastchild)) {
			return false;
		}
	}
	return true;
}

bool domnode::deleteChildren(const char *name) {
	return deleteChildren(NULL,name,false);
}

bool domnode::deleteChildren(const char *ns, const char *name) {
	return deleteChildren(ns,name,false);
}

bool domnode::deleteChildrenIgnoringCase(const char *name) {
	return deleteChildren(NULL,name,true);
}

bool domnode::deleteChildrenIgnoringCase(const char *ns,
						const char *name) {
	return deleteChildren(ns,name,true);
}

bool domnode::deleteChildren(const char *ns,
				const char *name,
				bool ignorecase) {
	bool	foundone=false;
	while (deleteFirstChild(ns,name,ignorecase)) {
		foundone=true;
	}
	return foundone;
}

bool domnode::deleteChildren(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChildren(NULL,name,attributename,attributevalue,false);
}

bool domnode::deleteChildren(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChildren(ns,name,attributename,attributevalue,false);
}


bool domnode::deleteChildrenIgnoringCase(const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteChildren(NULL,name,attributename,attributevalue,true);
}

bool domnode::deleteChildrenIgnoringCase(const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteChildren(ns,name,attributename,attributevalue,true);
}

bool domnode::deleteChildren(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue,
				bool ignorecase) {
	bool	foundone=false;
	for (;;) {
		domnode	*child=getFirstTagChild(ns,name,
						attributename,attributevalue,
						ignorecase);
		if (!child->isNullNode()) {
			if (deleteChild(child)) {
				foundone=true;
			} else {
				return false;
			}
		} else {
			return foundone;
		}
	}
}

bool domnode::deleteDescendents(const char *name) {
	return deleteDescendents(NULL,name,false);
}

bool domnode::deleteDescendents(const char *ns,
					const char *name) {
	return deleteDescendents(ns,name,false);
}

bool domnode::deleteDescendentsIgnoringCase(const char *name) {
	return deleteDescendents(NULL,name,true);
}

bool domnode::deleteDescendentsIgnoringCase(const char *ns,
						const char *name) {
	return deleteDescendents(ns,name,true);
}

bool domnode::deleteDescendents(const char *ns,
					const char *name,
					bool ignorecase) {

	// delete immediate children
	bool	foundone=deleteChildren(ns,name,ignorecase);

	// descend
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->deleteDescendents(ns,name,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool domnode::deleteDescendents(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteDescendents(NULL,name,attributename,attributevalue,false);
}

bool domnode::deleteDescendents(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteDescendents(ns,name,attributename,attributevalue,false);
}

bool domnode::deleteDescendentsIgnoringCase(const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteDescendents(NULL,name,attributename,attributevalue,true);
}

bool domnode::deleteDescendentsIgnoringCase(const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteDescendents(ns,name,attributename,attributevalue,true);
}

bool domnode::deleteDescendents(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) {

	// delete immediate children
	bool	foundone=deleteChildren(ns,name,
					attributename,attributevalue,
					ignorecase);

	// descend
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->deleteDescendents(ns,name,
						attributename,attributevalue,
						ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool domnode::renameFirstChild(const char *oldname, const char *newname) {
	return renameFirstChild(NULL,oldname,NULL,newname,false);
}

bool domnode::renameFirstChild(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname) {
	return renameFirstChild(oldns,oldname,newns,newname,false);
}

bool domnode::renameFirstChildIgnoringCase(const char *oldname,
						const char *newname) {
	return renameFirstChild(NULL,oldname,NULL,newname,true);
}

bool domnode::renameFirstChildIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameFirstChild(oldns,oldname,newns,newname,true);
}

bool domnode::renameFirstChild(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {
	domnode	*child=getFirstChild(oldns,oldname,ignorecase);
	if (!child->isNullNode()) {
		child->setNamespace(newns);
		child->setName(newname);
		return true;
	}
	return false;
}

bool domnode::renameChildren(const char *oldname,
						const char *newname) {
	return renameChildren(NULL,oldname,NULL,newname,false);
}

bool domnode::renameChildren(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname) {
	return renameChildren(oldns,oldname,newns,newname,false);
}

bool domnode::renameChildrenIgnoringCase(const char *oldname,
						const char *newname) {
	return renameChildren(NULL,oldname,NULL,newname,true);
}

bool domnode::renameChildrenIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameChildren(oldns,oldname,newns,newname,true);
}

bool domnode::renameChildren(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {

	// don't do anything if the ns/names are the same
	if ((ignorecase)?
		(!charstring::compareIgnoringCase(oldns,newns) &&
		!charstring::compareIgnoringCase(oldname,newname)):
		(!charstring::compare(oldns,newns) &&
		!charstring::compare(oldname,newname))) {
		return true;
	}

	// rename children
	bool	foundone=false;
	for (domnode *child=getChild((uint64_t)0);
			!child->isNullNode();
			child=child->getNextSibling()) {
		if (match(child,oldns,oldname,ignorecase)) {
			child->setNamespace(newns);
			child->setName(newname);
			foundone=true;
		}
	}
	return foundone;
}

bool domnode::renameDescendents(const char *oldname,
						const char *newname) {
	return renameDescendents(NULL,oldname,NULL,newname,false);
}

bool domnode::renameDescendents(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameDescendents(oldns,oldname,newns,newname,false);
}

bool domnode::renameDescendentsIgnoringCase(const char *oldname,
						const char *newname) {
	return renameDescendents(NULL,oldname,NULL,newname,true);
}

bool domnode::renameDescendentsIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameDescendents(oldns,oldname,newns,newname,true);
}

bool domnode::renameDescendents(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {
	// rename immediate children
	bool	foundone=renameChildren(oldns,oldname,newns,newname,ignorecase);

	// descend
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->renameDescendents(oldns,oldname,
						newns,newname,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool domnode::deleteAttribute(uint64_t position) {
	return deleteNode(NULL,position,NULL,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool domnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool domnode::deleteAttribute(domnode *attribute) {
	return deleteNode(attribute,0,NULL,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

void domnode::write(output *out) const {
	write(out,false);
}

void domnode::write(output *out, bool indent) const {
	uint16_t	indentlevel=0;
	write(out,indent,(indent)?&indentlevel:NULL);
}

stringbuffer *domnode::getPath() const {

	// Path: /element[index]/...

	// run up the tree, counting parent nodes
	int64_t			ancestors=0;
	const domnode	*node=this;
	while (!node->isNullNode() && node->getType()!=ROOT_DOMNODETYPE) {
		ancestors++;
		node=node->getParent();
	}

	// create pointers to the names of each parent node
	const char	**names=new const char *[ancestors];
	uint64_t	*indices=new uint64_t[ancestors];
	int64_t		index;
	node=this;
	for (index=ancestors-1; index>=0; index--) {

		// get the name
		names[index]=node->getName();

		// figure out which sibling this node is
		indices[index]=0;
		for (domnode *siblingnode=
				node->getPreviousTagSibling(names[index]);
			(!siblingnode->isNullNode() &&
				siblingnode->getType()!=ROOT_DOMNODETYPE);
			siblingnode=siblingnode->
				getPreviousTagSibling(names[index])) {
			indices[index]++;
		}
		
		node=node->getParent();
	}

	// run through the list of parent node names and indices,
	// append them all to the path
	stringbuffer	*path=new stringbuffer();
	for (index=0; index<ancestors; index++) {
		path->append('/')->append(names[index]);
		path->append('[')->append(indices[index])->append(']');
	}

	delete[] names;
	delete[] indices;

	return path;
}

domnode *domnode::getChildByPath(const char *path) const {

	// Path: /element[index]/...
	const domnode	*node=this;
	stringbuffer		name;
	stringbuffer		indexstring;
	stringbuffer		*buffer=&name;
	const char		*ptr=path;
	for (;;) {

		if (!*ptr || *ptr=='@') {
			break;
		} else if (*ptr=='/') {
			buffer=&name;
		} else if (*ptr=='[') {
			buffer=&indexstring;
		} else if (*ptr==']') {

			// get the first child
			node=node->getFirstChild(name.getString());

			// now skip until we get the specified index
			uint64_t	index=
				charstring::toUnsignedInteger(
						indexstring.getString());
			for (uint64_t i=0; i<index; i++) {
				node=node->getNextTagSibling(name.getString());
			}

			// reset the buffers
			name.clear();
			indexstring.clear();

		} else {
			buffer->append(*ptr);
		}

		ptr++;
	}

	return const_cast<domnode *>(node);
}

domnode *domnode::getAttributeByPath(const char *path,
						uint64_t position) const {
	return getChildByPath(path)->getAttribute(position);
}

domnode *domnode::getAttributeByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttribute(name);
}

const char *domnode::getAttributeValueByPath(const char *path,
						uint64_t position) const {
	return getChildByPath(path)->getAttributeValue(position);
}

const char *domnode::getAttributeValueByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttributeValue(name);
}

void domnode::setData(void *data) {
	pvt->_data=data;
}

void *domnode::getData() {
	return pvt->_data;
}

void domnode::setPrivateData(void *privatedata) {
	pvt->_privatedata=privatedata;
}

void *domnode::getPrivateData() {
	return pvt->_privatedata;
}

bool domnode::unwrapChild(domnode *child) {

	// get the position of the specified node
	uint64_t	position=child->getPosition();

	// deparent the children
	for (domnode *grandchild=child->getChild((uint64_t)0);
			!grandchild->isNullNode();
			grandchild=child->getChild((uint64_t)0)) {

		position++;
		if (!child->moveChild(grandchild,this,position)) {
			return false;
		}
	}

	// delete the specified child
	return deleteChild(child);
}

bool domnode::unwrapFirstChild(const char *name) {
	return unwrapFirstChild(NULL,name,false);
}

bool domnode::unwrapFirstChild(const char *ns, const char *name) {
	return unwrapFirstChild(ns,name,false);
}

bool domnode::unwrapFirstChildIgnoringCase(const char *name) {
	return unwrapFirstChild(NULL,name,true);
}

bool domnode::unwrapFirstChildIgnoringCase(const char *ns,
						const char *name) {
	return unwrapFirstChild(ns,name,true);
}

bool domnode::unwrapFirstChild(const char *ns,
					const char *name,
					bool ignorecase) {
	domnode	*child=getFirstTagChild(ns,name,ignorecase);
	if (child->isNullNode()) {
		return false;
	}
	return unwrapChild(child);
}

bool domnode::unwrapChildrenIgnoringCase(const char *name) {
	return unwrapChildren(NULL,name,false);
}

bool domnode::unwrapChildrenIgnoringCase(const char *ns,
						const char *name) {
	return unwrapChildren(ns,name,false);
}

bool domnode::unwrapChildren(const char *name) {
	return unwrapChildren(NULL,name,true);
}

bool domnode::unwrapChildren(const char *ns, const char *name) {
	return unwrapChildren(ns,name,true);
}

bool domnode::unwrapChildren(const char *ns,
				const char *name,
				bool ignorecase) {
	bool	foundone=false;
	while (unwrapFirstChild(ns,name,ignorecase)) {
		foundone=true;
	}
	return foundone;
}

bool domnode::unwrapDescendents(const char *name) {
	return unwrapDescendents(NULL,name,false);
}

bool domnode::unwrapDescendents(const char *ns, const char *name) {
	return unwrapDescendents(ns,name,false);
}

bool domnode::unwrapDescendentsIgnoringCase(const char *name) {
	return unwrapDescendents(NULL,name,true);
}

bool domnode::unwrapDescendentsIgnoringCase(const char *ns,
						const char *name) {
	return unwrapDescendents(ns,name,true);
}

bool domnode::unwrapDescendents(const char *ns,
					const char *name,
					bool ignorecase) {

	// unwrap immediate children
	bool	foundone=unwrapChildren(ns,name,ignorecase);

	// descend
	for (domnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->unwrapDescendents(ns,name,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

domnode *domnode::wrapChild(domnode *child, const char *name) {
	return wrapChildren(child,child,name);
}

domnode *domnode::wrapChild(domnode *child,
					const char *ns, const char *name) {
	return wrapChildren(child,child,ns,name);
}

domnode *domnode::wrapChildren(domnode *startchild,
						domnode *endchild,
						const char *name) {
	return wrapChildren(startchild,endchild,NULL,name);
}

domnode *domnode::wrapChildren(domnode *startchild,
						domnode *endchild,
						const char *ns,
						const char *name) {

	domnode	*newnode=insertTag(ns,name,startchild->getPosition());
	if (newnode->isNullNode()) {
		return pvt->_nullnode;
	}

	domnode	*currentchild=startchild;
	for (;;) {

		domnode	*nextchild=currentchild->getNextSibling();

		if (!moveChild(currentchild,newnode,newnode->getChildCount())) {
			return pvt->_nullnode;
		}

		if (currentchild==endchild) {
			break;
		}

		currentchild=nextchild;
	}
	return newnode;
}

domnode *domnode::wrapChildren(const char *name) {
	return wrapChildren(NULL,name);
}

domnode *domnode::wrapChildren(const char *ns, const char *name) {

	if (!getChildCount()) {
		return insertTag(ns,name,0);
	}

	return wrapChildren(getChild((uint64_t)0),
				getChild(getChildCount()-1),ns,name);
}

domnode *domnode::clone() {
	return clone(getTree());
}

domnode *domnode::clone(dom *dom) {

	// clone this node
	domnode	*clonednode=new domnode(dom,dom->getNullNode(),
						getType(),getNamespace(),
						getName(),getValue());

	// clone attributes
	for (uint64_t i=0; i<getAttributeCount(); i++) {
		clonednode->setAttributeValue(getAttribute(i)->getName(),
						getAttribute(i)->getValue());
	}

	// clone children
	for (domnode *child=pvt->_firstchild;
			!child->isNullNode(); child=child->getNextSibling()) {
		clonednode->appendChild(child->clone(dom));
	}

	return clonednode;
}

bool domnode::match(domnode *node,
				const char *ns,
				const char *name,
				bool ignorecase) const {

	const char	*nodens=node->getNamespace();
	const char	*nodename=node->getName();

	if (ns) {
		if (ignorecase) {
			if (charstring::compareIgnoringCase(nodens,ns)) {
				return false;
			}
		} else {
			if (charstring::compare(nodens,ns)) {
				return false;
			}
		}
	}

	if (name) {
		if (ignorecase) {
			if (charstring::compareIgnoringCase(nodename,name)) {
				return false;
			}
		} else {
			if (charstring::compare(nodename,name)) {
				return false;
			}
		}
	}
	return true;
}

bool domnode::match(domnode *node,
				const char *ns,
				const char * const *set) const {
	if (ns && charstring::compare(node->getNamespace(),ns)) {
		return false;
	}
	if (set && !charstring::inSet(node->getName(),set)) {
		return false;
	}
	return true;
}
