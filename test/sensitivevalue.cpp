// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sensitivevalue.h>
#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/signalclasses.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

#ifndef WIN32
const char	*svtestfile="/tmp/svtestfile";
const char	*svtestdir="/tmp/svtestdir";
const char	*svtestdirfile="/tmp/svtestdir.txt";
#else
const char	*svtestfile="svtestfile";
const char	*svtestdir="svtestdir";
const char	*svtestdirfile="svtestdir.txt";
#endif

static void writeFile(const char *name, const char *data, size_t size) {
	file	f;
	if (f.open(name,O_WRONLY|O_CREAT|O_TRUNC,
				permissions::getOwnerReadWrite())) {
		f.write(data,size);
		f.close();
	}
}

// checks that "in" parses to exactly "expected" of "expectedsize" bytes and
// that the value is followed by a NULL
static bool parsesTo(sensitivevalue *sv, const char *in,
				const char *expected, uint64_t expectedsize) {
	sv->parse(in);
	return (sv->getValueSize()==expectedsize &&
		sv->getValue() &&
		!bytestring::compare(sv->getValue(),expected,expectedsize) &&
		!sv->getValue()[expectedsize]);
}

// files that open but cannot be read, files that are shorter than they
// report and the size of the value
static void fileTests() {

	signalmanager::alarm(60);

	file::remove(svtestfile);
	file::remove(svtestdirfile);
	directory::remove(svtestdir);

	stdoutput.printf("file - directory\n");
	sensitivevalue	dv;
	test("text, default chomp",parsesTo(&dv,"[/etc]","[/etc]",6));
	dv.setChompTextFile(false);
	test("text, no chomp",parsesTo(&dv,"[/etc]","[/etc]",6));
	dv.setFileFormat(FORMAT_HEX);
	test("hex",parsesTo(&dv,"[/etc]","[/etc]",6));
	stdoutput.printf("\n\n");

	stdoutput.printf("file - directory, then the next name\n");
	test("create directory",directory::create(svtestdir,
				permissions::getOwnerReadWriteExecute()));
	writeFile(svtestdirfile,"next\n",5);
	sensitivevalue	nv;
	nv.setTextExtension("txt");
	test("text extension after a directory",
			parsesTo(&nv,"[/tmp/svtestdir]","next",4));
	sensitivevalue	hv;
	hv.setFileFormat(FORMAT_HEX);
	hv.setHexExtension("txt");
	writeFile(svtestdirfile,"4e657874",8);
	test("hex extension after a directory",
			parsesTo(&hv,"[/tmp/svtestdir]","Next",4));
	sensitivevalue	pv;
	pv.setPath("/tmp");
	test("path after a directory",
			parsesTo(&pv,"[svtestdir]","[svtestdir]",11));
	file::remove(svtestdirfile);
	directory::remove(svtestdir);
	stdoutput.printf("\n\n");

	stdoutput.printf("file - sizes\n");
	sensitivevalue	sv;
	writeFile(svtestfile,"",0);
	test("empty",parsesTo(&sv,"[/tmp/svtestfile]","",0));
	writeFile(svtestfile,"\n",1);
	test("1 byte, newline",parsesTo(&sv,"[/tmp/svtestfile]","",0));
	writeFile(svtestfile,"x",1);
	test("1 byte",parsesTo(&sv,"[/tmp/svtestfile]","x",1));
	writeFile(svtestfile,"ab\r\n",4);
	test("crlf",parsesTo(&sv,"[/tmp/svtestfile]","ab",2));
	writeFile(svtestfile,"ab\n\r",4);
	test("lfcr",parsesTo(&sv,"[/tmp/svtestfile]","ab",2));
	writeFile(svtestfile,"ab\r",3);
	test("cr",parsesTo(&sv,"[/tmp/svtestfile]","ab",2));
	writeFile(svtestfile,"a\nb",3);
	test("inner newline",parsesTo(&sv,"[/tmp/svtestfile]","a\nb",3));
	writeFile(svtestfile,"a\0b\n",4);
	test("binary",parsesTo(&sv,"[/tmp/svtestfile]","a\0b",3));
	sv.setFileFormat(FORMAT_HEX);
	writeFile(svtestfile,"",0);
	test("hex, empty",parsesTo(&sv,"[/tmp/svtestfile]","",0));
	writeFile(svtestfile,"4142\n",5);
	test("hex, trailing newline",parsesTo(&sv,"[/tmp/svtestfile]","AB",2));
	writeFile(svtestfile,"41424344",8);
	test("hex",parsesTo(&sv,"[/tmp/svtestfile]","ABCD",4));
	file::remove(svtestfile);
	stdoutput.printf("\n\n");

	#ifdef __linux__
	// st_size is 4096 but the file has less data
	const char	*sysfile="/sys/devices/system/cpu/online";
	if (file::exists(sysfile)) {
		stdoutput.printf("file - shorter than its size\n");
		char	*contents=file::getContents(sysfile);
		test("read the file",contents!=NULL);
		uint64_t	len=charstring::getLength(contents);
		while (len && (contents[len-1]=='\n' ||
					contents[len-1]=='\r')) {
			len--;
		}
		sensitivevalue	ssv;
		char	in[64];
		charstring::copy(in,"[");
		charstring::append(in,sysfile);
		charstring::append(in,"]");
		test("text",parsesTo(&ssv,in,contents,len));
		test("text, text value length",
				ssv.getTextValueLength()==len);
		ssv.setChompTextFile(false);
		ssv.parse(in);
		test("no chomp, size is the data read",
				ssv.getValueSize()==charstring::getLength(
								contents));
		ssv.setFileFormat(FORMAT_HEX);
		ssv.parse(in);
		test("hex, no read past the data",
				ssv.getValueSize()<=
				charstring::getLength(contents)/2);
		delete[] contents;
		stdoutput.printf("\n\n");
	}
	#endif

	signalmanager::alarm(0);
}

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

	fileTests();

	return 0;
}
