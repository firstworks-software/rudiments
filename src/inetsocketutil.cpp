// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketutil.h>
#include <rudiments/bytestring.h>

class inetsocketutilprivate {
	friend class inetsocketutil;
	private:
		const char	*_address;
		uint16_t	_port;
		sockaddr_in	_sin;
};

inetsocketutil::inetsocketutil() {
	pvt=new inetsocketutilprivate;
	init(NULL,0);
}

inetsocketutil::~inetsocketutil() {
	delete pvt;
}

void inetsocketutil::init(const char *address, uint16_t port) {
	pvt->_address=address;
	pvt->_port=port;
	bytestring::zero(&pvt->_sin,sizeof(pvt->_sin));
}

const char *inetsocketutil::_address() {
	return pvt->_address;
}

uint16_t *inetsocketutil::_port() {
	return &pvt->_port;
}

sockaddr_in *inetsocketutil::_sin() {
	return &pvt->_sin;
}
