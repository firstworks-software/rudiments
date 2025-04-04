// Copyright (c) David Muse
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
	"zero","one","two","three","four","five","six","seven","eight","nine"
	//"0","1","2","3","4","5","6","7","8","9"
};

struct test {
	const char	*blah;
};

int main(int argc, const char **argv) {

	header("collectionprint");

	stdoutput.printf("scalar (bool):\n");
	scalar<bool>	sb;
	sb.setValue(true);
	sb.write();
	sb.setValue(false);
	sb.write();
	stdoutput.printf("\n");

	stdoutput.printf("scalar (null/non-null):\n");
	scalar<const char *>	sn;
	sn.setValue(NULL);
	sn.write();
	sn.setValue("not-null");
	sn.write();
	stdoutput.printf("\n");

	stdoutput.printf("scalar (int32_t):\n");
	scalar<int32_t>	si32;
	for (int32_t i=-5; i<5; i++) {
		si32.setValue(i);
		si32.write();
	}
	stdoutput.printf("\n");

	stdoutput.printf("scalar (const char *):\n");
	scalar<const char *>	sc;
	for (uint32_t i=0; i<10; i++) {
		sc.setValue(numbers[i]);
		sc.write();
	}
	stdoutput.printf("\n");



	stdoutput.printf("linkedlist (int32_t):\n");
	linkedlist<int32_t>	lli32;
	for (int32_t i=-5; i<5; i++) {
		lli32.append(i);
	}
	lli32.write();
	stdoutput.printf("\n");

	stdoutput.printf("linkedlist (const char *):\n");
	linkedlist<const char *>	llc;
	for (uint32_t i=0; i<10; i++) {
		llc.append(numbers[i]);
	}
	llc.write();
	stdoutput.printf("\n");



	stdoutput.printf("singlylinkedlist (int32_t):\n");
	singlylinkedlist<int32_t>	slli32;
	for (int32_t i=-5; i<5; i++) {
		slli32.append(i);
	}
	slli32.write();
	stdoutput.printf("\n");

	stdoutput.printf("singlylinkedlist (const char *):\n");
	singlylinkedlist<const char *>	sllc;
	for (uint32_t i=0; i<10; i++) {
		sllc.append(numbers[i]);
	}
	sllc.write();
	stdoutput.printf("\n");



	stdoutput.printf("avltree (int32_t):\n");
	avltree<int32_t>	ati32;
	for (int32_t i=-5; i<5; i++) {
		ati32.insert(i);
	}
	ati32.write();
	ati32.writeXml();
	ati32.writeJson();
	stdoutput.printf("\n");

	stdoutput.printf("avltree (const char *):\n");
	avltree<const char *>	atc;
	for (uint32_t i=0; i<10; i++) {
		atc.insert(numbers[i]);
	}
	atc.write();
	atc.writeXml();
	ati32.writeJson();
	stdoutput.printf("\n");



	stdoutput.printf("dictionary (int32_t, const char *):\n");
	dictionary<int32_t,const char *>	d;
	for (int32_t i=0; i<10; i++) {
		d.setValue(i,numbers[i]);
	}
	d.write();
	stdoutput.printf("\n");



	stdoutput.printf("table (int32_t):\n");
	table<int32_t>	ti32;
	for (int32_t i=0; i<10; i++) {
		ti32.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			ti32.setValue(i,j,j);
		}
	}
	ti32.write();
	stdoutput.printf("\n");

	stdoutput.printf("table (const char *):\n");
	table<const char *>	tc;
	for (uint32_t i=0; i<10; i++) {
		tc.setColumnName(i,numbers[i]);
		for (int32_t j=0; j<10; j++) {
			tc.setValue(i,j,numbers[j]);
		}
	}
	tc.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (int32_t):\n");
	dynamicarray<int32_t>	dai32;
	for (int32_t i=0; i<10; i++) {
		dai32[i]=i;
	}
	dai32.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (const char *):\n");
	dynamicarray<const char *>	dac;
	for (int32_t i=0; i<10; i++) {
		dac[i]=numbers[i];
	}
	dac.write();
	stdoutput.printf("\n");



	stdoutput.printf("dynamicarray (struct test):\n");
	dynamicarray<struct test>	dt;
	for (uint32_t i=0; i<10; i++) {
		dt[i].blah=NULL;
	}
	dt.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (int32_t):\n");
	staticarray<int32_t,10>	sai32;
	for (int32_t i=0; i<10; i++) {
		sai32[i]=i;
	}
	sai32.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (const char *):\n");
	staticarray<const char *,10>	sac;
	for (int32_t i=0; i<10; i++) {
		sac[i]=numbers[i];
	}
	sac.write();
	stdoutput.printf("\n");



	stdoutput.printf("staticarray (struct test):\n");
	staticarray<struct test, 10>	st;
	st.write();
	stdoutput.printf("\n");

	stdoutput.printf("nested staticarray (int32_t):\n");
	staticarray<staticarray<int32_t,10>,10>	nsai32;
	for (int32_t i=0; i<10; i++) {
		for (int32_t j=0; j<10; j++) {
			nsai32[i][j]=j;
		}
	}
	nsai32.write();
	stdoutput.printf("\n");

	return 0;
}
