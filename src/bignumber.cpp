// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/bignumber.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>

// Without the BIGNUM functions in libcrypto, the built-in implementation is
// used instead.
#if !defined(RUDIMENTS_HAS_BN)
	#undef RUDIMENTS_HAS_SSL
#endif

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/bn.h>
	#include <openssl/crypto.h>
	#include <openssl/err.h>
#else
	#include "bignumberscratch.cpp"
#endif

class bignumberprivate {
	friend class bignumber;
	private:
		void	init();
		void	zero();
		void	setNegative(bool negative);
		bool	isZero();
		bool	isNegative();
		#if defined(RUDIMENTS_HAS_SSL)
			BN_CTX	*getContext();
		#endif

		bignumbererror_t	_err;
		char			*_str;
		#if defined(RUDIMENTS_HAS_SSL)
			BIGNUM			*_bn;
			BN_CTX			*_ctx;
		#else
			bignumberscratch	*_bn;
		#endif
};

void bignumberprivate::init() {
	_err=BIGNUMBER_ERROR_SUCCESS;
	_str=NULL;
	#if defined(RUDIMENTS_HAS_SSL)
		_bn=BN_new();
		_ctx=NULL;
	#else
		_bn=bignumberscratchnew();
	#endif
}

void bignumberprivate::zero() {
	#if defined(RUDIMENTS_HAS_SSL)
		BN_zero(_bn);
	#else
		bignumberscratchzero(_bn);
	#endif
}

// zero is never negative
void bignumberprivate::setNegative(bool negative) {
	#if defined(RUDIMENTS_HAS_SSL)
		BN_set_negative(_bn,(negative)?1:0);
	#else
		bignumberscratchsetnegative(_bn,negative);
	#endif
}

bool bignumberprivate::isZero() {
	#if defined(RUDIMENTS_HAS_SSL)
		return (BN_is_zero(_bn))?true:false;
	#else
		return bignumberscratchiszero(_bn);
	#endif
}

bool bignumberprivate::isNegative() {
	#if defined(RUDIMENTS_HAS_SSL)
		return (BN_is_negative(_bn))?true:false;
	#else
		return bignumberscratchisnegative(_bn);
	#endif
}

#if defined(RUDIMENTS_HAS_SSL)
// BN_mul() and BN_div() need a context.  One is created on first use and kept
// for the life of the instance.
BN_CTX *bignumberprivate::getContext() {
	if (!_ctx) {
		_ctx=BN_CTX_new();
	}
	return _ctx;
}

// clears the OpenSSL error queue, so a failure here doesn't confuse the next
// libcrypto call to look at it
static void bignumberclearsslerror() {
	while (ERR_get_error()) {}
}
#endif

// packs "value" into the 8 bytes of "buffer", most significant byte first
static void bignumberpack(uint64_t value, byte_t *buffer) {
	for (uint16_t i=0; i<8; i++) {
		buffer[7-i]=(byte_t)(value&0xff);
		value=value>>8;
	}
}

// unpacks the 8 bytes of "buffer", most significant byte first
static uint64_t bignumberunpack(const byte_t *buffer) {
	uint64_t	value=0;
	for (uint16_t i=0; i<8; i++) {
		value=(value<<8)|((uint64_t)buffer[i]);
	}
	return value;
}

bignumber::bignumber() {
	pvt=new bignumberprivate;
	pvt->init();
}

bignumber::bignumber(const char *value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(value,10);
}

bignumber::bignumber(const char *value, uint16_t base) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(value,base);
}

bignumber::bignumber(int32_t value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue((int64_t)value);
}

bignumber::bignumber(uint32_t value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue((uint64_t)value);
}

bignumber::bignumber(int64_t value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(value);
}

bignumber::bignumber(uint64_t value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(value);
}

bignumber::bignumber(const byte_t *magnitude, size_t size) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(magnitude,size,false);
}

bignumber::bignumber(const byte_t *magnitude, size_t size, bool negative) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(magnitude,size,negative);
}

bignumber::bignumber(const bignumber &value) {
	pvt=new bignumberprivate;
	pvt->init();
	setValue(value);
}

bignumber::~bignumber() {
	delete[] pvt->_str;
	#if defined(RUDIMENTS_HAS_SSL)
		BN_free(pvt->_bn);
		BN_CTX_free(pvt->_ctx);
	#else
		bignumberscratchfree(pvt->_bn);
	#endif
	delete pvt;
}

bool bignumber::setValue(const char *value) {
	return setValue(value,10);
}

bool bignumber::setValue(const char *value, uint16_t base) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (base!=10 && base!=16) {
		pvt->zero();
		setError(BIGNUMBER_ERROR_INVALID_BASE);
		return false;
	}

	if (!value) {
		pvt->zero();
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	// A leading - is handled by the conversion itself, but a leading + is
	// not, so it's skipped here.  A sign after that is malformed.
	const char	*v=value;
	bool		malformed=false;
	if (*v=='+') {
		v++;
		malformed=(*v=='+' || *v=='-');
	}
	if (malformed || !*v) {
		pvt->zero();
		setError(BIGNUMBER_ERROR_INVALID_FORMAT);
		return false;
	}

	// convert
	size_t	chars=0;
	#if defined(RUDIMENTS_HAS_SSL)
		int	result=(base==10)?BN_dec2bn(&pvt->_bn,v):
					BN_hex2bn(&pvt->_bn,v);
		if (result<=0) {
			pvt->zero();
			setError(BIGNUMBER_ERROR_INVALID_FORMAT);
			bignumberclearsslerror();
			return false;
		}
		chars=(size_t)result;
	#else
		if (!bignumberscratchsetstring(pvt->_bn,v,base,&chars)) {
			pvt->zero();
			setError(BIGNUMBER_ERROR_INVALID_FORMAT);
			return false;
		}
	#endif

	// the conversion stops at the first character that it can't use, but
	// the entire string is required to be part of the number
	if (v[chars]) {
		pvt->zero();
		setError(BIGNUMBER_ERROR_INVALID_FORMAT);
		return false;
	}

	return true;
}

void bignumber::setValue(int32_t value) {
	setValue((int64_t)value);
}

void bignumber::setValue(uint32_t value) {
	setValue((uint64_t)value);
}

void bignumber::setValue(int64_t value) {

	// take the magnitude through unsigned arithmetic, as the magnitude of
	// the smallest int64_t doesn't fit in an int64_t
	uint64_t	magnitude=(value<0)?
				((uint64_t)(-(value+1)))+1:(uint64_t)value;

	byte_t	buffer[8];
	bignumberpack(magnitude,buffer);
	setValue(buffer,sizeof(buffer),(value<0));
}

void bignumber::setValue(uint64_t value) {
	byte_t	buffer[8];
	bignumberpack(value,buffer);
	setValue(buffer,sizeof(buffer),false);
}

bool bignumber::setValue(const byte_t *magnitude, size_t size) {
	return setValue(magnitude,size,false);
}

bool bignumber::setValue(const byte_t *magnitude, size_t size,
							bool negative) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!magnitude) {
		pvt->zero();
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	if (!size) {
		pvt->zero();
		return true;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_bin2bn((const unsigned char *)magnitude,
						(int)size,pvt->_bn)) {
			pvt->zero();
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchsetbytes(pvt->_bn,magnitude,size)) {
			pvt->zero();
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	pvt->setNegative(negative && !isZero());

	return true;
}

void bignumber::setValue(const bignumber &value) {
	setError(BIGNUMBER_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_copy(pvt->_bn,value.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
		}
	#else
		if (!bignumberscratchcopy(pvt->_bn,value.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
		}
	#endif
}

bignumber &bignumber::operator=(const bignumber &value) {
	if (this!=&value) {
		setValue(value);
	}
	return *this;
}

bignumber &bignumber::operator=(const char *value) {
	setValue(value,10);
	return *this;
}

bignumber &bignumber::operator=(int32_t value) {
	setValue((int64_t)value);
	return *this;
}

bignumber &bignumber::operator=(uint32_t value) {
	setValue((uint64_t)value);
	return *this;
}

bignumber &bignumber::operator=(int64_t value) {
	setValue(value);
	return *this;
}

bignumber &bignumber::operator=(uint64_t value) {
	setValue(value);
	return *this;
}

const char *bignumber::getString() {
	return getString(10);
}

const char *bignumber::getString(uint16_t base) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (base!=10 && base!=16) {
		setError(BIGNUMBER_ERROR_INVALID_BASE);
		return NULL;
	}

	// discard the previously returned string
	delete[] pvt->_str;
	pvt->_str=NULL;

	#if defined(RUDIMENTS_HAS_SSL)
		// BN_bn2dec() and BN_bn2hex() return a string that libcrypto
		// owns, and that has to be released with OPENSSL_free() rather
		// than delete[], so it's copied out and released right away
		char	*str=(base==10)?BN_bn2dec(pvt->_bn):
						BN_bn2hex(pvt->_bn);
		if (!str) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return NULL;
		}
		pvt->_str=charstring::duplicate(str);
		OPENSSL_free(str);
	#else
		pvt->_str=bignumberscratchgetstring(pvt->_bn,base);
	#endif

	if (!pvt->_str) {
		setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
		return NULL;
	}

	return pvt->_str;
}

bool bignumber::getValue(int64_t *value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!value) {
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	if (getBitCount()>64) {
		setError(BIGNUMBER_ERROR_OVERFLOW);
		return false;
	}

	// get the magnitude, right-justified in 8 bytes
	byte_t	buffer[8];
	bytestring::zero(buffer,sizeof(buffer));
	size_t	size=getMagnitudeSize();
	getMagnitude(buffer+(sizeof(buffer)-size),size);
	uint64_t	magnitude=bignumberunpack(buffer);

	// apply the sign...
	//
	// The magnitude of the smallest int64_t is 1 more than the magnitude
	// of the largest, and doesn't fit in an int64_t itself, so the
	// negative value is built up without ever negating it directly.
	if (isNegative()) {
		if (magnitude>(((uint64_t)1)<<63)) {
			setError(BIGNUMBER_ERROR_OVERFLOW);
			return false;
		}
		*value=-((int64_t)(magnitude-1))-1;
	} else {
		if (magnitude>((((uint64_t)1)<<63)-1)) {
			setError(BIGNUMBER_ERROR_OVERFLOW);
			return false;
		}
		*value=(int64_t)magnitude;
	}

	return true;
}

bool bignumber::getValue(uint64_t *value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!value) {
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	if (isNegative() || getBitCount()>64) {
		setError(BIGNUMBER_ERROR_OVERFLOW);
		return false;
	}

	// get the magnitude, right-justified in 8 bytes
	byte_t	buffer[8];
	bytestring::zero(buffer,sizeof(buffer));
	size_t	size=getMagnitudeSize();
	getMagnitude(buffer+(sizeof(buffer)-size),size);

	*value=bignumberunpack(buffer);

	return true;
}

size_t bignumber::getMagnitudeSize() {

	// zero has no bytes at all internally, but it's reported as the single
	// 0 byte that getMagnitude() writes for it
	if (isZero()) {
		return 1;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		return (size_t)BN_num_bytes(pvt->_bn);
	#else
		return bignumberscratchgetbytecount(pvt->_bn);
	#endif
}

bool bignumber::getMagnitude(byte_t *buffer, size_t buffersize) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!buffer) {
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	if (buffersize<getMagnitudeSize()) {
		setError(BIGNUMBER_ERROR_BUFFER_TOO_SMALL);
		return false;
	}

	if (isZero()) {
		buffer[0]=0;
		return true;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		BN_bn2bin(pvt->_bn,(unsigned char *)buffer);
	#else
		bignumberscratchgetbytes(pvt->_bn,buffer);
	#endif

	return true;
}

size_t bignumber::getBitCount() {
	#if defined(RUDIMENTS_HAS_SSL)
		return (size_t)BN_num_bits(pvt->_bn);
	#else
		return bignumberscratchgetbitcount(pvt->_bn);
	#endif
}

int32_t bignumber::getSign() {
	if (isZero()) {
		return 0;
	}
	return (isNegative())?-1:1;
}

bool bignumber::isNegative() {
	return pvt->isNegative();
}

bool bignumber::isZero() {
	return pvt->isZero();
}

bool bignumber::add(const bignumber &addend) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_add(pvt->_bn,pvt->_bn,addend.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchadd(pvt->_bn,pvt->_bn,addend.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	return true;
}

bool bignumber::subtract(const bignumber &subtrahend) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_sub(pvt->_bn,pvt->_bn,subtrahend.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchsubtract(pvt->_bn,pvt->_bn,
						subtrahend.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	return true;
}

bool bignumber::multiply(const bignumber &multiplier) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	#if defined(RUDIMENTS_HAS_SSL)
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_mul(pvt->_bn,pvt->_bn,
					multiplier.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchmultiply(pvt->_bn,pvt->_bn,
						multiplier.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	return true;
}

bool bignumber::divide(const bignumber &divisor) {
	return divide(divisor,NULL);
}

bool bignumber::divide(const bignumber &divisor, bignumber *remainder) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// libcrypto reports a zero divisor through its error queue, but the
	// class reports it directly, so it's caught before the divide
	if (divisor.pvt->isZero()) {
		setError(BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		return false;
	}

	// the quotient and remainder are built up separately, so that this
	// instance is left alone if the divide fails
	bignumber	quotient;
	bignumber	rem;

	#if defined(RUDIMENTS_HAS_SSL)
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_div(quotient.pvt->_bn,rem.pvt->_bn,
					pvt->_bn,divisor.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchdivide(quotient.pvt->_bn,rem.pvt->_bn,
					pvt->_bn,divisor.pvt->_bn)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	setValue(quotient);
	if (remainder) {
		remainder->setValue(rem);
	}

	return true;
}

bool bignumber::modulo(const bignumber &divisor) {

	bignumber	remainder;
	if (!divide(divisor,&remainder)) {
		return false;
	}

	setValue(remainder);

	return true;
}

bool bignumber::nonNegativeModulo(const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// libcrypto reports a zero modulus through its error queue, but the
	// class reports it directly, so it's caught before the operation
	if (modulus.pvt->isZero()) {
		setError(BIGNUMBER_ERROR_DIVIDE_BY_ZERO);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result;

	#if defined(RUDIMENTS_HAS_SSL)
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_nnmod(result.pvt->_bn,pvt->_bn,
						modulus.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		// modulo() gives a remainder with the sign of the dividend, so
		// a negative remainder is brought into range by adding the
		// magnitude of the modulus to it.  That works for a negative
		// modulus too, as the remainder is always smaller than the
		// modulus in magnitude.
		result.setValue(*this);
		if (!result.modulo(modulus)) {
			setError(result.getError());
			return false;
		}
		if (result.isNegative()) {
			bignumber	absmodulus(modulus);
			absmodulus.absoluteValue();
			if (!result.add(absmodulus)) {
				setError(result.getError());
				return false;
			}
		}
	#endif

	setValue(result);

	return true;
}

// add() followed by nonNegativeModulo(), which both backends share
bool bignumber::modAdd(const bignumber &addend, const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// unlike nonNegativeModulo(), the modulus has to be positive here
	if (modulus.pvt->isZero() || modulus.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_INVALID_MODULUS);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result(*this);
	if (!result.add(addend) || !result.nonNegativeModulo(modulus)) {
		setError(result.getError());
		return false;
	}

	setValue(result);

	return true;
}

// subtract() followed by nonNegativeModulo(), which both backends share
bool bignumber::modSub(const bignumber &subtrahend, const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// unlike nonNegativeModulo(), the modulus has to be positive here
	if (modulus.pvt->isZero() || modulus.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_INVALID_MODULUS);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result(*this);
	if (!result.subtract(subtrahend) ||
			!result.nonNegativeModulo(modulus)) {
		setError(result.getError());
		return false;
	}

	setValue(result);

	return true;
}

bool bignumber::modMul(const bignumber &multiplier, const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// unlike nonNegativeModulo(), the modulus has to be positive here
	if (modulus.pvt->isZero() || modulus.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_INVALID_MODULUS);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result;

	#if defined(RUDIMENTS_HAS_SSL)
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_mod_mul(result.pvt->_bn,pvt->_bn,
						multiplier.pvt->_bn,
						modulus.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		// multiply first and reduce afterward, which can leave a large
		// intermediate value, but keeps the implementation simple
		result.setValue(*this);
		if (!result.multiply(multiplier) ||
				!result.nonNegativeModulo(modulus)) {
			setError(result.getError());
			return false;
		}
	#endif

	setValue(result);

	return true;
}

bool bignumber::modPow(const bignumber &exponent, const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (modulus.pvt->isZero() || modulus.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_INVALID_MODULUS);
		return false;
	}

	// libcrypto quietly uses the magnitude of a negative exponent, rather
	// than rejecting it, so it's caught here instead
	if (exponent.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_NEGATIVE_EXPONENT);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result;

	#if defined(RUDIMENTS_HAS_SSL)
		// An exponent of 0 needs no special case here.  BN_mod_exp()
		// gives 1 for it already, and 0 for it when the modulus is 1.
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_mod_exp(result.pvt->_bn,pvt->_bn,
						exponent.pvt->_bn,
						modulus.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		// square-and-multiply...
		//
		// The base is reduced up front, and again after every squaring,
		// so the intermediate values never grow beyond the modulus.
		result.setValue((int64_t)1);

		bignumber	base(*this);
		if (!base.nonNegativeModulo(modulus)) {
			setError(base.getError());
			return false;
		}

		// the bits of the exponent are taken from its magnitude, least
		// significant bit first
		bignumber	e(exponent);
		size_t		bits=e.getBitCount();
		size_t		size=e.getMagnitudeSize();
		byte_t		*magnitude=new byte_t[size];

		bignumbererror_t	err=BIGNUMBER_ERROR_SUCCESS;
		if (!e.getMagnitude(magnitude,size)) {
			err=e.getError();
		}
		for (size_t i=0; err==BIGNUMBER_ERROR_SUCCESS && i<bits; i++) {
			if (magnitude[size-1-i/8]&((byte_t)(1<<(i%8)))) {
				if (!result.modMul(base,modulus)) {
					err=result.getError();
					break;
				}
			}
			if (i+1<bits && !base.modMul(base,modulus)) {
				err=base.getError();
				break;
			}
		}

		delete[] magnitude;

		if (err!=BIGNUMBER_ERROR_SUCCESS) {
			setError(err);
			return false;
		}

		// An exponent of 0 skips the loop entirely, leaving the result
		// 1, which still has to be reduced, as every value is 0 modulo
		// 1.
		if (!result.nonNegativeModulo(modulus)) {
			setError(result.getError());
			return false;
		}
	#endif

	setValue(result);

	return true;
}

bool bignumber::modInverse(const bignumber &modulus) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (modulus.pvt->isZero() || modulus.pvt->isNegative()) {
		setError(BIGNUMBER_ERROR_INVALID_MODULUS);
		return false;
	}

	// every value is 0 modulo 1, and libcrypto fails outright for that
	// case, rather than giving 0, so it's handled here instead
	bignumber	one((int64_t)1);
	if (!one.compare(modulus)) {
		setValue((int64_t)0);
		return true;
	}

	// The inverse exists only if this instance and the modulus are
	// relatively prime.  libcrypto reports that case and an allocation
	// failure the same way, so the two are told apart by checking for it
	// up front.
	bignumber	divisor(*this);
	if (!divisor.gcd(modulus)) {
		setError(divisor.getError());
		return false;
	}
	if (divisor.compare(one)) {
		setError(BIGNUMBER_ERROR_NO_INVERSE);
		return false;
	}

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result;

	#if defined(RUDIMENTS_HAS_SSL)
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_mod_inverse(result.pvt->_bn,pvt->_bn,
						modulus.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		// extended euclidean algorithm...
		//
		// The remainders r0 and r1 run the plain euclidean algorithm,
		// while s0 and s1 track the coefficient of this instance in
		// r=s*this+t*modulus.  The corresponding t is never needed.
		// When the algorithm ends, r0 is the greatest common divisor,
		// which is known to be 1 here, and s0 is the inverse.
		bignumber	r0(modulus);
		bignumber	r1(*this);
		bignumber	s0((int64_t)0);
		bignumber	s1((int64_t)1);

		// reducing this instance up front keeps every value in the
		// loop non-negative, even if this instance is negative
		if (!r1.nonNegativeModulo(modulus)) {
			setError(r1.getError());
			return false;
		}

		while (!r1.isZero()) {

			bignumber	quotient(r0);
			bignumber	remainder;
			if (!quotient.divide(r1,&remainder)) {
				setError(quotient.getError());
				return false;
			}
			r0.setValue(r1);
			r1.setValue(remainder);

			bignumber	product(quotient);
			bignumber	news1(s0);
			if (!product.multiply(s1)) {
				setError(product.getError());
				return false;
			}
			if (!news1.subtract(product)) {
				setError(news1.getError());
				return false;
			}
			s0.setValue(s1);
			s1.setValue(news1);
		}

		// the coefficient can come out negative, so it's brought into
		// the range 0 through the modulus minus 1 here
		result.setValue(s0);
		if (!result.nonNegativeModulo(modulus)) {
			setError(result.getError());
			return false;
		}
	#endif

	setValue(result);

	return true;
}

bool bignumber::gcd(const bignumber &value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	// the result is built up separately, so that this instance is left
	// alone if the operation fails
	bignumber	result;

	#if defined(RUDIMENTS_HAS_SSL)
		// BN_gcd() ignores the signs and gives a non-negative result
		// already, including 0 when both values are 0
		BN_CTX	*ctx=pvt->getContext();
		if (!ctx || !BN_gcd(result.pvt->_bn,pvt->_bn,
						value.pvt->_bn,ctx)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		// The euclidean algorithm, run on the magnitudes, so the signs
		// are ignored and the result is non-negative.  Two zeros give
		// 0, and one zero gives the magnitude of the other value, as
		// the first pass through the loop swaps them.
		result.setValue(*this);
		result.absoluteValue();

		bignumber	divisor(value);
		divisor.absoluteValue();

		while (!divisor.isZero()) {
			bignumber	remainder;
			if (!result.divide(divisor,&remainder)) {
				setError(result.getError());
				return false;
			}
			result.setValue(divisor);
			divisor.setValue(remainder);
		}
	#endif

	setValue(result);

	return true;
}

bool bignumber::negate() {
	setError(BIGNUMBER_ERROR_SUCCESS);
	if (!isZero()) {
		pvt->setNegative(!isNegative());
	}
	return true;
}

bool bignumber::absoluteValue() {
	setError(BIGNUMBER_ERROR_SUCCESS);
	pvt->setNegative(false);
	return true;
}

int32_t bignumber::compare(const bignumber &value) {
	#if defined(RUDIMENTS_HAS_SSL)
		int	result=BN_cmp(pvt->_bn,value.pvt->_bn);
		return (result<0)?-1:((result>0)?1:0);
	#else
		return bignumberscratchcompare(pvt->_bn,value.pvt->_bn);
	#endif
}

bool bignumber::leftShift(uint64_t bits) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!bits || isZero()) {
		return true;
	}

	// a shift that far can't be represented, let alone allocated
	if (bits>(uint64_t)0x7fffffff) {
		setError(BIGNUMBER_ERROR_OVERFLOW);
		return false;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_lshift(pvt->_bn,pvt->_bn,(int)bits)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchleftshift(pvt->_bn,pvt->_bn,bits)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	return true;
}

bool bignumber::rightShift(uint64_t bits) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!bits) {
		return true;
	}

	// shifting the entire magnitude off gives 0, and the sign goes with
	// it, as there is no negative zero
	if (bits>=(uint64_t)getBitCount()) {
		pvt->zero();
		return true;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_rshift(pvt->_bn,pvt->_bn,(int)bits)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
			return false;
		}
	#else
		if (!bignumberscratchrightshift(pvt->_bn,pvt->_bn,bits)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			return false;
		}
	#endif

	return true;
}

// The bitwise methods, and the helpers below, are written entirely in terms of
// the class's own magnitude/sign methods, so both backends share them.
size_t bignumber::getTwosComplementSize(const bignumber &value) {

	// a two's complement value needs the bits of the magnitude, plus one
	// more bit for the sign, rounded up to a whole number of bytes, and
	// both values have to be converted at the same width
	bignumber	tmp(value);
	size_t		thissize=getBitCount()/8+1;
	size_t		valuesize=tmp.getBitCount()/8+1;
	return (thissize>valuesize)?thissize:valuesize;
}

bool bignumber::toTwosComplement(byte_t *buffer, size_t size) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!buffer) {
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	size_t	magnitudesize=getMagnitudeSize();
	if (size<magnitudesize) {
		setError(BIGNUMBER_ERROR_BUFFER_TOO_SMALL);
		return false;
	}

	// right-justify the magnitude, padding on the left with 0's
	bytestring::zero(buffer,size-magnitudesize);
	if (!getMagnitude(buffer+(size-magnitudesize),magnitudesize)) {
		return false;
	}

	// a positive value is just its magnitude
	if (!isNegative()) {
		return true;
	}

	// a negative value is the two's complement of its magnitude,
	// invert every bit and add 1, propagating the carry to the left
	uint16_t	carry=1;
	for (size_t i=size; i>0; i--) {
		carry=carry+(byte_t)(~buffer[i-1]);
		buffer[i-1]=(byte_t)(carry&0xff);
		carry=carry>>8;
	}

	return true;
}

bool bignumber::fromTwosComplement(const byte_t *buffer, size_t size) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!buffer) {
		setError(BIGNUMBER_ERROR_NULL);
		return false;
	}

	// a clear sign bit means the value is its own magnitude, and the
	// leading 0 bytes that the sign extension added are stripped off
	// when the value is set
	if (!size || !(buffer[0]&0x80)) {
		return setValue(buffer,size,false);
	}

	// otherwise the magnitude is the two's complement of the value
	byte_t		*magnitude=new byte_t[size];
	uint16_t	carry=1;
	for (size_t i=size; i>0; i--) {
		carry=carry+(byte_t)(~buffer[i-1]);
		magnitude[i-1]=(byte_t)(carry&0xff);
		carry=carry>>8;
	}

	bool	retval=setValue(magnitude,size,true);

	delete[] magnitude;

	return retval;
}

bool bignumber::bitwiseAnd(const bignumber &value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	bignumber	tmp(value);
	size_t		size=getTwosComplementSize(tmp);

	byte_t	*thisbuffer=new byte_t[size];
	byte_t	*valuebuffer=new byte_t[size];

	bool	retval=toTwosComplement(thisbuffer,size);
	if (retval) {
		retval=tmp.toTwosComplement(valuebuffer,size);
		if (!retval) {
			setError(tmp.getError());
		}
	}
	if (retval) {
		for (size_t i=0; i<size; i++) {
			thisbuffer[i]=thisbuffer[i]&valuebuffer[i];
		}
		retval=fromTwosComplement(thisbuffer,size);
	}

	delete[] thisbuffer;
	delete[] valuebuffer;

	return retval;
}

bool bignumber::bitwiseOr(const bignumber &value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	bignumber	tmp(value);
	size_t		size=getTwosComplementSize(tmp);

	byte_t	*thisbuffer=new byte_t[size];
	byte_t	*valuebuffer=new byte_t[size];

	bool	retval=toTwosComplement(thisbuffer,size);
	if (retval) {
		retval=tmp.toTwosComplement(valuebuffer,size);
		if (!retval) {
			setError(tmp.getError());
		}
	}
	if (retval) {
		for (size_t i=0; i<size; i++) {
			thisbuffer[i]=thisbuffer[i]|valuebuffer[i];
		}
		retval=fromTwosComplement(thisbuffer,size);
	}

	delete[] thisbuffer;
	delete[] valuebuffer;

	return retval;
}

bool bignumber::bitwiseXor(const bignumber &value) {

	setError(BIGNUMBER_ERROR_SUCCESS);

	bignumber	tmp(value);
	size_t		size=getTwosComplementSize(tmp);

	byte_t	*thisbuffer=new byte_t[size];
	byte_t	*valuebuffer=new byte_t[size];

	bool	retval=toTwosComplement(thisbuffer,size);
	if (retval) {
		retval=tmp.toTwosComplement(valuebuffer,size);
		if (!retval) {
			setError(tmp.getError());
		}
	}
	if (retval) {
		for (size_t i=0; i<size; i++) {
			thisbuffer[i]=thisbuffer[i]^valuebuffer[i];
		}
		retval=fromTwosComplement(thisbuffer,size);
	}

	delete[] thisbuffer;
	delete[] valuebuffer;

	return retval;
}

// the complement of x is -x-1
bool bignumber::bitwiseNot() {

	setError(BIGNUMBER_ERROR_SUCCESS);

	if (!negate()) {
		return false;
	}

	bignumber	one((int64_t)1);
	return subtract(one);
}

bignumber bignumber::operator+(const bignumber &value) {
	bignumber	result(*this);
	if (!result.add(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator-(const bignumber &value) {
	bignumber	result(*this);
	if (!result.subtract(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator*(const bignumber &value) {
	bignumber	result(*this);
	if (!result.multiply(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator/(const bignumber &value) {
	bignumber	result(*this);
	if (!result.divide(value)) {
		setError(result.getError());
		result.setValue((int64_t)0);
	}
	return result;
}

bignumber bignumber::operator%(const bignumber &value) {
	bignumber	result(*this);
	if (!result.modulo(value)) {
		setError(result.getError());
		result.setValue((int64_t)0);
	}
	return result;
}

bignumber &bignumber::operator+=(const bignumber &value) {
	add(value);
	return *this;
}

bignumber &bignumber::operator-=(const bignumber &value) {
	subtract(value);
	return *this;
}

bignumber &bignumber::operator*=(const bignumber &value) {
	multiply(value);
	return *this;
}

bignumber &bignumber::operator/=(const bignumber &value) {
	divide(value);
	return *this;
}

bignumber &bignumber::operator%=(const bignumber &value) {
	modulo(value);
	return *this;
}

bignumber bignumber::operator-() {
	bignumber	result(*this);
	result.negate();
	return result;
}

bignumber &bignumber::operator++() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_add_word(pvt->_bn,1)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
		}
	#else
		if (!bignumberscratchaddword(pvt->_bn,1)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
		}
	#endif
	return *this;
}

bignumber bignumber::operator++(int) {
	bignumber	result(*this);
	++(*this);
	return result;
}

bignumber &bignumber::operator--() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!BN_sub_word(pvt->_bn,1)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
			bignumberclearsslerror();
		}
	#else
		if (!bignumberscratchsubtractword(pvt->_bn,1)) {
			setError(BIGNUMBER_ERROR_OUT_OF_MEMORY);
		}
	#endif
	return *this;
}

bignumber bignumber::operator--(int) {
	bignumber	result(*this);
	--(*this);
	return result;
}

bool bignumber::operator==(const bignumber &value) {
	return (compare(value)==0);
}

bool bignumber::operator!=(const bignumber &value) {
	return (compare(value)!=0);
}

bool bignumber::operator<(const bignumber &value) {
	return (compare(value)<0);
}

bool bignumber::operator>(const bignumber &value) {
	return (compare(value)>0);
}

bool bignumber::operator<=(const bignumber &value) {
	return (compare(value)<=0);
}

bool bignumber::operator>=(const bignumber &value) {
	return (compare(value)>=0);
}

bignumber bignumber::operator&(const bignumber &value) {
	bignumber	result(*this);
	if (!result.bitwiseAnd(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator|(const bignumber &value) {
	bignumber	result(*this);
	if (!result.bitwiseOr(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator^(const bignumber &value) {
	bignumber	result(*this);
	if (!result.bitwiseXor(value)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator~() {
	bignumber	result(*this);
	if (!result.bitwiseNot()) {
		setError(result.getError());
	}
	return result;
}

bignumber &bignumber::operator&=(const bignumber &value) {
	bitwiseAnd(value);
	return *this;
}

bignumber &bignumber::operator|=(const bignumber &value) {
	bitwiseOr(value);
	return *this;
}

bignumber &bignumber::operator^=(const bignumber &value) {
	bitwiseXor(value);
	return *this;
}

bignumber bignumber::operator<<(uint64_t bits) {
	bignumber	result(*this);
	if (!result.leftShift(bits)) {
		setError(result.getError());
	}
	return result;
}

bignumber bignumber::operator>>(uint64_t bits) {
	bignumber	result(*this);
	if (!result.rightShift(bits)) {
		setError(result.getError());
	}
	return result;
}

bignumber &bignumber::operator<<=(uint64_t bits) {
	leftShift(bits);
	return *this;
}

bignumber &bignumber::operator>>=(uint64_t bits) {
	rightShift(bits);
	return *this;
}

bignumbererror_t bignumber::getError() {
	return pvt->_err;
}

void bignumber::setError(bignumbererror_t err) {
	pvt->_err=err;
}

// A fallback implementation is built in, so every platform supports this
// class.
bool bignumber::isSupported() {
	return true;
}
