// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/scalar.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, char **argv) {

	header("scalar");

	// integers
	stdoutput.printf("integers:\n");
	scalar<int32_t>	ints;

	// set/get
	stdoutput.printf("set/get...\n");
	for (int32_t i=-10; i<=10; i++) {
		ints.setValue(i);
		test("setValue()/getValue(): ",ints.getValue()==i);
	}
	stdoutput.printf("\n");

	// strings
	stdoutput.printf("strings:\n");
	scalar<const char *>	strs;

	// set/get
	stdoutput.printf("set/get...\n");
	const char *str[]={
		"-10",
		"-9",
		"-8",
		"-7",
		"-6",
		"-5",
		"-4",
		"-3",
		"-2",
		"-1",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"10"
	};
	for (uint16_t i=0; i<21; i++) {
		strs.setValue(str[i]);
		test("setValue()/getValue(): ",
			!charstring::compare(strs.getValue(),str[i]));
	}
	stdoutput.printf("\n");
}
