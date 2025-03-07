// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/file.h>

int main(int argc, const char **argv) {

	if (argc<2) {
		stdoutput.printf("usage: md5sum filename\n");
		return 0;
	}

	file	f;
	if (!f.open(argv[1],O_RDONLY)) {
		return 1;
	}

	byte_t	*contents=(byte_t *)f.getContents();

	md5	m;
	m.append(contents,f.getSize());
	char	*str=charstring::hexEncode(m.getHash(),m.getHashSize());
	stdoutput.printf("%s\n",str);
	delete[] str;

	delete[] contents;

	return 0;
}
