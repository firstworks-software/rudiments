#include <rudiments/prng.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create an instance and seed it
	prng	rn;
	rn.setSeed(prng::getSeed());


	// generate some unscaled numbers
	stdoutput.write("generating some random numbers:\n");
	for (uint16_t i=0; i<20; i++) {

		uint32_t	unscalednum;
		rn.generate(&unscalednum);

		stdoutput.printf("%d, ",unscalednum);
	}
	stdoutput.write("\n\n");


	// generate some scaled numbers
	stdoutput.write("generating some scaled random numbers:\n");
	for (uint16_t j=0; j<20; j++) {

		int32_t		scalednum;
		rn.generate(&scalednum,1,10);

		stdoutput.printf("%d, ",scalednum);
	}
	stdoutput.write("\n\n");
}
