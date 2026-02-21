#include <rudiments/locale.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display whether locales are supported...
	stdoutput.printf("locales supported: %s\n\n",
			(locale::isSupported())?"yes":"no");


	// display the current locale categories...
	stdoutput.printf("locale categories:\n");

	const char	*val;
	val=locale::getAll();
	stdoutput.printf("  LC_ALL:      %s\n",(val)?val:"(null)");
	val=locale::getCollate();
	stdoutput.printf("  LC_COLLATE:  %s\n",(val)?val:"(null)");
	val=locale::getCType();
	stdoutput.printf("  LC_CTYPE:    %s\n",(val)?val:"(null)");
	val=locale::getMessages();
	stdoutput.printf("  LC_MESSAGES: %s\n",(val)?val:"(null)");
	val=locale::getMonetary();
	stdoutput.printf("  LC_MONETARY: %s\n",(val)?val:"(null)");
	val=locale::getNumeric();
	stdoutput.printf("  LC_NUMERIC:  %s\n",(val)?val:"(null)");
	val=locale::getTime();
	stdoutput.printf("  LC_TIME:     %s\n",(val)?val:"(null)");
	stdoutput.write('\n');


	// set the locale to the system default...
	locale::setAll("");
	stdoutput.printf("after setting LC_ALL to system default:\n");
	val=locale::getAll();
	stdoutput.printf("  LC_ALL: %s\n",(val)?val:"(null)");
	stdoutput.write('\n');


	// display numeric formatting info...
	stdoutput.printf("numeric formatting:\n");

	char	*dp=locale::getNumericDecimalPoint();
	stdoutput.printf("  decimal point:          \"%s\"\n",
			(dp)?dp:"(null)");
	delete[] dp;

	char	*sep=locale::getNumericDigitGroupSeparator();
	stdoutput.printf("  digit group separator:  \"%s\"\n",
			(sep)?sep:"(null)");
	delete[] sep;

	stdoutput.printf("  digit group count (0):  %d\n",
			locale::getNumericDigitGroupCount(0));
	stdoutput.write('\n');


	// display monetary formatting info...
	stdoutput.printf("monetary formatting:\n");

	dp=locale::getMonetaryDecimalPoint();
	stdoutput.printf("  decimal point:          \"%s\"\n",
			(dp)?dp:"(null)");
	delete[] dp;

	sep=locale::getMonetaryDigitGroupSeparator();
	stdoutput.printf("  digit group separator:  \"%s\"\n",
			(sep)?sep:"(null)");
	delete[] sep;

	char	*sym=locale::getLocalCurrencySymbol();
	stdoutput.printf("  local currency symbol:  \"%s\"\n",
			(sym)?sym:"(null)");
	delete[] sym;

	char	*possign=locale::getMonetaryPositiveSign();
	stdoutput.printf("  positive sign:          \"%s\"\n",
			(possign)?possign:"(null)");
	delete[] possign;

	char	*negsign=locale::getMonetaryNegativeSign();
	stdoutput.printf("  negative sign:          \"%s\"\n",
			(negsign)?negsign:"(null)");
	delete[] negsign;

	stdoutput.printf("  decimal digits:         %d\n",
			locale::getLocalMonetaryDecimalDigits());
	stdoutput.printf("  symbol preceeds pos:    %s\n",
			(locale::getLocalCurrencySymbolPreceedsPositiveValue())?
			"yes":"no");
	stdoutput.printf("  symbol preceeds neg:    %s\n",
			(locale::getLocalCurrencySymbolPreceedsNegativeValue())?
			"yes":"no");
	stdoutput.write('\n');


	// display international currency info...
	stdoutput.printf("international currency:\n");

	char	*isym=locale::getInternationalCurrencySymbol();
	stdoutput.printf("  currency symbol:        \"%s\"\n",
			(isym)?isym:"(null)");
	delete[] isym;

	char	*isep=locale::getInternationalCurrencySymbolSeparator();
	stdoutput.printf("  symbol separator:       \"%s\"\n",
			(isep)?isep:"(null)");
	delete[] isep;

	stdoutput.printf("  decimal digits:         %d\n",
			locale::getInternationalMonetaryDecimalDigits());
}
