// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/sensitivevalue.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/sys.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>

class sensitivevalueprivate {
	friend class sensitivevalue;
	private:
		const char	*_includestart;
		uint64_t	_includestartlen;
		const char	*_includeend;
		uint64_t	_includeendlen;
		sensitivevalue_format_t	_verbatimformat;
		sensitivevalue_format_t	_fileformat;
		const char	*_path;
		uint64_t	_pathlen;
		const char	*_textext;
		uint64_t	_textextlen;
		const char	*_binaryext;
		uint64_t	_binaryextlen;
		const char	*_hexext;
		uint64_t	_hexextlen;
		bool		_chomptextfile;
		byte_t		*_value;
		uint64_t	_valuesize;
};

sensitivevalue::sensitivevalue() {
	pvt=new sensitivevalueprivate;
	construct();
}

sensitivevalue::~sensitivevalue() {
	delete[] pvt->_value;
	delete pvt;
}

void sensitivevalue::construct() {
	bytestring::zero(pvt,sizeof(sensitivevalueprivate));
	pvt->_includestart="[";
	pvt->_includestartlen=1;
	pvt->_includeend="]";
	pvt->_includeendlen=1;
	pvt->_chomptextfile=true;
}

void sensitivevalue::setIncludeStart(const char *delimiter) {
	setIncludeStart(delimiter,charstring::getLength(delimiter));
}

void sensitivevalue::setIncludeStart(const char *delimiter, uint64_t len) {
	pvt->_includestart=delimiter;
	pvt->_includestartlen=len;
}

void sensitivevalue::setIncludeEnd(const char *delimiter) {
	setIncludeEnd(delimiter,charstring::getLength(delimiter));
}

void sensitivevalue::setIncludeEnd(const char *delimiter, uint64_t len) {
	pvt->_includeend=delimiter;
	pvt->_includeendlen=len;
}

void sensitivevalue::setVerbatimFormat(sensitivevalue_format_t verbatimformat) {
	pvt->_verbatimformat=verbatimformat;
}

void sensitivevalue::setFileFormat(sensitivevalue_format_t fileformat) {
	pvt->_fileformat=fileformat;
}

void sensitivevalue::setChompTextFile(bool chomptextfile) {
	pvt->_chomptextfile=chomptextfile;
}

void sensitivevalue::setPath(const char *path) {
	setPath(path,charstring::getLength(path));
}

void sensitivevalue::setPath(const char *path, uint64_t len) {
	pvt->_path=path;
	pvt->_pathlen=len;
}

void sensitivevalue::setTextExtension(const char *ext) {
	setTextExtension(ext,charstring::getLength(ext));
}

void sensitivevalue::setTextExtension(const char *ext, uint64_t len) {
	pvt->_textext=ext;
	pvt->_textextlen=len;
}

void sensitivevalue::setBinaryExtension(const char *ext) {
	setBinaryExtension(ext,charstring::getLength(ext));
}

void sensitivevalue::setBinaryExtension(const char *ext, uint64_t len) {
	pvt->_binaryext=ext;
	pvt->_binaryextlen=len;
}

void sensitivevalue::setHexExtension(const char *ext) {
	setHexExtension(ext,charstring::getLength(ext));
}

void sensitivevalue::setHexExtension(const char *ext, uint64_t len) {
	pvt->_hexext=ext;
	pvt->_hexextlen=len;
}

const char *sensitivevalue::getIncludeStart() {
	return pvt->_includestart;
}

uint64_t sensitivevalue::getIncludeStartLength() {
	return pvt->_includestartlen;
}

const char *sensitivevalue::getIncludeEnd() {
	return pvt->_includeend;
}

uint64_t sensitivevalue::getIncludeEndLength() {
	return pvt->_includeendlen;
}

sensitivevalue_format_t sensitivevalue::getVerbatimFormat() {
	return pvt->_verbatimformat;
}

sensitivevalue_format_t sensitivevalue::getFileFormat() {
	return pvt->_fileformat;
}

bool sensitivevalue::getChompTextFile() {
	return pvt->_chomptextfile;
}

const char *sensitivevalue::getPath() {
	return pvt->_path;
}

uint64_t sensitivevalue::getPathLength() {
	return pvt->_pathlen;
}

const char *sensitivevalue::getTextExtension() {
	return pvt->_textext;
}

uint64_t sensitivevalue::getTextExtensionLength() {
	return pvt->_textextlen;
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
	parse(in,charstring::getLength(in));
}

void sensitivevalue::parse(const char *in, uint64_t inlen) {

	clear();

	// if the input is [...file...] then attempt to get the
	// contents of the specified file
	if (!charstring::compare(in,pvt->_includestart,
					pvt->_includestartlen) &&
		!charstring::compare(in+inlen-pvt->_includeendlen,
							pvt->_includeend,
							pvt->_includeendlen)) {

		stringbuffer	fn;

		// try the filename as-is
		fn.clear();
		fn.append(in+1,inlen-2);
		if (getValueFromFile(fn.getString(),
					pvt->_fileformat==FORMAT_HEX,
					pvt->_fileformat==FORMAT_TEXT &&
					pvt->_chomptextfile)) {
			return;
		}

		// try prepending a path
		if (!charstring::isNullOrEmpty(pvt->_path)) {
			fn.clear();
			fn.append(pvt->_path,pvt->_pathlen);
			fn.append(sys::getDirectorySeparator());
			fn.append(in+1,inlen-2);
			if (getValueFromFile(fn.getString(),
					pvt->_fileformat==FORMAT_HEX,
					pvt->_fileformat==FORMAT_TEXT &&
					pvt->_chomptextfile)) {
				return;
			}
		}

		if (!charstring::isNullOrEmpty(pvt->_textext)) {

			// try appending the text extension
			fn.clear();
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_textext,pvt->_textextlen);
			if (getValueFromFile(fn.getString(),
					false,pvt->_chomptextfile)) {
				return;
			}

			// try path + text extension
			if (!charstring::isNullOrEmpty(pvt->_path)) {
				fn.clear();
				fn.append(pvt->_path,pvt->_pathlen);
				fn.append(sys::getDirectorySeparator());
				fn.append(in+1,inlen-2);
				fn.append('.');
				fn.append(pvt->_textext,pvt->_textextlen);
				if (getValueFromFile(fn.getString(),
						false,pvt->_chomptextfile)) {
					return;
				}
			}
		}

		if (!charstring::isNullOrEmpty(pvt->_binaryext)) {

			// try appending the binary extension
			fn.clear();
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_binaryext,pvt->_binaryextlen);
			if (getValueFromFile(
				fn.getString(),false,false)) {
				return;
			}

			// try path + binary extension
			if (!charstring::isNullOrEmpty(pvt->_path)) {
				fn.clear();
				fn.append(pvt->_path,pvt->_pathlen);
				fn.append(sys::getDirectorySeparator());
				fn.append(in+1,inlen-2);
				fn.append('.');
				fn.append(pvt->_binaryext,pvt->_binaryextlen);
				if (getValueFromFile(
					fn.getString(),false,false)) {
					return;
				}
			}
		}

		if (!charstring::isNullOrEmpty(pvt->_hexext)) {

			// try appending the hex extension
			fn.clear();
			fn.append(in+1,inlen-2);
			fn.append('.');
			fn.append(pvt->_hexext,pvt->_hexextlen);
			if (getValueFromFile(
				fn.getString(),true,false)) {
				return;
			}

			// try path + hex extension
			if (!charstring::isNullOrEmpty(pvt->_path)) {
				fn.clear();
				fn.append(pvt->_path,pvt->_pathlen);
				fn.append(sys::getDirectorySeparator());
				fn.append(in+1,inlen-2);
				fn.append('.');
				fn.append(pvt->_hexext,pvt->_hexextlen);
				if (getValueFromFile(
					fn.getString(),true,false)) {
					return;
				}
			}
		}
	}

	// just return the in verbatim
	if (pvt->_verbatimformat==FORMAT_HEX) {
		charstring::hexDecode(in,inlen,
				&(pvt->_value),&(pvt->_valuesize));
	} else {
		pvt->_value=(byte_t *)charstring::duplicate(in,inlen);
		pvt->_valuesize=inlen;
	}
}

bool sensitivevalue::getValueFromFile(const char *filename,
						bool hexdecode, bool chomp) {
	file	f;
	if (f.open(filename,O_RDONLY)) {
		if (hexdecode) {
			charstring::hexDecode(f.getContents(),
							f.getSize(),
							&(pvt->_value),
							&(pvt->_valuesize));
		} else {
			pvt->_valuesize=f.getSize();
			pvt->_value=(byte_t *)f.getContents();
		}
		if (chomp) {
			if (pvt->_valuesize>=2) {
				if (!bytestring::compare(
						pvt->_value+
						pvt->_valuesize-2,
						"\r\n",2) ||
					!bytestring::compare(
						pvt->_value+
						pvt->_valuesize-2,
						"\n\r",2)) {
					*(pvt->_value+pvt->_valuesize-2)='\0';
					pvt->_valuesize-=2;
				} else if (
					*(pvt->_value+
						pvt->_valuesize-1)=='\r' ||
					*(pvt->_value+
						pvt->_valuesize-1)=='\n') {
					*(pvt->_value+pvt->_valuesize-1)='\0';
					pvt->_valuesize-=1;
				}
			} else if (pvt->_valuesize>=1) {
				if (*(pvt->_value+
						pvt->_valuesize-1)=='\r' ||
					*(pvt->_value+
						pvt->_valuesize-1)=='\n') {
					*(pvt->_value+pvt->_valuesize-1)='\0';
					pvt->_valuesize-=1;
				}
			}
		}
		return true;
	}
	return false;
}
 
const byte_t *sensitivevalue::getValue() {
	return pvt->_value;
}

byte_t *sensitivevalue::detachValue() {
	byte_t	*retval=pvt->_value;
	pvt->_value=NULL;
	pvt->_valuesize=0;
	return retval;
}

uint64_t sensitivevalue::getValueSize() {
	return pvt->_valuesize;
}

const char *sensitivevalue::getTextValue() {
	return (const char *)pvt->_value;
}

char *sensitivevalue::detachTextValue() {
	char	*retval=(char *)pvt->_value;
	pvt->_value=NULL;
	pvt->_valuesize=0;
	return retval;
}

uint64_t sensitivevalue::getTextValueLength() {
	return charstring::getLength(getTextValue());
}

bool sensitivevalue::clear() {
	delete[] pvt->_value;
	pvt->_value=NULL;
	pvt->_valuesize=0;
	return true;
}

bool sensitivevalue::reset() {
	delete[] pvt->_value;
	construct();
	return true;
}
