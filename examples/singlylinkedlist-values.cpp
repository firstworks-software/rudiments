#include <rudiments/singlylinkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	singlylinkedlist<uint32_t>	sll;

	// apppend values to the list
	sll.append(5);
	sll.append(6);
	sll.append(7);

	// prepend values to the list
	sll.prepend(2);
	sll.prepend(1);
	sll.prepend(-1);
	sll.prepend(-2);
	sll.prepend(-2);
	sll.prepend(-2);

	// insert values before and after other values
	sll.insertAfter(sll.find(2),3);
	sll.insertAfter(sll.find(3),4);

	// remove values from the list
	sll.remove(-1);

	// remove all of a particular value from the list
	sll.removeAll(-2);

	// count
	stdoutput.printf("The list contains %lld nodes.\n\n",sll.getCount());

	// print the list
	stdoutput.write("Current contents:\n");
	sll.write();
	stdoutput.write('\n');

	// clear the list
	sll.clear();
	stdoutput.printf("The list contains %lld nodes after clearing it.\n",
								sll.getCount());
}
