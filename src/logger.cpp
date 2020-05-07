// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/logger.h>
#include <rudiments/datetime.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
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

filedestination::filedestination(const filedestination &f) :
						logdestination(f) {
	pvt=new filedestinationprivate;
	pvt->_logfile=f.pvt->_logfile;
}

filedestination &filedestination::operator=(const filedestination &f) {
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

void stdoutdestination::write(const char *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stdoutput.write(string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR);
}

void stderrdestination::write(const char *string) {
	int32_t	result;
	error::clearError();
	do {
		result=stderror.write(string,charstring::length(string));
	} while (result==-1 && error::getErrorNumber()==EINTR);
}

class syslogdestinationprivate {
	friend class syslogdestination;
	private:
		int32_t	_priority;
};

syslogdestination::syslogdestination() : logdestination() {
	pvt=new syslogdestinationprivate;
}

syslogdestination::syslogdestination(const syslogdestination &s) :
						logdestination(s) {
	pvt=new syslogdestinationprivate;
}

syslogdestination &syslogdestination::operator=(const syslogdestination &s) {
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

class loggerprivate {
	friend class logger;
	private:
		loggerlist	_logdestlist;
		char		_indent;
};

logger::logger() {
	pvt=new loggerprivate;
	pvt->_indent='	';
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

char logger::getIndent() {
	return pvt->_indent;
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

void logger::start(const char *header, int32_t indent, const char *string) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append(string)->append(" {\n");
	write(str.getString());
}

void logger::write(const char *header, int32_t indent,
					const char *format, ...) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	va_list	argp;
	va_start(argp,format);
	str.writeFormatted(format,&argp);
	va_end(argp);
	str.append("\n");
	write(str.getString());
}

void logger::write(const char *header, int32_t indent,
					const char *format, va_list *argp) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.writeFormatted(format,argp);
	str.append("\n");
	write(str.getString());
}

void logger::write(const char *header, int32_t indent, char character) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append(character)->append("\n");
	write(str.getString());
}

void logger::write(const char *header, int32_t indent, int32_t number) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append(number)->append("\n");
	write(str.getString());
}

void logger::write(const char *header, int32_t indent, double number) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append(number)->append("\n");
	write(str.getString());
}

void logger::end(const char *header, int32_t indent) {
	stringbuffer	str;
	if (charstring::length(header)) {
		str.append(header)->append(" : ");
	}
	for (int32_t i=0; i<indent; i++) {
		str.append(pvt->_indent);
	}
	str.append("}\n");
	write(str.getString());
}

void logger::write(const char *logentry) {
	for (loggerlistnode *current=pvt->_logdestlist.getFirst();
				current; current=current->getNext()) {
		current->getValue()->write(logentry);
	}
}
