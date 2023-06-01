// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/iconvert.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>

#include "test.cpp"

int main(int argc, char **argv) {

	// define some text strings
	const char	*hellochar="Hello!";
	const wchar_t	*hellowchar=L"Hello!";
	ucs2literal(helloucs2char,"Hello!");

	// big or little endian
	const char	*ucs2=(sys::isBigEndian())?"UCS-2BE":"UCS-2LE";

	// some stuff to reuse
	iconvert	i;
	size_t		s;
	char		*c;
	wchar_t		*wc;
	ucs2_t		*uc;


	// wchar to char
	stdoutput.printf("wchar to char:\n");
	s=wcharstring::getSize(hellowchar);
	c=new char[s];
	i.setFromEncoding("WCHAR_T");
	i.setFromBuffer((const byte_t *)hellowchar);
	i.setFromBufferSize(s);
	i.setToBuffer((byte_t *)c);
	i.setToBufferSize(s);
	test("convert",i.convert());
	test("match",!charstring::compare(c,hellochar));
	delete[] c;
	stdoutput.printf("\n");

	// ucs2 to char
	stdoutput.printf("ucs2 to char:\n");
	s=ucs2charstring::getSize(helloucs2char);
	c=new char[s];
	i.setFromEncoding(ucs2);
	i.setFromBuffer((const byte_t *)helloucs2char);
	i.setFromBufferSize(s);
	i.setToBuffer((byte_t *)c);
	i.setToBufferSize(s);
	test("convert",i.convert());
	test("match",!charstring::compare(c,hellochar));
	delete[] c;
	stdoutput.printf("\n");


	// char to wchar
	stdoutput.printf("char to wchar:\n");
	s=charstring::getSize(hellochar);
	wc=new wchar_t[s];
	i.setFromEncoding("");
	i.setFromBuffer((const byte_t *)hellochar);
	i.setFromBufferSize(s);
	i.setToEncoding("WCHAR_T");
	i.setToBuffer((byte_t *)wc);
	i.setToBufferSize(s*sizeof(wchar_t));
	test("convert",i.convert());
	test("match",!wcharstring::compare(wc,hellowchar));
	delete[] wc;
	stdoutput.printf("\n");

	// ucs2 to wchar
	stdoutput.printf("ucs2 to char:\n");
	s=ucs2charstring::getSize(helloucs2char);
	wc=new wchar_t[s];
	i.setFromEncoding(ucs2);
	i.setFromBuffer((const byte_t *)helloucs2char);
	i.setFromBufferSize(s);
	i.setToEncoding("WCHAR_T");
	i.setToBuffer((byte_t *)wc);
	i.setToBufferSize(s*sizeof(wchar_t));
	test("convert",i.convert());
	test("match",!wcharstring::compare(wc,hellowchar));
	delete[] wc;
	stdoutput.printf("\n");


	// char to ucs2
	stdoutput.printf("char to ucs2:\n");
	s=charstring::getSize(hellochar);
	uc=new ucs2_t[s];
	i.setFromEncoding("");
	i.setFromBuffer((const byte_t *)hellochar);
	i.setFromBufferSize(s);
	i.setToEncoding(ucs2);
	i.setToBuffer((byte_t *)uc);
	i.setToBufferSize(s*sizeof(ucs2_t));
	test("convert",i.convert());
	test("match",!ucs2charstring::compare(uc,helloucs2char));
	delete[] uc;
	stdoutput.printf("\n");

	// wchar to ucs2
	stdoutput.printf("wchar to ucs2:\n");
	s=wcharstring::getSize(hellowchar);
	uc=new ucs2_t[s];
	i.setFromEncoding("WCHAR_T");
	i.setFromBuffer((const byte_t *)hellowchar);
	i.setFromBufferSize(s);
	i.setToEncoding(ucs2);
	i.setToBuffer((byte_t *)uc);
	i.setToBufferSize(s*sizeof(ucs2_t));
	test("convert",i.convert());
	test("match",!ucs2charstring::compare(uc,helloucs2char));
	delete[] uc;
	stdoutput.printf("\n");
}
