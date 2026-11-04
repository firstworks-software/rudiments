// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/aes192.h>
#include <rudiments/prng.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("aes192");

	aes192	a;
	aes192	b;
	
	// key and iv
	a.setRandomKey();
	a.setRandomIv();
	b.setKey(a.getKey(),a.getKeySize());
	b.setIv(a.getIv(),a.getIvSize());
	size_t		keysize=a.getKeySize();
	test("keysize",keysize==24);
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

	// setUsePadding() - used by e.g. Oracle's O5LOGON protocol: AES-CBC,
	// zero IV, no padding, block-aligned data only
	byte_t	zeroiv[16];
	bytestring::zero(zeroiv,sizeof(zeroiv));
	byte_t	blockaligned[32];
	for (uint8_t i=0; i<sizeof(blockaligned); i++) {
		blockaligned[i]=(byte_t)i;
	}

	// padding off: block-aligned data round-trips with no length change
	aes192	padoff;
	aes192	padoffb;
	padoff.setRandomKey();
	padoff.setIv(zeroiv,sizeof(zeroiv));
	padoff.setUsePadding(false);
	test("padding off",!padoff.getUsePadding());
	padoffb.setKey(padoff.getKey(),padoff.getKeySize());
	padoffb.setIv(zeroiv,sizeof(zeroiv));
	padoffb.setUsePadding(false);
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

	// padding on (the default, unchanged from before setUsePadding()
	// existed) still round-trips, and grows the ciphertext by a block,
	// unlike the padding-off case above
	aes192	padon;
	aes192	padonb;
	padon.setRandomKey();
	padon.setIv(zeroiv,sizeof(zeroiv));
	test("padding on by default",padon.getUsePadding());
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
	aes192	padoffbad;
	padoffbad.setKey(padoff.getKey(),padoff.getKeySize());
	padoffbad.setIv(zeroiv,sizeof(zeroiv));
	padoffbad.setUsePadding(false);
	byte_t	notblockaligned[17];
	bytestring::zero(notblockaligned,sizeof(notblockaligned));
	padoffbad.append(notblockaligned,sizeof(notblockaligned));
	test("padding off: non-block-aligned data fails",
		padoffbad.getEncryptedData()==NULL &&
		padoffbad.getError()==ENCRYPTION_ERROR_INVALID_PADDING);
	stdoutput.printf("\n");

	// a zero block with padding off ends in a 0 byte, which padding on
	// must reject as a pad count
	aes192	zeropadenc;
	zeropadenc.setKey(padoff.getKey(),padoff.getKeySize());
	zeropadenc.setIv(zeroiv,sizeof(zeroiv));
	zeropadenc.setUsePadding(false);
	byte_t	zeroblock[16];
	bytestring::zero(zeroblock,sizeof(zeroblock));
	test("zero pad byte: encrypt append",
		zeropadenc.append(zeroblock,sizeof(zeroblock)));
	const byte_t	*zeropadct=zeropadenc.getEncryptedData();
	uint64_t	zeropadctsize=zeropadenc.getEncryptedDataSize();
	test("zero pad byte: encrypted data",zeropadct!=NULL);
	aes192	zeropaddec;
	zeropaddec.setKey(padoff.getKey(),padoff.getKeySize());
	zeropaddec.setIv(zeroiv,sizeof(zeroiv));
	zeropaddec.append(zeropadct,zeropadctsize);
	test("zero pad byte: decrypt rejected",
		zeropaddec.getUsePadding() &&
		zeropaddec.getDecryptedData()==NULL &&
		zeropaddec.getError()!=ENCRYPTION_ERROR_SUCCESS);
	stdoutput.printf("\n");

	// a block with padding off that ends in a 2 byte after a 0xAA byte
	// has a bad pad byte, which padding on must reject
	aes192	badpadenc;
	badpadenc.setKey(padoff.getKey(),padoff.getKeySize());
	badpadenc.setIv(zeroiv,sizeof(zeroiv));
	badpadenc.setUsePadding(false);
	byte_t	badpadblock[16];
	bytestring::set(badpadblock,0xAA,sizeof(badpadblock));
	badpadblock[sizeof(badpadblock)-1]=2;
	test("bad pad byte: encrypt append",
		badpadenc.append(badpadblock,sizeof(badpadblock)));
	const byte_t	*badpadct=badpadenc.getEncryptedData();
	uint64_t	badpadctsize=badpadenc.getEncryptedDataSize();
	test("bad pad byte: encrypted data",badpadct!=NULL);
	aes192	badpaddec;
	badpaddec.setKey(padoff.getKey(),padoff.getKeySize());
	badpaddec.setIv(zeroiv,sizeof(zeroiv));
	badpaddec.append(badpadct,badpadctsize);
	test("bad pad byte: decrypt rejected",
		badpaddec.getUsePadding() &&
		badpaddec.getDecryptedData()==NULL &&
		badpaddec.getError()!=ENCRYPTION_ERROR_SUCCESS);
	stdoutput.printf("\n");

	// a block with padding off that ends in three 3 bytes holds valid
	// padding, which padding on must remove
	aes192	multipadenc;
	multipadenc.setKey(padoff.getKey(),padoff.getKeySize());
	multipadenc.setIv(zeroiv,sizeof(zeroiv));
	multipadenc.setUsePadding(false);
	byte_t	multipadblock[16];
	bytestring::set(multipadblock,0xAA,sizeof(multipadblock));
	bytestring::set(multipadblock+sizeof(multipadblock)-3,3,3);
	test("multi-byte pad: encrypt append",
		multipadenc.append(multipadblock,sizeof(multipadblock)));
	const byte_t	*multipadct=multipadenc.getEncryptedData();
	uint64_t	multipadctsize=multipadenc.getEncryptedDataSize();
	test("multi-byte pad: encrypted data",multipadct!=NULL);
	aes192	multipaddec;
	multipaddec.setKey(padoff.getKey(),padoff.getKeySize());
	multipaddec.setIv(zeroiv,sizeof(zeroiv));
	multipaddec.append(multipadct,multipadctsize);
	const byte_t	*multipadpt=multipaddec.getDecryptedData();
	test("multi-byte pad: decrypt removes padding",
		multipadpt!=NULL &&
		multipaddec.getDecryptedDataSize()==sizeof(multipadblock)-3 &&
		!bytestring::compare(multipadpt,multipadblock,
					sizeof(multipadblock)-3));
	stdoutput.printf("\n");

	// clear() keeps the key and iv, so decrypting with no input must
	// fail instead of decrypting stale data
	aes192	emptyenc;
	emptyenc.setKey(padoff.getKey(),padoff.getKeySize());
	emptyenc.setIv(zeroiv,sizeof(zeroiv));
	emptyenc.append(zeroblock,0);
	const byte_t	*emptyct=emptyenc.getEncryptedData();
	uint64_t	emptyctsize=emptyenc.getEncryptedDataSize();
	test("empty input: pad block encrypted",
		emptyct!=NULL && emptyctsize==16);
	aes192	emptydec;
	emptydec.setKey(padoff.getKey(),padoff.getKeySize());
	emptydec.setIv(zeroiv,sizeof(zeroiv));
	emptydec.append(emptyct,emptyctsize);
	test("empty input: valid ciphertext decrypts",
		emptydec.getDecryptedData()!=NULL &&
		emptydec.getDecryptedDataSize()==0);
	emptydec.clear();
	test("empty input: decrypt after clear rejected",
		emptydec.getDecryptedData()==NULL &&
		emptydec.getError()!=ENCRYPTION_ERROR_SUCCESS);
	aes192	emptyfresh;
	emptyfresh.setKey(padoff.getKey(),padoff.getKeySize());
	emptyfresh.setIv(zeroiv,sizeof(zeroiv));
	test("empty input: fresh object rejected",
		emptyfresh.getDecryptedData()==NULL &&
		emptyfresh.getError()!=ENCRYPTION_ERROR_SUCCESS);

	// padding off: no input decrypts to empty output
	aes192	emptypadoff;
	emptypadoff.setKey(padoff.getKey(),padoff.getKeySize());
	emptypadoff.setIv(zeroiv,sizeof(zeroiv));
	emptypadoff.setUsePadding(false);
	test("empty input: padding off decrypts empty",
		emptypadoff.getDecryptedData()!=NULL &&
		emptypadoff.getDecryptedDataSize()==0 &&
		emptypadoff.getError()==ENCRYPTION_ERROR_SUCCESS);
	stdoutput.printf("\n");

	prng	rn;
	int32_t		seed=prng::getSeed();
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
