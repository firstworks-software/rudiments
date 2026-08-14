// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_BIGNUMBER_H
#define RUDIMENTS_BIGNUMBER_H

#include <rudiments/private/bignumberincludes.h>

/** bignumber errors */
enum bignumbererror_t {
	BIGNUMBER_ERROR_SUCCESS=0,
	BIGNUMBER_ERROR_NULL,
	BIGNUMBER_ERROR_INVALID_FORMAT,
	BIGNUMBER_ERROR_INVALID_BASE,
	BIGNUMBER_ERROR_DIVIDE_BY_ZERO,
	BIGNUMBER_ERROR_OVERFLOW,
	BIGNUMBER_ERROR_BUFFER_TOO_SMALL,
	BIGNUMBER_ERROR_OUT_OF_MEMORY,
	BIGNUMBER_ERROR_INVALID_MODULUS,
	BIGNUMBER_ERROR_NEGATIVE_EXPONENT,
	BIGNUMBER_ERROR_NO_INVERSE
};

/** The bignumber class provides an arbitrary-precision signed integer.
 *
 *  A bignumber is a value type, like an int, rather than a stateful
 *  object like a cipher.  Instances may be freely copied, assigned, and
 *  passed by value.  There is no practical limit on the magnitude of a
 *  bignumber, other than the amount of memory available.
 *
 *  Internally, a bignumber is stored in signed magnitude form - a sign
 *  and a magnitude, rather than a two's complement bit pattern.  Zero is
 *  always positive.  There is no negative zero.
 *
 *  Arithmetic follows the same rules as the native C++ integer types:
 *
 *  * Division truncates toward zero.
 *  * The remainder takes the sign of the dividend.
 *
 *  So, for example, -7/2 is -3 and -7%2 is -1, while 7/-2 is -3 and
 *  7%-2 is 1.
 *
 *  The modular arithmetic methods - nonNegativeModulo(), modAdd(),
 *  modSub(), modMul(), modPow(), and modInverse() - follow the rules of
 *  modular arithmetic rather than the rules of the native C++ integer
 *  types.  Their results are always non-negative, in the range 0 through
 *  the magnitude of the modulus minus 1, no matter what the signs of the
 *  operands are.  This is unlike modulo() and operator%(), whose result
 *  takes the sign of the dividend.  For example, modulo() on -7 with 2
 *  gives -1, while nonNegativeModulo() on -7 with 2 gives 1.  The
 *  companion method gcd() computes the greatest common divisor, which is
 *  also always non-negative.
 *
 *  The bitwise operators (&, |, ^, and ~) behave as if each operand were
 *  an infinitely wide two's complement value, which is the same behavior
 *  that Java's BigInteger and Python's int provide.  In particular, ~x
 *  is defined as -x-1.  See the individual methods for details.
 *
 *  The shift operators (<< and >>) shift the magnitude and preserve the
 *  sign.  A right shift therefore truncates toward zero, rather than
 *  toward negative infinity.  So, -7>>1 is -3, and not the -4 that a
 *  native, two's complement, arithmetic right shift would give.  See
 *  operator>>() for details.
 *
 *  Operations that can fail - parsing a malformed string, dividing by
 *  zero, or converting a value that is too large to a native integer
 *  type - do not throw exceptions.  They report failure through their
 *  return value, and the reason for the failure is available from
 *  getError().
 *
 *  Note that, like other rudiments classes, none of the methods below
 *  are const methods, and a bignumber must be non-const to be used in an
 *  expression.
 *
 *  Also note that the arithmetic, comparison, and bitwise operators are
 *  defined for bignumber operands only.  An expression that mixes a
 *  bignumber with a native integer, such as bn+5, requires an explicit
 *  bignumber temporary, such as bn+bignumber(5).
 *
 *  When available, this class uses OpenSSL's BIGNUM implementation
 *  internally.  Otherwise it falls back to a built-in implementation. */
class RUDIMENTS_DLLSPEC bignumber {
	public:

		/** Creates an instance of the bignumber class with the
		 *  value 0. */
		bignumber();

		/** Creates an instance of the bignumber class with the
		 *  value given by the base-10 (decimal) representation in
		 *  "value".
		 *
		 *  See setValue(const char *) for the format that "value"
		 *  must have.  If "value" is malformed, then the instance
		 *  is created with the value 0 and getError() will return
		 *  the reason. */
		bignumber(const char *value);

		/** Creates an instance of the bignumber class with the
		 *  value given by the base-"base" representation in
		 *  "value".  Only bases 10 (decimal) and 16
		 *  (hexadecimal) are supported.
		 *
		 *  See setValue(const char *, uint16_t) for the format
		 *  that "value" must have.  If "value" is malformed, or
		 *  if "base" is not 10 or 16, then the instance is
		 *  created with the value 0 and getError() will return
		 *  the reason. */
		bignumber(const char *value, uint16_t base);

		/** Creates an instance of the bignumber class with the
		 *  value "value". */
		bignumber(int32_t value);

		/** Creates an instance of the bignumber class with the
		 *  value "value". */
		bignumber(uint32_t value);

		/** Creates an instance of the bignumber class with the
		 *  value "value". */
		bignumber(int64_t value);

		/** Creates an instance of the bignumber class with the
		 *  value "value". */
		bignumber(uint64_t value);

		/** Creates an instance of the bignumber class with the
		 *  positive value whose magnitude is given by the first
		 *  "size" bytes of "magnitude", most significant byte
		 *  first (big-endian).
		 *
		 *  If "magnitude" is NULL, or "size" is 0, then the
		 *  instance is created with the value 0. */
		bignumber(const byte_t *magnitude, size_t size);

		/** Creates an instance of the bignumber class with the
		 *  value whose magnitude is given by the first "size"
		 *  bytes of "magnitude", most significant byte first
		 *  (big-endian), and which is negative if "negative" is
		 *  true and positive otherwise.
		 *
		 *  If "magnitude" is NULL, or "size" is 0, then the
		 *  instance is created with the value 0.  A zero
		 *  magnitude yields the value 0, even if "negative" is
		 *  true, as there is no negative zero. */
		bignumber(const byte_t *magnitude, size_t size,
							bool negative);

		/** Creates an instance of the bignumber class with the
		 *  same value as "value". */
		bignumber(const bignumber &value);

		/** Deletes this instance of the bignumber class. */
		~bignumber();

		/** Sets this instance to the value given by the base-10
		 *  (decimal) representation in "value".
		 *
		 *  "value" must consist of an optional leading + or -,
		 *  followed by one or more decimal digits.  Leading
		 *  zeros are allowed.  No other characters, including
		 *  whitespace, thousands separators, or a decimal point,
		 *  are allowed anywhere in the string.
		 *
		 *  Returns true on success, or false if "value" is NULL
		 *  or malformed.  On failure, this instance is set to 0
		 *  and getError() will return the reason. */
		bool	setValue(const char *value);

		/** Sets this instance to the value given by the
		 *  base-"base" representation in "value".  Only bases 10
		 *  (decimal) and 16 (hexadecimal) are supported.
		 *
		 *  For base 10, see setValue(const char *).
		 *
		 *  For base 16, "value" must consist of an optional
		 *  leading + or -, followed by one or more hexadecimal
		 *  digits, in upper case, lower case, or a mix of the
		 *  two.  A leading 0x or 0X is not allowed, matching the
		 *  format that OpenSSL's BN_hex2bn() accepts.
		 *
		 *  Returns true on success, or false if "value" is NULL
		 *  or malformed, or if "base" is not 10 or 16.  On
		 *  failure, this instance is set to 0 and getError()
		 *  will return the reason. */
		bool	setValue(const char *value, uint16_t base);

		/** Sets this instance to the value "value". */
		void	setValue(int32_t value);

		/** Sets this instance to the value "value". */
		void	setValue(uint32_t value);

		/** Sets this instance to the value "value". */
		void	setValue(int64_t value);

		/** Sets this instance to the value "value". */
		void	setValue(uint64_t value);

		/** Sets this instance to the positive value whose
		 *  magnitude is given by the first "size" bytes of
		 *  "magnitude", most significant byte first
		 *  (big-endian).
		 *
		 *  Returns true on success, or false if "magnitude" is
		 *  NULL.  If "size" is 0, then this instance is set to 0
		 *  and true is returned. */
		bool	setValue(const byte_t *magnitude, size_t size);

		/** Sets this instance to the value whose magnitude is
		 *  given by the first "size" bytes of "magnitude", most
		 *  significant byte first (big-endian), and which is
		 *  negative if "negative" is true and positive
		 *  otherwise.
		 *
		 *  A zero magnitude yields the value 0, even if
		 *  "negative" is true, as there is no negative zero.
		 *
		 *  Returns true on success, or false if "magnitude" is
		 *  NULL.  If "size" is 0, then this instance is set to 0
		 *  and true is returned. */
		bool	setValue(const byte_t *magnitude, size_t size,
							bool negative);

		/** Sets this instance to the same value as "value". */
		void	setValue(const bignumber &value);

		/** Sets this instance to the same value as "value" and
		 *  returns a reference to this instance. */
		bignumber	&operator=(const bignumber &value);

		/** Sets this instance to the value given by the base-10
		 *  (decimal) representation in "value", as
		 *  setValue(const char *) does, and returns a reference
		 *  to this instance. */
		bignumber	&operator=(const char *value);

		/** Sets this instance to the value "value" and returns a
		 *  reference to this instance. */
		bignumber	&operator=(int32_t value);

		/** Sets this instance to the value "value" and returns a
		 *  reference to this instance. */
		bignumber	&operator=(uint32_t value);

		/** Sets this instance to the value "value" and returns a
		 *  reference to this instance. */
		bignumber	&operator=(int64_t value);

		/** Sets this instance to the value "value" and returns a
		 *  reference to this instance. */
		bignumber	&operator=(uint64_t value);

		/** Returns the base-10 (decimal) representation of this
		 *  instance, with a leading - if this instance is
		 *  negative, and with no leading zeros.  The value 0 is
		 *  returned as the single character string "0".
		 *
		 *  The buffer returned by this method is owned by this
		 *  instance and remains valid until the next call to
		 *  getString() on this instance, or until this instance
		 *  is modified or deleted.  The caller should neither
		 *  modify nor delete it.
		 *
		 *  Returns NULL if the string could not be generated. */
		const char	*getString();

		/** Returns the base-"base" representation of this
		 *  instance, with a leading - if this instance is
		 *  negative.  Only bases 10 (decimal) and 16
		 *  (hexadecimal) are supported.
		 *
		 *  For base 10, there are no leading zeros.
		 *
		 *  Hexadecimal digits are returned in upper case, with
		 *  no leading 0x, two digits per byte of the magnitude,
		 *  matching the format that OpenSSL's BN_bn2hex()
		 *  returns and that setValue(const char *, uint16_t)
		 *  accepts.  Because the digits come two per byte rather
		 *  than one per significant digit, a magnitude whose
		 *  highest byte is less than 0x10 has a leading 0, e.g.
		 *  getString(16) on the value 0xABC returns "0ABC", not
		 *  "ABC".  The value 0 is returned as the single
		 *  character string "0".
		 *
		 *  The buffer returned by this method is owned by this
		 *  instance and remains valid until the next call to
		 *  getString() on this instance, or until this instance
		 *  is modified or deleted.  The caller should neither
		 *  modify nor delete it.
		 *
		 *  Returns NULL if "base" is not 10 or 16, or if the
		 *  string could not be generated.  In that case,
		 *  getError() will return the reason. */
		const char	*getString(uint16_t base);

		/** Sets *"value" to the value of this instance and
		 *  returns true.
		 *
		 *  Returns false, without modifying *"value", if
		 *  "value" is NULL or if this instance is outside of the
		 *  range of an int64_t.  In that case, getError() will
		 *  return the reason. */
		bool	getValue(int64_t *value);

		/** Sets *"value" to the value of this instance and
		 *  returns true.
		 *
		 *  Returns false, without modifying *"value", if
		 *  "value" is NULL, if this instance is negative, or if
		 *  this instance is outside of the range of a uint64_t.
		 *  In that case, getError() will return the reason. */
		bool	getValue(uint64_t *value);

		/** Returns the number of bytes required to represent the
		 *  magnitude of this instance, most significant byte
		 *  first (big-endian), with no leading zero bytes.
		 *
		 *  Returns 1 if this instance is 0. */
		size_t	getMagnitudeSize();

		/** Copies the magnitude of this instance into "buffer",
		 *  most significant byte first (big-endian), with no
		 *  leading zero bytes.  Exactly getMagnitudeSize() bytes
		 *  are written.  The sign of this instance is not
		 *  represented in "buffer" at all - see isNegative() and
		 *  getSign().
		 *
		 *  If this instance is 0, then a single 0 byte is
		 *  written.
		 *
		 *  Returns true on success, or false if "buffer" is NULL
		 *  or if "buffersize" is less than getMagnitudeSize().
		 *  In that case, getError() will return the reason. */
		bool	getMagnitude(byte_t *buffer, size_t buffersize);

		/** Returns the number of bits required to represent the
		 *  magnitude of this instance, ignoring its sign.
		 *
		 *  Returns 0 if this instance is 0. */
		size_t	getBitCount();

		/** Returns -1 if this instance is negative, 0 if this
		 *  instance is 0, and 1 if this instance is positive. */
		int32_t	getSign();

		/** Returns true if this instance is negative and false
		 *  otherwise.  Zero is not negative. */
		bool	isNegative();

		/** Returns true if this instance is 0 and false
		 *  otherwise. */
		bool	isZero();

		/** Adds "addend" to this instance, in place.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	add(const bignumber &addend);

		/** Subtracts "subtrahend" from this instance, in place.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	subtract(const bignumber &subtrahend);

		/** Multiplies this instance by "multiplier", in place.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	multiply(const bignumber &multiplier);

		/** Divides this instance by "divisor", in place, leaving
		 *  the quotient in this instance.
		 *
		 *  The division truncates toward zero.  For example,
		 *  -7/2 is -3, and 7/-2 is -3.
		 *
		 *  Returns true on success, or false if "divisor" is 0
		 *  or if an error occurred.  On failure, this instance
		 *  is left unmodified and getError() will return the
		 *  reason. */
		bool	divide(const bignumber &divisor);

		/** Divides this instance by "divisor", in place, leaving
		 *  the quotient in this instance and, if "remainder" is
		 *  not NULL, the remainder in *"remainder".
		 *
		 *  The division truncates toward zero and the remainder
		 *  takes the sign of the dividend, which is the original
		 *  value of this instance.  For example, dividing -7 by
		 *  2 gives a quotient of -3 and a remainder of -1, and
		 *  dividing 7 by -2 gives a quotient of -3 and a
		 *  remainder of 1.
		 *
		 *  Note that this is not the non-negative remainder that
		 *  modular arithmetic uses.  It is the same remainder
		 *  that the native C++ % operator gives.
		 *
		 *  Returns true on success, or false if "divisor" is 0
		 *  or if an error occurred.  On failure, this instance
		 *  and *"remainder" are left unmodified and getError()
		 *  will return the reason. */
		bool	divide(const bignumber &divisor,
					bignumber *remainder);

		/** Divides this instance by "divisor", in place, leaving
		 *  the remainder in this instance.
		 *
		 *  The remainder takes the sign of the dividend, which
		 *  is the original value of this instance.  For example,
		 *  -7%2 is -1, and 7%-2 is 1.
		 *
		 *  Returns true on success, or false if "divisor" is 0
		 *  or if an error occurred.  On failure, this instance
		 *  is left unmodified and getError() will return the
		 *  reason. */
		bool	modulo(const bignumber &divisor);

		/** Replaces this instance with this instance modulo
		 *  "modulus", in place, using the non-negative
		 *  remainder that modular arithmetic uses, rather than
		 *  the remainder that the native C++ % operator gives.
		 *
		 *  The result is always in the range 0 through the
		 *  magnitude of "modulus" minus 1, no matter what the
		 *  signs of this instance and "modulus" are.  For
		 *  example, -7 modulo 2 is 1, and not the -1 that
		 *  modulo() gives, and -7 modulo -2 is also 1.
		 *
		 *  A negative "modulus" is allowed.  Its magnitude is
		 *  used and its sign is ignored, so the result is the
		 *  same as it would be for the absolute value of
		 *  "modulus".
		 *
		 *  Returns true on success, or false if "modulus" is 0
		 *  or if an error occurred.  If "modulus" is 0, then
		 *  getError() will return
		 *  BIGNUMBER_ERROR_DIVIDE_BY_ZERO, matching modulo().
		 *  On failure, this instance is left unmodified and
		 *  getError() will return the reason. */
		bool	nonNegativeModulo(const bignumber &modulus);

		/** Replaces this instance with the sum of this instance
		 *  and "addend", modulo "modulus", in place.
		 *
		 *  The result is always non-negative, in the range 0
		 *  through "modulus" minus 1, no matter what the signs
		 *  of this instance and "addend" are, as
		 *  nonNegativeModulo() describes.
		 *
		 *  "modulus" must be positive.  Returns true on
		 *  success, or false if "modulus" is 0 or negative, or
		 *  if an error occurred.  If "modulus" is 0 or
		 *  negative, then getError() will return
		 *  BIGNUMBER_ERROR_INVALID_MODULUS.  On failure, this
		 *  instance is left unmodified and getError() will
		 *  return the reason. */
		bool	modAdd(const bignumber &addend,
					const bignumber &modulus);

		/** Replaces this instance with the difference of this
		 *  instance and "subtrahend", modulo "modulus", in
		 *  place.
		 *
		 *  The result is always non-negative, in the range 0
		 *  through "modulus" minus 1, no matter what the signs
		 *  of this instance and "subtrahend" are, as
		 *  nonNegativeModulo() describes.  In particular, a
		 *  difference that is mathematically negative wraps
		 *  around, rather than staying negative.  For example,
		 *  3 minus 5, modulo 7, is 5.
		 *
		 *  "modulus" must be positive.  Returns true on
		 *  success, or false if "modulus" is 0 or negative, or
		 *  if an error occurred.  If "modulus" is 0 or
		 *  negative, then getError() will return
		 *  BIGNUMBER_ERROR_INVALID_MODULUS.  On failure, this
		 *  instance is left unmodified and getError() will
		 *  return the reason. */
		bool	modSub(const bignumber &subtrahend,
					const bignumber &modulus);

		/** Replaces this instance with the product of this
		 *  instance and "multiplier", modulo "modulus", in
		 *  place.
		 *
		 *  The result is always non-negative, in the range 0
		 *  through "modulus" minus 1, no matter what the signs
		 *  of this instance and "multiplier" are, as
		 *  nonNegativeModulo() describes.
		 *
		 *  "modulus" must be positive.  Returns true on
		 *  success, or false if "modulus" is 0 or negative, or
		 *  if an error occurred.  If "modulus" is 0 or
		 *  negative, then getError() will return
		 *  BIGNUMBER_ERROR_INVALID_MODULUS.  On failure, this
		 *  instance is left unmodified and getError() will
		 *  return the reason. */
		bool	modMul(const bignumber &multiplier,
					const bignumber &modulus);

		/** Replaces this instance with this instance raised to
		 *  the power of "exponent", modulo "modulus", in place.
		 *
		 *  The result is always non-negative, in the range 0
		 *  through "modulus" minus 1.  A negative base is
		 *  well defined and also gives a non-negative result.
		 *  For example, -3 raised to the power of 3, modulo 7,
		 *  is 1, and not -6.
		 *
		 *  If "exponent" is 0, then the result is 1, except
		 *  that if "modulus" is 1 then the result is 0, since
		 *  every value is 0 modulo 1.
		 *
		 *  "exponent" must not be negative.  A negative
		 *  exponent would call for the modular inverse of the
		 *  base, which this method does not compute.  Use
		 *  modInverse() and then modPow() with the
		 *  corresponding positive exponent instead.
		 *
		 *  "modulus" must be positive.
		 *
		 *  Returns true on success, or false if "exponent" is
		 *  negative, if "modulus" is 0 or negative, or if an
		 *  error occurred.  If "exponent" is negative, then
		 *  getError() will return
		 *  BIGNUMBER_ERROR_NEGATIVE_EXPONENT.  If "modulus" is
		 *  0 or negative, then getError() will return
		 *  BIGNUMBER_ERROR_INVALID_MODULUS.  On failure, this
		 *  instance is left unmodified and getError() will
		 *  return the reason. */
		bool	modPow(const bignumber &exponent,
					const bignumber &modulus);

		/** Replaces this instance with its modular
		 *  multiplicative inverse, modulo "modulus", in place.
		 *  That is, this instance is replaced with the value x
		 *  for which this instance times x, modulo "modulus",
		 *  is 1.
		 *
		 *  The result is always non-negative, in the range 0
		 *  through "modulus" minus 1.  A negative value has an
		 *  inverse too, and it is non-negative as well.  For
		 *  example, the inverse of -3, modulo 7, is 2.
		 *
		 *  The inverse exists only if this instance and
		 *  "modulus" are relatively prime, that is, only if
		 *  their greatest common divisor is 1.  For example, 6
		 *  has no inverse modulo 9, since their greatest common
		 *  divisor is 3.  If the inverse does not exist, then
		 *  this method fails and getError() will return
		 *  BIGNUMBER_ERROR_NO_INVERSE.  This is a normal,
		 *  well defined outcome, rather than an internal error.
		 *
		 *  If "modulus" is 1, then the result is 0, by
		 *  convention, since every value is 0 modulo 1.
		 *
		 *  "modulus" must be positive.
		 *
		 *  Returns true on success, or false if the inverse
		 *  does not exist, if "modulus" is 0 or negative, or if
		 *  an error occurred.  If "modulus" is 0 or negative,
		 *  then getError() will return
		 *  BIGNUMBER_ERROR_INVALID_MODULUS.  On failure, this
		 *  instance is left unmodified and getError() will
		 *  return the reason. */
		bool	modInverse(const bignumber &modulus);

		/** Replaces this instance with the greatest common
		 *  divisor of the magnitudes of this instance and
		 *  "value", in place.
		 *
		 *  The signs of this instance and "value" are ignored
		 *  and the result is always non-negative.  For example,
		 *  the greatest common divisor of -12 and 18 is 6.
		 *
		 *  If both this instance and "value" are 0, then the
		 *  result is 0.  If exactly one of them is 0, then the
		 *  result is the magnitude of the other one.
		 *
		 *  Returns true on success, or false if an error
		 *  occurred.  On failure, this instance is left
		 *  unmodified and getError() will return the reason. */
		bool	gcd(const bignumber &value);

		/** Negates this instance, in place.  Negating 0 leaves
		 *  it 0.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	negate();

		/** Replaces this instance with its absolute value, in
		 *  place.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	absoluteValue();

		/** Returns -1 if this instance is less than "value", 0
		 *  if this instance is equal to "value", and 1 if this
		 *  instance is greater than "value". */
		int32_t	compare(const bignumber &value);

		/** Shifts the magnitude of this instance left by "bits"
		 *  bits, in place, preserving its sign.  This
		 *  multiplies this instance by 2 to the power of "bits".
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	leftShift(uint64_t bits);

		/** Shifts the magnitude of this instance right by "bits"
		 *  bits, in place, preserving its sign.  Bits shifted
		 *  off of the right are discarded.
		 *
		 *  Since the magnitude is shifted, rather than a two's
		 *  complement bit pattern, this truncates toward zero
		 *  rather than toward negative infinity.  For example,
		 *  shifting -7 right by 1 bit gives -3, and not the -4
		 *  that a native, two's complement, arithmetic right
		 *  shift would give.  Shifting a negative value right
		 *  far enough eventually gives 0, rather than -1.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	rightShift(uint64_t bits);

		/** Replaces this instance with the bitwise AND of this
		 *  instance and "value", in place.
		 *
		 *  Both operands are treated as if they were infinitely
		 *  wide two's complement values, sign extended to the
		 *  same width, so a negative operand behaves as though
		 *  it had an infinite run of 1 bits above its most
		 *  significant bit.  For example, -2&3 is 2.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	bitwiseAnd(const bignumber &value);

		/** Replaces this instance with the bitwise OR of this
		 *  instance and "value", in place.
		 *
		 *  Both operands are treated as if they were infinitely
		 *  wide two's complement values, sign extended to the
		 *  same width, as described in bitwiseAnd().  For
		 *  example, -2|3 is -1.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	bitwiseOr(const bignumber &value);

		/** Replaces this instance with the bitwise exclusive OR
		 *  of this instance and "value", in place.
		 *
		 *  Both operands are treated as if they were infinitely
		 *  wide two's complement values, sign extended to the
		 *  same width, as described in bitwiseAnd().  For
		 *  example, -2^3 is -3.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	bitwiseXor(const bignumber &value);

		/** Replaces this instance with its bitwise complement,
		 *  in place.
		 *
		 *  This instance is treated as if it were an infinitely
		 *  wide two's complement value, so the complement of x
		 *  is -x-1.  For example, the complement of 0 is -1, and
		 *  the complement of 5 is -6.
		 *
		 *  Returns true on success or false if an error
		 *  occurred. */
		bool	bitwiseNot();

		/** Returns the sum of this instance and "value". */
		bignumber	operator+(const bignumber &value);

		/** Returns the difference of this instance and
		 *  "value". */
		bignumber	operator-(const bignumber &value);

		/** Returns the product of this instance and "value". */
		bignumber	operator*(const bignumber &value);

		/** Returns the quotient of this instance divided by
		 *  "value", truncated toward zero, as divide()
		 *  describes.
		 *
		 *  If "value" is 0, then 0 is returned and getError()
		 *  will return BIGNUMBER_ERROR_DIVIDE_BY_ZERO. */
		bignumber	operator/(const bignumber &value);

		/** Returns the remainder of this instance divided by
		 *  "value", with the sign of this instance, as modulo()
		 *  describes.
		 *
		 *  If "value" is 0, then 0 is returned and getError()
		 *  will return BIGNUMBER_ERROR_DIVIDE_BY_ZERO. */
		bignumber	operator%(const bignumber &value);

		/** Adds "value" to this instance, as add() does, and
		 *  returns a reference to this instance. */
		bignumber	&operator+=(const bignumber &value);

		/** Subtracts "value" from this instance, as subtract()
		 *  does, and returns a reference to this instance. */
		bignumber	&operator-=(const bignumber &value);

		/** Multiplies this instance by "value", as multiply()
		 *  does, and returns a reference to this instance. */
		bignumber	&operator*=(const bignumber &value);

		/** Divides this instance by "value", as divide() does,
		 *  and returns a reference to this instance.
		 *
		 *  If "value" is 0, then this instance is left
		 *  unmodified and getError() will return
		 *  BIGNUMBER_ERROR_DIVIDE_BY_ZERO. */
		bignumber	&operator/=(const bignumber &value);

		/** Sets this instance to the remainder of this instance
		 *  divided by "value", as modulo() does, and returns a
		 *  reference to this instance.
		 *
		 *  If "value" is 0, then this instance is left
		 *  unmodified and getError() will return
		 *  BIGNUMBER_ERROR_DIVIDE_BY_ZERO. */
		bignumber	&operator%=(const bignumber &value);

		/** Returns the negation of this instance.  This instance
		 *  itself is not modified. */
		bignumber	operator-();

		/** Adds 1 to this instance and returns a reference to
		 *  this instance. */
		bignumber	&operator++();

		/** Adds 1 to this instance and returns the value that
		 *  this instance had before the increment. */
		bignumber	operator++(int);

		/** Subtracts 1 from this instance and returns a
		 *  reference to this instance. */
		bignumber	&operator--();

		/** Subtracts 1 from this instance and returns the value
		 *  that this instance had before the decrement. */
		bignumber	operator--(int);

		/** Returns true if this instance is equal to "value" and
		 *  false otherwise. */
		bool	operator==(const bignumber &value);

		/** Returns true if this instance is not equal to "value"
		 *  and false otherwise. */
		bool	operator!=(const bignumber &value);

		/** Returns true if this instance is less than "value"
		 *  and false otherwise. */
		bool	operator<(const bignumber &value);

		/** Returns true if this instance is greater than "value"
		 *  and false otherwise. */
		bool	operator>(const bignumber &value);

		/** Returns true if this instance is less than or equal
		 *  to "value" and false otherwise. */
		bool	operator<=(const bignumber &value);

		/** Returns true if this instance is greater than or
		 *  equal to "value" and false otherwise. */
		bool	operator>=(const bignumber &value);

		/** Returns the bitwise AND of this instance and "value",
		 *  as bitwiseAnd() describes. */
		bignumber	operator&(const bignumber &value);

		/** Returns the bitwise OR of this instance and "value",
		 *  as bitwiseOr() describes. */
		bignumber	operator|(const bignumber &value);

		/** Returns the bitwise exclusive OR of this instance and
		 *  "value", as bitwiseXor() describes. */
		bignumber	operator^(const bignumber &value);

		/** Returns the bitwise complement of this instance, as
		 *  bitwiseNot() describes.  This instance itself is not
		 *  modified. */
		bignumber	operator~();

		/** Sets this instance to the bitwise AND of this
		 *  instance and "value", as bitwiseAnd() does, and
		 *  returns a reference to this instance. */
		bignumber	&operator&=(const bignumber &value);

		/** Sets this instance to the bitwise OR of this instance
		 *  and "value", as bitwiseOr() does, and returns a
		 *  reference to this instance. */
		bignumber	&operator|=(const bignumber &value);

		/** Sets this instance to the bitwise exclusive OR of
		 *  this instance and "value", as bitwiseXor() does, and
		 *  returns a reference to this instance. */
		bignumber	&operator^=(const bignumber &value);

		/** Returns this instance, with its magnitude shifted
		 *  left by "bits" bits and its sign preserved, as
		 *  leftShift() describes.  This instance itself is not
		 *  modified. */
		bignumber	operator<<(uint64_t bits);

		/** Returns this instance, with its magnitude shifted
		 *  right by "bits" bits and its sign preserved, as
		 *  rightShift() describes.  This instance itself is not
		 *  modified.
		 *
		 *  Note that this truncates toward zero, rather than
		 *  toward negative infinity, so -7>>1 is -3, and not the
		 *  -4 that a native, two's complement, arithmetic right
		 *  shift would give.  This is a deliberate departure
		 *  from the behavior of the native >> operator, and
		 *  matches the behavior of OpenSSL's BN_rshift(), which
		 *  shifts the magnitude and preserves the sign. */
		bignumber	operator>>(uint64_t bits);

		/** Shifts this instance left by "bits" bits, as
		 *  leftShift() does, and returns a reference to this
		 *  instance. */
		bignumber	&operator<<=(uint64_t bits);

		/** Shifts this instance right by "bits" bits, as
		 *  rightShift() does, and returns a reference to this
		 *  instance. */
		bignumber	&operator>>=(uint64_t bits);

		/** Returns the error from the most recent operation on
		 *  this instance that can fail.  Operations that can
		 *  fail set this to BIGNUMBER_ERROR_SUCCESS when they
		 *  succeed.  Operations that cannot fail leave it
		 *  alone. */
		bignumbererror_t	getError();

		/** Returns true if this platform supports bignumber and
		 *  false otherwise.  Since a fallback implementation is
		 *  built in, this method always returns true. */
		bool	isSupported();

	#include <rudiments/private/bignumber.h>
};

#endif
