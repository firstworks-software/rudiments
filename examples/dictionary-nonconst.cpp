#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary of name-value pairs
	dictionary<char *,char *>	nvp;
	nvp.setManageArrayKeys(true);
	nvp.setManageArrayValues(true);

	// set some name-value pairs
	nvp.setValue(charstring::duplicate("dog"),
			charstring::duplicate("mammal"));
	nvp.setValue(charstring::duplicate("eagle"),
			charstring::duplicate("bird"));
	nvp.setValue(charstring::duplicate("snake"),
			charstring::duplicate("reptile"));
	nvp.setValue(charstring::duplicate("frog"),
			charstring::duplicate("amphibian"));
	nvp.setValue(charstring::duplicate("trout"),
			charstring::duplicate("fish"));

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// remove a value
	nvp.remove((char *)"dog");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// clear the dictionary
	nvp.clear();
}
