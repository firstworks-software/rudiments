#include <rudiments/bignumber.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// check whether bignumber is supported
	if (!bignumber().isSupported()) {
		stdoutput.write("bignumber is not supported\n");
		return 1;
	}


	// construct large values from decimal strings
	bignumber	a("123456789012345678901234567890");
	bignumber	b("-987654321098765432109876543210");


	// arithmetic operators work like the native integer types
	bignumber	sum=a+b;
	bignumber	product=a*b;
	stdoutput.printf("a+b = %s\n",sum.getString());
	stdoutput.printf("a*b = %s\n",product.getString());


	// division truncates toward zero, and the remainder takes the sign
	// of the dividend, just like the native integer types
	bignumber	quotient(-7);
	bignumber	remainder;
	quotient.divide(bignumber(2),&remainder);
	stdoutput.printf("-7/2 = %s remainder %s\n",
				quotient.getString(),remainder.getString());


	// values can also be built from hex strings, and converted back to
	// either base.  Each instance owns a single cached string buffer,
	// reused by the next call to getString(), so the decimal string has
	// to be copied out before asking for the hex string.
	bignumber	c("DEADBEEF",16);
	char		*decimal=charstring::duplicate(c.getString());
	stdoutput.printf("c = %s (0x%s)\n",decimal,c.getString(16));
	delete[] decimal;


	// modular exponentiation, useful for diffie-hellman, rsa, and other
	// public-key protocols
	bignumber	base(3);
	base.modPow(bignumber(5),bignumber(7));
	stdoutput.printf("3^5 mod 7 = %s\n",base.getString());


	// the modular arithmetic methods always return a non-negative
	// result, in place, unlike modulo() and operator%()
	bignumber	dividend(-7);
	bignumber	nonnegative(dividend);
	nonnegative.nonNegativeModulo(bignumber(3));
	char		*nonnegativestr=charstring::duplicate(
						nonnegative.getString());
	stdoutput.printf("-7 nonNegativeModulo 3 = %s (-7%%3 is %s)\n",
			nonnegativestr,(dividend%bignumber(3)).getString());
	delete[] nonnegativestr;

	return 0;
}
