// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>

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

csvdom::csvdom(const csvdom &x) : csvsax(), dom() {
	init(stringCacheEnabled());
	// FIXME: call dom::dom(x)
}

csvdom &csvdom::operator=(const csvdom &x) {
	if (this!=&x) {
		reset();
		// FIXME: call dom::operator=
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
	return csvsax::parseFile(string);
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

bool csvdom::writeFile(const char *filename, mode_t perms) const {
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

	bool	retval=true;
	domnode	*root=getRootNode();
	domnode	*header=root->getFirstTagChild("header");
	bool	firstcolumn=true;
	for (domnode *column=header->getFirstTagChild("column");
			!column->isNullNode();
			column=column->getNextTagSibling("column")) {
		if (firstcolumn) {
			firstcolumn=false;
		} else {
			fl.write(getDelimiter());
		}
		writeValue(&fl,column);
	}
	fl.write("\n");
	for (domnode *row=root->getFirstTagChild("row");
			!row->isNullNode();
			row=row->getNextTagSibling("row")) {
		bool	firstrow=true;
		for (domnode *field=row->getFirstTagChild("field");
				!field->isNullNode();
				field=field->getNextTagSibling("field")) {
			if (firstrow) {
				firstrow=false;
			} else {
				fl.write(getDelimiter());
			}
			writeValue(&fl,field);
		}
		fl.write("\n");
	}
	fl.flushWriteBuffer(-1,-1);
	if (!fl.close()) {
		retval=false;
	}
	return retval;
}

void csvdom::writeValue(file *fl, domnode *value) const {
	fl->write(getQuote());
	fl->write(value->getAttributeValue("value"));
	fl->write(getQuote());
}

bool csvdom::headerStart() {
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*headernode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"header",NULL);
	pvt->_currentparent->appendChild(headernode);
	pvt->_currentparent=headernode;
	return true;
}

bool csvdom::column(const char *name) {
	domnode	*columnnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"column",NULL);
	columnnode->setAttributeValue("value",name);
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
						NULL,"row",NULL);
	pvt->_currentparent->appendChild(rownode);
	pvt->_currentparent=rownode;
	return true;
}

bool csvdom::field(const char *value) {
	domnode	*fieldnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						NULL,"field",NULL);
	fieldnode->setAttributeValue("value",value);
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
