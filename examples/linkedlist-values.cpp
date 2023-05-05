#include <rudiments/linkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	linkedlist<uint32_t>	ll;

	// apppend values to the list
	ll.append(5);
	ll.append(6);
	ll.append(7);

	// prepend values to the list
	ll.prepend(2);
	ll.prepend(1);
	ll.prepend(-1);
	ll.prepend(-2);
	ll.prepend(-2);
	ll.prepend(-2);

	// insert values before and after other values
	ll.insertAfter(ll.find(2),4);
	ll.insertBefore(ll.find(4),3);

	// remove values from the list
	ll.remove(-1);

	// remove all of a particular value from the list
	ll.removeAll(-2);

	// count
	stdoutput.printf("The list contains %lld nodes.\n\n",ll.getCount());

	// print the list
	stdoutput.write("Current contents:\n");
	ll.write();
	stdoutput.write('\n');

	// clear the list
	ll.clear();
	stdoutput.printf("The list contains %lld nodes after clearing it.\n",
								ll.getCount());
}
