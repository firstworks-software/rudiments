// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/datetime.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/wstringbuffer.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_SYSLOG_H
	#include <syslog.h>
#endif

logdestination::~logdestination() {}

class filedestinationprivate {
	friend class filedestination;
	private:
		file	_logfile;
};

filedestination::filedestination() : logdestination() {
	pvt=new filedestinationprivate;
}

filedestination::filedestination(filedestination &f) :
						logdestination(f) {
	pvt=new filedestinationprivate;
	pvt->_logfile=f.pvt->_logfile;
}

filedestination &filedestination::operator=(filedestination &f) {
	if (this!=&f) {
		pvt->_logfile=f.pvt->_logfile;
	}
	return *this;
}

filedestination::~filedestination() {
	close();
	delete pvt;
}

bool filedestination::open(const char *filename, mode_t perms) {
	return pvt->_logfile.open(filename,O_CREAT|O_WRONLY|O_APPEND,perms);
}

void filedestination::close() {
	pvt->_logfile.close();
}

void filedestination::write(const char *string) {
	pvt->_logfile.write(string);
}

void filedestination::write(const wchar_t *string) {
	pvt->_logfile.write(string);
}

void stdoutdestination::write(const char *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stdoutput.write(string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
}

void stdoutdestination::write(const wchar_t *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stdoutput.write(string,wcharstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
}

void stderrdestination::write(const char *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stderror.write(string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
}

void stderrdestination::write(const wchar_t *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stderror.write(string,wcharstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
}

class syslogdestinationprivate {
	friend class syslogdestination;
	private:
		int32_t	_priority;
};

syslogdestination::syslogdestination() : logdestination() {
	pvt=new syslogdestinationprivate;
}

syslogdestination::syslogdestination(syslogdestination &s) :
						logdestination(s) {
	pvt=new syslogdestinationprivate;
}

syslogdestination &syslogdestination::operator=(syslogdestination &s) {
	if (this!=&s) {
		pvt->_priority=s.pvt->_priority;
	}
	return *this;
}

syslogdestination::~syslogdestination() {
	close();
	delete pvt;
}

void syslogdestination::open(const char *ident, int32_t option,
					int32_t facility, int32_t priority) {
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	openlog(ident,option,facility);
#endif
	pvt->_priority=priority;
}

void syslogdestination::close() {
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	closelog();
#endif
}

void syslogdestination::write(const char *string) {
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	syslog(pvt->_priority,"%s",string);
#endif
}

void syslogdestination::write(const wchar_t *string) {
#ifdef RUDIMENTS_HAVE_SYSLOG_H
	char	*s=charstring::duplicate(string);
	syslog(pvt->_priority,"%s",s);
	delete[] s;
#endif
}

class loggerprivate {
	friend class logger;
	private:
		loggerlist	_logdestlist;
		char		_indent;
		char		_windent;
		uint8_t		_level;
};

logger::logger() : object() {
	pvt=new loggerprivate;
	pvt->_indent='	';
	pvt->_windent='	';
	pvt->_level=0;
}

logger::~logger() {
	removeAllLogDestinations();
	delete pvt;
}

void logger::addLogDestination(logdestination *logdest) {
	pvt->_logdestlist.append(logdest);
}

void logger::removeLogDestination(logdestination *logdest) {
	pvt->_logdestlist.remove(logdest);
}

void logger::removeAllLogDestinations() {
	pvt->_logdestlist.clear();
}

void logger::setIndent(char ch) {
	pvt->_indent=ch;
}

void logger::setWideIndent(wchar_t ch) {
	pvt->_windent=ch;
}

char logger::getWideIndent() {
	return pvt->_windent;
}

void logger::setLogLevel(uint8_t level) {
	pvt->_level=level;
}

uint8_t logger::getLogLevel() {
	return pvt->_level;
}

char *logger::logHeader(const char *name) {
	datetime	dt;
	dt.getSystemDateAndTime();
	stringbuffer	str;
	str.append(dt.getString())->append(" ");
	str.append(name)->append(" [");
	str.append((uint64_t)process::getProcessId())->append("]");
	return str.detachString();
}

wchar_t *logger::logHeader(const wchar_t *name) {
	datetime	dt;
	dt.getSystemDateAndTime();
	wstringbuffer	str;
	str.append(dt.getString())->append(L" ");
	str.append(name)->append(L" [");
	str.append((uint64_t)process::getProcessId())->append(L"]");
	return str.detachString();
}

void logger::start(uint8_t level, const char *header,
				uint32_t indent, const char *string) {
	if (level>pvt->_level) {
		return;
	}
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append(string)->append(" {\n");
	write(str.getString());
}

void logger::start(uint8_t level, const wchar_t *header,
				uint32_t indent, const wchar_t *string) {
	if (level>pvt->_level) {
		return;
	}
	wstringbuffer	str;
	if (wcharstring::length(header)) {
		str.append(header)->append(L" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_windent);
	}
	str.append(string)->append(L" {\n");
	write(str.getString());
}

void logger::write(uint8_t level, const char *header,
				uint32_t indent, const char *format, ...) {
	if (level>pvt->_level) {
		return;
	}
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	if (format) {
		va_list	argp;
		va_start(argp,format);
		str.printf(format,&argp);
		va_end(argp);
	}
	str.append("\n");
	write(str.getString());
}

void logger::write(uint8_t level, const wchar_t *header,
				uint32_t indent, const wchar_t *format, ...) {
	if (level>pvt->_level) {
		return;
	}
	wstringbuffer	str;
	if (wcharstring::length(header)) {
		str.append(header)->append(L" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_windent);
	}
	if (format) {
		va_list	argp;
		va_start(argp,format);
		str.printf(format,&argp);
		va_end(argp);
	}
	str.append(L"\n");
	write(str.getString());
}

void logger::write(uint8_t level, const char *header,
				uint32_t indent, const char *format,
				va_list *argp) {
	if (level>pvt->_level) {
		return;
	}
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.printf(format,argp);
	str.append("\n");
	write(str.getString());
}

void logger::write(uint8_t level, const wchar_t *header,
				uint32_t indent, const wchar_t *format,
				va_list *argp) {
	if (level>pvt->_level) {
		return;
	}
	wstringbuffer	str;
	if (wcharstring::length(header)) {
		str.append(header)->append(L" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_windent);
	}
	str.printf(format,argp);
	str.append(L"\n");
	write(str.getString());
}

void logger::end(uint8_t level, const char *header, uint32_t indent) {
	if (level>pvt->_level) {
		return;
	}
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append("}\n");
	write(str.getString());
}

void logger::end(uint8_t level, const wchar_t *header, uint32_t indent) {
	if (level>pvt->_level) {
		return;
	}
	wstringbuffer	str;
	if (wcharstring::length(header)) {
		str.append(header)->append(L" : ");
	}
	for (uint32_t i=0; i<indent; i++) {
		str.append(pvt->_windent);
	}
	str.append(L"}\n");
	write(str.getString());
}

void logger::write(const char *logentry) {
	for (loggerlistnode *current=pvt->_logdestlist.getFirst();
				current; current=current->getNext()) {
		current->getValue()->write(logentry);
	}
}

void logger::write(const wchar_t *logentry) {
	for (loggerlistnode *current=pvt->_logdestlist.getFirst();
				current; current=current->getNext()) {
		current->getValue()->write(logentry);
	}
}
