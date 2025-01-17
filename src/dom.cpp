// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/dom.h>
#include <rudiments/charstring.h>
#include <rudiments/dictionary.h>
#include <rudiments/filesystem.h>
#include <rudiments/file.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>

class domprivate {
	friend class dom;
	private:
		domnode		*_nullnode;
		domnode		*_rootnode;

		bool				_stringcacheenabled;
		dictionary< char *, uint64_t >	_strcache;
};

dom::dom() : collection() {
	construct(true);
}

dom::dom(bool stringcacheenabled) : collection() {
	construct(stringcacheenabled);
}

dom::dom(dom &x) : collection(x) {
	construct(x.pvt->_stringcacheenabled);
	pvt->_rootnode=x.pvt->_rootnode->clone(this);
}

dom &dom::operator=(dom &x) {
	if (this!=&x) {
		reset();
		pvt->_rootnode=x.pvt->_rootnode->clone(this);
	}
	return *this;
}

void dom::construct(bool stringcacheenabled) {
	pvt=new domprivate;
	pvt->_nullnode=domnode::createNullNode(this);
	pvt->_rootnode=pvt->_nullnode;
	pvt->_stringcacheenabled=stringcacheenabled;
	pvt->_strcache.setManageArrayKeys(true);
}

dom::~dom() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
	}
	delete pvt->_nullnode;
	pvt->_strcache.clear();
	delete pvt;
}

uint64_t dom::getCount() {
	return 0;
}

bool dom::clear() {
	// FIXME: this is only here to satisfy the pure virtual method defined
	// in the collection base class.  Arguably it ought to actually do
	// something - probably the same as reset(), but it needs to be safe to
	// call at any point, which I'm not sure reset() is.
	return true;
}

bool dom::reset() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
		pvt->_rootnode=pvt->_nullnode;
	}
	pvt->_strcache.clear();
	return true;
}

const char *dom::getType() {
	return "dom";
}

void dom::createRootNode() {
	pvt->_rootnode=new domnode(this,ROOT_DOMNODETYPE,NULL,"document",NULL);
}

domnode *dom::getRootNode() {
	return (pvt->_rootnode)?pvt->_rootnode:pvt->_nullnode;
}

domnode *dom::getNullNode() {
	return pvt->_nullnode;
}

domnode *dom::getChildByPath(const char *path) {
	return getRootNode()->getChildByPath(path);
}

domnode *dom::getAttributeByPath(const char *path, uint64_t position) {
	return getRootNode()->getAttributeByPath(path,position);
}

domnode *dom::getAttributeByPath(const char *path, const char *name) {
	return getRootNode()->getAttributeByPath(path,name);
}

const char *dom::getAttributeValueByPath(const char *path, uint64_t position) {
	return getRootNode()->getAttributeValueByPath(path,position);
}

const char *dom::getAttributeValueByPath(const char *path,const char *name) {
	return getRootNode()->getAttributeValueByPath(path,name);
}

ssize_t dom::writeFile(const char *filename, mode_t perms) {
	return writeFile(filename,perms,true);
}

ssize_t dom::writeFile(const char *filename, mode_t perms, bool indent) {

	// open the file
	file	fl;
	if (!fl.open(filename,O_RDWR|O_CREAT|O_TRUNC,perms)) {
		return RESULT_ERROR;
	}
	fl.setWriteBufferSize(
		filesystem::getOptimumTransferBlockSize(filename));

	// write the file, saving any error that may occur
	ssize_t	retval=write(&fl,indent);
	int32_t	err=0;
	if (retval<0) {
		err=error::getErrorNumber();
	}

	// flush the write buffer
	fl.flushWriteBuffer(-1,-1);

	// since flushWriteBuffer() might also (silently) fail and set errno,
	// we need to restore whatever error we saved earlier because that's
	// the error that we care about
	if (retval<0) {
		error::setErrorNumber(err);
	}

	// close (allowing this to overwrite the earlier error)
	if (!fl.close()) {
		retval=RESULT_ERROR;
	}

	// return the result
	return retval;
}

ssize_t dom::write() {
	return write(true);
}

ssize_t dom::write(bool indent) {
	return write(&stdoutput,indent);
}

ssize_t dom::write(output *out) {
	return write(out,true);
}

ssize_t dom::write(output *out, bool indent) {
	// by calling writeNode(), we get the version of the method which may
	// be overridden by a child class, and which writes node and its
	// children in the class' native format
	uint16_t	indentlevel=0;
	return writeNode(getRootNode(),out,indent,&indentlevel);
}

ssize_t dom::writeJson() {
	return writeJson(&stdoutput,true);
}

ssize_t dom::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

ssize_t dom::writeJson(output *out) {
	return writeJson(out,true);
}

ssize_t dom::writeJson(output *out, bool indent) {
	// There's no standard way to express a generic dom as JSON, so we'll
	// just write a degenerate object here.  Child classes can provide
	// implementations as necessary.
	ssize_t	retval=0;
	incOrErr(&retval,out->write('{')) &&
	(indent)?incOrErr(&retval,out->write('\n')):true &&
	incOrErr(&retval,out->write('}')) &&
	(indent)?incOrErr(&retval,out->write('\n')):true;
	return retval;
}

ssize_t dom::writeXml() {
	return writeXml(&stdoutput,true);
}

ssize_t dom::writeXml(bool indent) {
	return writeXml(&stdoutput,indent);
}

ssize_t dom::writeXml(output *out) {
	return writeXml(out,true);
}

ssize_t dom::writeXml(output *out, bool indent) {
	// by calling dom::writeNode(), we call the method from the dom
	// class, which writes node and its children in the XML format
	uint16_t	indentlevel=0;
	return dom::writeNode(getRootNode(),out,indent,&indentlevel);
}

ssize_t dom::writeNode(domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) {

	ssize_t	retval=0;

	domnode		*current;
	domnodetype	currenttype=dn->getType();
	const char	*currentns;
	const char	*currentname;
	const char	*currentvalue;

	switch (currenttype) {
		case ROOT_DOMNODETYPE:
			current=dn->getFirstChild();
			while (!current->isNullNode()) {
				if (!incOrErr(&retval,
						dom::writeNode(current,out,
							indent,indentlevel))) {
					return retval;
				}
				current=current->getNextSibling();
			}
			break;
		case TAG_DOMNODETYPE:
			if (indent && indentlevel) {
				if (!incOrErr(&retval,
					writeIndent(out,*indentlevel))) {
					return retval;
				}
			}
			if (!incOrErr(&retval,out->write('<'),1)) {
				return retval;
			}
			currentns=dn->getNamespace();
			currentname=dn->getName();
			if (currentns) {
				if (!incOrErr(&retval,
						safeWrite(out,currentns)) ||
					!incOrErr(&retval,out->write(':'),1)) {
					return retval;
				}
			}
			if (!incOrErr(&retval,safeWrite(out,currentname))) {
				return retval;
			}
			current=dn->getAttribute((uint64_t)0);
			while (current && !current->isNullNode()) {
				if (!incOrErr(&retval,out->write(' '),1) ||
					!incOrErr(&retval,
						dom::writeNode(current,out,
							indent,indentlevel))) {
					return retval;
				}
				current=current->getNextSibling();
			}
			current=dn->getFirstChild();
			if (!current->isNullNode()) {
				if (!incOrErr(&retval,out->write('>'),1)) {
					return retval;
				}
				if (indent && indentlevel) {
					if (current->getType()!=
							TEXT_DOMNODETYPE &&
						current->getType()!=
							CDATA_DOMNODETYPE) {
						if (!incOrErr(&retval,
							out->write('\n'),1)) {
							return retval;
						}
					}
					*indentlevel=*indentlevel+2;
				}
				domnodetype	prevtype=current->getType();
				while (!current->isNullNode()) {
					if (!incOrErr(&retval,
						dom::writeNode(current,out,
							indent,indentlevel))) {
						return retval;
					}
					prevtype=current->getType();
					current=current->getNextSibling();
				}
				if (indent && indentlevel) {
					*indentlevel=*indentlevel-2;
					if (prevtype!=TEXT_DOMNODETYPE &&
						prevtype!=CDATA_DOMNODETYPE) {
						if (!incOrErr(&retval,
							writeIndent(out,
							*indentlevel))) {
							return retval;
						}
					}
				}
				if (!incOrErr(&retval,out->write("</",2),2)) {
					return retval;
				}
				if (currentns) {
					if (!incOrErr(&retval,
						safeWrite(out,currentns))) {
						return retval;
					}
					if (!incOrErr(&retval,
							out->write(':'),1)) {
						return retval;
					}
				}
				if (!incOrErr(&retval,
					safeWrite(out,currentname)) ||
						!incOrErr(&retval,
							out->write('>'),1)) {
					return retval;
				}
				if (indent && indentlevel) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
			} else {
				currentname=dn->getName();
				if (*currentname=='?') {
					if (!incOrErr(&retval,
							out->write("?>",2),2)) {
						return retval;
					}
				} else if (*currentname=='!') {
					if (!incOrErr(&retval,
							out->write('>'),1)) {
						return retval;
					}
				} else {
					if (!incOrErr(&retval,
							out->write("/>",2),2)) {
						return retval;
					}
				}
				if (indent && indentlevel) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
			}
			break;
		case TEXT_DOMNODETYPE:
			incOrErr(&retval,safeWrite(out,dn->getValue()));
			break;
		case ATTRIBUTE_DOMNODETYPE:
			currentvalue=dn->getValue();
			if (dn->getParent()->getName()[0]=='!') {
				incOrErr(&retval,out->write('"'),1) &&
				incOrErr(&retval,
					safeWrite(out,currentvalue)) &&
				incOrErr(&retval,out->write('"'),1);
			} else {
				incOrErr(&retval,
					safeWrite(out,dn->getName())) &&
				incOrErr(&retval,out->write("=\"",2),2) &&
				incOrErr(&retval,safeWrite(out,currentvalue)) &&
				incOrErr(&retval,out->write('"'),1);
			}
			break;
		case COMMENT_DOMNODETYPE:
			incOrErr(&retval,out->write("<!--",4),4) &&
			incOrErr(&retval,safeWrite(out,dn->getValue())) &&
			incOrErr(&retval,out->write("-->",3),3);
			break;
		case CDATA_DOMNODETYPE:
			incOrErr(&retval,out->write("<![CDATA[",9),9) &&
			incOrErr(&retval,safeWrite(out,dn->getValue())) &&
			incOrErr(&retval,out->write("]]>",3),3);
			break;
		default:
			break;
	}
	return retval;
}

ssize_t dom::writeIndent(output *out, uint16_t indent) {
	ssize_t	retval=0;
	for (uint16_t i=0; i<indent; i++) {
		if (!incOrErr(&retval,out->write(' '),1)) {
			break;
		}
	}
	return retval;
}

ssize_t dom::safeWrite(output *out, const char *str) {

	if (!str) {
		return 0;
	}

	ssize_t		retval=0;
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
			num=(uint16_t)((byte_t)*ch);
		}
		if (entity || num) {
			incOrErr(&retval,out->write(start,ch-start),ch-start);
			if (entity) {
				ssize_t	len=charstring::getLength(entity);
				incOrErr(&retval,out->write(entity,len),len);
				entity=NULL;
			} else {
				incOrErr(&retval,out->write("&#",2),2);
				char	*numstr=charstring::parseNumber(num);
				ssize_t	len=charstring::getLength(numstr);
				incOrErr(&retval,out->write(numstr,len),len);
				delete[] numstr;
				incOrErr(&retval,out->write(';'),1);
				num=0;
			}
			start=ch+1;
		}
	}
	incOrErr(&retval,out->write(start,ch-start),ch-start);
	return retval;
}

bool dom::getStringCacheEnabled() {
	return pvt->_stringcacheenabled;
}

const char *dom::cacheString(const char *string) {
	if (!string) {
		return NULL;
	}
	uint64_t	refcount;
	if (pvt->_strcache.getValue((char *)string,&refcount)) {
		pvt->_strcache.setValue((char *)string,refcount+1);
		return pvt->_strcache.getKey((char *)string);
	}
	char	*copy=charstring::duplicate(string);
	pvt->_strcache.setValue(copy,1);
	return copy;
}

void dom::unCacheString(const char *string) {
	if (!string) {
		return;
	}
	uint64_t	refcount;
	if (pvt->_strcache.getValue((char *)string,&refcount)) {
		refcount--;
		if (refcount) {
			pvt->_strcache.setValue((char *)string,refcount);
		} else {
			pvt->_strcache.remove((char *)string);
		}
	}
}
