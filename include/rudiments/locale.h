// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOCALE_H
#define RUDIMENTS_LOCALE_H

#include <rudiments/private/localeincludes.h>

/** The locale class provides static methods for accessing and manipulating
 *  locale categories. */
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

		/** Returns true if the platform supports locales and
		 *  false otherwise. */
		static bool	isSupported();
};

#endif
