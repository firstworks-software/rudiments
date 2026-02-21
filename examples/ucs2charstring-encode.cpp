#include <rudiments/ucs2charstring.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// backslash-escaping of quote, backslash and space characters...
	ucs2literal(path,"\"C:\\Program Files\\Firstworks\"");

	char	*pathstr=charstring::duplicateUcs2(path);
	stdoutput.printf("original path:\n  %s\n",pathstr);
	delete[] pathstr;

	ucs2literal(escchars,"\"\\ ");
	ucs2_t	*escapedpath=ucs2charstring::escape(path,escchars);
	char	*escapedpathstr=charstring::duplicateUcs2(escapedpath);
	stdoutput.printf("escaped path:\n  %s\n",escapedpathstr);
	delete[] escapedpathstr;

	ucs2_t	*unescapedpath=ucs2charstring::unescape(escapedpath);
	char	*unescapedpathstr=charstring::duplicateUcs2(unescapedpath);
	stdoutput.printf("unescaped path:\n  %s\n",unescapedpathstr);
	stdoutput.write('\n');

	delete[] unescapedpathstr;
	delete[] escapedpath;
	delete[] unescapedpath;
}
