// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/stencilhttprequest.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>

stencilhttprequest::stencilhttprequest(httpserverapi *sapi) :
						httprequest(sapi) {
	initUrlVariables();
	initSkinVariables();
}

stencilhttprequest::~stencilhttprequest() {

	cleanSkinVariables();
}

void stencilhttprequest::dumpVariables() {

	httprequest::dumpVariables();

	sapi->write("Skin Variables:\n");

	for (linkedlistnode<namevaluepairsnode *>
				*svnode=skinvars.getList()->getFirst();
				svnode; svnode=svnode->getNext()) {
		sapi->write(svnode->getValue()->getKey());
		sapi->write("=");
		sapi->write(svnode->getValue()->getValue());
		sapi->write("\n");
	}

	sapi->write("\n");

	sapi->write("URL Variables:\n");

	for (uint64_t i=0; i<urlvariablecount; i++) {
		sapi->write(urlvars[i]);
		sapi->write("=");
		sapi->write(urlvals[i]);
		sapi->write("\n");
	}
	sapi->write("parametersAsGetString=");
	getParametersAsGetString(sapi,NULL);
	sapi->write("\n");
	sapi->write("parametersAsHiddenVariables=");
	getParametersAsHiddenVariables(sapi,NULL);
	sapi->write("\n");
}

void stencilhttprequest::initSkinVariables() {

	char	slash=sys::getDirectorySeparator();

	dirtyskinvariables=false;
	skinvariablevars=NULL;
	skinvariablevals=NULL;

	// build the path from the application to the page
	stringbuffer	path;
	path.append(skinstr.getString());
	if (modulestr.getStringLength()) {
		path.append(slash)->append(modulestr.getString());
	}
	path.append(slash)->append(pagestr.getString());

	// iterate over the path, at each slash, try to parse an 
	// skin file at that level
	const char	*ptr1=path.getString();
	const char	*docroot=getEnvironmentVariable("DOCUMENT_ROOT");
	stringbuffer	skinvarfilename;
	for (;;) {
		if (*ptr1==slash || !*ptr1) {
			skinvarfilename.clear();
			skinvarfilename.append(docroot)->append(slash);
			if (applicationstr.getStringLength()) {
				skinvarfilename.append(
						applicationstr.getString());
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

void stencilhttprequest::parseSkinVariableFile(const char *skinvarfilename) {

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

bool stencilhttprequest::setSkinVariable(const char *name, const char *value) {
	dirtyskinvariables=true;
	dirtyallvars=true;
	skinvars.setValue(charstring::duplicate(name),
				charstring::duplicate(value));
	return true;
}

void stencilhttprequest::cleanSkinVariables() {
	cleanUp(&skinvariablevars,&skinvariablevals,&skinvars);
}

const char *stencilhttprequest::getSkinVariable(const char *name) {
	return skinvars.getValue((char *)name);
}

uint64_t stencilhttprequest::getSkinVariableCount() {
	return skinvars.getList()->getLength();
}

const char * const *stencilhttprequest::getSkinVariables() {
	buildSkinVariableList();
	return skinvariablevars;
}

const char * const *stencilhttprequest::getSkinValues() {
	buildSkinVariableList();
	return skinvariablevals;
}

void stencilhttprequest::buildSkinVariableList() {
	if (!skinvariablevals || dirtyskinvariables) {
		buildList(&skinvariablevars,&skinvariablevals,&skinvars);
		dirtyskinvariables=false;
	}
}

void stencilhttprequest::initUrlVariables() {

	char	slash=sys::getDirectorySeparator();

	urlvariablecount=0;

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
	applicationpath.append(docroot);
	if (scriptnamelistlength>0) {
		for (uint64_t index=0;
			index<scriptnamelistlength-1;
			index++) {
			if (index) {
				applicationstr.append(slash);
			}
			applicationstr.append(scriptnamelist[index]);
			applicationpath.append(slash);
			applicationpath.append(scriptnamelist[index]);
			applicationurl.append("/");
			applicationurl.append(scriptnamelist[index]);
			delete[] scriptnamelist[index];
		}
	}
	urlvars[urlvariablecount]="application";
	urlvals[urlvariablecount++]=applicationstr.getString();
	urlvars[urlvariablecount]="applicationPath";
	urlvals[urlvariablecount++]=applicationpath.getString();
	urlvars[urlvariablecount]="applicationUrl";
	urlvals[urlvariablecount++]=applicationurl.getString();

	// build the program name
	programpath.append(applicationpath.getString());
	if (scriptnamelistlength) {
		programstr.append(scriptnamelist[scriptnamelistlength-1]);
		programpath.append(slash);
		programpath.append(scriptnamelist[scriptnamelistlength-1]);
		programurl.append("/");
		programurl.append(scriptnamelist[scriptnamelistlength-1]);
		delete[] scriptnamelist[scriptnamelistlength-1];
	}
	urlvars[urlvariablecount]="program";
	urlvals[urlvariablecount++]=programstr.getString();
	urlvars[urlvariablecount]="programPath";
	urlvals[urlvariablecount++]=programpath.getString();
	urlvars[urlvariablecount]="programUrl";
	urlvals[urlvariablecount++]=programurl.getString();

	// clean up
	delete[] scriptnamelist;

	// parse the pathinfo
	char		**pathinfolist=NULL;
	uint64_t	pathinfolistlength=0;
	charstring::split(pathinfo,"/",true,&pathinfolist,&pathinfolistlength);

	// skin is the first thing in the list (unless the page was)
	skinpath.append(applicationpath.getString());
	skinurl.append(applicationurl.getString());
	if (pathinfolistlength>1) {
		skinstr.append(pathinfolist[0]);
		skinpath.append(slash);
		skinpath.append(pathinfolist[0]);
		skinurl.append("/");
		skinurl.append(pathinfolist[0]);
		delete[] pathinfolist[0];
	}
	urlvars[urlvariablecount]="skin";
	urlvals[urlvariablecount++]=skinstr.getString();
	urlvars[urlvariablecount]="skinPath";
	urlvals[urlvariablecount++]=skinpath.getString();
	urlvars[urlvariablecount]="skinUrl";
	urlvals[urlvariablecount++]=skinurl.getString();

	// module is everything between the skin and page
	modulepath.append(skinpath.getString());
	moduleurl.append(skinurl.getString());
	if (pathinfolistlength>2) {
		for (uint64_t index=1;
				index<pathinfolistlength-1;
				index++) {
			if (index>1) {
				modulestr.append(slash);
			}
			modulestr.append(pathinfolist[index]);
			modulepath.append(slash);
			modulepath.append(pathinfolist[index]);
			moduleurl.append("/");
			moduleurl.append(pathinfolist[index]);
			delete[] pathinfolist[index];
		}
	}
	urlvars[urlvariablecount]="module";
	urlvals[urlvariablecount++]=modulestr.getString();
	urlvars[urlvariablecount]="modulePath";
	urlvals[urlvariablecount++]=modulepath.getString();
	urlvars[urlvariablecount]="moduleUrl";
	urlvals[urlvariablecount++]=moduleurl.getString();

	// page is the last thing in the list
	pagepath.append(modulepath.getString());
	pageurl.append(moduleurl.getString());
	if (pathinfolistlength) {
		pagestr.append(pathinfolist[pathinfolistlength-1]);
		pagepath.append(slash);
		pagepath.append(pathinfolist[pathinfolistlength-1]);
		pageurl.append("/");
		pageurl.append(pathinfolist[pathinfolistlength-1]);
		delete[] pathinfolist[pathinfolistlength-1];
	}
	urlvars[urlvariablecount]="page";
	urlvals[urlvariablecount++]=pagestr.getString();
	urlvars[urlvariablecount]="pagePath";
	urlvals[urlvariablecount++]=pagepath.getString();
	urlvars[urlvariablecount]="pageUrl";
	urlvals[urlvariablecount++]=pageurl.getString();

	urlvars[urlvariablecount]=NULL;
	urlvals[urlvariablecount]=NULL;

	// clean up
	delete[] pathinfolist;
}

const char *stencilhttprequest::application() {
	return applicationstr.getString();
}

const char *stencilhttprequest::applicationPath() {
	return applicationpath.getString();
}

const char *stencilhttprequest::applicationUrl() {
	return applicationurl.getString();
}

const char *stencilhttprequest::program() {
	return programstr.getString();
}

const char *stencilhttprequest::programPath() {
	return programpath.getString();
}

const char *stencilhttprequest::programUrl() {
	return programurl.getString();
}

const char *stencilhttprequest::module() {
	return modulestr.getString();
}

const char *stencilhttprequest::modulePath() {
	return modulepath.getString();
}

const char *stencilhttprequest::moduleUrl() {
	return moduleurl.getString();
}

const char *stencilhttprequest::skin() {
	return skinstr.getString();
}

const char *stencilhttprequest::skinPath() {
	return skinpath.getString();
}

const char *stencilhttprequest::skinUrl() {
	return skinurl.getString();
}

const char *stencilhttprequest::page() {
	return pagestr.getString();
}

const char *stencilhttprequest::pagePath() {
	return pagepath.getString();
}

const char *stencilhttprequest::pageUrl() {
	return pageurl.getString();
}

uint64_t stencilhttprequest::getUrlVariableCount() {
	return urlvariablecount;
}

const char * const *stencilhttprequest::getUrlVariables() {
	return urlvars;
}

const char * const *stencilhttprequest::getUrlValues() {
	return urlvals;
}

void stencilhttprequest::buildAllVariables() {

	if (allvals && allvars && !dirtyallvars) {
		return;
	}

	httprequest::buildAllVariables();

	// extend the allvars/allvals array
	uint64_t	index=allvariablecount;
	uint64_t	newallvariablecount=allvariablecount+
						getSkinVariableCount()+
						getUrlVariableCount();
	const char	**newallvars=new const char *[newallvariablecount+1];
	const char	**newallvals=new const char *[newallvariablecount+1];
	for (uint64_t i=0; i<allvariablecount; i++) {
		newallvars[i]=allvars[i];
		newallvals[i]=allvals[i];
	}
	delete[] allvars;
	delete[] allvals;
	allvars=newallvars;
	allvals=newallvals;
	allvariablecount=newallvariablecount;

	// add skin variables
	for (linkedlistnode<namevaluepairsnode *>
				*sknode=skinvars.getList()->getFirst();
				sknode; sknode=sknode->getNext()) {
		allvars[index]=sknode->getValue()->getKey();
		allvals[index++]=sknode->getValue()->getValue();
	}

	// add url variables
	for (uint64_t resind=0; resind<getUrlVariableCount(); resind++) {
		allvars[index]=urlvars[resind];
		allvals[index++]=urlvals[resind];
	}

	// terminate the array
	allvars[index]=NULL;
	allvals[index]=NULL;

	dirtyallvars=false;
}

bool stencilhttprequest::methodAllowed() {
	return httprequest::methodAllowed(
				getSkinVariable("allowed-methods"),
				getSkinVariable("denied-methods"));
}

bool stencilhttprequest::ipAllowed() {
	return httprequest::ipAllowed(
				getSkinVariable("allowed-ips"),
				getSkinVariable("denied-ips"));
}

bool stencilhttprequest::refererAllowed() {
	return httprequest::refererAllowed(
				getSkinVariable("allowed-ips"),
				getSkinVariable("denied-ips"));
}
