// Copyright (c) David Muse
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
	// Duplicate the name before removing it, in case it is one of the
	// keys managed by _data, which removeData() would free.
	char	*key=charstring::duplicate(name);
	removeData(name);
	pvt->_data.setValue(key,data);
}

void mvcresult::attachData(const char *name, collection *data) {
	setData(name,data);
	pvt->_wb.attach(data);
}

void mvcresult::removeData(const char *name) {
	pvt->_data.remove((char *)name);
}

void mvcresult::clearData() {
	pvt->_data.clear();
}

collection *mvcresult::detachData(const char *name) {
	collection	*data=NULL;
	if (!pvt->_data.getValue((char *)name,&data)) {
		return NULL;
	}
	removeData(name);
	pvt->_wb.detach(data);
	return data;
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

	char	*code=charstring::parseNumber(pvt->_code);
	size_t	clen=charstring::getLength(code);

	// encode the message, a NULL message as ""
	stringbuffer	enc;
	charstring::jsonEncode((pvt->_message)?pvt->_message:"",&enc);

	// begin object
	if (!(incOrErr(&retval,out->write('{'),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	// success: true/false
	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write('\t'),1):true) &&
		incOrErr(&retval,out->write("\"s\":",4),4) &&
		((indent)?incOrErr(&retval,out->write(' '),1):true) &&
		((pvt->_success)?incOrErr(&retval,out->write("true",4),4):
				incOrErr(&retval,out->write("false",5),5)) &&
		incOrErr(&retval,out->write(','),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	// status code
	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write('\t'),1):true) &&
		incOrErr(&retval,out->write("\"c\":",4),4) &&
		((indent)?incOrErr(&retval,out->write(' '),1):true) &&
		incOrErr(&retval,out->write(code,clen),clen) &&
		incOrErr(&retval,out->write(','),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	// status message
	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write('\t'),1):true) &&
		incOrErr(&retval,out->write("\"m\":",4),4) &&
		((indent)?incOrErr(&retval,out->write(' '),1):true) &&
		incOrErr(&retval,out->write(enc.getString(),
						enc.getStringLength()),
						enc.getStringLength()) &&
		((pvt->_success && pvt->_data.getKeys()->getFirst())?
				incOrErr(&retval,out->write(','),1):true) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	if (pvt->_success && retval>-1) {

		// collections
		for (listnode<char *> *key=
				pvt->_data.getKeys()->getFirst();
				key && retval>-1; key=key->getNext()) {

			// key, a NULL key as ""
			enc.clear();
			charstring::jsonEncode((key->getValue())?
						key->getValue():"",&enc);
			if (!(((indent)?incOrErr(&retval,
						out->write('\t'),1):true) &&
				incOrErr(&retval,out->write(enc.getString(),
						enc.getStringLength()),
						enc.getStringLength()) &&
				incOrErr(&retval,out->write(':'),1) &&
				((indent)?incOrErr(&retval,
						out->write(' '),1):true)) &&
				retval>-1) {
				retval=RESULT_ERROR;
			}
			if (retval<0) {
				break;
			}

			// value
			collection	*c=
				pvt->_data.getValue(key->getValue());
			if (!c) {
				if (!incOrErr(&retval,
						out->write("null",4),4) &&
						retval>-1) {
					retval=RESULT_ERROR;
				}
			} else if (!indent) {
				ssize_t	r=c->writeJson(out,false);
				if (r<0) {
					retval=r;
				} else {
					retval+=r;
				}
			} else {

				// indent the collection one more level
				stringbuffer	cbuf;
				ssize_t	r=c->writeJson(&cbuf,true);
				if (r<0) {
					retval=r;
					break;
				}
				const char	*start=cbuf.getString();
				const char	*end=start+
						cbuf.getStringLength();
				if (end!=start && *(end-1)=='\n') {
					end--;
				}
				enc.clear();
				for (const char *ch=start; ch!=end; ch++) {
					if (*ch=='\n') {
						enc.append(start,ch-start+1);
						enc.append('\t');
						start=ch+1;
					}
				}
				enc.append(start,end-start);
				if (!incOrErr(&retval,
						out->write(enc.getString(),
						enc.getStringLength()),
						enc.getStringLength()) &&
						retval>-1) {
					retval=RESULT_ERROR;
				}
			}

			// separator
			if (!(retval>-1 &&
				((key->getNext())?incOrErr(&retval,
						out->write(','),1):true) &&
				((indent)?incOrErr(&retval,
						out->write('\n'),1):true)) &&
				retval>-1) {
				retval=RESULT_ERROR;
			}
		}

	}

	// end object
	if (!(retval>-1 &&
		incOrErr(&retval,out->write('}'),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}

	// clean up
	delete[] code;

	return retval;
}

wastebasket *mvcresult::getWastebasket() {
	return &pvt->_wb;
}
