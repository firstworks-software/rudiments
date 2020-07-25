// Copyright (c) 1999-2018 David Muse
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
	unsigned char	*blankkey=new unsigned char[keysize];
	bytestring::zero(blankkey,keysize);
	unsigned char	*key=a.getKey();
	test("sane key",bytestring::compare(key,blankkey,keysize));
	size_t		ivsize=a.getIvSize();
	test("ivsize",ivsize==16);
	unsigned char	*blankiv=new unsigned char[ivsize];
	bytestring::zero(blankiv,ivsize);
	unsigned char	*iv=a.getIv();
	test("sane iv",bytestring::compare(iv,blankiv,ivsize));
	stdoutput.printf("\n");

	randomnumber	rn;
	int32_t		seed=randomnumber::getSeed();
	rn.setSeed(seed);

	// for 2^0 through 2^24...
	bytebuffer	unenc;
	uint64_t	size=0;
	while (size<=16777216) {

		stdoutput.printf("size: %lld\n",size);

		// reset
		a.clear();
		b.clear();

		// fill a buffer with random data
		unenc.clear();
		for (uint64_t i=0; i<size; i++) {
			rn.generateScaledNumber(0,255,&seed);
			unenc.append((unsigned char)seed);
		}

		// encrypt
		test("encrypt append",
			a.append(unenc.getBuffer(),unenc.getSize()));
		const unsigned char	*enc=a.getEncryptedData();
		uint64_t		enclen=a.getDataLength();
		test("sane encrypted data",enc!=NULL);
		test("sane encrypted length",enclen);

		// decrypt
		test ("decrypt append",b.append(enc,enclen));
		const unsigned char	*dec=b.getDecryptedData();
		uint64_t		declen=b.getDataLength();
		test("sane decrypted data",dec!=NULL);
		test("sane decrypted length",(size)?declen:!declen);

		test("valid decrypted length",
			declen==unenc.getSize());
		test("valid decrypted data",
			!bytestring::compare(dec,unenc.getBuffer(),declen));

		// bump size
		#define GROW_EXP 1
		#if GROW_EXP==1
			if (size) {
				size*=2;
			} else {
				size=2;
			}
		#else
			size++;
		#endif
	
		stdoutput.printf("\n");
	}
}
