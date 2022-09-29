#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stdoutput.printf("random number between 0 and %d: %d\n",
			randomnumber::getRandMax(),
			randomnumber::generate(randomnumber::getSeed()));

	stdoutput.printf("random number between 1 and 10: %d\n",
			randomnumber::generate(randomnumber::getSeed(),1,10));
}
