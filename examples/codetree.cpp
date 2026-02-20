#include <rudiments/codetree.h>
#include <rudiments/domnode.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

// helper function to print a code tree
static void printtree(domnode *node, int indent) {

	for (domnode *n=node->getFirstTagChild();
			!n->isNullNode();
			n=n->getNextTagSibling()) {

		for (int i=0; i<indent; i++) {
			stdoutput.write("  ");
		}
		stdoutput.printf("%s",n->getName());

		const char	*value=n->getAttributeValue("value");
		if (value) {
			stdoutput.printf(": %s",value);
		}
		stdoutput.write('\n');

		printtree(n,indent+1);
	}
}

int main(int argc, const char **argv) {

	codetree	ct;

	// define a simple grammar for basic math expressions
	// (numbers and addition)
	const char	*grammar=
		"<grammar>"
		"  <definition name=\"expression\">"
		"    <concatenation>"
		"      <nonterminal name=\"number\"/>"
		"      <repetition min=\"0\">"
		"        <concatenation>"
		"          <terminal value=\"+\"/>"
		"          <nonterminal name=\"number\"/>"
		"        </concatenation>"
		"      </repetition>"
		"    </concatenation>"
		"  </definition>"
		"  <definition name=\"number\" type=\"literal\">"
		"    <repetition min=\"1\">"
		"      <alternation>"
		"        <terminal value=\"0\"/>"
		"        <terminal value=\"1\"/>"
		"        <terminal value=\"2\"/>"
		"        <terminal value=\"3\"/>"
		"        <terminal value=\"4\"/>"
		"        <terminal value=\"5\"/>"
		"        <terminal value=\"6\"/>"
		"        <terminal value=\"7\"/>"
		"        <terminal value=\"8\"/>"
		"        <terminal value=\"9\"/>"
		"      </alternation>"
		"    </repetition>"
		"  </definition>"
		"</grammar>";


	// parse some input
	stdoutput.write("parsing: 12+34\n\n");

	domnode		*out=new domnode(NULL);
	const char	*codeposition=NULL;

	if (ct.parse("12+34",grammar,"expression",out,&codeposition)) {

		// print the resulting tree
		stdoutput.write("parse tree:\n");
		printtree(out,0);
		stdoutput.write('\n');

		// write the tree back out as code
		stringbuffer	sb;
		ct.write(out,grammar,&sb);
		stdoutput.printf("write output: %s\n",sb.getString());

	} else {
		stdoutput.write("parse failed");
		if (codeposition) {
			stdoutput.printf(" at: %s",codeposition);
		}
		stdoutput.write('\n');
	}

	delete out;
}
