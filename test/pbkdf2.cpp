// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/pbkdf2.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// runs one PBKDF2 test vector and compares the derived key to "expected"
static void runvector(const char *name, pbkdf2algorithm_t alg,
			const char *password, const char *salt,
			uint32_t iterations, uint32_t keysize,
			const byte_t *expected) {

	pbkdf2	p;
	p.setAlgorithm(alg);
	p.setIterations(iterations);
	p.setKeySize(keysize);
	p.setSalt((const byte_t *)salt,charstring::getLength(salt));
	p.append((const byte_t *)password,charstring::getLength(password));

	const byte_t	*key=p.getHash();

	// a NULL key means this algorithm isn't available (eg. sha-256 on
	// pre-1.0.0 openssl) - skip rather than fail
	if (!key) {
		stdoutput.printf("%s: not supported\n",name);
		return;
	}

	test(name,p.getHashSize()==keysize &&
			!bytestring::compare(key,expected,keysize));
}

int main(int argc, const char **argv) {

	header("pbkdf2");

	pbkdf2	supported;
	if (!supported.isSupported()) {
		stdoutput.printf("not supported\n\n");
		return 0;
	}

	// RFC 6070 PBKDF2-HMAC-SHA1 test vectors
	static const byte_t	sha1_1[]={
		0x0c,0x60,0xc8,0x0f,0x96,0x1f,0x0e,0x71,0xf3,0xa9,
		0xb5,0x24,0xaf,0x60,0x12,0x06,0x2f,0xe0,0x37,0xa6
	};
	runvector("sha1 c=1",PBKDF2_ALGORITHM_SHA1,
			"password","salt",1,sizeof(sha1_1),sha1_1);

	static const byte_t	sha1_2[]={
		0xea,0x6c,0x01,0x4d,0xc7,0x2d,0x6f,0x8c,0xcd,0x1e,
		0xd9,0x2a,0xce,0x1d,0x41,0xf0,0xd8,0xde,0x89,0x57
	};
	runvector("sha1 c=2",PBKDF2_ALGORITHM_SHA1,
			"password","salt",2,sizeof(sha1_2),sha1_2);

	static const byte_t	sha1_4096[]={
		0x4b,0x00,0x79,0x01,0xb7,0x65,0x48,0x9a,0xbe,0xad,
		0x49,0xd9,0x26,0xf7,0x21,0xd0,0x65,0xa4,0x29,0xc1
	};
	runvector("sha1 c=4096",PBKDF2_ALGORITHM_SHA1,
			"password","salt",4096,sizeof(sha1_4096),sha1_4096);

	// PBKDF2-HMAC-SHA256 test vectors (skipped where sha-256 is
	// unavailable)
	static const byte_t	sha256_1[]={
		0x12,0x0f,0xb6,0xcf,0xfc,0xf8,0xb3,0x2c,
		0x43,0xe7,0x22,0x52,0x56,0xc4,0xf8,0x37,
		0xa8,0x65,0x48,0xc9,0x2c,0xcc,0x35,0x48,
		0x08,0x05,0x98,0x7c,0xb7,0x0b,0xe1,0x7b
	};
	runvector("sha256 c=1",PBKDF2_ALGORITHM_SHA256,
			"password","salt",1,sizeof(sha256_1),sha256_1);

	static const byte_t	sha256_2[]={
		0xae,0x4d,0x0c,0x95,0xaf,0x6b,0x46,0xd3,
		0x2d,0x0a,0xdf,0xf9,0x28,0xf0,0x6d,0xd0,
		0x2a,0x30,0x3f,0x8e,0xf3,0xc2,0x51,0xdf,
		0xd6,0xe2,0xd8,0x5a,0x95,0x47,0x4c,0x43
	};
	runvector("sha256 c=2",PBKDF2_ALGORITHM_SHA256,
			"password","salt",2,sizeof(sha256_2),sha256_2);

	stdoutput.printf("\n");

	return 0;
}
