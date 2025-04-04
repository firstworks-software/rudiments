// Copyright (c) David Muse
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getAll() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ALL)
	return setlocale(LC_ALL,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getCollate() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_COLLATE)
	return setlocale(LC_COLLATE,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getCType() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_CTYPE)
	return setlocale(LC_CTYPE,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getMessages() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MESSAGES)
	return setlocale(LC_MESSAGES,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getMonetary() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MONETARY)
	return setlocale(LC_MONETARY,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getNumeric() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NUMERIC)
	return setlocale(LC_NUMERIC,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getTime() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TIME)
	return setlocale(LC_TIME,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getAddress() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_ADDRESS)
	return setlocale(LC_ADDRESS,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getIdentification() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_IDENTIFICATION)
	return setlocale(LC_IDENTIFICATION,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getMeasurement() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_MEASUREMENT)
	return setlocale(LC_MEASUREMENT,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getName() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_NAME)
	return setlocale(LC_NAME,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getPaper() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_PAPER)
	return setlocale(LC_PAPER,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

const char *locale::getTelephone() {
#if defined(RUDIMENTS_HAVE_SETLOCALE) && defined(LC_TELEPHONE)
	return setlocale(LC_TELEPHONE,NULL);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->decimal_point);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

char *locale::getNumericDigitGroupSeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->thousands_sep);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

uint8_t locale::getNumericDigitGroupCount(uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return getDigitGroupCount(l->grouping,index);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return 0;
#endif
}

uint8_t locale::getDigitGroupCount(const char *g, uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)

	// bail and set error if the grouping array is NULL
	if (!g) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
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

char *locale::getMonetaryDecimalPoint() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return charstring::duplicate(l->mon_decimal_point);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

char *locale::getMonetaryDigitGroupSeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return charstring::duplicate(l->mon_thousands_sep);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

uint8_t locale::getMonetaryDigitGroupCount(uint8_t index) {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	return getDigitGroupCount(l->mon_grouping,index);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return 0;
#endif
}

char *locale::getMonetaryPositiveSign() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->positive_sign);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

char *locale::getMonetaryNegativeSign() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->negative_sign);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

char *locale::getLocalCurrencySymbol() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	return charstring::duplicate(l->currency_symbol);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

uint8_t locale::getLocalMonetaryDecimalDigits() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	// If frac_digits is CHAR_MAX (indicating that this value is undefined
	// for the current locale) then it could be 127 or 255, depending on
	// whether char is explicitly signed or not on the current platform.
	// Eg. on ARM and MIPS, char is not explicitly signed.  For consistency,
	// we'll always return 255.
	return (l->frac_digits==127)?255:l->frac_digits;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return 0;
#endif
}

bool locale::getLocalCurrencySymbolPreceedsPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->p_cs_precedes==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->p_cs_precedes;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

bool locale::getLocalSpaceSeparatesCurrencySymbolAndPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->p_sep_by_space==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->p_sep_by_space;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

monetary_sign_position_t locale::getLocalMonetaryPositiveSignPosition() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
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
		case CHAR_MAX:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			#ifdef ENOTSUP
				error::setErrorNumber(ENOTSUP);
			#endif
			return MONETARY_SIGN_POSITION_ERROR;
		default:
			error::setErrorNumber(ERANGE);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return MONETARY_SIGN_POSITION_ERROR;
#endif
}

bool locale::getLocalCurrencySymbolPreceedsNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->n_cs_precedes==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->n_cs_precedes;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

bool locale::getLocalSpaceSeparatesCurrencySymbolAndNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->n_sep_by_space==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->n_sep_by_space;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

monetary_sign_position_t locale::getLocalMonetaryNegativeSignPosition() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
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
		case CHAR_MAX:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			#ifdef ENOTSUP
				error::setErrorNumber(ENOTSUP);
			#endif
			return MONETARY_SIGN_POSITION_ERROR;
		default:
			error::setErrorNumber(ERANGE);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return MONETARY_SIGN_POSITION_ERROR;
#endif
}

char *locale::getInternationalCurrencySymbol() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	// the first 3 characters are the symbol, but do some sanity checking
	const char	*s=l->int_curr_symbol;
	if (!s) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return NULL;
	}
	size_t	len=charstring::getLength(s);
	// in some locales (eg. C) this is empty, which is ok
	if (!len) {
		return charstring::duplicate("");
	}
	if (len<4) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return NULL;
	}
	return charstring::duplicate(s,3);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

char *locale::getInternationalCurrencySymbolSeparator() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return NULL;
	}
	// the 4th character is the separator, but do some sanity checking
	const char	*s=l->int_curr_symbol;
	if (!s) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return NULL;
	}
	size_t	len=charstring::getLength(s);
	// in some locales (eg. C) this is empty, which is ok
	if (!len) {
		return charstring::duplicate("");
	}
	if (len<4) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return NULL;
	}
	return charstring::duplicate(s+3,1);
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return NULL;
#endif
}

uint8_t locale::getInternationalMonetaryDecimalDigits() {
#if defined(RUDIMENTS_HAVE_LOCALECONV)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return 0;
	}
	// If int_frac_digits is CHAR_MAX (indicating that this value is
	// undefined for the current locale) then it could be 127 or 255,
	// depending on whether char is explicitly signed or not on the current
	// platform.  Eg. on ARM and MIPS, char is not explicitly signed.  For
	// consistency, we'll always return 255.
	return (l->int_frac_digits==127)?255:l->int_frac_digits;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return 0;
#endif
}

bool locale::getInternationalCurrencySymbolPreceedsPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_P_CS_PRECEDES)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->int_p_cs_precedes==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->int_p_cs_precedes;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

bool locale::getInternationalSpaceSeparatesCurrencySymbolAndPositiveValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_P_SEP_BY_SPACE)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->int_p_sep_by_space==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->int_p_sep_by_space;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

monetary_sign_position_t
locale::getInternationalMonetaryPositiveSignPosition() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_P_SIGN_POSN)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return MONETARY_SIGN_POSITION_ERROR;
	}
	switch (l->int_p_sign_posn) {
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
		case CHAR_MAX:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			#ifdef ENOTSUP
				error::setErrorNumber(ENOTSUP);
			#endif
			return MONETARY_SIGN_POSITION_ERROR;
		default:
			error::setErrorNumber(ERANGE);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return MONETARY_SIGN_POSITION_ERROR;
#endif
}

bool locale::getInternationalCurrencySymbolPreceedsNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_N_CS_PRECEDES)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->int_n_cs_precedes==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->int_n_cs_precedes;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

bool locale::getInternationalSpaceSeparatesCurrencySymbolAndNegativeValue() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_N_SEP_BY_SPACE)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return false;
	}
	// on Linux, this is CHAR_MAX if not supported in the current locale
	if (l->int_n_sep_by_space==CHAR_MAX) {
		#ifdef ENOTSUP
			error::setErrorNumber(ENOTSUP);
		#endif
		return false;
	}
	return l->int_n_sep_by_space;
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
	return false;
#endif
}

monetary_sign_position_t
locale::getInternationalMonetaryNegativeSignPosition() {
#if defined(RUDIMENTS_HAVE_LOCALECONV) && \
	defined(RUDIMENTS_HAVE_LCONV_INT_N_SIGN_POSN)
	error::clearError();
	lconv	*l=localeconv();
	if (!l) {
		return MONETARY_SIGN_POSITION_ERROR;
	}
	switch (l->int_n_sign_posn) {
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
		case CHAR_MAX:
			// on Linux, this is CHAR_MAX if it's not supported in
			// the current locale
			#ifdef ENOTSUP
				error::setErrorNumber(ENOTSUP);
			#endif
			return MONETARY_SIGN_POSITION_ERROR;
		default:
			error::setErrorNumber(ERANGE);
			return MONETARY_SIGN_POSITION_ERROR;
	}
#else
	#ifdef ENOTSUP
		error::setErrorNumber(ENOTSUP);
	#endif
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
