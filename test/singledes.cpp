// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/singledes.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include "test.cpp"

// computes the Oracle O3LOGON (pre-11g) password hash: uppercase the
// user/password pair, encode as UTF-16BE, zero-pad to a multiple of 8 bytes,
// DES-CBC encrypt under a fixed key with a zero iv, then DES-CBC encrypt the
// same data again under the last block of that first pass; the last block of
// the second pass is the 8-byte hash
static void oracleHash(const char *username, const char *password,
						byte_t *hash) {

	char	userpass[256];
	charstring::copy(userpass,username);
	charstring::append(userpass,password);
	charstring::upper(userpass);

	bytebuffer	input;
	size_t	len=charstring::getLength(userpass);
	for (size_t i=0; i<len; i++) {
		input.append((byte_t)0);
		input.append((byte_t)userpass[i]);
	}
	while (input.getSize()%8) {
		input.append((byte_t)0);
	}

	byte_t	zeroiv[8];
	bytestring::zero(zeroiv,sizeof(zeroiv));
	byte_t	fixedkey[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};

	// first pass
	singledes	pass1;
	pass1.setKey(fixedkey,sizeof(fixedkey));
	pass1.setIv(zeroiv,sizeof(zeroiv));
	pass1.setUsePadding(false);
	pass1.append(input.getBuffer(),input.getSize());
	const byte_t	*out1=pass1.getEncryptedData();
	uint64_t	out1size=pass1.getEncryptedDataSize();

	// zero the hash if the first pass failed, so the caller's
	// comparison against the golden vector fails rather than crashing
	if (!out1 || out1size<8) {
		bytestring::zero(hash,8);
		return;
	}

	// second pass, keyed with the last block of the first pass
	singledes	pass2;
	pass2.setKey(out1+out1size-8,8);
	pass2.setIv(zeroiv,sizeof(zeroiv));
	pass2.setUsePadding(false);
	pass2.append(input.getBuffer(),input.getSize());
	const byte_t	*out2=pass2.getEncryptedData();
	uint64_t	out2size=pass2.getEncryptedDataSize();

	if (!out2 || out2size<8) {
		bytestring::zero(hash,8);
		return;
	}

	bytestring::copy(hash,out2+out2size-8,8);
}

int main(int argc, const char **argv) {

	header("singledes");

	singledes	a;
	singledes	b;

	// key and iv
	a.setRandomKey();
	a.setRandomIv();
	b.setKey(a.getKey(),a.getKeySize());
	b.setIv(a.getIv(),a.getIvSize());
	size_t		keysize=a.getKeySize();
	test("keysize",keysize==8);
	size_t		ivsize=a.getIvSize();
	test("ivsize",ivsize==8);
	stdoutput.printf("\n");

	// FIPS 81 DES-CBC known-answer vector: key 0123456789abcdef,
	// iv 1234567890abcdef, plaintext "Now is the time for all " (24
	// bytes, an exact multiple of the block size)
	byte_t	fipskey[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	byte_t	fipsiv[8]={0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef};
	const char	*fipsplaintext="Now is the time for all ";
	byte_t	fipsciphertext[24]={
		0xe5,0xc7,0xcd,0xde,0x87,0x2b,0xf2,0x7c,
		0x43,0xe9,0x34,0x00,0x8c,0x38,0x9c,0x0f,
		0x68,0x37,0x88,0x49,0x9a,0x7c,0x05,0xf6
	};

	// padding off: raw-block encrypt of the exact-multiple-of-8
	// plaintext reproduces the FIPS 81 ciphertext exactly
	singledes	fipsenc;
	fipsenc.setKey(fipskey,sizeof(fipskey));
	fipsenc.setIv(fipsiv,sizeof(fipsiv));
	fipsenc.setUsePadding(false);
	test("fips81: encrypt append",
		fipsenc.append((const byte_t *)fipsplaintext,
					charstring::getLength(fipsplaintext)));
	const byte_t	*fipsenc_out=fipsenc.getEncryptedData();
	uint64_t	fipsenc_outsize=fipsenc.getEncryptedDataSize();
	test("fips81: encrypted data",fipsenc_out!=NULL);
	test("fips81: encrypted size",fipsenc_outsize==sizeof(fipsciphertext));
	test("fips81: matches known-answer ciphertext",
		!bytestring::compare(fipsenc_out,fipsciphertext,
						sizeof(fipsciphertext)));

	// padding off: decrypting the known-answer ciphertext round-trips
	// back to the original plaintext
	singledes	fipsdec;
	fipsdec.setKey(fipskey,sizeof(fipskey));
	fipsdec.setIv(fipsiv,sizeof(fipsiv));
	fipsdec.setUsePadding(false);
	test("fips81: decrypt append",
		fipsdec.append(fipsciphertext,sizeof(fipsciphertext)));
	const byte_t	*fipsdec_out=fipsdec.getDecryptedData();
	uint64_t	fipsdec_outsize=fipsdec.getDecryptedDataSize();
	test("fips81: decrypted data",fipsdec_out!=NULL);
	test("fips81: decrypted size",
		fipsdec_outsize==charstring::getLength(fipsplaintext));
	test("fips81: round trip matches",
		!bytestring::compare(fipsdec_out,fipsplaintext,fipsdec_outsize));
	stdoutput.printf("\n");

	// padding on (the default): the same plaintext, still an exact
	// multiple of 8, gets a full extra block of CMS padding
	byte_t	fipspaddedciphertext[32]={
		0xe5,0xc7,0xcd,0xde,0x87,0x2b,0xf2,0x7c,
		0x43,0xe9,0x34,0x00,0x8c,0x38,0x9c,0x0f,
		0x68,0x37,0x88,0x49,0x9a,0x7c,0x05,0xf6,
		0x62,0xc1,0x6a,0x27,0xe4,0xfc,0xf2,0x77
	};
	singledes	padenc;
	padenc.setKey(fipskey,sizeof(fipskey));
	padenc.setIv(fipsiv,sizeof(fipsiv));
	test("padding on by default",padenc.getUsePadding());
	test("padding on: encrypt append",
		padenc.append((const byte_t *)fipsplaintext,
					charstring::getLength(fipsplaintext)));
	const byte_t	*padenc_out=padenc.getEncryptedData();
	uint64_t	padenc_outsize=padenc.getEncryptedDataSize();
	test("padding on: encrypted data",padenc_out!=NULL);
	test("padding on: matches known-answer ciphertext",
		padenc_outsize==sizeof(fipspaddedciphertext) &&
		!bytestring::compare(padenc_out,fipspaddedciphertext,
						sizeof(fipspaddedciphertext)));
	singledes	paddec;
	paddec.setKey(fipskey,sizeof(fipskey));
	paddec.setIv(fipsiv,sizeof(fipsiv));
	test("padding on: decrypt append",
		paddec.append(fipspaddedciphertext,
					sizeof(fipspaddedciphertext)));
	const byte_t	*paddec_out=paddec.getDecryptedData();
	uint64_t	paddec_outsize=paddec.getDecryptedDataSize();
	test("padding on: decrypted data",paddec_out!=NULL);
	test("padding on: round trip matches",
		paddec_outsize==charstring::getLength(fipsplaintext) &&
		!bytestring::compare(paddec_out,fipsplaintext,paddec_outsize));
	stdoutput.printf("\n");

	// Decryption always operates on whole blocks, so a ciphertext that
	// isn't a multiple of the block size must be rejected, even with
	// padding on - otherwise the block-decrypt step reads past the end
	// of the input and returns garbage.  Every partial-block length,
	// under a range of iv's, is checked: decrypting a partial block
	// anyway only sometimes lands on a pad byte big enough for the
	// padding check to catch, so a single case would pass by luck.
	bool	truncrejected=true;
	for (int32_t ivbyte=0; ivbyte<32; ivbyte++) {

		byte_t	truncatediv[8];
		bytestring::copy(truncatediv,fipsiv,sizeof(fipsiv));
		truncatediv[0]=(byte_t)ivbyte;

		singledes	truncsrc;
		truncsrc.setKey(fipskey,sizeof(fipskey));
		truncsrc.setIv(truncatediv,sizeof(truncatediv));
		truncsrc.append((const byte_t *)fipsplaintext,
					charstring::getLength(fipsplaintext));
		const byte_t	*truncct=truncsrc.getEncryptedData();
		uint64_t	trunctsize=truncsrc.getEncryptedDataSize();

		for (uint64_t truncsize=1; truncsize<trunctsize; truncsize++) {

			if (!(truncsize%8)) {
				continue;
			}

			singledes	truncdec;
			truncdec.setKey(fipskey,sizeof(fipskey));
			truncdec.setIv(truncatediv,sizeof(truncatediv));
			truncdec.append(truncct,truncsize);
			if (!truncdec.getUsePadding() ||
				truncdec.getDecryptedData()!=NULL ||
				truncdec.getError()!=
					ENCRYPTION_ERROR_INVALID_PADDING) {
				truncrejected=false;
				stdoutput.printf("	iv[0]=%02x truncated "
							"to %d not rejected\n",
							ivbyte,
							(int32_t)truncsize);
			}
		}
	}
	test("truncated: every partial-block ciphertext rejected",
							truncrejected);

	// encryption with padding on accepts any length, though
	singledes	truncenc;
	truncenc.setKey(fipskey,sizeof(fipskey));
	truncenc.setIv(fipsiv,sizeof(fipsiv));
	test("truncated: encrypt append",
		truncenc.append((const byte_t *)fipsplaintext,20));
	test("truncated: getEncryptedData not null",
		truncenc.getEncryptedData()!=NULL);
	test("truncated: encrypted size",
		truncenc.getEncryptedDataSize()==24);
	stdoutput.printf("\n");

	// a zero block with padding off ends in a 0 byte, which padding on
	// must reject as a pad count
	singledes	zeropadenc;
	zeropadenc.setKey(fipskey,sizeof(fipskey));
	zeropadenc.setIv(fipsiv,sizeof(fipsiv));
	zeropadenc.setUsePadding(false);
	byte_t	zeroblock[8];
	bytestring::zero(zeroblock,sizeof(zeroblock));
	test("zero pad byte: encrypt append",
		zeropadenc.append(zeroblock,sizeof(zeroblock)));
	const byte_t	*zeropadct=zeropadenc.getEncryptedData();
	uint64_t	zeropadctsize=zeropadenc.getEncryptedDataSize();
	test("zero pad byte: encrypted data",zeropadct!=NULL);
	singledes	zeropaddec;
	zeropaddec.setKey(fipskey,sizeof(fipskey));
	zeropaddec.setIv(fipsiv,sizeof(fipsiv));
	zeropaddec.append(zeropadct,zeropadctsize);
	test("zero pad byte: decrypt rejected",
		zeropaddec.getUsePadding() &&
		zeropaddec.getDecryptedData()==NULL &&
		zeropaddec.getError()==ENCRYPTION_ERROR_INVALID_PADDING);
	stdoutput.printf("\n");

	// a block with padding off that ends in a 2 byte after a 0xAA byte
	// has a bad pad byte, which padding on must reject
	singledes	badpadenc;
	badpadenc.setKey(fipskey,sizeof(fipskey));
	badpadenc.setIv(fipsiv,sizeof(fipsiv));
	badpadenc.setUsePadding(false);
	byte_t	badpadblock[8];
	bytestring::set(badpadblock,0xAA,sizeof(badpadblock));
	badpadblock[sizeof(badpadblock)-1]=2;
	test("bad pad byte: encrypt append",
		badpadenc.append(badpadblock,sizeof(badpadblock)));
	const byte_t	*badpadct=badpadenc.getEncryptedData();
	uint64_t	badpadctsize=badpadenc.getEncryptedDataSize();
	test("bad pad byte: encrypted data",badpadct!=NULL);
	singledes	badpaddec;
	badpaddec.setKey(fipskey,sizeof(fipskey));
	badpaddec.setIv(fipsiv,sizeof(fipsiv));
	badpaddec.append(badpadct,badpadctsize);
	test("bad pad byte: decrypt rejected",
		badpaddec.getUsePadding() &&
		badpaddec.getDecryptedData()==NULL &&
		badpaddec.getError()==ENCRYPTION_ERROR_INVALID_PADDING);
	stdoutput.printf("\n");

	// a block with padding off that ends in three 3 bytes holds valid
	// padding, which padding on must remove
	singledes	multipadenc;
	multipadenc.setKey(fipskey,sizeof(fipskey));
	multipadenc.setIv(fipsiv,sizeof(fipsiv));
	multipadenc.setUsePadding(false);
	byte_t	multipadblock[8];
	bytestring::set(multipadblock,0xAA,sizeof(multipadblock));
	bytestring::set(multipadblock+sizeof(multipadblock)-3,3,3);
	test("multi-byte pad: encrypt append",
		multipadenc.append(multipadblock,sizeof(multipadblock)));
	const byte_t	*multipadct=multipadenc.getEncryptedData();
	uint64_t	multipadctsize=multipadenc.getEncryptedDataSize();
	test("multi-byte pad: encrypted data",multipadct!=NULL);
	singledes	multipaddec;
	multipaddec.setKey(fipskey,sizeof(fipskey));
	multipaddec.setIv(fipsiv,sizeof(fipsiv));
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
	singledes	emptyenc;
	emptyenc.setKey(fipskey,sizeof(fipskey));
	emptyenc.setIv(fipsiv,sizeof(fipsiv));
	emptyenc.append(zeroblock,0);
	const byte_t	*emptyct=emptyenc.getEncryptedData();
	uint64_t	emptyctsize=emptyenc.getEncryptedDataSize();
	test("empty input: pad block encrypted",
		emptyct!=NULL && emptyctsize==8);
	singledes	emptydec;
	emptydec.setKey(fipskey,sizeof(fipskey));
	emptydec.setIv(fipsiv,sizeof(fipsiv));
	emptydec.append(emptyct,emptyctsize);
	test("empty input: valid ciphertext decrypts",
		emptydec.getDecryptedData()!=NULL &&
		emptydec.getDecryptedDataSize()==0);
	emptydec.clear();
	test("empty input: decrypt after clear rejected",
		emptydec.getDecryptedData()==NULL &&
		emptydec.getError()==ENCRYPTION_ERROR_INVALID_PADDING);
	singledes	emptyfresh;
	emptyfresh.setKey(fipskey,sizeof(fipskey));
	emptyfresh.setIv(fipsiv,sizeof(fipsiv));
	test("empty input: fresh object rejected",
		emptyfresh.getDecryptedData()==NULL &&
		emptyfresh.getError()==ENCRYPTION_ERROR_INVALID_PADDING);

	// padding off: no input decrypts to empty output
	singledes	emptypadoff;
	emptypadoff.setKey(fipskey,sizeof(fipskey));
	emptypadoff.setIv(fipsiv,sizeof(fipsiv));
	emptypadoff.setUsePadding(false);
	test("empty input: padding off decrypts empty",
		emptypadoff.getDecryptedData()!=NULL &&
		emptypadoff.getDecryptedDataSize()==0 &&
		emptypadoff.getError()==ENCRYPTION_ERROR_SUCCESS);
	stdoutput.printf("\n");

	// the Oracle O3LOGON password hash, checked against two public
	// golden vectors
	byte_t	systemhash[8];
	oracleHash("SYSTEM","MANAGER",systemhash);
	byte_t	systemexpected[8]={0xd4,0xdf,0x79,0x31,0xab,0x13,0x0e,0x37};
	test("oracle hash: SYSTEM/MANAGER",
		!bytestring::compare(systemhash,systemexpected,8));

	byte_t	scotthash[8];
	oracleHash("SCOTT","TIGER",scotthash);
	byte_t	scottexpected[8]={0xf8,0x94,0x84,0x4c,0x34,0x40,0x2b,0x67};
	test("oracle hash: SCOTT/TIGER",
		!bytestring::compare(scotthash,scottexpected,8));
	stdoutput.printf("\n");

	// DES only examines the top 7 bits of each key byte; a key with its
	// low (parity) bits flipped is still accepted and produces identical
	// ciphertext
	byte_t	goodkey[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
	byte_t	oddparitykey[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xee};
	byte_t	zeroiv[8];
	bytestring::zero(zeroiv,sizeof(zeroiv));
	byte_t	block[8]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};

	singledes	goodkeyenc;
	test("parity: good-parity key accepted",
		goodkeyenc.setKey(goodkey,sizeof(goodkey)));
	goodkeyenc.setIv(zeroiv,sizeof(zeroiv));
	goodkeyenc.setUsePadding(false);
	test("parity: good-parity key encrypt",
		goodkeyenc.append(block,sizeof(block)));
	const byte_t	*goodkeyout=goodkeyenc.getEncryptedData();
	test("parity: good-parity key encrypted data",goodkeyout!=NULL);

	singledes	oddparitykeyenc;
	test("parity: flipped-parity key accepted",
		oddparitykeyenc.setKey(oddparitykey,sizeof(oddparitykey)));
	oddparitykeyenc.setIv(zeroiv,sizeof(zeroiv));
	oddparitykeyenc.setUsePadding(false);
	test("parity: flipped-parity key encrypt",
		oddparitykeyenc.append(block,sizeof(block)));
	const byte_t	*oddparitykeyout=oddparitykeyenc.getEncryptedData();
	test("parity: flipped-parity key encrypted data",
					oddparitykeyout!=NULL);

	test("parity: both keys produce identical ciphertext",
		!bytestring::compare(goodkeyout,oddparitykeyout,sizeof(block)));
	stdoutput.printf("\n");

	// this implementation doesn't reject DES's published weak keys.
	// 0101010101010101 is one of them; all 16 of its round keys are
	// identical, which makes encryption its own inverse, so with a zero
	// iv a single block encrypted twice comes back to the original block
	byte_t	weakkey[8]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};

	singledes	weakkeyenc;
	test("weak key: accepted",
		weakkeyenc.setKey(weakkey,sizeof(weakkey)));
	weakkeyenc.setIv(zeroiv,sizeof(zeroiv));
	weakkeyenc.setUsePadding(false);
	test("weak key: encrypt",weakkeyenc.append(block,sizeof(block)));
	const byte_t	*weakkeyout=weakkeyenc.getEncryptedData();
	test("weak key: encrypted data",weakkeyout!=NULL);
	test("weak key: ciphertext differs from plaintext",
		bytestring::compare(weakkeyout,block,sizeof(block))!=0);

	singledes	weakkeyenc2;
	weakkeyenc2.setKey(weakkey,sizeof(weakkey));
	weakkeyenc2.setIv(zeroiv,sizeof(zeroiv));
	weakkeyenc2.setUsePadding(false);
	test("weak key: re-encrypt",weakkeyenc2.append(weakkeyout,8));
	const byte_t	*weakkeyout2=weakkeyenc2.getEncryptedData();
	test("weak key: encrypting twice restores the plaintext",
		weakkeyout2!=NULL &&
		!bytestring::compare(weakkeyout2,block,sizeof(block)));
	stdoutput.printf("\n");

	// only CBC is supported; any other block cipher mode is rejected
	singledes	unsupported;
	unsupported.setKey(goodkey,sizeof(goodkey));
	unsupported.setIv(zeroiv,sizeof(zeroiv));
	unsupported.setBlockCipherMode(BLOCK_CIPHER_MODE_ECB);
	test("unsupported mode: isSupported false",
		!unsupported.isSupported());
	unsupported.append(block,sizeof(block));
	test("unsupported mode: getEncryptedData null",
		unsupported.getEncryptedData()==NULL);
	test("unsupported mode: error set",
		unsupported.getError()==ENCRYPTION_ERROR_UNSUPPORTED);
	stdoutput.printf("\n");

	return 0;
}
