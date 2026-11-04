// Copyright (c) David Muse
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
	char	*err=error::getErrorString();
	bool	ok=charstring::isInSet(err,intr);
	delete[] err;
	test("getErrorString()",ok);

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
	err=error::getErrorString();
	ok=charstring::isInSet(err,success);
	delete[] err;
	test("clearError()/getErrorString()",ok);

	return 0;
}
