#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// conversion of numbers to strings...
	wchar_t	*intstr=wcharstring::parseNumber((uint64_t)12345);
	wchar_t	*floatstr=wcharstring::parseNumber((float)12.345,5,3);

	stdoutput.printf("numbers as strings: %ls, %ls\n",intstr,floatstr);
	stdoutput.write('\n');

	delete[] intstr;
	delete[] floatstr;



	// conversion of strings to numbers...
	int64_t		intnum=wcharstring::convertToInteger(L"12345");
	uint64_t	uintnum=wcharstring::convertToUnsignedInteger(L"12345");
	long double	floatnum=wcharstring::convertToFloat(L"12.345");

	stdoutput.printf("strings as numbers: %lld, %lld, %5.3Lf\n",
					intnum,uintnum,floatnum);
	stdoutput.write('\n');



	// identification of numeric strings...
	const wchar_t * const numbers[]={
		L"1",L"-1",L"1.1",L"-1.1",L"one",L"hello",NULL
	};

	for (const wchar_t * const *n=numbers; *n; n++) {
		stdoutput.printf("%ls %s a number\n",*n,
				(wcharstring::isNumber(*n))?"is":"is not");
		stdoutput.printf("%ls %s an integer\n",*n,
				(wcharstring::isInteger(*n))?"is":"is not");
	}
	stdoutput.write('\n');



	// integer lengths
	uint64_t	integers[]={
		1,23,456,7890,12345,678901,0
	};

	for (uint64_t *i=integers; *i; i++) {
		stdoutput.printf("it would take %d characters to store "
				"%lld as a string\n",
				wcharstring::getIntegerLength(*i),*i);
	}
	stdoutput.write('\n');



	// dollar amounts
	const wchar_t	dollarstr[]=L"$123.45";
	int64_t	pennies=wcharstring::convertAmount(dollarstr);
	wchar_t	*dollars=wcharstring::convertAmount(pennies);
	stdoutput.printf("%ls as pennies: %lld\n",dollarstr,pennies);
	stdoutput.printf("%lld pennies as dollars: %ls\n",pennies,dollars);
}
