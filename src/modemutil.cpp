// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/modemutil.h>
#include <rudiments/serialport.h>
#include <rudiments/serialportprofile.h>
#include <rudiments/signalclasses.h>
#include <rudiments/file.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <string.h>
#endif

class modemutilprivate {
	friend class modemutil;
	private:
		const char	*_devicename;
		const char	*_baud;
};

modemutil::modemutil() {
	pvt=new modemutilprivate;
	pvt->_devicename="";
	pvt->_baud="";
}

modemutil::~modemutil() {
	delete pvt;
}

void modemutil::setDevice(const char *device) {
	pvt->_devicename=device;
}

void modemutil::setBaud(const char *baud) {
	pvt->_baud=baud;
}

const char *modemutil::getDevice() {
	return pvt->_devicename;
}

const char *modemutil::getBaud() {
	return pvt->_baud;
}

bool modemutil::configureSerialPort(int32_t fd, const char *baud) {

	serialportprofile	spp;
	spp.baud(baud);
	spp.ignoreParityErrors(true);
	spp.hardwareFlowControl(true);
	spp.characterSize(serialportprofile::cs_8);
	spp.receiverOn(true);
	spp.ignoreModemControlLines(true);
	spp.readThreshold(1);
	spp.readTimeout(0);

	// this is kind of lame, this class should somehow
	// inherit from serialport
	serialport	sp;
	sp.setFileDescriptor(fd);
	bool	retval=(sp.flush() && sp.setProfileNow(&spp));
	// set the file descriptor to -1 so it won't get
	// closed when the instance of serialport goes away
	sp.setFileDescriptor(-1);
	return retval;
}
