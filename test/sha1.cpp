// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sha1.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// compares the digest accumulated in "h" to "expected", which is lower-case
// hex.  "appended" is whether every append() along the way succeeded.
static void checkhash(const char *name, sha1 *h, bool appended,
						const char *expected) {

	const byte_t	*hashval=h->getHash();
	if (!appended || !hashval) {
		test(name,false);
		return;
	}

	char	*hex=charstring::hexEncode(hashval,(size_t)h->getHashSize());
	test(name,!charstring::compare(hex,expected));
	delete[] hex;
}

// hashes "str" in a single append()
static void runstring(const char *name, const char *str,
					const char *expected) {

	sha1	h;
	bool	ok=h.append((const byte_t *)str,
				(uint32_t)charstring::getLength(str));
	checkhash(name,&h,ok,expected);
}

// hashes "str" "chunksize" bytes per append(), to exercise the streaming path
static void runchunks(const char *name, const char *str, uint32_t chunksize,
						const char *expected) {

	sha1		h;
	bool		ok=true;
	uint32_t	size=(uint32_t)charstring::getLength(str);
	for (uint32_t i=0; i<size; i+=chunksize) {
		uint32_t	remaining=size-i;
		ok=h.append((const byte_t *)str+i,
				(remaining<chunksize)?remaining:chunksize) && ok;
	}
	checkhash(name,&h,ok,expected);
}

// appends "size" bytes of "data" "count" times over
static void runrepeated(const char *name, const byte_t *data, uint32_t size,
					uint32_t count, const char *expected) {

	sha1	h;
	bool	ok=true;
	for (uint32_t i=0; i<count; i++) {
		ok=h.append(data,size) && ok;
	}
	checkhash(name,&h,ok,expected);
}

int main(int argc, const char **argv) {

	header("sha1");

	sha1	supported;
	if (!supported.isSupported()) {
		stdoutput.printf("not supported\n\n");
		return 0;
	}

	test("hash size",supported.getHashSize()==20);

	// empty input
	runstring("empty",
		"",
		"da39a3ee5e6b4b0d3255bfef95601890afd80709");

	// rfc 3174 test vectors
	runstring("rfc 3174 1",
		"abc",
		"a9993e364706816aba3e25717850c26c9cd0d89d");
	runstring("rfc 3174 2",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		"84983e441c3bd26ebaae4aa1f95129e5e54670f1");

	// rfc 3174 test vector 3 - a million a's, a thousand per append()
	byte_t	athousandas[1000];
	bytestring::set(athousandas,'a',sizeof(athousandas));
	runrepeated("rfc 3174 3",
		athousandas,sizeof(athousandas),1000,
		"34aa973cd4c4daa4f61eeb2bdbad27316534016f");

	// rfc 3174 test vector 4 - the same 64 byte block, ten times
	runrepeated("rfc 3174 4",
		(const byte_t *)"0123456701234567012345670123456701234567"
				"012345670123456701234567",64,10,
		"dea356a2cddd90c7a7ecedc5ebb563934f460452");

	// digests confirmed against coreutils sha1sum
	runstring("hello world",
		"hello world",
		"2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
	runstring("goodbye world",
		"goodbye world",
		"0078bb8e5c9d8abf7f1e4e14c87d9023235b6230");

	// the same data, one append() at a time
	runchunks("streaming, 1 byte",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",1,
		"84983e441c3bd26ebaae4aa1f95129e5e54670f1");
	runchunks("streaming, 10 bytes",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",10,
		"84983e441c3bd26ebaae4aa1f95129e5e54670f1");
	runchunks("streaming, 55 bytes",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",55,
		"84983e441c3bd26ebaae4aa1f95129e5e54670f1");
	runchunks("streaming, hello world",
		"hello world",1,
		"2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");

	// clear() has to report success, and put the instance back where it
	// started
	sha1	reused;
	reused.append((const byte_t *)"hello world",11);
	reused.getHash();
	test("clear",reused.clear());
	bool	ok=reused.append((const byte_t *)"abc",3);
	checkhash("reuse after clear",&reused,ok,
		"a9993e364706816aba3e25717850c26c9cd0d89d");

	stdoutput.printf("\n");

	return 0;
}
