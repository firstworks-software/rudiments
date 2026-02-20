#include <rudiments/pbkdf2.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	pbkdf2	p;

	// check whether pbkdf2 is supported
	if (!p.isSupported()) {
		stdoutput.write("pbkdf2 is not supported\n");
		return 1;
	}


	// configure pbkdf2
	p.setAlgorithm(PBKDF2_ALGORITHM_SHA256);
	p.setIterations(10000);
	p.setKeySize(32);

	// set a salt
	const byte_t	*salt=(const byte_t *)"somesalt";
	p.setSalt(salt,8);

	// derive a key from a password
	const char	*password="hello world";
	p.append((const byte_t *)password,charstring::getLength(password));

	// print the derived key in hexadecimal
	const byte_t	*hashval=p.getHash();
	uint64_t	hashsize=p.getHashSize();

	stdoutput.printf("pbkdf2(\"%s\") = ",password);
	for (uint64_t i=0; i<hashsize; i++) {
		stdoutput.printf("%02x",hashval[i]);
	}
	stdoutput.write('\n');


	// clear and derive from a different password
	p.clear();

	password="goodbye world";
	p.append((const byte_t *)password,charstring::getLength(password));

	hashval=p.getHash();
	hashsize=p.getHashSize();

	stdoutput.printf("pbkdf2(\"%s\") = ",password);
	for (uint64_t i=0; i<hashsize; i++) {
		stdoutput.printf("%02x",hashval[i]);
	}
	stdoutput.write('\n');


	// derive with a different algorithm
	p.reset();

	p.setAlgorithm(PBKDF2_ALGORITHM_SHA512);
	p.setIterations(10000);
	p.setKeySize(64);
	p.setSalt(salt,8);

	password="hello world";
	p.append((const byte_t *)password,charstring::getLength(password));

	hashval=p.getHash();
	hashsize=p.getHashSize();

	stdoutput.printf("pbkdf2-sha512(\"%s\") = ",password);
	for (uint64_t i=0; i<hashsize; i++) {
		stdoutput.printf("%02x",hashval[i]);
	}
	stdoutput.write('\n');
}
