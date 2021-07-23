// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/sensitivevalue.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("sensitivevalue");

	const char	*svstring="Passw0rd";
	uint64_t	svstringsize=8;
	const char	*svstringwithtrailingtext="Passw0rdBlahBlahBlah";
	const char	*svhex="5061737377307264";
	const char	*svbinfile="[password.bin]";
	const char	*svhexfile="[password.hex]";

	
	stdoutput.printf("verbatim");
	sensitivevalue	sv;
	sv.parse(svstring);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim with trailing text");
	sv.parse(svstringwithtrailingtext,svstringsize);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim is hex");
	sv.setVerbatimIsHex(true);
	test("getVerbatimIsHex()",sv.getVerbatimIsHex());
	sv.parse(svhex);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file");
	sv.parse(svbinfile);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file is hex");
	sv.setFileIsHex(true);
	test("getFileIsHex()",sv.getFileIsHex());
	sv.parse(svhexfile);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file extensions");
	sv.setBinaryExtension("bin");
	test("getBinaryExtension()",
		!charstring::compare(sv.getBinaryExtension(),"bin"));
	sv.setHexExtension("hex");
	test("getHexExtension()",
		!charstring::compare(sv.getHexExtension(),"hex"));
	sv.parse(svbinfile);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	sv.parse(svhexfile);
	test("getValue()",!bytestring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");
	
	stdoutput.printf("path");
	// FIXME: implement this...
	stdoutput.printf("\n\n");
}
