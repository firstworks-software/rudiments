// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/locale.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include <rudiments/sys.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("locale");

	// bail if not supported
	if (!locale::isSupported()) {
		stdoutput.printf("not supported\n");
		return 0;
	}

	// NetBSD has some strangeness
	char	*osname=sys::getOperatingSystemName();
	bool	isfreebsd=!charstring::compare(osname,"FreeBSD");
	bool	isnetbsd=!charstring::compare(osname,"NetBSD");
	bool	isopenbsd=!charstring::compare(osname,"OpenBSD");
	bool	issolaris=!charstring::compare(osname,"SunOS");
	bool	isdarwin=!charstring::compare(osname,"Darwin");
	delete[] osname;

	// default locale
	stdoutput.printf("default locale (C):\n");
	if (locale::isAllSupported()) {
		test("getAll",
			!charstring::compare(locale::getAll(),"C"));
	}
	if (locale::isCollateSupported()) {
		test("getCollate",
			!charstring::compare(locale::getCollate(),"C"));
	}
	if (locale::isCTypeSupported()) {
		test("getCType",
			!charstring::compare(locale::getCType(),"C"));
	}
	if (locale::isMessagesSupported()) {
		test("getMessages",
			!charstring::compare(locale::getMessages(),"C"));
	}
	if (locale::isMonetarySupported()) {
		test("getMonetary",
			!charstring::compare(locale::getMonetary(),"C"));
	}
	if (locale::isNumericSupported()) {
		test("getNumeric",
			!charstring::compare(locale::getNumeric(),"C"));
	}
	if (locale::isTimeSupported()) {
		test("getTime",
			!charstring::compare(locale::getTime(),"C"));
	}
	if (locale::isAddressSupported()) {
		test("getAddress",
			!charstring::compare(locale::getAddress(),"C"));
	}
	if (locale::isIdentificationSupported()) {
		test("getIdentification",
			!charstring::compare(locale::getIdentification(),"C"));
	}
	if (locale::isMeasurementSupported()) {
		test("getMeasurement",
			!charstring::compare(locale::getMeasurement(),"C"));
	}
	if (locale::isNameSupported()) {
		test("getName",
			!charstring::compare(locale::getName(),"C"));
	}
	if (locale::isPaperSupported()) {
		test("getPaper",
			!charstring::compare(locale::getPaper(),"C"));
	}
	if (locale::isTelephoneSupported()) {
		test("getTelephone",
			!charstring::compare(locale::getTelephone(),"C"));
	}
	stdoutput.printf("\n");

	// set LC_ALL to another locale...

	// try en_US.UTF-8, but if that fails (eg. on Solaris 10),
	// fall back to POSIX
	const char	*l="en_US.UTF-8";
	if (!locale::setAll(l)) {
		l="POSIX";
	}

	stdoutput.printf("set LC_ALL to %s:\n",l);
	test("setAll",locale::setAll(l));

	// on NetBSD, LC_ALL isn't just a single value, but rather
	// a /-delimited list of the values for the other categories,
	// so skip testing that on NetBSD
	if (!isnetbsd && locale::isAllSupported()) {
		test("getAll",
			!charstring::compare(locale::getAll(),l));
	}
	// on NetBSD, LC_COLLATE is always set to C so, skip testing
	// that on NetBSD
	if (!isnetbsd && locale::isCollateSupported()) {
		test("getCollate",
			!charstring::compare(locale::getCollate(),l));
	}
	if (locale::isCTypeSupported()) {
		test("getCType",
			!charstring::compare(locale::getCType(),l));
	}
	if (locale::isMessagesSupported()) {
		test("getMessages",
			!charstring::compare(locale::getMessages(),l));
	}
	if (locale::isMonetarySupported()) {
		test("getMonetary",
			!charstring::compare(locale::getMonetary(),l));
	}
	if (locale::isNumericSupported()) {
		test("getNumeric",
			!charstring::compare(locale::getNumeric(),l));
	}
	if (locale::isTimeSupported()) {
		test("getTime",
			!charstring::compare(locale::getTime(),l));
	}
	if (locale::isAddressSupported()) {
		test("getAddress",
			!charstring::compare(locale::getAddress(),l));
	}
	if (locale::isIdentificationSupported()) {
		test("getIdentification",
			!charstring::compare(locale::getIdentification(),l));
	}
	if (locale::isMeasurementSupported()) {
		test("getMeasurement",
			!charstring::compare(locale::getMeasurement(),l));
	}
	if (locale::isNameSupported()) {
		test("getName",
			!charstring::compare(locale::getName(),l));
	}
	if (locale::isPaperSupported()) {
		test("getPaper",
			!charstring::compare(locale::getPaper(),l));
	}
	if (locale::isTelephoneSupported()) {
		test("getTelephone",
			!charstring::compare(locale::getTelephone(),l));
	}
	stdoutput.printf("\n");

	// set individual categories back to C
	stdoutput.printf("individuals back to C:\n");

	if (locale::isCollateSupported()) {
		test("setCollate",locale::setCollate("C"));
		test("getCollate",
			!charstring::compare(locale::getCollate(),"C"));
	}
	test("others not C",
		(!locale::isCTypeSupported() ||
			charstring::compare(locale::getCType(),"C")) &&
		(!locale::isMessagesSupported() ||
			charstring::compare(locale::getMessages(),"C")) &&
		(!locale::isMonetarySupported() ||
			charstring::compare(locale::getMonetary(),"C")) &&
		(!locale::isNumericSupported() ||
			charstring::compare(locale::getNumeric(),"C")) &&
		(!locale::isTimeSupported() ||
			charstring::compare(locale::getTime(),"C")) &&
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isCTypeSupported()) {
		test("setCType",locale::setCType("C"));
		test("getCType",
			!charstring::compare(locale::getCType(),"C"));
	}
	test("others not C",
		(!locale::isMessagesSupported() ||
			charstring::compare(locale::getMessages(),"C")) &&
		(!locale::isMonetarySupported() ||
			charstring::compare(locale::getMonetary(),"C")) &&
		(!locale::isNumericSupported() ||
			charstring::compare(locale::getNumeric(),"C")) &&
		(!locale::isTimeSupported() ||
			charstring::compare(locale::getTime(),"C")) &&
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isMessagesSupported()) {
		test("setMessages",locale::setMessages("C"));
		test("getMessages",
			!charstring::compare(locale::getMessages(),"C"));
	}
	test("others not C",
		(!locale::isMonetarySupported() ||
			charstring::compare(locale::getMonetary(),"C")) &&
		(!locale::isNumericSupported() ||
			charstring::compare(locale::getNumeric(),"C")) &&
		(!locale::isTimeSupported() ||
			charstring::compare(locale::getTime(),"C")) &&
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isMonetarySupported()) {
		test("setMonetary",locale::setMonetary("C"));
		test("getMonetary",
			!charstring::compare(locale::getMonetary(),"C"));
	}
	test("others not C",
		(!locale::isNumericSupported() ||
			charstring::compare(locale::getNumeric(),"C")) &&
		(!locale::isTimeSupported() ||
			charstring::compare(locale::getTime(),"C")) &&
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isNumericSupported()) {
		test("setNumeric",locale::setNumeric("C"));
		test("getNumeric",
			!charstring::compare(locale::getNumeric(),"C"));
	}
	test("others not C",
		(!locale::isTimeSupported() ||
			charstring::compare(locale::getTime(),"C")) &&
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isTimeSupported()) {
		test("setTime",locale::setTime("C"));
		test("getTime",
			!charstring::compare(locale::getTime(),"C"));
	}
	test("others not C",
		(!locale::isAddressSupported() ||
			charstring::compare(locale::getAddress(),"C")) &&
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isAddressSupported()) {
		test("setAddress",locale::setAddress("C"));
		test("getAddress",
			!charstring::compare(locale::getAddress(),"C"));
	}
	test("others not C",
		(!locale::isIdentificationSupported() ||
			charstring::compare(
				locale::getIdentification(),"C")) &&
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isIdentificationSupported()) {
		test("setIdentification",locale::setIdentification("C"));
		test("getIdentification",
			!charstring::compare(locale::getIdentification(),"C"));
	}
	test("others not C",
		(!locale::isMeasurementSupported() ||
			charstring::compare(locale::getMeasurement(),"C")) &&
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isMeasurementSupported()) {
		test("setMeasurement",locale::setMeasurement("C"));
		test("getMeasurement",
			!charstring::compare(locale::getMeasurement(),"C"));
	}
	test("others not C",
		(!locale::isNameSupported() ||
			charstring::compare(locale::getName(),"C")) &&
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isNameSupported()) {
		test("setName",locale::setName("C"));
		test("getName",
			!charstring::compare(locale::getName(),"C"));
	}
	test("others not C",
		(!locale::isPaperSupported() ||
			charstring::compare(locale::getPaper(),"C")) &&
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isNameSupported()) {
		test("setPaper",locale::setPaper("C"));
		test("getPaper",
			!charstring::compare(locale::getPaper(),"C"));
	}
	test("others not C",
		(!locale::isTelephoneSupported() ||
			charstring::compare(locale::getTelephone(),"C")));

	if (locale::isTelephoneSupported()) {
		test("setTelephone",locale::setTelephone("C"));
		test("getTelephone",
			!charstring::compare(locale::getTelephone(),"C"));
	}
	stdoutput.printf("\n");

	stdoutput.printf("strings and settings for the C locale:\n");
	char	*c=NULL;
	uint8_t	n=0;
	bool	b=false;
	monetary_sign_position_t	m;
	c=locale::getNumericDecimalPoint();
	test("numeric decimal point",!charstring::compare(c,"."));
	delete[] c;
	c=locale::getNumericDigitGroupSeparator();
	test("numeric digit group separator",!charstring::compare(c,""));
	delete[] c;
	n=locale::getNumericDigitGroupCount(0);
	test("numeric digit group count(0)",n==0);
	c=locale::getMonetaryDecimalPoint();
	test("monetary decimal point",!charstring::compare(c,""));
	delete[] c;
	c=locale::getMonetaryDigitGroupSeparator();
	test("monetary digit group separator",!charstring::compare(c,""));
	delete[] c;
	n=locale::getMonetaryDigitGroupCount(0);
	test("monetary digit group count(0)",n==0);
	c=locale::getMonetaryPositiveSign();
	test("monetary positive sign",!charstring::compare(c,""));
	delete[] c;
	c=locale::getMonetaryNegativeSign();
	test("monetary negative sign",!charstring::compare(c,""));
	delete[] c;

	c=locale::getLocalCurrencySymbol();
	test("local currency symbol",!charstring::compare(c,""));
	delete[] c;
	n=locale::getLocalMonetaryDecimalDigits();
	test("local monetary decimal digits",n==127);
	b=locale::getLocalCurrencySymbolPreceedsPositiveValue();
	test("local currency symbol preceeds positive value",!b);
	b=locale::getLocalSpaceSeparatesCurrencySymbolAndPositiveValue();
	test("local space separates currency symbol and positive value",!b);
	m=locale::getLocalMonetaryPositiveSignPosition();
	test("local monetary sign position for positive values",
			m==MONETARY_SIGN_POSITION_ERROR);
	b=locale::getLocalCurrencySymbolPreceedsNegativeValue();
	test("local currency symbol preceeds negative value",!b);
	b=locale::getLocalSpaceSeparatesCurrencySymbolAndNegativeValue();
	test("local space separates currency symbol and negative value",!b);
	m=locale::getLocalMonetaryNegativeSignPosition();
	test("local monetary sign position for negative values",
			m==MONETARY_SIGN_POSITION_ERROR);
	c=locale::getInternationalCurrencySymbol();
	test("int currency symbol",!charstring::compare(c,""));
	delete[] c;
	c=locale::getInternationalCurrencySymbolSeparator();
	test("int currency separator",!charstring::compare(c,""));
	delete[] c;
	n=locale::getInternationalMonetaryDecimalDigits();
	test("int monetary decimal digits",n==127);
	c=locale::getInternationalCurrencySymbol();
	test("int currency symbol",!charstring::compare(c,""));
	delete[] c;
	n=locale::getInternationalMonetaryDecimalDigits();
	test("int monetary decimal digits",n==127);
	b=locale::getInternationalCurrencySymbolPreceedsPositiveValue();
	test("int currency symbol preceeds positive value",!b);
	b=locale::getInternationalSpaceSeparatesCurrencySymbolAndPositiveValue();
	test("int space separates currency symbol and positive value",!b);
	m=locale::getInternationalMonetaryPositiveSignPosition();
	test("int monetary sign position for positive values",
			m==MONETARY_SIGN_POSITION_ERROR);
	b=locale::getInternationalCurrencySymbolPreceedsNegativeValue();
	test("int currency symbol preceeds negative value",!b);
	b=locale::getInternationalSpaceSeparatesCurrencySymbolAndNegativeValue();
	test("int space separates currency symbol and negative value",!b);
	m=locale::getInternationalMonetaryNegativeSignPosition();
	test("int monetary sign position for negative values",
			m==MONETARY_SIGN_POSITION_ERROR);
	stdoutput.printf("\n");

	// openbsd's en_US.UTF-8 is very inconsistent, compared to other
	// platforms, so just bail on this entirely for openbsd
	if (locale::setAll("en_US.UTF-8") && !isopenbsd) {
		stdoutput.printf("strings and settings for "
					"the en_US-UTF-8 locale:\n");
		c=locale::getNumericDecimalPoint();
		test("numeric decimal point",!charstring::compare(c,"."));
		delete[] c;

		// inconsistent across platforms
		if (!issolaris) {
			c=locale::getNumericDigitGroupSeparator();
			test("numeric digit group separator",
						!charstring::compare(c,","));
			delete[] c;
		}

		n=locale::getNumericDigitGroupCount(0);
		test("numeric digit group count(0)",n==3);
		c=locale::getMonetaryDecimalPoint();
		test("monetary decimal point",!charstring::compare(c,"."));
		delete[] c;
		c=locale::getMonetaryDigitGroupSeparator();
		test("monetary digit group separator",
					!charstring::compare(c,","));
		delete[] c;
		n=locale::getMonetaryDigitGroupCount(0);
		test("monetary digit group count(0)",n==3);
		c=locale::getMonetaryPositiveSign();
		test("monetary positive sign",!charstring::compare(c,""));
		delete[] c;
		c=locale::getMonetaryNegativeSign();
		test("monetary negative sign",!charstring::compare(c,"-"));
		delete[] c;
		c=locale::getLocalCurrencySymbol();
		test("local currency symbol",!charstring::compare(c,"$"));
		delete[] c;
		n=locale::getLocalMonetaryDecimalDigits();
		test("local monetary decimal digits",n==2);
		b=locale::getLocalCurrencySymbolPreceedsPositiveValue();
		test("local currency symbol preceeds positive value",b);
		b=locale::getLocalSpaceSeparatesCurrencySymbolAndPositiveValue();
		test("local space separates currency symbol and positive value",
			!b);
		m=locale::getLocalMonetaryPositiveSignPosition();
		test("local monetary sign position for positive values",
				m==MONETARY_SIGN_POSITION_BEFORE_STRING);
		b=locale::getLocalCurrencySymbolPreceedsNegativeValue();
		test("local currency symbol preceeds negative value",b);
		b=locale::getLocalSpaceSeparatesCurrencySymbolAndNegativeValue();
		test("local space separates currency symbol and negative value",
			!b);
		m=locale::getLocalMonetaryNegativeSignPosition();
		test("local monetary sign position for negative values",
				m==MONETARY_SIGN_POSITION_BEFORE_STRING);
		c=locale::getInternationalCurrencySymbol();
		test("int currency symbol",!charstring::compare(c,"USD"));
		delete[] c;
		c=locale::getInternationalCurrencySymbolSeparator();
		test("int currency separator",!charstring::compare(c," "));
		delete[] c;
		n=locale::getInternationalMonetaryDecimalDigits();
		test("int monetary decimal digits",n==2);

		// inconsistent across platforms
		if (!issolaris) {
			b=locale::getInternationalCurrencySymbolPreceedsPositiveValue();
			test("int currency symbol preceeds positive value",b);
			if (!isnetbsd && !isdarwin) {
				b=locale::getInternationalSpaceSeparatesCurrencySymbolAndPositiveValue();
				test("int space separates currency symbol and positive value",b);
			}
			m=locale::getInternationalMonetaryPositiveSignPosition();
			test("int monetary sign position for positive values",
					m==MONETARY_SIGN_POSITION_BEFORE_STRING);
			b=locale::getInternationalCurrencySymbolPreceedsNegativeValue();
			test("int currency symbol preceeds negative value",b);
			if (!isnetbsd && !isdarwin) {
				b=locale::getInternationalSpaceSeparatesCurrencySymbolAndNegativeValue();
				test("int space separates currency symbol and negative value",b);
			}
			m=locale::getInternationalMonetaryNegativeSignPosition();
			test("int monetary sign position for negative values",
					m==MONETARY_SIGN_POSITION_BEFORE_STRING);
		}
		stdoutput.printf("\n");
	}

	return 0;
}
