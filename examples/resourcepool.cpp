#include <rudiments/resourcepool.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a pool of stringbuffers...
	resourcepool<stringbuffer>	pool;
	pool.setMin(3);
	pool.setMax(5);
	pool.setGrowBy(1);
	pool.create();

	stdoutput.printf("pool created:\n");
	stdoutput.printf("  min:             %lld\n",pool.getMin());
	stdoutput.printf("  max:             %lld\n",pool.getMax());
	stdoutput.printf("  growby:          %lld\n",pool.getGrowBy());
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.printf("  available ondem: %lld\n",
				pool.getAvailableOnDemandResourceCount());
	stdoutput.write('\n');


	// borrow a resource from the pool...
	stringbuffer	*sb1=pool.borrowResource();

	stdoutput.printf("after borrowing 1:\n");
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.write('\n');


	// use the resource...
	sb1->append("hello, ");
	sb1->append("goodbye.");
	stdoutput.printf("sb1 contents: \"%s\"\n",sb1->getString());
	stdoutput.write('\n');


	// borrow the remaining initial resources...
	stringbuffer	*sb2=pool.borrowResource();
	stringbuffer	*sb3=pool.borrowResource();

	stdoutput.printf("after borrowing all initial resources:\n");
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.printf("  available ondem: %lld\n",
				pool.getAvailableOnDemandResourceCount());
	stdoutput.write('\n');


	// borrow one more, which triggers on-demand creation...
	stringbuffer	*sb4=pool.borrowResource();

	stdoutput.printf("after borrowing 1 on-demand:\n");
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.printf("  available ondem: %lld\n",
				pool.getAvailableOnDemandResourceCount());
	stdoutput.write('\n');


	// return resources to the pool...
	pool.returnResource(sb1);
	pool.returnResource(sb2);
	pool.returnResource(sb3);
	pool.returnResource(sb4);

	stdoutput.printf("after returning all resources:\n");
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.printf("  available ondem: %lld\n",
				pool.getAvailableOnDemandResourceCount());
	stdoutput.write('\n');


	// clear the pool...
	pool.clear();

	stdoutput.printf("after clear:\n");
	stdoutput.printf("  available init:  %lld\n",
				pool.getAvailableInitialResourceCount());
	stdoutput.printf("  available ondem: %lld\n",
				pool.getAvailableOnDemandResourceCount());
}
