// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sha256.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// compares the digest accumulated in "h" to "expected", which is lower-case
// hex.  "appended" is whether every append() along the way succeeded.
static void checkhash(const char *name, sha256 *h, bool appended,
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

	sha256	h;
	bool	ok=h.append((const byte_t *)str,
				(uint32_t)charstring::getLength(str));
	checkhash(name,&h,ok,expected);
}

// hashes "str" "chunksize" bytes per append(), to exercise the streaming path
static void runchunks(const char *name, const char *str, uint32_t chunksize,
						const char *expected) {

	sha256		h;
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

	sha256	h;
	bool	ok=true;
	for (uint32_t i=0; i<count; i++) {
		ok=h.append(data,size) && ok;
	}
	checkhash(name,&h,ok,expected);
}

int main(int argc, const char **argv) {

	header("sha256");

	sha256	supported;
	if (!supported.isSupported()) {
		stdoutput.printf("not supported\n\n");
		return 0;
	}

	test("hash size",supported.getHashSize()==32);

	// empty input
	runstring("empty",
		"",
		"e3b0c44298fc1c149afbf4c8996fb92427ae41e4"
		"649b934ca495991b7852b855");

	// rfc 4634 test vectors
	runstring("rfc 4634 1",
		"abc",
		"ba7816bf8f01cfea414140de5dae2223b00361a3"
		"96177a9cb410ff61f20015ad");
	runstring("rfc 4634 2",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		"248d6a61d20638b8e5c026930c3e6039a33ce459"
		"64ff2167f6ecedd419db06c1");

	// rfc 4634 test vector 3 - a million a's, a thousand per append()
	byte_t	athousandas[1000];
	bytestring::set(athousandas,'a',sizeof(athousandas));
	runrepeated("rfc 4634 3",
		athousandas,sizeof(athousandas),1000,
		"cdc76e5c9914fb9281a1c7e284d73e67f1809a48"
		"a497200e046d39ccc7112cd0");

	// rfc 4634 test vector 4 - the same 64 byte block, ten times
	runrepeated("rfc 4634 4",
		(const byte_t *)"0123456701234567012345670123456701234567"
				"012345670123456701234567",64,10,
		"594847328451bdfa85056225462cc1d867d877fb"
		"388df0ce35f25ab5562bfbb5");

	// digests confirmed against coreutils sha256sum
	runstring("hello world",
		"hello world",
		"b94d27b9934d3e08a52e52d7da7dabfac484efe3"
		"7a5380ee9088f7ace2efcde9");
	runstring("goodbye world",
		"goodbye world",
		"9150e02727e29ca8522c29ad4aa5a8343c21ccf9"
		"09b40f73c41bf478df7e6fc3");

	// the same data, one append() at a time
	runchunks("streaming, 1 byte",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",1,
		"248d6a61d20638b8e5c026930c3e6039a33ce459"
		"64ff2167f6ecedd419db06c1");
	runchunks("streaming, 10 bytes",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",10,
		"248d6a61d20638b8e5c026930c3e6039a33ce459"
		"64ff2167f6ecedd419db06c1");
	runchunks("streaming, 55 bytes",
		"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",55,
		"248d6a61d20638b8e5c026930c3e6039a33ce459"
		"64ff2167f6ecedd419db06c1");
	runchunks("streaming, hello world",
		"hello world",1,
		"b94d27b9934d3e08a52e52d7da7dabfac484efe3"
		"7a5380ee9088f7ace2efcde9");

	// clear() has to put the instance back where it started
	sha256	reused;
	reused.append((const byte_t *)"hello world",11);
	reused.getHash();
	reused.clear();
	bool	ok=reused.append((const byte_t *)"abc",3);
	checkhash("reuse after clear",&reused,ok,
		"ba7816bf8f01cfea414140de5dae2223b00361a3"
		"96177a9cb410ff61f20015ad");

	stdoutput.printf("\n");

	return 0;
}
