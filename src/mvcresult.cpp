// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvc.h>
#include <rudiments/dictionary.h>

class mvcresultprivate {
	friend class mvcresult;
	private:
		bool		_success;
		uint32_t	_code;
		char		*_message;

		dictionary<char *, const collection *>	_data;

		wastebasket	_wb;
		
};

mvcresult::mvcresult() : object() {
	pvt=new mvcresultprivate;
	pvt->_success=false;
	pvt->_code=0;
	pvt->_message=NULL;
	pvt->_data.setManageArrayKeys(true);
}

mvcresult::~mvcresult() {
	delete[] pvt->_message;
	delete pvt;
}

void mvcresult::setResult(bool success) {
	setResult(success,0,NULL);
}

void mvcresult::setResult(bool success, uint32_t code) {
	setResult(success,code,NULL);
}

void mvcresult::setResult(bool success, uint32_t code, const char *message) {
	setSuccess(success);
	setCode(code);
	setMessage(message);
}

void mvcresult::setSuccess() {
	setSuccess(0,NULL);
}

void mvcresult::setSuccess(uint32_t code) {
	setSuccess(code,NULL);
}

void mvcresult::setSuccess(uint32_t code, const char *message) {
	setResult(true,code,message);
}

void mvcresult::setFailed() {
	setFailed(0,NULL);
}

void mvcresult::setFailed(uint32_t code) {
	setFailed(code,NULL);
}

void mvcresult::setFailed(uint32_t code, const char *message) {
	setResult(false,code,message);
}

void mvcresult::setSuccess(bool success) {
	pvt->_success=success;
}

bool mvcresult::getSuccess() {
	return pvt->_success;
}

void mvcresult::setCode(uint32_t code) {
	pvt->_code=code;
}

uint32_t mvcresult::getCode() {
	return pvt->_code;
}

void mvcresult::setMessage(const char *message) {
	delete[] pvt->_message;
	pvt->_message=charstring::duplicate(message);
}

const char *mvcresult::getMessage() {
	return pvt->_message;
}

void mvcresult::setData(const char *name, const collection *data) {

	// remove any existing data for this name
	pvt->_data.remove((char *)name);
	
	// attach data
	pvt->_data.setValue(charstring::duplicate(name),data);
}

void mvcresult::attachData(const char *name, collection *data) {
	setData(name,data);
	pvt->_wb.attach(data);
}

linkedlist<char *> *mvcresult::getKeys() {
	return pvt->_data.getKeys();
}

const collection *mvcresult::getData(const char *name) {
	return pvt->_data.getValue((char *)name);
}

ssize_t mvcresult::write() const {
	return write();
}

ssize_t mvcresult::write(output *out) const {
	return writeJson(out,true);
}

ssize_t mvcresult::writeJson() const {
	return writeJson(true);
}

ssize_t mvcresult::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

ssize_t mvcresult::writeJson(output *out) const {
	return writeJson(out,true);
}

ssize_t mvcresult::writeJson(output *out, bool indent) const {

	ssize_t			retval=0;

	// begin object
	incOrErr(&retval,out->write('{')) &&
	((indent)?incOrErr(&retval,out->write("\n")):true) &&

	// success: true/false
	incOrErr(&retval,out->write("\"s\":")) &&
	((indent)?incOrErr(&retval,out->write(' ')):true) &&
	incOrErr(&retval,out->write((pvt->_success)?"true":"false")) &&
	incOrErr(&retval,out->write(',')) &&
	((indent)?incOrErr(&retval,out->write('\n')):true);

	if (pvt->_success) {

		// collections
		bool	first=true;
		for (listnode<char *> *key=
				pvt->_data.getKeys()->getFirst();
				key; key=key->getNext()) {
			
			if (first) {
				first=false;
			} else {
				incOrErr(&retval,out->write(',')) &&
				((indent)?incOrErr(&retval,
						out->write('\n')):true);
			}
			incOrErr(&retval,out->write('"')) &&
			incOrErr(&retval,out->write(key->getValue())) &&
			incOrErr(&retval,out->write("\":")) &&
			((indent)?incOrErr(&retval,out->write(' ')):true);
			const collection	*c=
				pvt->_data.getValue(key->getValue());
			if (c) {
				incOrErr(&retval,c->writeJson(out,indent));
			} else {
				incOrErr(&retval,out->write("null"));
			}
		}
		
	} else {

		// error code
		incOrErr(&retval,out->write("\"c\":")) &&
		((indent)?incOrErr(&retval,out->write(' ')):true) &&
		incOrErr(&retval,out->write(pvt->_code)) &&
		incOrErr(&retval,out->write(',')) &&
		((indent)?incOrErr(&retval,out->write('\n')):true) &&

		// error message
		incOrErr(&retval,out->write("\"m\":")) &&
		((indent)?incOrErr(&retval,out->write(' ')):true) &&
		incOrErr(&retval,out->write('"')) &&
		incOrErr(&retval,out->write(pvt->_message)) &&
		incOrErr(&retval,out->write('"')) &&
		((indent)?incOrErr(&retval,out->write('\n')):true);
	}

	// end object
	incOrErr(&retval,out->write('}')) &&
	((indent)?incOrErr(&retval,out->write('\n')):true);

	return retval;
}

ssize_t mvcresult::writeXml() const {
	return writeXml(true);
}

ssize_t mvcresult::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

ssize_t mvcresult::writeXml(output *out) const {
	return writeXml(out,true);
}

ssize_t mvcresult::writeXml(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

bool mvcresult::incOrErr(ssize_t *retval, ssize_t val) const {

	// FIXME: this is duplicated in collection, move them somewhere

	// add val to *retval unless:
	// * retval is already negative, indicating that an error condition
	//   occurred previously, in this case leave retval set to the error
	//   condition
	// * val is negative, indicating an error condition just occurred, in
	//   this case set retval to the error condition
	// return true on success or false if an error condition occurred

	if (*retval>-1) {
		if (val>-1) {
			(*retval)+=val;
			return true;
		} else {
			(*retval)=val;
		}
	}
	return false;
}

wastebasket *mvcresult::getWastebasket() {
	return &pvt->_wb;
}
