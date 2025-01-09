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
	// FIXME: do this with 2 bytestring::compare()s
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

class fileparameter {
	friend class httprequest;
	private:
			fileparameter(const char *name, const char *filename, 
				const char *tempfilename, const char *mimetype);
	public:
			~fileparameter();
	private:
		char	*name;
		char	*filename;
		char	*tempfilename;
		char	*mimetype;
	public:
		void	print() {}
};

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

class httprequestprivate {
	friend class httprequest;
	private:
		httpserverapi	*_sapi;

		dictionary<char *, char *>	_parameters;
		bool		_dirtyparameters;
		const char	**_parametervars;
		const char	**_parametervals;

		size_t		_tmpdirlen;
		const char	*_boundary;
		dictionary<char *, fileparameter *>	_fileparameters;
		bool		_dirtyfilenames;
		const char	**_filenames;

		dictionary<char *, char *>	_cookies;
		bool		_dirtycookies;
		const char	**_cookievars;
		const char	**_cookievals;

		bool		_dirtyallvars;
		uint64_t	_allvariablecount;
		const char	**_allvars;
		const char	**_allvals;
};

httprequest::httprequest(httpserverapi *sapi) : object() {
	pvt=new httprequestprivate;

	pvt->_sapi=sapi;

	// tempdir length
	pvt->_tmpdirlen=charstring::getLength(TMPDIR);

	// initialize some variables
	pvt->_boundary=NULL;

	pvt->_fileparameters.setManageArrayKeys(true);
	pvt->_fileparameters.setManageValues(true);

	pvt->_sapi->initEnvironmentVariables();

	pvt->_cookies.setManageArrayKeys(true);
	pvt->_cookies.setManageArrayValues(true);
	initCookies();

	pvt->_parameters.setManageArrayKeys(true);
	pvt->_parameters.setManageArrayValues(true);
	initParameters();

	initFileNames();

	pvt->_dirtyallvars=false;
	pvt->_allvars=NULL;
	pvt->_allvals=NULL;
	pvt->_allvariablecount=0;
}

httprequest::~httprequest() {
	delete[] pvt->_allvals;
	delete[] pvt->_allvars;

	cleanParameters();
	cleanFiles();
	cleanCookies();

	delete pvt;
}

void httprequest::cleanUp(const char ***vars, const char ***vals,
					dictionary<char *, char *> *nvp) {

	nvp->clear();
	delete[] (*vars);
	delete[] (*vals);
}

void httprequest::writeVariables() {

	pvt->_sapi->write("Environment Variables:\n");

	writeEnvironment();

	pvt->_sapi->write("\n");

	pvt->_sapi->write("Multipart Boundary:\n");

	if (pvt->_boundary) {
		pvt->_sapi->write("\"");
		pvt->_sapi->write(pvt->_boundary);
		pvt->_sapi->write("\"");
		pvt->_sapi->write("\n");
	}

	pvt->_sapi->write("\n");

	pvt->_sapi->write("Parameters:\n");

	// parameters
	for (listnode<char *> *fenode=pvt->_parameters.getKeys()->getFirst();
					fenode; fenode=fenode->getNext()) {
		char	*key=fenode->getValue();
		pvt->_sapi->write(key);
		pvt->_sapi->write("=");
		pvt->_sapi->write(pvt->_parameters.getValue(key));
		pvt->_sapi->write("\n");
	}

	pvt->_sapi->write("\n");

	pvt->_sapi->write("File Parameters:\n");

	// file parameters
	for (listnode<char *> *flnode=
				pvt->_fileparameters.getKeys()->getFirst();
				flnode; flnode=flnode->getNext()) {
		char		*key=flnode->getValue();
		fileparameter	*fp=pvt->_fileparameters.getValue(key);
		pvt->_sapi->write(key);
		pvt->_sapi->write("=");
		pvt->_sapi->write(fp->filename);
		pvt->_sapi->write(" : ");
		pvt->_sapi->write(fp->tempfilename);
		pvt->_sapi->write("\n");
	}

	pvt->_sapi->write("\n");

	pvt->_sapi->write("Cookies:\n");

	// cookies
	for (listnode<char *> *cknode=pvt->_cookies.getKeys()->getFirst();
					cknode; cknode=cknode->getNext()) {
		char	*key=cknode->getValue();
		pvt->_sapi->write(key);
		pvt->_sapi->write("=");
		pvt->_sapi->write(pvt->_cookies.getValue(key));
		pvt->_sapi->write("\n");
	}

	pvt->_sapi->write("\n");
}

void httprequest::buildList(const char ***vars, const char ***vals,
					dictionary<char *, char *> *nvp) {

	delete[] (*vars);
	delete[] (*vals);

	uint64_t	length=nvp->getCount();

	(*vars)=new const char *[length+1];
	(*vals)=new const char *[length+1];

	uint64_t	index=0;
	for (listnode<char *> *node=nvp->getKeys()->getFirst();
					node; node=node->getNext()) {
		char	*key=node->getValue();
		(*vars)[index]=key;
		(*vals)[index]=nvp->getValue(key);
		index++;
	}
	(*vars)[index]=NULL;
	(*vals)[index]=NULL;
}

void httprequest::removeTempFiles() {

	for (listnode<char *> *flnode=
				pvt->_fileparameters.getKeys()->getFirst();
				flnode; flnode=flnode->getNext()) {
		char		*key=flnode->getValue();
		fileparameter	*value=pvt->_fileparameters.getValue(key);
		file::remove(value->tempfilename);
	}
}

void httprequest::cleanParameters() {
	cleanUp(&(pvt->_parametervars),
			&(pvt->_parametervals),
			&(pvt->_parameters));
}

void httprequest::cleanFiles() {
	removeTempFiles();
	delete[] pvt->_filenames;
}

void httprequest::initParameters() {

	pvt->_dirtyparameters=false;
	pvt->_parametervars=NULL;
	pvt->_parametervals=NULL;

	// handle a get or a head
	const char	*requestmethod=getEnvironmentVariable("REQUEST_METHOD");
	const char	*contenttype=getEnvironmentVariable("CONTENT_TYPE");
	if (requestmethod &&
		(!charstring::compareIgnoringCase(requestmethod,"get") || 
		!charstring::compareIgnoringCase(requestmethod,"head"))) {

		parseQueryString(get_request);
	}

	// handle a post
	if (requestmethod &&
		!charstring::compareIgnoringCase(requestmethod,"post")) {

		// handle application/x-www-form-urlencoded
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
	pvt->_dirtyfilenames=false;
	pvt->_filenames=NULL;
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
	size_t		contentlength=
				charstring::convertToUnsignedInteger(contentlengthstr);
	if (method==get_request || method==head_request) {
		if (!charstring::isNullOrEmpty(querystring)) {
			length=charstring::getLength(querystring);
		} else {
			length=contentlength;
		}
	} else {
		if (contentlength) {
			length=contentlength;
		} else if (!charstring::isNullOrEmpty(querystring)) {
			length=charstring::getLength(querystring);
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
			pvt->_sapi->read(&charbuf);
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
				pvt->_sapi->read(&charbuf);
			}

			sixteens=character::upper(charbuf);
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
				pvt->_sapi->read(&charbuf);
			}

			ones=character::upper(charbuf);
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
	pvt->_dirtyparameters=true;
	pvt->_dirtyallvars=true;
	pvt->_parameters.setValue(charstring::duplicate(name),
					charstring::duplicate(value));
	return true;
}

void httprequest::parseMultipart() {

	// get the boundary and its length
	pvt->_boundary=charstring::findFirst(
				getEnvironmentVariable("CONTENT_TYPE"),
				"boundary=")+9;
	size_t	boundarylen=charstring::getLength(pvt->_boundary);

	// create a buffer and copy the boundary into the buffer,
	// preceeded by \r\n--
	char	*altboundary=new char[boundarylen+5];
	charstring::copy(altboundary,"\r\n--");
	charstring::append(altboundary,pvt->_boundary);
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
		if (pvt->_sapi->read(&charbuf)!=sizeof(char)) {
			break;
		}

		// put it in the ring buffer
		buffer.append(charbuf);

		// if we found a boundary string, add the parameter
		if (buffer.compare(pvt->_boundary,boundarylen) ||
			buffer.compare(altboundary,altboundarylen)) {

			// get the next 2 characters, if they're -- then
			// we're done, otherwise they're \r\n and we need to
			// add the parameter
			bool	finalboundary=false;
			if (pvt->_sapi->read(&(boundaryend[0]))!=
							sizeof(char) ||
				pvt->_sapi->read(&(boundaryend[1]))!=
							sizeof(char) ||
				!charstring::compare(boundaryend,"--")) {
				finalboundary=true;
			}

			// add the parameter
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

			// start on the next parameter
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

void httprequest::parseJsonOrXml() {
}

void httprequest::getNewNames(stringbuffer **name, stringbuffer **filename, 
						stringbuffer **mimetype) {

	// get the new name and possibly filename
	*name=getName();
	char	charbuf;
	pvt->_sapi->read(&charbuf);
	if (charbuf==';') {

		*filename=getFileName();
		pvt->_sapi->read(&charbuf);

		*mimetype=getMimeType();
	} else {

		*filename=NULL;
		*mimetype=NULL;
	}

	// get the 2 \r\n's before the content
	// (we should already have the first \r)
	pvt->_sapi->read(&charbuf);
	pvt->_sapi->read(&charbuf);
	pvt->_sapi->read(&charbuf);
}

void httprequest::getTempFile(const char *filename, file **tempfile,
						char **tempfilename) {

	char	slash[2];
	slash[0]=sys::getDirectorySeparator();
	slash[1]='\0';

	if (charstring::getLength(filename)) {
		// generate a unique temporary filename using mkstemp
		size_t	tempfilenamelen=pvt->_tmpdirlen+1+
					charstring::getLength(filename)+8;
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
		if (pvt->_sapi->read(&charbuf)!=sizeof(char)) {
			return name;
		} else if (charbuf==c) {
			break;
		}
	}

	// skip to the first " in (file)name="...";
	for (;;) {
		if (pvt->_sapi->read(&charbuf)!=sizeof(char)) {
			return name;
		} else if (charbuf=='"') {
			break;
		}
	}

	// get everything before the last " in (file)name="...";
	for (;;) {
		if (pvt->_sapi->read(&charbuf)!=sizeof(char)) {
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
		if (pvt->_sapi->read(&charbuf)!=sizeof(char) || charbuf==':') {
			break;
		}
	}

	// skip the space after "Content-type:"
	pvt->_sapi->read(&charbuf);

	// get the mime type
	while (pvt->_sapi->read(&charbuf)==sizeof(char) && charbuf!='\r') {
		mimetype->append(charbuf);
	}

	return mimetype;
}

bool httprequest::setFileParameter(const char *name,
						const char *filename, 
						const char *tempfilename,
						const char *mimetype) {
	pvt->_dirtyfilenames=true;
	fileparameter	*data;
	if (!pvt->_fileparameters.getValue((char *)name,&data)) {
		pvt->_fileparameters.setValue(charstring::duplicate(name),
			new fileparameter(name,filename,tempfilename,mimetype));
		return true;
	}
	return false;
}

ssize_t	httprequest::read(byte_t *buffer, size_t size) {
	return pvt->_sapi->read(buffer,size);
}

ssize_t	httprequest::read(char *buffer, size_t length) {
	return pvt->_sapi->read(buffer,length);
}

ssize_t	httprequest::read(char *character) {
	return pvt->_sapi->read(character);
}

ssize_t	httprequest::read(wchar_t *buffer, size_t length) {
	return pvt->_sapi->read(buffer,length);
}

ssize_t	httprequest::read(wchar_t *character) {
	return pvt->_sapi->read(character);
}

ssize_t	httprequest::readUcs2(ucs2_t *buffer, size_t length) {
	return pvt->_sapi->readUcs2(buffer,length);
}

ssize_t	httprequest::readUcs2(ucs2_t *character) {
	return pvt->_sapi->readUcs2(character);
}

ssize_t	httprequest::read(int16_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(int32_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(int64_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(byte_t *character) {
	return pvt->_sapi->read(character);
}

ssize_t	httprequest::read(uint16_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(uint32_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(uint64_t *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(float *number) {
	return pvt->_sapi->read(number);
}

ssize_t	httprequest::read(double *number) {
	return pvt->_sapi->read(number);
}

const char *httprequest::getParameter(const char *name) {
	return pvt->_parameters.getValue((char *)name);
}

void httprequest::getParametersAsGetString(output *out,
					const char * const *exceptions) {

	// start off the string with the ?
	out->write('?');

	// set up some helpful variables
	bool	writeentry;
	bool	first=true;

	// run through the paramters queue
	for (listnode<char *> *fenode=pvt->_parameters.getKeys()->getFirst();
					fenode; fenode=fenode->getNext()) {

		char	*name=fenode->getValue();
		char	*value=pvt->_parameters.getValue(name);

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
			char	*escapedvalue=charstring::urlEncode(value);
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
	for (listnode<char *> *fenode=pvt->_parameters.getKeys()->getFirst();
					fenode; fenode=fenode->getNext()) {

		char	*name=fenode->getValue();
		char	*value=pvt->_parameters.getValue(name);

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
	return pvt->_parameters.getCount();
}

const char * const *httprequest::getParameterVariables() {
	buildParameterList();
	return pvt->_parametervars;
}

const char * const *httprequest::getParameterValues() {
	buildParameterList();
	return pvt->_parametervals;
}

void httprequest::buildParameterList() {
	if (!pvt->_parametervals || pvt->_dirtyparameters) {
		buildList(&(pvt->_parametervars),
				&pvt->_parametervals,
				&(pvt->_parameters));
		pvt->_dirtyparameters=false;
	}
}

const char *httprequest::getFileParameterFileName(const char *name) {
	fileparameter	*value;
	return (pvt->_fileparameters.getValue((char *)name,&value))?
						value->filename:NULL;
}

const char *httprequest::getFileParameterTempFileName(const char *name) {
	fileparameter	*value;
	return (pvt->_fileparameters.getValue((char *)name,&value))?
						value->tempfilename:NULL;
}

const char *httprequest::getFileParameterMimeType(const char *name) {
	fileparameter	*value;
	return (pvt->_fileparameters.getValue((char *)name,&value))?
						value->mimetype:NULL;
}

uint64_t httprequest::getFileCount() {
	return pvt->_fileparameters.getCount();
}

const char * const *httprequest::getFileNames() {
	buildFileNamesList();
	return pvt->_filenames;
}

void httprequest::buildFileNamesList() {

	if (!pvt->_filenames || pvt->_dirtyfilenames) {

		delete[] pvt->_filenames;

		uint64_t	length=pvt->_fileparameters.getCount();

		pvt->_filenames=new const char *[length+1];

		uint64_t	index=0;
		for (listnode<char *> *node=
				pvt->_fileparameters.getKeys()->getFirst();
				node; node=node->getNext()) {
			pvt->_filenames[index]=node->getValue();
			index++;
		}
		pvt->_filenames[index]=NULL;
	
		pvt->_dirtyfilenames=false;
	}
}

void httprequest::initCookies() {

	pvt->_dirtycookies=false;
	pvt->_cookievars=NULL;
	pvt->_cookievals=NULL;

	// run through string, unescaping as I go, creating cookies
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
	
				// create new cookie
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
				sixteens=character::upper(httpcookie[index]);
				if (sixteens>='0' && sixteens<='9') {
					sixteens=(sixteens-48)*16;
				} else {
					sixteens=(sixteens-55)*16;
				}
	
				// get next char in stream: ones place
				index++;
	
				ones=character::upper(httpcookie[index]);
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

		// create last new cookie
		if (index>0) {
			setCookie(name.getString(),value.getString());
		}
	}
}

bool httprequest::setCookie(const char *name, const char *value) {
	pvt->_dirtycookies=true;
	pvt->_dirtyallvars=true;
	pvt->_cookies.setValue(charstring::duplicate(name),
				charstring::duplicate(value));
	return true;
}

void httprequest::cleanCookies() {
	cleanUp(&(pvt->_cookievars),
			&(pvt->_cookievals),
			&(pvt->_cookies));
}

const char *httprequest::getCookie(const char *name) {
	return pvt->_cookies.getValue((char *)name);
}

uint64_t httprequest::getCookieCount() {
	return pvt->_cookies.getCount();
}

const char * const *httprequest::getCookieVariables() {
	buildCookieList();
	return pvt->_cookievars;
}

const char * const *httprequest::getCookieValues() {
	buildCookieList();
	return pvt->_cookievars;
}

void httprequest::buildCookieList() {
	if (!pvt->_cookievars || pvt->_dirtycookies) {
		buildList(&(pvt->_cookievars),
				&(pvt->_cookievals),
				&(pvt->_cookies));
		pvt->_dirtycookies=false;
	}
}

uint64_t httprequest::getAllVariableCount() {
	buildAllVariables();
	return pvt->_allvariablecount;
}

const char * const *httprequest::getAllVariables() {
	buildAllVariables();
	return pvt->_allvars;
}

const char * const *httprequest::getAllValues() {
	buildAllVariables();
	return pvt->_allvals;
}

void httprequest::buildAllVariables() {

	if (pvt->_allvals && pvt->_allvars && !pvt->_dirtyallvars) {
		return;
	}

	// delete any existing array
	delete[] pvt->_allvars;
	delete[] pvt->_allvals;

	// create a new array
	pvt->_allvariablecount=getEnvironmentVariableCount()+
						getParameterCount()+
						getCookieCount();
	pvt->_allvars=new const char *[pvt->_allvariablecount+1];
	pvt->_allvals=new const char *[pvt->_allvariablecount+1];

	uint64_t	index=0;

	// add environment variables
	for (uint64_t envind=0;
			envind<getEnvironmentVariableCount(); envind++) {
		pvt->_allvars[index]=getEnvironmentVariables()[envind];
		pvt->_allvals[index++]=getEnvironmentValues()[envind];
	}

	// add parameters
	for (listnode<char *> *fenode=pvt->_parameters.getKeys()->getFirst();
					fenode; fenode=fenode->getNext()) {
		char	*key=fenode->getValue();
		pvt->_allvars[index]=key;
		pvt->_allvals[index++]=pvt->_parameters.getValue(key);
	}

	// add cookies
	for (listnode<char *> *cknode=pvt->_cookies.getKeys()->getFirst();
					cknode; cknode=cknode->getNext()) {
		char	*key=cknode->getValue();
		pvt->_allvars[index]=key;
		pvt->_allvals[index++]=pvt->_cookies.getValue(key);
	}

	// terminate the array
	pvt->_allvars[index]=NULL;
	pvt->_allvals[index]=NULL;

	pvt->_dirtyallvars=false;
}

bool httprequest::isMethodAllowed(const char *deniedmethods,
					const char *allowedmethods) {
	return isThingAllowed(
			getEnvironmentVariable("REQUEST_METHOD"),true,
			deniedmethods,allowedmethods);
}

bool httprequest::isThingAllowed(const char *thing,
					bool allowemptything,
					const char *deniedthings,
					const char *allowedthings) {

	// if the thing itself is empty then we may just allow it
	if (allowemptything && charstring::isNullOrEmpty(thing)) {
		return true;
	}

	// if denied...
	if (!charstring::isNullOrEmpty(deniedthings) &&
			regularexpression::match(thing,deniedthings)) {
		return false;
	}

	// if allowed...
	if (!charstring::isNullOrEmpty(allowedthings) && 
			regularexpression::match(thing,allowedthings)) {
		return true;
	}
	return false;
}

bool httprequest::isContentTypeAllowed(
				const char *deniedcontenttypes,
				const char *allowedcontenttypes) {
	return isThingAllowed(getEnvironmentVariable("CONTENT_TYPE"),true,
					deniedcontenttypes,allowedcontenttypes);
}

bool httprequest::isIpAllowed(const char *deniedips,
					const char *allowedips) {
	return isThingAllowed(getEnvironmentVariable("REMOTE_ADDR"),false,
							deniedips,allowedips);
}

bool httprequest::isRefererAllowed(const char *deniedreferers,
					const char *allowedreferers) {
	return isThingAllowed(getEnvironmentVariable("HTTP_REFERER"),true,
						deniedreferers,allowedreferers);
}

bool httprequest::getRequiredParametersWereProvided(parameterrequirement **pr) {

	for (uint64_t index=0; pr[index]; index++) {
		const char	*fe=getParameter(pr[index]->variable);
		if (!fe || (fe && !(pr[index]->allowempty) && !fe[0])) {
			return false;
		}
	}
	return true;
}

const char *httprequest::getEnvironmentVariable(const char *name) {
	return pvt->_sapi->getEnvironmentVariable(name);
}

uint64_t httprequest::getEnvironmentVariableCount() {
	return pvt->_sapi->getEnvironmentVariableCount();
}

const char * const *httprequest::getEnvironmentVariables() {
	return pvt->_sapi->getEnvironmentVariables();
}

const char * const *httprequest::getEnvironmentValues() {
	return pvt->_sapi->getEnvironmentValues();
}

bool httprequest::setEnvironmentVariable(const char *name,
						const char *value) {
	return pvt->_sapi->setEnvironmentVariable(name,value);
}

void httprequest::writeEnvironment() {
	const char * const	*envvars=getEnvironmentVariables();
	const char * const	*envvals=getEnvironmentValues();
	for (uint64_t index=0;
		index<pvt->_sapi->getEnvironmentVariableCount(); index++) {
		pvt->_sapi->write(envvars[index],
					charstring::getLength(envvars[index]));
		pvt->_sapi->write("=",1);
		pvt->_sapi->write(envvals[index],
					charstring::getLength(envvals[index]));
		pvt->_sapi->write("\n",1);
	}
}

bool httprequest::dirtyAllVars() {
	return pvt->_dirtyallvars;
}

void httprequest::dirtyAllVars(bool dirtyallvars) {
	pvt->_dirtyallvars=dirtyallvars;
}

uint64_t httprequest::allVariableCount() {
	return pvt->_allvariablecount;
}

void httprequest::allVariableCount(uint64_t allvariablecount) {
	pvt->_allvariablecount=allvariablecount;
}

const char ***httprequest::allVars() {
	return &(pvt->_allvars);
}

const char ***httprequest::allVals() {
	return &(pvt->_allvals);
}
