// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sha512.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// compares the digest accumulated in "h" to "expected", which is lower-case
// hex.  "appended" is whether every append() along the way succeeded.
static void checkhash(const char *name, sha512 *h, bool appended,
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

	sha512	h;
	bool	ok=h.append((const byte_t *)str,
				(uint32_t)charstring::getLength(str));
	checkhash(name,&h,ok,expected);
}

// hashes "str" "chunksize" bytes per append(), to exercise the streaming path
static void runchunks(const char *name, const char *str, uint32_t chunksize,
						const char *expected) {

	sha512		h;
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

	sha512	h;
	bool	ok=true;
	for (uint32_t i=0; i<count; i++) {
		ok=h.append(data,size) && ok;
	}
	checkhash(name,&h,ok,expected);
}

int main(int argc, const char **argv) {

	header("sha512");

	sha512	supported;
	if (!supported.isSupported()) {
		stdoutput.printf("not supported\n\n");
		return 0;
	}

	test("hash size",supported.getHashSize()==64);

	// empty input
	runstring("empty",
		"",
		"cf83e1357eefb8bdf1542850d66d8007d620e40"
		"50b5715dc83f4a921d36ce9ce47d0d13c5d85f2b"
		"0ff8318d2877eec2f63b931bd47417a81a538327"
		"af927da3e");

	// fips 180-4 test vectors
	runstring("fips 180-4 1",
		"abc",
		"ddaf35a193617abacc417349ae20413112e6fa4e"
		"89a97ea20a9eeee64b55d39a2192992a274fc1a83"
		"6ba3c23a3feebbd454d4423643ce80e2a9ac94fa5"
		"4ca49f");

	// fips 180-4 test vector - a million a's, a thousand per append()
	byte_t	athousandas[1000];
	bytestring::set(athousandas,'a',sizeof(athousandas));
	runrepeated("fips 180-4 2",
		athousandas,sizeof(athousandas),1000,
		"e718483d0ce769644e2e42c7bc15b4638e1f98b1"
		"3b2044285632a803afa973ebde0ff244877ea60a"
		"4cb0432ce577c31beb009c5c2c49aa2e4eadb217a"
		"d8cc09b");

	// digests confirmed against coreutils sha512sum
	runstring("hello world",
		"hello world",
		"309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee5"
		"11a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670"
		"da34255b45b0cfd830e81f605dcf7dc5542e93ae9"
		"cd76f");
	runstring("goodbye world",
		"goodbye world",
		"8c56ee6d5cbe80288b1f28acc5f017eef68bec062"
		"c2d4e4196bebb332d6f8909a9b1734cf4e3786026"
		"227dddb4e6b11d5c0f94765280fe973c8e57cfd8c"
		"0ccde");

	// the same data, one append() at a time
	runchunks("streaming, 1 byte",
		"abc",1,
		"ddaf35a193617abacc417349ae20413112e6fa4e"
		"89a97ea20a9eeee64b55d39a2192992a274fc1a83"
		"6ba3c23a3feebbd454d4423643ce80e2a9ac94fa5"
		"4ca49f");
	runchunks("streaming, 10 bytes",
		"hello world",10,
		"309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee5"
		"11a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670"
		"da34255b45b0cfd830e81f605dcf7dc5542e93ae9"
		"cd76f");
	runchunks("streaming, 55 bytes",
		"abc",55,
		"ddaf35a193617abacc417349ae20413112e6fa4e"
		"89a97ea20a9eeee64b55d39a2192992a274fc1a83"
		"6ba3c23a3feebbd454d4423643ce80e2a9ac94fa5"
		"4ca49f");
	runchunks("streaming, hello world",
		"hello world",1,
		"309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee5"
		"11a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670"
		"da34255b45b0cfd830e81f605dcf7dc5542e93ae9"
		"cd76f");

	// clear() has to report success, and put the instance back where it
	// started
	sha512	reused;
	reused.append((const byte_t *)"hello world",11);
	reused.getHash();
	test("clear",reused.clear());
	bool	ok=reused.append((const byte_t *)"abc",3);
	checkhash("reuse after clear",&reused,ok,
		"ddaf35a193617abacc417349ae20413112e6fa4e"
		"89a97ea20a9eeee64b55d39a2192992a274fc1a83"
		"6ba3c23a3feebbd454d4423643ce80e2a9ac94fa5"
		"4ca49f");

	// reset() has to clear the hash, not just the salt
	sha512	afterreset;
	afterreset.append((const byte_t *)"hello world",11);
	afterreset.getHash();
	test("reset",afterreset.reset());
	bool	resetok=afterreset.append((const byte_t *)"abc",3);
	checkhash("reuse after reset",&afterreset,resetok,
		"ddaf35a193617abacc417349ae20413112e6fa4e"
		"89a97ea20a9eeee64b55d39a2192992a274fc1a83"
		"6ba3c23a3feebbd454d4423643ce80e2a9ac94fa5"
		"4ca49f");

	stdoutput.printf("\n");

	return 0;
}
