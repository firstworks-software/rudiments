#include <rudiments/prng.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stdoutput.printf("random number between 0 and %d: %d\n",
			prng::getRandMax(),
			prng::generate(prng::getSeed()));

	stdoutput.printf("random number between 1 and 10: %d\n",
			prng::generate(prng::getSeed(),1,10));
}
