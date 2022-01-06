// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/avltree.h>
#include <rudiments/charstring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/snooze.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include "test.cpp"


const int32_t	randomlow=10000;
const int32_t	randomhigh=99999;
#ifdef SLOWSYSTEM
const uint16_t	nodecount=512;
#else
const uint16_t	nodecount=4096;
#endif


// generates a random number
template<class type>
inline
void generateRandom(randomnumber *r, type *number) {
	int32_t	num=0;
	r->generateScaledNumber(randomlow,randomhigh,&num);
	*number=(type)num;
}


// walk the tree, looking for errors in order, balance, or length
template<class type>
inline
void walk(bool forwards, avltree<type> *t,
				bool *ordererror,
				bool *balanceerror,
				bool *lengtherror) {

	*ordererror=false;
	*balanceerror=false;
	*lengtherror=false;

	uint64_t	length=0;
	type		prev;

	treenode<type> *node=(forwards)?t->getFirst():t->getLast();
	while (node) {

		if (length &&
			((forwards &&
				t->getComparator()->compare(
					node->getValue(),prev)==-1) ||
			(!forwards &&
				t->getComparator()->compare(
					node->getValue(),prev)==1))) {
			*ordererror=true;
		}
		prev=node->getValue();

		int64_t	factor=(int64_t)node->getLeftHeight()-
				(int64_t)node->getRightHeight();

		if (factor>1 || factor<-1) {
			*balanceerror=true;
		}

		if (forwards) {
			node=node->getNext();
		} else {
			node=node->getPrevious();
		}

		length++;
	}

	*lengtherror=(length!=t->getLength());
}


// walk the tree, doing a find() on each value to verify
// that it's found in the location that it's in
template<class type>
inline
void find(avltree<type> *t, bool *finderror) {

	for (treenode<type> *n=t->getFirst(); n; n=n->getNext()) {
		if (t->find(n->getValue())!=n) {
			*finderror=true;
			break;
		}
	}
	*finderror=false;
}

const char *num[10]={
	"zero","one","two","three","four","five","six","seven","eight","nine"
};

char *toString(int32_t number) {
	stringbuffer	numstr;
	uint16_t	len=charstring::integerLength(number);
	for (uint16_t i=0; i<len; i++) {
		if (i) {
			numstr.append('-');
		}
		numstr.append(num[number-(number/10*10)]);
		number/=10;
	}
	return numstr.detachString();
}


int main(int argc, char **argv) {

	header("avltree");

	// initialize random number generator
	randomnumber	r;
	uint32_t	seed=randomnumber::getSeed();
	stdoutput.printf("(seed: %d)\n\n",seed);
	if (argc==2) {
		seed=charstring::toInteger(argv[1]);
	}
	r.setSeed(seed);

	// error flags
	bool		ordererror=false;
	bool		balanceerror=false;
	bool		lengtherror=false;
	bool		finderror=false;


	// integers
	stdoutput.printf("integers: \n");
	avltree<int32_t>	intt;

	// insert
	for (uint16_t i=0; i<nodecount; i++) {

		// generate a random number
		int32_t	number=0;
		generateRandom(&r,&number);

		// insert the number
		intt.insert(number);

		// verify that the tree is still ok
		walk(true,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&intt,&finderror);
	}
	test("insert/order",!ordererror);
	test("insert/balance",!balanceerror);
	test("insert/length",!lengtherror);
	test("insert/find",!finderror);
	test("insert/ultimate length",intt.getLength()==nodecount);
	stdoutput.printf("\n");

	// remove
	for (treenode<int32_t> *node=intt.getTop();
					node; node=intt.getTop()) {

		// go left a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getLeftChild()) {
				node=node->getLeftChild();
			} else {
				break;
			}
		}

		// go right a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getRightChild()) {
				node=node->getRightChild();
			} else {
				break;
			}
		}

		// remove the node
		intt.remove(node);

		// verify that the tree is still ok
		walk(true,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&intt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&intt,&finderror);
	}
	test("remove/order",!ordererror);
	test("remove/balance",!balanceerror);
	test("remove/length",!lengtherror);
	test("remove/find",!finderror);
	test("remove/ultimate length",intt.getLength()==0);
	stdoutput.printf("\n");


	// doubles
	stdoutput.printf("doubles: \n");
	avltree<double>	dblt;

	// insert
	for (uint16_t i=0; i<nodecount; i++) {

		// generate a random number
		int32_t	integerpartint=0;
		generateRandom(&r,&integerpartint);
		int32_t	decimalpartint=0;
		generateRandom(&r,&decimalpartint);
		double	number=(double)integerpartint;
		double	decimalpart=(double)decimalpartint;
		uint16_t	exp=charstring::integerLength(decimalpartint);
		for (uint16_t j=0; j<exp; j++) {
			decimalpart=decimalpart/10;
		}
		number=number+decimalpart;
	

		// insert the number
		dblt.insert(number);

		// verify that the tree is still ok
		walk(true,&dblt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&dblt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&dblt,&finderror);
	}
	test("insert/order",!ordererror);
	test("insert/balance",!balanceerror);
	test("insert/length",!lengtherror);
	test("insert/find",!finderror);
	test("insert/ultimate length",dblt.getLength()==nodecount);
	stdoutput.printf("\n");

	// remove
	for (treenode<double> *node=dblt.getTop();
					node; node=dblt.getTop()) {

		// go left a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getLeftChild()) {
				node=node->getLeftChild();
			} else {
				break;
			}
		}

		// go right a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getRightChild()) {
				node=node->getRightChild();
			} else {
				break;
			}
		}

		// remove the node
		dblt.remove(node);

		// verify that the tree is still ok
		walk(true,&dblt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&dblt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&dblt,&finderror);
	}
	test("remove/order",!ordererror);
	test("remove/balance",!balanceerror);
	test("remove/length",!lengtherror);
	test("remove/find",!finderror);
	test("remove/ultimate length",dblt.getLength()==0);
	stdoutput.printf("\n");


	// strings
	stdoutput.printf("strings: \n");
	avltree<char *>	strt;

	// insert
	for (uint16_t i=0; i<nodecount; i++) {

		// generate a random number
		int32_t	number=0;
		generateRandom(&r,&number);
		char	*str=toString(number);

		// insert the number
		strt.insert(str);

		// verify that the tree is still ok
		walk(true,&strt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&strt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&strt,&finderror);
	}
	test("insert/order",!ordererror);
	test("insert/balance",!balanceerror);
	test("insert/length",!lengtherror);
	test("insert/find",!finderror);
	test("insert/ultimate length",strt.getLength()==nodecount);
	stdoutput.printf("\n");

	// remove
	for (treenode<char *> *node=strt.getTop();
					node; node=strt.getTop()) {

		// go left a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getLeftChild()) {
				node=node->getLeftChild();
			} else {
				break;
			}
		}

		// go right a random amount
		for (;;) {
			int32_t	yes=0;
			r.generateScaledNumber(0,1,&yes);
			if (yes && node->getRightChild()) {
				node=node->getRightChild();
			} else {
				break;
			}
		}

		// remove the node
		char	*str=node->getValue();
		strt.remove(node);
		delete[] str;

		// verify that the tree is still ok
		walk(true,&strt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		walk(false,&strt,&ordererror,&balanceerror,&lengtherror);
		if (ordererror || balanceerror || lengtherror) {
			break;
		}
		find(&strt,&finderror);
	}
	test("remove/order",!ordererror);
	test("remove/balance",!balanceerror);
	test("remove/length",!lengtherror);
	test("remove/find",!finderror);
	test("remove/ultimate length",strt.getLength()==0);
	stdoutput.printf("\n");


	// remove/clear and delete
	stdoutput.printf("remove/clear and delete...\n");
	avltree<stringbuffer *>	strbt;
	strbt.setManageValues(true);
	stringbuffer	*strb1=new stringbuffer();
	strb1->append("1");
	strbt.insert(strb1);
	stringbuffer	*strb2=new stringbuffer();
	strb2->append("2");
	strbt.insert(strb2);
	stringbuffer	*strb3=new stringbuffer();
	strb3->append("3");
	strbt.insert(strb3);
	stringbuffer	*strb4=new stringbuffer();
	strb4->append("4");
	strbt.insert(strb4);
	strbt.remove(strb3);
	test("remove, getLength(): ",strbt.getLength()==3);
	strbt.remove(strb4);
	test("remove, getLength(): ",strbt.getLength()==2);
	strbt.clear();
	test("clear, getLength(): ",!strbt.getLength());
	stdoutput.printf("\n");

	// remove/clear and array delete
	stdoutput.printf("remove/clear and array delete...\n");
	avltree<char *>	chart;
	chart.setManageArrayValues(true);
	chart.insert(charstring::duplicate("1"));
	chart.insert(charstring::duplicate("2"));
	chart.insert(charstring::duplicate("3"));
	chart.insert(charstring::duplicate("4"));
	chart.remove((char *)"1");
	test("remove, getLength(): ",chart.getLength()==3);
	chart.remove((char *)"2");
	test("remove, getLength(): ",chart.getLength()==2);
	chart.clear();
	test("clear, getLength(): ",!chart.getLength());
	stdoutput.printf("\n");

	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		avltree<char *>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		for (const char * const *v=values; *v; v++) {
			if (!i) {
				cch1.insert((char *)*v);
			} else {
				cch1.insert(charstring::duplicate((char *)*v));
			}
		}
	
		avltree<char *>	cch2(cch1);
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
			treenode<char *>	*cch1n=cch1.getFirst();
			treenode<char *>	*cch2n=cch2.getFirst();
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
