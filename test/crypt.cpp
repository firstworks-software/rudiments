// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("crypt");

	char	*osname=sys::getOperatingSystemName();
	if (charstring::compare(osname,"Windows") &&
		charstring::compare(osname,"OpenBSD")) {

		const char	*salts[]={
			"FW",
			"fw",
			NULL
		};

		const char	*unencrypted[]={
			"rudiments",
			"RUDIMENTS",
			"firstworks",
			"FIRSTWORKS",
			NULL
		};

		const char	*encrypted[][4]={
			{
			"FWR60yTxIvBZo",
			"FWlt9zHgfs4j2",
			"FW8vxm0D2fdZQ",
			"FWBVR0cwLQHEw",
			},
			{
			"fwdzejOvuN/mc",
			"fwNiqldtGLMeI",
			"fwaWWmdtSjxcA",
			"fwDHL5QynIMNg",
			}
		};

		crypt	c;
		uint16_t	i=0;
		for (const char * const *salt=salts; *salt; salt++) {

			stdoutput.printf("salt=%s\n",*salt);

			uint16_t	j=0;
			for (const char * const *str=unencrypted; *str; str++) {
				c.append((const unsigned char *)*str,
						charstring::length(*str));
				c.setIv((const unsigned char *)*salt,
							c.getIvSize());
				const char	*enc=
					(const char *)c.getEncryptedData();
				test(*str,!charstring::compare(enc,
							encrypted[i][j]));
				test("size",
					c.getEncryptedDataSize()==
					charstring::length(encrypted[i][j]));
				c.clear();
				j++;
			}
			i++;
		}

	} else {

		stdoutput.printf("not supported\n");
	}

	stdoutput.printf("\n");

	delete[] osname;
}
