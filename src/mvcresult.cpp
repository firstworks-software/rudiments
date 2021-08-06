// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvcresult.h>
#include <rudiments/dictionary.h>

class mvcresultprivate {
	friend class mvcresult;
	private:
		bool		success;
		uint32_t	code;
		char		*message;

		dictionary<char *, char *>		types;
		dictionary<char *, collection *>	data;

		linkedlist<object *>	objects;
		
};

mvcresult::mvcresult() : object() {
	pvt=new mvcresultprivate;
	pvt->success=false;
	pvt->code=0;
	pvt->message=NULL;
	pvt->types.setManageArrayKeys(true);
	pvt->types.setManageArrayValues(true);
	pvt->data.setManageArrayKeys(true);
	pvt->data.setManageValues(true);
	pvt->objects.setManageValues(true);
}

mvcresult::~mvcresult() {
	delete[] pvt->message;
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
	pvt->success=success;
}

bool mvcresult::getSuccess() {
	return pvt->success;
}

void mvcresult::setCode(uint32_t code) {
	pvt->code=code;
}

uint32_t mvcresult::getCode() {
	return pvt->code;
}

void mvcresult::setMessage(const char *message) {
	delete[] pvt->message;
	pvt->message=charstring::duplicate(message);
}

const char *mvcresult::getMessage() {
	return pvt->message;
}

void mvcresult::attachData(const char *name,
				const char *type,
				collection *data) {

	// remove any existing type for this name
	pvt->types.remove((char *)name);

	// remove any existing data for this name
	pvt->data.remove((char *)name);
	
	// set type
	pvt->types.setValue(charstring::duplicate(name),
				charstring::duplicate(type));

	// attach data
	pvt->data.setValue(charstring::duplicate(name),data);
}

linkedlist<char *> *mvcresult::getKeys() {
	return pvt->data.getKeys();
}

const char *mvcresult::getType(const char *name) {
	return pvt->types.getValue((char *)name);
}

collection *mvcresult::getData(const char *name) {
	return pvt->data.getValue((char *)name);
}

void mvcresult::attachObject(object *obj) {
	pvt->objects.append(obj);
}
