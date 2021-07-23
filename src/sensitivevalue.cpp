// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/sensitivevalue.h>
#include <rudiments/charstring.h>

class sensitivevalueprivate {
	friend class sensitivevalue;
	private:
		const char	*_redirectstart;
		uint64_t	_redirectstartlen;
		const char	*_redirectend;
		uint64_t	_redirectendlen;
		bool		_verbatimishex;
		bool		_fileishex;
		const char	*_path;
		uint64_t	_pathlen;
		const char	*_binaryext;
		uint64_t	_binaryextlen;
		const char	*_hexext;
		uint64_t	_hexextlen;
		unsigned char	*_value;
		uint64_t	_valuesize;
};

sensitivevalue::sensitivevalue() {
	pvt=new sensitivevalueprivate;
}

sensitivevalue::~sensitivevalue() {
	delete pvt;
}

void sensitivevalue::setRedirectStart(const char *delimiter) {
	setRedirectStart(delimiter,charstring::length(delimiter));
}

void sensitivevalue::setRedirectStart(const char *delimiter, uint64_t len) {
	pvt->_redirectstart=delimiter;
	pvt->_redirectstartlen=len;
}

void sensitivevalue::setRedirectEnd(const char *delimiter) {
	setRedirectEnd(delimiter,charstring::length(delimiter));
}

void sensitivevalue::setRedirectEnd(const char *delimiter, uint64_t len) {
	pvt->_redirectend=delimiter;
	pvt->_redirectendlen=len;
}

void sensitivevalue::setVerbatimIsHex(bool verbatimishex) {
	pvt->_verbatimishex=verbatimishex;
}

void sensitivevalue::setFileIsHex(bool fileishex) {
	pvt->_fileishex=fileishex;
}

void sensitivevalue::setPath(const char *path) {
	setPath(path,charstring::length(path));
}

void sensitivevalue::setPath(const char *path, uint64_t len) {
	pvt->_path=path;
	pvt->_pathlen=len;
}

void sensitivevalue::setBinaryExtension(const char *ext) {
	setBinaryExtension(ext,charstring::length(ext));
}

void sensitivevalue::setBinaryExtension(const char *ext, uint64_t len) {
	pvt->_binaryext=ext;
	pvt->_binaryextlen=len;
}

void sensitivevalue::setHexExtension(const char *ext) {
	setHexExtension(ext,charstring::length(ext));
}

void sensitivevalue::setHexExtension(const char *ext, uint64_t len) {
	pvt->_hexext=ext;
	pvt->_hexextlen=len;
}

const char *sensitivevalue::getRedirectStart() {
	return pvt->_redirectstart;
}

uint64_t sensitivevalue::getRedirectStartLength() {
	return pvt->_redirectstartlen;
}

const char *sensitivevalue::getRedirectEnd() {
	return pvt->_redirectend;
}

uint64_t sensitivevalue::getRedirectEndLength() {
	return pvt->_redirectendlen;
}

bool sensitivevalue::getVerbatimIsHex() {
	return pvt->_verbatimishex;
}

bool sensitivevalue::getFileIsHex() {
	return pvt->_fileishex;
}

const char *sensitivevalue::getPath() {
	return pvt->_path;
}

uint64_t sensitivevalue::getPathLength() {
	return pvt->_pathlen;
}

const char *sensitivevalue::getBinaryExtension() {
	return pvt->_binaryext;
}

uint64_t sensitivevalue::getBinaryExtensionLength() {
	return pvt->_binaryextlen;
}

const char *sensitivevalue::getHexExtension() {
	return pvt->_hexext;
}

uint64_t sensitivevalue::getHexExtensionLength() {
	return pvt->_hexextlen;
}

void sensitivevalue::parse(const char *in) {
	parse(in,charstring::length(in));
}

void sensitivevalue::parse(const char *in, uint64_t inlen) {
	// FIXME: implement this
}

bool sensitivevalue::getValueFromFile(const char *filename,
						bool hexdecode) {
	// FIXME: implement this
	return true;
}
 
const unsigned char *sensitivevalue::getValue() {
	return pvt->_value;
}

uint64_t sensitivevalue::getValueSize() {
	return pvt->_valuesize;
}

const char *sensitivevalue::getTextValue() {
	return (const char *)pvt->_value;
}

uint64_t sensitivevalue::getTextValueLength() {
	return charstring::length(getTextValue());
}
