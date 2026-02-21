#include <rudiments/iconvert.h>
#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// define some text in various encodings...
	const char	*hellochar="Hello!";
	const wchar_t	*hellowchar=L"Hello!";
	ucs2literal(helloucs2,"Hello!");

	// determine endianness for UCS-2 encoding name
	const char	*ucs2=(sys::isBigEndian())?"UCS-2BE":"UCS-2LE";

	iconvert	ic;


	// convert char to wchar_t...
	size_t	fromsize=charstring::getSize(hellochar);
	size_t	tosize=fromsize*sizeof(wchar_t);
	wchar_t	*wc=new wchar_t[fromsize];

	ic.setFromEncoding("");
	ic.setToEncoding("WCHAR_T");
	ic.setFromBuffer((const byte_t *)hellochar);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)wc);
	ic.setToBufferSize(tosize);

	if (ic.convert()) {
		stdoutput.printf("char \"%s\" -> wchar_t \"%ls\"\n",
						hellochar,wc);
	}
	delete[] wc;


	// convert wchar_t to char...
	fromsize=wcharstring::getSize(hellowchar);
	char	*c=new char[fromsize];

	ic.setFromEncoding("WCHAR_T");
	ic.setToEncoding("");
	ic.setFromBuffer((const byte_t *)hellowchar);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)c);
	ic.setToBufferSize(fromsize);

	if (ic.convert()) {
		stdoutput.printf("wchar_t \"%ls\" -> char \"%s\"\n",
						hellowchar,c);
	}
	delete[] c;


	// convert char to UCS-2...
	fromsize=charstring::getSize(hellochar);
	tosize=fromsize*sizeof(ucs2_t);
	ucs2_t	*uc=new ucs2_t[fromsize];

	ic.setFromEncoding("");
	ic.setToEncoding(ucs2);
	ic.setFromBuffer((const byte_t *)hellochar);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)uc);
	ic.setToBufferSize(tosize);

	if (ic.convert()) {
		char	*ucstr=charstring::duplicateUcs2(uc);
		stdoutput.printf("char \"%s\" -> UCS-2 \"%s\"\n",
						hellochar,ucstr);
		delete[] ucstr;
	}
	delete[] uc;


	// convert UCS-2 to char...
	fromsize=ucs2charstring::getSize(helloucs2);
	c=new char[fromsize];

	ic.setFromEncoding(ucs2);
	ic.setToEncoding("");
	ic.setFromBuffer((const byte_t *)helloucs2);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)c);
	ic.setToBufferSize(fromsize);

	if (ic.convert()) {
		stdoutput.printf("UCS-2 -> char \"%s\"\n",c);
	}
	delete[] c;


	// convert wchar_t to UCS-2...
	fromsize=wcharstring::getSize(hellowchar);
	tosize=fromsize*sizeof(ucs2_t);
	uc=new ucs2_t[fromsize];

	ic.setFromEncoding("WCHAR_T");
	ic.setToEncoding(ucs2);
	ic.setFromBuffer((const byte_t *)hellowchar);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)uc);
	ic.setToBufferSize(tosize);

	if (ic.convert()) {
		char	*ucstr=charstring::duplicateUcs2(uc);
		stdoutput.printf("wchar_t \"%ls\" -> UCS-2 \"%s\"\n",
						hellowchar,ucstr);
		delete[] ucstr;
	}
	delete[] uc;


	// convert UCS-2 to wchar_t...
	fromsize=ucs2charstring::getSize(helloucs2);
	tosize=fromsize*sizeof(wchar_t);
	wc=new wchar_t[fromsize];

	ic.setFromEncoding(ucs2);
	ic.setToEncoding("WCHAR_T");
	ic.setFromBuffer((const byte_t *)helloucs2);
	ic.setFromBufferSize(fromsize);
	ic.setToBuffer((byte_t *)wc);
	ic.setToBufferSize(tosize);

	if (ic.convert()) {
		stdoutput.printf("UCS-2 -> wchar_t \"%ls\"\n",wc);
	}
	delete[] wc;
}
