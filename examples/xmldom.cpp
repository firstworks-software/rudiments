#include <rudiments/xmldom.h>
#include <rudiments/domnode.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	xmldom	xml;

	// parse an xml string
	xml.parseString(
		"<animals>"
		"  <animal type=\"mammal\">"
		"    <name>dog</name>"
		"    <legs>4</legs>"
		"    <domestic>yes</domestic>"
		"  </animal>"
		"  <animal type=\"bird\">"
		"    <name>eagle</name>"
		"    <legs>2</legs>"
		"    <domestic>no</domestic>"
		"  </animal>"
		"  <animal type=\"reptile\">"
		"    <name>snake</name>"
		"    <legs>0</legs>"
		"    <domestic>no</domestic>"
		"  </animal>"
		"</animals>");


	// get the root node and first child
	domnode	*root=xml.getRootNode();
	stdoutput.printf("root: %s\n\n",root->getFirstTagChild()->getName());


	// access nodes by path
	const char	*name=xml.getAttributeValueByPath(
						"/animals/animal","type");
	stdoutput.printf("first animal type: %s\n\n",name);


	// walk the tree manually
	stdoutput.write("all animals:\n");
	domnode	*animals=root->getFirstTagChild();
	for (domnode *animal=animals->getFirstTagChild("animal");
			!animal->isNullNode();
			animal=animal->getNextTagSibling("animal")) {

		// get the type attribute
		const char	*type=animal->getAttributeValue("type");

		// get the name child's text
		domnode	*namenode=animal->getFirstTagChild("name");
		const char	*aname=
			namenode->getFirstChild()->getValue();

		// get the legs child's text
		domnode	*legsnode=animal->getFirstTagChild("legs");
		const char	*legs=
			legsnode->getFirstChild()->getValue();

		// get the domestic child's text
		domnode	*domesticnode=animal->getFirstTagChild("domestic");
		const char	*domestic=
			domesticnode->getFirstChild()->getValue();

		stdoutput.printf("  %s (%s) - %s legs - domestic: %s\n",
						aname,type,legs,domestic);
	}
	stdoutput.write('\n');


	// write the xml tree to standard output
	stdoutput.write("xml output:\n");
	xml.writeXml(true);
	stdoutput.write('\n');
}
