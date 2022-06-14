// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/scalar.h>
#include <rudiments/linkedlist.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/avltree.h>
#include <rudiments/table.h>
#include <rudiments/dictionary.h>
#include <rudiments/dynamicarray.h>
#include <rudiments/staticarray.h>
#include "test.cpp"

const char *numbers[]={
	"one","two","three","four","five","six","seven","eight","nine","ten"
};

int main(int argc, const char **argv) {

	header("collectionprint");

	stdoutput.printf("scalar (int32_t):\n");
	scalar<int32_t>	si32;
	for (int32_t i=-5; i<5; i++) {
		si32.setValue(i);
		si32.print();
	}
	stdoutput.printf("\n");

	stdoutput.printf("scalar (const char *):\n");
	scalar<const char *>	sc;
	for (uint32_t i=0; i<10; i++) {
		sc.setValue(numbers[i]);
		sc.print();
	}
	stdoutput.printf("\n");



	stdoutput.printf("linkedlist (int32_t):\n");
	linkedlist<int32_t>	lli32;
	for (int32_t i=-5; i<5; i++) {
		lli32.append(i);
	}
	lli32.print();
	stdoutput.printf("\n");

	stdoutput.printf("linkedlist (const char *):\n");
	linkedlist<const char *>	llc;
	for (uint32_t i=0; i<10; i++) {
		llc.append(numbers[i]);
	}
	llc.print();
	stdoutput.printf("\n");



	stdoutput.printf("singlylinkedlist (int32_t):\n");
	singlylinkedlist<int32_t>	slli32;
	for (int32_t i=-5; i<5; i++) {
		slli32.append(i);
	}
	slli32.print();
	stdoutput.printf("\n");

	stdoutput.printf("singlylinkedlist (const char *):\n");
	singlylinkedlist<const char *>	sllc;
	for (uint32_t i=0; i<10; i++) {
		sllc.append(numbers[i]);
	}
	sllc.print();
	stdoutput.printf("\n");



	stdoutput.printf("avltree (int32_t):\n");
	avltree<int32_t>	ati32;
	for (int32_t i=-5; i<5; i++) {
		ati32.insert(i);
	}
	ati32.print();
	stdoutput.printf("\n");

	stdoutput.printf("avltree (const char *):\n");
	avltree<const char *>	atc;
	for (uint32_t i=0; i<10; i++) {
		atc.insert(numbers[i]);
	}
	atc.print();
	stdoutput.printf("\n");



	stdoutput.printf("dictionary (int32_t, const char *):\n");
	dictionary<int32_t,const char *>	d;
	for (int32_t i=0; i<10; i++) {
		d.setValue(i,numbers[i]);
	}
	d.print();
	stdoutput.printf("\n");
}
