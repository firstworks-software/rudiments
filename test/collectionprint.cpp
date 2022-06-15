// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/scalar.h>
#include <rudiments/linkedlist.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/avltree.h>
#include <rudiments/dictionary.h>
#include <rudiments/table.h>
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
		si32.collection::write();
	}
	stdoutput.printf("\n");

	stdoutput.printf("scalar (const char *):\n");
	scalar<const char *>	sc;
	for (uint32_t i=0; i<10; i++) {
		sc.setValue(numbers[i]);
		sc.collection::write();
	}
	stdoutput.printf("\n");



	stdoutput.printf("linkedlist (int32_t):\n");
	linkedlist<int32_t>	lli32;
	for (int32_t i=-5; i<5; i++) {
		lli32.append(i);
	}
	lli32.collection::write();
	stdoutput.printf("\n");

	stdoutput.printf("linkedlist (const char *):\n");
	linkedlist<const char *>	llc;
	for (uint32_t i=0; i<10; i++) {
		llc.append(numbers[i]);
	}
	llc.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("singlylinkedlist (int32_t):\n");
	singlylinkedlist<int32_t>	slli32;
	for (int32_t i=-5; i<5; i++) {
		slli32.append(i);
	}
	slli32.collection::write();
	stdoutput.printf("\n");

	stdoutput.printf("singlylinkedlist (const char *):\n");
	singlylinkedlist<const char *>	sllc;
	for (uint32_t i=0; i<10; i++) {
		sllc.append(numbers[i]);
	}
	sllc.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("avltree (int32_t):\n");
	avltree<int32_t>	ati32;
	for (int32_t i=-5; i<5; i++) {
		ati32.insert(i);
	}
	ati32.collection::write();
	stdoutput.printf("\n");

	stdoutput.printf("avltree (const char *):\n");
	avltree<const char *>	atc;
	for (uint32_t i=0; i<10; i++) {
		atc.insert(numbers[i]);
	}
	atc.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("dictionary (int32_t, const char *):\n");
	dictionary<int32_t,const char *>	d;
	for (int32_t i=0; i<10; i++) {
		d.setValue(i,numbers[i]);
	}
	d.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("table (int32_t):\n");
	table<int32_t>	ti32;
	for (int32_t i=0; i<10; i++) {
		ti32.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			ti32.setValue(i,j,j);
		}
	}
	ti32.collection::write();
	stdoutput.printf("\n");

	stdoutput.printf("table (const char *):\n");
	table<const char *>	tc;
	for (uint32_t i=0; i<10; i++) {
		tc.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			tc.setValue(i,j,numbers[j]);
		}
	}
	tc.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (int32_t):\n");
	dynamicarray<int32_t>	dai32;
	for (int32_t i=0; i<10; i++) {
		dai32[i]=i;
	}
	dai32.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (const char *):\n");
	dynamicarray<const char *>	dac;
	for (int32_t i=0; i<10; i++) {
		dac[i]=numbers[i];
	}
	dac.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (int32_t):\n");
	staticarray<int32_t,10>	sai32;
	for (int32_t i=0; i<10; i++) {
		sai32[i]=i;
	}
	sai32.collection::write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (const char *):\n");
	staticarray<const char *,10>	sac;
	for (int32_t i=0; i<10; i++) {
		sac[i]=numbers[i];
	}
	sac.collection::write();
	stdoutput.printf("\n");
}
