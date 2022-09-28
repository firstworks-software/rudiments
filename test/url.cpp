// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/url.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/permissions.h>
#include <rudiments/snooze.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("url");
// disabled for now #7122
return 0;

	char	*testcontents=file::getContents("knowncontent.html");

	const char	*urlname=
			"http://rudiments.sourceforge.net/knowncontent.html";

	url	u;
	for (uint16_t i=0; i<4; i++) {

		u.useHttpGet();
		if (i%2) {
			u.setHttpUserAgent("unknown/1.0");
			u.setHttpHeaders("Foo: foo\r\n"
					"Bar: bar\r\n"
					"Baz: baz\r\n");
		} else {
			u.setHttpUserAgent(NULL);
			u.setHttpHeaders(NULL);
		}

		if (i%2) {
			test("close",u.close());
		}
		test("open",u.open(urlname,O_RDONLY|O_BINARY));
		char	*contents=u.getContents();
		test("get contents",
			!charstring::compare(testcontents,contents));
		delete[] contents;

		if (i%2) {
			test("close",u.close());
		}
		test("open",u.open(urlname,O_RDONLY));
		const char	*ptr=testcontents;
		char	buffer[1024];
		for (;;) {
			ssize_t	s=u.read(buffer,sizeof(buffer));
			test("chunk",!charstring::compare(ptr,buffer,s));
			if (s<(ssize_t)sizeof(buffer)) {
				break;
			}
			ptr+=s;
		}
	}

	stdoutput.printf("\n");

	return 0;
}
