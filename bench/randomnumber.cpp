// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>
#include "../test/test.cpp"

int main(int argc, const char **argv) {

	uint64_t	randmax=randomnumber::getRandMax();
        stdoutput.printf("Generating full range of numbers (max=%lld)...\n",
								randmax);

	randomnumber	r;
	r.setSeed(0);

	datetime	start;
	datetime	end;

	start.getSystemDateAndTime();
	for (uint64_t l=0; l<randmax; l++) {
		uint32_t	result;
		if (!r.generateNumber(&result)) {
        		stdoutput.printf("generateNumber failed: %lld\n",l);
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
	end.getSystemDateAndTime();

	displayTime(&start,&end);
}
