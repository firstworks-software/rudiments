#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// backslash-escaping of quote, backslash and space characters...
	const wchar_t	path[]=L"\"C:\\Program Files\\Firstworks\"";

	stdoutput.printf("original path:\n  %ls\n",path);

	wchar_t	*escapedpath=wcharstring::escape(path,L"\"\\ ");
	stdoutput.printf("escaped path:\n  %ls\n",escapedpath);

	wchar_t	*unescapedpath=wcharstring::unescape(escapedpath);
	stdoutput.printf("unescaped path:\n  %ls\n",unescapedpath);
	stdoutput.write('\n');

	delete[] escapedpath;
	delete[] unescapedpath;
}
