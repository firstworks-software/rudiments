// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/server.h>

class serverprivate {
	friend class server;
	private:
		uint32_t	_backlog;
};

server::server() : filedescriptor() {
	pvt=new serverprivate;
	pvt->_backlog=15;
	type("server");
}

server::~server() {
	delete pvt;
}

void server::setBacklog(uint32_t backlog) {
	pvt->_backlog=backlog;
}

uint32_t server::getBacklog() {
	return pvt->_backlog;
}

bool server::bind() {
	// by default, just return success
	return true;
}

bool server::listen() {
	// by default, just return success
	return true;
}

filedescriptor *server::accept() {
	// create a new filedescriptor and set it to the fd() from this instance
	filedescriptor	*retval=new filedescriptor;
	retval->setFileDescriptor(fd());
	return retval;
}
