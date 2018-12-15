// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class csvdomprivate {
	friend class csvdom;
	private:
		domnode		*_currentparent;
};

csvdom::csvdom() : csvsax(), dom() {
	init(true);
}

csvdom::csvdom(bool stringcacheenabled) : csvsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

csvdom::csvdom(const csvdom &x) : csvsax(), dom(x) {
	init(stringCacheEnabled());
}

csvdom &csvdom::operator=(const csvdom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void csvdom::init(bool stringcacheenabled) {
	pvt=new csvdomprivate;
}

csvdom::~csvdom() {
	delete pvt;
}

bool csvdom::parseFile(const char *filename) {
	return csvsax::parseFile(filename);
}

bool csvdom::parseString(const char *string) {
	return csvsax::parseString(string);
}

void csvdom::reset() {
	pvt->_currentparent=NULL;
	dom::reset();
	csvsax::reset();
}

void csvdom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

bool csvdom::write(output *out, bool indent) const {

	domnode	*root=getRootNode();
#ifdef DEBUG_MESSAGES
	root->print(&stdoutput);
#endif
	domnode	*header=root->getFirstTagChild("h");
	bool	firstcolumn=true;
	for (domnode *column=header->getFirstTagChild("c");
			!column->isNullNode();
			column=column->getNextTagSibling("c")) {
		if (firstcolumn) {
			firstcolumn=false;
		} else {
			out->write(getDelimiter());
		}
		writeValue(out,column);
	}
	out->write("\n");
	for (domnode *row=root->getFirstTagChild("r");
			!row->isNullNode();
			row=row->getNextTagSibling("r")) {
		bool	firstrow=true;
		for (domnode *field=row->getFirstTagChild("f");
				!field->isNullNode();
				field=field->getNextTagSibling("f")) {
			if (firstrow) {
				firstrow=false;
			} else {
				out->write(getDelimiter());
			}
			writeValue(out,field);
		}
		out->write("\n");
	}
	return true;
}

void csvdom::writeValue(output *out, domnode *value) const {
	const char	*v=value->getAttributeValue("v");
	if (value->getAttributeValue("q")[0]=='y') {
		out->write(getQuote());
		for (const char *ptr=v; *ptr; ptr++) {
			if (*ptr==getQuote()) {
				out->write(*ptr);
			}
			out->write(*ptr);
		}
		out->write(getQuote());
	} else {
		out->write(v);
	}
}

bool csvdom::headerStart() {
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*headernode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"h",NULL);
	pvt->_currentparent->appendChild(headernode);
	pvt->_currentparent=headernode;
	return true;
}

bool csvdom::column(const char *name, bool quoted) {
	domnode	*columnnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"c",NULL);
	columnnode->setAttributeValue("v",name);
	columnnode->setAttributeValue("q",(quoted)?"y":"n");
	pvt->_currentparent->appendChild(columnnode);
	return true;
}

bool csvdom::headerEnd() {
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool csvdom::bodyStart() {
	return true;
}

bool csvdom::rowStart() {
	domnode	*rownode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"r",NULL);
	pvt->_currentparent->appendChild(rownode);
	pvt->_currentparent=rownode;
	return true;
}

bool csvdom::field(const char *value, bool quoted) {
	domnode	*fieldnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"f",NULL);
	fieldnode->setAttributeValue("v",value);
	fieldnode->setAttributeValue("q",(quoted)?"y":"n");
	pvt->_currentparent->appendChild(fieldnode);
	return true;
}

bool csvdom::rowEnd() {
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool csvdom::bodyEnd() {
	return true;
}
