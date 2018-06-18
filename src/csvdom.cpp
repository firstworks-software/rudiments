// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/csvdom.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>

class csvdomprivate {
	friend class csvdom;
	private:
};

csvdom::csvdom() : csvsax(), dom() {
	init(true);
}

csvdom::csvdom(bool stringcacheenabled) : csvsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

csvdom::csvdom(const csvdom &x) : csvsax(), dom() {
	init(stringCacheEnabled());
}

csvdom &csvdom::operator=(const csvdom &x) {
	if (this!=&x) {
		reset();
	}
	return *this;
}

void csvdom::init(bool stringcacheenabled) {
	pvt=new csvdomprivate;
}

csvdom::~csvdom() {
	delete pvt;
}

void csvdom::reset() {
	dom::reset();
	csvsax::reset();
}

bool csvdom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.open(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=sys::getPageSize();
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);

	bool	retval=true;

	fl.flushWriteBuffer(-1,-1);
	if (!fl.close()) {
		retval=false;
	}
	return retval;
}
