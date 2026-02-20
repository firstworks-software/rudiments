#include <rudiments/compiler.h>
#include <rudiments/codetree.h>
#include <rudiments/domnode.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

// a simple preprocessor that logs input
class mypreprocessor : public compilerpreprocessor {
	public:
		bool	process(const char *input,
					stringbuffer *output,
					domnode *metadata) {
			stdoutput.printf("preprocessor input: %s\n",input);
			output->append(input);
			return true;
		}
};

// a simple processor that transforms the code tree
class myprocessor : public compilerprocessor {
	public:
		bool	process(domnode *tree,
					domnode *metadata) {
			stdoutput.write("processor: walking tree\n");
			return true;
		}
};

// a simple postprocessor that logs output
class mypostprocessor : public compilerpostprocessor {
	public:
		bool	process(const char *input,
					stringbuffer *output,
					domnode *metadata) {
			stdoutput.printf("postprocessor input: %s\n",input);
			output->append(input);
			return true;
		}
};

int main(int argc, const char **argv) {

	// define a simple grammar
	const char	*grammar=
		"<grammar>"
		"  <definition name=\"word\" type=\"literal\">"
		"    <repetition min=\"1\">"
		"      <alternation>"
		"        <terminal value=\"a\"/>"
		"        <terminal value=\"b\"/>"
		"        <terminal value=\"c\"/>"
		"      </alternation>"
		"    </repetition>"
		"  </definition>"
		"</grammar>";


	// create the compiler
	compiler	comp;

	// set the input and output grammars
	comp.setInputGrammar(grammar,"word");
	comp.setOutputGrammar(grammar);


	// add preprocessor, processor, and postprocessor modules
	mypreprocessor	pp;
	myprocessor	p;
	mypostprocessor	postp;

	comp.appendPreProcessor(&pp);
	comp.appendProcessor(&p);
	comp.appendPostProcessor(&postp);


	// compile some input
	stdoutput.write("compiling: abc\n\n");
	stringbuffer	output;
	if (comp.compile("abc",&output)) {
		stdoutput.printf("\ncompilation succeeded\n");
		stdoutput.printf("output: %s\n",output.getString());
	} else {
		stdoutput.printf("\ncompilation failed: %s\n",
						comp.getError());
	}
}
