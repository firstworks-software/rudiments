#include <rudiments/crypt.h>
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

	crypt	c;
	for (const char * const *salt=salts; *salt; salt++) {

		stdoutput.printf("salt=%s\n",*salt);

		for (const char * const *str=strings; *str; str++) {

			const unsigned char	*data=
					(const unsigned char *)*str;
			const unsigned char	*iv=
					(const unsigned char *)*salt;

			c.append(data,charstring::length(*str));
			c.setIv(iv,c.getIvSize());

			const char	*encrypted=
					(const char *)c.getEncryptedData();

			stdoutput.printf("  %s: %s\n",*str,encrypted);

			c.clear();
		}
	}
}
