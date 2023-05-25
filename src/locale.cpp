// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/locale.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

bool locale::setAll(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ALL)
	return setlocale(LC_ALL,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getAll() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ALL)
	return setlocale(LC_ALL,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isAllSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ALL)
	return true;
#else
	return false;
#endif
}

bool locale::setCollate(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_COLLATE)
	return setlocale(LC_COLLATE,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getCollate() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_COLLATE)
	return setlocale(LC_COLLATE,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isCollateSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_COLLATE)
	return true;
#else
	return false;
#endif
}

bool locale::setCType(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_CTYPE)
	return setlocale(LC_CTYPE,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getCType() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_CTYPE)
	return setlocale(LC_CTYPE,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isCTypeSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_CTYPE)
	return true;
#else
	return false;
#endif
}

bool locale::setMessages(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MESSAGES)
	return setlocale(LC_MESSAGES,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getMessages() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MESSAGES)
	return setlocale(LC_MESSAGES,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isMessagesSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MESSAGES)
	return true;
#else
	return false;
#endif
}

bool locale::setMonetary(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MONETARY)
	return setlocale(LC_MONETARY,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getMonetary() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MONETARY)
	return setlocale(LC_MONETARY,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isMonetarySupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MONETARY)
	return true;
#else
	return false;
#endif
}

bool locale::setNumeric(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NUMERIC)
	return setlocale(LC_NUMERIC,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getNumeric() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NUMERIC)
	return setlocale(LC_NUMERIC,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isNumericSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NUMERIC)
	return true;
#else
	return false;
#endif
}

bool locale::setTime(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TIME)
	return setlocale(LC_TIME,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getTime() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TIME)
	return setlocale(LC_TIME,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isTimeSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TIME)
	return true;
#else
	return false;
#endif
}

bool locale::setAddress(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ADDRESS)
	return setlocale(LC_ADDRESS,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getAddress() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ADDRESS)
	return setlocale(LC_ADDRESS,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isAddressSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ADDRESS)
	return true;
#else
	return false;
#endif
}

bool locale::setIdentification(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_IDENTIFICATION)
	return setlocale(LC_IDENTIFICATION,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getIdentification() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_IDENTIFICATION)
	return setlocale(LC_IDENTIFICATION,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isIdentificationSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_IDENTIFICATION)
	return true;
#else
	return false;
#endif
}

bool locale::setMeasurement(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MEASUREMENT)
	return setlocale(LC_MEASUREMENT,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getMeasurement() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MEASUREMENT)
	return setlocale(LC_MEASUREMENT,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isMeasurementSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MEASUREMENT)
	return true;
#else
	return false;
#endif
}

bool locale::setName(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NAME)
	return setlocale(LC_NAME,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getName() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NAME)
	return setlocale(LC_NAME,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isNameSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NAME)
	return true;
#else
	return false;
#endif
}

bool locale::setPaper(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_PAPER)
	return setlocale(LC_PAPER,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getPaper() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_PAPER)
	return setlocale(LC_PAPER,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isPaperSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_PAPER)
	return true;
#else
	return false;
#endif
}

bool locale::setTelephone(const char *value) {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TELEPHONE)
	return setlocale(LC_TELEPHONE,value);
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

const char *locale::getTelephone() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TELEPHONE)
	return setlocale(LC_TELEPHONE,NULL);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

bool locale::isTelephoneSupported() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TELEPHONE)
	return true;
#else
	return false;
#endif
}

bool locale::isSupported() {
#ifdef RUDIMENTS_HAVE_SETLOCALE
	return true;
#else
	return false;
#endif
}
