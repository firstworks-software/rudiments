#include <math.h>
#include <tgmath.h>
#undef remainder
#undef floor
#undef round

inline int32_t math::absoluteValue(int32_t j) {
	// FIXME: use abs on platforms where an int is 32 bits
	return labs(j);
}

inline div_t math::divide(int32_t numer, int32_t denom) {
	// FIXME: use div on platforms where an int is 32 bits
	return ldiv(numer,denom);
}

inline int64_t math::absoluteValue(int64_t j) {
	return llabs(j);
}

inline lldiv_t math::divide(int64_t numer, int64_t denom) {
	return lldiv(numer,denom);
}




// float methods

inline bool math::isFinite(float x) {
	return isfinite(x);
}

inline bool math::isNormal(float x) {
	return isnormal(x);
}

inline bool math::isSubNormal(float x) {
	return (fpclassify(x)==FP_SUBNORMAL);
}

inline bool math::isNaN(float x) {
	return isnan(x);
}

inline bool math::isInfinite(float x) {
	return isinf(x);
}

inline bool math::isGreater(float x, float y) {
	return isgreater(x,y);
}

inline bool math::isGreaterOrEqual(float x, float y) {
	return isgreaterequal(x,y);
}

inline bool math::isLess(float x, float y) {
	return isless(x,y);
}

inline bool math::isLessOrEqual(float x, float y) {
	return islessequal(x,y);
}

inline bool math::isLessOrGreater(float x, float y) {
	return islessgreater(x,y);
}

inline bool math::areNaN(float x, float y) {
	return isunordered(x,y);
}

inline bool math::isSignBitSet(float x) {
	return signbit(x);
}

inline float math::arcCosine(float x) {
	return acosf(x);
}

inline float math::arcSine(float x) {
	return asinf(x);
}

inline float math::arcTangent(float x) {
	return atanf(x);
}

inline float math::arcTangent(float y, float x) {
	return atan2f(y,x);
}

inline float math::cosine(float x) {
	return cosf(x);
}

inline float math::sine(float x) {
	return sinf(x);
}

inline float math::tangent(float x) {
	return tanf(x);
}

inline float math::hyperbolicArcCosine(float x) {
	return acoshf(x);
}

inline float math::hyperbolicArcSine(float x) {
	return asinhf(x);
}

inline float math::hyperbolicArcTangent(float x) {
	return atanhf(x);
}

inline float math::hyperbolicCosine(float x) {
	return coshf(x);
}

inline float math::hyperbolicSine(float x) {
	return sinhf(x);
}

inline float math::hyperbolicTangent(float x) {
	return tanhf(x);
}

inline float math::naturalExponent(float x) {
	return expf(x);
}

inline float math::normalize(float x, int32_t *exp) {
	return frexpf(x,exp);
}

inline float math::naturalLog(float x) {
	return logf(x);
}

inline float math::logBase10(float x) {
	return log10f(x);
}

inline float math::naturalExponentMinusOne(float x) {
	return expm1f(x);
}

inline float math::naturalLogPlusOne(float x) {
	return log1pf(x);
}

inline float math::exponent(float x) {
	return logbf(x);
}

inline float math::exponentBase2(float x) {
	return exp2f(x);
}

inline float math::logBase2(float x) {
	return log2f(x);
}

inline float math::power(float x, float y) {
	return powf(x,y);
}

inline float math::squareRoot(float x) {
	return sqrtf(x);
}

inline float math::hypotenuse(float x, float y) {
	return hypotf(x,y);
}

inline float math::cubeRoot(float x) {
	return cbrtf(x);
}

inline float math::ceiling(float x) {
	return ceilf(x);
}

inline float math::absoluteValue(float x) {
	return fabsf(x);
}

inline float math::floor(float x) {
	return floorf(x);
}

inline float math::remainder(float x, float y) {
	return fmodf(x,y);
}

inline float math::nearbyInteger(float x) {
	return nearbyintf(x);
}

inline float math::round(float x) {
	return roundf(x);
}

inline float math::truncate(float x) {
	return truncf(x);
}

inline float math::remainder(float x, float y, int32_t *quo) {
	return remquof(x,y,quo);
}

inline long math::roundToLong(float x) {
	return lrintf(x);
}

inline int64_t math::roundToLongLong(float x) {
	return llrintf(x);
}

inline long math::roundAwayFromZeroToLong(float x) {
	return lroundf(x);
}

inline int64_t math::roundAwayFromZeroToLongLong(float x) {
	return llroundf(x);
}

inline float math::copySignBit(float x, float y) {
	return copysignf(x,y);
}

inline float math::errorFunction(float x) {
	return erff(x);
}

inline float math::complementaryErrorFunction(float x) {
	return erfcf(x);
}

inline float math::trueGamma(float x) {
	return tgammaf(x);
}

inline float math::naturalLogGamma(float x) {
	return lgammaf(x);
}

inline float math::roundInexact(float x) {
	return rintf(x);
}

inline float math::nextAfter(float x, float y) {
	return nextafterf(x,y);
}

inline float math::nextToward(float x, float y) {
	return nexttowardf(x,y);
}

inline float math::scaleByRadixToPower(float x, float n) {
	return scalbf(x,n);
}

inline float math::scaleByRadixToPower(float x, int32_t n) {
	return scalbnf(x,n);
}

inline float math::scaleByRadixToPower(float x, long n) {
	return scalblnf(x,n);
}

inline int32_t math::integralExponent(float x) {
	return ilogbf(x);
}

inline float math::positiveDifference(float x, float y) {
	return fdimf(x,y);
}

inline float math::larger(float x, float y) {
	return fmaxf(x,y);
}

inline float math::smaller(float x, float y) {
	return fminf(x,y);
}

inline float math::multiplyAndAdd(float x, float y, float z) {
	return fmaf(x,y,z);
}

inline float math::argument(float complex z) {
	return cargf(z);
}

inline float complex math::conjugate(float complex z) {
	return conjf(z);
}

inline float complex math::project(float complex z) {
	return cprojf(z);
}

inline float math::imaginary(float complex z) {
	return cimagf(z);
}

inline float math::real(float complex z) {
	return crealf(z);
}



// double methods

inline bool math::isFinite(double x) {
	return isfinite(x);
}

inline bool math::isNormal(double x) {
	return isnormal(x);
}

inline bool math::isSubNormal(double x) {
	return (fpclassify(x)==FP_SUBNORMAL);
}

inline bool math::isNaN(double x) {
	return isnan(x);
}

inline bool math::isInfinite(double x) {
	return isinf(x);
}

inline bool math::isGreater(double x, double y) {
	return isgreater(x,y);
}

inline bool math::isGreaterOrEqual(double x, double y) {
	return isgreaterequal(x,y);
}

inline bool math::isLess(double x, double y) {
	return isless(x,y);
}

inline bool math::isLessOrEqual(double x, double y) {
	return islessequal(x,y);
}

inline bool math::isLessOrGreater(double x, double y) {
	return islessgreater(x,y);
}

inline bool math::areNaN(double x, double y) {
	return isunordered(x,y);
}

inline bool math::isSignBitSet(double x) {
	return signbit(x);
}

inline double math::arcCosine(double x) {
	return acos(x);
}

inline double math::arcSine(double x) {
	return asin(x);
}

inline double math::arcTangent(double x) {
	return atan(x);
}

inline double math::arcTangent(double y, double x) {
	return atan2(y,x);
}

inline double math::cosine(double x) {
	return cos(x);
}

inline double math::sine(double x) {
	return sin(x);
}

inline double math::tangent(double x) {
	return tan(x);
}

inline double math::hyperbolicArcCosine(double x) {
	return acosh(x);
}

inline double math::hyperbolicArcSine(double x) {
	return asinh(x);
}

inline double math::hyperbolicArcTangent(double x) {
	return atanh(x);
}

inline double math::hyperbolicCosine(double x) {
	return cosh(x);
}

inline double math::hyperbolicSine(double x) {
	return sinh(x);
}

inline double math::hyperbolicTangent(double x) {
	return tanh(x);
}

inline double math::naturalExponent(double x) {
	return exp(x);
}

inline double math::normalize(double x, int32_t *exp) {
	return frexp(x,exp);
}

inline double math::naturalLog(double x) {
	return log(x);
}

inline double math::logBase10(double x) {
	return log10(x);
}

inline double math::naturalExponentMinusOne(double x) {
	return expm1(x);
}

inline double math::naturalLogPlusOne(double x) {
	return log1p(x);
}

inline double math::exponent(double x) {
	return logb(x);
}

inline double math::exponentBase2(double x) {
	return exp2(x);
}

inline double math::logBase2(double x) {
	return log2(x);
}

inline double math::power(double x, double y) {
	return pow(x,y);
}

inline double math::squareRoot(double x) {
	return sqrt(x);
}

inline double math::hypotenuse(double x, double y) {
	return hypot(x,y);
}

inline double math::cubeRoot(double x) {
	return cbrt(x);
}

inline double math::ceiling(double x) {
	return ceil(x);
}

inline double math::absoluteValue(double x) {
	return fabs(x);
}

inline double math::floor(double x) {
	return floor(x);
}

inline double math::remainder(double x, double y) {
	return fmod(x,y);
}

inline double math::nearbyInteger(double x) {
	return nearbyint(x);
}

inline double math::round(double x) {
	return round(x);
}

inline double math::truncate(double x) {
	return trunc(x);
}

inline double math::remainder(double x, double y, int32_t *quo) {
	return remquo(x,y,quo);
}

inline long math::roundToLong(double x) {
	return lrint(x);
}

inline int64_t math::roundToLongLong(double x) {
	return llrint(x);
}

inline long math::roundAwayFromZeroToLong(double x) {
	return lround(x);
}

inline int64_t math::roundAwayFromZeroToLongLong(double x) {
	return llround(x);
}

inline double math::copySignBit(double x, double y) {
	return copysign(x,y);
}

inline double math::errorFunction(double x) {
	return erf(x);
}

inline double math::complementaryErrorFunction(double x) {
	return erfc(x);
}

inline double math::trueGamma(double x) {
	return tgamma(x);
}

inline double math::naturalLogGamma(double x) {
	return lgamma(x);
}

inline double math::roundInexact(double x) {
	return rint(x);
}

inline double math::nextAfter(double x, double y) {
	return nextafter(x,y);
}

inline double math::nextToward(double x, double y) {
	return nexttoward(x,y);
}

inline double math::scaleByRadixToPower(double x, double n) {
	return scalb(x,n);
}

inline double math::scaleByRadixToPower(double x, int32_t n) {
	return scalbn(x,n);
}

inline double math::scaleByRadixToPower(double x, long n) {
	return scalbln(x,n);
}

inline int32_t math::integralExponent(double x) {
	return ilogb(x);
}

inline double math::positiveDifference(double x, double y) {
	return fdim(x,y);
}

inline double math::larger(double x, double y) {
	return fmax(x,y);
}

inline double math::smaller(double x, double y) {
	return fmin(x,y);
}

inline double math::multiplyAndAdd(double x, double y, double z) {
	return fma(x,y,z);
}

inline double math::argument(double complex z) {
	return carg(z);
}

inline double complex math::conjugate(double complex z) {
	return conj(z);
}

inline double complex math::project(double complex z) {
	return cproj(z);
}

inline double math::imaginary(double complex z) {
	return cimag(z);
}

inline double math::real(double complex z) {
	return creal(z);
}


// long double methods

inline bool math::isFinite(long double x) {
	return isfinite(x);
}

inline bool math::isNormal(long double x) {
	return isnormal(x);
}

inline bool math::isSubNormal(long double x) {
	return (fpclassify(x)==FP_SUBNORMAL);
}

inline bool math::isNaN(long double x) {
	return isnan(x);
}

inline bool math::isInfinite(long double x) {
	return isinf(x);
}

inline bool math::isGreater(long double x, long double y) {
	return isgreater(x,y);
}

inline bool math::isGreaterOrEqual(long double x, long double y) {
	return isgreaterequal(x,y);
}

inline bool math::isLess(long double x, long double y) {
	return isless(x,y);
}

inline bool math::isLessOrEqual(long double x, long double y) {
	return islessequal(x,y);
}

inline bool math::isLessOrGreater(long double x, long double y) {
	return islessgreater(x,y);
}

inline bool math::areNaN(long double x, long double y) {
	return isunordered(x,y);
}

inline bool math::isSignBitSet(long double x) {
	return signbit(x);
}

inline long double math::arcCosine(long double x) {
	return acosl(x);
}

inline long double math::arcSine(long double x) {
	return asinl(x);
}

inline long double math::arcTangent(long double x) {
	return atanl(x);
}

inline long double math::arcTangent(long double y, long double x) {
	return atan2l(y,x);
}

inline long double math::cosine(long double x) {
	return cosl(x);
}

inline long double math::sine(long double x) {
	return sinl(x);
}

inline long double math::tangent(long double x) {
	return tanl(x);
}

inline long double math::hyperbolicArcCosine(long double x) {
	return acoshl(x);
}

inline long double math::hyperbolicArcSine(long double x) {
	return asinhl(x);
}

inline long double math::hyperbolicArcTangent(long double x) {
	return atanhl(x);
}

inline long double math::hyperbolicCosine(long double x) {
	return coshl(x);
}

inline long double math::hyperbolicSine(long double x) {
	return sinhl(x);
}

inline long double math::hyperbolicTangent(long double x) {
	return tanhl(x);
}

inline long double math::naturalExponent(long double x) {
	return expl(x);
}

inline long double math::normalize(long double x, int32_t *exp) {
	return frexpl(x,exp);
}

inline long double math::naturalLog(long double x) {
	return logl(x);
}

inline long double math::logBase10(long double x) {
	return log10l(x);
}

inline long double math::naturalExponentMinusOne(long double x) {
	return expm1l(x);
}

inline long double math::naturalLogPlusOne(long double x) {
	return log1pl(x);
}

inline long double math::exponent(long double x) {
	return logbl(x);
}

inline long double math::exponentBase2(long double x) {
	return exp2l(x);
}

inline long double math::logBase2(long double x) {
	return log2l(x);
}

inline long double math::power(long double x, long double y) {
	return powl(x,y);
}

inline long double math::squareRoot(long double x) {
	return sqrtl(x);
}

inline long double math::hypotenuse(long double x, long double y) {
	return hypotl(x,y);
}

inline long double math::cubeRoot(long double x) {
	return cbrtl(x);
}

inline long double math::ceiling(long double x) {
	return ceill(x);
}

inline long double math::absoluteValue(long double x) {
	return fabsl(x);
}

inline long double math::floor(long double x) {
	return floorl(x);
}

inline long double math::remainder(long double x, long double y) {
	return fmodl(x,y);
}

inline long double math::nearbyInteger(long double x) {
	return nearbyintl(x);
}

inline long double math::round(long double x) {
	return roundl(x);
}

inline long double math::truncate(long double x) {
	return truncl(x);
}

inline long double math::remainder(long double x,
						long double y, int32_t *quo) {
	return remquol(x,y,quo);
}

inline long math::roundToLong(long double x) {
	return lrintl(x);
}

inline int64_t math::roundToLongLong(long double x) {
	return llrintl(x);
}

inline long math::roundAwayFromZeroToLong(long double x) {
	return lroundl(x);
}

inline int64_t math::roundAwayFromZeroToLongLong(long double x) {
	return llroundl(x);
}

inline long double math::copySignBit(long double x, long double y) {
	return copysignl(x,y);
}

inline long double math::errorFunction(long double x) {
	return erfl(x);
}

inline long double math::complementaryErrorFunction(long double x) {
	return erfcl(x);
}

inline long double math::trueGamma(long double x) {
	return tgammal(x);
}

inline long double math::naturalLogGamma(long double x) {
	return lgammal(x);
}

inline long double math::roundInexact(long double x) {
	return rintl(x);
}

inline long double math::nextAfter(long double x, long double y) {
	return nextafterl(x,y);
}

inline long double math::nextToward(long double x, long double y) {
	return nexttowardl(x,y);
}

inline long double math::scaleByRadixToPower(long double x,
							long double n) {
	return scalbl(x,n);
}

inline long double math::scaleByRadixToPower(long double x,
								int32_t n) {
	return scalbnl(x,n);
}

inline long double math::scaleByRadixToPower(long double x, long n) {
	return scalblnl(x,n);
}

inline int32_t math::integralExponent(long double x) {
	return ilogbl(x);
}

inline long double math::positiveDifference(long double x,
							long double y) {
	return fdiml(x,y);
}

inline long double math::larger(long double x, long double y) {
	return fmaxl(x,y);
}

inline long double math::smaller(long double x, long double y) {
	return fminl(x,y);
}

inline long double math::multiplyAndAdd(long double x,
						long double y, long double z) {
	return fmal(x,y,z);
}

inline long double math::argument(long double complex z) {
	return cargl(z);
}

inline long double complex math::conjugate(long double complex z) {
	return conjl(z);
}

inline long double complex math::project(long double complex z) {
	return cprojl(z);
}

inline long double math::imaginary(long double complex z) {
	return cimagl(z);
}

inline long double math::real(long double complex z) {
	return creall(z);
}

inline float math::loadExponent(float x, int32_t exp) {
	return ldexpf(x,exp);
}

inline double math::loadExponent(double x, int32_t exp) {
	return ldexp(x,exp);
}

inline long double math::loadExponent(long double x, int32_t exp) {
	return ldexpl(x,exp);
}
