// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvcresult.h>
#include <rudiments/dictionary.h>

class mvcresultprivate {
	friend class mvcresult;
	private:
		bool		_success;
		uint32_t	_code;
		char		*_message;

		dictionary<char *, char *>		_types;
		dictionary<char *, collection *>	_data;

		wastebasket	_wb;
		
};

mvcresult::mvcresult() : object() {
	pvt=new mvcresultprivate;
	pvt->_success=false;
	pvt->_code=0;
	pvt->_message=NULL;
	pvt->_types.setManageArrayKeys(true);
	pvt->_types.setManageArrayValues(true);
	pvt->_data.setManageArrayKeys(true);
	pvt->_data.setManageValues(true);
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

void mvcresult::attachData(const char *name,
				const char *type,
				collection *data) {

	// remove any existing type for this name
	pvt->_types.remove((char *)name);

	// remove any existing data for this name
	pvt->_data.remove((char *)name);
	
	// set type
	pvt->_types.setValue(charstring::duplicate(name),
				charstring::duplicate(type));

	// attach data
	pvt->_data.setValue(charstring::duplicate(name),data);
}

linkedlist<char *> *mvcresult::getKeys() {
	return pvt->_data.getKeys();
}

const char *mvcresult::getType(const char *name) {
	return pvt->_types.getValue((char *)name);
}

collection *mvcresult::getData(const char *name) {
	return pvt->_data.getValue((char *)name);
}

wastebasket *mvcresult::getWastebasket() {
	return &pvt->_wb;
}
