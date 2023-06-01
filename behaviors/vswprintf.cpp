// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/stdio.h>
#include <rudiments/wcharstring.h>

#include <stdio.h>
#include <wchar.h>

#include "../test/test.cpp"

int myprintf(wchar_t *buffer, size_t length, const wchar_t *format, ...) {
	va_list	args;
	va_start(args,format);
	size_t	result=vswprintf(buffer,length,format,args);
	va_end(args);
	return result;
}

int main() {

	header("vswprintf");

	stdoutput.printf("too small of a buffer:\n");
	wchar_t	buf[2];
	int	result=myprintf(buf,2,L"hello",NULL);
	stdoutput.printf("  vswprintf(buf,2,L\"hello\",NULL);\n");
	stdoutput.printf("  result: %d\n",result);
	stdoutput.printf("  errno:  %d\n",error::getErrorNumber());
	stdoutput.printf("\n");
}
