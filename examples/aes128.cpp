#include <rudiments/aes128.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	aes128	a;

	// check whether aes128 is supported
	if (!a.isSupported()) {
		stdoutput.write("aes128 is not supported\n");
		return 1;
	}


	// set a random key and initialization vector
	a.setRandomKey();
	a.setRandomIv();


	// encrypt some data
	const char	*data="hello world";
	a.append((const byte_t *)data,charstring::getLength(data));

	const byte_t	*enc=a.getEncryptedData();
	uint64_t	encsize=a.getEncryptedDataSize();

	// print the encrypted data in hexadecimal
	stdoutput.printf("encrypted: ");
	for (uint64_t i=0; i<encsize; i++) {
		stdoutput.printf("%02x",enc[i]);
	}
	stdoutput.write('\n');


	// decrypt using the same key and initialization vector
	aes128	b;
	b.setKey(a.getKey(),a.getKeySize());
	b.setIv(a.getIv(),a.getIvSize());

	b.append(enc,encsize);

	const byte_t	*dec=b.getDecryptedData();
	uint64_t	decsize=b.getDecryptedDataSize();

	stdoutput.printf("decrypted: %.*s\n",decsize,dec);


	// clear and encrypt different data
	a.clear();

	data="goodbye world";
	a.append((const byte_t *)data,charstring::getLength(data));

	enc=a.getEncryptedData();
	encsize=a.getEncryptedDataSize();

	stdoutput.printf("encrypted: ");
	for (uint64_t i=0; i<encsize; i++) {
		stdoutput.printf("%02x",enc[i]);
	}
	stdoutput.write('\n');

	// decrypt
	b.clear();
	b.append(enc,encsize);

	dec=b.getDecryptedData();
	decsize=b.getDecryptedDataSize();

	stdoutput.printf("decrypted: %.*s\n",decsize,dec);
}
