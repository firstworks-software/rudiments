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


	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		scalar<char *>	cch1[26];
		for (uint16_t j=0; j<26; j++) {
			cch1[j].setManageArrayValues(i);
		}
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (uint16_t j=0; j<26; j++) {
			if (!i) {
				cch1[j].setValue((char *)values[j]);
			} else {
				cch1[j].setValue(
					charstring::duplicate(
						(char *)values[j]));
			}
		}
	
		for (uint16_t k=0; k<26; k++) {

			scalar<char *>	cch2(cch1[k]);
			for (uint16_t j=0; j<3; j++) {

				// 1st iteration is copy
				// 2nd is assignment
				// 3rd is assignment after clear
				if (j==1) {
					cch2.clear();
				}
				if (j) {
					cch2=cch1[k];
				}

				// verify flags
				test((!j)?"copy: manage values":
						"assignment: manage values",
						cch2.getManageValues()==
						cch1[k].getManageValues());
				test((!j)?"copy: manage array values":
					"assignment: manage array values",
						cch2.getManageArrayValues()==
						cch1[k].getManageArrayValues());

				// verify values
				bool	success=true;
				if (charstring::compare(cch1[k].getValue(),
							cch2.getValue())) {
					success=false;
					break;
				}
				test((!j)?"copy: values":
						"assignment: values",success);
			}
		}
	}
	stdoutput.printf("\n");

	return 0;
}
