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
	init(true);
}

dom::dom(bool stringcacheenabled) : collection() {
	init(stringcacheenabled);
}

dom::dom(const dom &x) : collection(x) {
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

void dom::clear() {
	// FIXME: this is only here to satisfy the pure virtual method defined
	// in the collection base class.  Arguably it ought to actually do
	// something - probably the same as reset(), but it needs to be safe to
	// call at any point, which I'm not sure reset() is.
}

void dom::reset() {
	if (!pvt->_rootnode->isNullNode()) {
		pvt->_rootnode->cascadeOnDelete();
		delete pvt->_rootnode;
		pvt->_rootnode=pvt->_nullnode;
	}
	pvt->_strcache.clear();
}

const char *dom::getType() const {
	return "dom";
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

ssize_t dom::writeFile(const char *filename, mode_t perms) const {
	return writeFile(filename,perms,true);
}

ssize_t dom::writeFile(const char *filename, mode_t perms, bool indent) const {

	// get the optimum block size for I/O on this filesystem
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.open(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=sys::getPageSize();
	}

	// open the file
	file	fl;
	if (!fl.open(filename,O_RDWR|O_CREAT|O_TRUNC,perms)) {
		return RESULT_ERROR;
	}
	fl.setWriteBufferSize(optblocksize);

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

ssize_t dom::write() const {
	return write(true);
}

ssize_t dom::write(bool indent) const {
	return write(&stdoutput,indent);
}

ssize_t dom::write(output *out) const {
	return write(out,true);
}

ssize_t dom::write(output *out, bool indent) const {
	// by calling writeNode(), we get the version of the method which may
	// be overridden by a child class, and which writes node and its
	// children in the class' native format
	uint16_t	indentlevel=0;
	return writeNode(getRootNode(),out,indent,&indentlevel);
}

ssize_t dom::writeJson() const {
	return writeJson(&stdoutput,true);
}

ssize_t dom::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

ssize_t dom::writeJson(output *out) const {
	return writeJson(out,true);
}

ssize_t dom::writeJson(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

ssize_t dom::writeXml() const {
	return writeXml(&stdoutput,true);
}

ssize_t dom::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

ssize_t dom::writeXml(output *out) const {
	return writeXml(out,true);
}

ssize_t dom::writeXml(output *out, bool indent) const {
	// by calling dom::writeNode(), we call the method from the dom
	// class, which writes node and its children in the XML format
	uint16_t	indentlevel=0;
	return dom::writeNode(getRootNode(),out,indent,&indentlevel);
}

ssize_t dom::writeNode(const domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) const {

	// NOTE: this method is written a little strangely
	// to work correctly with cursordomnodes

	ssize_t	retval=0;

	domnode	*current;
	if (dn->getType()==ROOT_DOMNODETYPE) {
		current=dn->getFirstChild();
		while (!current->isNullNode()) {
			if (!incOrErr(&retval,dom::writeNode(current,out,
							indent,indentlevel))) {
				return retval;
			}
			current=current->getNextSibling();
		}
	} else if (dn->getType()==TAG_DOMNODETYPE) {
		if (indent && indentlevel) {
			if (!incOrErr(&retval,writeIndent(out,*indentlevel))) {
				return retval;
			}
		}
		if (!incOrErr(&retval,out->write("<"))) {
			return retval;
		}
		if (dn->getNamespace()) {
			if (!incOrErr(&retval,
					safeWrite(out,dn->getNamespace())) ||
				!incOrErr(&retval,out->write(":"))) {
				return retval;
			}
		}
		if (!incOrErr(&retval,safeWrite(out,dn->getName()))) {
			return retval;
		}
		current=dn->getAttribute((uint64_t)0);
		while (current && !current->isNullNode()) {
			if (!incOrErr(&retval,out->write(" ")) ||
				!incOrErr(&retval,
					dom::writeNode(current,out,
						indent,indentlevel))) {
				return retval;
			}
			current=current->getNextSibling();
		}
		current=dn->getFirstChild();
		if (!current->isNullNode()) {
			if (!incOrErr(&retval,out->write(">"))) {
				return retval;
			}
			if (indent && indentlevel) {
				if (current->getType()!=TEXT_DOMNODETYPE &&
					current->getType()!=CDATA_DOMNODETYPE) {
					if (!incOrErr(&retval,
							out->write("\n"))) {
						return retval;
					}
				}
				*indentlevel=*indentlevel+2;
			}
			domnodetype	prevtype=current->getType();
			while (!current->isNullNode()) {
				if (!incOrErr(&retval,
						dom::writeNode(
							current,out,
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
			if (!incOrErr(&retval,out->write("</"))) {
				return retval;
			}
			if (dn->getNamespace()) {
				if (!incOrErr(&retval,
					safeWrite(out,dn->getNamespace()))) {
					return retval;
				}
				if (!incOrErr(&retval,out->write(":"))) {
					return retval;
				}
			}
			if (!incOrErr(&retval,safeWrite(out,dn->getName())) ||
					!incOrErr(&retval,out->write(">"))) {
				return retval;
			}
			if (indent && indentlevel) {
				if (!incOrErr(&retval,out->write("\n"))) {
					return retval;
				}
			}
		} else {
			if (dn->getName()[0]=='?') {
				if (!incOrErr(&retval,out->write("?>"))) {
					return retval;
				}
			} else if (dn->getName()[0]=='!') {
				if (!incOrErr(&retval,out->write(">"))) {
					return retval;
				}
			} else {
				if (!incOrErr(&retval,out->write("/>"))) {
					return retval;
				}
			}
			if (indent && indentlevel) {
				if (!incOrErr(&retval,out->write("\n"))) {
					return retval;
				}
			}
		}
	} else if (dn->getType()==TEXT_DOMNODETYPE) {
		incOrErr(&retval,safeWrite(out,dn->getValue()));
	} else if (dn->getType()==ATTRIBUTE_DOMNODETYPE) {
		if (dn->getParent()->getName()[0]=='!') {
			incOrErr(&retval,out->write("\"")) &&
			incOrErr(&retval,safeWrite(out,dn->getValue())) &&
			incOrErr(&retval,out->write("\""));
		} else {
			incOrErr(&retval,safeWrite(out,dn->getName())) &&
			incOrErr(&retval,out->write("=\"")) &&
			incOrErr(&retval,safeWrite(out,dn->getValue())) &&
			incOrErr(&retval,out->write("\""));
		}
	} else if (dn->getType()==COMMENT_DOMNODETYPE) {
		incOrErr(&retval,out->write("<!--")) &&
		incOrErr(&retval,safeWrite(out,dn->getValue())) &&
		incOrErr(&retval,out->write("-->"));
	} else if (dn->getType()==CDATA_DOMNODETYPE) {
		incOrErr(&retval,out->write("<![CDATA[")) &&
		incOrErr(&retval,safeWrite(out,dn->getValue())) &&
		incOrErr(&retval,out->write("]]>"));
	}
	return retval;
}

ssize_t dom::writeIndent(output *out, uint16_t indent) const {
	ssize_t	retval=0;
	for (uint16_t i=0; i<indent; i++) {
		incOrErr(&retval,out->write(' '));
	}
	return retval;
}

ssize_t dom::safeWrite(output *out, const char *str) const {

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
			num=static_cast<uint16_t>(
					static_cast<unsigned char>(*ch));
		}
		if (entity || num) {
			incOrErr(&retval,out->write(start,ch-start));
			if (entity) {
				ssize_t	len=charstring::length(entity);
				incOrErr(&retval,out->write(entity,len));
				entity=NULL;
			} else {
				incOrErr(&retval,out->write("&#"));
				char	*numstr=charstring::parseNumber(num);
				ssize_t	len=charstring::length(numstr);
				incOrErr(&retval,out->write(numstr,len));
				delete[] numstr;
				incOrErr(&retval,out->write(";"));
				num=0;
			}
			start=ch+1;
		}
	}
	incOrErr(&retval,out->write(start,ch-start));
	return retval;
}

bool dom::stringCacheEnabled() {
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
