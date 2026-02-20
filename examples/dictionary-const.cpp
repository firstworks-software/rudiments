#include <rudiments/dictionary.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a dictionary of name-value pairs
	dictionary<const char *,const char *>	nvp;

	// set some name-value pairs
	nvp.setValue("dog","mammal");
	nvp.setValue("eagle","bird");
	nvp.setValue("snake","reptile");
	nvp.setValue("frog","amphibian");
	nvp.setValue("trout","fish");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// replace some values
	nvp.setValue("eagle","raptor");
	nvp.setValue("frog","poison dart frog");
	nvp.setValue("trout","freshwater fish");

	// remove a value
	nvp.remove("dog");

	// print the dictionary
	stdoutput.write("Current contents:\n");
	nvp.write();
	stdoutput.write('\n');

	// clear the dictionary
	nvp.clear();

	// set some new values
	nvp.setValue("cat","mammal");
	nvp.setValue("parrot","bird");
	nvp.setValue("turtle","reptile");

	// print out the dictionary a different way
	stdoutput.write("Current contents:\n");
	linkedlist<const char *>	*keys=nvp.getKeys();
	for (listnode<const char *> *kn=keys->getFirst();
						kn; kn=kn->getNext()) {
		stdoutput.printf("%s=%s ",
			kn->getValue(),nvp.getValue(kn->getValue()));
	}
	delete keys;
	stdoutput.write('\n');
}
