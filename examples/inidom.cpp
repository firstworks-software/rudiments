#include <rudiments/inidom.h>
#include <rudiments/domnode.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	inidom	ini;

	// parse an ini string
	ini.parseString(
		"[dog]\n"
		"type=mammal\n"
		"legs=4\n"
		"domestic=yes\n"
		"\n"
		"[eagle]\n"
		"type=bird\n"
		"legs=2\n"
		"domestic=no\n"
		"\n"
		"[snake]\n"
		"type=reptile\n"
		"legs=0\n"
		"domestic=no\n");


	// walk the tree
	// the dom tree structure is:
	//   root -> s (section) -> k (key, with "k" and "v" attributes)
	domnode	*root=ini.getRootNode();

	stdoutput.write("all animals:\n");
	for (domnode *section=root->getFirstTagChild()->
					getFirstTagChild("s");
			!section->isNullNode();
			section=section->getNextTagSibling("s")) {

		const char	*name=section->getAttributeValue("v");

		const char	*type=NULL;
		const char	*legs=NULL;
		const char	*domestic=NULL;

		for (domnode *key=section->getFirstTagChild("k");
				!key->isNullNode();
				key=key->getNextTagSibling("k")) {

			const char	*k=key->getAttributeValue("k");
			const char	*v=key->getAttributeValue("v");

			if (!charstring::compare(k,"type")) {
				type=v;
			} else if (!charstring::compare(k,"legs")) {
				legs=v;
			} else if (!charstring::compare(k,"domestic")) {
				domestic=v;
			}
		}

		stdoutput.printf("  %s (%s) - %s legs - domestic: %s\n",
						name,type,legs,domestic);
	}
	stdoutput.write('\n');


	// write the ini tree to standard output
	stdoutput.write("ini output:\n");
	ini.write();
}
