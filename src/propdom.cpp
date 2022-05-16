// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/propdom.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class propdomprivate {
	friend class propdom;
	private:
		domnode		*_current;
		#ifdef DEBUG_MESSAGES
		uint16_t	_indent;
		#endif
};

#ifdef DEBUG_MESSAGES
void indent(uint16_t ind) {
	if (ind>128) {
		stdoutput.printf("warning! ind=%d\n",ind);
		return;
	}
	for (uint16_t i=0; i<ind; i++) {
		stdoutput.write(' ');
	}
}
#endif

propdom::propdom() : propsax(), dom() {
	init(true);
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
}

propdom::propdom(bool stringcacheenabled) : propsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

propdom::propdom(const propdom &x) : propsax(), dom(x) {
	init(stringCacheEnabled());
}

propdom &propdom::operator=(const propdom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void propdom::init(bool stringcacheenabled) {
	pvt=new propdomprivate;
	pvt->_current=NULL;
}

propdom::~propdom() {
	delete pvt;
}

bool propdom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool propdom::parseFile(const char *filename,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_current=parent;
	} else {
		reset();
	}
	bool	retval=propsax::parseFile(filename);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
}

bool propdom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool propdom::parseString(const char *string,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_current=parent;
	} else {
		reset();
	}
	bool	retval=propsax::parseString(string);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
}

void propdom::reset() {
	pvt->_current=NULL;
	dom::reset();
	propsax::reset();
}

void propdom::createRootNode() {
	dom::createRootNode();
	pvt->_current=getRootNode();
}

bool propdom::whitespaceStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("whitespace {\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	return true;
}

bool propdom::whitespace(const char *w) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("\"%s\"\n",w);
#endif
	pvt->_current->appendText(w);
	return true;
}

bool propdom::whitespaceEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool propdom::exclamationCommentStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("! comment {\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	return true;
}

bool propdom::exclamationComment(const char *c) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("%s\n",c);
#endif
	pvt->_current->appendTag("c")->appendAttribute("v",c);
	return true;
}

bool propdom::exclamationCommentEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool propdom::poundCommentStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("# comment {\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	return true;
}

bool propdom::poundComment(const char *c) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("%s\n",c);
#endif
	pvt->_current->appendTag("p")->appendAttribute("v",c);
	return true;
}

bool propdom::poundCommentEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool propdom::keyStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	pvt->_current=pvt->_current->appendTag("k");
	return true;
}

bool propdom::key(const char *k) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",k);
#endif
	pvt->_current->setAttributeValue("k",k);
	return true;
}

bool propdom::equals(const char *e) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s",e);
#endif
	pvt->_current->setAttributeValue("e",e);
	return true;
}

bool propdom::keyEnd() {
	return true;
}

bool propdom::valueStart() {
	return true;
}

bool propdom::value(const char *v) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s\n",v);
#endif
	pvt->_current->appendAttribute("v",v);
	return true;
}

bool propdom::valueEnd() {
	pvt->_current=pvt->_current->getParent();
	return true;
}

bool propdom::writeAndEscape(output *out, const char *value) const {
	for (const char *c=value; *c; c++) {
		if (character::isWhitespace(*c)) {
			if (out->write('\\')<1) {
				return false;
			}
		}
		if (out->write(*c)<1) {
			return false;
		}
	}
	return true;
}

bool propdom::writeNode(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	switch (dn->getType()) {
		case ROOT_DOMNODETYPE:
			for (domnode *child=dn->getFirstChild();
					!child->isNullNode();
					child=child->getNextSibling()) {
				if (!writeNode(child,out,indent,indentlevel)) {
					return false;
				}
			}
			break;
		case TAG_DOMNODETYPE:
			{
			const char	*name=dn->getName();
			const char	*e=NULL;
			ssize_t		elen=0;
			const char	*val=dn->getAttributeValue("v");
			ssize_t		vlen=charstring::length(val);
			switch (*name) {
				case 'c':
					if (out->write('!')<1 ||
						out->write(val,vlen)<vlen) {
						return false;
					}
					break;
				case 'p':
					if (out->write('#')<1 ||
						out->write(val,vlen)<vlen) {
						return false;
					}
					break;
				case 'k':
					e=dn->getAttributeValue("e");
					elen=charstring::length(e);
					if (!writeAndEscape(out,
						dn->getAttributeValue("k")) ||
						out->write(e,elen)<elen ||
						!writeAndEscape(out,val)) {
						return false;
					}
					break;
			}
			}
			break;
		case TEXT_DOMNODETYPE:
			{
			const char	*val=dn->getValue();
			ssize_t		vlen=charstring::length(val);
			if (out->write(val,vlen)<vlen) {
				return false;
			}
			}
			break;
		default:
			break;
	}
	return true;
}
