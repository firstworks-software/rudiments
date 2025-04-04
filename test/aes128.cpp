// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/aes128.h>
#include <rudiments/randomnumber.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("aes128");

	aes128	a;
	aes128	b;
	
	// key and iv
	a.setRandomKey();
	a.setRandomIv();
	b.setKey(a.getKey(),a.getKeySize());
	b.setIv(a.getIv(),a.getIvSize());
	size_t		keysize=a.getKeySize();
	test("keysize",keysize==16);
	byte_t	*blankkey=new byte_t[keysize];
	bytestring::zero(blankkey,keysize);
	byte_t	*key=a.getKey();
	test("sane key",bytestring::compare(key,blankkey,keysize));
	size_t		ivsize=a.getIvSize();
	test("ivsize",ivsize==16);
	byte_t	*blankiv=new byte_t[ivsize];
	bytestring::zero(blankiv,ivsize);
	byte_t	*iv=a.getIv();
	test("sane iv",bytestring::compare(iv,blankiv,ivsize));
	stdoutput.printf("\n");

	randomnumber	rn;
	int32_t		seed=randomnumber::getSeed();
	rn.setSeed(seed);

	// for 2^0 through 2^23...
	bytebuffer	unenc;
#ifdef SLOWSYSTEM
	// skip the first iteration on slow systems
	for (uint16_t i=1; i<2; i++) {
#else
	for (uint16_t i=0; i<2; i++) {
#endif
		uint64_t	size=0;
		for (;;) {

			stdoutput.printf("size: %lld\n",size);

			// reset
			a.clear();
			b.clear();

			// fill a buffer with random data
			unenc.clear();
			for (uint64_t j=0; j<size; j++) {
				rn.generate(&seed,0,255);
				unenc.append((byte_t)seed);
			}

			// encrypt
			test("encrypt append",
				a.append(unenc.getBuffer(),unenc.getSize()));
			const byte_t	*enc=a.getEncryptedData();
			uint64_t	encsize=a.getEncryptedDataSize();
			test("sane encrypted data",enc!=NULL);
			test("sane encrypted size",encsize);

			// decrypt
			test ("decrypt append",b.append(enc,encsize));
			const byte_t	*dec=b.getDecryptedData();
			uint64_t	decsize=b.getDecryptedDataSize();
			test("sane decrypted data",dec!=NULL);
			test("sane decrypted size",(size)?decsize:!decsize);

			test("valid decrypted size",
					decsize==unenc.getSize());
			test("valid decrypted data",
					!bytestring::compare(dec,
							unenc.getBuffer(),
							decsize));
			stdoutput.printf("\n");

			// bump size...
			// increment during first iteration,
			// square during second iteration
			if (!i) {
				size++;
				if (size>4096) {
					break;
				}
			} else {
				if (size) {
					size*=2;
				} else {
					size=2;
				}
				if (size>8388608) {
					break;
				}
			}
		}
	}

	return 0;
}
