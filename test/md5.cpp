// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// compares the digest accumulated in "h" to "expected", which is lower-case
// hex.  "appended" is whether every append() along the way succeeded.
static void checkhash(const char *name, md5 *h, bool appended,
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

	md5	h;
	bool	ok=h.append((const byte_t *)str,
				(uint32_t)charstring::getLength(str));
	checkhash(name,&h,ok,expected);
}

// hashes "str" "chunksize" bytes per append(), to exercise the streaming path
static void runchunks(const char *name, const char *str, uint32_t chunksize,
						const char *expected) {

	md5		h;
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

	md5	h;
	bool	ok=true;
	for (uint32_t i=0; i<count; i++) {
		ok=h.append(data,size) && ok;
	}
	checkhash(name,&h,ok,expected);
}

int main(int argc, const char **argv) {

	header("md5");

	md5	supported;
	if (!supported.isSupported()) {
		stdoutput.printf("not supported\n\n");
		return 0;
	}

	test("hash size",supported.getHashSize()==16);

	// empty input
	runstring("empty",
		"",
		"d41d8cd98f00b204e9800998ecf8427e");

	// rfc 1321 test suite
	runstring("rfc 1321 1",
		"a",
		"0cc175b9c0f1b6a831c399e269772661");
	runstring("rfc 1321 2",
		"abc",
		"900150983cd24fb0d6963f7d28e17f72");
	runstring("rfc 1321 3",
		"message digest",
		"f96b697d7cb7938d525a2f31aaf161d0");
	runstring("rfc 1321 4",
		"abcdefghijklmnopqrstuvwxyz",
		"c3fcd3d76192e4007dfb496cca67e13b");
	runstring("rfc 1321 5",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789",
		"d174ab98d277d9f5a5611c2c9f419d9f");
	runstring("rfc 1321 6",
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890",
		"57edf4a22be3c955ac49da2e2107b67a");

	// a million a's, a thousand per append()
	byte_t	athousandas[1000];
	bytestring::set(athousandas,'a',sizeof(athousandas));
	runrepeated("a million a's",
		athousandas,sizeof(athousandas),1000,
		"7707d6ae4e027c70eea2a935c2296f21");

	// the same 64 byte block, ten times
	runrepeated("repeated block",
		(const byte_t *)"0123456701234567012345670123456701234567"
				"012345670123456701234567",64,10,
		"ffeaeb581c29c85301f6d7252808fa3d");

	// digests confirmed against coreutils md5sum
	runstring("hello world",
		"hello world",
		"5eb63bbbe01eeed093cb22bb8f5acdc3");
	runstring("goodbye world",
		"goodbye world",
		"0949f7eb1f66dad39d488d5d22531166");

	// the same data, a chunk at a time.  80 bytes straddles md5's 64 byte
	// block, so the chunked path has to carry a partial block.
	runchunks("streaming, 1 byte",
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890",1,
		"57edf4a22be3c955ac49da2e2107b67a");
	runchunks("streaming, 10 bytes",
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890",10,
		"57edf4a22be3c955ac49da2e2107b67a");
	runchunks("streaming, 55 bytes",
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890",55,
		"57edf4a22be3c955ac49da2e2107b67a");
	runchunks("streaming, hello world",
		"hello world",1,
		"5eb63bbbe01eeed093cb22bb8f5acdc3");

	// clear() has to put the instance back where it started
	md5	reused;
	reused.append((const byte_t *)"hello world",11);
	reused.getHash();
	reused.clear();
	bool	ok=reused.append((const byte_t *)"abc",3);
	checkhash("reuse after clear",&reused,ok,
		"900150983cd24fb0d6963f7d28e17f72");

	stdoutput.printf("\n");

	return 0;
}
