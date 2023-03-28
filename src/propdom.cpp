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
	construct(true);
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
}

propdom::propdom(bool stringcacheenabled) : propsax(), dom(stringcacheenabled) {
	construct(stringcacheenabled);
}

propdom::propdom(propdom &x) : propsax(), dom(x) {
	construct(stringCacheEnabled());
}

propdom &propdom::operator=(propdom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void propdom::construct(bool stringcacheenabled) {
	pvt=new propdomprivate;
	pvt->_current=NULL;
}

propdom::~propdom() {
	delete pvt;
}

const char *propdom::getType() {
	return "propdom";
}

bool propdom::parse(input *in) {
	return parse(in,NULL,0);
}

bool propdom::parse(input *in, domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_current=parent;
	} else {
		reset();
	}
	bool	retval=sax::parse(in);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
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
	bool	retval=sax::parseFile(filename);
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
	bool	retval=sax::parseString(string);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
}

bool propdom::reset() {
	pvt->_current=NULL;
	return dom::reset() && propsax::reset();
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
	pvt->_current->appendTag("e")->appendAttribute("v",c);
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

ssize_t propdom::writeAndEscape(output *out, const char *value) {
	ssize_t	retval=0;
	for (const char *c=value; *c; c++) {
		if (character::isWhitespace(*c)) {
			if (!incOrErr(&retval,out->write('\\'),1)) {
				return retval;
			}
		}
		if (!incOrErr(&retval,out->write(*c),1)) {
			return retval;
		}
	}
	return retval;
}

ssize_t propdom::writeNode(domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) {

	ssize_t	retval=0;

	switch (dn->getType()) {
		case ROOT_DOMNODETYPE:
			for (domnode *child=dn->getFirstChild();
					!child->isNullNode();
					child=child->getNextSibling()) {
				if (!incOrErr(&retval,
						writeNode(child,out,
							indent,indentlevel))) {
					return retval;
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
					if (!incOrErr(&retval,
							out->write('!'),1) ||
						!incOrErr(&retval,
							out->write(val,vlen),
							vlen)) {
						return retval;
					}
					break;
				case 'p':
					if (!incOrErr(&retval,
							out->write('#'),1) ||
						!incOrErr(&retval,
							out->write(val,vlen),
							vlen)) {
						return retval;
					}
					break;
				case 'k':
					e=dn->getAttributeValue("e");
					elen=charstring::length(e);
					if (!incOrErr(&retval,writeAndEscape(out,dn->getAttributeValue("k"))) ||
						!incOrErr(&retval,out->write(e,elen),elen) ||
						!incOrErr(&retval,writeAndEscape(out,val))) {
						return retval;
					}
					break;
			}
			}
			break;
		case TEXT_DOMNODETYPE:
			{
			const char	*val=dn->getValue();
			ssize_t		vlen=charstring::length(val);
			if (!incOrErr(&retval,out->write(val,vlen),vlen)) {
				return retval;
			}
			}
			break;
		default:
			break;
	}
	return retval;
}
