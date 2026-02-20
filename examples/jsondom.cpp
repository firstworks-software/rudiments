#include <rudiments/jsondom.h>
#include <rudiments/domnode.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	jsondom	json;

	// parse a json string
	json.parseString(
		"{"
		"  \"animals\": ["
		"    {"
		"      \"name\": \"dog\","
		"      \"type\": \"mammal\","
		"      \"legs\": 4,"
		"      \"domestic\": true"
		"    },"
		"    {"
		"      \"name\": \"eagle\","
		"      \"type\": \"bird\","
		"      \"legs\": 2,"
		"      \"domestic\": false"
		"    },"
		"    {"
		"      \"name\": \"snake\","
		"      \"type\": \"reptile\","
		"      \"legs\": 0,"
		"      \"domestic\": false"
		"    }"
		"  ]"
		"}");


	// walk the tree
	// the dom tree structure is:
	//   r (root) -> animals (array) -> v (value) -> name, type, legs, ...
	domnode	*root=json.getRootNode();
	domnode	*animals=root->getFirstTagChild()->
				getFirstTagChild("animals");

	stdoutput.write("all animals:\n");
	for (domnode *v=animals->getFirstTagChild("v");
			!v->isNullNode();
			v=v->getNextTagSibling("v")) {

		const char	*name=v->getFirstTagChild("name")->
						getAttributeValue("v");
		const char	*type=v->getFirstTagChild("type")->
						getAttributeValue("v");
		const char	*legs=v->getFirstTagChild("legs")->
						getAttributeValue("v");
		const char	*domestictype=
			v->getFirstTagChild("domestic")->
						getAttributeValue("t");
		const char	*domestic=
			(!charstring::compare(domestictype,"t"))?
							"yes":"no";

		stdoutput.printf("  %s (%s) - %s legs - domestic: %s\n",
						name,type,legs,domestic);
	}
	stdoutput.write('\n');


	// write the dom tree to standard output
	stdoutput.write("dom output:\n");
	json.write(true);
}
