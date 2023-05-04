/* MIT (BSD) license

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


/* SHA256 core code translated from the Bitcoin project's C++:
 *
 * src/crypto/sha256.cpp commit 417532c8acb93c36c2b6fd052b7c11b6a2906aa2
 * Copyright (c) 2014 The Bitcoin Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include <rudiments/bytestring.h>
#include <rudiments/filedescriptor.h>

struct sha256_t {
	union {
		uint32_t u32[8];
		unsigned char u8[32];
	} u;
};

struct sha256_ctx {
	uint32_t s[8];
	union {
		uint32_t u32[16];
		unsigned char u8[64];
	} buf;
	size_t bytes;
};

#define SHA256_INIT							\
	{ { 0x6a09e667ul, 0xbb67ae85ul, 0x3c6ef372ul, 0xa54ff53aul,	\
	    0x510e527ful, 0x9b05688cul, 0x1f83d9abul, 0x5be0cd19ul },	\
	  { { 0 } }, 0 }

static uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
{
	return z ^ (x & (y ^ z));
}
static uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
{
	return (x & y) | (z & (x | y));
}
static uint32_t Sigma0(uint32_t x)
{
	return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
}
static uint32_t Sigma1(uint32_t x)
{
	return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
}
static uint32_t sigma0(uint32_t x)
{
	return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
}
static uint32_t sigma1(uint32_t x)
{
	return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
}

/** One round of SHA-256. */
static void Round(uint32_t a, uint32_t b, uint32_t c, uint32_t *d, uint32_t e, uint32_t f, uint32_t g, uint32_t *h, uint32_t k, uint32_t w)
{
	uint32_t t1 = *h + Sigma1(e) + Ch(e, f, g) + k + w;
	uint32_t t2 = Sigma0(a) + Maj(a, b, c);
	*d += t1;
	*h = t1 + t2;
}

/** Perform one SHA-256 transformation, processing a 64-byte chunk. */
static void Transform(uint32_t *s, const uint32_t *chunk)
{
	uint32_t a = s[0], b = s[1], c = s[2], d = s[3], e = s[4], f = s[5], g = s[6], h = s[7];
	uint32_t w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15;

	Round(a, b, c, &d, e, f, g, &h, 0x428a2f98, w0 = filedescriptor::convertNetToHost(chunk[0]));
	Round(h, a, b, &c, d, e, f, &g, 0x71374491, w1 = filedescriptor::convertNetToHost(chunk[1]));
	Round(g, h, a, &b, c, d, e, &f, 0xb5c0fbcf, w2 = filedescriptor::convertNetToHost(chunk[2]));
	Round(f, g, h, &a, b, c, d, &e, 0xe9b5dba5, w3 = filedescriptor::convertNetToHost(chunk[3]));
	Round(e, f, g, &h, a, b, c, &d, 0x3956c25b, w4 = filedescriptor::convertNetToHost(chunk[4]));
	Round(d, e, f, &g, h, a, b, &c, 0x59f111f1, w5 = filedescriptor::convertNetToHost(chunk[5]));
	Round(c, d, e, &f, g, h, a, &b, 0x923f82a4, w6 = filedescriptor::convertNetToHost(chunk[6]));
	Round(b, c, d, &e, f, g, h, &a, 0xab1c5ed5, w7 = filedescriptor::convertNetToHost(chunk[7]));
	Round(a, b, c, &d, e, f, g, &h, 0xd807aa98, w8 = filedescriptor::convertNetToHost(chunk[8]));
	Round(h, a, b, &c, d, e, f, &g, 0x12835b01, w9 = filedescriptor::convertNetToHost(chunk[9]));
	Round(g, h, a, &b, c, d, e, &f, 0x243185be, w10 = filedescriptor::convertNetToHost(chunk[10]));
	Round(f, g, h, &a, b, c, d, &e, 0x550c7dc3, w11 = filedescriptor::convertNetToHost(chunk[11]));
	Round(e, f, g, &h, a, b, c, &d, 0x72be5d74, w12 = filedescriptor::convertNetToHost(chunk[12]));
	Round(d, e, f, &g, h, a, b, &c, 0x80deb1fe, w13 = filedescriptor::convertNetToHost(chunk[13]));
	Round(c, d, e, &f, g, h, a, &b, 0x9bdc06a7, w14 = filedescriptor::convertNetToHost(chunk[14]));
	Round(b, c, d, &e, f, g, h, &a, 0xc19bf174, w15 = filedescriptor::convertNetToHost(chunk[15]));

	Round(a, b, c, &d, e, f, g, &h, 0xe49b69c1, w0 += sigma1(w14) + w9 + sigma0(w1));
	Round(h, a, b, &c, d, e, f, &g, 0xefbe4786, w1 += sigma1(w15) + w10 + sigma0(w2));
	Round(g, h, a, &b, c, d, e, &f, 0x0fc19dc6, w2 += sigma1(w0) + w11 + sigma0(w3));
	Round(f, g, h, &a, b, c, d, &e, 0x240ca1cc, w3 += sigma1(w1) + w12 + sigma0(w4));
	Round(e, f, g, &h, a, b, c, &d, 0x2de92c6f, w4 += sigma1(w2) + w13 + sigma0(w5));
	Round(d, e, f, &g, h, a, b, &c, 0x4a7484aa, w5 += sigma1(w3) + w14 + sigma0(w6));
	Round(c, d, e, &f, g, h, a, &b, 0x5cb0a9dc, w6 += sigma1(w4) + w15 + sigma0(w7));
	Round(b, c, d, &e, f, g, h, &a, 0x76f988da, w7 += sigma1(w5) + w0 + sigma0(w8));
	Round(a, b, c, &d, e, f, g, &h, 0x983e5152, w8 += sigma1(w6) + w1 + sigma0(w9));
	Round(h, a, b, &c, d, e, f, &g, 0xa831c66d, w9 += sigma1(w7) + w2 + sigma0(w10));
	Round(g, h, a, &b, c, d, e, &f, 0xb00327c8, w10 += sigma1(w8) + w3 + sigma0(w11));
	Round(f, g, h, &a, b, c, d, &e, 0xbf597fc7, w11 += sigma1(w9) + w4 + sigma0(w12));
	Round(e, f, g, &h, a, b, c, &d, 0xc6e00bf3, w12 += sigma1(w10) + w5 + sigma0(w13));
	Round(d, e, f, &g, h, a, b, &c, 0xd5a79147, w13 += sigma1(w11) + w6 + sigma0(w14));
	Round(c, d, e, &f, g, h, a, &b, 0x06ca6351, w14 += sigma1(w12) + w7 + sigma0(w15));
	Round(b, c, d, &e, f, g, h, &a, 0x14292967, w15 += sigma1(w13) + w8 + sigma0(w0));

	Round(a, b, c, &d, e, f, g, &h, 0x27b70a85, w0 += sigma1(w14) + w9 + sigma0(w1));
	Round(h, a, b, &c, d, e, f, &g, 0x2e1b2138, w1 += sigma1(w15) + w10 + sigma0(w2));
	Round(g, h, a, &b, c, d, e, &f, 0x4d2c6dfc, w2 += sigma1(w0) + w11 + sigma0(w3));
	Round(f, g, h, &a, b, c, d, &e, 0x53380d13, w3 += sigma1(w1) + w12 + sigma0(w4));
	Round(e, f, g, &h, a, b, c, &d, 0x650a7354, w4 += sigma1(w2) + w13 + sigma0(w5));
	Round(d, e, f, &g, h, a, b, &c, 0x766a0abb, w5 += sigma1(w3) + w14 + sigma0(w6));
	Round(c, d, e, &f, g, h, a, &b, 0x81c2c92e, w6 += sigma1(w4) + w15 + sigma0(w7));
	Round(b, c, d, &e, f, g, h, &a, 0x92722c85, w7 += sigma1(w5) + w0 + sigma0(w8));
	Round(a, b, c, &d, e, f, g, &h, 0xa2bfe8a1, w8 += sigma1(w6) + w1 + sigma0(w9));
	Round(h, a, b, &c, d, e, f, &g, 0xa81a664b, w9 += sigma1(w7) + w2 + sigma0(w10));
	Round(g, h, a, &b, c, d, e, &f, 0xc24b8b70, w10 += sigma1(w8) + w3 + sigma0(w11));
	Round(f, g, h, &a, b, c, d, &e, 0xc76c51a3, w11 += sigma1(w9) + w4 + sigma0(w12));
	Round(e, f, g, &h, a, b, c, &d, 0xd192e819, w12 += sigma1(w10) + w5 + sigma0(w13));
	Round(d, e, f, &g, h, a, b, &c, 0xd6990624, w13 += sigma1(w11) + w6 + sigma0(w14));
	Round(c, d, e, &f, g, h, a, &b, 0xf40e3585, w14 += sigma1(w12) + w7 + sigma0(w15));
	Round(b, c, d, &e, f, g, h, &a, 0x106aa070, w15 += sigma1(w13) + w8 + sigma0(w0));

	Round(a, b, c, &d, e, f, g, &h, 0x19a4c116, w0 += sigma1(w14) + w9 + sigma0(w1));
	Round(h, a, b, &c, d, e, f, &g, 0x1e376c08, w1 += sigma1(w15) + w10 + sigma0(w2));
	Round(g, h, a, &b, c, d, e, &f, 0x2748774c, w2 += sigma1(w0) + w11 + sigma0(w3));
	Round(f, g, h, &a, b, c, d, &e, 0x34b0bcb5, w3 += sigma1(w1) + w12 + sigma0(w4));
	Round(e, f, g, &h, a, b, c, &d, 0x391c0cb3, w4 += sigma1(w2) + w13 + sigma0(w5));
	Round(d, e, f, &g, h, a, b, &c, 0x4ed8aa4a, w5 += sigma1(w3) + w14 + sigma0(w6));
	Round(c, d, e, &f, g, h, a, &b, 0x5b9cca4f, w6 += sigma1(w4) + w15 + sigma0(w7));
	Round(b, c, d, &e, f, g, h, &a, 0x682e6ff3, w7 += sigma1(w5) + w0 + sigma0(w8));
	Round(a, b, c, &d, e, f, g, &h, 0x748f82ee, w8 += sigma1(w6) + w1 + sigma0(w9));
	Round(h, a, b, &c, d, e, f, &g, 0x78a5636f, w9 += sigma1(w7) + w2 + sigma0(w10));
	Round(g, h, a, &b, c, d, e, &f, 0x84c87814, w10 += sigma1(w8) + w3 + sigma0(w11));
	Round(f, g, h, &a, b, c, d, &e, 0x8cc70208, w11 += sigma1(w9) + w4 + sigma0(w12));
	Round(e, f, g, &h, a, b, c, &d, 0x90befffa, w12 += sigma1(w10) + w5 + sigma0(w13));
	Round(d, e, f, &g, h, a, b, &c, 0xa4506ceb, w13 += sigma1(w11) + w6 + sigma0(w14));
	Round(c, d, e, &f, g, h, a, &b, 0xbef9a3f7, w14 + sigma1(w12) + w7 + sigma0(w15));
	Round(b, c, d, &e, f, g, h, &a, 0xc67178f2, w15 + sigma1(w13) + w8 + sigma0(w0));

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

static void add(struct sha256_ctx *ctx, const void *p, size_t len)
{
	const unsigned char *data = (const unsigned char *)p;
	size_t bufsize = ctx->bytes % 64;

	if (bufsize + len >= 64) {
		/* Fill the buffer, and process it. */
		bytestring::copy(ctx->buf.u8 + bufsize, data, 64 - bufsize);
		ctx->bytes += 64 - bufsize;
		data += 64 - bufsize;
		len -= 64 - bufsize;
		Transform(ctx->s, ctx->buf.u32);
		bufsize = 0;
	}

	while (len >= 64) {
		/* Process full chunks directly from the source. */
		if (alignment_ok(data, sizeof(uint32_t)))
			Transform(ctx->s, (const uint32_t *)data);
		else {
			bytestring::copy(ctx->buf.u8, data, sizeof(ctx->buf));
			Transform(ctx->s, ctx->buf.u32);
		}
		ctx->bytes += 64;
		data += 64;
		len -= 64;
	}
	    
	if (len) {
		/* Fill the buffer with what remains. */
		bytestring::copy(ctx->buf.u8 + bufsize, data, len);
		ctx->bytes += len;
	}
}

void sha256_init(struct sha256_ctx *ctx)
{
	struct sha256_ctx init = SHA256_INIT;
	*ctx = init;
}

void sha256_update(struct sha256_ctx *ctx, const void *p, size_t size)
{
	add(ctx, p, size);
}

void sha256_done(struct sha256_ctx *ctx, struct sha256_t *res)
{
	static const unsigned char pad[64] = {0x80};
	uint64_t sizedesc;
	size_t i;

	sizedesc = filedescriptor::convertHostToNet((uint64_t)ctx->bytes << 3);
	/* Add '1' bit to terminate, then all 0 bits, up to next block - 8. */
	add(ctx, pad, 1 + ((128 - 8 - (ctx->bytes % 64) - 1) % 64));
	/* Add number of bits of data (big endian) */
	add(ctx, &sizedesc, 8);
	for (i = 0; i < sizeof(ctx->s) / sizeof(ctx->s[0]); i++)
		res->u.u32[i] = filedescriptor::convertHostToNet(ctx->s[i]);
	ctx->bytes = (size_t)-1;
}
