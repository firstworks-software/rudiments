// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/httpresponse.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>

class httpresponseprivate {
	friend class httpresponse;
	private:
		httpserverapi	*_sapi;
};

httpresponse::httpresponse(httpserverapi *sapi) : object() {
	pvt=new httpresponseprivate;
	pvt->_sapi=sapi;
}

httpresponse::~httpresponse() {
	delete pvt;
}

void httpresponse::writeStatusLineBlock(const char *status) {
	writeStatusLine(status);
	writeHeaderTerminator();
}

void httpresponse::writeStatusHeaderBlock(const char *status) {
	writeStatusHeader(status);
	writeHeaderTerminator();
}

void httpresponse::writeTextHtmlHeaderBlock() {
	writeTextHtmlHeader();
	writeHeaderTerminator();
}

void httpresponse::writeTextPlainHeaderBlock() {
	writeTextPlainHeader();
	writeHeaderTerminator();
}

void httpresponse::writeContentTypeHeaderBlock(const char *type,
						const char *subtype) {
	writeContentTypeHeader(type,subtype);
	writeHeaderTerminator();
}

void httpresponse::writeContentTypeHeaderBlock(const char *type,
						const char *subtype,
						const char *charset) {
	writeContentTypeHeader(type,subtype,charset);
	writeHeaderTerminator();
}

void httpresponse::writeContentTypeHeaderBlock(const char *type,
						const char *subtype,
						const char *charset,
						const char *boundary) {
	writeContentTypeHeader(type,subtype,charset,boundary);
	writeHeaderTerminator();
}

void httpresponse::writeStatusLine(const char *status) {
	pvt->_sapi->writeStatusLine(status);
}

void httpresponse::writeStatusHeader(const char *status) {
	pvt->_sapi->writeStatusHeader(status);
}

void httpresponse::writeTextHtmlHeader() {
	writeContentTypeHeader("text","html");
}

void httpresponse::writeTextPlainHeader() {
	writeContentTypeHeader("text","plain");
}

void httpresponse::writeContentTypeHeader(const char *type,
						const char *subtype) {
	writeContentTypeHeader(type,subtype,NULL,NULL);
}

void httpresponse::writeContentTypeHeader(const char *type, const char *subtype,
							const char *charset) {
	writeContentTypeHeader(type,subtype,charset,NULL);
}

void httpresponse::writeContentTypeHeader(const char *type,
						const char *subtype,
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
	pvt->_sapi->writeHeader("Content-type",contenttypestr.getString());
}

void httpresponse::writeHeaderTerminator() {
	pvt->_sapi->writeHeaderTerminator();
}

void httpresponse::writeSetCookie(const char *name, const char *value,
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
	pvt->_sapi->writeHeader("Set-Cookie",cookiestr.getString());
}

const char *httpresponse::getBoundaryString() {
	return "ThisRandomString";
}

void httpresponse::writeMultiPartBoundary(output *out) {
	out->write('\n');
	out->write("--");
	out->write(getBoundaryString());
	out->write('\n');
}

void httpresponse::writeFinalMultiPartBoundary(output *out) {
	out->write('\n');
	out->write("--");
	out->write(getBoundaryString());
	out->write("--");
	out->write('\n');
}

void httpresponse::writeHeader(const char *header, const char *value) {
	pvt->_sapi->writeHeader(header,value);
}

ssize_t httpresponse::write(const byte_t *string, size_t size) {
	return pvt->_sapi->write(string,size);
}

ssize_t httpresponse::write(const char *string) {
	return pvt->_sapi->write(string);
}

ssize_t httpresponse::write(const char *string, size_t length) {
	return pvt->_sapi->write(string,length);
}

ssize_t httpresponse::write(char character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(const wchar_t *string) {
	return pvt->_sapi->write(string);
}

ssize_t httpresponse::write(const wchar_t *string, size_t length) {
	return pvt->_sapi->write(string,length);
}

ssize_t httpresponse::write(wchar_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::writeUcs2(const ucs2_t *string) {
	return pvt->_sapi->writeUcs2(string);
}

ssize_t httpresponse::writeUcs2(const ucs2_t *string, size_t length) {
	return pvt->_sapi->writeUcs2(string,length);
}

ssize_t httpresponse::writeUcs2(ucs2_t character) {
	return pvt->_sapi->writeUcs2(character);
}

ssize_t httpresponse::write(int16_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(int32_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(int64_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(byte_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(uint16_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(uint32_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(uint64_t character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(float character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::write(double character) {
	return pvt->_sapi->write(character);
}

ssize_t httpresponse::printfDelegate(const char *format, va_list *argp) {
	return pvt->_sapi->printf(format,argp);
}

ssize_t httpresponse::printfDelegate(const wchar_t *format, va_list *argp) {
	return pvt->_sapi->printf(format,argp);
}

ssize_t httpresponse::printfUcs2Delegate(const ucs2_t *format, va_list *argp) {
	return pvt->_sapi->printfUcs2(format,argp);
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
