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
	const char	*svstringunchomped="Passw0rd\n";
	uint64_t	svstringunchompedsize=9;
	const char	*svstringwithtrailingtext="Passw0rdBlahBlahBlah";
	const char	*svhex="5061737377307264";
	const char	*svtxtfile="[passwordtxt]";
	const char	*svbinfile="[passwordbin]";
	const char	*svhexfile="[passwordhex]";
	const char	*svtfile="[passwordt]";
	const char	*svbfile="[passwordb]";
	const char	*svhfile="[passwordh]";

	sensitivevalue	sv;

	stdoutput.printf("delimiters\n");
	const char	*rs="[[[";
	sv.setIncludeStart(rs);
	test("getIncludeStart()",
		!charstring::compare(sv.getIncludeStart(),rs));
	test("getIncludeStartLength()",
		sv.getIncludeStartLength()==charstring::getLength(rs));
	const char	*re="]]]";
	sv.setIncludeEnd(re);
	test("getIncludeEnd()",
		!charstring::compare(sv.getIncludeEnd(),re));
	test("getIncludeEndLength()",
		sv.getIncludeEndLength()==charstring::getLength(rs));
	rs="[";
	sv.setIncludeStart(rs);
	test("getIncludeStart()",
		!charstring::compare(sv.getIncludeStart(),rs));
	test("getIncludeStartLength()",
		sv.getIncludeStartLength()==charstring::getLength(rs));
	re="]";
	sv.setIncludeEnd(re);
	test("getIncludeEnd()",
		!charstring::compare(sv.getIncludeEnd(),re));
	test("getIncludeEndLength()",
		sv.getIncludeEndLength()==charstring::getLength(rs));
	stdoutput.printf("\n\n");

	
	stdoutput.printf("verbatim\n");
	sv.parse(svstring);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getValueSize()",
			sv.getValueSize()==charstring::getLength(svstring));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	test("getTextValueLength()",
			sv.getTextValueLength()==charstring::getLength(svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim with trailing text\n");
	sv.parse(svstringwithtrailingtext,svstringsize);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("verbatim - hex\n");
	sv.setVerbatimFormat(FORMAT_HEX);
	test("getVerbatimFormat()",sv.getVerbatimFormat()==FORMAT_HEX);
	sv.parse(svhex);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file - txt (unchomped)\n");
	sv.setChompTextFile(false);
	sv.parse(svtxtfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstringunchomped,
							svstringunchompedsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),
							svstringunchomped,
							svstringunchompedsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),
							svstringunchomped));
	stdoutput.printf("\n\n");

	stdoutput.printf("file - txt (chomped)\n");
	sv.setChompTextFile(true);
	sv.parse(svtxtfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file - bin\n");
	sv.parse(svbinfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file - hex\n");
	sv.setFileFormat(FORMAT_HEX);
	test("getFileFormat()",sv.getFileFormat()==FORMAT_HEX);
	sv.parse(svhexfile);
	test("getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("getTextValue()",!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");

	stdoutput.printf("file extensions\n");
	sv.setTextExtension("txt");
	test("getTextExtension()",
		!charstring::compare(sv.getTextExtension(),"txt"));
	test("getTextExtensionLength()",sv.getTextExtensionLength()==3);
	sv.setBinaryExtension("bin");
	test("getBinaryExtension()",
		!charstring::compare(sv.getBinaryExtension(),"bin"));
	test("getBinaryExtensionLength()",sv.getBinaryExtensionLength()==3);
	sv.setHexExtension("hex");
	test("getHexExtension()",
		!charstring::compare(sv.getHexExtension(),"hex"));
	test("getHexExtensionLength()",sv.getHexExtensionLength()==3);
	sv.parse(svtfile);
	test("txt: getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("txt: getTextValue()",
			!charstring::compare(sv.getTextValue(),svstring));
	sv.parse(svbfile);
	test("bin: getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("bin: getTextValue()",
			!charstring::compare(sv.getTextValue(),svstring));
	sv.parse(svhfile);
	test("hex getValue()",!bytestring::compare(sv.getValue(),
							svstring,
							svstringsize));
	test("hex: getTextValue()",
			!charstring::compare(sv.getTextValue(),svstring));
	stdoutput.printf("\n\n");
	
	stdoutput.printf("path\n");
	char	*pwd=directory::getCurrentWorkingDirectory();
	sv.setPath(pwd);
	test("getPath()",!charstring::compare(sv.getPath(),pwd));
	test("getPathLength()",sv.getPathLength()==charstring::getLength(pwd));
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

	stdoutput.printf("detach\n");
	sv.reset();
	sv.parse(svstring);
	uint64_t	valsize=sv.getValueSize();
	byte_t		*val=sv.detachValue();
	test("detachValue() (size)",valsize==charstring::getLength(svstring));
	test("detachValue()",!bytestring::compare(val,svstring,valsize));
	delete[] val;
	sv.parse(svstring);
	uint64_t	textvalsize=sv.getTextValueLength();
	char		*textval=sv.detachTextValue();
	test("detachTextValue() (size)",
			textvalsize==charstring::getLength(svstring));
	test("detachTextValue()",
			!bytestring::compare(textval,svstring,textvalsize));
	delete[] textval;
	stdoutput.printf("\n\n");

	return 0;
}
