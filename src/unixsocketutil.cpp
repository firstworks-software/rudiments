// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketutil.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

class unixsocketutilprivate {
	friend class unixsocketutil;
	private:
		const char	*_filename;
		sockaddr_un	_sun;
};

unixsocketutil::unixsocketutil() {
	pvt=new unixsocketutilprivate;
	init(NULL);
}

unixsocketutil::~unixsocketutil() {
	delete pvt;
}

void unixsocketutil::init(const char *filename) {
	pvt->_filename=filename;
	bytestring::zero(&pvt->_sun,sizeof(pvt->_sun));
}

const char *unixsocketutil::_filename() {
	return pvt->_filename;
}

sockaddr_un *unixsocketutil::_sun() {
	return &pvt->_sun;
}

uint16_t unixsocketutil::filenameToPort(const char *filename) {
	uint16_t	port=charstring::toInteger(filename);
	if (!port) {
		port=10240;
		for (const char *c=filename; *c; c++) {
			port=port+*c;
		}
	}
	return port;
}
