/* SHA-512 core code written directly from the specification:
 *
 * FIPS PUB 180-4, Secure Hash Standard, section 6.4
 * (equivalently RFC 6234, section 6.4)
 *
 * The surrounding buffering/padding code mirrors sha256rfc4642.cpp.
 *
 * Placed under the same terms as sha256rfc4642.cpp:
 *
 * MIT (BSD) license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>

struct sha512_t {
	union {
		uint64_t u64[8];
		unsigned char u8[64];
	} u;
};

struct sha512_ctx {
	uint64_t s[8];
	union {
		uint64_t u64[16];
		unsigned char u8[128];
	} buf;
	size_t bytes;
};

#define SHA512_INIT							\
	{ { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,		\
	    0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,		\
	    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,		\
	    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL },		\
	  { { 0 } }, 0 }

static const uint64_t K[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
	0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
	0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
	0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
	0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
	0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
	0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
	0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
	0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
	0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
	0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
	0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
	0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
	0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static uint64_t Ch(uint64_t x, uint64_t y, uint64_t z)
{
	return z ^ (x & (y ^ z));
}
static uint64_t Maj(uint64_t x, uint64_t y, uint64_t z)
{
	return (x & y) | (z & (x | y));
}
static uint64_t Sigma0(uint64_t x)
{
	return (x >> 28 | x << 36) ^ (x >> 34 | x << 30) ^ (x >> 39 | x << 25);
}
static uint64_t Sigma1(uint64_t x)
{
	return (x >> 14 | x << 50) ^ (x >> 18 | x << 46) ^ (x >> 41 | x << 23);
}
static uint64_t sigma0(uint64_t x)
{
	return (x >> 1 | x << 63) ^ (x >> 8 | x << 56) ^ (x >> 7);
}
static uint64_t sigma1(uint64_t x)
{
	return (x >> 19 | x << 45) ^ (x >> 61 | x << 3) ^ (x >> 6);
}

/** Perform one SHA-512 transformation, processing a 128-byte chunk. */
static void Transform(uint64_t *s, const uint64_t *chunk)
{
	uint64_t w[80];
	uint64_t a, b, c, d, e, f, g, h;
	uint64_t t1, t2;
	size_t i;

	/* build the message schedule */
	for (i = 0; i < 16; i++)
		w[i] = filedescriptor::convertNetToHost(chunk[i]);
	for (i = 16; i < 80; i++)
		w[i] = sigma1(w[i - 2]) + w[i - 7] +
			sigma0(w[i - 15]) + w[i - 16];

	/* init the working variables */
	a = s[0]; b = s[1]; c = s[2]; d = s[3];
	e = s[4]; f = s[5]; g = s[6]; h = s[7];

	/* 80 rounds */
	for (i = 0; i < 80; i++) {
		t1 = h + Sigma1(e) + Ch(e, f, g) + K[i] + w[i];
		t2 = Sigma0(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	/* fold in */
	s[0] += a;
	s[1] += b;
	s[2] += c;
	s[3] += d;
	s[4] += e;
	s[5] += f;
	s[6] += g;
	s[7] += h;
}

static bool alignment_ok(const void *p, size_t n)
{
	return ((size_t)p % n == 0);
}

static void add(struct sha512_ctx *ctx, const void *p, size_t len)
{
	const unsigned char *data = (const unsigned char *)p;
	size_t bufsize = ctx->bytes % 128;

	if (bufsize + len >= 128) {
		/* Fill the buffer, and process it. */
		bytestring::copy(ctx->buf.u8 + bufsize, data, 128 - bufsize);
		ctx->bytes += 128 - bufsize;
		data += 128 - bufsize;
		len -= 128 - bufsize;
		Transform(ctx->s, ctx->buf.u64);
		bufsize = 0;
	}

	while (len >= 128) {
		/* Process full chunks directly from the source. */
		if (alignment_ok(data, sizeof(uint64_t)))
			Transform(ctx->s, (const uint64_t *)data);
		else {
			bytestring::copy(ctx->buf.u8, data, sizeof(ctx->buf));
			Transform(ctx->s, ctx->buf.u64);
		}
		ctx->bytes += 128;
		data += 128;
		len -= 128;
	}

	if (len) {
		/* Fill the buffer with what remains. */
		bytestring::copy(ctx->buf.u8 + bufsize, data, len);
		ctx->bytes += len;
	}
}

static void sha512_init(struct sha512_ctx *ctx)
{
	struct sha512_ctx init = SHA512_INIT;
	*ctx = init;
}

static void sha512_update(struct sha512_ctx *ctx, const void *p, size_t size)
{
	add(ctx, p, size);
}

static void sha512_done(struct sha512_ctx *ctx, struct sha512_t *res)
{
	static const unsigned char pad[128] = {0x80};
	static const unsigned char zero[8] = {0};
	uint64_t sizedesc;
	size_t i;

	sizedesc = filedescriptor::convertHostToNet((uint64_t)ctx->bytes << 3);
	/* Add '1' bit to terminate, then all 0 bits, up to next block - 16. */
	add(ctx, pad, 1 + ((256 - 16 - (ctx->bytes % 128) - 1) % 128));
	/* Add number of bits of data, as a 128-bit big endian count */
	add(ctx, zero, 8);
	add(ctx, &sizedesc, 8);
	for (i = 0; i < sizeof(ctx->s) / sizeof(ctx->s[0]); i++)
		res->u.u64[i] = filedescriptor::convertHostToNet(ctx->s[i]);
	ctx->bytes = (size_t)-1;
}
