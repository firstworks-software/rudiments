// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/sensitivevalue.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/sys.h>
#include <rudiments/file.h>

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
	pvt->_redirectstart="[";
	pvt->_redirectstartlen=1;
	pvt->_redirectend="]";
	pvt->_redirectendlen=1;
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

	delete[] pvt->_value;
	pvt->_value=NULL;
	pvt->_valuesize=0;

	// if the input is [...file...] then attempt to get the
	// contents of the specified file
	if (!charstring::compare(in,pvt->_redirectstart,
					pvt->_redirectstartlen) &&
		!charstring::compare(in+inlen-pvt->_redirectendlen,
							pvt->_redirectend,
							pvt->_redirectendlen)) {

		stringbuffer	fn;

		// try the filename as-is
		fn.clear();
		fn.append(in+1,inlen-2);
		if (getValueFromFile(fn.getString(),pvt->_fileishex)) {
			return;
		}

		// try prepending a path
		fn.clear();
		fn.append(pvt->_path,pvt->_pathlen);
		fn.append(sys::getDirectorySeparator());
		fn.append(in+1,inlen-2);
		if (getValueFromFile(fn.getString(),pvt->_fileishex)) {
			return;
		}

		if (!charstring::isNullOrEmpty(pvt->_binaryext)) {

			// try appending the binary extension
			fn.clear();
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_binaryext,pvt->_binaryextlen);
			if (getValueFromFile(fn.getString(),false)) {
				return;
			}

			// try path + binary extension
			fn.clear();
			fn.append(pvt->_path,pvt->_pathlen);
			fn.append(sys::getDirectorySeparator());
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_binaryext,pvt->_binaryextlen);
			if (getValueFromFile(fn.getString(),false)) {
				return;
			}
		}

		if (!charstring::isNullOrEmpty(pvt->_hexext)) {

			// try appending the hex extension
			fn.clear();
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_hexext,pvt->_hexextlen);
			if (getValueFromFile(fn.getString(),true)) {
				return;
			}

			// try path + hex extension
			fn.clear();
			fn.append(pvt->_path,pvt->_pathlen);
			fn.append(sys::getDirectorySeparator());
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_hexext,pvt->_hexextlen);
			if (getValueFromFile(fn.getString(),true)) {
				return;
			}
		}
	}

	// just return the in verbatim
	if (pvt->_verbatimishex) {
		charstring::hexDecode(in,inlen,&pvt->_value,&pvt->_valuesize);
	} else {
		pvt->_value=(unsigned char *)charstring::duplicate(in,inlen);
		pvt->_valuesize=inlen;
	}
}

bool sensitivevalue::getValueFromFile(const char *filename,
						bool hexdecode) {
	file	f;
	if (f.open(filename,O_RDONLY)) {
		if (hexdecode) {
			charstring::hexDecode(f.getContents(),
							f.getSize(),
							&pvt->_value,
							&pvt->_valuesize);
		} else {
			pvt->_valuesize=f.getSize();
			pvt->_value=(unsigned char *)f.getContents();
		}
		return true;
	}
	return false;
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
