#include <rudiments/propdom.h>
#include <rudiments/domnode.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	propdom	prop;

	// parse a properties string
	prop.parseString(
		"# animal types\n"
		"dog=mammal\n"
		"eagle=bird\n"
		"snake=reptile\n");


	// walk the tree
	// the dom tree structure is:
	//   root -> k (key, with "k" and "v" attributes)
	//           p (pound comment, with "v" attribute)
	domnode	*root=prop.getRootNode();

	stdoutput.write("all animals:\n");
	for (domnode *node=root->getFirstTagChild()->
					getFirstTagChild("k");
			!node->isNullNode();
			node=node->getNextTagSibling("k")) {

		const char	*name=node->getAttributeValue("k");
		const char	*type=node->getAttributeValue("v");

		stdoutput.printf("  %s = %s\n",name,type);
	}
	stdoutput.write('\n');


	// write the properties tree to standard output
	stdoutput.write("properties output:\n");
	prop.write();
}
