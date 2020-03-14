// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inidom.h>
#include <rudiments/charstring.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class inidomprivate {
	friend class inidom;
	private:
		domnode			*_current;
		linkedlist<bool>	_inarray;

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
	bool	retval=inisax::parseFile(filename);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::write(getRootNode(),&stdoutput,true,&indentlevel);
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
	bool	retval=inisax::parseString(string);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::write(getRootNode(),&stdoutput,true,&indentlevel);
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

bool inidom::write(output *out) const {
	return dom::write(out,true);
}

void inidom::write(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	switch (dn->getType()) {
		case ROOT_DOMNODETYPE:
			for (domnode *child=dn->getFirstChild();
					!child->isNullNode();
					child=child->getNextSibling()) {
				write(child,out,indent,indentlevel);
			}
			break;
		case TAG_DOMNODETYPE:
			{
			const char	*name=dn->getName();
			switch (*name) {
				case 'c':
					out->write(';');
					out->write(dn->getAttributeValue("v"));
					break;
				case 'p':
					out->write('#');
					out->write(dn->getAttributeValue("v"));
					break;
				case 's':
					out->write('[');
					out->write(dn->getAttributeValue("v"));
					out->write(']');
					for (domnode *child=dn->getFirstChild();
						!child->isNullNode();
						child=child->getNextSibling()) {
						write(child,out,
							indent,indentlevel);
					}
					break;
				case 'k':
					out->write(dn->getAttributeValue("k"));
					out->write('=');
					out->write(dn->getAttributeValue("v"));
					break;
			}
			}
			break;
		case TEXT_DOMNODETYPE:
			out->write(dn->getValue());
			break;
		default:
			break;
	}
}
