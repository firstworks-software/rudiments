// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketutil.h>
#include <rudiments/bytestring.h>

class inetsocketutilprivate {
	friend class inetsocketutil;
	private:
		const char	*_host;
		uint16_t	_port;
		sockaddr_in	_sin;
};

inetsocketutil::inetsocketutil() {
	pvt=new inetsocketutilprivate;
	pvt->_host=NULL;
	pvt->_port=0;
	bytestring::zero(&pvt->_sin,sizeof(pvt->_sin));
}

inetsocketutil::~inetsocketutil() {
	delete pvt;
}

void inetsocketutil::setHost(const char *host) {
	pvt->_host=host;
}

void inetsocketutil::setPort(uint16_t port) {
	pvt->_port=port;
}

const char *inetsocketutil::getHost() {
	return pvt->_host;
}

uint16_t inetsocketutil::getPort() {
	return pvt->_port;
}

sockaddr_in *inetsocketutil::getSin() {
	return &pvt->_sin;
}
