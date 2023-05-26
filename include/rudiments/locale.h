// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOCALE_H
#define RUDIMENTS_LOCALE_H

#include <rudiments/private/localeincludes.h>

/** The locale class provides static methods for accessing and manipulating
 *  locale categories. */

enum monetary_sign_position_t {
	MONETARY_SIGN_POSITION_PARENTHESES=0,
	MONETARY_SIGN_POSITION_BEFORE_STRING,
	MONETARY_SIGN_POSITION_AFTER_STRING,
	MONETARY_SIGN_POSITION_BEFORE_SYMBOL,
	MONETARY_SIGN_POSITION_AFTER_SYMBOL,
	MONETARY_SIGN_POSITION_ERROR
};

class RUDIMENTS_DLLSPEC locale {
	public:
		/** Sets the value of the LC_ALL locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_ALL are not supported. */
		static bool	setAll(const char *value);

		/** Returns the value of the LC_ALL locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_ALL are not supported. */
		static const char	*getAll();

		/** Returns true if the LC_ALL category is supported and
		 *  false otherwise. */
		static bool	isAllSupported();

		/** Sets the value of the LC_COLLATE locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_COLLATE are not supported. */
		static bool	setCollate(const char *value);

		/** Returns the value of the LC_COLLATE locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_COLLATE are not supported. */
		static const char	*getCollate();

		/** Returns true if the LC_COLLATE category is supported and
		 *  false otherwise. */
		static bool	isCollateSupported();

		/** Sets the value of the LC_CTYPE locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_CTYPE are not supported. */
		static bool	setCType(const char *value);

		/** Returns the value of the LC_CTYPE locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_CTYPE are not supported. */
		static const char	*getCType();

		/** Returns true if the LC_CTYPE category is supported and
		 *  false otherwise. */
		static bool	isCTypeSupported();

		/** Sets the value of the LC_MESSAGES locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_MESSAGES are not supported. */
		static bool	setMessages(const char *value);

		/** Returns the value of the LC_MESSAGES locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_MESSAGES are not supported. */
		static const char	*getMessages();

		/** Returns true if the LC_MESSAGES category is supported and
		 *  false otherwise. */
		static bool	isMessagesSupported();

		/** Sets the value of the LC_MONETARY locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_MONETARY are not supported. */
		static bool	setMonetary(const char *value);

		/** Returns the value of the LC_MONETARY locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_MONETARY are not supported. */
		static const char	*getMonetary();

		/** Returns true if the LC_MONETARY category is supported and
		 *  false otherwise. */
		static bool	isMonetarySupported();

		/** Sets the value of the LC_NUMERIC locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_NUMERIC are not supported. */
		static bool	setNumeric(const char *value);

		/** Returns the value of the LC_NUMERIC locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_NUMERIC are not supported. */
		static const char	*getNumeric();

		/** Returns true if the LC_NUMERIC category is supported and
		 *  false otherwise. */
		static bool	isNumericSupported();

		/** Sets the value of the LC_TIME locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_TIME are not supported. */
		static bool	setTime(const char *value);

		/** Returns the value of the LC_TIME locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_TIME are not supported. */
		static const char	*getTime();

		/** Returns true if the LC_TIME category is supported and
		 *  false otherwise. */
		static bool	isTimeSupported();

		/** Sets the value of the LC_ADDRESS locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_ADDRESS are not supported. */
		static bool	setAddress(const char *value);

		/** Returns the value of the LC_ADDRESS locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_ADDRESS are not supported. */
		static const char	*getAddress();

		/** Returns true if the LC_ADDRESS category is supported and
		 *  false otherwise. */
		static bool	isAddressSupported();

		/** Sets the value of the LC_IDENTIFICATION locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_IDENTIFICATION are not supported. */
		static bool	setIdentification(const char *value);

		/** Returns the value of the LC_IDENTIFICATION locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_IDENTIFICATION are not supported. */
		static const char	*getIdentification();

		/** Returns true if the LC_IDENTIFICATION category is supported
		 *  and false otherwise. */
		static bool	isIdentificationSupported();

		/** Sets the value of the LC_MEASUREMENT locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_MEASUREMENT are not supported. */
		static bool	setMeasurement(const char *value);

		/** Returns the value of the LC_MEASUREMENT locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_MEASUREMENT are not supported. */
		static const char	*getMeasurement();

		/** Returns true if the LC_MEASUREMENT category is supported and
		 *  false otherwise. */
		static bool	isMeasurementSupported();

		/** Sets the value of the LC_NAME locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_NAME are not supported. */
		static bool	setName(const char *value);

		/** Returns the value of the LC_NAME locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_NAME are not supported. */
		static const char	*getName();

		/** Returns true if the LC_NAME category is supported and
		 *  false otherwise. */
		static bool	isNameSupported();

		/** Sets the value of the LC_PAPER locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_PAPER are not supported. */
		static bool	setPaper(const char *value);

		/** Returns the value of the LC_PAPER locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_PAPER are not supported. */
		static const char	*getPaper();

		/** Returns true if the LC_PAPER category is supported and
		 *  false otherwise. */
		static bool	isPaperSupported();

		/** Sets the value of the LC_TELEPHONE locale category
		 *  to "value".
		 *
		 *  Returns true on success and false on failure, eg. if
		 *  either locales or LC_TELEPHONE are not supported. */
		static bool	setTelephone(const char *value);

		/** Returns the value of the LC_TELEPHONE locale category.
		 *
		 *  Returns the value on success and NULL on failure, eg. if
		 *  either locales or LC_TELEPHONE are not supported. */
		static const char	*getTelephone();

		/** Returns true if the LC_TELEPHONE category is supported and
		 *  false otherwise. */
		static bool	isTelephoneSupported();

		/** Returns the decimal point string, for non-monetary
		 *  values, for the current locale.
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getNumericDecimalPoint();

		/** Returns the separator string for groups of digits before the
		 *  decimal point, for non-monetary values, for the current
		 *  locale.
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getNumericDigitGroupSeparator();

		/** Returns the number of digits in a group, where index 0
		 *  is the rightmost group, and subsequent indices refer to
		 *  groups of digits to the left of the rightmost group,
		 *  for non-monetary values, for the current locale.
		 *
		 *  For example...
		 *
		 *  If a number is grouped like:
		 *  100,000,000,000
		 *  Then:
		 *  * getNumericDigitGroupCount(0) would return 3
		 *  * getNumericDigitGroupCount(1) would return 3
		 *  * getNumericDigitGroupCount(2) would return 3
		 *  * getNumericDigitGroupCount(>2) would return 3
		 *
		 *  If a number is grouped like:
		 *  100,000,00,00
		 *  Then:
		 *  * getNumericDigitGroupCount(0) would return 2
		 *  * getNumericDigitGroupCount(1) would return 2
		 *  * getNumericDigitGroupCount(2) would return 3
		 *  * getNumericDigitGroupCount(>2) would return 3
		 *
		 *  If a number is grouped like:
		 *  100,000,00,0
		 *  Then:
		 *  * getNumericDigitGroupCount(0) would return 1
		 *  * getNumericDigitGroupCount(1) would return 2
		 *  * getNumericDigitGroupCount(2) would return 3
		 *  * getNumericDigitGroupCount(>2) would return 3
		 *
		 *  Returns 0 if no grouping is performed at that index.
		 *  Returns 0 and sets errno on error.  */
		static uint8_t	getNumericDigitGroupCount(uint8_t index);

		/** Returns the decimal point string, for monetary values, for
		 *  the current locale.
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getMonetaryDecimalPoint();

		/** Returns the separator string for groups of digits before
 		 *  the decimal point, for monetary values, for the current
		 *  locale.
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getMonetaryDigitGroupSeparator();

		/** Returns the number of digits in a group, where index 0
		 *  is the rightmost group, and subsequent indices refer to
		 *  groups of digits to the left of the rightmost group,
		 *  for monetary values, for the current locale.
		 *
		 *  For example...
		 *
		 *  If an amount is grouped like:
		 *  $100,000,000,000
		 *  Then:
		 *  * getMonetaryDigitGroupCount(0) would return 3
		 *  * getMonetaryDigitGroupCount(1) would return 3
		 *  * getMonetaryDigitGroupCount(2) would return 3
		 *  * getMonetaryDigitGroupCount(>2) would return 3
		 *
		 *  If an amount is grouped like:
		 *  $100,000,00,00
		 *  Then:
		 *  * getMonetaryDigitGroupCount(0) would return 2
		 *  * getMonetaryDigitGroupCount(1) would return 2
		 *  * getMonetaryDigitGroupCount(2) would return 3
		 *  * getMonetaryDigitGroupCount(>2) would return 3
		 *
		 *  If an amount is grouped like:
		 *  $100,000,00,0
		 *  Then:
		 *  * getMonetaryDigitGroupCount(0) would return 1
		 *  * getMonetaryDigitGroupCount(1) would return 2
		 *  * getMonetaryDigitGroupCount(2) would return 3
		 *  * getMonetaryDigitGroupCount(>2) would return 3
		 *
		 *  Returns 0 if no grouping is performed at that index.
		 *  Returns 0 and sets errno on error.  */
		static uint8_t	getMonetaryDigitGroupCount(uint8_t index);

		/** Returns the positive sign string, for monetary values,
		 *  for the current locale.  Eg. "+" but usually ""
		 *  (empty string).
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getMonetaryPositiveSign();

		/** Returns the negative sign string, for monetary values,
		 *  for the current locale.  Eg. "-"
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getMonetaryNegativeSign();

		/** Returns the local currency symbol string, for the current
		 *  locale.  Eg. "$"
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getLocalCurrencySymbol();

		/** Returns the number of digits after the decimal point
		 *  for an locally formatted monetary value, for the current
		 *  locale.
		 *
		 *  Returns 0 and sets errno on error. */
		static uint8_t	getLocalMonetaryDecimalDigits();

		/** Returns true if the currency symbol preceeds a positive
		 *  monetary value, for the current locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for $100
		 *  * false for 100$
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getLocalCurrencySymbolPreceedsPositiveValue();

		/** Returns true if a space separates the currency symbol and a
		 *  locally formatted positive monetary value, for the current
		 *  locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for $ 100
		 *  * false for $100
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getLocalSpaceSeparatesCurrencySymbolAndPositiveValue();

		/** Returns the position of the sign, for locally formatted
		 *  positive monetary values, for the current locale, as
		 *  follows:
		 *
		 *  MONETARY_SIGN_POSITION_PARENTHESES if parentheses surround
		 *  the entire string.  Eg. ($100)
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_STRING if the sign comes
		 *  before the entire string.  Eg. +$100
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_STRING if the sign comes
		 *  after the entire string. Eg. $100+
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_SYMBOL if the sign comes
		 *  immediately before the currency symbol.  Eg. 100+$
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_SYMBOL if the sign comes
		 *  immediately after the currency symbol.  Eg. 100$+
		 *
		 *  MONETARY_SIGN_POSITION_ERROR and sets errno on error. */
		static monetary_sign_position_t
		getLocalMonetaryPositiveSignPosition();

		/** Returns true if the currency symbol preceeds a locally
		 *  formatted negative monetary value, for the current locale,
		 *  and false otherwise.
		 *
		 *  Eg.
		 *  * true for -$100
		 *  * false for 100$-
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getLocalCurrencySymbolPreceedsNegativeValue();

		/** Returns true if a space separates the currency symbol and a
		 *  locally formatted negative monetary value, for the current
		 *  locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for -$ 100
		 *  * false for -$100
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getLocalSpaceSeparatesCurrencySymbolAndNegativeValue();

		/** Returns the position of the sign, for locally formatted
		 *  negative monetary values, for the current locale, as
		 *  follows:
		 *
		 *  MONETARY_SIGN_POSITION_PARENTHESES if parentheses surround
		 *  the entire string.  Eg. (-$100)
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_STRING if the sign comes
		 *  before the entire string.  Eg. -$100
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_STRING if the sign comes
		 *  after the entire string. Eg. $100-
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_SYMBOL if the sign comes
		 *  immediately before the currency symbol.  Eg. 100-$
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_SYMBOL if the sign comes
		 *  immediately after the currency symbol.  Eg. 100$-
		 *
		 *  MONETARY_SIGN_POSITION_ERROR and sets errno on error. */
		static monetary_sign_position_t
		getLocalMonetaryNegativeSignPosition();

		/** Returns the ISO 4217:1995 international currency symbol
		 *  string, for the current locale.  Eg. "USD"
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getInternationalCurrencySymbol();

		/** Returns the separator character that goes between the
		 *  ISO 4217:1995 international currency symbol and the
		 *  monetary amount, for the current locale.  Eg. " " (a space)
		 *
		 *  Returns NULL and sets errno on error. */
		static char	*getInternationalCurrencySymbolSeparator();

		/** Returns the number of digits after the decimal point
		 *  for an internationally formatted monetary value, for the
		 *  current locale.
		 *
		 *  Returns 0 and sets errno on error. */
		static uint8_t	getInternationalMonetaryDecimalDigits();

		/** Returns true if the currency symbol preceeds a positive
		 *  monetary value, for the current locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for $100
		 *  * false for 100$
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getInternationalCurrencySymbolPreceedsPositiveValue();

		/** Returns true if a space separates the currency symbol and a
		 *  internationally formatted positive monetary value, for the
		 *  current locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for $ 100
		 *  * false for $100
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getInternationalSpaceSeparatesCurrencySymbolAndPositiveValue();

		/** Returns the position of the sign, for internationally
		 *  formatted positive monetary values, for the current locale,
		 *  as follows:
		 *
		 *  MONETARY_SIGN_POSITION_PARENTHESES if parentheses surround
		 *  the entire string.  Eg. ($100)
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_STRING if the sign comes
		 *  before the entire string.  Eg. +$100
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_STRING if the sign comes
		 *  after the entire string. Eg. $100+
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_SYMBOL if the sign comes
		 *  immediately before the currency symbol.  Eg. 100+$
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_SYMBOL if the sign comes
		 *  immediately after the currency symbol.  Eg. 100$+
		 *
		 *  MONETARY_SIGN_POSITION_ERROR and sets errno on error. */
		static monetary_sign_position_t
		getInternationalMonetaryPositiveSignPosition();

		/** Returns true if the currency symbol preceeds a
		 *  internationally formatted negative monetary value, for the
		 *  current locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for -$100
		 *  * false for 100$-
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getInternationalCurrencySymbolPreceedsNegativeValue();

		/** Returns true if a space separates the currency symbol and a
		 *  internationally formatted negative monetary value, for the
		 *  current locale, and false otherwise.
		 *
		 *  Eg.
		 *  * true for -$ 100
		 *  * false for -$100
		 *
		 *  Returns false and sets errno on error. */
		static bool
		getInternationalSpaceSeparatesCurrencySymbolAndNegativeValue();

		/** Returns the position of the sign, for internationally
		 *  formatted negative monetary values, for the current locale,
		 *  as follows:
		 *
		 *  MONETARY_SIGN_POSITION_PARENTHESES if parentheses surround
		 *  the entire string.  Eg. (-$100)
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_STRING if the sign comes
		 *  before the entire string.  Eg. -$100
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_STRING if the sign comes
		 *  after the entire string. Eg. $100-
		 *
		 *  MONETARY_SIGN_POSITION_BEFORE_SYMBOL if the sign comes
		 *  immediately before the currency symbol.  Eg. 100-$
		 *
		 *  MONETARY_SIGN_POSITION_AFTER_SYMBOL if the sign comes
		 *  immediately after the currency symbol.  Eg. 100$-
		 *
		 *  MONETARY_SIGN_POSITION_ERROR and sets errno on error. */
		static monetary_sign_position_t
		getInternationalMonetaryNegativeSignPosition();

		/** Returns true if the platform supports locales and
		 *  false otherwise. */
		static bool	isSupported();

	#include <rudiments/private/locale.h>
};

#endif
