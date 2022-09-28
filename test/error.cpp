// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/error.h>
#include <rudiments/charstring.h>
#include <stdio.h>
#include <string.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("error");

	error::setErrorNumber(EINTR);
	test("getErrorNumber()",error::getErrorNumber()==EINTR);
	const char	*intr[]={
		"Interrupted function call",
		"Interrupted system call",
		NULL
	};
	test("getErrorString()",
		charstring::inSet(error::getErrorString(),intr));

	error::clearError();
	test("clearError()/getErrorNumber()",!error::getErrorNumber());

	const char	*success[]={
		"Success",
		"No error",
		"No error: 0",
		"Error 0",
		"Unknown error",
		"Unknown error: 0",
		"Undefined error: 0",
		NULL
	};
	test("clearError()/getErrorString()",
		charstring::inSet(error::getErrorString(),success));

	return 0;
}
