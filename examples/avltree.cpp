#include <rudiments/avltree.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	avltree<int32_t>	tree;

	// insert some values
	tree.insert(5);
	tree.insert(3);
	tree.insert(7);
	tree.insert(1);
	tree.insert(4);
	tree.insert(6);
	tree.insert(9);
	tree.insert(2);
	tree.insert(8);

	stdoutput.printf("count: %lld\n\n",tree.getCount());


	// get the top node
	treenode<int32_t>	*top=tree.getTop();
	stdoutput.printf("top value: %d\n",top->getValue());
	stdoutput.printf("  left height:  %d\n",top->getLeftHeight());
	stdoutput.printf("  right height: %d\n\n",top->getRightHeight());


	// walk the tree in order using getFirst()/getNext()
	stdoutput.write("in-order traversal:\n");
	for (treenode<int32_t> *node=tree.getFirst();
				node; node=node->getNext()) {
		stdoutput.printf("  %d\n",node->getValue());
	}
	stdoutput.write('\n');


	// find a value
	treenode<int32_t>	*found=tree.find(7);
	if (found) {
		stdoutput.printf("found: %d\n",found->getValue());
	}


	// find a value that doesn't exist
	found=tree.find(10);
	stdoutput.printf("find 10: %s\n\n",(found)?"found":"not found");


	// remove a value
	tree.remove(5);
	stdoutput.write("removed 5\n");
	stdoutput.printf("count: %lld\n\n",tree.getCount());


	// walk again after removal
	stdoutput.write("in-order traversal after removal:\n");
	for (treenode<int32_t> *node=tree.getFirst();
				node; node=node->getNext()) {
		stdoutput.printf("  %d\n",node->getValue());
	}
	stdoutput.write('\n');


	// clear the tree
	tree.clear();
	stdoutput.printf("after clear, count: %lld\n",tree.getCount());
}
