#include <rudiments/linkedlist.h>
#include <rudiments/prng.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	linkedlist<uint32_t>	llis;
	linkedlist<uint32_t>	llhs;

	// generate random numbers and append them to the lists
	prng	rr;
	rr.setSeed(prng::getSeed());

	stdoutput.printf("generating numbers...\n");
	for (uint16_t i=0; i<20000; i++) {

		uint32_t	num;
		rr.generate(&num);

		llis.append(num);
		llhs.append(num);
	}

	// sort one list inexpensively
	stdoutput.printf("sorting inexpensively...\n");
	llis.sortInexpensively();

	// sort one list quickly
	stdoutput.printf("sorting quickly...\n");
	llhs.sortQuickly();

	// print the lists
	stdoutput.printf("inexpensively sorted list\n");
	llis.write();
	stdoutput.write("...\n\n");
	stdoutput.printf("quickly sorted list\n");
	llhs.write();
	stdoutput.write("...\n\n");
}
