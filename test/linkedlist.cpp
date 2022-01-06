// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/linkedlist.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

class testclass {
	public:
		uint64_t	val;
};

int main(int argc, char **argv) {

	header("linkedlist");

	// integers
	stdoutput.printf("integers:\n");
	linkedlist<int32_t>	intl;

	// append
	intl.append((int32_t)0);
	test("append(0)/getLength()",intl.getLength()==1);
	test("append(0)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(0)/walk backwards",intl.getLast()->getValue()==0);
	intl.append((int32_t)1);
	test("append(1)/getLength()",intl.getLength()==2);
	test("append(1)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(1)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("append(1)/walk backwards",intl.getLast()->getValue()==1);
	test("append(1)/walk backwards",intl.getLast()->
						getPrevious()->getValue()==0);
	intl.append((int32_t)3);
	test("append(3)/getLength()",intl.getLength()==3);
	test("append(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("append(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==3);
	test("append(3)/walk backwards",intl.getLast()->getValue()==3);
	test("append(3)/walk backwards",intl.getLast()->
						getPrevious()->getValue()==1);
	test("append(3)/walk backwards",intl.getLast()->getPrevious()->
						getPrevious()->getValue()==0);

	// insert
	intl.insertAfter(intl.getFirst()->getNext(),2);
	test("insertAfter(2,2)/getLength()",intl.getLength()==4);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getValue()==0);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getNext()->
						getValue()==3);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->getValue()==3);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->
						getPrevious()->getValue()==2);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->getPrevious()->
						getPrevious()->getValue()==1);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->getPrevious()->
						getPrevious()->getPrevious()->
						getValue()==0);

	// remove
	intl.remove(3);
	test("remove(3)/getLength()",intl.getLength()==3);
	test("remove(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("remove(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("remove(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("remove(3)/walk backwards",intl.getLast()->getValue()==2);
	test("remove(3)/walk backwards",intl.getLast()->
						getPrevious()->getValue()==1);
	test("remove(3)/walk backwards",intl.getLast()->getPrevious()->
						getPrevious()->getValue()==0);

	// clear
	intl.clear();
	test("clear()/getLength()",intl.getLength()==0);
	test("clear()/getFirst()",!intl.getFirst());
	test("clear()/getLast()",!intl.getLast());
	stdoutput.printf("\n\n");


	// strings
	stdoutput.printf("strings:\n");
	linkedlist<const char *>	strl;

	// append
	strl.append("zero");
	test("append(zero)/getLength()",strl.getLength()==1);
	test("append(zero)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(zero)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"zero"));
	strl.append("one");
	test("append(one)/getLength()",strl.getLength()==2);
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	test("append(one)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"one"));
	test("append(one)/walk backwards",!charstring::compare(
						strl.getLast()->
						getPrevious()->getValue(),
						"zero"));
	strl.append("three");
	test("append(three)/getLength()",strl.getLength()==3);
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	test("append(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getNext()->
						getNext()->getValue(),
						"three"));
	test("append(three)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"three"));
	test("append(three)/walk backwards",!charstring::compare(
						strl.getLast()->
						getPrevious()->getValue(),
						"one"));
	test("append(three)/walk backwards",!charstring::compare(
						strl.getLast()->getPrevious()->
						getPrevious()->getValue(),
						"zero"));

	// insert
	strl.insertAfter(strl.getFirst()->getNext(),"two");
	test("insertAfter(two,two)/getLength()",strl.getLength()==4);
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getValue(),
							"zero"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getValue(),
							"one"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getNext()->
							getValue(),
							"two"));
	test("insertAfter(two,two)/walk forwards",!charstring::compare(
							strl.getFirst()->
							getNext()->getNext()->
							getNext()->getValue(),
							"three"));
	test("insertAfter(two,two)/walk backwards",!charstring::compare(
							strl.getLast()->
							getValue(),
							"three"));
	test("insertAfter(two,two)/walk backwards",!charstring::compare(
							strl.getLast()->
							getPrevious()->
							getValue(),
							"two"));
	test("insertAfter(two,two)/walk backwards",!charstring::compare(
							strl.getLast()->
							getPrevious()->
							getPrevious()->
							getValue(),
							"one"));
	test("insertAfter(two,two)/walk backwards",!charstring::compare(
							strl.getLast()->
							getPrevious()->
							getPrevious()->
							getPrevious()->
							getValue(),
							"zero"));

	// remove
	strl.remove("three");
	test("remove(three)/getLength()",strl.getLength()==3);
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	test("remove(three)/walk forwards",!charstring::compare(
						strl.getFirst()->getNext()->
						getNext()->getValue(),
						"two"));
	test("remove(three)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"two"));
	test("remove(three)/walk backwards",!charstring::compare(
						strl.getLast()->
						getPrevious()->getValue(),
						"one"));
	test("remove(three)/walk backwards",!charstring::compare(
						strl.getLast()->getPrevious()->
						getPrevious()->getValue(),
						"zero"));

	// clear
	strl.clear();
	test("clear()/getLength()",strl.getLength()==0);
	test("clear()/getFirst()",!strl.getFirst());
	test("clear()/getLast()",!strl.getLast());
	stdoutput.printf("\n");


	// heap sort
	stdoutput.printf("heapSort() (from exactly backwards)\n");
	strl.append("5");
	strl.append("4");
	strl.append("3");
	strl.append("2");
	strl.append("1");
	strl.append("0");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (after sorted from backwards)\n");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));



	strl.clear();
	stdoutput.printf("heapSort() (from random order)\n");
	strl.append("4");
	strl.append("5");
	strl.append("2");
	strl.append("6");
	strl.append("1");
	strl.append("7");
	strl.append("3");
	strl.append("0");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (after sorted from random order)\n");
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (reverse comparator)\n");
	strl.getComparator()->setReverse(true);
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"7"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"6"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"5"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"4"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"3"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"2"));

	stdoutput.printf("heapSort() (forward comparator)\n");
	strl.getComparator()->setReverse(false);
	strl.heapSort();
	test("0",!charstring::compare(
				strl.getFirst()->getValue(),
				"0"));
	test("1",!charstring::compare(
				strl.getFirst()->getNext()->getValue(),
				"1"));
	test("2",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getValue(),
				"2"));
	test("3",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getValue(),
				"3"));
	test("4",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getValue(),
				"4"));
	test("5",!charstring::compare(
				strl.getFirst()->getNext()->getNext()->
					getNext()->getNext()->getNext()->
					getValue(),
				"5"));

	stdoutput.printf("heapSort() (natural, reverse, delimiters)\n");
	strl.clear();
	const char *dirs[]={
		"/u01/app/oracle/product/10/client_1/",
		"/u01/app/oracle/product/10/client_2/",
		"/u01/app/oracle/product/10.0/client_1/",
		"/u01/app/oracle/product/10.0/client_2/",
		"/u01/app/oracle/product/10.1/client_1/",
		"/u01/app/oracle/product/10.1/client_2/",
		"/u01/app/oracle/product/10.2/client_1/",
		"/u01/app/oracle/product/10.2/client_2/",
		"/u01/app/oracle/product/11/client_1/",
		"/u01/app/oracle/product/11/client_2/",
		"/u01/app/oracle/product/11.0/client_1/",
		"/u01/app/oracle/product/11.0/client_2/",
		"/u01/app/oracle/product/11.1/client_1/",
		"/u01/app/oracle/product/11.1/client_2/",
		"/u01/app/oracle/product/11.2/client_1/",
		"/u01/app/oracle/product/11.2/client_2/",
		"/u01/app/oracle/product/12/client_1/",
		"/u01/app/oracle/product/12/client_2/",
		"/u01/app/oracle/product/12.0/client_1/",
		"/u01/app/oracle/product/12.0/client_2/",
		"/u01/app/oracle/product/12.1/client_1/",
		"/u01/app/oracle/product/12.1/client_2/",
		"/u01/app/oracle/product/12.2/client_1/",
		"/u01/app/oracle/product/12.2/client_2/",
		"/u01/app/oracle/product/18/client_1/",
		"/u01/app/oracle/product/18/client_2/",
		"/u01/app/oracle/product/18.1/client_1/",
		"/u01/app/oracle/product/18.1/client_2/",
		"/u01/app/oracle/product/18.2/client_1/",
		"/u01/app/oracle/product/18.2/client_2/",
		"/u01/app/oracle/product/18.3.0/client_1/",
		"/u01/app/oracle/product/18.3.0/client_2/",
		"/u01/app/oracle/product/19/client_1/",
		"/u01/app/oracle/product/19/client_2/",
		"/u01/app/oracle/product/19.0/client_1/",
		"/u01/app/oracle/product/19.0/client_2/",
		"/u01/app/oracle/product/19.0.0/client_1/",
		"/u01/app/oracle/product/19.0.0/client_2/",
		"/u01/app/oracle/product/19.0.0/client_2/libclntsh.so",
		"/u01/app/oracle/product/19.0.0/client_2/libclntsh.so.19",
		"/u01/app/oracle/product/19.0.0/client_2/libclntsh.so.19.1",
		NULL
	};
	const char * const *d;
	for (d=dirs; *d; d++) {
		strl.append(*d);
	}
	strl.getComparator()->setReverse(true);
	strl.getComparator()->setNatural(true);
	strl.getComparator()->setNumberDelimiters("._");
	strl.heapSort();
	stringbuffer	title;
	for (listnode<const char *> *node=strl.getFirst();
					node; node=node->getNext()) {
		title.clear();
		d--;
		test(*d,!charstring::compare(node->getValue(),*d));
	}
	stdoutput.printf("\n");


	// remove/clear and delete
	stdoutput.printf("remove/clear and delete...\n");
	linkedlist<stringbuffer *>	strbl;
	strbl.setManageValues(true);
	stringbuffer	*strb1=new stringbuffer();
	strb1->append("1");
	strbl.append(strb1);
	stringbuffer	*strb2=new stringbuffer();
	strb2->append("2");
	strbl.append(strb2);
	stringbuffer	*strb3=new stringbuffer();
	strb3->append("3");
	strbl.append(strb3);
	stringbuffer	*strb4=new stringbuffer();
	strb4->append("4");
	strbl.append(strb4);
	strbl.remove(strb3);
	test("remove, getLength(): ",strbl.getLength()==3);
	strbl.remove(strb4);
	test("remove, getLength(): ",strbl.getLength()==2);
	strbl.clear();
	test("clear, getLength(): ",!strbl.getLength());
	stdoutput.printf("\n");

	// remove/clear and array delete
	stdoutput.printf("remove/clear and array delete...\n");
	linkedlist<char *>	charl;
	charl.setManageArrayValues(true);
	charl.append(charstring::duplicate("1"));
	charl.append(charstring::duplicate("2"));
	charl.append(charstring::duplicate("3"));
	charl.append(charstring::duplicate("4"));
	charl.remove((char *)"1");
	test("remove, getLength(): ",charl.getLength()==3);
	charl.remove((char *)"2");
	test("remove, getLength(): ",charl.getLength()==2);
	charl.clear();
	test("clear, getLength(): ",!charl.getLength());
	stdoutput.printf("\n");

	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		linkedlist<char *>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (const char * const *v=values; *v; v++) {
			if (!i) {
				cch1.append((char *)*v);
			} else {
				cch1.append(charstring::duplicate((char *)*v));
			}
		}
	
		linkedlist<char *>	cch2(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==1) {
				cch2.clear();
			}
			if (j) {
				cch2=cch1;
			}

			// verify flags
			test((!j)?"copy: manage values":
					"assignment: manage values",
						cch2.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy: manage array values":
					"assignment: manage array values",
						cch2.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy: length":"assignment: length",
						cch2.getLength()==
						cch1.getLength());

			// verify values
			bool	success=true;
			listnode<char *>	*cch1n=cch1.getFirst();
			listnode<char *>	*cch2n=cch2.getFirst();
			while (cch1n) {
				if (charstring::compare(cch1n->getValue(),
							cch2n->getValue())) {
					success=false;
					break;
				}
				cch1n=cch1n->getNext();
				cch2n=cch2n->getNext();
			}
			test((!j)?"copy: values":"assignment: values",success);
		}
	}
	stdoutput.printf("\n");
}
