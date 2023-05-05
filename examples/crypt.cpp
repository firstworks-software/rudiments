#include <rudiments/des.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char * const salts[]={
		"FW",
		"fw",
		NULL
	};

	const char * const strings[]={
		"rudiments",
		"RUDIMENTS",
		"firstworks",
		"FIRSTWORKS",
		NULL
	};

	des	c;
	for (const char * const *salt=salts; *salt; salt++) {

		stdoutput.printf("salt=%s\n",*salt);

		for (const char * const *str=strings; *str; str++) {

			const byte_t	*data=(const byte_t *)*str;
			const byte_t	*iv=(const byte_t *)*salt;

			c.append(data,charstring::getLength(*str));
			c.setSalt(iv,c.getSaltSize());

			const char	*encrypted=(const char *)c.getHash();

			stdoutput.printf("  %s: %s\n",*str,encrypted);

			c.clear();
		}
	}
}
