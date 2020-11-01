// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/urlhttprequest.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>

#define URL_VARIABLE_COUNT 15

class urlhttprequestprivate {
	friend class urlhttprequest;
	private:
		httpserverapi	*_sapi;
		
		const char	*_urlvars[URL_VARIABLE_COUNT+1];
		const char	*_urlvals[URL_VARIABLE_COUNT+1];

		uint64_t	_urlvariablecount;
		stringbuffer	_applicationstr;
		stringbuffer	_applicationpath;
		stringbuffer	_applicationurl;
		stringbuffer	_programstr;
		stringbuffer	_programpath;
		stringbuffer	_programurl;
		stringbuffer	_skinstr;
		stringbuffer	_skinpath;
		stringbuffer	_skinurl;
		stringbuffer	_modulestr;
		stringbuffer	_modulepath;
		stringbuffer	_moduleurl;
		stringbuffer	_pagestr;
		stringbuffer	_pagepath;
		stringbuffer	_pageurl;

		namevaluepairs	_skinvars;
		bool		_dirtyskinvariables;
		const char	**_skinvariablevars;
		const char	**_skinvariablevals;
};

urlhttprequest::urlhttprequest(httpserverapi *sapi) : httprequest(sapi) {
	pvt=new urlhttprequestprivate;
	pvt->_sapi=sapi;
	initUrlVariables();
	initSkinVariables();
}

urlhttprequest::~urlhttprequest() {
	cleanSkinVariables();
	delete pvt;
}

void urlhttprequest::dumpVariables() {

	httprequest::dumpVariables();

	pvt->_sapi->write("Skin Variables:\n");

	for (linkedlistnode<namevaluepairsnode *>
				*svnode=pvt->_skinvars.getList()->getFirst();
				svnode; svnode=svnode->getNext()) {
		pvt->_sapi->write(svnode->getValue()->getKey());
		pvt->_sapi->write("=");
		pvt->_sapi->write(svnode->getValue()->getValue());
		pvt->_sapi->write("\n");
	}

	pvt->_sapi->write("\n");

	pvt->_sapi->write("URL Variables:\n");

	for (uint64_t i=0; i<pvt->_urlvariablecount; i++) {
		pvt->_sapi->write(pvt->_urlvars[i]);
		pvt->_sapi->write("=");
		pvt->_sapi->write(pvt->_urlvals[i]);
		pvt->_sapi->write("\n");
	}
	pvt->_sapi->write("parametersAsGetString=");
	getParametersAsGetString(pvt->_sapi,NULL);
	pvt->_sapi->write("\n");
	pvt->_sapi->write("parametersAsHiddenVariables=");
	getParametersAsHiddenVariables(pvt->_sapi,NULL);
	pvt->_sapi->write("\n");
}

void urlhttprequest::initSkinVariables() {

	char	slash=sys::getDirectorySeparator();

	pvt->_dirtyskinvariables=false;
	pvt->_skinvariablevars=NULL;
	pvt->_skinvariablevals=NULL;

	// build the path from the application to the page
	stringbuffer	path;
	path.append(pvt->_skinstr.getString());
	if (pvt->_modulestr.getStringLength()) {
		path.append(slash)->append(pvt->_modulestr.getString());
	}
	path.append(slash)->append(pvt->_pagestr.getString());

	// iterate over the path, at each slash, try to parse an 
	// skin file at that level
	const char	*ptr1=path.getString();
	const char	*docroot=getEnvironmentVariable("DOCUMENT_ROOT");
	stringbuffer	skinvarfilename;
	for (;;) {
		if (*ptr1==slash || !*ptr1) {
			skinvarfilename.clear();
			skinvarfilename.append(docroot)->append(slash);
			if (pvt->_applicationstr.getStringLength()) {
				skinvarfilename.append(
					pvt->_applicationstr.getString());
				skinvarfilename.append(slash);
			}
			const char	*ptr2=path.getString();
			while (ptr2!=ptr1) {
				skinvarfilename.append(*ptr2);
				ptr2++;
			}
			skinvarfilename.append(".var");
			parseSkinVariableFile(skinvarfilename.getString());
		}
		if (!*ptr1) {
			break;
		}
		ptr1++;
	}
}

void urlhttprequest::parseSkinVariableFile(const char *skinvarfilename) {

	// open file
	xmldom	skinvarfile;
	if (!skinvarfile.parseFile(skinvarfilename)) {
		return;
	}

	// get the skin node
	domnode	*skinnode=skinvarfile.getRootNode()->getFirstTagChild("skin");
	if (skinnode->isNullNode()) {
		return;
	}

	// get the variables
	stringbuffer	buffer;
	for (domnode *varnode=skinnode->getFirstTagChild("var");
			!varnode->isNullNode();
			varnode=varnode->getNextTagSibling("var")) {

		// just in case the variable contains xml
		for (domnode *child=varnode->getChild((uint64_t)0);
				!child->isNullNode();
				child=child->getNextSibling()) {
			if (child->getType()==TEXT_DOMNODETYPE) {
				buffer.append(child->getValue());
			} else {
				child->write(&buffer);
			}
		}

		setSkinVariable(varnode->getAttribute("name")->getValue(),
							buffer.getString());
		buffer.clear();
	}
}

bool urlhttprequest::setSkinVariable(const char *name, const char *value) {
	pvt->_dirtyskinvariables=true;
	dirtyAllVars(true);
	pvt->_skinvars.setValue(charstring::duplicate(name),
				charstring::duplicate(value));
	return true;
}

void urlhttprequest::cleanSkinVariables() {
	cleanUp(&(pvt->_skinvariablevars),
			&(pvt->_skinvariablevals),
			&(pvt->_skinvars));
}

const char *urlhttprequest::getSkinVariable(const char *name) {
	return pvt->_skinvars.getValue((char *)name);
}

uint64_t urlhttprequest::getSkinVariableCount() {
	return pvt->_skinvars.getList()->getLength();
}

const char * const *urlhttprequest::getSkinVariables() {
	buildSkinVariableList();
	return pvt->_skinvariablevars;
}

const char * const *urlhttprequest::getSkinValues() {
	buildSkinVariableList();
	return pvt->_skinvariablevals;
}

void urlhttprequest::buildSkinVariableList() {
	if (!pvt->_skinvariablevals || pvt->_dirtyskinvariables) {
		buildList(&(pvt->_skinvariablevars),
				&(pvt->_skinvariablevals),
				&(pvt->_skinvars));
		pvt->_dirtyskinvariables=false;
	}
}

void urlhttprequest::initUrlVariables() {

	char	slash=sys::getDirectorySeparator();

	pvt->_urlvariablecount=0;

	// get the document root, script name and pathinfo
	const char	*docroot=getEnvironmentVariable("DOCUMENT_ROOT");
	const char	*scriptname=getEnvironmentVariable("SCRIPT_NAME");
	const char	*pathinfo=getEnvironmentVariable("PATH_INFO");

	// parse the scriptname
	char		**scriptnamelist=NULL;
	uint64_t	scriptnamelistlength=0;
	charstring::split(scriptname,"/",true,
				&scriptnamelist,&scriptnamelistlength);

	// build the application name
	pvt->_applicationpath.append(docroot);
	if (scriptnamelistlength>0) {
		for (uint64_t index=0;
			index<scriptnamelistlength-1;
			index++) {
			if (index) {
				pvt->_applicationstr.append(slash);
			}
			pvt->_applicationstr.append(scriptnamelist[index]);
			pvt->_applicationpath.append(slash);
			pvt->_applicationpath.append(scriptnamelist[index]);
			pvt->_applicationurl.append("/");
			pvt->_applicationurl.append(scriptnamelist[index]);
			delete[] scriptnamelist[index];
		}
	}
	pvt->_urlvars[pvt->_urlvariablecount]="application";
	pvt->_urlvals[pvt->_urlvariablecount++]=
					pvt->_applicationstr.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="applicationPath";
	pvt->_urlvals[pvt->_urlvariablecount++]=
					pvt->_applicationpath.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="applicationUrl";
	pvt->_urlvals[pvt->_urlvariablecount++]=
					pvt->_applicationurl.getString();

	// build the program name
	pvt->_programpath.append(pvt->_applicationpath.getString());
	pvt->_programurl.append(pvt->_applicationurl.getString());
	if (scriptnamelistlength) {
		pvt->_programstr.append(
				scriptnamelist[scriptnamelistlength-1]);
		pvt->_programpath.append(slash);
		pvt->_programpath.append(
				scriptnamelist[scriptnamelistlength-1]);
		pvt->_programurl.append("/");
		pvt->_programurl.append(scriptnamelist[scriptnamelistlength-1]);
		delete[] scriptnamelist[scriptnamelistlength-1];
	}
	pvt->_urlvars[pvt->_urlvariablecount]="program";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_programstr.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="programPath";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_programpath.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="programUrl";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_programurl.getString();

	// clean up
	delete[] scriptnamelist;

	// parse the pathinfo
	char		**pathinfolist=NULL;
	uint64_t	pathinfolistlength=0;
	charstring::split(pathinfo,"/",true,&pathinfolist,&pathinfolistlength);

	// skin is the first thing in the list (unless the page was)
	pvt->_skinpath.append(pvt->_applicationpath.getString());
	pvt->_skinurl.append(pvt->_applicationurl.getString());
	if (pathinfolistlength>1) {
		pvt->_skinstr.append(pathinfolist[0]);
		pvt->_skinpath.append(slash);
		pvt->_skinpath.append(pathinfolist[0]);
		pvt->_skinurl.append("/");
		pvt->_skinurl.append(pathinfolist[0]);
		delete[] pathinfolist[0];
	}
	pvt->_urlvars[pvt->_urlvariablecount]="skin";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_skinstr.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="skinPath";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_skinpath.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="skinUrl";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_skinurl.getString();

	// module is everything between the skin and page
	pvt->_modulepath.append(pvt->_skinpath.getString());
	pvt->_moduleurl.append(pvt->_skinurl.getString());
	if (pathinfolistlength>2) {
		for (uint64_t index=1;
				index<pathinfolistlength-1;
				index++) {
			if (index>1) {
				pvt->_modulestr.append(slash);
			}
			pvt->_modulestr.append(pathinfolist[index]);
			pvt->_modulepath.append(slash);
			pvt->_modulepath.append(pathinfolist[index]);
			pvt->_moduleurl.append("/");
			pvt->_moduleurl.append(pathinfolist[index]);
			delete[] pathinfolist[index];
		}
	}
	pvt->_urlvars[pvt->_urlvariablecount]="module";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_modulestr.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="modulePath";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_modulepath.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="moduleUrl";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_moduleurl.getString();

	// page is the last thing in the list
	pvt->_pagepath.append(pvt->_modulepath.getString());
	pvt->_pageurl.append(pvt->_moduleurl.getString());
	if (pathinfolistlength) {
		pvt->_pagestr.append(pathinfolist[pathinfolistlength-1]);
		pvt->_pagepath.append(slash);
		pvt->_pagepath.append(pathinfolist[pathinfolistlength-1]);
		pvt->_pageurl.append("/");
		pvt->_pageurl.append(pathinfolist[pathinfolistlength-1]);
		delete[] pathinfolist[pathinfolistlength-1];
	}
	pvt->_urlvars[pvt->_urlvariablecount]="page";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_pagestr.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="pagePath";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_pagepath.getString();
	pvt->_urlvars[pvt->_urlvariablecount]="pageUrl";
	pvt->_urlvals[pvt->_urlvariablecount++]=pvt->_pageurl.getString();

	pvt->_urlvars[pvt->_urlvariablecount]=NULL;
	pvt->_urlvals[pvt->_urlvariablecount]=NULL;

	// clean up
	delete[] pathinfolist;
}

const char *urlhttprequest::application() {
	return pvt->_applicationstr.getString();
}

const char *urlhttprequest::applicationPath() {
	return pvt->_applicationpath.getString();
}

const char *urlhttprequest::applicationUrl() {
	return pvt->_applicationurl.getString();
}

const char *urlhttprequest::program() {
	return pvt->_programstr.getString();
}

const char *urlhttprequest::programPath() {
	return pvt->_programpath.getString();
}

const char *urlhttprequest::programUrl() {
	return pvt->_programurl.getString();
}

const char *urlhttprequest::module() {
	return pvt->_modulestr.getString();
}

const char *urlhttprequest::modulePath() {
	return pvt->_modulepath.getString();
}

const char *urlhttprequest::moduleUrl() {
	return pvt->_moduleurl.getString();
}

const char *urlhttprequest::skin() {
	return pvt->_skinstr.getString();
}

const char *urlhttprequest::skinPath() {
	return pvt->_skinpath.getString();
}

const char *urlhttprequest::skinUrl() {
	return pvt->_skinurl.getString();
}

const char *urlhttprequest::page() {
	return pvt->_pagestr.getString();
}

const char *urlhttprequest::pagePath() {
	return pvt->_pagepath.getString();
}

const char *urlhttprequest::pageUrl() {
	return pvt->_pageurl.getString();
}

uint64_t urlhttprequest::getUrlVariableCount() {
	return pvt->_urlvariablecount;
}

const char * const *urlhttprequest::getUrlVariables() {
	return pvt->_urlvars;
}

const char * const *urlhttprequest::getUrlValues() {
	return pvt->_urlvals;
}

void urlhttprequest::buildAllVariables() {

	const char	***allvars=allVars();
	const char	***allvals=allVals();

	if (*allvals && *allvars && !dirtyAllVars()) {
		return;
	}

	httprequest::buildAllVariables();

	// extend the allvars/allvals array
	uint64_t	index=allVariableCount();
	uint64_t	newallvariablecount=allVariableCount()+
						getSkinVariableCount()+
						getUrlVariableCount();
	const char	**newallvars=new const char *[newallvariablecount+1];
	const char	**newallvals=new const char *[newallvariablecount+1];
	for (uint64_t i=0; i<allVariableCount(); i++) {
		newallvars[i]=(*allvars)[i];
		newallvals[i]=(*allvals)[i];
	}
	delete[] *allvars;
	delete[] *allvals;
	*allvars=newallvars;
	*allvals=newallvals;
	allVariableCount(newallvariablecount);

	// add skin variables
	for (linkedlistnode<namevaluepairsnode *>
				*sknode=pvt->_skinvars.getList()->getFirst();
				sknode; sknode=sknode->getNext()) {
		(*allvars)[index]=sknode->getValue()->getKey();
		(*allvals)[index++]=sknode->getValue()->getValue();
	}

	// add url variables
	for (uint64_t resind=0; resind<getUrlVariableCount(); resind++) {
		(*allvars)[index]=pvt->_urlvars[resind];
		(*allvals)[index++]=pvt->_urlvals[resind];
	}

	// terminate the array
	(*allvars)[index]=NULL;
	(*allvals)[index]=NULL;

	dirtyAllVars(false);
}

bool urlhttprequest::methodAllowed() {
	return httprequest::methodAllowed(
				getSkinVariable("allowed-methods"),
				getSkinVariable("denied-methods"));
}

bool urlhttprequest::ipAllowed() {
	return httprequest::ipAllowed(
				getSkinVariable("allowed-ips"),
				getSkinVariable("denied-ips"));
}

bool urlhttprequest::refererAllowed() {
	return httprequest::refererAllowed(
				getSkinVariable("allowed-ips"),
				getSkinVariable("denied-ips"));
}
