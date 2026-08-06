// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/des.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("des");

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

		class des	c;
		uint16_t	i=0;
		for (const char * const *salt=salts; *salt; salt++) {

			stdoutput.printf("salt=%s\n",*salt);
			c.setSalt((const byte_t *)*salt,
					c.getRequiredSaltSize());

			uint16_t	j=0;
			for (const char * const *str=unencrypted; *str; str++) {
				c.append((const byte_t *)*str,
						charstring::getLength(*str));
				const char	*enc=(const char *)c.getHash();
				test(*str,!charstring::compare(enc,
							encrypted[i][j]));
				test("size",c.getHashSize()==
					charstring::getLength(encrypted[i][j]));
				c.clear();
				j++;
			}
			c.reset();
			i++;
		}

		// reset() drops the salt, clear() keeps it
		class des	s;
		s.setSalt((const byte_t *)"FW",s.getRequiredSaltSize());
		s.clear();
		test("clear keeps the salt",s.getSalt()!=NULL);
		s.reset();
		test("reset drops the salt",s.getSalt()==NULL);

	} else {

		stdoutput.printf("not supported\n");
	}

	stdoutput.printf("\n");

	delete[] osname;

	return 0;
}
