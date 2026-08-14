// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/dynamicarray.h>

// bignumberscratch is a signed magnitude arbitrary-precision integer.  The
// functions below mirror the subset of the OpenSSL BIGNUM API that the
// bignumber class uses.
//
// Unless noted otherwise, a "result" may be the same instance as an input, and
// each function returns false if it ran out of memory.
//
// The magnitude is stored in base 256, least significant byte first, so index
// 0 is the least significant byte.  The array only ever grows, so the logical
// length is tracked separately, rather than relying on the array's own count.
//
// Zero is stored canonically - a length of 0, with no stored zero byte, and a
// non-negative sign.  There is no negative zero.  Every function below leaves
// its result in that form.
class bignumberscratch {
	public:
		bignumberscratch() {
			_len=0;
			_neg=false;
		}

		dynamicarray<byte_t>	_mag;
		size_t			_len;
		bool			_neg;
};

// strips leading zero bytes and restores the canonical form of zero
static void bignumberscratchnormalize(bignumberscratch *n) {
	while (n->_len && !n->_mag[n->_len-1]) {
		n->_len--;
	}
	if (!n->_len) {
		n->_neg=false;
	}
}

// returns the value of the hex digit "c", or -1 if it isn't a hex digit
static int16_t bignumberscratchhexvalue(char c) {
	if (c>='0' && c<='9') {
		return (int16_t)(c-'0');
	}
	if (c>='a' && c<='f') {
		return (int16_t)(c-'a'+10);
	}
	if (c>='A' && c<='F') {
		return (int16_t)(c-'A'+10);
	}
	return -1;
}

// returns the upper case hex digit for the low nibble of "value"
static char bignumberscratchhexchar(byte_t value) {
	byte_t	nibble=(byte_t)(value&0x0f);
	return (char)((nibble<10)?('0'+nibble):('A'+nibble-10));
}

// lifetime and assignment...

static bignumberscratch *bignumberscratchnew() {
	return new bignumberscratch;
}

static void bignumberscratchfree(bignumberscratch *n) {
	delete n;
}

static bool bignumberscratchcopy(bignumberscratch *dest,
					bignumberscratch *src) {
	if (dest!=src) {
		dest->_mag=src->_mag;
		dest->_len=src->_len;
		dest->_neg=src->_neg;
	}
	return true;
}

static void bignumberscratchzero(bignumberscratch *n) {
	// the storage is left alone, as a later value can just reuse it
	n->_len=0;
	n->_neg=false;
}

// input...

// multiplies the magnitude of "n" by 10 and adds "digit", in a single pass
// with a carry.  This is the one piece of arbitrary-precision arithmetic that
// converting a base 10 string needs, and it's a much simpler special case than
// the general multiply and add, so it's kept local here.
static void bignumberscratchmultiplytenadd(bignumberscratch *n, byte_t digit) {

	// the largest product of a byte is 255*10+9, which fits in 16 bits
	uint16_t	carry=(uint16_t)digit;
	for (size_t i=0; i<n->_len; i++) {
		uint16_t	product=(uint16_t)
					(((uint16_t)n->_mag[i])*10+carry);
		n->_mag[i]=(byte_t)(product&0xff);
		carry=(uint16_t)(product>>8);
	}
	while (carry) {
		n->_mag[n->_len]=(byte_t)(carry&0xff);
		n->_len++;
		carry=(uint16_t)(carry>>8);
	}
}

// "base" is 10 or 16, "value" is not NULL, and any leading + has already been
// skipped.  A leading - is allowed.  Trailing characters are allowed here, as
// BN_dec2bn() and BN_hex2bn() allow them, and *chars is set to the number of
// characters consumed so that the caller can reject them.  Returns false if
// "value" is malformed.
static bool bignumberscratchsetstring(bignumberscratch *n,
					const char *value,
					uint16_t base,
					size_t *chars) {

	// handle a leading -
	const char	*v=value;
	bool		negative=false;
	if (*v=='-') {
		negative=true;
		v++;
	}

	// find the end of the run of digits
	const char	*end=v;
	if (base==16) {
		while (bignumberscratchhexvalue(*end)!=-1) {
			end++;
		}
	} else {
		while (*end>='0' && *end<='9') {
			end++;
		}
	}
	size_t	digits=(size_t)(end-v);
	if (!digits) {
		return false;
	}

	// convert
	n->_len=0;
	n->_neg=false;
	if (base==16) {

		// BN_hex2bn() right-aligns the digits into bytes, so an odd
		// number of digits leaves the first one as the high nibble of
		// the most significant byte, and "F" is 15 rather than 240
		size_t	len=(digits+1)/2;
		for (size_t i=0; i<len; i++) {
			const char	*lo=end-1-i*2;
			byte_t		b=(byte_t)
					bignumberscratchhexvalue(*lo);
			const char	*hi=lo-1;
			if (hi>=v) {
				b=(byte_t)(b|(((byte_t)
					bignumberscratchhexvalue(*hi))<<4));
			}
			n->_mag[i]=b;
		}
		n->_len=len;

	} else {

		for (const char *p=v; p<end; p++) {
			bignumberscratchmultiplytenadd(n,(byte_t)(*p-'0'));
		}
	}

	// leading zeros in the string leave leading zero bytes behind
	bignumberscratchnormalize(n);

	// zero is never negative
	if (negative && n->_len) {
		n->_neg=true;
	}

	*chars=((negative)?1:0)+digits;

	return true;
}

// "magnitude" is not NULL, "size" is not 0, and the value is set positive
static bool bignumberscratchsetbytes(bignumberscratch *n,
					const byte_t *magnitude,
					size_t size) {

	// skip any leading zero bytes
	size_t	first=0;
	while (first<size && !magnitude[first]) {
		first++;
	}

	// copy in, reversing the byte order
	size_t	len=size-first;
	for (size_t i=0; i<len; i++) {
		n->_mag[i]=magnitude[size-1-i];
	}
	n->_len=len;
	n->_neg=false;

	return true;
}

// output...

// getstring() returns a new[] allocated string that the caller delete[]s, or
// NULL if it ran out of memory.  Base 10 gives the format that BN_bn2dec()
// gives, and base 16 the format that BN_bn2hex() gives - upper case, and two
// hex digits per byte, so a leading zero digit is possible.  Both give "0" for
// zero and a leading - for a negative value.
static char *bignumberscratchgetstring(bignumberscratch *n, uint16_t base) {

	if (!n->_len) {
		char	*str=new char[2];
		str[0]='0';
		str[1]='\0';
		return str;
	}

	size_t	sign=(n->_neg)?1:0;

	if (base==16) {

		// 2 digits per byte, most significant byte first
		char	*str=new char[sign+n->_len*2+1];
		char	*p=str;
		if (n->_neg) {
			*(p++)='-';
		}
		for (size_t i=n->_len; i>0; i--) {
			byte_t	b=n->_mag[i-1];
			*(p++)=bignumberscratchhexchar((byte_t)(b>>4));
			*(p++)=bignumberscratchhexchar(b);
		}
		*p='\0';
		return str;
	}

	// Base 10 needs the magnitude divided by 10 over and over, taking each
	// remainder as a decimal digit, least significant digit first.  That's
	// a single pass with a carry, rather than the general divide, so it's
	// kept local here.  The division is destructive, so it runs against a
	// copy of the magnitude.
	byte_t	*work=new byte_t[n->_len];
	for (size_t i=0; i<n->_len; i++) {
		work[i]=n->_mag[i];
	}

	// each byte contributes fewer than 3 decimal digits
	char	*digits=new char[n->_len*3+1];
	size_t	digitcount=0;
	size_t	worklen=n->_len;
	while (worklen) {
		uint16_t	remainder=0;
		for (size_t i=worklen; i>0; i--) {
			uint16_t	current=(uint16_t)
					((remainder<<8)|work[i-1]);
			work[i-1]=(byte_t)(current/10);
			remainder=(uint16_t)(current%10);
		}
		digits[digitcount]=(char)('0'+remainder);
		digitcount++;
		while (worklen && !work[worklen-1]) {
			worklen--;
		}
	}

	// reverse the digits into the result
	char	*str=new char[sign+digitcount+1];
	char	*p=str;
	if (n->_neg) {
		*(p++)='-';
	}
	for (size_t i=digitcount; i>0; i--) {
		*(p++)=digits[i-1];
	}
	*p='\0';

	delete[] digits;
	delete[] work;

	return str;
}

// getbytecount() returns 0 for zero, as BN_num_bytes() does, and getbytes()
// writes that many bytes, most significant byte first
static size_t bignumberscratchgetbytecount(bignumberscratch *n) {
	return n->_len;
}

static void bignumberscratchgetbytes(bignumberscratch *n, byte_t *buffer) {
	for (size_t i=0; i<n->_len; i++) {
		buffer[n->_len-1-i]=n->_mag[i];
	}
}

// returns 0 for zero, as BN_num_bits() does
static size_t bignumberscratchgetbitcount(bignumberscratch *n) {

	if (!n->_len) {
		return 0;
	}

	// the bits below the most significant byte, plus the position of the
	// highest set bit of that byte, plus 1
	size_t	bits=(n->_len-1)*8;
	byte_t	top=n->_mag[n->_len-1];
	while (top) {
		bits++;
		top=(byte_t)(top>>1);
	}
	return bits;
}

// sign

static bool bignumberscratchiszero(bignumberscratch *n) {
	return (n->_len==0);
}

static bool bignumberscratchisnegative(bignumberscratch *n) {
	return n->_neg;
}

// zero is never negative
static void bignumberscratchsetnegative(bignumberscratch *n, bool negative) {
	n->_neg=(negative && n->_len);
}

// arithmetic

// defined below, with the rest of the comparisons
static int32_t bignumberscratchcomparemagnitude(bignumberscratch *a,
						bignumberscratch *b);

// Adds "a" and "b", using "aneg" and "bneg" as their signs rather than the
// signs they carry, so that subtract can run the same code with the sign of
// "b" flipped without having to modify "b" itself.  "result" may be the same
// instance as "a" or "b", so each byte of each input is read before the byte
// at that same index is written, and the lengths and signs are read up front.
static bool bignumberscratchaddsigned(bignumberscratch *result,
					bignumberscratch *a,
					bool aneg,
					bignumberscratch *b,
					bool bneg) {

	size_t	alen=a->_len;
	size_t	blen=b->_len;

	// the same sign - add the magnitudes and keep the sign
	if (aneg==bneg) {

		size_t		len=(alen>blen)?alen:blen;
		uint16_t	carry=0;
		for (size_t i=0; i<len; i++) {
			uint16_t	sum=carry;
			if (i<alen) {
				sum=(uint16_t)(sum+a->_mag[i]);
			}
			if (i<blen) {
				sum=(uint16_t)(sum+b->_mag[i]);
			}
			result->_mag[i]=(byte_t)(sum&0xff);
			carry=(uint16_t)(sum>>8);
		}
		if (carry) {
			result->_mag[len]=(byte_t)carry;
			len++;
		}

		result->_len=len;
		result->_neg=aneg;
		bignumberscratchnormalize(result);

		return true;
	}

	// different signs - subtract the smaller magnitude from the larger and
	// take the sign of the larger, which leaves zero when they're equal
	int32_t	cmp=bignumberscratchcomparemagnitude(a,b);
	if (!cmp) {
		bignumberscratchzero(result);
		return true;
	}

	bignumberscratch	*larger=(cmp>0)?a:b;
	bignumberscratch	*smaller=(cmp>0)?b:a;
	bool			neg=(cmp>0)?aneg:bneg;
	size_t			largerlen=larger->_len;
	size_t			smallerlen=smaller->_len;

	int16_t	borrow=0;
	for (size_t i=0; i<largerlen; i++) {
		int16_t	difference=(int16_t)(larger->_mag[i]-borrow);
		if (i<smallerlen) {
			difference=(int16_t)(difference-smaller->_mag[i]);
		}
		if (difference<0) {
			difference=(int16_t)(difference+256);
			borrow=1;
		} else {
			borrow=0;
		}
		result->_mag[i]=(byte_t)difference;
	}

	result->_len=largerlen;
	result->_neg=neg;
	bignumberscratchnormalize(result);

	return true;
}

static bool bignumberscratchadd(bignumberscratch *result,
					bignumberscratch *a,
					bignumberscratch *b) {
	return bignumberscratchaddsigned(result,a,a->_neg,b,b->_neg);
}

// a-b is a+(-b), and the flipped sign is passed along rather than written to
// "b", which the caller still owns
static bool bignumberscratchsubtract(bignumberscratch *result,
					bignumberscratch *a,
					bignumberscratch *b) {
	return bignumberscratchaddsigned(result,a,a->_neg,b,!b->_neg);
}

static bool bignumberscratchmultiply(bignumberscratch *result,
					bignumberscratch *a,
					bignumberscratch *b) {

	size_t	alen=a->_len;
	size_t	blen=b->_len;
	bool	neg=(a->_neg!=b->_neg);

	// zero times anything is zero, and never negative
	if (!alen || !blen) {
		bignumberscratchzero(result);
		return true;
	}

	// "result" may be the same instance as "a" or "b", so the product is
	// built up somewhere else entirely and only copied in at the end.  It
	// can't be longer than the two lengths added together.
	size_t	len=alen+blen;
	byte_t	*product=new byte_t[len];
	for (size_t i=0; i<len; i++) {
		product[i]=0;
	}

	// schoolbook long multiplication - each byte of "a" against all of "b",
	// accumulated into the product at the combined position.  The largest
	// value the accumulator has to hold is 255*255+255+255, which fits in
	// 32 bits with room to spare.
	for (size_t i=0; i<alen; i++) {
		uint32_t	carry=0;
		for (size_t j=0; j<blen; j++) {
			uint32_t	sum=(uint32_t)
				(((uint32_t)a->_mag[i])*((uint32_t)b->_mag[j])+
					(uint32_t)product[i+j]+carry);
			product[i+j]=(byte_t)(sum&0xff);
			carry=(uint32_t)(sum>>8);
		}
		// nothing has been written that high yet, so the carry lands
		// there rather than accumulating into it
		product[i+blen]=(byte_t)carry;
	}

	for (size_t i=0; i<len; i++) {
		result->_mag[i]=product[i];
	}
	result->_len=len;
	result->_neg=neg;
	bignumberscratchnormalize(result);

	delete[] product;

	return true;
}

// sets "n" to the unsigned value "value"
static void bignumberscratchsetword(bignumberscratch *n, uint32_t value) {
	n->_len=0;
	n->_neg=false;
	while (value) {
		n->_mag[n->_len]=(byte_t)(value&0xff);
		n->_len++;
		value=(uint32_t)(value>>8);
	}
}

static bool bignumberscratchaddword(bignumberscratch *n, uint32_t value) {
	bignumberscratch	word;
	bignumberscratchsetword(&word,value);
	return bignumberscratchadd(n,n,&word);
}

static bool bignumberscratchsubtractword(bignumberscratch *n, uint32_t value) {
	bignumberscratch	word;
	bignumberscratchsetword(&word,value);
	return bignumberscratchsubtract(n,n,&word);
}

// compares two magnitudes held in plain byte arrays, each already stripped of
// its leading zero bytes, and returns -1, 0, or 1
static int32_t bignumberscratchcomparebytes(const byte_t *a, size_t alen,
						const byte_t *b, size_t blen) {

	if (alen!=blen) {
		return (alen<blen)?-1:1;
	}
	for (size_t i=alen; i>0; i--) {
		if (a[i-1]!=b[i-1]) {
			return (a[i-1]<b[i-1])?-1:1;
		}
	}
	return 0;
}

// divide()...
//
// "quotient" and "remainder" may each be NULL, but neither may be the same
// instance as the other, as "a", or as "b".  "b" is not zero.  The quotient
// truncates toward zero and the remainder takes the sign of "a", as BN_div()
// gives.
static bool bignumberscratchdivide(bignumberscratch *quotient,
					bignumberscratch *remainder,
					bignumberscratch *a,
					bignumberscratch *b) {
	size_t	alen=a->_len;
	size_t	blen=b->_len;

	// the quotient takes the signs of both, and the remainder the sign of
	// the dividend, which is what makes the quotient truncate toward zero
	bool	qneg=(a->_neg!=b->_neg);
	bool	rneg=a->_neg;

	// zero divided by anything is zero, with nothing left over
	if (!alen) {
		if (quotient) {
			bignumberscratchzero(quotient);
		}
		if (remainder) {
			bignumberscratchzero(remainder);
		}
		return true;
	}

	// the divisor's magnitude, in a plain byte array, so that the running
	// remainder can be compared against it and subtracted from directly
	byte_t	*divisor=new byte_t[blen];
	for (size_t i=0; i<blen; i++) {
		divisor[i]=b->_mag[i];
	}

	// The running remainder is always smaller than the divisor at the top
	// of each step, so bringing one more byte down can't make it longer
	// than the divisor by more than a byte.
	byte_t	*rem=new byte_t[blen+1];
	size_t	remlen=0;

	// the quotient bytes, most significant first, one per dividend byte
	byte_t	*quo=new byte_t[alen];

	// schoolbook long division, most significant byte of the dividend first
	for (size_t i=alen; i>0; i--) {

		// shift the running remainder up a byte and bring the next
		// byte of the dividend down into the gap
		for (size_t k=remlen; k>0; k--) {
			rem[k]=rem[k-1];
		}
		rem[0]=a->_mag[i-1];
		remlen++;
		while (remlen && !rem[remlen-1]) {
			remlen--;
		}

		// The running remainder is smaller than 256 times the divisor,
		// so the quotient byte is at most 255 and repeated subtraction
		// finds it.  That's slower than estimating it, but it can't be
		// off by one.
		byte_t	digit=0;
		while (bignumberscratchcomparebytes(rem,remlen,
							divisor,blen)>=0) {
			int16_t	borrow=0;
			for (size_t k=0; k<remlen; k++) {
				int16_t	difference=(int16_t)(rem[k]-borrow);
				if (k<blen) {
					difference=(int16_t)
							(difference-divisor[k]);
				}
				if (difference<0) {
					difference=(int16_t)(difference+256);
					borrow=1;
				} else {
					borrow=0;
				}
				rem[k]=(byte_t)difference;
			}
			while (remlen && !rem[remlen-1]) {
				remlen--;
			}
			digit++;
		}
		quo[alen-i]=digit;
	}

	if (quotient) {
		for (size_t i=0; i<alen; i++) {
			quotient->_mag[i]=quo[alen-1-i];
		}
		quotient->_len=alen;
		quotient->_neg=qneg;
		bignumberscratchnormalize(quotient);
	}

	if (remainder) {
		for (size_t i=0; i<remlen; i++) {
			remainder->_mag[i]=rem[i];
		}
		remainder->_len=remlen;
		remainder->_neg=rneg;
		bignumberscratchnormalize(remainder);
	}

	delete[] quo;
	delete[] rem;
	delete[] divisor;

	return true;
}

// compares the magnitudes only, ignoring both signs
static int32_t bignumberscratchcomparemagnitude(bignumberscratch *a,
						bignumberscratch *b) {

	if (a->_len!=b->_len) {
		return (a->_len<b->_len)?-1:1;
	}
	for (size_t i=a->_len; i>0; i--) {
		if (a->_mag[i-1]!=b->_mag[i-1]) {
			return (a->_mag[i-1]<b->_mag[i-1])?-1:1;
		}
	}
	return 0;
}

// returns -1, 0, or 1, as BN_cmp() does
static int32_t bignumberscratchcompare(bignumberscratch *a,
					bignumberscratch *b) {

	// zeros compare equal, whatever sign they somehow ended up with
	if (!a->_len && !b->_len) {
		return 0;
	}

	bool	anegative=(a->_neg && a->_len);
	bool	bnegative=(b->_neg && b->_len);
	if (anegative!=bnegative) {
		return (anegative)?-1:1;
	}

	// for two negative values, the larger magnitude is the smaller value
	int32_t	result=bignumberscratchcomparemagnitude(a,b);
	return (anegative)?-result:result;
}

// the magnitude is shifted and the sign is preserved, as BN_lshift() and
// BN_rshift() do
static bool bignumberscratchleftshift(bignumberscratch *result,
					bignumberscratch *a,
					uint64_t bits) {

	size_t	bytes=(size_t)(bits/8);
	uint16_t	offset=(uint16_t)(bits%8);

	// the counts are read before anything is written, as "result" may be
	// the same instance as "a"
	size_t	alen=a->_len;
	bool	aneg=a->_neg;

	if (!alen) {
		bignumberscratchzero(result);
		return true;
	}

	// 1 extra byte for whatever the sub-byte shift carries out of the top
	size_t	len=alen+bytes+1;

	// The bytes are written from the top down, so that each one is written
	// only after the lower bytes it was built from have been read, which
	// is what makes shifting in place safe.
	for (size_t i=len; i>0; i--) {
		size_t	index=i-1;
		byte_t	current=0;
		byte_t	previous=0;
		if (index>=bytes && index-bytes<alen) {
			current=a->_mag[index-bytes];
		}
		if (index>=bytes+1 && index-bytes-1<alen) {
			previous=a->_mag[index-bytes-1];
		}
		result->_mag[index]=(offset)?
			(byte_t)((current<<offset)|(previous>>(8-offset))):
			current;
	}

	result->_len=len;
	result->_neg=aneg;
	bignumberscratchnormalize(result);

	return true;
}

static bool bignumberscratchrightshift(bignumberscratch *result,
					bignumberscratch *a,
					uint64_t bits) {

	size_t	bytes=(size_t)(bits/8);
	uint16_t	offset=(uint16_t)(bits%8);

	size_t	alen=a->_len;
	bool	aneg=a->_neg;

	// the whole magnitude shifted off, and the sign goes with it
	if (bytes>=alen) {
		bignumberscratchzero(result);
		return true;
	}

	// The bytes are written from the bottom up, so that each one is
	// written only after the higher bytes it was built from have been
	// read, which is what makes shifting in place safe.
	size_t	len=alen-bytes;
	for (size_t index=0; index<len; index++) {
		byte_t	current=a->_mag[index+bytes];
		byte_t	next=(index+bytes+1<alen)?a->_mag[index+bytes+1]:0;
		result->_mag[index]=(offset)?
			(byte_t)((current>>offset)|(next<<(8-offset))):
			current;
	}

	result->_len=len;
	result->_neg=aneg;
	bignumberscratchnormalize(result);

	return true;
}
