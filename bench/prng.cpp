// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/prng.h>
#include <rudiments/stdio.h>
#include "../test/test.cpp"

int main(int argc, const char **argv) {

	uint64_t	randmax=prng::getRandMax();
        stdoutput.printf("Generating full range of numbers (max=%lld)...\n",
								randmax);

	prng	r;
	r.setSeed(0);

	datetime	start;
	datetime	end;

	start.initFromSystemDateTime();
	for (uint64_t l=0; l<randmax; l++) {
		uint32_t	result;
		if (!r.generate(&result)) {
        		stdoutput.printf("generate failed: %lld\n",l);
			break;
		} else {
			if (l<10) {
        			stdoutput.printf("%lld: ",l);
        			stdoutput.printf("%ld\n",result);
			} else if (l==10) {
        			stdoutput.printf("...\n");
			}
		}
	}
	end.initFromSystemDateTime();

	displayTime(&start,&end);
}
