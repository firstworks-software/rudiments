// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/serialportprofile.h>
#include <rudiments/bytestring.h>
#include <rudiments/character.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stdio.h>

class serialportprofileprivate {
	friend class serialportprofile;
	private:
		termios	_tio;
};

serialportprofile::serialportprofile() : object() {
	pvt=new serialportprofileprivate;
	setDefaultOptions();
}

serialportprofile::~serialportprofile() {
	delete pvt;
}

void serialportprofile::setControlCharacters(const byte_t *c_cc) {
	bytestring::copy(&pvt->_tio.c_cc,c_cc,sizeof(byte_t)*NCCS);
}

void serialportprofile::setDefaultControlOptions() {
	pvt->_tio.c_cflag=0;
}

void serialportprofile::setDefaultLocalOptions() {
	pvt->_tio.c_lflag=0;
}

void serialportprofile::setDefaultInputOptions() {
	pvt->_tio.c_iflag=0;
}

void serialportprofile::setDefaultOutputOptions() {
	pvt->_tio.c_oflag=0;
}

void serialportprofile::setDefaultControlCharacters() {
	bytestring::zero(&pvt->_tio.c_cc,sizeof(pvt->_tio.c_cc));
}

void serialportprofile::setDefaultOptions() {
	bytestring::zero(&pvt->_tio,sizeof(pvt->_tio));
}

static tcflag_t	br[]={
	B0,
	B50,
	B75,
	B110,
	B134,
	B150,
	B200,
	B300,
	B600,
	B1200,
	B1800,
	B2400,
	B4800,
	B9600,
	#if defined(B19200)
	B19200,
	#elif defined(EXTA)
	EXTA,
	#endif
	#if defined(B38400)
	B38400,
	#elif defined(EXTB)
	EXTB,
	#endif
	#ifdef B57600
	B57600,
	#endif
	#ifdef B76800
	B76800,
	#endif
	#ifdef B115200
	B115200,
	#endif
	#ifdef B230400
	B230400,
	#endif
	#ifdef B460800
	B460800,
	#endif
	#ifdef B500000
	B500000,
	#endif
	#ifdef B576000
	B576000,
	#endif
	#ifdef B921600
	B921600,
	#endif
	#ifdef B1000000
	B1000000,
	#endif
	#ifdef B1142000
	B1152000,
	#endif
	#ifdef B1500000
	B1500000,
	#endif
	#ifdef B2000000
	B2000000,
	#endif
	#ifdef B2500000
	B2500000,
	#endif
	#ifdef B3000000
	B3000000,
	#endif
	#ifdef B3500000
	B3500000,
	#endif
	#ifdef B4000000
	B4000000,
	#endif
};

bool serialportprofile::setInputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetispeed(&pvt->_tio,br[baudrate]);
}

bool serialportprofile::setOutputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetospeed(&pvt->_tio,br[baudrate]);
}

void serialportprofile::setBaud(serialportprofile::baudrate_t baudrate) {

	#ifdef CBAUD
		#ifdef CBAUDEX
		pvt->_tio.c_cflag&=~(CBAUD|CBAUDEX);
		#else
		pvt->_tio.c_cflag&=~CBAUD;
		#endif
	#else
		pvt->_tio.c_cflag&=~(B0|B50|B75|B110|B134|B150|B200|B300|
				B600|B1200|B1800|B2400|B4800|B9600
				#if defined(B19200)
				|B19200
				#elif defined(EXTA)
				|EXTA
				#endif
				#if defined(B38400)
				|B38400
				#elif defined(EXTB)
				|EXTB
				#endif
				#ifdef B57600
				|B57600
				#endif
				#ifdef B76800
				|B76800
				#endif
				#ifdef B115200
				|B115200
				#endif
				#ifdef B230400
				|B230400
				#endif
				#ifdef B460800
				|B460800
				#endif
				#ifdef B500000
				|B500000
				#endif
				#ifdef B576000
				|B576000
				#endif
				#ifdef B921600
				|B921600
				#endif
				#ifdef B1000000
				|B1000000
				#endif
				#ifdef B1142000
				|B1152000
				#endif
				#ifdef B1500000
				|B1500000
				#endif
				#ifdef B2000000
				|B2000000
				#endif
				#ifdef B2500000
				|B2500000
				#endif
				#ifdef B3000000
				|B3000000
				#endif
				#ifdef B3500000
				|B3500000
				#endif
				#ifdef B4000000
				|B4000000
				#endif
				);
	#endif
	pvt->_tio.c_cflag|=br[baudrate];
}

void serialportprofile::setCharacterSize(serialportprofile::charsize_t size) {
	pvt->_tio.c_cflag&=~CSIZE;
	switch (size) {
		case cs_5:
			pvt->_tio.c_cflag|=CS5;
			return;
		case cs_6:
			pvt->_tio.c_cflag|=CS6;
			return;
		case cs_7:
			pvt->_tio.c_cflag|=CS7;
			return;
		case cs_8:
			pvt->_tio.c_cflag|=CS8;
			return;
	}
	return;
}

#define SET_FLAG(truefalse,flag,value) \
	if (truefalse) { pvt->_tio.flag|=value; } else { pvt->_tio.flag&=~value; }

#define SET_CHAR(character,value) \
	pvt->_tio.c_cc[character]=value;

#define GET_FLAG(flag,value) (pvt->_tio.flag&value)

#define GET_CHAR(character) (pvt->_tio.c_cc[character])

void serialportprofile::setTwoStopBits(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CSTOPB)
}

void serialportprofile::setReceiverOn(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CREAD)
}

void serialportprofile::setParityCheck(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARENB)
}

void serialportprofile::setOddParity(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARODD)
}

void serialportprofile::setHangupOnClose(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,HUPCL)
}

void serialportprofile::setIgnoreModemControlLines(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CLOCAL)
}

void serialportprofile::setBlockJobControlOutput(bool truefalse) {
	#ifdef LOBLK
		SET_FLAG(truefalse,c_cflag,LOBLK)
	#endif
}

void serialportprofile::setHardwareFlowControl(bool truefalse) {
	#if defined(CRTSCTS)
		SET_FLAG(truefalse,c_cflag,CRTSCTS)
	#elif defined(NEW_CRTSCTS)
		SET_FLAG(truefalse,c_cflag,NEW_CRTSCTS)
	#elif defined(CRTS_IFLOW)
		SET_FLAG(truefalse,c_cflag,CRTS_IFLOW)
	#elif defined(CCTS_OFLOW)
		SET_FLAG(truefalse,c_cflag,CRTS_IFLOW)
	#endif
}

// FIXME:
//	CDTRCTS - DTR/CTS full-duplex flow control
//	MDMBUF - DTR/DCD hardware flow control
//	CHWFLOW - (MDMBUF|CRTSCTS|CDTRCTS)

serialportprofile::baudrate_t serialportprofile::getBaud() {
	#ifdef CBAUD
		return (baudrate_t)GET_FLAG(c_cflag,CBAUD);
	#else
		tcflag_t	cbaud=
				(B0|B50|B75|B110|B134|B150|B200|B300|
				B600|B1200|B1800|B2400|B4800|B9600
				#if defined(B19200)
				|B19200
				#elif defined(EXTA)
				|EXTA
				#endif
				#if defined(B38400)
				|B38400
				#elif defined(EXTB)
				|EXTB
				#endif
				#ifdef B57600
				|B57600
				#endif
				#ifdef B76800
				|B76800
				#endif
				#ifdef B115200
				|B115200
				#endif
				#ifdef B230400
				|B230400
				#endif
				#ifdef B460800
				|B460800
				#endif
				#ifdef B500000
				|B500000
				#endif
				#ifdef B576000
				|B576000
				#endif
				#ifdef B921600
				|B921600
				#endif
				#ifdef B1000000
				|B1000000
				#endif
				#ifdef B1142000
				|B1152000
				#endif
				#ifdef B1500000
				|B1500000
				#endif
				#ifdef B2000000
				|B2000000
				#endif
				#ifdef B2500000
				|B2500000
				#endif
				#ifdef B3000000
				|B3000000
				#endif
				#ifdef B3500000
				|B3500000
				#endif
				#ifdef B4000000
				|B4000000
				#endif
				);
		return (baudrate_t)GET_FLAG(c_cflag,cbaud);
	#endif
}

serialportprofile::baudrate_t serialportprofile::getInputBaud() {
	return (serialportprofile::baudrate_t)cfgetispeed(&pvt->_tio);
}

serialportprofile::baudrate_t serialportprofile::getOutputBaud() {
	return (serialportprofile::baudrate_t)cfgetospeed(&pvt->_tio);
}

serialportprofile::charsize_t serialportprofile::getCharacterSize() {
	switch (GET_FLAG(c_cflag,CSIZE)) {
		// Haiku only supports CS7/CS8 and defines CS5,6 and 7 to 0x00
		#if CS5!=CS6
		case CS5:
			return cs_5;
		#endif
		#if CS6!=CS7
		case CS6:
			return cs_6;
		#endif
		case CS7:
			return cs_7;
	}
	return cs_8;
}

bool serialportprofile::getTwoStopBits() {
	return GET_FLAG(c_cflag,CSTOPB);
}

bool serialportprofile::getReceiverOn() {
	return GET_FLAG(c_cflag,CREAD);
}

bool serialportprofile::getParityCheck() {
	return GET_FLAG(c_cflag,PARENB);
}

bool serialportprofile::getOddParity() {
	return GET_FLAG(c_cflag,PARODD);
}

bool serialportprofile::getHangupOnClose() {
	return GET_FLAG(c_cflag,HUPCL);
}

bool serialportprofile::getIgnoreModemControlLines() {
	return GET_FLAG(c_cflag,CLOCAL);
}

bool serialportprofile::getBlockJobControlOutput() {
	#ifdef LOBLK
		return GET_FLAG(c_cflag,LOBLK);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getHardwareFlowControl() {
	#if defined(CRTSCTS)
		return GET_FLAG(c_cflag,CRTSCTS);
	#elif defined(NEW_CRTSCTS)
		return GET_FLAG(c_cflag,NEW_CRTSCTS);
	#elif defined(CRTS_IFLOW)
		return GET_FLAG(c_cflag,CRTS_IFLOW);
	#elif defined(CCTS_OFLOW)
		return GET_FLAG(c_cflag,CRTS_OFLOW);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void serialportprofile::setGenerateSignals(bool truefalse){
	SET_FLAG(truefalse,c_lflag,ISIG)
}

void serialportprofile::setCanonicalInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ICANON)
}

void serialportprofile::setEscapedUpperCase(bool truefalse) {
	#ifdef XCASE
		SET_FLAG(truefalse,c_lflag,XCASE)
	#endif
}

void serialportprofile::setEchoInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHO)
}

void serialportprofile::setEraseCharactersOn(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOE)
}

void serialportprofile::setKillCharacterOn(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOK)
}

void serialportprofile::setEchoNewLine(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHONL)
}

void serialportprofile::setExtendedFunctions(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,IEXTEN)
}

void serialportprofile::setEchoControlCharacters(bool truefalse) {
	#ifdef ECHOCTL
		SET_FLAG(truefalse,c_lflag,ECHOCTL)
	#endif
}

void serialportprofile::setEchoErasedCharacter(bool truefalse) {
	#ifdef ECHOPRT
		SET_FLAG(truefalse,c_lflag,ECHOPRT)
	#endif
}

void serialportprofile::setEmulateKill(bool truefalse) {
	#ifdef ECHOKE
		SET_FLAG(truefalse,c_lflag,ECHOKE)
	#endif
}

void serialportprofile::setNoFlushAfterInterruptOrQuit(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,NOFLSH)
}

void serialportprofile::setRetypePendingCharacters(bool truefalse) {
	#ifdef PENDIN
		SET_FLAG(truefalse,c_lflag,PENDIN)
	#endif
}

void serialportprofile::setSendSignalForBackgroundOutput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,TOSTOP)
}

bool serialportprofile::getGenerateSignals() {
	return GET_FLAG(c_lflag,ISIG);
}

bool serialportprofile::getCanonicalInput() {
	return GET_FLAG(c_lflag,ICANON);
}

bool serialportprofile::getEscapedUpperCase() {
	#ifdef XCASE
		return GET_FLAG(c_lflag,XCASE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getEchoInput() {
	return GET_FLAG(c_lflag,ECHO);
}

bool serialportprofile::getEraseCharactersOn() {
	return GET_FLAG(c_lflag,ECHOE);
}

bool serialportprofile::getKillCharacterOn() {
	return GET_FLAG(c_lflag,ECHOK);
}

bool serialportprofile::getEchoNewLine() {
	return GET_FLAG(c_lflag,ECHONL);
}

bool serialportprofile::getExtendedFunctions() {
	return GET_FLAG(c_lflag,IEXTEN);
}

bool serialportprofile::getEchoControlCharacters() {
	#ifdef ECHOCTL
		return GET_FLAG(c_lflag,ECHOCTL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getEchoErasedCharacter() {
	#ifdef ECHOPRT
		return GET_FLAG(c_lflag,ECHOPRT);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getEmulateKill() {
	#ifdef ECHOKE
		return GET_FLAG(c_lflag,ECHOKE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getNoFlushAfterInterruptOrQuit() {
	return GET_FLAG(c_lflag,NOFLSH);
}

bool serialportprofile::getRetypePendingCharacters() {
	#ifdef PENDIN
		return GET_FLAG(c_lflag,PENDIN);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getSendSignalForBackgroundOutput() {
	return GET_FLAG(c_lflag,TOSTOP);
}

void serialportprofile::setInputParityCheck(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INPCK)
}

void serialportprofile::setIgnoreParityErrors(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNPAR)
}

void serialportprofile::setMarkParityErrors(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,PARMRK)
}

void serialportprofile::setStripParityBits(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ISTRIP)
}

void serialportprofile::setSoftwareFlowControlOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXON)
}

void serialportprofile::setSoftwareFlowControlOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXOFF)
}

void serialportprofile::setAnyCharacterStartsFlow(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXANY)
}

void serialportprofile::setIgnoreBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNBRK)
}

void serialportprofile::setSendSignalOnBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,BRKINT)
}

void serialportprofile::setMapNewLineToCarriageReturnOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INLCR)
}

void serialportprofile::setIgnoreCarriageReturn(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNCR)
}

void serialportprofile::setMapCarriageReturnToNewLineOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ICRNL)
}

void serialportprofile::setLowerCase(bool truefalse) {
	#ifdef IUCLC
		SET_FLAG(truefalse,c_iflag,IUCLC)
	#endif
}

void serialportprofile::setBellIfLineTooLong(bool truefalse) {
	#ifdef IMAXBEL
		SET_FLAG(truefalse,c_iflag,IMAXBEL)
	#endif
}

bool serialportprofile::getInputParityCheck() {
	return GET_FLAG(c_iflag,INPCK);
}

bool serialportprofile::getIgnoreParityErrors() {
	return GET_FLAG(c_iflag,IGNPAR);
}

bool serialportprofile::getMarkParityErrors() {
	return GET_FLAG(c_iflag,PARMRK);
}

bool serialportprofile::getStripParityBits() {
	return GET_FLAG(c_iflag,ISTRIP);
}

bool serialportprofile::getSoftwareFlowControlOnOutput() {
	return GET_FLAG(c_iflag,IXON);
}

bool serialportprofile::getSoftwareFlowControlOnInput() {
	return GET_FLAG(c_iflag,IXOFF);
}

bool serialportprofile::getAnyCharacterStartsFlow() {
	return GET_FLAG(c_iflag,IXANY);
}

bool serialportprofile::getIgnoreBreak() {
	return GET_FLAG(c_iflag,IGNBRK);
}

bool serialportprofile::getSendSignalOnBreak() {
	return GET_FLAG(c_iflag,BRKINT);
}

bool serialportprofile::getMapNewLineToCarriageReturnOnInput() {
	return GET_FLAG(c_iflag,INLCR);
}

bool serialportprofile::getIgnoreCarriageReturn() {
	return GET_FLAG(c_iflag,IGNCR);
}

bool serialportprofile::getMapCarriageReturnToNewLineOnInput() {
	return GET_FLAG(c_iflag,ICRNL);
}

bool serialportprofile::getLowerCase() {
	#ifdef IUCLC
		return GET_FLAG(c_iflag,IUCLC);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getBellIfLineTooLong() {
	#ifdef IMAXBEL
		return GET_FLAG(c_iflag,IMAXBEL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void serialportprofile::setPostProcessOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OPOST)
}

void serialportprofile::setOutputUpperCase(bool truefalse) {
	#ifdef OLCUC
		SET_FLAG(truefalse,c_oflag,OLCUC)
	#endif
}

void serialportprofile::setMapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLCR)
}

void serialportprofile::setDiscardEndOfTransmission(bool truefalse) {
	#ifdef ONOEOT
		SET_FLAG(truefalse,c_oflag,ONOEOT)
	#endif
}

void serialportprofile::setMapCarriageReturnToNewLineOnOutput(bool truefalse) {
	#ifdef OCRNL
		SET_FLAG(truefalse,c_oflag,OCRNL)
	#endif
}

void serialportprofile::setDontOutputCarriageReturnAtColumnZero(
							bool truefalse) {
	#ifdef ONOCR
		SET_FLAG(truefalse,c_oflag,ONOCR)
	#endif
}

void serialportprofile::setMapNewLineToCarriageReturnOnOutput(bool truefalse) {
	#ifdef ONLRET
		SET_FLAG(truefalse,c_oflag,ONLRET)
	#endif
}

void serialportprofile::setUseFillCharactersForDelay(bool truefalse) {
	#ifdef OFILL
		SET_FLAG(truefalse,c_oflag,OFILL)
	#endif
}

void serialportprofile::setUseDelForFill(bool truefalse) {
	#ifdef OFDEL
		SET_FLAG(truefalse,c_oflag,OFDEL)
	#endif
}

void serialportprofile::setExpandTabToSpaces(bool truefalse) {
	#if defined(XTABS)
		SET_FLAG(truefalse,c_oflag,XTABS)
	#elif defined(OXTABS)
		SET_FLAG(truefalse,c_oflag,OXTABS)
	#else
		SET_FLAG(truefalse,c_oflag,TAB3)
	#endif
}

void serialportprofile::setDelayAfterNewLine(
		serialportprofile::newlinedelay_t nldelay) {
	#ifdef NLDLY
		static tcflag_t	nld[]={NL0,NL1};
		pvt->_tio.c_oflag&=~NLDLY;
		pvt->_tio.c_oflag|=nld[nldelay];
	#endif
}

void serialportprofile::setDelayAfterCarriageReturn(
		serialportprofile::carriagereturndelay_t crdelay) {
	#ifdef CRDLY
		static tcflag_t	crd[]={CR0,CR1,CR2,CR3};
		pvt->_tio.c_oflag&=~CRDLY;
		pvt->_tio.c_oflag|=crd[crdelay];
	#endif
}

void serialportprofile::setDelayAfterTab(
		serialportprofile::tabdelay_t tabdelay) {
	#ifdef TABDLY
		static tcflag_t td[]={
			#ifdef TAB0
			TAB0,
			#endif
			#ifdef TAB1
			TAB1,
			#endif
			#ifdef TAB2
			TAB2,
			#endif
			#ifdef TAB3
			TAB3
			#endif
			};
		pvt->_tio.c_oflag&=~TABDLY;
		pvt->_tio.c_oflag|=td[tabdelay];
	#endif
}

void serialportprofile::setDelayAfterBackSpace(
		serialportprofile::backspacedelay_t bsdelay) {
	#ifdef BSDLY
		static tcflag_t	bsd[]={BS0,BS1};
		pvt->_tio.c_oflag&=~BSDLY;
		pvt->_tio.c_oflag|=bsd[bsdelay];
	#endif
}

void serialportprofile::setDelayAfterVerticalTab(
		serialportprofile::verticaltabdelay_t vtdelay) {
	#ifdef VTDLY
		static tcflag_t	vtd[]={VT0,VT1};
		pvt->_tio.c_oflag&=~VTDLY;
		pvt->_tio.c_oflag|=vtd[vtdelay];
	#endif
}

void serialportprofile::setDelayAfterFormFeed(
		serialportprofile::formfeeddelay_t ffdelay) {
	#ifdef FFDLY
		static tcflag_t	ffd[]={FF0,FF1};
		pvt->_tio.c_oflag&=~FFDLY;
		pvt->_tio.c_oflag|=ffd[ffdelay];
	#endif
}

bool serialportprofile::getPostProcessOutput() {
	return GET_FLAG(c_oflag,OPOST);
}

bool serialportprofile::getOutputUpperCase() {
	#ifdef OLCUC
		return GET_FLAG(c_oflag,OLCUC);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getMapNewLineToCarriageReturnNewLineOnOutput() {
	return GET_FLAG(c_oflag,ONLCR);
}

bool serialportprofile::getDiscardEndOfTransmission() {
	#ifdef ONOEOT
		return GET_FLAG(c_oflag,ONOEOT);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getMapCarriageReturnToNewLineOnOutput() {
	#ifdef OCRNL
		return GET_FLAG(c_oflag,OCRNL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getDontOutputCarriageReturnAtColumnZero() {
	#ifdef ONOCR
		return GET_FLAG(c_oflag,ONOCR);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getMapNewLineToCarriageReturnOnOutput() {
	#ifdef ONLRET
		return GET_FLAG(c_oflag,ONLRET);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getUseFillCharactersForDelay() {
	#ifdef OFILL
		return GET_FLAG(c_oflag,OFILL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getUseDelForFill() {
	#ifdef OFDEL
		return GET_FLAG(c_oflag,OFDEL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool serialportprofile::getExpandTabToSpaces() {
	#if defined(XTABS)
		return GET_FLAG(c_oflag,XTABS);
	#elif defined(OXTABS)
		return GET_FLAG(c_oflag,OXTABS);
	#else
		return GET_FLAG(c_oflag,TAB3);
	#endif
}

serialportprofile::newlinedelay_t
		serialportprofile::getDelayAfterNewLine() {
	#ifdef NLDLY
		if (GET_FLAG(c_cflag,NLDLY)==NL1) {
			return nl_100;
		}
	#endif
	return nl_none;
}

serialportprofile::carriagereturndelay_t
		serialportprofile::getDelayAfterCarriageReturn() {
	#ifdef CRDLY
		switch (GET_FLAG(c_cflag,CRDLY)) {
			case CR1:
				return cr_depends;
			case CR2:
				return cr_100;
			case CR3:
				return cr_150;
		}
	#endif
	return cr_none;
}

serialportprofile::tabdelay_t
		serialportprofile::getDelayAfterTab() {
	#ifdef TABDLY
		switch (GET_FLAG(c_cflag,TABDLY)) {
			#ifdef TAB1
			case TAB1:
				return td_1;
			#endif
			#ifdef TAB2
			case TAB2:
				return td_2;
			#endif
			#ifdef TAB3
			case TAB3:
				return td_xtabs;
			#endif
		}
	#endif
	return td_0;
}

serialportprofile::backspacedelay_t
		serialportprofile::getDelayAfterBackSpace() {
	#ifdef BSDLY
		if (GET_FLAG(c_cflag,BSDLY)==BS1) {
			return bs_50;
		}
	#endif
	return bs_none;
}

serialportprofile::verticaltabdelay_t
		serialportprofile::getDelayAfterVerticalTab() {
	#ifdef VTDLY
		if (GET_FLAG(c_cflag,VTDLY)==VT1) {
			return vt_2;
		}
	#endif
	return vt_none;
}

serialportprofile::formfeeddelay_t
		serialportprofile::getDelayAfterFormFeed() {
	#ifdef FFDLY
		if (GET_FLAG(c_cflag,FFDLY)==FF1) {
			return ff_2;
		}
	#endif
	return ff_none;
}

void serialportprofile::setInterruptCharacter(byte_t character) {
	SET_CHAR(VINTR,character)
}

void serialportprofile::setQuitCharacter(byte_t character) {
	SET_CHAR(VQUIT,character)
}

void serialportprofile::setEraseCharacter(byte_t character) {
	SET_CHAR(VERASE,character)
}

void serialportprofile::setKillCharacter(byte_t character) {
	SET_CHAR(VKILL,character)
}

void serialportprofile::setEndOfFileCharacter(byte_t character) {
	SET_CHAR(VEOF,character)
}

void serialportprofile::setEndOfLineCharacter(byte_t character) {
	SET_CHAR(VEOL,character)
}

void serialportprofile::setSecondEndOfLineCharacter(byte_t character) {
	#ifdef VEOL2
		SET_CHAR(VEOL2,character)
	#endif
}

void serialportprofile::setSwitchCharacer(byte_t character) {
	#if defined(VSWTCH)
		SET_CHAR(VSWTCH,character)
	#elif defined(VSWTC)
		SET_CHAR(VSWTC,character)
	#endif
}

void serialportprofile::setStartCharacter(byte_t character) {
	SET_CHAR(VSTART,character)
}

void serialportprofile::setStopCharacter(byte_t character) {
	SET_CHAR(VSTOP,character)
}

void serialportprofile::setSuspendCharacter(byte_t character) {
	SET_CHAR(VSUSP,character)
}

void serialportprofile::setDelayedSuspendCharacter(byte_t character) {
	#ifdef VDSUSP
		SET_CHAR(VDSUSP,character)
	#endif
}

void serialportprofile::setLiteralNextCharcter(byte_t character) {
	#ifdef VLNEXT
		SET_CHAR(VLNEXT,character)
	#endif
}

void serialportprofile::setWordEraseCharcter(byte_t character) {
	#ifdef VWERASE
		SET_CHAR(VWERASE,character)
	#endif
}

void serialportprofile::setReprintCharacter(byte_t character) {
	#ifdef VREPRINT
		SET_CHAR(VREPRINT,character)
	#endif
}

void serialportprofile::setDiscardPendingOutputCharacter(byte_t character) {
	#ifdef VDISCARD
		SET_CHAR(VDISCARD,character)
	#endif
}

void serialportprofile::setStatusRequestCharacter(byte_t character) {
	#ifdef VSTATUS
		SET_CHAR(VSTATUS,character)
	#endif
}

void serialportprofile::setReadThreshold(byte_t count) {
	SET_CHAR(VMIN,count)
}

void serialportprofile::setReadTimeout(byte_t deciseconds) {
	SET_CHAR(VTIME,deciseconds)
}

byte_t serialportprofile::getInterruptCharacter() {
	return GET_CHAR(VINTR);
}

byte_t serialportprofile::getQuitCharacter() {
	return GET_CHAR(VQUIT);
}

byte_t serialportprofile::getEraseCharacter() {
	return GET_CHAR(VERASE);
}

byte_t serialportprofile::getKillCharacter() {
	return GET_CHAR(VKILL);
}

byte_t serialportprofile::getEndOfFileCharacter() {
	return GET_CHAR(VEOF);
}

byte_t serialportprofile::getEndOfLineCharacter() {
	return GET_CHAR(VEOL);
}

byte_t serialportprofile::getSecondEndOfLineCharacter() {
	#ifdef VEOL2
		return GET_CHAR(VEOL2);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getSwitchCharacer() {
	#if defined(VSWTCH)
		return GET_CHAR(VSWTCH);
	#elif defined(VSWTC)
		return GET_CHAR(VSWTC);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getStartCharacter() {
	return GET_CHAR(VSTART);
}

byte_t serialportprofile::getStopCharacter() {
	return GET_CHAR(VSTOP);
}

byte_t serialportprofile::getSuspendCharacter() {
	return GET_CHAR(VSUSP);
}

byte_t serialportprofile::getDelayedSuspendCharacter() {
	#ifdef VDSUSP
		return GET_CHAR(VDSUSP);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getLiteralNextCharcter() {
	#ifdef VLNEXT
		return GET_CHAR(VLNEXT);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getWordEraseCharcter() {
	#ifdef VWERASE
		return GET_CHAR(VWERASE);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getReprintCharacter() {
	#ifdef VREPRINT
		return GET_CHAR(VREPRINT);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getDiscardPendingOutputCharacter() {
	#ifdef VDISCARD
		return GET_CHAR(VDISCARD);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getStatusRequestCharacter() {
	#ifdef VSTATUS
		return GET_CHAR(VSTATUS);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

byte_t serialportprofile::getReadThreshold() {
	return GET_CHAR(VMIN);
}

byte_t serialportprofile::getReadTimeout() {
	return GET_CHAR(VTIME);
}

serialportprofile::baudrate_t serialportprofile::translateBaudString(
							const char *baudrate) {

	if (!charstring::compare(baudrate,"0")) {
		return baud_0;
	} else if (!charstring::compare(baudrate,"50")) {
		return baud_50;
	} else if (!charstring::compare(baudrate,"75")) {
		return baud_75;
	} else if (!charstring::compare(baudrate,"110")) {
		return baud_110;
	} else if (!charstring::compare(baudrate,"134")) {
		return baud_134;
	} else if (!charstring::compare(baudrate,"150")) {
		return baud_150;
	} else if (!charstring::compare(baudrate,"200")) {
		return baud_200;
	} else if (!charstring::compare(baudrate,"300")) {
		return baud_300;
	} else if (!charstring::compare(baudrate,"600")) {
		return baud_600;
	} else if (!charstring::compare(baudrate,"1200")) {
		return baud_1200;
	} else if (!charstring::compare(baudrate,"1800")) {
		return baud_1800;
	} else if (!charstring::compare(baudrate,"2400")) {
		return baud_2400;
	} else if (!charstring::compare(baudrate,"4800")) {
		return baud_4800;
	} else if (!charstring::compare(baudrate,"9600")) {
		return baud_9600;
	#if defined(B19200)
	} else if (!charstring::compare(baudrate,"19200")) {
		return baud_19200;
	#elif defined(EXTA)
	} else if (!charstring::compare(baudrate,"EXTA")) {
		return baud_19200;
	#endif
	#if defined(B38400)
	} else if (!charstring::compare(baudrate,"38400")) {
		return baud_38400;
	#elif defined(EXTB)
	} else if (!charstring::compare(baudrate,"EXTB")) {
		return baud_38400;
	#endif
	#ifdef B57600
	} else if (!charstring::compare(baudrate,"57600")) {
		return baud_57600;
	#endif
	#ifdef B76800
	} else if (!charstring::compare(baudrate,"76800")) {
		return baud_76800;
	#endif
	#ifdef B115200
	} else if (!charstring::compare(baudrate,"115200")) {
		return baud_115200;
	#endif
	#ifdef B230400
	} else if (!charstring::compare(baudrate,"230400")) {
		return baud_230400;
	#endif
	#ifdef B460800
	} else if (!charstring::compare(baudrate,"460800")) {
		return baud_460800;
	#endif
	#ifdef B500000
	} else if (!charstring::compare(baudrate,"500000")) {
		return baud_500000;
	#endif
	#ifdef B576000
	} else if (!charstring::compare(baudrate,"576000")) {
		return baud_576000;
	#endif
	#ifdef B921600
	} else if (!charstring::compare(baudrate,"921600")) {
		return baud_921600;
	#endif
	#ifdef B1000000
	} else if (!charstring::compare(baudrate,"1000000")) {
		return baud_1000000;
	#endif
	#ifdef B1142000
	} else if (!charstring::compare(baudrate,"1152000")) {
		return baud_1152000;
	#endif
	#ifdef B1500000
	} else if (!charstring::compare(baudrate,"1500000")) {
		return baud_1500000;
	#endif
	#ifdef B2000000
	} else if (!charstring::compare(baudrate,"2000000")) {
		return baud_2000000;
	#endif
	#ifdef B2500000
	} else if (!charstring::compare(baudrate,"2500000")) {
		return baud_2500000;
	#endif
	#ifdef B3000000
	} else if (!charstring::compare(baudrate,"3000000")) {
		return baud_3000000;
	#endif
	#ifdef B3500000
	} else if (!charstring::compare(baudrate,"3500000")) {
		return baud_3500000;
	#endif
	#ifdef B4000000
	} else if (!charstring::compare(baudrate,"4000000")) {
		return baud_4000000;
	#endif
	}
	return baud_0;
}

void serialportprofile::setBaud(const char *baudrate) {
	setBaud(translateBaudString(baudrate));
}

bool serialportprofile::setInputBaud(const char *baudrate) {
	return setInputBaud(translateBaudString(baudrate));
}

bool serialportprofile::setOutputBaud(const char *baudrate) {
	return setOutputBaud(translateBaudString(baudrate));
}

void serialportprofile::setInputMode(serialportprofile::inputmode_t inputmode) {
	if (inputmode==cannonical) {
		setCanonicalInput(true);
		setEchoInput(true);
		setEraseCharactersOn(true);
	} else {
		setCanonicalInput(false);
		setEchoInput(false);
		setEraseCharactersOn(false);
	}
}

serialportprofile::inputmode_t serialportprofile::getInputMode() {
	return (getCanonicalInput() &&
			getEchoInput() &&
			getEraseCharactersOn())?
			cannonical:raw;
}

void serialportprofile::evalOptionsString(const char *string) {

	char	bitsperchar=string[0];
	char	parity=character::upper(string[1]);
	char	stopbits=string[2];

	charsize_t	charsize=(charsize_t)cs_8;
	switch (bitsperchar) {
		case '5':
			charsize=cs_5;
			break;
		case '6':
			charsize=cs_6;
			break;
		case '7':
			charsize=cs_7;
			break;
	}
	setCharacterSize(charsize);

	switch (parity) {
		case 'E':
			setInputParityCheck(true);
			setStripParityBits(true);
			setParityCheck(true);
			setOddParity(false);
			break;
		case 'O':
			setInputParityCheck(true);
			setStripParityBits(true);
			setParityCheck(true);
			setOddParity(true);
			break;
		default:
			setInputParityCheck(false);
			setStripParityBits(false);
			setParityCheck(false);
			break;
	}

	
	setTwoStopBits((stopbits=='2'));
}

void serialportprofile::setFlowControl(
		serialportprofile::flowcontrol_t flowcontrol) {

	if (flowcontrol==fc_none) {
		setSoftwareFlowControlOnOutput(false);
		setSoftwareFlowControlOnInput(false);
		setAnyCharacterStartsFlow(false);
		setHardwareFlowControl(false);
	} else if (flowcontrol==fc_software) {
		setSoftwareFlowControlOnOutput(true);
		setSoftwareFlowControlOnInput(true);
		setAnyCharacterStartsFlow(true);
		setHardwareFlowControl(false);
	} else if (flowcontrol==fc_hardware) {
		setSoftwareFlowControlOnOutput(false);
		setSoftwareFlowControlOnInput(false);
		setAnyCharacterStartsFlow(false);
		setHardwareFlowControl(true);
	}
}

serialportprofile::flowcontrol_t serialportprofile::getFlowControl() {
	if (getHardwareFlowControl()) {
		return fc_hardware;
	} else if (getSoftwareFlowControlOnOutput() ||
			getSoftwareFlowControlOnInput()) {
		return fc_software;
	} else {
		return fc_none;
	}
}

void serialportprofile::setOptions(termios *tio) {
	bytestring::copy(&pvt->_tio,tio,sizeof(pvt->_tio));
}

termios *serialportprofile::getTermios() {
	return &pvt->_tio;
}
