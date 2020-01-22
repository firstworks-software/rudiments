// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/httprequest.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/xmldom.h>
#include <rudiments/regularexpression.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/file.h>
#include <rudiments/sys.h>

class ringbuffer {
	public:
			ringbuffer(uint64_t size);
			~ringbuffer();
		bool	compare(const char *string, uint64_t length);
		void	append(char ch);
		bool	isFull();
		char	getHeadCharacter();
		void	clear();
	private:
		char		*buffer;
		uint64_t	size;
		uint64_t	filledsize;
		uint64_t	head;
		uint64_t	tail;
};

ringbuffer::ringbuffer(uint64_t size) {
	buffer=new char[size];
	this->size=size;
	filledsize=0;
	head=0;
	tail=0;
}

ringbuffer::~ringbuffer() {
	delete[] buffer;
}

bool ringbuffer::compare(const char *string, uint64_t length) {
	uint64_t	bufferindex=head;
	for (uint64_t stringindex=0; stringindex<length; stringindex++) {
		if (buffer[bufferindex]!=string[stringindex]) {
			return false;
		}
		bufferindex++;
		if (bufferindex==size) {
			bufferindex=0;
		}
	}
	return true;
}

void ringbuffer::append(char ch) {
	buffer[tail]=ch;
	tail++;
	if (filledsize<size) {
		filledsize++;
	} else {
		head++;
	}
	if (tail==size) {
		tail=0;
	}
	if (head==size) {
		head=0;
	}
}

bool ringbuffer::isFull() {
	return (filledsize==size);
}

char ringbuffer::getHeadCharacter() {
	return buffer[head];
}

void ringbuffer::clear() {
	head=0;
	tail=0;
	filledsize=0;
}

fileparameter::fileparameter(const char *name, const char *filename, 
			const char *tempfilename, const char *mimetype) {
	this->name=charstring::duplicate(name);
	this->filename=charstring::duplicate(filename);
	this->tempfilename=charstring::duplicate(tempfilename);
	this->mimetype=charstring::duplicate(mimetype);
}

fileparameter::~fileparameter() {
	delete[] name;
	delete[] filename;
	delete[] tempfilename;
	delete[] mimetype;
}

httprequest::httprequest(httpserverapi *sapi) {

	this->sapi=sapi;

	// tempdir length
	tmpdirlen=charstring::length(TMPDIR);

	// initialize some variables
	boundary=NULL;

	sapi->initEnvironmentVariables();
	initCookies();
	initParameters();
	initFileNames();

	dirtyallvars=false;
	allvars=NULL;
	allvals=NULL;
	allvariablecount=0;
}

httprequest::~httprequest() {

	delete[] allvals;
	delete[] allvars;

	cleanParameters();
	cleanFiles();
	cleanCookies();
}

void httprequest::cleanUp(const char ***vars, const char ***vals,
						namevaluepairs *nvp) {

	nvp->clearAndArrayDelete();
	delete[] (*vars);
	delete[] (*vals);
}

void httprequest::dumpVariables() {

	sapi->write("Environment Variables:\n");

	dumpEnvironment();

	sapi->write("\n");

	sapi->write("Multipart Boundary:\n");

	if (boundary) {
		sapi->write("\"");
		sapi->write(boundary);
		sapi->write("\"");
		sapi->write("\n");
	}

	sapi->write("\n");

	sapi->write("Parameters:\n");

	// parameters
	for (linkedlistnode<namevaluepairsnode *>
				*fenode=parameters.getList()->getFirst();
				fenode; fenode=fenode->getNext()) {
		sapi->write(fenode->getValue()->getKey());
		sapi->write("=");
		sapi->write(fenode->getValue()->getValue());
		sapi->write("\n");
	}

	sapi->write("\n");

	sapi->write("File Entries:\n");

	// file entries
	for (linkedlistnode<dictionarynode< char *, fileparameter *> *>
				*flnode=fileparameters.getList()->getFirst();
				flnode; flnode=flnode->getNext()) {
		sapi->write(flnode->getValue()->getKey());
		sapi->write("=");
		sapi->write(flnode->getValue()->getValue()->filename);
		sapi->write(" : ");
		sapi->write(flnode->getValue()->getValue()->tempfilename);
		sapi->write("\n");
	}

	sapi->write("\n");

	sapi->write("Cookies:\n");

	// cookie entries
	for (linkedlistnode<namevaluepairsnode *>
				*cknode=cookies.getList()->getFirst();
				cknode; cknode=cknode->getNext()) {
		sapi->write(cknode->getValue()->getKey());
		sapi->write("=");
		sapi->write(cknode->getValue()->getValue());
		sapi->write("\n");
	}

	sapi->write("\n");
}

void httprequest::buildList(const char ***vars, const char ***vals,
						namevaluepairs *nvp) {

	delete[] (*vars);
	delete[] (*vals);

	uint64_t	length=nvp->getList()->getLength();

	(*vars)=new const char *[length+1];
	(*vals)=new const char *[length+1];

	uint64_t	index=0;
	for (linkedlistnode<namevaluepairsnode *>
				*node=nvp->getList()->getFirst();
				node; node=node->getNext()) {
		(*vars)[index]=node->getValue()->getKey();
		(*vals)[index]=node->getValue()->getValue();
		index++;
	}
	(*vars)[index]=NULL;
	(*vals)[index]=NULL;
}

void httprequest::removeTempFiles() {

	for (linkedlistnode<dictionarynode< char *, fileparameter *> *>
				*flnode=fileparameters.getList()->getFirst();
				flnode; flnode=flnode->getNext()) {
		file::remove(flnode->getValue()->getValue()->tempfilename);
	}
}

void httprequest::cleanParameters() {
	cleanUp(&parametervars,&parametervals,&parameters);
}

void httprequest::cleanFiles() {

	for (linkedlistnode<dictionarynode<char *, fileparameter *> *>
				*flnode=fileparameters.getList()->getFirst();
				flnode; flnode=flnode->getNext()) {
		delete[] flnode->getValue()->getKey();
		file::remove(flnode->getValue()->getValue()->filename);
		delete flnode->getValue()->getValue();
	}
	delete[] filenames;
}

void httprequest::initParameters() {

	dirtyparameters=false;
	parametervars=NULL;
	parametervals=NULL;

	// handle a get or a head
	const char	*requestmethod=
				getEnvironmentVariable("REQUEST_METHOD");
	const char	*contenttype=
				getEnvironmentVariable("CONTENT_TYPE");
	if (requestmethod &&
		(!charstring::compareIgnoringCase(requestmethod,"get") || 
		!charstring::compareIgnoringCase(requestmethod,"head"))) {

		parseQueryString(get_request);
	}

	// handle a post
	if (requestmethod &&
		!charstring::compareIgnoringCase(requestmethod,"post")) {

		// handle application/x-www-form-urlencoded and 
		if (contenttype && !charstring::compare(contenttype,
				"application/x-www-form-urlencoded",33)) {
			parseQueryString(post_request);

		// handle multipart/form-data
		} else if (contenttype && !charstring::compare(contenttype,
						"multipart/form-data",19)) {
			parseMultipart();
		}
	}
}

void httprequest::initFileNames() {
	dirtyfilenames=false;
	filenames=NULL;
}

void httprequest::parseQueryString(httprequestmethod method) {

	// set up some buffers
	stringbuffer	name;
	stringbuffer	value;
	stringbuffer	*out=&name;
	char		sixteens;
	char		ones;
	char		charbuf;
	size_t		length=0;

	// run through string, unescaping as I go, creating parameters
	const char	*querystring=
				getEnvironmentVariable("QUERY_STRING");
	const char	*contentlengthstr=
				getEnvironmentVariable("CONTENT_LENGTH");
	size_t	contentlength;
	if (!charstring::isNullOrEmpty(contentlengthstr)) {
		contentlength=charstring::toUnsignedInteger(contentlengthstr);
	} else {
		contentlength=0;
	}
	if (method==get_request || method==head_request) {
		if (!charstring::isNullOrEmpty(querystring)) {
			length=charstring::length(querystring);
		} else {
			length=contentlength;
		}
	} else {
		if (contentlength) {
			length=contentlength;
		} else if (!charstring::isNullOrEmpty(querystring)) {
			length=charstring::length(querystring);
		} else {
			length=0;
		}
	}
	for (size_t i=0; i<length; i++) {

		if (method==get_request || method==head_request) {
			// for get or head get from querystring
			charbuf=querystring[i];
		} else { 
			// for post get from stdin
			sapi->getCharacter(&charbuf);
		}

		if (charbuf=='&') {

			// create new parameter
			setParameter(name.getString(),value.getString());

			// clear buffers
			name.clear();
			value.clear();

			// reset out
			out=&name;
		} else if (charbuf=='=') {
			out=&value;
		} else if (charbuf=='+') {
			out->append(' ');
		} else if (charbuf=='%') {
			// convert hex to ascii

			// get next char in stream: sixteens place
			i++;
			if (method==get_request || method==head_request) {
				charbuf=querystring[i];
			} else { 
				sapi->getCharacter(&charbuf);
			}

			sixteens=character::toUpperCase(charbuf);
			if (sixteens>='0' && sixteens<='9') {
				sixteens=(sixteens-48)*16;
			} else {
				sixteens=(sixteens-55)*16;
			}

			// get next char in stream: ones place
			i++;
			if (method==get_request || method==head_request) {
				charbuf=querystring[i];
			} else { 
				sapi->getCharacter(&charbuf);
			}

			ones=character::toUpperCase(charbuf);
			if (ones>='0' && ones<='9') {
				ones=ones-48;
			} else {
				ones=ones-55;
			}

			out->append((char)(sixteens+ones));
		} else {
			out->append(charbuf);
		}
	}

	// create last new parameter
	if (length>0) {
		setParameter(name.getString(),value.getString());
	}
}

bool httprequest::setParameter(const char *name, const char *value) {
	dirtyparameters=true;
	dirtyallvars=true;
	parameters.setValue(charstring::duplicate(name),
				charstring::duplicate(value));
	return true;
}

void httprequest::parseMultipart() {

	// get the boundary and its length
	boundary=charstring::findFirst(getEnvironmentVariable("CONTENT_TYPE"),
								"boundary=")+9;
	size_t	boundarylen=charstring::length(boundary);

	// create a buffer and copy the boundary into the buffer,
	// preceeded by \r\n--
	char	*altboundary=new char[boundarylen+5];
	charstring::copy(altboundary,"\r\n--");
	charstring::append(altboundary,boundary);
	size_t	altboundarylen=boundarylen+4;
	
	// create a ring buffer large enough to store the altboundary
	ringbuffer	buffer(boundarylen+4);

	// some variables that we'll use later...
	stringbuffer	*name=NULL;
	stringbuffer	*filename=NULL;
	stringbuffer	*value=NULL;
	file		*tempfile=NULL;
	char		*tempfilename=NULL;
	stringbuffer	*mimetype=NULL;
	char		boundaryend[3];
	boundaryend[2]='\0';
	char		charbuf;

	for (;;) {

		// get a character
		if (!sapi->getCharacter(&charbuf)) {
			break;
		}

		// put it in the ring buffer
		buffer.append(charbuf);

		// if we found a boundary string, add the form or file entry
		if (buffer.compare(boundary,boundarylen) ||
			buffer.compare(altboundary,altboundarylen)) {

			// get the next 2 characters, if they're -- then
			// we're done, otherwise they're \r\n and we need to
			// add the parameter
			bool	finalboundary=false;
			if (!sapi->getCharacter(&(boundaryend[0])) ||
				!sapi->getCharacter(&(boundaryend[1])) ||
				!charstring::compare(boundaryend,"--")) {
				finalboundary=true;
			}

			// add the form or file entry
			if (value) {
				setParameter(name->getString(),
						value->getString());
				delete value;
			} else if (tempfile) {
				setFileParameter(name->getString(),
						filename->getString(),
						tempfilename,
						mimetype->getString());
				tempfile->close();
				delete tempfile;
				delete[] tempfilename;
			}
			delete name;
			delete filename;
			delete mimetype;

			// if we hit the final boundary, we're done...
			if (finalboundary) {
				return;
			}

			// clear the ring buffer
			buffer.clear();

			// start on the next entry
			getNewNames(&name,&filename,&mimetype);
			if (filename) {
				getTempFile(filename->getString(),
						&tempfile,
						&tempfilename);
				value=NULL;
			} else {
				value=new stringbuffer();
				tempfile=NULL;
			}

			continue;
		}

		// get the data
		if (buffer.isFull()) {
			charbuf=buffer.getHeadCharacter();
			if (value) {
				value->append(charbuf);
			} else if (tempfile) {
				tempfile->write(charbuf);
			}
		}
	}

	// clean up
	delete name;
	delete filename;
	delete value;
	delete[] altboundary;
}

void httprequest::getNewNames(stringbuffer **name, stringbuffer **filename, 
						stringbuffer **mimetype) {

	// get the new name and possibly filename
	*name=getName();
	char	charbuf;
	sapi->getCharacter(&charbuf);
	if (charbuf==';') {

		*filename=getFileName();
		sapi->getCharacter(&charbuf);

		*mimetype=getMimeType();
	} else {

		*filename=NULL;
		*mimetype=NULL;
	}

	// get the 2 \r\n's before the content
	// (we should already have the first \r)
	sapi->getCharacter(&charbuf);
	sapi->getCharacter(&charbuf);
	sapi->getCharacter(&charbuf);
}

void httprequest::getTempFile(const char *filename, file **tempfile,
						char **tempfilename) {

	char	slash[2];
	slash[0]=sys::getDirectorySeparator();
	slash[1]='\0';

	if (charstring::length(filename)) {
		// generate a unique temporary filename using mkstemp
		size_t	tempfilenamelen=tmpdirlen+1+
					charstring::length(filename)+8;
		*tempfilename=new char[tempfilenamelen];
		charstring::copy(*tempfilename,TMPDIR);
		charstring::append(*tempfilename,slash);
		charstring::append(*tempfilename,filename);
		charstring::append(*tempfilename,"-XXXXXX");
		*tempfile=new file;
		(*tempfile)->setFileDescriptor(
				file::createTemporaryFile(*tempfilename));
	} else {
		*tempfile=NULL;
		*tempfilename=NULL;
	}
}

stringbuffer *httprequest::getName() {
	return getSomeKindOfName('n');
}

stringbuffer *httprequest::getFileName() {
	return getSomeKindOfName('f');
}

stringbuffer *httprequest::getSomeKindOfName(char c) {

	char		charbuf;
	stringbuffer	*name=new stringbuffer();

	// skip to the "n" or "f" in (file)name="...";
	for (;;) {
		if (!sapi->getCharacter(&charbuf)) {
			return name;
		} else if (charbuf==c) {
			break;
		}
	}

	// skip to the first " in (file)name="...";
	for (;;) {
		if (!sapi->getCharacter(&charbuf)) {
			return name;
		} else if (charbuf=='"') {
			break;
		}
	}

	// get everything before the last " in (file)name="...";
	for (;;) {
		if (!sapi->getCharacter(&charbuf)) {
			return name;
		} else if (charbuf=='"') {
			break;
		} else {
			name->append(charbuf);
		}
	}

	return name;
}

stringbuffer *httprequest::getMimeType() {

	stringbuffer	*mimetype=new stringbuffer();

	// skip past "Content-type:"
	char	charbuf;
	for (;;) {
		if (!sapi->getCharacter(&charbuf) || charbuf==':') {
			break;
		}
	}

	// skip the space after "Content-type:"
	sapi->getCharacter(&charbuf);

	// get the mime type
	while (sapi->getCharacter(&charbuf) && charbuf!='\r') {
		mimetype->append(charbuf);
	}

	return mimetype;
}

bool httprequest::setFileParameter(const char *name,
						const char *filename, 
						const char *tempfilename,
						const char *mimetype) {
	dirtyfilenames=true;
	fileparameter	*data;
	if (!fileparameters.getValue((char *)name,&data)) {
		fileparameters.setValue(charstring::duplicate(name),
			new fileparameter(name,filename,tempfilename,mimetype));
		return true;
	}
	return false;
}

const char *httprequest::getParameter(const char *name) {
	return parameters.getValue((char *)name);
}

void httprequest::getParametersAsGetString(output *out,
					const char * const *exceptions) {

	// start off the string with the ?
	out->write('?');

	// set up some helpful variables
	bool	writeentry;
	bool	first=true;

	// run through the paramters queue
	for (linkedlistnode<namevaluepairsnode *>
				*fenode=parameters.getList()->getFirst();
				fenode; fenode=fenode->getNext()) {

		char	*name=fenode->getValue()->getKey();
		char	*value=fenode->getValue()->getValue();

		// initialize the write entry flag
		writeentry=true;

		// run through the arguments list
		if (exceptions) {
			for (const char * const *e=exceptions; *e; e++) {

				// if we find the name of the current form
				// entry in the arguments list, don't write
				// it
				if (!charstring::compare(*e,name)) {
					writeentry=false;
					break;
				}
			}
		}

		// if it's OK to write, do so
		if (writeentry) {

			// if it's not the first iteration, write a & first
			if (first) {
				first=false;
			} else {
				out->write('&');
			}

			// write the name=value part
			out->write(name);
			out->write('=');

			// make sure to escape the value part
			char	*escapedvalue=charstring::httpEscape(value);
			out->write(escapedvalue);
			delete[] escapedvalue;
		}

	}
}

void httprequest::getParametersAsHiddenVariables(output *out,
					const char * const *exceptions) {

	// set up some helpful variables
	bool	writeentry;

	// run through the parameters queue
	for (linkedlistnode<namevaluepairsnode *>
				*fenode=parameters.getList()->getFirst();
				fenode; fenode=fenode->getNext()) {

		char	*name=fenode->getValue()->getKey();
		char	*value=fenode->getValue()->getValue();

		// initialize the write entry flag
		writeentry=true;

		// run through the arguments list
		if (exceptions) {
			for (const char * const *e=exceptions; *e; e++) {

				// if we find the name of the current form
				// entry in the arguments list, don't write
				// it
				if (!charstring::compare(*e,name)) {
					writeentry=false;
					break;
				}
			}
		}

		// if it's OK to write, do so
		if (writeentry) {

			out->write("<input type=\"hidden\" name=\"");
			out->write(name);
			out->write("\" value=\"");
			out->write(value);
			out->write("\">");
		}
	}
}

uint64_t httprequest::getParameterCount() {
	return parameters.getList()->getLength();
}

const char * const *httprequest::getParameterVariables() {
	buildParameterList();
	return parametervars;
}

const char * const *httprequest::getParameterValues() {
	buildParameterList();
	return parametervals;
}

void httprequest::buildParameterList() {
	if (!parametervals || dirtyparameters) {
		buildList(&parametervars,&parametervals,&parameters);
		dirtyparameters=false;
	}
}

const char *httprequest::getFileParameterFilename(const char *name) {
	fileparameter	*value;
	return (fileparameters.getValue((char *)name,&value))?
						value->filename:NULL;
}

const char *httprequest::getFileParameterTempFilename(const char *name) {
	fileparameter	*value;
	return (fileparameters.getValue((char *)name,&value))?
						value->tempfilename:NULL;
}

const char *httprequest::getFileParameterMimeType(const char *name) {
	fileparameter	*value;
	return (fileparameters.getValue((char *)name,&value))?
						value->mimetype:NULL;
}

uint64_t httprequest::getFileCount() {
	return fileparameters.getList()->getLength();
}

const char * const *httprequest::getFileNames() {
	buildFileNamesList();
	return filenames;
}

void httprequest::buildFileNamesList() {

	if (!filenames || dirtyfilenames) {

		delete[] filenames;

		uint64_t	length=fileparameters.getList()->getLength();

		filenames=new const char *[length+1];

		uint64_t	index=0;
		for (linkedlistnode<dictionarynode<char *, fileparameter *> *>
				*node=fileparameters.getList()->getFirst();
				node; node=node->getNext()) {
			filenames[index]=node->getValue()->getKey();
			index++;
		}
		filenames[index]=NULL;
	
		dirtyfilenames=false;
	}
}

void httprequest::initCookies() {

	dirtycookies=false;
	cookievars=NULL;
	cookievals=NULL;

	// run through string, unescaping as I go, creating cookie entries
	const char	*httpcookie=getEnvironmentVariable("HTTP_COOKIE");
	if (!charstring::isNullOrEmpty(httpcookie)) {

		// set up some buffers
		stringbuffer	name;
		stringbuffer	value;
		stringbuffer	*out=&name;
		char		sixteens;
		char		ones;
		int		index=0;

		while (httpcookie[index]) {

			if (httpcookie[index]==';') {
	
				// theres always a space or NULL after a ; 
				index++;
	
				// create new cookie entry
				setCookie(name.getString(),value.getString());
	
				// renew stringbuffers
				name.clear();
				value.clear();
	
				// reset out
				out=&name;
			} else if (httpcookie[index]=='=') {
				out=&value;
			} else if (httpcookie[index]=='+') {
				out->append(' ');
			} else if (httpcookie[index]=='%') {
				// convert hex to ascii
	
				// get next char in stream: sixteens place
				index++;
				sixteens=character::
						toUpperCase(httpcookie[index]);
				if (sixteens>='0' && sixteens<='9') {
					sixteens=(sixteens-48)*16;
				} else {
					sixteens=(sixteens-55)*16;
				}
	
				// get next char in stream: ones place
				index++;
	
				ones=character::toUpperCase(httpcookie[index]);
				if (ones>='0' && ones<='9') {
					ones=ones-48;
				} else {
					ones=ones-55;
				}
	
				out->append((char)(sixteens+ones));
			} else {
				out->append(httpcookie[index]);
			}

			index++;
		}

		// create last new cookie entry
		if (index>0) {
			setCookie(name.getString(),value.getString());
		}
	}
}

bool httprequest::setCookie(const char *name, const char *value) {
	dirtycookies=true;
	dirtyallvars=true;
	cookies.setValue(charstring::duplicate(name),
				charstring::duplicate(value));
	return true;
}

void httprequest::cleanCookies() {
	cleanUp(&cookievars,&cookievals,&cookies);
}

const char *httprequest::getCookie(const char *name) {
	return cookies.getValue((char *)name);
}

uint64_t httprequest::getCookieCount() {
	return cookies.getList()->getLength();
}

const char * const *httprequest::getCookieVariables() {
	buildCookieList();
	return cookievars;
}

const char * const *httprequest::getCookieValues() {
	buildCookieList();
	return cookievars;
}

void httprequest::buildCookieList() {
	if (!cookievars || dirtycookies) {
		buildList(&cookievars,&cookievals,&cookies);
		dirtycookies=false;
	}
}

uint64_t httprequest::getAllVariableCount() {
	buildAllVariables();
	return allvariablecount;
}

const char * const *httprequest::getAllVariables() {
	buildAllVariables();
	return allvars;
}

const char * const *httprequest::getAllValues() {
	buildAllVariables();
	return allvals;
}

void httprequest::buildAllVariables() {

	if (allvals && allvars && !dirtyallvars) {
		return;
	}

	// delete any existing array
	delete[] allvars;
	delete[] allvals;

	// create a new array
	allvariablecount=getEnvironmentVariableCount()+
					getParameterCount()+
					getCookieCount();
	allvars=new const char *[allvariablecount+1];
	allvals=new const char *[allvariablecount+1];

	uint64_t	index=0;

	// add environment variables
	for (uint64_t envind=0;
			envind<getEnvironmentVariableCount(); envind++) {
		allvars[index]=getEnvironmentVariables()[envind];
		allvals[index++]=getEnvironmentValues()[envind];
	}

	// add parameters
	for (linkedlistnode<namevaluepairsnode *>
				*fenode=parameters.getList()->getFirst();
				fenode; fenode=fenode->getNext()) {
		allvars[index]=fenode->getValue()->getKey();
		allvals[index++]=fenode->getValue()->getValue();
	}

	// add cookies
	for (linkedlistnode<namevaluepairsnode *>
				*cknode=cookies.getList()->getFirst();
				cknode; cknode=cknode->getNext()) {
		allvars[index]=cknode->getValue()->getKey();
		allvals[index++]=cknode->getValue()->getValue();
	}

	// terminate the array
	allvars[index]=NULL;
	allvals[index]=NULL;

	dirtyallvars=false;
}

bool httprequest::methodAllowed(const char *allowedmethods,
					const char *deniedmethods) {

	const char	*requestmethod=getEnvironmentVariable("REQUEST_METHOD");

	if ((!charstring::isNullOrEmpty(deniedmethods) &&
			regularexpression::match(requestmethod,
							deniedmethods)) &&
		!(!charstring::isNullOrEmpty(allowedmethods) && 
			regularexpression::match(requestmethod,
							allowedmethods))) {
		return false;
	}
	return true;
}

bool httprequest::ipAllowed(const char *allowedips,
					const char *deniedips) {

	const char	*remoteaddr=getEnvironmentVariable("REMOTE_ADDR");

	if ((!charstring::isNullOrEmpty(deniedips) &&
			regularexpression::match(remoteaddr,deniedips)) &&
		!(!charstring::isNullOrEmpty(allowedips) && 
			regularexpression::match(remoteaddr,allowedips))) {
		return false;
	}
	return true;
}

bool httprequest::refererAllowed(const char *allowedreferers,
					const char *deniedreferers) {

	const char	*httpreferer=getEnvironmentVariable("HTTP_REFERER");

	if ((!charstring::isNullOrEmpty(deniedreferers) &&
			regularexpression::match(httpreferer,
							deniedreferers)) &&
		!(!charstring::isNullOrEmpty(allowedreferers) && 
			regularexpression::match(httpreferer,
							allowedreferers))) {
		return false;
	}
	return true;
}

bool httprequest::requiredParameters(parameterrequirement **pr) {

	for (uint64_t index=0; pr[index]; index++) {
		const char	*fe=getParameter(pr[index]->variable);
		if (!fe || (fe && !(pr[index]->allowempty) && !fe[0])) {
			return false;
		}
	}
	return true;
}

const char *httprequest::getEnvironmentVariable(const char *name) {
	return sapi->getEnvironmentVariable(name);
}

uint64_t httprequest::getEnvironmentVariableCount() {
	return sapi->getEnvironmentVariableCount();
}

const char * const *httprequest::getEnvironmentVariables() {
	return sapi->getEnvironmentVariables();
}

const char * const *httprequest::getEnvironmentValues() {
	return sapi->getEnvironmentValues();
}

bool httprequest::setEnvironmentVariable(const char *name,
						const char *value) {
	return sapi->setEnvironmentVariable(name,value);
}

void httprequest::dumpEnvironment() {
	const char * const	*envvars=getEnvironmentVariables();
	const char * const	*envvals=getEnvironmentValues();
	for (uint64_t index=0;
		index<sapi->getEnvironmentVariableCount(); index++) {
		sapi->write(envvars[index],charstring::length(envvars[index]));
		sapi->write("=",1);
		sapi->write(envvals[index],charstring::length(envvals[index]));
		sapi->write("\n",1);
	}
}
