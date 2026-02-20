#include <rudiments/des.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	des	h;

	// check whether des is supported
	if (!h.isSupported()) {
		stdoutput.write("des is not supported\n");
		return 1;
	}


	// set a 2-character salt
	const byte_t	*salt=(const byte_t *)"ab";
	h.setSalt(salt,h.getRequiredSaltSize());

	// hash some data
	const char	*data="hello world";
	h.append((const byte_t *)data,charstring::getLength(data));

	// print the hash (des output is printable ascii)
	stdoutput.printf("des(\"%s\") = %s\n",data,(const char *)h.getHash());


	// clear and hash different data
	h.clear();

	data="goodbye world";
	h.append((const byte_t *)data,charstring::getLength(data));

	stdoutput.printf("des(\"%s\") = %s\n\n",data,
						(const char *)h.getHash());


	// hash with a different salt
	h.clear();

	salt=(const byte_t *)"xy";
	h.setSalt(salt,h.getRequiredSaltSize());

	data="hello world";
	h.append((const byte_t *)data,charstring::getLength(data));

	stdoutput.printf("des(\"%s\") with salt \"xy\" = %s\n",
					data,(const char *)h.getHash());
}
