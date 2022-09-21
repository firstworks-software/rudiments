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
	setParameters(NULL,-1,-1,0,0);
}

client::~client() {
	delete[] pvt->_connecterror;
	delete pvt;
}

void client::setParameters(char *connecterror,
				int32_t timeoutsec, int32_t timeoutusec,
				uint32_t retrywait, uint32_t tries) {
	pvt->_connecterror=connecterror;
	pvt->_timeoutsec=timeoutsec;
	pvt->_timeoutusec=timeoutusec;
	pvt->_retrywait=retrywait;
	pvt->_tries=tries;
	type("client");
}

void client::setParameters(dictionary<const char *, const char *> *cd) {
	// by default, do nothing
}

int32_t client::connect() {
	// by default, do nothing, return success
	return RESULT_SUCCESS;
}

const char *client::getVerboseConnectError() {
	return pvt->_connecterror;
}

void client::setVerboseConnectError(const char *error) {
	delete[] pvt->_connecterror;
	pvt->_connecterror=charstring::duplicate(error);
}

char **client::_connecterror() {
	return &pvt->_connecterror;
}

int32_t client::_timeoutsec() {
	return pvt->_timeoutsec;
}

int32_t client::_timeoutusec() {
	return pvt->_timeoutusec;
}

uint32_t client::_retrywait() {
	return pvt->_retrywait;
}

uint32_t client::_tries() {
	return pvt->_tries;
}
