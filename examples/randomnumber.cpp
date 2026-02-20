#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// generate a random number using the static method
	stdoutput.printf("random number: %d\n\n",
			randomnumber::generate(randomnumber::getSeed()));


	// generate a scaled random number between 1 and 100
	stdoutput.printf("random number (1-100): %d\n\n",
			randomnumber::generate(
				randomnumber::getSeed(),1,100));


	// scale an arbitrary number into a range
	stdoutput.printf("scale 12345 to 1-10: %d\n\n",
			randomnumber::scale(12345,1,10));


	// print the maximum random number value
	stdoutput.printf("max random number: %u\n",
			randomnumber::getRandMax());
}
