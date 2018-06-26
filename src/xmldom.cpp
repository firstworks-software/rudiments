// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>

class xmldomprivate {
	friend class xmldom;
	private:
		domnode		*_currentparent;
		domnode		*_currentattribute;

		domnode		*_top;
		uint64_t	_topposition;
};

xmldom::xmldom() : xmlsax(), dom() {
	init(true);
}

xmldom::xmldom(bool stringcacheenabled) : xmlsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

xmldom::xmldom(const xmldom &x) : xmlsax(), dom(x) {
	init(stringCacheEnabled());
}

xmldom &xmldom::operator=(const xmldom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void xmldom::init(bool stringcacheenabled) {
	pvt=new xmldomprivate;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
}

xmldom::~xmldom() {
	delete pvt;
}

bool xmldom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool xmldom::parseFile(const char *filename,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return xmlsax::parseFile(filename);
}

bool xmldom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool xmldom::parseString(const char *string,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return xmlsax::parseString(string);
}

void xmldom::reset() {
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
	dom::reset();
	xmlsax::reset();
}

void xmldom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

bool xmldom::tagStart(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*tagnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						ns,name,NULL);
	insertChild(tagnode);
	pvt->_currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(const char *name) {
	pvt->_currentattribute=new domnode(this,getNullNode(),
						ATTRIBUTE_DOMNODETYPE,
						NULL,name,NULL);
	pvt->_currentparent->insertAttribute(pvt->_currentattribute,
				pvt->_currentparent->getAttributeCount());
	return true;
}

bool xmldom::attributeValue(const char *value) {
	if (!pvt->_currentattribute) {
		if (!attributeName(value)) {
			return false;
		}
	}
	pvt->_currentattribute->setValue(value);
	pvt->_currentattribute=NULL;
	return true;
}

bool xmldom::text(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,getNullNode(),
					TEXT_DOMNODETYPE,
					NULL,"text",string));
	return true;
}

bool xmldom::tagEnd(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool xmldom::comment(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,getNullNode(),
					COMMENT_DOMNODETYPE,
					NULL,"comment",string));
	return true;
}

bool xmldom::cdata(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,getNullNode(),
					CDATA_DOMNODETYPE,
					NULL,"cdata",string));
	return true;
}

void xmldom::insertChild(domnode *child) {
	uint64_t	pos=pvt->_currentparent->getChildCount();
	if (pvt->_currentparent==pvt->_top) {
		pos=pvt->_topposition;
		pvt->_topposition++;
	}
	pvt->_currentparent->insertChild(child,pos);
}

void xmldom::write(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	// NOTE: this method is written a little strangely
	// to work correctly with cursordomnodes

	domnode	*current;
	if (dn->getType()==ROOT_DOMNODETYPE) {
		current=dn->getFirstChild();
		while (!current->isNullNode()) {
			write(current,out,indent,indentlevel);
			current=current->getNextSibling();
		}
	} else if (dn->getType()==TAG_DOMNODETYPE) {
		if (indent && indentlevel) {
			for (uint16_t i=0; i<*indentlevel; i++) {
				out->write(" ");
			}
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
			write(current,out,indent,indentlevel);
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
				write(current,out,indent,indentlevel);
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

void xmldom::safeWrite(output *out, const char *str) const {

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
