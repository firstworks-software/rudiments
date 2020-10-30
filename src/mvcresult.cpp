// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvcresult.h>
#include <rudiments/jsondom.h>

class mvcresultprivate {
	friend class mvcresult;
	private:
		jsondom		d;
		domnode		*statusnode;
		domnode		*successnode;
		domnode		*codenode;
		domnode		*messagenode;
		domnode		*datanode;
};

mvcresult::mvcresult() {
	pvt=new mvcresultprivate;
	pvt->d.createRootNode();
	domnode		*rootnode=pvt->d.getRootNode()->appendTag("r");
	pvt->statusnode=rootnode->appendTag("status");
	pvt->successnode=pvt->statusnode->appendTag("success");
	pvt->codenode=pvt->statusnode->appendTag("code");
	pvt->codenode->setAttributeValue("t","n");
	pvt->messagenode=pvt->statusnode->appendTag("message");
	pvt->messagenode->setAttributeValue("t","s");
	pvt->datanode=rootnode->appendTag("data");
}

mvcresult::~mvcresult() {
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
	pvt->successnode->setAttributeValue("value",(success)?"true":"false");
}

bool mvcresult::getSuccess() {
	return !charstring::compare(
			pvt->successnode->getAttributeValue("value"),"true");
}

void mvcresult::setCode(uint32_t code) {
	pvt->codenode->setAttributeValue("value",(uint64_t)code);
}

uint32_t mvcresult::getCode() {
	return charstring::toUnsignedInteger(
				pvt->codenode->getAttributeValue("value"));
}

void mvcresult::setMessage(const char *message) {
	pvt->messagenode->setAttributeValue("value",message);
}

const char *mvcresult::getMessage() {
	return pvt->messagenode->getAttributeValue("value");
}

domnode *mvcresult::getDataNode() {
	return pvt->datanode;
}

dom *mvcresult::getDom() {
	return &pvt->d;
}
