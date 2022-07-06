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
	return writeFile(filename,perms,true);
}

bool dom::writeFile(const char *filename, mode_t perms, bool indent) const {

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
		return false;
	}
	fl.setWriteBufferSize(optblocksize);

	// write the file, saving any error that may occur
	bool	retval=write(&fl,indent);
	int32_t	err=0;
	if (!retval) {
		err=error::getErrorNumber();
	}

	// flush the write buffer
	fl.flushWriteBuffer(-1,-1);

	// since flushWriteBuffer() might also (silently) fail and set errno,
	// we need to restore whatever error we saved earlier because that's
	// the error that we care about
	if (!retval) {
		error::setErrorNumber(err);
	}

	// close (allowing this to overwrite the earlier error)
	if (!fl.close()) {
		retval=false;
	}

	// return the result
	return retval;
}

ssize_t dom::write() const {
	return write(&stdoutput);
}

ssize_t dom::write(output *out) const {
	return (write(out,true))?1:RESULT_ERROR;
}

ssize_t dom::write(output *out, bool indent) const {
	return getRootNode()->write(out,indent);
}

ssize_t dom::writeJson() const {
	return writeJson(&stdoutput);
}

ssize_t dom::writeJson(output *out) const {
	return (writeJson(out,true))?1:RESULT_ERROR;
}

ssize_t dom::writeJson(output *out, bool indent) const {
	return RESULT_ERROR;
}

ssize_t dom::writeXml() const {
	return writeXml(&stdoutput,true);
}

ssize_t dom::writeXml(output *out) const {
	return writeXml(out,true);
}

ssize_t dom::writeXml(output *out, bool indent) const {
	uint16_t	indentlevel=0;
	return (dom::writeNode(getRootNode(),out,
				indent,&indentlevel))?1:RESULT_ERROR;
}

bool dom::writeNode(const domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) const {

	// NOTE: this method is written a little strangely
	// to work correctly with cursordomnodes

	domnode	*current;
	if (dn->getType()==ROOT_DOMNODETYPE) {
		current=dn->getFirstChild();
		while (!current->isNullNode()) {
			if (!dom::writeNode(current,out,indent,indentlevel)) {
				return false;
			}
			current=current->getNextSibling();
		}
	} else if (dn->getType()==TAG_DOMNODETYPE) {
		if (indent && indentlevel) {
			if (!writeIndent(out,*indentlevel)) {
				return false;
			}
		}
		if (out->write("<")<1) {
			return false;
		}
		if (dn->getNamespace()) {
			if (!safeWrite(out,dn->getNamespace()) ||
						out->write(":")<1) {
				return false;
			}
		}
		if (!safeWrite(out,dn->getName())) {
			return false;
		}
		current=dn->getAttribute((uint64_t)0);
		while (current && !current->isNullNode()) {
			if (out->write(" ")<1 ||
				!dom::writeNode(current,out,
						indent,indentlevel)) {
				return false;
			}
			current=current->getNextSibling();
		}
		current=dn->getFirstChild();
		if (!current->isNullNode()) {
			if (out->write(">")<1) {
				return false;
			}
			if (indent && indentlevel) {
				if (current->getType()!=TEXT_DOMNODETYPE &&
					current->getType()!=CDATA_DOMNODETYPE) {
					if (out->write("\n")<1) {
						return false;
					}
				}
				*indentlevel=*indentlevel+2;
			}
			domnodetype	prevtype=current->getType();
			while (!current->isNullNode()) {
				if (!dom::writeNode(
					current,out,indent,indentlevel)) {
					return false;
				}
				prevtype=current->getType();
				current=current->getNextSibling();
			}
			if (indent && indentlevel) {
				*indentlevel=*indentlevel-2;
				if (prevtype!=TEXT_DOMNODETYPE &&
					prevtype!=CDATA_DOMNODETYPE) {
					if (!writeIndent(out,*indentlevel)) {
						return false;
					}
				}
			}
			if (out->write("</")<2) {
				return false;
			}
			if (dn->getNamespace()) {
				safeWrite(out,dn->getNamespace());
				if (out->write(":")<1) {
					return false;
				}
			}
			if (!safeWrite(out,dn->getName())) {
				return false;
			}
			if (out->write(">")<1) {
				return false;
			}
			if (indent && indentlevel) {
				if (out->write("\n")<1) {
					return false;
				}
			}
		} else {
			if (dn->getName()[0]=='?') {
				if (out->write("?>")<2) {
					return false;
				}
			} else if (dn->getName()[0]=='!') {
				if (out->write(">")<1) {
					return false;
				}
			} else {
				if (out->write("/>")<2) {
					return false;
				}
			}
			if (indent && indentlevel) {
				if (out->write("\n")<1) {
					return false;
				}
			}
		}
	} else if (dn->getType()==TEXT_DOMNODETYPE) {
		if (!safeWrite(out,dn->getValue())) {
			return false;
		}
	} else if (dn->getType()==ATTRIBUTE_DOMNODETYPE) {
		if (dn->getParent()->getName()[0]=='!') {
			if (out->write("\"")<1 ||
					!safeWrite(out,dn->getValue()) ||
					out->write("\"")<1) {
				return false;
			}
		} else {
			if (!safeWrite(out,dn->getName()) ||
					out->write("=\"")<2 ||
					!safeWrite(out,dn->getValue()) ||
					out->write("\"")<1) {
				return false;
			}
		}
	} else if (dn->getType()==COMMENT_DOMNODETYPE) {
		if (out->write("<!--")<4 ||
				!safeWrite(out,dn->getValue()) ||
				out->write("-->")<3) {
			return false;
		}
	} else if (dn->getType()==CDATA_DOMNODETYPE) {
		if (out->write("<![CDATA[")<9 ||
				!safeWrite(out,dn->getValue()) ||
				out->write("]]>")<3) {
			return false;
		}
	}
	return true;
}

bool dom::writeIndent(output *out, uint16_t indent) const {
	for (uint16_t i=0; i<indent; i++) {
		if (out->write(" ")<1) {
			return false;
		}
	}
	return true;
}

bool dom::safeWrite(output *out, const char *str) const {

	if (!str) {
		return true;
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
			if (out->write(start,ch-start)<ch-start) {
				return false;
			}
			if (entity) {
				ssize_t	len=charstring::length(entity);
				if (out->write(entity,len)<len) {
					return false;
				}
				entity=NULL;
			} else {
				if (out->write("&#")<2) {
					return false;
				}
				char	*numstr=charstring::parseNumber(num);
				ssize_t	len=charstring::length(numstr);
				if (out->write(numstr,len)<len) {
					delete[] numstr;
					return false;
				}
				delete[] numstr;
				if (out->write(";")<1) {
					return false;
				}
				num=0;
			}
			start=ch+1;
		}
	}
	return (out->write(start,ch-start)==ch-start);
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
