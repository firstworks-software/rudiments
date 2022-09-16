// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/resourcepool.h>
#include <rudiments/stdio.h>
#include "test.cpp"

class testclass {
	public:
		uint64_t	val;
};

int main(int argc, char **argv) {

	header("resourcepool");

	// init
	resourcepool<testclass>	r;
	stdoutput.printf("init:\n");
	test("min (before set)",r.getMin()==0);
	test("max (before set)",r.getMax()==10);
	test("growby (before set)",r.getGrowBy()==1);
	r.setMin(10);
	r.setMax(20);
	r.setGrowBy(3);
	test("min (after set)",r.getMin()==10);
	test("max (after set)",r.getMax()==20);
	test("growby (after set)",r.getGrowBy()==3);
	test("initialize",r.init());
	stdoutput.printf("\n");

	testclass	**t=new testclass *[r.getMax()];
	for (uint64_t i=0; i<r.getMax(); i++) {
		t[i]=NULL;
	}

	// borrow from initial pool
	uint64_t	count=r.getMin();
	for (uint64_t i=0; i<r.getMin(); i++) {
		stdoutput.printf("borrowed (initial) %lld:\n",i+1);
		test("available initial (before)",
			r.getAvailableInitialResourceCount()==count);
		t[i]=r.borrowResource();
		test("borrow",t[i]);
		count--;
		test("available initial (after)",
			r.getAvailableInitialResourceCount()==count);
	}
	stdoutput.printf("\n");

	// borrow from on-demand pool
	uint64_t	total=r.getMin();
	count=0;
	for (uint64_t i=0; i<r.getMax()-r.getMin(); i++) {
		stdoutput.printf("borrowed (on-demand) %lld:\n",i+1);
		test("available on-demand (before)",
			r.getAvailableOnDemandResourceCount()==count);
		t[r.getMin()+i]=r.borrowResource();
		test("borrow",t[r.getMin()+i]);
		if (!count) {
			total+=r.getGrowBy();
			count=r.getGrowBy();
			while (total>r.getMax()) {
				total--;
				count--;
			}
		}
		count--;
		test("available on-demand (after)",
			r.getAvailableOnDemandResourceCount()==count);
	}
	stdoutput.printf("\n");

	// borrow too many
	stdoutput.printf("borrow too many\n");
	test("borrow",!r.borrowResource());
	test("available initial (after)",
			!r.getAvailableInitialResourceCount());
	test("available on-demand (after)",
			!r.getAvailableOnDemandResourceCount());
	stdoutput.printf("\n");

	// return to initial pool
	count=0;
	for (uint64_t i=0; i<r.getMin(); i++) {
		stdoutput.printf("returned (initial) %lld:\n",i+1);
		test("available initial (before)",
			r.getAvailableInitialResourceCount()==count);
		r.returnResource(t[i]);
		count++;
		test("available initial (after)",
			r.getAvailableInitialResourceCount()==count);
	}
	stdoutput.printf("\n");

	// return to on-demand pool
	for (uint64_t i=0; i<r.getMax()-r.getMin(); i++) {
		stdoutput.printf("returned (on-demand) %lld:\n",i+1);
		test("available on-demand (before)",
			!r.getAvailableOnDemandResourceCount());
		r.returnResource(t[r.getMin()+i]);
		test("available on-demand (after)",
			!r.getAvailableOnDemandResourceCount());
	}
	stdoutput.printf("\n");

	// clear
	stdoutput.printf("clear:\n");
	r.clear();
	test("available initial after clear",
			!r.getAvailableInitialResourceCount());
	test("available on-demand after clear",
			!r.getAvailableOnDemandResourceCount());
	stdoutput.printf("\n");

	process::exit(0);
}
