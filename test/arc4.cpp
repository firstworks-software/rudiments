// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/arc4.h>
#include <rudiments/bytestring.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("arc4");

	test("isSupported",arc4().isSupported());

	// published RC4 test vectors (key, plaintext, expected ciphertext)
	{
		const byte_t	key[]="Key";
		const byte_t	pt[]="Plaintext";
		const byte_t	ct[]={
			0xbb,0xf3,0x16,0xe8,0xd9,0x40,0xaf,0x0a,0xd3
		};
		byte_t	buf[sizeof(pt)-1];
		bytestring::copy(buf,pt,sizeof(buf));
		arc4	a(key,sizeof(key)-1);
		test("vector \"Key\"/\"Plaintext\": crypt",a.crypt(buf,sizeof(buf)));
		test("vector \"Key\"/\"Plaintext\": matches",
			!bytestring::compare(buf,ct,sizeof(buf)));
	}
	{
		const byte_t	key[]="Wiki";
		const byte_t	pt[]="pedia";
		const byte_t	ct[]={0x10,0x21,0xbf,0x04,0x20};
		byte_t	buf[sizeof(pt)-1];
		bytestring::copy(buf,pt,sizeof(buf));
		arc4	a(key,sizeof(key)-1);
		test("vector \"Wiki\"/\"pedia\": crypt",a.crypt(buf,sizeof(buf)));
		test("vector \"Wiki\"/\"pedia\": matches",
			!bytestring::compare(buf,ct,sizeof(buf)));
	}
	{
		const byte_t	key[]="Secret";
		const byte_t	pt[]="Attack at dawn";
		const byte_t	ct[]={
			0x45,0xa0,0x1f,0x64,0x5f,0xc3,0x5b,0x38,
			0x35,0x52,0x54,0x4b,0x9b,0xf5
		};
		byte_t	buf[sizeof(pt)-1];
		bytestring::copy(buf,pt,sizeof(buf));
		arc4	a(key,sizeof(key)-1);
		test("vector \"Secret\"/\"Attack at dawn\": crypt",
			a.crypt(buf,sizeof(buf)));
		test("vector \"Secret\"/\"Attack at dawn\": matches",
			!bytestring::compare(buf,ct,sizeof(buf)));
	}

	// RFC 6229 keystream vectors: encrypting an all-zero buffer produces
	// the keystream itself
	{
		const byte_t	key[]={0x01,0x02,0x03,0x04,0x05};
		const byte_t	ks[]={
			0xb2,0x39,0x63,0x05,0xf0,0x3d,0xc0,0x27,
			0xcc,0xc3,0x52,0x4a,0x0a,0x11,0x18,0xa8
		};
		byte_t	buf[sizeof(ks)];
		bytestring::zero(buf,sizeof(buf));
		arc4	a(key,sizeof(key));
		test("RFC 6229 40-bit key: crypt",a.crypt(buf,sizeof(buf)));
		test("RFC 6229 40-bit key: matches",
			!bytestring::compare(buf,ks,sizeof(buf)));
	}
	{
		const byte_t	key[]={
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
			0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10
		};
		const byte_t	ks[]={
			0x9a,0xc7,0xcc,0x9a,0x60,0x9d,0x1e,0xf7,
			0xb2,0x93,0x28,0x99,0xcd,0xe4,0x1b,0x97
		};
		byte_t	buf[sizeof(ks)];
		bytestring::zero(buf,sizeof(buf));
		arc4	a(key,sizeof(key));
		test("RFC 6229 128-bit key: crypt",a.crypt(buf,sizeof(buf)));
		test("RFC 6229 128-bit key: matches",
			!bytestring::compare(buf,ks,sizeof(buf)));
	}

	// crypt() is its own inverse: encrypting then decrypting with fresh,
	// identically-keyed instances round-trips
	{
		const byte_t	key[]="round trip key";
		const byte_t	pt[]="the quick brown fox jumps over the lazy dog";
		byte_t	buf[sizeof(pt)-1];
		bytestring::copy(buf,pt,sizeof(buf));
		arc4	enc(key,sizeof(key)-1);
		test("round trip: encrypt",enc.crypt(buf,sizeof(buf)));
		test("round trip: ciphertext differs",
			bytestring::compare(buf,pt,sizeof(buf)));
		arc4	dec(key,sizeof(key)-1);
		test("round trip: decrypt",dec.crypt(buf,sizeof(buf)));
		test("round trip: matches original",
			!bytestring::compare(buf,pt,sizeof(buf)));
	}

	// repeated crypt() calls continue the same keystream as one big call
	{
		const byte_t	key[]="running keystream";
		byte_t	whole[32];
		byte_t	split[32];
		for (uint8_t i=0; i<sizeof(whole); i++) {
			whole[i]=(byte_t)i;
			split[i]=(byte_t)i;
		}
		arc4	a(key,sizeof(key)-1);
		test("running keystream: whole crypt",a.crypt(whole,sizeof(whole)));
		arc4	b(key,sizeof(key)-1);
		test("running keystream: split crypt 1",b.crypt(split,7));
		test("running keystream: split crypt 2",b.crypt(split+7,11));
		test("running keystream: split crypt 3",
			b.crypt(split+18,sizeof(split)-18));
		test("running keystream: matches",
			!bytestring::compare(whole,split,sizeof(whole)));
	}

	// two independent instances constructed from the same key run
	// concurrently without interfering with each other, the way a duplex
	// stream uses one instance per direction
	{
		const byte_t	key[]="duplex stream key";
		byte_t	outbound1[16];
		byte_t	outbound2[16];
		for (uint8_t i=0; i<sizeof(outbound1); i++) {
			outbound1[i]=(byte_t)(i+1);
			outbound2[i]=(byte_t)(i+1);
		}
		byte_t	inbound1[16];
		byte_t	inbound2[16];
		for (uint8_t i=0; i<sizeof(inbound1); i++) {
			inbound1[i]=(byte_t)(0xff-i);
			inbound2[i]=(byte_t)(0xff-i);
		}

		// a lone pair, run start-to-finish with no interleaving,
		// used as the expected result below
		arc4	expectedout(key,sizeof(key)-1);
		test("duplex: expected outbound crypt",
			expectedout.crypt(outbound1,sizeof(outbound1)));
		arc4	expectedin(key,sizeof(key)-1);
		test("duplex: expected inbound crypt",
			expectedin.crypt(inbound1,sizeof(inbound1)));

		// two instances from the same key, interleaved, simulating
		// concurrent use on the two directions of a duplex stream
		arc4	writecipher(key,sizeof(key)-1);
		arc4	readcipher(key,sizeof(key)-1);
		test("duplex: write 1",writecipher.crypt(outbound2,8));
		test("duplex: read 1",readcipher.crypt(inbound2,8));
		test("duplex: write 2",writecipher.crypt(outbound2+8,8));
		test("duplex: read 2",readcipher.crypt(inbound2+8,8));

		test("duplex: outbound matches independent of interleaving",
			!bytestring::compare(outbound1,outbound2,sizeof(outbound1)));
		test("duplex: inbound matches independent of interleaving",
			!bytestring::compare(inbound1,inbound2,sizeof(inbound1)));
	}

	// crypt() fails cleanly when no key has been set
	{
		arc4	nokey;
		byte_t	buf[4]={0,1,2,3};
		test("no key: crypt fails",!nokey.crypt(buf,sizeof(buf)));
	}

	// setKey() re-initializes the keystream, discarding prior state
	{
		const byte_t	key[]="rekey test";
		byte_t	first[8]={0,1,2,3,4,5,6,7};
		byte_t	second[8]={0,1,2,3,4,5,6,7};
		arc4	a(key,sizeof(key)-1);
		test("rekey: first crypt",a.crypt(first,4));
		test("rekey: setKey resets",a.setKey(key,sizeof(key)-1));
		test("rekey: second crypt from start",a.crypt(second,4));
		test("rekey: matches a fresh instance",
			!bytestring::compare(first,second,4));
	}

	return 0;
}
