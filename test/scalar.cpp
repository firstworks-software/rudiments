// Copyright (c) David Muse
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

	// default constructor
	stdoutput.printf("default constructor...\n");
	scalar<int32_t>		defint;
	scalar<const char *>	defstr;
	test("integer: getValue()",defint.getValue()==0);
	test("string: getValue()",defstr.getValue()==NULL);
	stdoutput.printf("\n");

	// initial value, no flags
	stdoutput.printf("initial value...\n");
	scalar<int32_t>		ival5(5);
	test("integer 5: getValue()",ival5.getValue()==5);
	test("integer 5: getManageValues()",!ival5.getManageValues());
	test("integer 5: getManageArrayValues()",
					!ival5.getManageArrayValues());
	scalar<int32_t>		ival0(0);
	test("integer 0: getValue()",ival0.getValue()==0);
	test("integer 0: getManageValues()",!ival0.getManageValues());
	test("integer 0: getManageArrayValues()",
					!ival0.getManageArrayValues());
	scalar<const char *>	sval("dog");
	test("string: getValue()",!charstring::compare(sval.getValue(),"dog"));
	test("string: getManageValues()",!sval.getManageValues());
	test("string: getManageArrayValues()",!sval.getManageArrayValues());
	scalar<const char *>	sval0(NULL);
	test("string NULL: getValue()",sval0.getValue()==NULL);
	test("string NULL: getManageValues()",!sval0.getManageValues());
	test("string NULL: getManageArrayValues()",
					!sval0.getManageArrayValues());
	stdoutput.printf("\n");

	// managevalues only
	stdoutput.printf("managevalues only...\n");
	int32_t		*ip=new int32_t(7);
	scalar<int32_t *>	mval(ip,true,false);
	test("getValue()",mval.getValue()==ip && *mval.getValue()==7);
	test("getManageValues()",mval.getManageValues());
	test("getManageArrayValues()",!mval.getManageArrayValues());
	stdoutput.printf("\n");

	// managearrayvalues only
	stdoutput.printf("managearrayvalues only...\n");
	char		*dog=charstring::duplicate("dog");
	scalar<char *>	maval(dog,false,true);
	test("getValue()",maval.getValue()==dog &&
				!charstring::compare(maval.getValue(),"dog"));
	test("getManageValues()",!maval.getManageValues());
	test("getManageArrayValues()",maval.getManageArrayValues());
	stdoutput.printf("\n");

	// both flags, managearrayvalues wins
	stdoutput.printf("both flags...\n");
	char		*cat=charstring::duplicate("cat");
	scalar<char *>	bval(cat,true,true);
	test("getValue()",bval.getValue()==cat &&
				!charstring::compare(bval.getValue(),"cat"));
	test("getManageValues()",!bval.getManageValues());
	test("getManageArrayValues()",bval.getManageArrayValues());
	stdoutput.printf("\n");

	// neither flag
	stdoutput.printf("neither flag...\n");
	scalar<const char *>	nval("eagle",false,false);
	test("getValue()",!charstring::compare(nval.getValue(),"eagle"));
	test("getManageValues()",!nval.getManageValues());
	test("getManageArrayValues()",!nval.getManageArrayValues());
	stdoutput.printf("\n");

	return 0;
}
