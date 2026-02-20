#include <rudiments/md5.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	md5	h;

	// check whether md5 is supported
	if (!h.isSupported()) {
		stdoutput.write("md5 is not supported\n");
		return 1;
	}


	// hash some data
	const char	*data="hello world";
	h.append((const byte_t *)data,charstring::getLength(data));

	// print the hash
	const byte_t	*hashval=h.getHash();
	uint64_t	hashsize=h.getHashSize();

	stdoutput.printf("md5(\"%s\") = ",data);
	for (uint64_t i=0; i<hashsize; i++) {
		stdoutput.printf("%02x",hashval[i]);
	}
	stdoutput.write('\n');


	// clear and hash different data
	h.clear();

	data="goodbye world";
	h.append((const byte_t *)data,charstring::getLength(data));

	hashval=h.getHash();
	hashsize=h.getHashSize();

	stdoutput.printf("md5(\"%s\") = ",data);
	for (uint64_t i=0; i<hashsize; i++) {
		stdoutput.printf("%02x",hashval[i]);
	}
	stdoutput.write('\n');
}
