#include <rudiments/dynamicarray.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dynamic array with an initial extent of 4
	// and incremental extents of 2
	dynamicarray<int>	da(4,2);

	// set values
	da[0]=0;
	da[1]=10;
	da[2]=20;
	da[3]=30;

	// print the array
	stdoutput.printf("Array (%lld elements):\n",da.getCount());
	for (uint64_t i=0; i<da.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,da[i]);
	}
	stdoutput.write('\n');

	// the array grows automatically when new elements are set
	da[4]=40;
	da[5]=50;
	da[6]=60;

	stdoutput.printf("After growing (%lld elements):\n",da.getCount());
	for (uint64_t i=0; i<da.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,da[i]);
	}
	stdoutput.write('\n');

	// zero out all elements
	da.zero(da.getCount());

	stdoutput.printf("After zeroing:\n");
	for (uint64_t i=0; i<da.getCount(); i++) {
		stdoutput.printf("  [%lld] = %d\n",i,da[i]);
	}
	stdoutput.write('\n');

	// clear the array
	da.clear();
	stdoutput.printf("After clearing: %lld elements\n",da.getCount());
}
