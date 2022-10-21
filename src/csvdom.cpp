// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>
#include <rudiments/debugprint.h>

class csvdomprivate {
	friend class csvdom;
	private:
		domnode		*_currentparent;
};

csvdom::csvdom() : csvsax(), dom() {
	construct(true);
}

csvdom::csvdom(bool stringcacheenabled) : csvsax(), dom(stringcacheenabled) {
	construct(stringcacheenabled);
}

csvdom::csvdom(csvdom &x) : csvsax(), dom(x) {
	construct(stringCacheEnabled());
}

csvdom &csvdom::operator=(csvdom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void csvdom::construct(bool stringcacheenabled) {
	pvt=new csvdomprivate;
}

csvdom::~csvdom() {
	delete pvt;
}

const char *csvdom::getType() {
	return "csvdom";
}

bool csvdom::parse(input *in) {
	reset();
	return sax::parse(in);
}

bool csvdom::parseFile(const char *filename) {
	reset();
	return sax::parseFile(filename);
}

bool csvdom::parseString(const char *string) {
	reset();
	return sax::parseString(string);
}

bool csvdom::reset() {
	pvt->_currentparent=NULL;
	return dom::reset() && csvsax::reset();
}

void csvdom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

uint64_t csvdom::getColumnCount() {
	uint64_t	retval=0;
	for (domnode *c=getRootNode()->
				getFirstTagChild("h")->
				getFirstTagChild("c");
			!c->isNullNode(); c=c->getNextTagSibling("c")) {
		retval++;
	}
	return retval;
}

domnode *csvdom::getColumn(uint64_t position) {
	return getRootNode()->getFirstTagChild("h")->getChild(position);
}

domnode *csvdom::getColumn(const char *name) {
	for (domnode *c=getRootNode()->
				getFirstTagChild("h")->
				getFirstTagChild("c");
			!c->isNullNode(); c=c->getNextTagSibling("c")) {
		if (!charstring::compare(c->getAttributeValue("v"),name)) {
			return c;
		}
	}
	return getNullNode();
}

const char *csvdom::getColumnName(uint64_t position) {
	domnode *c=getColumn(position);
	return (!c->isNullNode())?c->getAttributeValue("v"):NULL;
}

bool csvdom::getColumnPosition(const char *name, uint64_t *position) {
	uint64_t	i=0;
	for (domnode *c=getRootNode()->
				getFirstTagChild("h")->
				getFirstTagChild("c");
			!c->isNullNode(); c=c->getNextTagSibling("c")) {
		if (!charstring::compare(c->getAttributeValue("v"),name)) {
			*position=i;
			return true;
		}
		i++;
	}
	return false;
}

bool csvdom::caseColumn(domnode *c, bool upper) {
	if (c->isNullNode()) {
		return false;
	}
	char	*v=charstring::duplicate(c->getAttributeValue("v"));
	if (upper) {
		charstring::upper(v);
	} else {
		charstring::lower(v);
	}
	c->setAttributeValue("v",v);
	delete[] v;
	return true;
}

bool csvdom::upperCaseColumn(uint64_t position) {
	return caseColumn(getColumn(position),true);
}

bool csvdom::upperCaseColumn(const char *name) {
	return caseColumn(getColumn(name),true);
}

bool csvdom::caseColumns(bool upper) {
	for (domnode *c=getRootNode()->
				getFirstTagChild("h")->
				getFirstTagChild("c");
			!c->isNullNode(); c=c->getNextTagSibling("c")) {
		if (!caseColumn(c,upper)) {
			return false;
		}
	}
	return true;
}

bool csvdom::upperCaseColumns() {
	return caseColumns(true);
}

bool csvdom::lowerCaseColumn(uint64_t position) {
	return caseColumn(getColumn(position),false);
}

bool csvdom::lowerCaseColumn(const char *name) {
	return caseColumn(getColumn(name),false);
}

bool csvdom::lowerCaseColumns() {
	return caseColumns(false);
}

bool csvdom::trimNode(domnode *node, bool left, bool right) {
	if (node->isNullNode()) {
		return false;
	}
	char	*v=charstring::duplicate(node->getAttributeValue("v"));
	if (left) {
		charstring::leftTrim(v);
	}
	if (right) {
		charstring::rightTrim(v);
	}
	node->setAttributeValue("v",v);
	delete[] v;
	return true;
}

bool csvdom::rightTrimColumn(uint64_t position) {
	return trimNode(getColumn(position),false,true);
}

bool csvdom::rightTrimColumn(const char *name) {
	return trimNode(getColumn(name),false,true);
}

bool csvdom::leftTrimColumn(uint64_t position) {
	return trimNode(getColumn(position),true,false);
}

bool csvdom::leftTrimColumn(const char *name) {
	return trimNode(getColumn(name),true,false);
}

bool csvdom::bothTrimColumn(uint64_t position) {
	return trimNode(getColumn(position),true,true);
}

bool csvdom::bothTrimColumn(const char *name) {
	return trimNode(getColumn(name),true,true);
}

bool csvdom::trimColumns(bool left, bool right) {
	for (domnode *c=getRootNode()->
				getFirstTagChild("h")->
				getFirstTagChild("c");
			!c->isNullNode(); c=c->getNextTagSibling("c")) {
		if (!trimNode(c,left,right)) {
			return false;
		}
	}
	return true;
}

bool csvdom::rightTrimColumns() {
	return trimColumns(false,true);
}

bool csvdom::leftTrimColumns() {
	return trimColumns(true,false);
}

bool csvdom::bothTrimColumns() {
	return trimColumns(true,true);
}

void csvdom::setValue(domnode *node, const char *name, bool quoted) {
	node->setAttributeValue("v",name);
	node->setAttributeValue("q",(quoted)?"y":"n");
}

bool csvdom::renameColumn(const char *oldname,
				uint64_t position, bool useposition,
				const char *name, bool quoted) {
	domnode *c=(useposition)?getColumn(position):getColumn(oldname);
	if (c->isNullNode()) {
		return false;
	}
	setValue(c,name,quoted);
	return true;
}

bool csvdom::renameColumn(uint64_t position, const char *name, bool quoted) {
	return renameColumn(NULL,position,true,name,quoted);
}

bool csvdom::renameColumn(const char *oldname,
				const char *newname, bool quoted) {
	return renameColumn(oldname,0,false,newname,quoted);
}

bool csvdom::renameColumn(const char * const *oldnames,
				const char *newname, bool quoted) {
	bool	retval=false;
	for (const char * const *on=oldnames; *on; on++) {
		if (renameColumn(*on,newname,quoted)) {
			retval=true;
		}
	}
	return retval;
}

bool csvdom::renameAllColumns(const char *oldname,
				const char *newname, bool quoted) {
	bool	retval=false;
	for (;;) {
		if (renameColumn(oldname,0,false,newname,quoted)) {
			retval=true;
		} else {
			break;
		}
	}
	return retval;
}

bool csvdom::renameAllColumns(const char * const *oldnames,
				const char *newname, bool quoted) {
	bool	retval=false;
	for (;;) {
		if (renameColumn(oldnames,newname,quoted)) {
			retval=true;
		} else {
			break;
		}
	}
	return retval;
}

bool csvdom::insertColumnAt(uint64_t position, const char *name, bool quoted) {

	// get the header
	domnode	*h=getRootNode()->getFirstTagChild("h");
	if (h->isNullNode()) {
		return false;
	}

	// insert the new column
	domnode	*c=new domnode(this,TAG_DOMNODETYPE,NULL,"c",NULL);
	setValue(c,name,quoted);
	if (!h->insertChild(c,position)) {
		return false;
	}

	// insert blank fields
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {
		domnode	*f=new domnode(this,TAG_DOMNODETYPE,NULL,"f",NULL);
		setValue(f,"",false);
		r->insertChild(f,position);
	}
	return true;
}

bool csvdom::insertColumnBefore(uint64_t position,
					const char *name, bool quoted) {
	return insertColumnAt(position,name,quoted);
}

bool csvdom::insertColumnBefore(const char *name,
					const char *newname, bool quoted) {
	uint64_t	pos;
	if (!getColumnPosition(name,&pos)) {
		return false;
	}
	return insertColumnBefore(pos,newname,quoted);
}

bool csvdom::insertColumnAfter(uint64_t position,
					const char *name, bool quoted) {
	return insertColumnAt(position+1,name,quoted);
}

bool csvdom::insertColumnAfter(const char *name,
					const char *newname, bool quoted) {
	uint64_t	pos;
	if (!getColumnPosition(name,&pos)) {
		return false;
	}
	return insertColumnAfter(pos,newname,quoted);
}

bool csvdom::moveColumn(uint64_t oldposition, uint64_t newposition) {

	// get the header
	domnode	*h=getRootNode()->getFirstTagChild("h");
	if (h->isNullNode()) {
		return false;
	}

	// get the column
	domnode	*c=getColumn(oldposition);
	if (c->isNullNode()) {
		return false;
	}

	// move the column
	if (!h->moveChild(c,h,newposition)) {
		return false;
	}

	// run through the rows
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {

		// move the field
		r->moveChild(r->getChild(oldposition),r,newposition);
	}
	return true;
}

bool csvdom::moveColumn(const char *name, uint64_t position) {
	uint64_t	oldposition;
	return (getColumnPosition(name,&oldposition) &&
				moveColumn(oldposition,position));
}

bool csvdom::deleteColumn(uint64_t position) {

	// get the header
	domnode	*h=getRootNode()->getFirstTagChild("h");
	if (h->isNullNode()) {
		return false;
	}

	// delete the column
	if (!h->deleteChild(position)) {
		return false;
	}

	// run through the rows
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {

		// delete the field
		if (!r->deleteChild(position)) {
			return false;
		}
	}
	return true;
}

bool csvdom::deleteColumn(const char *name) {
	uint64_t	position;
	return (getColumnPosition(name,&position) && deleteColumn(position));
}

bool csvdom::getColumnIsEmpty(uint64_t position) {
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {
		if (r->getChild(position)->getAttributeValue("v")[0]) {
			return false;
		}
	}
	return true;
}

bool csvdom::getColumnIsEmpty(const char *name) {
	uint64_t	position;
	return (getColumnPosition(name,&position) &&
				getColumnIsEmpty(position));
}

bool csvdom::deleteEmptyColumns() {
	uint64_t	i=0;
	uint64_t	count=getColumnCount();
	while (i<count) {
		if (getColumnIsEmpty(i)) {
			if (!deleteColumn(i)) {
				return false;
			}
			count--;
		} else {
			i++;
		}
	}
	return true;
}

uint64_t csvdom::getRowCount() {
	return getRootNode()->getChildCount()-1;
}

domnode *csvdom::getRow(uint64_t position) {
	return getRootNode()->getChild(position+1);
}

const char *csvdom::getField(uint64_t row, uint64_t column) {
	domnode	*r=getRootNode()->getChild(row+1);
	if (r->isNullNode()) {
		return NULL;
	}
	domnode	*f=r->getChild(column);
	return (f->isNullNode())?NULL:f->getAttributeValue("v");
}

const char *csvdom::getField(uint64_t row, const char *column) {
	uint64_t	pos;
	if (!getColumnPosition(column,&pos)) {
		return NULL;
	}
	return getField(row,pos);
}

bool csvdom::setField(uint64_t row, uint64_t column,
					const char *value, bool quoted) {
	domnode	*r=getRootNode()->getChild(row+1);
	if (r->isNullNode()) {
		return false;
	}
	setValue(r->getChild(column),value,quoted);
	return true;
}

bool csvdom::setField(uint64_t row, const char *column,
					const char *value, bool quoted) {
	uint64_t	pos;
	if (!getColumnPosition(column,&pos)) {
		return false;
	}
	return setField(row,pos,value,quoted);
}

bool csvdom::trimField(uint64_t row, uint64_t column, bool left, bool right) {
	domnode	*f=getRootNode()->getChild(row+1)->getChild(column);
	if (f->isNullNode()) {
		return false;
	}
	return trimNode(f,left,right);
}

bool csvdom::leftTrimField(uint64_t row, uint64_t column) {
	return trimField(row,column,true,false);
}

bool csvdom::leftTrimField(uint64_t row, const char *column) {
	uint64_t	pos;
	if (!getColumnPosition(column,&pos)) {
		return false;
	}
	return trimField(row,pos,true,false);
}

bool csvdom::rightTrimField(uint64_t row, uint64_t column) {
	return trimField(row,column,false,true);
}

bool csvdom::rightTrimField(uint64_t row, const char *column) {
	uint64_t	pos;
	if (!getColumnPosition(column,&pos)) {
		return false;
	}
	return trimField(row,pos,false,true);
}

bool csvdom::bothTrimField(uint64_t row, uint64_t column) {
	return trimField(row,column,true,true);
}

bool csvdom::bothTrimField(uint64_t row, const char *column) {
	uint64_t	pos;
	if (!getColumnPosition(column,&pos)) {
		return false;
	}
	return trimField(row,pos,true,true);
}

void csvdom::trimFields(bool left, bool right) {
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {
		for (domnode *f=r->getFirstTagChild("f");
				!f->isNullNode(); f=f->getNextTagSibling("f")) {
			trimNode(f,left,right);
		}
	}
}

void csvdom::leftTrimFields() {
	trimFields(true,false);
}

void csvdom::rightTrimFields() {
	trimFields(false,true);
}

void csvdom::bothTrimFields() {
	trimFields(true,true);
}

bool csvdom::insertRowAt(uint64_t position) {
	domnode	*rownode=new domnode(this,TAG_DOMNODETYPE,NULL,"r",NULL);
	uint64_t	count=getColumnCount();
	for (uint64_t i=0; i<count; i++) {
		domnode	*fieldnode=new domnode(
					this,TAG_DOMNODETYPE,NULL,"f",NULL);
		setValue(fieldnode,"",false);
		rownode->appendChild(fieldnode);
	}
	return getRootNode()->insertChild(rownode,position+1);
}

bool csvdom::insertRowBefore(uint64_t position) {
	return insertRowAt(position);
}

bool csvdom::insertRowAfter(uint64_t position) {
	return insertRowAt(position+1);
}

bool csvdom::moveRow(uint64_t oldposition, uint64_t newposition) {
	domnode	*r=getRootNode()->getChild(oldposition+1);
	if (r->isNullNode()) {
		return false;
	}
	return getRootNode()->moveChild(r,getRootNode(),newposition+1);
}

bool csvdom::deleteRow(uint64_t position) {
	return getRootNode()->deleteChild(position+1);
}

bool csvdom::getRowIsEmpty(uint64_t position) {
	domnode	*r=getRootNode()->getChild(position+1);
	if (r->isNullNode()) {
		return false;
	}
	for (domnode *f=r->getFirstTagChild("f");
			!f->isNullNode(); f=f->getNextTagSibling("f")) {
		if (f->getAttributeValue("v")[0]) {
			return false;
		}
	}
	return true;
}

bool csvdom::deleteEmptyRows() {
	uint64_t	i=0;
	uint64_t	count=getRowCount();
	while (i<count) {
		if (getRowIsEmpty(i)) {
			if (!deleteRow(i)) {
				return false;
			}
			count--;
		} else {
			i++;
		}
	}
	return true;
}

void csvdom::carryValueDown(uint64_t position) {
	const char	*prev="";
	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {
		domnode		*f=r->getChild(position);
		const char	*v=f->getAttributeValue("v");
		if (!v[0]) {
			f->setAttributeValue("v",prev);
		} else {
			prev=v;
		}
	}
}

void csvdom::carryValueDown(const char *name) {
	uint64_t	pos;
	if (getColumnPosition(name,&pos)) {
		carryValueDown(pos);
	}
}

void csvdom::carryAllValuesDown() {

	uint64_t	colcount=getColumnCount();
	const char **prev=new const char *[colcount];
	for (uint64_t i=0; i<colcount; i++) {
		prev[i]="";
	}

	for (domnode *r=getRootNode()->getFirstTagChild("r");
			!r->isNullNode(); r=r->getNextTagSibling("r")) {

		uint64_t	i=0;
		for (domnode *f=r->getFirstTagChild("f");
				!f->isNullNode(); f=f->getNextTagSibling("f")) {

			const char	*v=f->getAttributeValue("v");
			if (!v[0]) {
				f->setAttributeValue("v",prev[i]);
			} else {
				prev[i]=v;
			}

			i++;
		}
	}
}

ssize_t csvdom::writeNode(domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) {

	ssize_t	retval=0;
	domnode	*header=dn->getFirstTagChild("h");
	bool	firstcolumn=true;
	for (domnode *column=header->getFirstTagChild("c");
			!column->isNullNode();
			column=column->getNextTagSibling("c")) {
		if (firstcolumn) {
			firstcolumn=false;
		} else {
			if (!incOrErr(&retval,out->write(getDelimiter()),1)) {
				return retval;
			}
		}
		if (!incOrErr(&retval,writeValue(out,column))) {
			return retval;
		}
	}
	if (!incOrErr(&retval,out->write('\n'),1)) {
		return retval;
	}
	for (domnode *row=dn->getFirstTagChild("r");
			!row->isNullNode();
			row=row->getNextTagSibling("r")) {
		bool	firstrow=true;
		for (domnode *field=row->getFirstTagChild("f");
				!field->isNullNode();
				field=field->getNextTagSibling("f")) {
			if (firstrow) {
				firstrow=false;
			} else {
				if (!incOrErr(&retval,
					out->write(getDelimiter()),1)) {
					return retval;
				}
			}
			if (!incOrErr(&retval,writeValue(out,field))) {
				return retval;
			}
		}
		if (!incOrErr(&retval,out->write('\n'),1)) {
			return retval;
		}
	}
	return retval;
}

ssize_t csvdom::writeValue(output *out, domnode *value) {
	ssize_t		retval=0;
	const char	*v=value->getAttributeValue("v");
	if (value->getAttributeValue("q")[0]=='y') {
		if (!incOrErr(&retval,out->write(getQuote()),1)) {
			return retval;
		}
		for (const char *ptr=v; *ptr; ptr++) {
			if (*ptr==getQuote()) {
				if (!incOrErr(&retval,out->write(*ptr),1)) {
					return retval;
				}
			}
			if (!incOrErr(&retval,out->write(*ptr),1)) {
				return retval;
			}
		}
		incOrErr(&retval,out->write(getQuote()),1);
	} else {
		size_t	len=charstring::length(v);
		incOrErr(&retval,out->write(v,len),len);
	}
	return retval;
}

bool csvdom::headerStart() {
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*headernode=new domnode(this,TAG_DOMNODETYPE,NULL,"h",NULL);
	pvt->_currentparent->appendChild(headernode);
	pvt->_currentparent=headernode;
	return true;
}

bool csvdom::column(const char *name, bool quoted) {
	domnode	*columnnode=new domnode(this,TAG_DOMNODETYPE,NULL,"c",NULL);
	setValue(columnnode,name,quoted);
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
	domnode	*rownode=new domnode(this,TAG_DOMNODETYPE,NULL,"r",NULL);
	pvt->_currentparent->appendChild(rownode);
	pvt->_currentparent=rownode;
	return true;
}

bool csvdom::field(const char *value, bool quoted) {
	domnode	*fieldnode=new domnode(this,TAG_DOMNODETYPE,NULL,"f",NULL);
	setValue(fieldnode,value,quoted);
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
