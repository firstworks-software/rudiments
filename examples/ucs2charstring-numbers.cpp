#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// conversion of numbers to strings...
	ucs2_t	*intstr=ucs2charstring::parseNumber((uint64_t)12345);
	ucs2_t	*floatstr=ucs2charstring::parseNumber((float)12.345,5,3);

	char	*intstrstr=charstring::duplicateUcs2(intstr);
	char	*floatstrstr=charstring::duplicateUcs2(floatstr);
	stdoutput.printf("numbers as strings: %s, %s\n",intstrstr,floatstrstr);
	stdoutput.write('\n');

	delete[] intstrstr;
	delete[] floatstrstr;
	delete[] intstr;
	delete[] floatstr;



	// conversion of strings to numbers...
	ucs2literal(intlit,"12345");
	ucs2literal(floatlit,"12.345");
	int64_t		intnum=ucs2charstring::convertToInteger(intlit);
	uint64_t	uintnum=ucs2charstring::convertToUnsignedInteger(intlit);
	long double	floatnum=ucs2charstring::convertToFloat(floatlit);

	stdoutput.printf("strings as numbers: %lld, %lld, %5.3Lf\n",
					intnum,uintnum,floatnum);
	stdoutput.write('\n');



	// identification of numeric strings...
	ucs2literal(n1,"1");
	ucs2literal(nm1,"-1");
	ucs2literal(n1p1,"1.1");
	ucs2literal(nm1p1,"-1.1");
	ucs2literal(none,"one");
	ucs2literal(nhello,"hello");

	const ucs2_t * const numbers[]={
		n1,nm1,n1p1,nm1p1,none,nhello,NULL
	};

	for (const ucs2_t * const *n=numbers; *n; n++) {
		char	*nstr=charstring::duplicateUcs2(*n);
		stdoutput.printf("%s %s a number\n",nstr,
				(ucs2charstring::isNumber(*n))?"is":"is not");
		stdoutput.printf("%s %s an integer\n",nstr,
				(ucs2charstring::isInteger(*n))?"is":"is not");
		delete[] nstr;
	}
	stdoutput.write('\n');



	// integer lengths
	uint64_t	integers[]={
		1,23,456,7890,12345,678901,0
	};

	for (uint64_t *i=integers; *i; i++) {
		stdoutput.printf("it would take %d characters to store "
				"%lld as a string\n",
				ucs2charstring::getIntegerLength(*i),*i);
	}
	stdoutput.write('\n');



	// dollar amounts
	ucs2literal(dollarstr,"$123.45");
	int64_t	pennies=ucs2charstring::convertAmount(dollarstr);
	ucs2_t	*dollars=ucs2charstring::convertAmount(pennies);
	char	*dollarstrstr=charstring::duplicateUcs2(dollarstr);
	char	*dollarsstr=charstring::duplicateUcs2(dollars);
	stdoutput.printf("%s as pennies: %lld\n",dollarstrstr,pennies);
	stdoutput.printf("%lld pennies as dollars: %s\n",pennies,dollarsstr);

	delete[] dollarstrstr;
	delete[] dollarsstr;
}
