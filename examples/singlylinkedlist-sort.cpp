#include <rudiments/singlylinkedlist.h>
#include <rudiments/prng.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	singlylinkedlist<uint32_t>	sllis;
	singlylinkedlist<uint32_t>	sllhs;

	// generate random numbers and append them to the lists
	prng	rr;
	rr.setSeed(prng::getSeed());

	stdoutput.printf("generating numbers...\n");
	for (uint16_t i=0; i<20000; i++) {

		uint32_t	num;
		rr.generate(&num);

		sllis.append(num);
		sllhs.append(num);
	}

	// sort one list inexpensively
	stdoutput.printf("sorting inexpensively...\n");
	sllis.sortInexpensively();

	// sort one list quickly
	stdoutput.printf("sorting quickly...\n");
	sllhs.sortQuickly();

	// print the lists
	stdoutput.printf("inexpensively sorted list\n");
	sllis.write();
	stdoutput.write("...\n\n");
	stdoutput.printf("quickly sorted list\n");
	sllhs.write();
	stdoutput.write("...\n\n");
}
