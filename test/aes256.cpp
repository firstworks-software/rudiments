// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/aes256.h>
#include <rudiments/randomnumber.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("aes256");

	aes256	a;
	aes256	b;
	
	// key and iv
	a.setRandomKey();
	a.setRandomIv();
	b.setKey(a.getKey(),a.getKeySize());
	b.setIv(a.getIv(),a.getIvSize());
	size_t		keysize=a.getKeySize();
	test("keysize",keysize==32);
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

	// setPadding() - used by e.g. Oracle's O5LOGON protocol: AES-CBC,
	// zero IV, no padding, block-aligned data only
	byte_t	zeroiv[16];
	bytestring::zero(zeroiv,sizeof(zeroiv));
	byte_t	blockaligned[32];
	for (uint8_t i=0; i<sizeof(blockaligned); i++) {
		blockaligned[i]=(byte_t)i;
	}

	// padding off: block-aligned data round-trips with no length change
	aes256	padoff;
	aes256	padoffb;
	padoff.setRandomKey();
	padoff.setIv(zeroiv,sizeof(zeroiv));
	padoff.setPadding(false);
	test("padding off",!padoff.getPadding());
	padoffb.setKey(padoff.getKey(),padoff.getKeySize());
	padoffb.setIv(zeroiv,sizeof(zeroiv));
	padoffb.setPadding(false);
	test("padding off: encrypt append",
		padoff.append(blockaligned,sizeof(blockaligned)));
	const byte_t	*encna=padoff.getEncryptedData();
	uint64_t	encnasize=padoff.getEncryptedDataSize();
	test("padding off: encrypted data",encna!=NULL);
	test("padding off: encrypted size unchanged",
			encnasize==sizeof(blockaligned));
	test("padding off: decrypt append",padoffb.append(encna,encnasize));
	const byte_t	*decna=padoffb.getDecryptedData();
	uint64_t	decnasize=padoffb.getDecryptedDataSize();
	test("padding off: decrypted data",decna!=NULL);
	test("padding off: decrypted size unchanged",
			decnasize==sizeof(blockaligned));
	test("padding off: round trip matches",
		!bytestring::compare(decna,blockaligned,sizeof(blockaligned)));

	// padding on (the default, unchanged from before setPadding()
	// existed) still round-trips, and grows the ciphertext by a block,
	// unlike the padding-off case above
	aes256	padon;
	aes256	padonb;
	padon.setRandomKey();
	padon.setIv(zeroiv,sizeof(zeroiv));
	test("padding on by default",padon.getPadding());
	padonb.setKey(padon.getKey(),padon.getKeySize());
	padonb.setIv(zeroiv,sizeof(zeroiv));
	test("padding on: encrypt append",
		padon.append(blockaligned,sizeof(blockaligned)));
	const byte_t	*encpad=padon.getEncryptedData();
	uint64_t	encpadsize=padon.getEncryptedDataSize();
	test("padding on: encrypted data",encpad!=NULL);
	test("padding on: encrypted size grows",encpadsize>sizeof(blockaligned));
	test("padding on: decrypt append",padonb.append(encpad,encpadsize));
	const byte_t	*decpad=padonb.getDecryptedData();
	uint64_t	decpadsize=padonb.getDecryptedDataSize();
	test("padding on: round trip matches",
		decpadsize==sizeof(blockaligned) &&
		!bytestring::compare(decpad,blockaligned,sizeof(blockaligned)));

	// padding off: a non-block-aligned buffer must fail
	aes256	padoffbad;
	padoffbad.setKey(padoff.getKey(),padoff.getKeySize());
	padoffbad.setIv(zeroiv,sizeof(zeroiv));
	padoffbad.setPadding(false);
	byte_t	notblockaligned[17];
	bytestring::zero(notblockaligned,sizeof(notblockaligned));
	padoffbad.append(notblockaligned,sizeof(notblockaligned));
	test("padding off: non-block-aligned data fails",
		padoffbad.getEncryptedData()==NULL &&
		padoffbad.getError()==ENCRYPTION_ERROR_INVALID_PADDING);
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
