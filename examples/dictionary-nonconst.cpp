#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary of name-value pairs
	dictionary<char *,char *>	nvp;
	nvp.setManageArrayKeys(true);
	nvp.setManageArrayValues(true);

	// set some name-value pairs
	nvp.setValue(charstring::duplicate("color"),
			charstring::duplicate("blue"));
	nvp.setValue(charstring::duplicate("number"),
			charstring::duplicate("one"));
	nvp.setValue(charstring::duplicate("automobile"),
			charstring::duplicate("car"));
	nvp.setValue(charstring::duplicate("dwelling"),
			charstring::duplicate("house"));
	nvp.setValue(charstring::duplicate("parent"),
			charstring::duplicate("dad"));

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// remove a value
	nvp.remove((char *)"color");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// clear the dictionary
	nvp.clear();
}
