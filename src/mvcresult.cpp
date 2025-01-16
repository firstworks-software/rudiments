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

		dictionary<char *, collection *>	_data;

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

void mvcresult::setStatus(bool success) {
	setStatus(success,0,NULL);
}

void mvcresult::setStatus(bool success, uint32_t code) {
	setStatus(success,code,NULL);
}

void mvcresult::setStatus(bool success, uint32_t code, const char *message) {
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
	setStatus(true,code,message);
}

void mvcresult::setFailed() {
	setFailed(0,NULL);
}

void mvcresult::setFailed(uint32_t code) {
	setFailed(code,NULL);
}

void mvcresult::setFailed(uint32_t code, const char *message) {
	setStatus(false,code,message);
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

void mvcresult::setData(const char *name, collection *data) {
	pvt->_data.remove((char *)name);
	pvt->_data.setValue(charstring::duplicate(name),data);
}

void mvcresult::attachData(const char *name, collection *data) {
	setData(name,data);
	pvt->_wb.attach(data);
}

linkedlist<char *> *mvcresult::getKeys() {
	return pvt->_data.getKeys();
}

collection *mvcresult::getData(const char *name) {
	return pvt->_data.getValue((char *)name);
}

ssize_t mvcresult::write() {
	return write(&stdoutput);
}

ssize_t mvcresult::write(output *out) {
	return writeJson(out,true);
}

ssize_t mvcresult::writeJson() {
	return writeJson(true);
}

ssize_t mvcresult::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

ssize_t mvcresult::writeJson(output *out) {
	return writeJson(out,true);
}

ssize_t mvcresult::writeJson(output *out, bool indent) {

	ssize_t	retval=0;

	size_t	mlen=charstring::getLength(pvt->_message);
	char	*code=charstring::parseNumber(pvt->_code);
	size_t	clen=charstring::getLength(code);

	// begin object
	incOrErr(&retval,out->write('{'),1) &&
	((indent)?incOrErr(&retval,out->write("\n")):true) &&

	// success: true/false
	incOrErr(&retval,out->write("\"s\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	((pvt->_success)?incOrErr(&retval,out->write("true",4),4):
			incOrErr(&retval,out->write("false",5),5)) &&
	incOrErr(&retval,out->write(','),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true) &&

	// status code
	incOrErr(&retval,out->write("\"c\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	incOrErr(&retval,out->write(code,clen),clen) &&
	incOrErr(&retval,out->write(','),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true) &&

	// status message
	incOrErr(&retval,out->write("\"m\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	incOrErr(&retval,out->write('"'),1) &&
	incOrErr(&retval,out->write(pvt->_message,mlen),mlen) &&
	incOrErr(&retval,out->write('"'),1) &&
	((pvt->_success && pvt->_data.getKeys()->getFirst())?
			incOrErr(&retval,out->write(','),1):true) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);

	if (pvt->_success) {

		// collections
		bool	first=true;
		for (listnode<char *> *key=
				pvt->_data.getKeys()->getFirst();
				key; key=key->getNext()) {

			ssize_t	klen=charstring::getLength(key->getValue());
			
			if (first) {
				first=false;
			} else {
				incOrErr(&retval,out->write(','),1) &&
				((indent)?incOrErr(&retval,
						out->write('\n'),1):true);
			}
			incOrErr(&retval,out->write('"'),1) &&
			incOrErr(&retval,out->write(
					key->getValue(),klen),klen) &&
			incOrErr(&retval,out->write("\":",2),2) &&
			((indent)?incOrErr(&retval,out->write(' '),1):true);
			collection	*c=
				pvt->_data.getValue(key->getValue());
			if (c) {
				incOrErr(&retval,c->writeJson(out,indent));
			} else {
				incOrErr(&retval,out->write("null",4),4);
			}
		}
		
	}

	// end object
	incOrErr(&retval,out->write('}'),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);

	// clean up
	delete[] code;

	return retval;
}

wastebasket *mvcresult::getWastebasket() {
	return &pvt->_wb;
}
