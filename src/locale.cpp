// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/locale.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
#endif

// for CHAR_MAX
#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
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

char *locale::getNumericDecimalPoint() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->decimal_point);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getNumericSeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->thousands_sep);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

uint8_t locale::getNumericGrouping(uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return getGrouping(l->grouping,index);
#else
	error::setErrorNumber(ENOTSUP);
	return 0;
#endif
}

uint8_t locale::getGrouping(const char *g, uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)

	// bail and set error if the grouping array is NULL
	if (!g) {
		error::setErrorNumber(ENOTSUP);
		return 0;
	}

	// In the grouping array, each element is the number of digits to group,
	// where the 0th element is the rightmost number of digits to group,
	// the 1st is the next rightmost, the 2nd is the next rightmost, etc.
	//
	// A value of 0 means repeat the previous grouping forever.
	// A value of CHAR_MAX means no more grouping is performed.
	//
	// Eg.
	//
	// for a number grouped like: 100,000,00,0
	// the values would be 1,2,3,0
	//
	// for a number grouped like: 10000000,000,00,0
	// the values would be 1,2,3,CHAR_MAX
	//
	uint8_t	retval=0;
	for (;;) {
		// I hope we don't get into trouble on some random ARM platform
		// with this...
		if (*g==CHAR_MAX) {
			return 0;
		}
		retval=*g;
		if (!index || !*g) {
			return retval;
		}
		g++;
		index--;
	}
#else
	return 0;
#endif
}

char *locale::getInternationalCurrencySymbol() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	// the first 3 characters are the symbol, but do some sanity checking
	const char	*s=l->int_curr_symbol;
	if (!s) {
		error::setErrorNumber(ENOTSUP);
		return NULL;
	}
	size_t	len=charstring::getLength(s);
	// in some locales (eg. C) this is empty, which is ok
	if (!len) {
		return charstring::duplicate("");
	}
	if (len<4) {
		error::setErrorNumber(ENOTSUP);
		return NULL;
	}
	return charstring::duplicate(s,3);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getInternationalCurrencySeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	// the 4th character is the separator, but do some sanity checking
	const char	*s=l->int_curr_symbol;
	if (!s) {
		error::setErrorNumber(ENOTSUP);
		return NULL;
	}
	size_t	len=charstring::getLength(s);
	// in some locales (eg. C) this is empty, which is ok
	if (!len) {
		return charstring::duplicate("");
	}
	if (len<4) {
		error::setErrorNumber(ENOTSUP);
		return NULL;
	}
	return charstring::duplicate(s+3,1);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getLocalCurrencySymbol() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->currency_symbol);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getMonetaryDecimalPoint() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return charstring::duplicate(l->mon_decimal_point);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getMonetarySeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return charstring::duplicate(l->mon_thousands_sep);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

uint8_t locale::getMonetaryGrouping(uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return getGrouping(l->mon_grouping,index);
#else
	error::setErrorNumber(ENOTSUP);
	return 0;
#endif
}

char *locale::getMonetaryPositiveSign() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->positive_sign);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

char *locale::getMonetaryNegativeSign() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->negative_sign);
#else
	error::setErrorNumber(ENOTSUP);
	return NULL;
#endif
}

uint8_t locale::getInternationalMonetaryFractionalDigits() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return l->int_frac_digits;
#else
	error::setErrorNumber(ENOTSUP);
	return 0;
#endif
}

uint8_t locale::getLocalMonetaryFractionalDigits() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return l->frac_digits;
#else
	error::setErrorNumber(ENOTSUP);
	return 0;
#endif
}

bool locale::getCurrencySymbolPreceedsPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->p_cs_precedes==CHAR_MAX) {
		error::setErrorNumber(ENOTSUP);
		return false;
	}
	return l->p_cs_precedes;
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

bool locale::getSpaceSeparatesCurrencySymbolAndPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->p_sep_by_space==CHAR_MAX) {
		error::setErrorNumber(ENOTSUP);
		return false;
	}
	return l->p_sep_by_space;
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

monetary_sign_position_t locale::getMonetarySignPositionForPositiveValues() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return MONETARY_SIGN_POSITION_ERROR;
	}
	switch (l->p_sign_posn) {
		case 0:
        		return MONETARY_SIGN_POSITION_PARENTHESES;
		case 1:
        		return MONETARY_SIGN_POSITION_BEFORE_STRING;
		case 2:
        		return MONETARY_SIGN_POSITION_AFTER_STRING;
		case 3:
        		return MONETARY_SIGN_POSITION_BEFORE_SYMBOL;
		case 4:
        		return MONETARY_SIGN_POSITION_AFTER_SYMBOL;
		default:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			error::setErrorNumber(ENOTSUP);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	error::setErrorNumber(ENOTSUP);
	return MONETARY_SIGN_POSITION_ERROR;
#endif
}

bool locale::getCurrencySymbolPreceedsNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->n_cs_precedes==CHAR_MAX) {
		error::setErrorNumber(ENOTSUP);
		return false;
	}
	return l->n_cs_precedes;
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

bool locale::getSpaceSeparatesCurrencySymbolAndNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->n_sep_by_space==CHAR_MAX) {
		error::setErrorNumber(ENOTSUP);
		return false;
	}
	return l->n_sep_by_space;
#else
	error::setErrorNumber(ENOTSUP);
	return false;
#endif
}

monetary_sign_position_t locale::getMonetarySignPositionForNegativeValues() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	lconv	*l=localeconv();
	if (!l) {
		return MONETARY_SIGN_POSITION_ERROR;
	}
	switch (l->n_sign_posn) {
		case 0:
        		return MONETARY_SIGN_POSITION_PARENTHESES;
		case 1:
        		return MONETARY_SIGN_POSITION_BEFORE_STRING;
		case 2:
        		return MONETARY_SIGN_POSITION_AFTER_STRING;
		case 3:
        		return MONETARY_SIGN_POSITION_BEFORE_SYMBOL;
		case 4:
        		return MONETARY_SIGN_POSITION_AFTER_SYMBOL;
		default:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			error::setErrorNumber(ENOTSUP);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	error::setErrorNumber(ENOTSUP);
	return MONETARY_SIGN_POSITION_ERROR;
#endif
}

bool locale::isSupported() {
#ifdef RUDIMENTS_HAVE_SETLOCALE
	return true;
#else
	return false;
#endif
}
