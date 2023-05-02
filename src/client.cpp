// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/client.h>

#include <rudiments/charstring.h>

class clientprivate {
	friend class client;
	private:
		char		*_connecterror;
		int32_t		_timeoutsec;
		int32_t		_timeoutusec;
		uint32_t	_retrywait;
		uint32_t	_tries;
};

client::client() : filedescriptor() {
	pvt=new clientprivate;
	pvt->_connecterror=NULL;
	pvt->_timeoutsec=-1;
	pvt->_timeoutusec=-1;
	pvt->_retrywait=0;
	pvt->_tries=1;
	type("client");
}

client::~client() {
	delete[] pvt->_connecterror;
	delete pvt;
}

void client::setParameters(dictionary<const char *, const char *> *cd) {

	if (cd) {
		const char	*timeoutsec=NULL;
		cd->getValue("timeoutsec",&timeoutsec);
		const char	*timeoutusec=NULL;
		cd->getValue("timeoutusec",&timeoutusec);
		const char	*retrywait=NULL;
		cd->getValue("retrywait",&retrywait);
		const char	*tries=NULL;
		cd->getValue("tries",&tries);

		setTimeoutSeconds(charstring::convertToInteger(
					(timeoutsec)?timeoutsec:"0"));
		setTimeoutMicroseconds(charstring::convertToInteger(
					(timeoutusec)?timeoutusec:"0"));
		setRetryWait(charstring::convertToUnsignedInteger(
					(retrywait)?retrywait:"0"));
		setTries(charstring::convertToUnsignedInteger(
					(tries)?tries:"0"));
	}
}

void client::setTimeoutSeconds(int32_t timeoutsec) {
	pvt->_timeoutsec=timeoutsec;
}

void client::setTimeoutMicroseconds(int32_t timeoutusec) {
	pvt->_timeoutusec=timeoutusec;
}

void client::setRetryWait(uint32_t retrywait) {
	pvt->_retrywait=retrywait;
}

void client::setTries(uint32_t tries) {
	pvt->_tries=tries;
}

void client::setVerboseConnectError(const char *error) {
	delete[] pvt->_connecterror;
	pvt->_connecterror=charstring::duplicate(error);
}

int32_t client::getTimeoutSeconds() {
	return pvt->_timeoutsec;
}

int32_t client::getTimeoutMicroseconds() {
	return pvt->_timeoutusec;
}

uint32_t client::getRetryWait() {
	return pvt->_retrywait;
}

uint32_t client::getTries() {
	return pvt->_tries;
}

const char *client::getVerboseConnectError() {
	return pvt->_connecterror;
}

int32_t client::connect() {
	// by default, do nothing, return success
	return RESULT_SUCCESS;
}
