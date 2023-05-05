#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {


	// backslash-escaping of quote, backslash and space characters...
	const char	path[]="\"C:\\Program Files\\Firstworks\"";

	stdoutput.printf("original path:\n  %s\n",path);

	char	*escapedpath=charstring::escape(path,"\"\\ ");
	stdoutput.printf("escaped path:\n  %s\n",escapedpath);

	char	*unescapedpath=charstring::unescape(escapedpath);
	stdoutput.printf("unescaped path:\n  %s\n",unescapedpath);
	stdoutput.write('\n');

	delete[] escapedpath;
	delete[] unescapedpath;



	// url encoding...
	const char	urlstr[]="string with spaces and symbols: \\{}\"\'";

	stdoutput.printf("original string:\n  %s\n",urlstr);

	char	*encodedstr=charstring::urlEncode(urlstr);
	stdoutput.printf("url encoded string:\n  %s\n",encodedstr);

	char	*unencodedstr=charstring::urlDecode(encodedstr);
	stdoutput.printf("url unencoded string:\n  %s\n",unencodedstr);
	stdoutput.write('\n');

	delete[] encodedstr;
	delete[] unencodedstr;



	// base-64 encoding...
	const byte_t	text[]="All along the untrodden "
					"paths of the future...";

	stdoutput.printf("original text:\n  %s\n",text);

	char	*encodedtext=charstring::base64Encode(text);
	stdoutput.printf("encoded text:\n  %s\n",encodedtext);

	byte_t	*decodedtext=charstring::base64Decode(encodedtext);
	stdoutput.printf("decoded text:\n  %s\n",decodedtext);
	stdoutput.write('\n');

	delete[] encodedtext;
	delete[] decodedtext;



	// obfuscation...
	char	data[]="sensitive data";

	stdoutput.printf("original data:\n  %s\n",data);

	charstring::obfuscate(data);
	stdoutput.write("obfuscated data:\n  ");
	stdoutput.safePrint(data);
	stdoutput.write("\n");

	charstring::deobfuscate(data);
	stdoutput.printf("deobfuscated data:\n  %s\n",data);
}
