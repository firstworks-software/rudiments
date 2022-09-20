// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/url.h>
#ifdef RUDIMENTS_HAS_LIBCURL
	#include <rudiments/bytebuffer.h>
	#include <rudiments/bytestring.h>
	#include <rudiments/threadmutex.h>
	#include <rudiments/snooze.h>
#endif
#include <rudiments/listener.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/datetime.h>
#include <rudiments/sensitivevalue.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

// Solaris 11 needs this for FD_ZERO
#if defined(RUDIMENTS_HAS_LIBCURL) && defined(RUDIMENTS_HAVE_STRING_H)
	#include <string.h>
#endif

//#define DEBUG_CURL 1
//#define DEBUG_HTTP 1

#ifdef RUDIMENTS_HAS_LIBCURL
	#include <curl/curl.h>

	threadmutex	_urlmutex;
	bool		_initialized=false;
#endif

#define MAX_HEADER_SIZE 16*1024

class urlprivate {
	friend class url;
	private:
		bool			_usingbuiltin;
		inetsocketclient	_isc;

		stringbuffer		*_request;

		uint64_t		_contentlength;
		uint64_t		_fetchedsofar;

		bool			_chunked;
		bool			_bof;
		ssize_t			_chunksize;
		ssize_t			_chunkpos;

		bool			_keepalive;
		char			*_previousproto;
		char			*_httpprevioushost;
		char			*_httppreviousport;
		char			*_httpuseragent;
		char			*_httpheaders;

		bool			_usehttppost;
		const char		*_httppostcontenttype;
		const char		*_httppostdata;
		uint64_t		_httppostdatasize;

		#ifdef RUDIMENTS_HAS_LIBCURL
			CURL	*_curl;
			CURLM	*_curlm;
			bytebuffer	_bb;
			//unsigned char	_b[CURL_MAX_WRITE_SIZE];
			uint64_t	_breadpos;
			bool		_eof;
			int32_t		_stillrunning;
			listener	_l;
		#endif

		bool		_opentimings;
		uint64_t	_openbuild;
		uint64_t	_openconnect;
		uint64_t	_opensend;
		uint64_t	_openwait;
		uint64_t	_openrecv;

		datetime	_start;
		datetime	_end;

		stringbuffer	_error;
};

url::url() : file() {

	pvt=new urlprivate;
	dontGetCurrentPropertiesOnOpen();

	pvt->_isc.allowShortReads();
	pvt->_isc.setWriteBufferSize(65536);

	pvt->_request=NULL;

	pvt->_keepalive=false;
	pvt->_previousproto=NULL;
	pvt->_httpprevioushost=NULL;
	pvt->_httppreviousport=NULL;
	pvt->_httpuseragent=charstring::duplicate(
				"rudiments/" RUDIMENTS_VERSION);
	pvt->_httpheaders=NULL;

	pvt->_usehttppost=false;
	pvt->_httppostcontenttype="application/x-www-form-urlencoded";
	pvt->_httppostdata=NULL;
	pvt->_httppostdatasize=0;

	#ifdef RUDIMENTS_HAS_LIBCURL
	pvt->_curl=NULL;
	pvt->_curlm=NULL;
	#endif

	pvt->_opentimings=false;
	pvt->_openbuild=0;
	pvt->_openconnect=0;
	pvt->_opensend=0;
	pvt->_openwait=0;
	pvt->_openrecv=0;

	init();
	type("url");
	setIsStream(true);

	winsock::initWinsock();
}

url::~url() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class.  If close() is called here, it will
	// eventually call this method's lowLevelClose() rather than
	// filedescriptor::lowLevelClose().  Also, lowLevelClose() needs to
	// access pvt->_curl or pvt->_isc so we need to call close(), before
	// deleting pvt.
	close();

	delete[] pvt->_previousproto;
	delete[] pvt->_httpprevioushost;
	delete[] pvt->_httppreviousport;
	delete[] pvt->_httpuseragent;
	delete[] pvt->_httpheaders;

	delete pvt->_request;

	delete pvt;
}

void url::init() {
	pvt->_usingbuiltin=true;

	pvt->_contentlength=0;
	pvt->_fetchedsofar=0;

	pvt->_bof=false;
	pvt->_chunked=false;
	pvt->_chunksize=0;
	pvt->_chunkpos=0;

	#ifdef RUDIMENTS_HAS_LIBCURL
	pvt->_bb.clear();
	pvt->_breadpos=0;
	pvt->_eof=false;
	pvt->_stillrunning=0;
	#endif

	pvt->_error.clear();
}

void url::useHttpGet() {
	pvt->_usehttppost=false;
}

void url::useHttpPost() {
	pvt->_usehttppost=true;
}

void url::setHttpPostContentType(const char *contenttype) {
	if (contenttype) {
		pvt->_httppostcontenttype=contenttype;
	} else {
		pvt->_httppostcontenttype="application/x-www-form-urlencoded";
	}
}

void url::setHttpPostData(const char *data, uint64_t size) {
	pvt->_httppostdata=data;
	pvt->_httppostdatasize=size;
}

void url::setHttpUserAgent(const char *useragent) {
	delete[] pvt->_httpuseragent;
	if (useragent) {
		pvt->_httpuseragent=charstring::duplicate(useragent);
	} else {
		pvt->_httpuseragent=charstring::duplicate(
					"rudiments/" RUDIMENTS_VERSION);
	}
}

void url::setHttpHeaders(const char *headers) {
	delete[] pvt->_httpheaders;
	pvt->_httpheaders=charstring::duplicate(headers);
}

bool url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	// timings
	if (pvt->_opentimings) {
		pvt->_openbuild=0;
		pvt->_openconnect=0;
		pvt->_opensend=0;
		pvt->_openwait=0;
		pvt->_openrecv=0;
		pvt->_start.initFromSystemDateTime();
	}

	// skip leading whitespace
	while (*name && character::isWhitespace(*name)) {
		name++;
	}

	// disable keepalive if protocol changed
	const char	*protodelim=charstring::findFirst(name,"://");
	char		*proto=NULL;
	if (protodelim) {
		proto=charstring::duplicate(name,protodelim-name);
		if (pvt->_keepalive &&
			charstring::compare(proto,pvt->_previousproto)) {
			pvt->_keepalive=false;
		}
	}

	// clean up from a previous run
	if (!pvt->_keepalive) {
		close();
	}
	init();

	// reset previous protocol
	delete[] pvt->_previousproto;
	pvt->_previousproto=proto;

	// for now, don't support create or write
	if (perms || useperms || flags&O_WRONLY || flags&O_RDWR) {
		return false;
	}

	// don't support local files
	if (!protodelim || !charstring::compare(name,"file://",7)) {
		return false;
	}

	// init the framework, if necessary
	if (!initUrl()) {
		return false;
	}

	// extract user, password, and combined userpwd from url...
	char	*userpwd=NULL;
	char	*cleanurl=NULL;

	// is there an @ in the url? (between the first :// and subsequent /)
	const char	*nextslash=charstring::findFirstOrEnd(protodelim+3,'/');
	const char	*at=charstring::findFirst(protodelim+3,'@');
	if (at && at<nextslash) {

		// extract the user/password block
		userpwd=charstring::duplicate(protodelim+3,at-protodelim-3);

		// handle password files
		sensitivevalue	sv;
		sv.setChompTextFile(true);
		sv.parse(userpwd);
		delete[] userpwd;
		userpwd=sv.detachTextValue();

		// build a clean url, without the user/password in it
		cleanurl=new char[charstring::length(name)+1];
		charstring::copy(cleanurl,name,protodelim+3-name);
		charstring::append(cleanurl,at+1);

	} else {
		cleanurl=charstring::duplicate(name);
	}

	// open...
	bool	retval=false;
	bool	usehttp=!charstring::compare(cleanurl,"http://",7);
	if ((usehttp && httpOpen(cleanurl,userpwd)) ||
			curlOpen(cleanurl,userpwd)) {
		retval=true;
	} else {
		close();
	}

	delete[] cleanurl;
	delete[] userpwd;

	return retval;
}

bool url::httpOpen(const char *urlname, char *userpwd) {

	// parse out the host, port and path
	const char	*protodelim=charstring::findFirst(urlname,"://");
	const char	*path=charstring::findFirstOrEnd(protodelim+3,'/');
	char		*host=charstring::duplicate(protodelim+3,
							path-protodelim-3);
	const char	*port="80";
	char		*colon=charstring::findFirst(host,':');
	if (colon) {
		port=colon+1;
		*colon='\0';
	}

	#ifdef DEBUG_HTTP
	stdoutput.printf("url: %s\n",urlname),
	stdoutput.printf("host: %s\n",host),
	stdoutput.printf("port: %s\n",port);
	stdoutput.printf("previous host: %s\n",pvt->_httpprevioushost);
	stdoutput.printf("previous port: %s\n",pvt->_httppreviousport);
	stdoutput.printf("userpwd: %s\n",userpwd);
	stdoutput.printf("useragent: \"%s\"\n",pvt->_httpuseragent);
	stdoutput.printf("keepalive: %d\n\n",pvt->_keepalive);
	#endif

	// split headers
	dictionary<char *,const char *>	headerdict;
	headerdict.setManageArrayKeys(true);
	#ifdef DEBUG_HTTP
	stdoutput.printf("headers:\n");
	#endif
	char		**headers;
	uint64_t	headercount;
	charstring::split(pvt->_httpheaders,"\n",true,&headers,&headercount);
	for (uint64_t i=0; i<headercount; i++) {

		charstring::bothTrim(headers[i],'\r');
		charstring::bothTrim(headers[i],'\n');

		char	*headerkey=headers[i];
		char	*headervalue=NULL;
		char	*colon=charstring::findFirst(headers[i],":");
		if (colon) {
			*colon='\0';
			do {
				colon++;
			} while (*colon && character::isWhitespace(*colon));
			headervalue=colon;
		}

		headerdict.setValue(headerkey,headervalue);
	}
	delete[] headers;

	// connect to the host, or reuse existing connection, if possible...
	if (!pvt->_keepalive ||
		(charstring::compare(host,pvt->_httpprevioushost) &&
		charstring::compare(port,pvt->_httppreviousport))) {

		// timings
		if (pvt->_opentimings) {
			pvt->_end.initFromSystemDateTime();
			pvt->_openbuild+=interval();
			pvt->_start.initFromSystemDateTime();
		}

		// close any connection left open by a previous keepalive
		// (if we're not doing keepalive then open() will already
		// have called close(), so we don't need to do it here)
		if (pvt->_keepalive) {
			close();
		}

		// connect to the host
		#ifdef DEBUG_HTTP
		stdoutput.printf("Connecting...\n\n");
		#endif
		if (!pvt->_isc.connect(host,charstring::toUnsignedInteger(port),
								-1,-1,0,0)) {
			pvt->_error.append("http - connect failed: ");
			char	*err=error::getErrorString();
			pvt->_error.append(err);
			delete[] err;
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			delete[] host;
			return false;
		}

		// timings
		if (pvt->_opentimings) {
			pvt->_end.initFromSystemDateTime();
			pvt->_openconnect+=interval();
			pvt->_start.initFromSystemDateTime();
		}

	} else {
		#ifdef DEBUG_HTTP
		stdoutput.printf("Reusing existing connection...\n\n");
		#endif
	}

	// reset keepalive
	bool	reused=pvt->_keepalive;
	pvt->_keepalive=false;

	// finagle path
	if (charstring::isNullOrEmpty(path)) {
		path="/";
	}

	// build the request
	if (!pvt->_request) {
		pvt->_request=new stringbuffer();
	}
	pvt->_request->clear();

	pvt->_request->append((pvt->_usehttppost)?"POST ":"GET ")->append(path);
	pvt->_request->append(" HTTP/1.1\r\n");

	// user agent (if provided)
	const char	*useragent=headerdict.getValue((char *)"User-Agent");
	if (useragent) {
		pvt->_request->append("User-Agent: ");
		pvt->_request->append(useragent);
		pvt->_request->append("\r\n");
		headerdict.remove((char *)"User-Agent");
	} else {
		pvt->_request->append("User-Agent: ");
		pvt->_request->append(pvt->_httpuseragent);
		pvt->_request->append("\r\n");
	}

	// host
	const char	*hostval=headerdict.getValue((char *)"Host");
	if (hostval) {
		pvt->_request->append("Host: ");
		pvt->_request->append(hostval);
		pvt->_request->append("\r\n");
		headerdict.remove((char *)"Host");
	} else {
		pvt->_request->append("Host: ");
		pvt->_request->append(host);
		pvt->_request->append("\r\n");
	}

	// authorization
	const char	*auth=headerdict.getValue((char *)"Authorization");
	if (auth) {
		pvt->_request->append("Authorization: Basic ");
		pvt->_request->append(auth);
		pvt->_request->append("\r\n");
		headerdict.remove((char *)"Authorization");
	} else if (userpwd) {
		pvt->_request->append("Authorization: Basic ");
		char	*userpwd64=charstring::base64Encode(
				(const unsigned char *)userpwd);
		pvt->_request->append(userpwd64);
		delete[] userpwd64;
		pvt->_request->append("\r\n");
	}

	// accept
	const char	*accept=headerdict.getValue((char *)"Accept");
	if (accept) {
		pvt->_request->append("Accept: ");
		pvt->_request->append(accept);
		pvt->_request->append("\r\n");
		headerdict.remove((char *)"Accept");
	} else {
		pvt->_request->append("Accept: */*\r\n");
	}

	// content-type (allow httppost data to override)
	const char	*ctype=headerdict.getValue((char *)"Content-Type");
	if (ctype) {
		if (!pvt->_usehttppost) {
			pvt->_request->append("Content-Type: ");
			pvt->_request->append(ctype);
			pvt->_request->append("\r\n");
		}
		headerdict.remove((char *)"Content-Type");
	}

	// content-length (allow httppost data to override)
	const char	*clen=headerdict.getValue((char *)"Content-Length");
	if (clen) {
		if (!pvt->_usehttppost) {
			pvt->_request->append("Content-Length: ");
			pvt->_request->append(clen);
			pvt->_request->append("\r\n");
		}
		headerdict.remove((char *)"Content-Length");
	}

	// content-type/length from httppost data
	if (pvt->_usehttppost) {
		pvt->_request->append("Content-Type: ");
		pvt->_request->append(pvt->_httppostcontenttype);
		pvt->_request->append("\r\n");
		pvt->_request->append("Content-Length: ");
		pvt->_request->append(pvt->_httppostdatasize);
		pvt->_request->append("\r\n");
	}

	// send the rest of the headers...
	for (listnode<char *> *node=headerdict.getKeys()->getFirst();
						node; node=node->getNext()) {
		pvt->_request->append(node->getValue());
		pvt->_request->append(": ");
		pvt->_request->append(headerdict.getValue(node->getValue()));
		pvt->_request->append("\r\n");
	}
	headerdict.clear();

	// for http post, add an additional line break
	// to divide headers and post data
	if (pvt->_usehttppost) {
		pvt->_request->append("\r\n");
	}

	// timings
	if (pvt->_opentimings) {
		pvt->_end.initFromSystemDateTime();
		pvt->_openbuild+=interval();
		pvt->_start.initFromSystemDateTime();
	}

	// send the request
	#ifdef DEBUG_HTTP
	stdoutput.printf("Request:\n%s",pvt->_request->getString());
	if (pvt->_usehttppost) {
		stdoutput.write(pvt->_httppostdata,pvt->_httppostdatasize);
		stdoutput.write("\r\n");
	}
	stdoutput.write("\r\n");
	#endif
	ssize_t	requestlength=pvt->_request->getStringLength();
	if (pvt->_isc.write(pvt->_request->getString(),
					requestlength)!=requestlength ||
		((pvt->_usehttppost)?
			(pvt->_isc.write(pvt->_httppostdata,
					pvt->_httppostdatasize)!=
					(ssize_t)pvt->_httppostdatasize ||
			pvt->_isc.write("\r\n",2)!=2):false) ||
		pvt->_isc.write("\r\n",2)!=2 ||
		!pvt->_isc.flushWriteBuffer(-1,-1)) {

		pvt->_error.append("http - send request failed:");
		char	*err=error::getErrorString();
		pvt->_error.append(err);
		delete[] err;
		#ifdef DEBUG_HTTP
		stdoutput.printf("%s\n",pvt->_error.getString());
		#endif
		delete[] host;

		if (reused) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http - retrying "
					"without keep-alive\n");
			#endif
			close();
			return httpOpen(urlname,userpwd);
		} else {
			return false;
		}
	}

	// timings
	if (pvt->_opentimings) {
		pvt->_end.initFromSystemDateTime();
		pvt->_opensend+=interval();

		pvt->_start.initFromSystemDateTime();
		listener	lsnr;
		lsnr.addReadFileDescriptor(this);
		lsnr.listen();
		pvt->_end.initFromSystemDateTime();
		pvt->_openwait+=interval();

		pvt->_start.initFromSystemDateTime();
	}

	// fetch and process the headers
	bool	retval=false;
	pvt->_contentlength=0;
	pvt->_chunked=false;
	pvt->_bof=true;
	char	*location=NULL;
	bool	connectionclose=false;
	#ifdef DEBUG_HTTP
	stdoutput.printf("Response Headers:\n");
	#endif
	for (;;) {

		char	*header=NULL;
		ssize_t	result=pvt->_isc.read(&header,"\r\n",MAX_HEADER_SIZE);
		if (reused && result<1) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http - fetch headers failed\n");
			stdoutput.printf("http - retrying "
					"without keep-alive\n");
			#endif
			delete[] location;
			delete[] header;
			delete[] host;
			close();

			// timings
			if (pvt->_opentimings) {
				pvt->_end.initFromSystemDateTime();
				pvt->_openrecv+=interval();
				pvt->_start.initFromSystemDateTime();
			}

			return httpOpen(urlname,userpwd);
		}

		if (result<2) {
			pvt->_error.append("http - fetch headers failed: ");
			char	*err=error::getErrorString();
			pvt->_error.append(err);
			delete[] err;
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			delete[] location;
			delete[] header;
			delete[] host;
			return false;
		}

		#ifdef DEBUG_HTTP
		stdoutput.write(header);
		#endif

		// look for individual headers...
		if (!charstring::compare(header,
					"Content-Length: ",16)) {
			pvt->_contentlength=charstring::toInteger(header+16);
			pvt->_chunked=false;
			retval=true;
		} else if (!charstring::compare(header,
					"Transfer-Encoding: chunked",26)) {
			pvt->_contentlength=0;
			pvt->_chunked=true;
			retval=true;
		} else if (!charstring::compare(header,"Location: ",10)) {
			const char	*loc=header+10;
			if (charstring::findFirst(loc,"://")) {
				// handle absolute urls
				location=charstring::duplicate(header+10);
			} else {
				// handle relative urls
				stringbuffer	locstr;
				locstr.append("http://");
				locstr.append(host)->append(':')->append(port);
				if (loc[0]!='/') {
					locstr.append('/');
				}
				locstr.append(loc);
				location=locstr.detachString();
			}
			charstring::rightTrim(location);
		} else if (!charstring::compare(header,
					"Connection: close",17)) {
			connectionclose=true;
		}

		if (!charstring::compare(header,"\r\n")) {
			delete[] header;
			break;
		}
		delete[] header;
	}

	// keep-alive is the default in HTTP/1.1, so we'll reenable it
	// unless the server explicitly requested "Connection: close"
	if (!connectionclose) {
		pvt->_keepalive=true;
		delete[] pvt->_httpprevioushost;
		pvt->_httpprevioushost=charstring::duplicate(host);
		delete[] pvt->_httppreviousport;
		pvt->_httppreviousport=charstring::duplicate(port);
	}

	#ifdef DEBUG_HTTP
	if (pvt->_contentlength) {
		stdoutput.printf("http - content length: %lld\n",
						pvt->_contentlength);
	}
	if (pvt->_chunked) {
		stdoutput.printf("http - chunked encoding\n");
	}
	if (location) {
		stdoutput.printf("http - location: %s\n",location);
	}
	#endif

	// if we got a location header, then try to open that
	if (location) {
		pvt->_isc.close();
		retval=httpOpen(location,userpwd);
		delete[] location;
	}

	// otherwise, handle errors
	else if (!retval) {
		pvt->_error.append("http - fetch headers failed: no "
					"\"Content-Length\", "
					"\"Transfer-Encoding: chunked\", or "
					"\"Location\" header found");
		#ifdef DEBUG_HTTP
		stdoutput.printf("%s\n",pvt->_error.getString());
		#endif
	}

	// timings
	if (pvt->_opentimings) {
		pvt->_end.initFromSystemDateTime();
		pvt->_openrecv+=interval();
	}

	delete[] host;

	// set file descriptor on success
	if (retval) {
		fd(pvt->_isc.getFileDescriptor());
	}

	#ifdef DEBUG_HTTP
	if (retval) {
		stdoutput.printf("open succeeded, fd: %d\n",
						getFileDescriptor());
	} else {
		stdoutput.printf("open failed\n");
	}
	#endif

	return retval;
}

bool url::curlOpen(const char *urlname, char *userpwd) {

	bool	retval=false;

	#ifdef RUDIMENTS_HAS_LIBCURL

	// not using built-in protocol handler
	pvt->_usingbuiltin=false;

	// clear any existing errors
	error::clearError();

	// split to separate user/password if necessary
	#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
	char	*user=NULL;
	char	*password=NULL;
	if (userpwd) {
		user=userpwd;
		password=charstring::findFirst(userpwd,':');
		if (password) {
			password++;
			*(password-1)='\0';
		}
	}
	#endif

	#ifdef DEBUG_CURL
	stdoutput.printf("url: \"%s\"\n",urlname);
	stdoutput.printf("userpwd: \"%s\"\n",userpwd);
	#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
	stdoutput.printf("user: \"%s\"\n",user);
	stdoutput.printf("password: \"%s\"\n",password);
	#endif
	stdoutput.printf("useragent: \"%s\"\n",pvt->_httpuseragent);
	#endif

	// detect some specific protocols
	bool	ishttp=!charstring::compare(urlname,"https://",8);
	bool	isssh=!charstring::compare(urlname,"ssh://",6);

	// split http headers if necessary
	struct curl_slist	*headerlist=NULL;
	if (ishttp && pvt->_httpheaders) {
		#ifdef DEBUG_CURL
		stdoutput.printf("headers:\n");
		#endif
		char		**headers;
		uint64_t	headercount;
		charstring::split(pvt->_httpheaders,"\n",true,
					&headers,&headercount);
		for (uint64_t i=0; i<headercount; i++) {
			charstring::bothTrim(headers[i],'\r');
			charstring::bothTrim(headers[i],'\n');
			#ifdef DEBUG_CURL
			stdoutput.printf("%s\n",headers[i]);
			#endif
			headerlist=curl_slist_append(
					headerlist,headers[i]);
			delete[] headers[i];
		}
		delete[] headers;
	}

	bool	success=true;
	// init this curl instance
	pvt->_curl=curl_easy_init();
	if (!pvt->_curl) {
		success=false;
	}

	// set up debug
	#ifdef DEBUG_CURL
	if (success && curl_easy_setopt(pvt->_curl,
				CURLOPT_VERBOSE,1L)!=CURLE_OK) {
		stdoutput.printf("curl debug failed\n");
		success=false;
	}
	#endif

	// set up the url to open
	if (success && curl_easy_setopt(pvt->_curl,
				CURLOPT_URL,urlname)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set url failed\n");
		#endif
		success=false;
	}

	#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
	// set the user
	if (success && user && curl_easy_setopt(pvt->_curl,
				CURLOPT_USERNAME,user)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set username failed\n");
		#endif
		success=false;
	}
	
	// set the password
	if (success && password && curl_easy_setopt(pvt->_curl,
				CURLOPT_PASSWORD,password)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set password failed\n");
		#endif
		success=false;
	}

	#elif defined(RUDIMENTS_HAS_CURLOPT_USERPWD)
	// set the user/password
	if (success && userpwd && curl_easy_setopt(pvt->_curl,
				CURLOPT_USERPWD,userpwd)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set userpwd failed\n");
		#endif
		success=false;
	}
	#endif

	if (success && isssh) {

		// if a password is supplied, then use password
		// authentication for ssh protocols, or otherwise
		// allow curl to choose an appropriate auth type
		#ifdef RUDIMENTS_HAS_CURLOPT_SSH_AUTH_TYPES
			if (
			#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
				password &&
			#elif defined(RUDIMENTS_HAS_CURLOPT_USERPWD)
				userpwd &&
			#endif
				curl_easy_setopt(pvt->_curl,
					CURLOPT_SSH_AUTH_TYPES,
					CURLSSH_AUTH_PASSWORD)!=CURLE_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl set ssh "
						"auth type password failed\n");
				#endif
				success=false;
			} else if (curl_easy_setopt(pvt->_curl,
					CURLOPT_SSH_AUTH_TYPES,
					CURLSSH_AUTH_ANY)!=CURLE_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl set ssh "
						"auth type all failed\n");
				#endif
				success=false;
			}
		#endif
	}

	if (ishttp) {

		// set the user agent
		if (success && curl_easy_setopt(pvt->_curl,
					CURLOPT_USERAGENT,
					pvt->_httpuseragent)!=CURLE_OK) {
			#ifdef DEBUG_CURL
			stdoutput.printf("curl set user agent failed\n");
			#endif
			success=false;
		}

		// set post data
		if (pvt->_usehttppost) {

			// use post
			if (success && curl_easy_setopt(pvt->_curl,
						CURLOPT_POST,1L)!=CURLE_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl set use post failed\n");
				#endif
				success=false;
			}

			// post data size
			if (success && curl_easy_setopt(pvt->_curl,
					CURLOPT_POSTFIELDSIZE,
					pvt->_httppostdatasize)!=CURLE_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl set post "
							"data size failed\n");
				#endif
				success=false;
			}

			// post data
			if (success && curl_easy_setopt(pvt->_curl,
					CURLOPT_POSTFIELDS,
					pvt->_httppostdata)!=CURLE_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl set post "
							"data failed\n");
				#endif
				success=false;
			}

			// post header
			if (success) {
				stringbuffer	hdr;
				hdr.append("Content-Type: ");
				hdr.append(pvt->_httppostcontenttype);
				curl_slist_append(headerlist,hdr.getString());
			}
		}

		// set headers
		if (success && headerlist && curl_easy_setopt(pvt->_curl,
						CURLOPT_HTTPHEADER,
						headerlist)!=CURLE_OK) {
			#ifdef DEBUG_CURL
			stdoutput.printf("curl set header list failed\n");
			#endif
			success=false;
		}
	}

	// follow any Location headers we might get
	if (success && curl_easy_setopt(pvt->_curl,
				CURLOPT_FOLLOWLOCATION,1L)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set follow location failed\n");
		#endif
		success=false;
	}

	// set up write handler
	if (success && curl_easy_setopt(pvt->_curl,
				CURLOPT_WRITEFUNCTION,curlReadData)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set write function failed\n");
		#endif
		success=false;
	}
	if (success && curl_easy_setopt(pvt->_curl,
				CURLOPT_WRITEDATA,this)!=CURLE_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl set write data failed\n");
		#endif
		success=false;
	}

	// init the multi instance
	if (success) {
		pvt->_curlm=curl_multi_init();
		if (!pvt->_curlm) {
			#ifdef DEBUG_CURL
			stdoutput.printf("curl init multi instance failed\n");
			#endif
			success=false;
		}
	}

	// add the curl instance to the multi instance
	if (curl_multi_add_handle(pvt->_curlm,pvt->_curl)!=CURLM_OK) {
		#ifdef DEBUG_CURL
		stdoutput.printf("curl add multi instance failed\n");
		#endif
		success=false;
	}

	// connect
	if (success && curlPerform()) {

		// It's actually possible for all of this above
		// to succeed, but for the file descriptor to
		// still be -1.
		//
		// As soon as curl has read the entire file, it
		// closes the connection and invalidates the
		// descriptor.
		//
		// If the file is small enough, that'll all happen
		// during a single call to curl_multi_perform().
		//
		// I tried telling it to just connect and not read
		// anything, but even then, it appears to buffer the
		// data, close the connection and invalidate the
		// file descriptor anyway, it just doesn't call its
		// "I've got data callback" until the next call
		// to curl_multi_perform().
		//
		// Even when you can get a hold of it, the file
		// descriptor appears to be in non-blocking mode,
		// and setting it otherwise appears to break curl.
		//
		// It seems a bit of a ponderous heap, actually.
		// I'm sure there's some paradigm that it fits neatly
		// into, but definitely not into the unix "everything's
		// a file" or Windows "everything's a handle" paradigms.

		retval=true;

	} else {

		close();
	}

	curl_slist_free_all(headerlist);
	#endif

	#ifdef DEBUG_CURL
	if (retval) {
		stdoutput.printf("open succeeded, fd: %d\n",
						getFileDescriptor());
	} else {
		stdoutput.printf("open failed\n");
	}
	#endif

	return retval;
}

int32_t url::lowLevelClose() {

	int32_t	retval=0;
	if (pvt->_usingbuiltin) {
		retval=pvt->_isc.lowLevelClose();
		if (!retval) {
			pvt->_isc.setFileDescriptor(-1);
		}
	}
	#ifdef RUDIMENTS_HAS_LIBCURL
	else {
		if (pvt->_curlm && pvt->_curl) {
			curl_multi_remove_handle(pvt->_curlm,pvt->_curl);
		}
		if (pvt->_curl) {
			curl_easy_cleanup(pvt->_curl);
			pvt->_curl=NULL;
		}
		if (pvt->_curlm) {
			curl_multi_cleanup(pvt->_curlm);
			pvt->_curlm=NULL;
		}
	}
	#endif

	return retval;
}

ssize_t url::lowLevelRead(void *buffer, ssize_t size) {

	ssize_t	bytesread=0;

	if (pvt->_usingbuiltin) {

		#ifdef DEBUG_HTTP
		stdoutput.printf("\nhttp - lowLevelRead(%d)\n",size);
		#endif

		if (pvt->_chunked) {

			// we either at the beginning or
			// just finished reading a chunk...
			if (!pvt->_chunksize) {

				// get the chunk size
				if (!getChunkSize(pvt->_bof)) {
					return -1;
				}
				pvt->_bof=false;
			}

			// eof condition
			if (!pvt->_chunksize) {
				return 0;
			}

			// don't try to read more than there is in the chunk
			if (size>pvt->_chunksize-pvt->_chunkpos) {
				size=pvt->_chunksize-pvt->_chunkpos;
			} 

			// read some data
			bytesread=pvt->_isc.lowLevelRead(buffer,size);

			// adjust chunk position, reset if necessary
			pvt->_chunkpos+=bytesread;
			if (pvt->_chunkpos==pvt->_chunksize) {
				pvt->_chunkpos=0;
				pvt->_chunksize=0;
			}

			#ifdef DEBUG_HTTP
			stdoutput.printf("http - returning %d bytes "
						"(%d remain in chunk)\n",
						bytesread,
						pvt->_chunksize-pvt->_chunkpos);
			#endif

		} else {

			// eof condition
			if (pvt->_fetchedsofar>=pvt->_contentlength) {
				return 0;
			}

			// read some data
			bytesread=pvt->_isc.lowLevelRead(buffer,size);

			// update the total bytes read
			// (so we can decide later if we've hit an eof)
			pvt->_fetchedsofar+=bytesread;

			#ifdef DEBUG_HTTP
			stdoutput.printf("http - returning %d bytes\n",
								bytesread);
			#endif
		}
	}
	#ifdef RUDIMENTS_HAS_LIBCURL
	else {

		#ifdef DEBUG_CURL
		stdoutput.printf("\ncurl - lowLevelRead(%d)\n",size);
		#endif

		// buffer some data, if necessary
		if (pvt->_bb.getSize()==0 && !pvt->_eof) {

			error::clearError();

			#ifdef DEBUG_CURL
				stdoutput.printf("\ncalling curlPerform()\n");
			#endif
			if (!curlPerform()) {
				return -1;
			}

			// ignore EAGAIN, even if this succeeds,
			// the error number is sometimes set to EAGAIN
			if (error::getErrorNumber()==EAGAIN) {
				error::clearError();
			}
		}

		// don't attempt to return more bytes
		// than there are in the buffer
		if ((uint64_t)size>pvt->_bb.getSize()-pvt->_breadpos) {
			size=pvt->_bb.getSize()-pvt->_breadpos;
		}

		// copy data from the buffer
		bytestring::copy(buffer,
			pvt->_bb.getBuffer()+pvt->_breadpos,
			size);

		// adjust the buffer position and size
		pvt->_breadpos=pvt->_breadpos+size;

		// return how much was actually read
		bytesread=size;

		// clear the buffer, if we read all of it
		if (pvt->_breadpos==pvt->_bb.getPosition()) {
			pvt->_bb.clear();
			pvt->_breadpos=0;
		}
	}
	#endif

	return bytesread;
}

bool url::getChunkSize(bool bof) {

	#ifdef DEBUG_HTTP
	stdoutput.printf("http -   get chunk size...\n");
	#endif

	// read the \r\n trailing behind the previous chunk
	if (!bof) {

		char	crlf[2];
		if (pvt->_isc.read(crlf,sizeof(crlf))<(ssize_t)sizeof(crlf)) {
			pvt->_error.append("http -   "
					"get trailing crlf failed: ");
			char	*err=error::getErrorString();
			pvt->_error.append(err);
			delete[] err;
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			return false;
		}
		if (charstring::compare(crlf,"\r\n",2)) {
			pvt->_error.appendFormatted("http -   "
						"unexpected characters "
						"in trailing crlf: "
						"0x%02x 0x%02x",
						crlf[0],crlf[1]);
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			return false;
		}
	}

	// get until the end-of-line (should be at least 3 bytes too)
	char	*csstring=NULL;
	ssize_t	bytesread=pvt->_isc.read(&csstring,"\r\n",MAX_HEADER_SIZE);
	if (bytesread<3) {
		pvt->_error.appendFormatted("http -   "
						"invalid chunk size: %.*s",
						bytesread,csstring);
		#ifdef DEBUG_HTTP
		stdoutput.printf("%s\n",pvt->_error.getString());
		#endif
		return false;
	}

	// reset chunk size
	pvt->_chunksize=0;

	// initialize return value
	bool		retval=true;

	// run backwards through the string, converting ascii representations
	// of hex digits to integers, multiplying them by the correct place and
	// adding that to the chunksize
	uint64_t	place=1;
	char		*c=csstring+bytesread-2-1;
	for (;;) {
		char	base='\0';
		if (*c>='0' && *c<='9') {
			base='0';
		} else if (*c>='a' && *c<='f') {
			base='a'-10;
		} else if (*c>='A' && *c<='F') {
			base='A'-10;
		} else {
			pvt->_error.appendFormatted("http -   "
						"invalid chunk size: %.*s",
						bytesread,csstring);
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			retval=false;
			break;
		}
		pvt->_chunksize+=((*c-base)*place);
		place=place*16;
		if (c==csstring) {
			break;
		}
		c--;
	}

	#ifdef DEBUG_HTTP
	charstring::rightTrim(csstring);
	if (retval) {
		stdoutput.printf("http -   chunk size is: %d (%s in hex)\n",
						pvt->_chunksize,csstring);
	}
	#endif

	// clean up
	delete[] csstring;

	// if we find a chunk size of 0, then read the crlf after it
	if (pvt->_chunksize==0) {
		char	crlf[2];
		if (pvt->_isc.read(crlf,sizeof(crlf))<(ssize_t)sizeof(crlf)) {
			pvt->_error.append("http -   "
					"get trailing crlf failed: ");
			char	*err=error::getErrorString();
			pvt->_error.append(err);
			delete[] err;
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			return false;
		}
		if (charstring::compare(crlf,"\r\n",2)) {
			pvt->_error.appendFormatted("http -   "
						"unexpected characters "
						"in trailing crlf: "
						"0x%02x 0x%02x",
						crlf[0],crlf[1]);
			#ifdef DEBUG_HTTP
			stdoutput.printf("%s\n",pvt->_error.getString());
			#endif
			return false;
		}
	}

	// return
	return retval;
}

bool url::curlPerform() {

	#ifdef RUDIMENTS_HAS_LIBCURL

	do {
		// Later, we're going to have to wait.
		// Figure out how long we shold wait here.
		uint32_t	sec=60;
		uint32_t	usec=0;
		#ifdef RUDIMENTS_HAS_CURL_MULTI_TIMEOUT
			long		timeout=-1;
			if (curl_multi_timeout(pvt->_curlm,
						&timeout)!=CURLM_OK) {
				#ifdef DEBUG_CURL
				stdoutput.printf("curl multi timeout failed\n");
				#endif
				return false;
			}
			if (timeout>=0) {
				sec=timeout/1000;
				if (sec>1) {
					sec=1;
				} else {
					usec=(timeout%1000)*1000;
				}
			}
		#endif

		// Try to get the file descriptors associated with this thing.
		int	maxfd=-1;
		fd_set	fdread;
		fd_set	fdwrite;
		fd_set	fdexecp;
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexecp);
		if (curl_multi_fdset(pvt->_curlm,
				&fdread,&fdwrite,&fdexecp,&maxfd)!=CURLM_OK) {
			#ifdef DEBUG_CURL
			stdoutput.printf("curl multi fdset failed\n");
			#endif
			return false;
		}

		// Here's that wait I was talking about...
		if (maxfd==-1) {

			// if we weren't able to get a file descriptor,
			// then just wait for 100ms
			snooze::microsnooze(0,100000);

		} else {

			// if we were able to get a file descriptor...

			// if the class doesn't already have a file
			// descriptor associated with it...
			if (fd()==-1) {

				pvt->_l.removeAllFileDescriptors();

				// figure out which fd it was and set the
				// class' file descriptor to it
				for (int32_t i=0; i<=maxfd; i++) {

					// turns out we have to check both,
					// even if we're only reading
					if (FD_ISSET(i,&fdread) ||
						FD_ISSET(i,&fdwrite)) {
						#ifdef DEBUG_CURL
						stdoutput.printf(
							"listening "
							"on fd: %d\n",i);
						#endif
						fd(i);
						break;
					}
				}

				pvt->_l.addFileDescriptor(this);
			}

			// now, instead of just waiting, we'll listen on
			// the file descriptor, and time out after the
			// recomended amount of time...
			if (pvt->_l.listen(sec,usec)==RESULT_ERROR) {
				#ifdef DEBUG_CURL
				char	*err=error::getErrorString();
				stdoutput.printf("listen failed: %s\n",err);
				delete[] err;
				#endif
				return false;
			}
		}

		// and now, hopefully, some data will be ready
		if (curl_multi_perform(pvt->_curlm,
				(int *)&pvt->_stillrunning)!=CURLM_OK) {
			#ifdef DEBUG_CURL
			stdoutput.printf("curl multi perform failed\n");
			#endif
			return false;
		}

	// if no data is available though, or if we just need to
	// try and get more data, then loop back and try again
	} while (!pvt->_bb.getSize() && pvt->_stillrunning);

	return true;

	#else

	return false;

	#endif
}

size_t url::curlReadData(void *buffer, size_t size, size_t nmemb, void *userp) {

	#ifdef RUDIMENTS_HAS_LIBCURL

	#ifdef DEBUG_CURL
		stdoutput.printf("curlReadData(%d,%d) (%d bytes)\n",
						size,nmemb,size*nmemb);
	#endif

	url	*u=(url *)userp;

	// get the actual size (in bytes)
	size=size*nmemb;

	// copy data into the buffer
	u->pvt->_bb.append((unsigned char *)buffer,size);

	// did we fetch the entire file?
	u->pvt->_eof=!size;

	#ifdef DEBUG_CURL
		stdoutput.printf("    %d bytes buffered this read\n",size);
		stdoutput.printf("    %d bytes buffered total\n",
						u->pvt->_bb.getSize());
	#endif

	// return how much data was buffered
	return size;

	#else

	return 0;

	#endif
}

bool url::initUrl() {

	bool	result=true;

	#ifdef RUDIMENTS_HAS_LIBCURL
	_urlmutex.lock();
	if (!_initialized) {
		result=!curl_global_init(CURL_GLOBAL_ALL);
	}
	_urlmutex.unlock();
	#endif

	return result;
}

void url::shutDownUrl() {
	#ifdef RUDIMENTS_HAS_LIBCURL
	if (_initialized) {
		curl_global_cleanup();
	}
	#endif
}

bool url::getCurrentProperties() {
	return false;
}

off64_t url::getCurrentPosition() {
	return -1;
}

off64_t url::setPositionRelativeToBeginning(off64_t offset) {
	return -1;
}

off64_t url::setPositionRelativeToCurrent(off64_t offset) {
	return -1;
}

off64_t url::setPositionRelativeToEnd(off64_t offset) {
	return -1;
}

char *url::getContents() {
	return file::getContents();
}

ssize_t url::getContents(unsigned char *buffer, size_t buffersize) {
	return file::getContents(buffer,buffersize);
}

char *url::getContents(const char *name) {
	url	u;
	u.open(name,O_RDONLY);
	char	*contents=u.getContents();
	u.close();
	return contents;
}

ssize_t url::getContents(const char *name, unsigned char *buffer,
						size_t buffersize) {
	url	u;
	u.open(name,O_RDONLY);
	ssize_t	bytes=u.getContents(buffer,buffersize);
	u.close();
	return bytes;
}

const char *url::getError() {
	return (pvt->_error.getSize())?pvt->_error.getString():NULL;
}

void url::enableOpenTimings() {
	pvt->_opentimings=true;
}

void url::disableOpenTimings() {
	pvt->_opentimings=false;
}

void url::getOpenTimings(uint64_t *build,
				uint64_t *connect,
				uint64_t *send,
				uint64_t *wait,
				uint64_t *recv) {
	if (build) {
		*build=pvt->_openbuild;
	}
	if (connect) {
		*connect=pvt->_openconnect;
	}
	if (send) {
		*send=pvt->_opensend;
	}
	if (wait) {
		*wait=pvt->_openwait;
	}
	if (recv) {
		*recv=pvt->_openrecv;
	}
}

uint64_t url::interval() {
	return ((pvt->_end.getEpoch()*1000000+pvt->_end.getMicrosecond())-
		(pvt->_start.getEpoch()*1000000+pvt->_start.getMicrosecond()));
}
