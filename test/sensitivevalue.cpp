// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/sensitivevalue.h>
#include <rudiments/directory.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("sensitivevalue");

	const char	*svstring="Passw0rd";
	uint64_t	svstringsize=8;
	const char	*svstringwithtrailingtext="Passw0rdBlahBlahBlah";
	const char	*svhex="5061737377307264";
	const char	*svbinfile="[passwordbin]";
	const char	*svhexfile="[passwordhex]";
	const char	*svbfile="[passwordb]";
	const char	*svhfile="[passwordh]";

	sensitivevalue	sv;

	stdoutput.printf("delimiters\n");
	const char	*rs="[[[";
	sv.setRedirectStart(rs);
	test("getRedirectStart()",
		!charstring::compare(sv.getRedirectStart(),rs));
	test("getRedirectStartLength()",
		sv.getRedirectStartLength()==charstring::length(rs));
	const char	*re="]]]";
	sv.setRedirectEnd(re);
	test("getRedirectEnd()",
		!charstring::compare(sv.getRedirectEnd(),re));
	test("getRedirectEndLength()",
		sv.getRedirectEndLength()==charstring::length(rs));
	rs="[";
	sv.setRedirectStart(rs);
	test("getRedirectStart()",
		!charstring::compare(sv.getRedirectStart(),rs));
	test("getRedirectStartLength()",
		sv.getRedirectStartLength()==charstring::length(rs));
	re="]";
	sv.setRedirectEnd(re);
	test("getRedirectEnd()",
		!charstring::compare(sv.getRedirectEnd(),re));
	test("getRedirectEndLength()",
		sv.getRedirectEndLength()==charstring::length(rs));
	stdoutput.printf("\n\n");

	
	stdoutput.printf("verbatim\n");
	sv.parse(svstring);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim with trailing text\n");
	sv.parse(svstringwithtrailingtext,svstringsize);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim is hex\n");
	sv.setVerbatimIsHex(true);
	test("getVerbatimIsHex()",sv.getVerbatimIsHex());
	sv.parse(svhex);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file\n");
	sv.parse(svbinfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file is hex\n");
	sv.setFileIsHex(true);
	test("getFileIsHex()",sv.getFileIsHex());
	sv.parse(svhexfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file extensions\n");
	sv.setBinaryExtension("bin");
	test("getBinaryExtension()",
		!charstring::compare(sv.getBinaryExtension(),"bin"));
	test("getBinaryExtensionLength()",sv.getBinaryExtensionLength()==3);
	sv.setHexExtension("hex");
	test("getHexExtension()",
		!charstring::compare(sv.getHexExtension(),"hex"));
	test("getHexExtensionLength()",sv.getHexExtensionLength()==3);
	sv.parse(svbfile);
	test("bin: getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	sv.parse(svhfile);
	test("hex getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");
	
	stdoutput.printf("path\n");
	char	*pwd=directory::getCurrentWorkingDirectory();
	sv.setPath(pwd);
	test("getPath()",!charstring::compare(sv.getPath(),pwd));
	test("getPathLength()",sv.getPathLength()==charstring::length(pwd));
	directory::changeDirectory("..");
	sv.parse(svbfile);
	test("bin: getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("bin: getTextValue()",
			!charstring::compare(sv.getTextValue(),svstring));
	sv.parse(svhfile);
	test("hex: getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("hex: getTextValue()",
			!charstring::compare(sv.getTextValue(),svstring));
	delete[] pwd;
	stdoutput.printf("\n\n");
}
