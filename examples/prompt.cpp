#include <rudiments/prompt.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	prompt	p;


	// configure the history file and limits
	p.setHistoryFile("/tmp/.prompt_example_history");
	p.setMaxHistoryLines(100);
	p.setMaxHistoryQueue(10);

	// display the history configuration
	stdoutput.printf("history file: %s\n",p.getHistoryFile());
	stdoutput.printf("max history lines: %d\n",p.getMaxHistoryLines());
	stdoutput.printf("max history queue: %d\n",p.getMaxHistoryQueue());
	stdoutput.write('\n');


	// set the prompt and display it
	p.setPrompt("enter a command> ");
	stdoutput.printf("prompt: %s\n\n",p.getPrompt());


	// read lines until ctrl-D is pressed
	stdoutput.write("Type lines of input and press enter.\n");
	stdoutput.write("Press ctrl-D to exit.\n\n");
	for (;;) {

		// read a line
		char	*line=p.read();

		// NULL means ctrl-D was pressed
		if (!line) {
			stdoutput.write("done\n");
			break;
		}

		// an empty string means enter was hit by itself
		if (!line[0]) {
			stdoutput.write("(empty line)\n");
			delete[] line;
			continue;
		}

		// print the line
		stdoutput.printf("you entered: %s\n",line);

		// clean up
		delete[] line;
	}


	// flush history to the history file
	p.flushHistory();
}
