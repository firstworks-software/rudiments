// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/singlylinkedlist.h>
#include <rudiments/linkedlist.h>
#include <rudiments/avltree.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// value that counts its deletions
class countedvalue {
	public:
		countedvalue(int32_t value) : value(value) {
		}
		~countedvalue() {
			deleted++;
		}
		int32_t		value;
		static uint32_t	deleted;
};

uint32_t	countedvalue::deleted=0;

// true if l has exactly the n values in v, in order, with a correct last
static bool listis(singlylinkedlist<int32_t> *l,
			const int32_t *v, uint64_t n) {
	if (l->getCount()!=n) {
		return false;
	}
	listnode<int32_t>	*node=l->getFirst();
	listnode<int32_t>	*prev=NULL;
	for (uint64_t i=0; i<n; i++) {
		if (!node || node->getValue()!=v[i]) {
			return false;
		}
		prev=node;
		node=node->getNext();
	}
	return !node && l->getLast()==prev;
}

int main(int argc, char **argv) {

	header("singlylinkedlist");

	// integers
	stdoutput.printf("integers:\n");
	singlylinkedlist<int32_t>	intl;

	// append
	intl.append((int32_t)0);
	test("append(0)/getCount()",intl.getCount()==1);
	test("append(0)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(0)/walk backwards",intl.getLast()->getValue()==0);
	intl.append((int32_t)1);
	test("append(1)/getCount()",intl.getCount()==2);
	test("append(1)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(1)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	intl.append((int32_t)3);
	test("append(3)/getCount()",intl.getCount()==3);
	test("append(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("append(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("append(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==3);
	test("append(3)/walk backwards",intl.getLast()->getValue()==3);

	// insert
	intl.insertAfter(intl.getFirst()->getNext(),2);
	test("insertAfter(2,2)/getCount()",intl.getCount()==4);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getValue()==0);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("insertAfter(2,2)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getNext()->
						getValue()==3);
	test("insertAfter(2,2)/walk backwards",intl.getLast()->getValue()==3);

	// remove
	intl.remove(3);
	test("remove(3)/getCount()",intl.getCount()==3);
	test("remove(3)/walk forwards",intl.getFirst()->getValue()==0);
	test("remove(3)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("remove(3)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("remove(3)/walk backwards",intl.getLast()->getValue()==2);

	// remove all, no match
	test("removeAll(9)",intl.removeAll(9));
	test("removeAll(9)/getCount()",intl.getCount()==3);
	test("removeAll(9)/walk forwards",intl.getFirst()->getValue()==0);
	test("removeAll(9)/walk forwards",intl.getFirst()->
						getNext()->getValue()==1);
	test("removeAll(9)/walk forwards",intl.getFirst()->getNext()->
						getNext()->getValue()==2);
	test("removeAll(9)/walk backwards",intl.getLast()->getValue()==2);

	// clear
	intl.clear();
	test("clear()/getCount()",intl.getCount()==0);
	test("clear()/getFirst()",!intl.getFirst());
	test("clear()/getLast()",!intl.getLast());
	stdoutput.printf("\n\n");

	// empty list
	stdoutput.printf("empty list:\n");
	singlylinkedlist<int32_t>	emptyl;
	singlylinkedlist<int32_t>	otherl;
	otherl.append((int32_t)1);
	otherl.append((int32_t)2);
	test("empty remove(value)",!emptyl.remove(1));
	test("empty remove(value)/getCount()",emptyl.getCount()==0);
	test("empty removeAll(value)",emptyl.removeAll(1));
	test("empty remove(node)",!emptyl.remove(otherl.getFirst()));
	test("empty remove(node)/getCount()",emptyl.getCount()==0);
	test("empty remove(NULL)",!emptyl.remove((listnode<int32_t> *)NULL));
	emptyl.detach(NULL);
	test("empty detach(NULL)/getCount()",emptyl.getCount()==0);
	emptyl.detach(otherl.getFirst());
	test("empty detach(node)/getCount()",emptyl.getCount()==0);
	test("empty detach(node)/node",otherl.getFirst()->getNext()==
							otherl.getLast());
	emptyl.moveAfter(otherl.getFirst(),otherl.getLast());
	test("empty moveAfter()/getCount()",emptyl.getCount()==0);
	test("empty moveAfter()/getFirst()",!emptyl.getFirst());
	test("empty moveAfter()/getLast()",!emptyl.getLast());
	listnode<int32_t>	*newnode=new singlylinkedlistnode<int32_t>(5);
	emptyl.insertAfter(otherl.getFirst(),newnode);
	test("empty insertAfter(node)/getCount()",emptyl.getCount()==0);
	test("empty insertAfter(node)/getFirst()",!emptyl.getFirst());
	delete newnode;
	emptyl.insertAfter(otherl.getFirst(),(int32_t)5);
	test("empty insertAfter(value)/getCount()",emptyl.getCount()==0);
	test("empty insertAfter(value)/getFirst()",!emptyl.getFirst());
	test("other list intact",otherl.getCount()==2 &&
				otherl.getFirst()->getValue()==1 &&
				otherl.getLast()->getValue()==2);
	stdoutput.printf("\n\n");


	// nodes of another list
	stdoutput.printf("nodes of another list:\n");
	const int32_t	mine[]={1,2,3};
	const int32_t	theirs[]={10,20,30};
	for (uint16_t i=0; i<3; i++) {
	for (uint16_t j=0; j<3; j++) {
		singlylinkedlist<int32_t>	mel;
		singlylinkedlist<int32_t>	thl;
		for (uint16_t k=0; k<3; k++) {
			mel.append(mine[k]);
			thl.append(theirs[k]);
		}

		// my node i, foreign node j
		listnode<int32_t>	*mn=mel.getFirst();
		listnode<int32_t>	*fn=thl.getFirst();
		for (uint16_t k=0; k<i; k++) {
			mn=mn->getNext();
		}
		for (uint16_t k=0; k<j; k++) {
			fn=fn->getNext();
		}

		// detach
		if (i==j) {
			mel.detach(fn);
			test("foreign detach(node)/list",
					listis(&mel,mine,3));
			test("foreign detach(node)/other",
					listis(&thl,theirs,3));
		}

		// move a foreign node after my node
		mel.moveAfter(mn,fn);
		test("foreign moveAfter(node)/list",listis(&mel,mine,3));
		test("foreign moveAfter(node)/other",listis(&thl,theirs,3));

		// move my node after a foreign node
		mel.moveAfter(fn,mn);
		test("moveAfter(foreign)/list",listis(&mel,mine,3));
		test("moveAfter(foreign)/other",listis(&thl,theirs,3));
	}
	}
	stdoutput.printf("\n\n");


	// strings
	stdoutput.printf("strings:\n");
	singlylinkedlist<const char *>	strl;

	// append
	strl.append("zero");
	test("append(zero)/getCount()",strl.getCount()==1);
	test("append(zero)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(zero)/walk backwards",!charstring::compare(
						strl.getLast()->getValue(),
						"zero"));
	strl.append("one");
	test("append(one)/getCount()",strl.getCount()==2);
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->getValue(),
						"zero"));
	test("append(one)/walk forwards",!charstring::compare(
						strl.getFirst()->
						getNext()->getValue(),
						"one"));
	strl.append("three");
	test("append(three)/getCount()",strl.getCount()==3);
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

	// insert
	strl.insertAfter(strl.getFirst()->getNext(),"two");
	test("insertAfter(two,two)/getCount()",strl.getCount()==4);
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

	// remove
	strl.remove("three");
	test("remove(three)/getCount()",strl.getCount()==3);
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

	// clear
	strl.clear();
	test("clear()/getCount()",strl.getCount()==0);
	test("clear()/getFirst()",!strl.getFirst());
	test("clear()/getLast()",!strl.getLast());
	stdoutput.printf("\n");


	// sort quickly
	stdoutput.printf("sortQuickly() (from exactly backwards)\n");
	strl.append("5");
	strl.append("4");
	strl.append("3");
	strl.append("2");
	strl.append("1");
	strl.append("0");
	strl.sortQuickly();
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

	stdoutput.printf("sortQuickly() (after sorted from backwards)\n");
	strl.sortQuickly();
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
	stdoutput.printf("sortQuickly() (from random order)\n");
	strl.append("4");
	strl.append("5");
	strl.append("2");
	strl.append("6");
	strl.append("1");
	strl.append("7");
	strl.append("3");
	strl.append("0");
	strl.sortQuickly();
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

	stdoutput.printf("sortQuickly() (after sorted from random order)\n");
	strl.sortQuickly();
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

	stdoutput.printf("sortQuickly() (reverse comparator)\n");
	strl.getComparator()->setReverse(true);
	strl.sortQuickly();
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

	stdoutput.printf("sortQuickly() (forward comparator)\n");
	strl.getComparator()->setReverse(false);
	strl.sortQuickly();
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
	stdoutput.printf("\n");


	// remove/clear and delete
	stdoutput.printf("remove/clear and delete...\n");
	singlylinkedlist<stringbuffer *>	strbl;
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
	test("remove, getCount(): ",strbl.getCount()==3);
	strbl.remove(strb4);
	test("remove, getCount(): ",strbl.getCount()==2);
	strbl.clear();
	test("clear, getCount(): ",!strbl.getCount());
	stdoutput.printf("\n");

	// remove/clear and array delete
	stdoutput.printf("remove/clear and array delete...\n");
	singlylinkedlist<char *>	charl;
	charl.setManageArrayValues(true);
	charl.append(charstring::duplicate("1"));
	charl.append(charstring::duplicate("2"));
	charl.append(charstring::duplicate("3"));
	charl.append(charstring::duplicate("4"));
	charl.remove((char *)"1");
	test("remove, getCount(): ",charl.getCount()==3);
	charl.remove((char *)"2");
	test("remove, getCount(): ",charl.getCount()==2);
	charl.clear();
	test("clear, getCount(): ",!charl.getCount());
	stdoutput.printf("\n");

	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		singlylinkedlist<char *>	cch1;
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
	
		singlylinkedlist<char *>	cch2(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
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
						cch2.getCount()==
						cch1.getCount());

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
	
		linkedlist<char *>	cch3(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
				cch3.clear();
			}
			if (j) {
				cch3=cch1;
			}

			// verify flags
			test((!j)?"copy (linkedlist): manage values":
				"assignment (linkedlist): manage values",
						cch3.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy (linkedlist): manage array values":
				"assignment (linkedlist): manage array values",
						cch3.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy (linkedlist): length":
					"assignment (linkedlist): length",
						cch3.getCount()==
						cch1.getCount());

			// verify values
			bool	success=true;
			listnode<char *>	*cch1n=cch1.getFirst();
			listnode<char *>	*cch3n=cch3.getFirst();
			while (cch1n) {
				if (charstring::compare(cch1n->getValue(),
							cch3n->getValue())) {
					success=false;
					break;
				}
				cch1n=cch1n->getNext();
				cch3n=cch3n->getNext();
			}
			test((!j)?"copy (linkedlist): values":
					"assignment (linkedlist): values",
					success);
		}

		avltree<char *>	cch4(cch1);
		for (uint16_t j=0; j<3; j++) {

			// 1st iteration is copy
			// 2nd is assignment
			// 3rd is assignment after clear
			if (j==2) {
				cch4.clear();
			}
			if (j) {
				cch4=cch1;
			}

			// verify flags
			test((!j)?"copy (avltree): manage values":
					"assignment: manage values",
						cch4.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy (avltree): "
						"manage array values":
					"assignment (avltree): "
						"manage array values",
						cch4.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy (avltree): length":
					"assignment (avltree): length",
						cch4.getCount()==
						cch1.getCount());

			// verify values
			bool	success=true;
			listnode<char *>	*cch1n=cch1.getFirst();
			treenode<char *>	*cch4n=cch4.getFirst();
			while (cch1n) {
				if (charstring::compare(cch1n->getValue(),
							cch4n->getValue())) {
					success=false;
					break;
				}
				cch1n=cch1n->getNext();
				cch4n=cch4n->getNext();
			}
			test((!j)?"copy (avltree): values":
					"assignment (avltree): values",
					success);
		}
	}
	stdoutput.printf("\n");

	// 11172 dictionary - a nan key cannot be found
	// nan sorts after every number, reverse puts nan first
	stdoutput.printf("nan ordering:\n");
	volatile double	zero=0;
	double		nan=zero/zero;
	double		inf=1/zero;
	for (uint16_t r=0; r<2; r++) {
		singlylinkedlist<double>	nl;
		nl.getComparator()->setReverse(r==1);
		nl.append(nan);
		nl.append(inf);
		nl.append(1.5);
		nl.append(-inf);
		nl.append(nan);
		nl.sortQuickly();
		listnode<double>	*n=nl.getFirst();
		double	v[5];
		uint16_t	c=0;
		while (n && c<5) {
			v[c++]=n->getValue();
			n=n->getNext();
		}
		bool	ok=(c==5 && !n);
		if (ok && r==0) {
			ok=(v[0]==-inf && v[1]==1.5 && v[2]==inf &&
				v[3]!=v[3] && v[4]!=v[4]);
		} else if (ok) {
			ok=(v[0]!=v[0] && v[1]!=v[1] && v[2]==inf &&
				v[3]==1.5 && v[4]==-inf);
		}
		test((r)?"nan ordering (reverse)":"nan ordering",ok);
		test("find(nan)",nl.find(nan)!=NULL);
		test("remove(nan)",nl.remove(nan));
		test("remove(nan): getCount",nl.getCount()==4);
	}
	stdoutput.printf("\n");

	// 10835 container setters should free the old value when manage
	// values is set
	// The node can't see the list's manage flags, so it never frees.
	stdoutput.printf("node setValue() and the old value...\n");
	{
		singlylinkedlist<countedvalue *>	cvl;
		cvl.setManageValues(true);
		countedvalue	*old=new countedvalue(1);
		cvl.append(old);
		uint32_t	before=countedvalue::deleted;
		cvl.getFirst()->setValue(new countedvalue(2));
		test("managed, node setValue(): old value not freed",
					countedvalue::deleted==before);
		test("managed, node setValue(): new value stored",
					cvl.getFirst()->getValue()->value==2);
		delete old;
		cvl.clear();
		test("managed, clear(): new value freed",
					countedvalue::deleted==before+2);
	}
	stdoutput.printf("\n");
	return 0;
}
