// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/tripledes.h>
#include <rudiments/singledes.h>
#include <rudiments/sha1.h>
#include <rudiments/bytestring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// derives a 24-byte 3des-ede key from an arbitrary-length input and a fixed
// entropy blob: the first 20 bytes are sha1(input+entropy), and the last 4
// bytes are the first 4 bytes of sha1(input+0x02+digest[1:20]+entropy) -
// this is the o3logon key fold-in used to protect both the session key and
// the password on the wire
static void createKeySHA1(const byte_t *input, uint32_t inputlen,
				const byte_t *entropy, uint32_t entropylen,
				byte_t *out24) {

	sha1	first;
	first.append(input,inputlen);
	first.append(entropy,entropylen);
	byte_t	digest1[20];
	bytestring::copy(digest1,first.getHash(),20);

	byte_t	tag=0x02;
	sha1	second;
	second.append(input,inputlen);
	second.append(&tag,1);
	second.append(digest1+1,19);
	second.append(entropy,entropylen);

	bytestring::copy(out24,digest1,20);
	bytestring::copy(out24+20,second.getHash(),4);
}

int main(int argc, const char **argv) {

	header("tripledes");

	tripledes	t;
	test("keysize",t.getKeySize()==24);
	test("ivsize",t.getIvSize()==8);
	stdoutput.printf("\n");

	// NIST SP 800-67 TECB known-answer vector: a 24-byte key (three
	// 8-byte des keys), applied as ecb (no chaining) to 24 bytes of
	// plaintext (three independent blocks), no padding.  the published
	// plaintext hex is well known to decode to slightly garbled ascii
	// (a single-character typo where "quick" would need an 'i') - that's
	// part of the widely circulated vector itself, not a transcription
	// error here, so it's used as hex, not as a string literal
	byte_t	*k1=charstring::hexDecode("0123456789ABCDEF");
	byte_t	*k2=charstring::hexDecode("23456789ABCDEF01");
	byte_t	*k3=charstring::hexDecode("456789ABCDEF0123");
	byte_t	nistkey[24];
	bytestring::copy(nistkey,k1,8);
	bytestring::copy(nistkey+8,k2,8);
	bytestring::copy(nistkey+16,k3,8);
	delete[] k1;
	delete[] k2;
	delete[] k3;

	byte_t	*nistplaintext=charstring::hexDecode(
		"54686520717566636B2062726F776E20666F78206A756D70");
	byte_t	*nistciphertext=charstring::hexDecode(
		"A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900");

	tripledes	nist;
	nist.setKey(nistkey,24);
	nist.setBlockCipherMode(BLOCK_CIPHER_MODE_ECB);
	nist.setUsePadding(false);

	// ecb is only available where the platform provides it - the
	// built-in fallback implementation only does cbc
	if (nist.isSupported()) {

		test("nist tecb: append",nist.append(nistplaintext,24));
		const byte_t	*nistenc=nist.getEncryptedData();
		test("nist tecb: encrypted data",nistenc!=NULL);
		test("nist tecb: encrypted size",
			nist.getEncryptedDataSize()==24);
		test("nist tecb: matches known answer",
			!bytestring::compare(nistenc,nistciphertext,24));

	} else {

		stdoutput.printf("	nist tecb: not supported\n");
	}

	delete[] nistplaintext;
	delete[] nistciphertext;
	stdoutput.printf("\n");

	// k1=k2=k3 collapses ede (encrypt/decrypt/encrypt under the same
	// key) down to a single encrypt, so 3des-cbc under a 24-byte key
	// built from one 8-byte key repeated three times must produce
	// exactly what singledes produces under that 8-byte key - a
	// structural check on the ede chaining itself
	byte_t	samekey[8];
	for (uint8_t i=0; i<8; i++) {
		samekey[i]=(byte_t)(0x11*(i+1));
	}
	byte_t	treskey[24];
	bytestring::copy(treskey,samekey,8);
	bytestring::copy(treskey+8,samekey,8);
	bytestring::copy(treskey+16,samekey,8);

	byte_t	collapseiv[8];
	for (uint8_t i=0; i<8; i++) {
		collapseiv[i]=i;
	}
	byte_t	collapsedata[16];
	for (uint8_t i=0; i<16; i++) {
		collapsedata[i]=(byte_t)(i*7);
	}

	tripledes	collapse3;
	collapse3.setKey(treskey,24);
	collapse3.setIv(collapseiv,8);
	collapse3.setUsePadding(false);
	test("k1=k2=k3: 3des append",
		collapse3.append(collapsedata,16));
	const byte_t	*collapse3enc=collapse3.getEncryptedData();
	test("k1=k2=k3: 3des encrypted data",collapse3enc!=NULL);

	singledes	collapse1;
	collapse1.setKey(samekey,8);
	collapse1.setIv(collapseiv,8);
	collapse1.setUsePadding(false);
	test("k1=k2=k3: des append",
		collapse1.append(collapsedata,16));
	const byte_t	*collapse1enc=collapse1.getEncryptedData();
	test("k1=k2=k3: des encrypted data",collapse1enc!=NULL);

	test("k1=k2=k3: 3des matches single des",
		!bytestring::compare(collapse3enc,collapse1enc,16));
	stdoutput.printf("\n");

	// the o3logon key fold-in, exercised against real captured oracle
	// wire bytes: a fixed non-zero cbc iv and two fixed entropy blobs
	// key two rounds of 3des-ede-cbc (padding off) - the first recovers
	// the session key from auth_sesskey, the second recovers an
	// obfuscated password buffer from auth_password
	byte_t	*o3iv=charstring::hexDecode("8020400408021001");
	byte_t	*entropysesskey=charstring::hexDecode(
		"A2FBE6AD4C7D1E3D6EB0B76C97EFFF84"
		"4471"
		"0284ACF13B295C0F0CB18775EF");
	byte_t	*entropypassword=charstring::hexDecode(
		"F2FF9787153707760727E27FA3B1D6733F2FD152ABACC0");
	byte_t	*passwordhash=charstring::hexDecode("29B64D9CB5625E3A");
	byte_t	*wiresesskey=charstring::hexDecode(
		"CE34C0E3F3E9D2ED0ADA50474587B44F");
	byte_t	*wirepassword=charstring::hexDecode(
		"0EC13F49D13A727E31ED00E3377B089636D521D63699048D");

	// derive tdk1 from the oracle password hash and recover the
	// session key from the wire auth_sesskey
	byte_t	tdk1[24];
	createKeySHA1(passwordhash,8,entropysesskey,31,tdk1);

	tripledes	sesskeydec;
	sesskeydec.setKey(tdk1,24);
	sesskeydec.setIv(o3iv,8);
	sesskeydec.setUsePadding(false);
	test("o3logon: sesskey decrypt append",
		sesskeydec.append(wiresesskey,16));
	const byte_t	*sesskey=sesskeydec.getDecryptedData();
	test("o3logon: sesskey decrypted",sesskey!=NULL);
	test("o3logon: sesskey size",sesskeydec.getDecryptedDataSize()==16);

	// derive tdk2 from the recovered session key and recover the
	// obfuscated password buffer from the wire auth_password
	byte_t	tdk2[24];
	createKeySHA1(sesskey,16,entropypassword,23,tdk2);

	tripledes	passworddec;
	passworddec.setKey(tdk2,24);
	passworddec.setIv(o3iv,8);
	passworddec.setUsePadding(false);
	test("o3logon: password decrypt append",
		passworddec.append(wirepassword,24));
	const byte_t	*obf=passworddec.getDecryptedData();
	test("o3logon: password decrypted",obf!=NULL);
	test("o3logon: password size",
		passworddec.getDecryptedDataSize()==24);

	// undo the rotation: the clear password's first 4 bytes were moved
	// to the end of the buffer as a throwaway salt
	byte_t	deobf[24];
	bytestring::copy(deobf,obf+20,4);
	bytestring::copy(deobf+4,obf+4,20);
	test("o3logon: recovered password",
		!bytestring::compare(deobf,"testpassword",12));

	// and the reverse direction: re-encrypting the recovered session
	// key and obfuscated password must reproduce the exact wire bytes
	tripledes	sesskeyenc;
	sesskeyenc.setKey(tdk1,24);
	sesskeyenc.setIv(o3iv,8);
	sesskeyenc.setUsePadding(false);
	test("o3logon: sesskey encrypt append",
		sesskeyenc.append(sesskey,16));
	const byte_t	*sesskeyroundtrip=sesskeyenc.getEncryptedData();
	test("o3logon: sesskey re-encrypt matches wire bytes",
		sesskeyroundtrip!=NULL &&
		!bytestring::compare(sesskeyroundtrip,wiresesskey,16));

	tripledes	passwordenc;
	passwordenc.setKey(tdk2,24);
	passwordenc.setIv(o3iv,8);
	passwordenc.setUsePadding(false);
	test("o3logon: password encrypt append",
		passwordenc.append(obf,24));
	const byte_t	*passwordroundtrip=passwordenc.getEncryptedData();
	test("o3logon: password re-encrypt matches wire bytes",
		passwordroundtrip!=NULL &&
		!bytestring::compare(passwordroundtrip,wirepassword,24));

	delete[] o3iv;
	delete[] entropysesskey;
	delete[] entropypassword;
	delete[] passwordhash;
	delete[] wiresesskey;
	delete[] wirepassword;
	stdoutput.printf("\n");

	// cbc is available everywhere - the built-in fallback implementation
	// provides it when the platform doesn't
	tripledes	cbc;
	cbc.setBlockCipherMode(BLOCK_CIPHER_MODE_CBC);
	test("cbc: isSupported",cbc.isSupported());
	stdoutput.printf("\n");

	// padding on (the default): a 20-byte plaintext, not a multiple of
	// the block size, encrypts to 24 bytes and round trips
	byte_t	padiv[8];
	for (uint8_t i=0; i<8; i++) {
		padiv[i]=(byte_t)(0x10+i);
	}
	const char	*padplaintext="the quick brown fox!";

	tripledes	padenc;
	padenc.setKey(nistkey,24);
	padenc.setIv(padiv,8);
	test("padding on by default",padenc.getUsePadding());
	test("padding on: encrypt append",
		padenc.append((const byte_t *)padplaintext,20));
	const byte_t	*padenc_out=padenc.getEncryptedData();
	uint64_t	padenc_outsize=padenc.getEncryptedDataSize();
	test("padding on: encrypted data",padenc_out!=NULL);
	test("padding on: encrypted size",padenc_outsize==24);

	tripledes	paddec;
	paddec.setKey(nistkey,24);
	paddec.setIv(padiv,8);
	test("padding on: decrypt append",
		paddec.append(padenc_out,padenc_outsize));
	const byte_t	*paddec_out=paddec.getDecryptedData();
	test("padding on: decrypted data",paddec_out!=NULL);
	test("padding on: round trip matches",
		paddec.getDecryptedDataSize()==20 &&
		!bytestring::compare(paddec_out,padplaintext,20));
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
		bytestring::copy(truncatediv,padiv,sizeof(padiv));
		truncatediv[0]=(byte_t)ivbyte;

		tripledes	truncsrc;
		truncsrc.setKey(nistkey,24);
		truncsrc.setIv(truncatediv,8);
		truncsrc.append((const byte_t *)padplaintext,20);
		const byte_t	*truncct=truncsrc.getEncryptedData();
		uint64_t	trunctsize=truncsrc.getEncryptedDataSize();

		for (uint64_t truncsize=1; truncsize<trunctsize; truncsize++) {

			if (!(truncsize%8)) {
				continue;
			}

			tripledes	truncdec;
			truncdec.setKey(nistkey,24);
			truncdec.setIv(truncatediv,8);
			truncdec.append(truncct,truncsize);
			if (!truncdec.getUsePadding() ||
				truncdec.getDecryptedData()!=NULL ||
				truncdec.getError()==
					ENCRYPTION_ERROR_SUCCESS) {
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
	stdoutput.printf("\n");

	// setKey() must reject anything but the correct key size
	tripledes	badkey;
	byte_t		wrongsizekey[16];
	bytestring::zero(wrongsizekey,16);
	test("setKey rejects wrong size",!badkey.setKey(wrongsizekey,16));
	test("setKey wrong size error",
		badkey.getError()==ENCRYPTION_ERROR_INVALID_KEY_SIZE);
	stdoutput.printf("\n");

	return 0;
}
