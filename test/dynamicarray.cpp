// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/dynamicarray.h>
#include <rudiments/staticarray.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <new>
#include <stdlib.h>
#include "test.cpp"

// number of operator new/new[] and delete/delete[] calls in the whole program,
// read before and after the statement under test
static uint64_t	newcount=0;
static uint64_t	deletecount=0;

void *operator new(size_t size) throw(std::bad_alloc) {
	newcount++;
	void	*ptr=malloc(size);
	if (!ptr) {
		throw std::bad_alloc();
	}
	return ptr;
}

void *operator new[](size_t size) throw(std::bad_alloc) {
	newcount++;
	void	*ptr=malloc(size);
	if (!ptr) {
		throw std::bad_alloc();
	}
	return ptr;
}

void operator delete(void *ptr) throw() {
	if (ptr) {
		deletecount++;
	}
	free(ptr);
}

void operator delete[](void *ptr) throw() {
	if (ptr) {
		deletecount++;
	}
	free(ptr);
}

struct test {
	char		str[100];
	uint32_t	uintval;
};

class testclass {
	public:
		testclass();
		uint32_t	getValue();
		void		setValue(uint32_t value);
	private:
		uint32_t	value;
};

testclass::testclass() {
	value=1;
}

uint32_t testclass::getValue() {
	return value;
}

void testclass::setValue(uint32_t value) {
	this->value=value;
}

#define COUNT 4096

// allocations made and allocations still held since a snapshot
struct allocsnapshot {
	uint64_t	newcount;
	uint64_t	deletecount;
};

static void takeSnapshot(allocsnapshot *snap) {
	snap->newcount=newcount;
	snap->deletecount=deletecount;
}

static uint64_t allocsSince(allocsnapshot *snap) {
	return newcount-snap->newcount;
}

static uint64_t liveSince(allocsnapshot *snap) {
	return (newcount-snap->newcount)-(deletecount-snap->deletecount);
}

static uint64_t valueAt(uint64_t index) {
	return index*3+7;
}

static void fillValues(arraycollection<uint64_t> &a, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		a[i]=valueAt(i);
	}
}

static bool checkValues(arraycollection<uint64_t> &a, uint64_t count) {
	if (a.getCount()!=count) {
		return false;
	}
	for (uint64_t i=0; i<count; i++) {
		if (a[i]!=valueAt(i)) {
			return false;
		}
	}
	return true;
}

// allocations of a new array that is filled with count values
static void freshAllocs(uint64_t initcount, uint64_t inccount, uint64_t count,
				uint64_t *allocs, uint64_t *live) {
	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	fresh(initcount,inccount);
	fillValues(fresh,count);
	*allocs=allocsSince(&snap);
	*live=liveSince(&snap);
}

// the live allocations must match those of a new array, and when the
// target started out with the same counts as the new array, so must
// the total allocations
static void testAllocs(const char *name, uint64_t allocs, uint64_t live,
				uint64_t freshallocs, uint64_t freshlive,
				bool compareallocs) {
	bool	success=(live==freshlive &&
			(!compareallocs || allocs==freshallocs));
	if (!success) {
		stdoutput.printf("\t%s: allocations %lld (new %lld), "
				"live %lld (new %lld)\n",name,
				(long long)allocs,(long long)freshallocs,
				(long long)live,(long long)freshlive);
	}
	test(name,success);
}

// assign a dynamicarray of count values to a dynamicarray that was
// created with different counts
static void testAssign(const char *name,
			uint64_t srcinit, uint64_t srcinc,
			uint64_t dstinit, uint64_t dstinc,
			uint64_t count) {

	dynamicarray<uint64_t>	src(srcinit,srcinc);
	fillValues(src,count);

	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	dst(dstinit,dstinc);
	dst=src;
	uint64_t	allocs=allocsSince(&snap);
	uint64_t	live=liveSince(&snap);

	uint64_t	freshallocs;
	uint64_t	freshlive;
	freshAllocs(srcinit,srcinc,count,&freshallocs,&freshlive);

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: values",name);
	test(msg,checkValues(dst,count) && checkValues(src,count));
	charstring::printf(msg,sizeof(msg),"%s: initial count",name);
	test(msg,dst.getInitialCount()==srcinit &&
			dst.getIncrementCount()==srcinc);
	charstring::printf(msg,sizeof(msg),"%s: allocations",name);
	testAllocs(msg,allocs,live,freshallocs,freshlive,
			(srcinit==dstinit && srcinc==dstinc));
}

// assign a staticarray to a dynamicarray
template <uint64_t count>
static void testAssignStatic(const char *name,
				uint64_t dstinit, uint64_t dstinc) {

	staticarray<uint64_t,count>	src;
	fillValues(src,count);

	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	dst(dstinit,dstinc);
	dst=src;
	uint64_t	allocs=allocsSince(&snap);
	uint64_t	live=liveSince(&snap);

	uint64_t	freshallocs;
	uint64_t	freshlive;
	freshAllocs(dstinit,dstinc,count,&freshallocs,&freshlive);

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: values",name);
	test(msg,checkValues(dst,count) && checkValues(src,count));
	charstring::printf(msg,sizeof(msg),"%s: allocations",name);
	testAllocs(msg,allocs,live,freshallocs,freshlive,true);
}

// copy construct a dynamicarray from a staticarray, as an arraycollection
template <uint64_t count>
static void testCopyStatic(const char *name) {

	staticarray<uint64_t,count>	src;
	fillValues(src,count);
	arraycollection<uint64_t>	&base=src;

	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	dst(base);
	uint64_t	allocs=allocsSince(&snap);
	uint64_t	live=liveSince(&snap);

	uint64_t	freshallocs;
	uint64_t	freshlive;
	freshAllocs(dst.getInitialCount(),dst.getIncrementCount(),
			count,&freshallocs,&freshlive);

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: values",name);
	test(msg,checkValues(dst,count) && checkValues(src,count));
	charstring::printf(msg,sizeof(msg),"%s: allocations",name);
	testAllocs(msg,allocs,live,freshallocs,freshlive,true);
}

// copy construct a dynamicarray from a dynamicarray, as an arraycollection
static void testCopyDynamic(const char *name, uint64_t count) {

	dynamicarray<uint64_t>	src(1000,32);
	fillValues(src,count);
	arraycollection<uint64_t>	&base=src;

	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	dst(base);
	uint64_t	allocs=allocsSince(&snap);
	uint64_t	live=liveSince(&snap);

	uint64_t	freshallocs;
	uint64_t	freshlive;
	freshAllocs(dst.getInitialCount(),dst.getIncrementCount(),
			count,&freshallocs,&freshlive);

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: values",name);
	test(msg,checkValues(dst,count) && checkValues(src,count));
	charstring::printf(msg,sizeof(msg),"%s: allocations",name);
	testAllocs(msg,allocs,live,freshallocs,freshlive,true);
}

// assign a chain of dynamicarrays, then grow the last one
static void testChain(const char *name, uint64_t count, uint64_t growcount) {

	dynamicarray<uint64_t>	a(1000,32);
	fillValues(a,count);

	uint64_t	freshallocs;
	uint64_t	freshlive;
	freshAllocs(1000,32,count,&freshallocs,&freshlive);

	// b=a
	allocsnapshot	snap;
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	b(1000,32);
	b=a;
	uint64_t	ballocs=allocsSince(&snap);
	uint64_t	blive=liveSince(&snap);

	// c=b
	takeSnapshot(&snap);
	dynamicarray<uint64_t>	c(1000,32);
	c=b;
	uint64_t	callocs=allocsSince(&snap);
	uint64_t	clive=liveSince(&snap);

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: b=a values",name);
	test(msg,checkValues(b,count) && checkValues(a,count));
	charstring::printf(msg,sizeof(msg),"%s: c=b values",name);
	test(msg,checkValues(c,count) && checkValues(b,count));
	charstring::printf(msg,sizeof(msg),"%s: b=a allocations",name);
	testAllocs(msg,ballocs,blive,freshallocs,freshlive,true);
	charstring::printf(msg,sizeof(msg),"%s: c=b allocations",name);
	testAllocs(msg,callocs,clive,freshallocs,freshlive,true);

	// grow the assigned array
	takeSnapshot(&snap);
	for (uint64_t i=count; i<growcount; i++) {
		c[i]=valueAt(i);
	}
	uint64_t	growallocs=allocsSince(&snap);
	uint64_t	growlive=liveSince(&snap);

	// grow a new array in the same way
	dynamicarray<uint64_t>	fresh(1000,32);
	fillValues(fresh,count);
	takeSnapshot(&snap);
	for (uint64_t i=count; i<growcount; i++) {
		fresh[i]=valueAt(i);
	}
	uint64_t	freshgrowallocs=allocsSince(&snap);
	uint64_t	freshgrowlive=liveSince(&snap);

	charstring::printf(msg,sizeof(msg),"%s: grow values",name);
	test(msg,checkValues(c,growcount) && checkValues(b,count));
	charstring::printf(msg,sizeof(msg),"%s: grow allocations",name);
	testAllocs(msg,growallocs,growlive,
			freshgrowallocs,freshgrowlive,true);
}

// assign a dynamicarray to itself
static void testSelfAssign(const char *name, uint64_t count) {

	dynamicarray<uint64_t>	a(1000,32);
	fillValues(a,count);
	dynamicarray<uint64_t>	&ref=a;
	arraycollection<uint64_t>	&base=a;

	allocsnapshot	snap;
	takeSnapshot(&snap);
	a=ref;
	uint64_t	allocs=allocsSince(&snap);
	uint64_t	live=liveSince(&snap);
	a=base;
	uint64_t	baseallocs=allocsSince(&snap)-allocs;
	uint64_t	baselive=liveSince(&snap)-live;

	char	msg[200];
	charstring::printf(msg,sizeof(msg),"%s: values",name);
	test(msg,checkValues(a,count));
	charstring::printf(msg,sizeof(msg),"%s: allocations",name);
	test(msg,allocs==0 && live==0 && baseallocs==0 && baselive==0);
}


int main(int argc, const char **argv) {

	header("dynamicarray");

	// Create a dynamicarray
	dynamicarray<struct test>	da1;
	stdoutput.printf("create <test>...\n");
	test("create, getCount()",da1.getCount()==0);
	test("create, getInitialCount()",da1.getInitialCount()==128);
	test("create, getIncrementCount()",da1.getIncrementCount()==32);
	da1.clear(25,10);
	test("clear, getInitialCount()",da1.getInitialCount()==25);
	test("clear, getIncrementCount()",da1.getIncrementCount()==10);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("set values...\n");
	uint32_t	count=COUNT;
	uint32_t	i;
	for (i=0; i<count; i++) {
		char	*val=charstring::parseNumber(i);
		charstring::copy(da1[i].str,val);
		delete[] val;
		da1[i].uintval=i;
	}
	test("get values, getCount()",(uint32_t)da1.getCount()==count);
	bool	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using =
	stdoutput.printf("copy (using =)...\n");
	dynamicarray<struct test>	da1copy1(25,10);
	da1copy1=da1;
	test("get values, getCount()",(uint32_t)da1copy1.getCount()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1copy1[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1copy1[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1copy1[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1copy1[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// copy using constructor
	stdoutput.printf("copy (using constructor)...\n");
	dynamicarray<struct test>	da1copy2(da1);
	test("get values, getCount()",(uint32_t)da1copy2.getCount()==count);
	success=true;
	for (i=0; success && i<count; i++) {
		char	*val=charstring::parseNumber(i);
		success=!charstring::compare(da1copy2[i].str,val);
		delete[] val;
	}
	test("get values, string",success);
	for (i=0; success && i<count; i++) {
		success=(da1copy2[i].uintval==i);
	}
	test("get values, int",success);
	for (i=count; success && i>0; i--) {
		char	*val=charstring::parseNumber(i-1);
		success=!charstring::compare(da1copy2[i-1].str,val);
		delete[] val;
	}
	test("get values, string (backwards)",success);
	for (i=count; success && i>0; i--) {
		success=(da1copy2[i-1].uintval==i-1);
	}
	test("get values, int (backwards)",success);
	stdoutput.printf("\n");

	// assignment and copy extents
	stdoutput.printf("assignment and copy (extents)...\n");
	testAssign("assign, same counts",1000,32,1000,32,1);
	testAssign("assign, different counts",1000,32,500,16,1);
	testAssign("assign, multiple extents",100,10,25,10,2500);
	testAssign("assign, same counts, multiple extents",100,10,100,10,2500);
	testAssignStatic<100>("assign from staticarray of 100",1000,32);
	testAssignStatic<1500>("assign from staticarray of 1500",1000,32);
	testAssignStatic<5000>("assign from staticarray of 5000",1000,32);
	testCopyStatic<129>("copy from staticarray of 129");
	testCopyStatic<1000>("copy from staticarray of 1000");
	testCopyStatic<5000>("copy from staticarray of 5000");
	testCopyDynamic("copy from dynamicarray (base) of 129",129);
	testCopyDynamic("copy from dynamicarray (base) of 1000",1000);
	testCopyDynamic("copy from dynamicarray (base) of 5000",5000);
	testChain("chain of assignments",5000,20000);
	testSelfAssign("self-assignment",2500);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da1.clear();
	test("clear, getCount()",da1.getCount()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray<testclass>	da2(25,10);
	stdoutput.printf("create <testclass>...\n");
	test("create, getCount()",da2.getCount()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		success=(da2[i].getValue()==1);
	}
	test("get values, default",success);
	test("get values, getCount()",(uint32_t)da2.getCount()==count);
	for (i=0; success && i<count; i++) {
		da2[i].setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(da2[i].getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(da2[i-1].getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da2.clear();
	test("clear, getCount()",da2.getCount()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray<testclass *>	da3(25,10);
	stdoutput.printf("create <testclass *>...\n");
	test("create, getCount()",da3.getCount()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<count; i++) {
		da3[i]=new testclass();
	}
	for (i=0; success && i<count; i++) {
		success=(da3[i]->getValue()==1);
	}
	test("get values, default",success);
	for (i=0; i<count; i++) {
		da3[i]->setValue(2);
	}
	for (i=0; success && i<count; i++) {
		success=(da3[i]->getValue()==2);
	}
	test("get values, forwards",success);
	for (i=count; success && i>0; i--) {
		success=(da3[i-1]->getValue()==2);
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	for (i=0; i<count; i++) {
		delete da3[i];
	}
	da3.clear();
	test("clear, getCount()",da3.getCount()==0);
	stdoutput.printf("\n");



	// create
	dynamicarray< dynamicarray< uint32_t > >	da4(25,10);
	stdoutput.printf("create nested...\n");
	test("create, getCount()",da4.getCount()==0);
	stdoutput.printf("\n");

	// set/check values
	stdoutput.printf("get values...\n");
	count=COUNT;
	success=true;
	for (i=0; i<10; i++) {
		for (uint32_t j=0; j<count; j++) {
			da4[i][j]=i*count+j;
		}
	}
	for (i=0; success && i<10; i++) {
		for (uint32_t j=0; success && j<count; j++) {
			success=(da4[i][j]==i*count+j);
		}
	}
	test("get values, forwards",success);
	for (i=10; success && i>0; i--) {
		for (uint32_t j=count; success && j>0; j--) {
			success=(da4[i-1][j-1]==(i-1)*count+(j-1));
		}
	}
	test("get values, backwards",success);
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear...\n");
	da4.clear();
	test("clear, getCount()",da4.getCount()==0);
	stdoutput.printf("\n");



	// create, zero, and clear (successful if it doesn't crash)
	stdoutput.printf("create, zero and clear...\n");
	for (uint16_t i=0; i<1024; i++) {
		dynamicarray<struct test *>	da5;
		da5.setManageValues(true);
		da5.zero(i);
		da5.clear();
	}
	test("create, zero, clear",true);
	stdoutput.printf("\n");
	


	// copy and assignment
	for (uint16_t i=0; i<2; i++) {

		stdoutput.printf("copy/assignment%s...\n",(!i)?"":" (managed)");

		dynamicarray<char *>	cch1;
		cch1.setManageArrayValues(i);
		const char *values[]={
			"a","b","c","d","e","f","g","h","i","j","k","l","m",
			"n","o","p","q","r","s","t","u","v","w","x","y","z",
			NULL
		};
		uint16_t	k=0;
		for (const char * const *v=values; *v; v++) {
			if (!i) {
				cch1[k]=(char *)*v;
			} else {
				cch1[k]=charstring::duplicate((char *)*v);
			}
			k++;
		}
	
		// copy/assign to another dynamicarray...
		dynamicarray<char *>	cch2(cch1);
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
			for (k=0; k<26; k++) {
				if (charstring::compare(cch1[k],cch2[k])) {
					success=false;
					break;
				}
			}
			test((!j)?"copy: values":"assignment: values",success);
		}
	
		// copy/assign to a staticarray...
		staticarray<char *,26>	cch3(cch1);
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
			test((!j)?"copy (staticarray): manage values":
					"assignment: manage values",
						cch3.getManageValues()==
						cch1.getManageValues());
			test((!j)?"copy (staticarray): manage array values":
					"assignment: manage array values",
						cch3.getManageArrayValues()==
						cch1.getManageArrayValues());

			// verify length
			test((!j)?"copy (staticarray): length":
						"assignment: length",
						cch3.getCount()==
						cch1.getCount());

			// verify values
			bool	success=true;
			for (k=0; k<26; k++) {
				if (charstring::compare(cch1[k],cch3[k])) {
					success=false;
					break;
				}
			}
			test((!j)?"copy (staticarray): values":
						"assignment: values",success);
		}
	}
	stdoutput.printf("\n");

	return 0;
}
