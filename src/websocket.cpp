// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/websocket.h>

class websocketprivate {
	friend class websocket;
	private:
		bool			_isclient;
};

websocket::websocket() : securitycontext() {
	pvt=new websocketprivate;
	pvt->_isclient=false;
}

websocket::~websocket() {
	delete pvt;
}

void websocket::setFileDescriptor(filedescriptor *fd) {
}

filedescriptor *websocket::getFileDescriptor() {
	return NULL;
}

bool websocket::connect() {
	return false;
}

bool websocket::accept() {
	return false;
}

ssize_t websocket::read(void *buf, ssize_t count) {
	return -1;
}

ssize_t websocket::write(const void *buf, ssize_t count) {
	return -1;
}

ssize_t websocket::pending() {
	return -1;
}

bool websocket::close() {
	return false;
}

ssize_t websocket::getSizeMax() {
	return 0;
}
