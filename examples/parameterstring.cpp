#include <rudiments/parameterstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	parameterstring	ps;

	// parse a parameter string with the default semicolon delimiter
	ps.parse("host=localhost;port=9000;user='testuser';password='testpwd'");

	// access individual values
	stdoutput.printf("host:     %s\n",ps.getValue("host"));
	stdoutput.printf("port:     %s\n",ps.getValue("port"));
	stdoutput.printf("user:     %s\n",ps.getValue("user"));
	stdoutput.printf("password: %s\n\n",ps.getValue("password"));


	// clear and parse with a different delimiter
	ps.clear();
	ps.setDelimiter(',');
	ps.parse("name=hello,value=world");

	stdoutput.printf("name:  %s\n",ps.getValue("name"));
	stdoutput.printf("value: %s\n",ps.getValue("value"));
}
