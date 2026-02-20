#include <rudiments/staticarray.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a static array of 5 elements
	staticarray<int,5>	sa;

	// zero all elements
	sa.zero();

	// set values
	sa[0]=0;
	sa[1]=10;
	sa[2]=20;
	sa[3]=30;
	sa[4]=40;

	// print the array
	stdoutput.printf("Array (%lld elements):\n",sa.getCount());
	for (uint64_t i=0; i<sa.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,sa[i]);
	}
	stdoutput.write('\n');

	// modify values
	sa[2]=200;
	sa[4]=400;

	stdoutput.printf("After modification:\n");
	for (uint64_t i=0; i<sa.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,sa[i]);
	}
	stdoutput.write('\n');

	// zero a range of elements
	sa.zero(1,3);

	stdoutput.printf("After zeroing elements 1-3:\n");
	for (uint64_t i=0; i<sa.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,sa[i]);
	}
	stdoutput.write('\n');

	// clear the array
	sa.clear();
	stdoutput.printf("After clearing: %lld elements\n",sa.getCount());
}
