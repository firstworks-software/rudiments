#include <rudiments/linkedlist.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	linkedlist<uint32_t>	ll;

	// apppend nodes to the list
	ll.append(new linkedlistnode<uint32_t>(5));
	ll.append(new linkedlistnode<uint32_t>(6));
	ll.append(new linkedlistnode<uint32_t>(7));

	// prepend nodes to the list
	ll.prepend(new linkedlistnode<uint32_t>(2));
	ll.prepend(new linkedlistnode<uint32_t>(1));
	ll.prepend(new linkedlistnode<uint32_t>(0));

	// insert nodes before and after other nodes
	ll.insertAfter(ll.find(2),new linkedlistnode<uint32_t>(4));
	ll.insertBefore(ll.find(4),new linkedlistnode<uint32_t>(3));

	// move nodes around
	ll.append(new linkedlistnode<uint32_t>(-1));
	ll.append(new linkedlistnode<uint32_t>(-2));
	ll.append(new linkedlistnode<uint32_t>(-3));
	ll.moveBefore(ll.find(0),ll.find(-3));
	ll.moveBefore(ll.find(0),ll.find(-2));
	ll.moveAfter(ll.find(-2),ll.find(-1));

	// count
	stdoutput.printf("The list contains %lld nodes.\n\n",ll.getCount());

	// print the list
	stdoutput.write("Current contents:\n");
	ll.write();
	stdoutput.write('\n');
}
