// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inidom.h>
#include <rudiments/charstring.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class inidomprivate {
	friend class inidom;
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

inidom::inidom() : inisax(), dom() {
	init(true);
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
}

inidom::inidom(bool stringcacheenabled) : inisax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

inidom::inidom(const inidom &x) : inisax(), dom(x) {
	init(stringCacheEnabled());
}

inidom &inidom::operator=(const inidom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void inidom::init(bool stringcacheenabled) {
	pvt=new inidomprivate;
	pvt->_current=NULL;
}

inidom::~inidom() {
	delete pvt;
}

const char *inidom::getType() const {
	return "inidom";
}

bool inidom::parse(input *in) {
	return parse(in,NULL,0);
}

bool inidom::parse(input *in, domnode *parent, uint64_t position) {
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

bool inidom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool inidom::parseFile(const char *filename,
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

bool inidom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool inidom::parseString(const char *string,
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

void inidom::reset() {
	pvt->_current=NULL;
	dom::reset();
	inisax::reset();
}

void inidom::createRootNode() {
	dom::createRootNode();
	pvt->_current=getRootNode();
}

bool inidom::whitespaceStart() {
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

bool inidom::whitespace(const char *w) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("\"%s\"\n",w);
#endif
	pvt->_current->appendText(w);
	return true;
}

bool inidom::whitespaceEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool inidom::semicolonCommentStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("; comment {\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	return true;
}

bool inidom::semicolonComment(const char *c) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("%s\n",c);
#endif
	pvt->_current->appendTag("c")->appendAttribute("v",c);
	return true;
}

bool inidom::semicolonCommentEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool inidom::poundCommentStart() {
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

bool inidom::poundComment(const char *c) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("%s\n",c);
#endif
	pvt->_current->appendTag("p")->appendAttribute("v",c);
	return true;
}

bool inidom::poundCommentEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	return true;
}

bool inidom::sectionStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode();
	}
	pvt->_current=pvt->_current->appendTag("s");
	return true;
}

bool inidom::section(const char *s) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s {\n",s);
	pvt->_indent+=2;
#endif
	pvt->_current->setAttributeValue("v",s);
	return true;
}

bool inidom::sectionEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	pvt->_current=pvt->_current->getParent();
	return true;
}

bool inidom::keyStart() {
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

bool inidom::key(const char *k) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s=",k);
#endif
	pvt->_current->setAttributeValue("k",k);
	return true;
}

bool inidom::keyEnd() {
	return true;
}

bool inidom::valueStart() {
	return true;
}

bool inidom::value(const char *v) {
#ifdef DEBUG_MESSAGES
	debugPrintf("%s\n",v);
#endif
	pvt->_current->appendAttribute("v",v);
	return true;
}

bool inidom::valueEnd() {
	pvt->_current=pvt->_current->getParent();
	return true;
}

ssize_t inidom::writeNode(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

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
			const char	*key=NULL;
			ssize_t		klen=0;
			const char	*val=dn->getAttributeValue("v");
			ssize_t		vlen=charstring::length(val);
			switch (*name) {
				case 'c':
					if (!incOrErr(&retval,
							out->write(';')) ||
						!incOrErr(&retval,
							out->write(val,vlen))) {
						return retval;
					}
					break;
				case 'p':
					if (!incOrErr(&retval,
							out->write('#')) ||
						!incOrErr(&retval,
							out->write(val,vlen))) {
						return retval;
					}
					break;
				case 's':
					if (!incOrErr(&retval,
							out->write('[')) ||
						!incOrErr(&retval,
							out->write(val,vlen)) ||
						!incOrErr(&retval,
							out->write(']'))) {
						return retval;
					}
					for (domnode *child=dn->getFirstChild();
						!child->isNullNode();
						child=child->getNextSibling()) {
						if (!incOrErr(&retval,
							writeNode(child,
								out,indent,
								indentlevel))) {
							return retval;
						}
					}
					break;
				case 'k':
					key=dn->getAttributeValue("k");
					klen=charstring::length(key);
					if (!incOrErr(&retval,
							out->write(key,klen)) ||
						!incOrErr(&retval,
							out->write('=')) ||
						!incOrErr(&retval,
							out->write(val,vlen))) {
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
			if (!incOrErr(&retval,out->write(val,vlen))) {
				return retval;
			}
			}
			break;
		default:
			break;
	}
	return retval;
}
