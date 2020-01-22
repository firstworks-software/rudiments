// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/httpresponse.h>
#include <rudiments/charstring.h>

httpresponse::httpresponse(httpserverapi *sapi) {
	this->sapi=sapi;
}

httpresponse::~httpresponse() {
}

void httpresponse::cr() {
	sapi->header("\r\n");
}

void httpresponse::textHtml() {
	contentType("text","html");
	cr();
	cr();
}

void httpresponse::textPlain() {
	contentType("text","plain");
	cr();
	cr();
}

void httpresponse::contentType(const char *type, const char *subtype) {
	contentType(type,subtype,NULL,NULL);
}

void httpresponse::contentType(const char *type, const char *subtype,
							const char *charset) {
	contentType(type,subtype,charset,NULL);
}

void httpresponse::contentType(const char *type, const char *subtype,
						const char *charset,
						const char *boundary) {
	stringbuffer	contenttypestr;
	contenttypestr.append(type)->append('/')->append(subtype);
	if (!charstring::isNullOrEmpty(charset)) {
		contenttypestr.append(";charset=")->append(charset);
	}
	if (!charstring::isNullOrEmpty(boundary)) {
		contenttypestr.append(";boundary=")->append(boundary);
	}
	sapi->header("Content-type",contenttypestr.getString());
}

void httpresponse::status(const char *protocol, const char *version,
						const char *code) {
	stringbuffer	statusline;
	statusline.append(protocol)->append('/');
	statusline.append(version)->append(' ')->append(code);
	sapi->status(statusline.getString());
}

void httpresponse::setCookie(const char *name, const char *value,
				const char *domain, const char *path,
				const char *expires, bool secure) {
	stringbuffer	cookiestr;
	cookiestr.append(name)->append('=')->append(value);
	if (!charstring::isNullOrEmpty(domain)) {
		cookiestr.append("; domain=")->append(domain);
	}
	if (!charstring::isNullOrEmpty(path)) {
		cookiestr.append("; path=")->append(path);
	}
	if (!charstring::isNullOrEmpty(expires)) {
		cookiestr.append("; expires=")->append(expires);
	}
	if (secure) {
		cookiestr.append("; secure");
	}
	sapi->header("Set-Cookie",cookiestr.getString());
}

const char *httpresponse::boundaryString() {
	return "ThisRandomString";
}

void httpresponse::multiPartBoundary(output *out) {
	out->write('\n');
	out->write("--");
	out->write(boundaryString());
	out->write('\n');
}

void httpresponse::multiPartEnd(output *out) {
	out->write('\n');
	out->write("--");
	out->write(boundaryString());
	out->write("--");
	out->write('\n');
}

httpresponse *httpresponse::status(const char *status) {
	sapi->status(status);
	return this;
}

httpresponse *httpresponse::header(const char *header, const char *value) {
	sapi->header(header,value);
	return this;
}

httpresponse *httpresponse::header(const char *header) {
	sapi->header(header);
	return this;
}

ssize_t httpresponse::write(const unsigned char *string, size_t size) {
	return sapi->write(string,size);
}

ssize_t httpresponse::write(const char *string) {
	return sapi->write(string);
}

ssize_t httpresponse::write(const char *string, size_t size) {
	return sapi->write(string,size);
}

ssize_t httpresponse::write(char character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(int16_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(int32_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(int64_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(unsigned char character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(uint16_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(uint32_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(uint64_t character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(float character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(double character) {
	return sapi->write(character);
}

ssize_t httpresponse::write(file *filebuffer) {
	char	buffer[1024];
	ssize_t	count=0;
	for (;;) {
		ssize_t	size=filebuffer->read(buffer,sizeof(buffer));
		if (size<=0) {
			return size;
		}
		ssize_t	result=write(buffer,size);
		if (result>=0) {
			count+=result;
		} else {
			return result;
		}
		if (size<(ssize_t)sizeof(buffer)) {
			break;
		}
	}
	return count;
}
