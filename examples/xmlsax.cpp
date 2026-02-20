#include <rudiments/xmlsax.h>
#include <rudiments/stdio.h>

// create an xml sax handler by inheriting from xmlsax
class myxmlhandler : public xmlsax {

	protected:

		bool tagStart(const char *ns, const char *name) {
			stdoutput.printf("start tag: %s\n",name);
			return true;
		}

		bool attributeName(const char *name) {
			stdoutput.printf("  attribute: %s",name);
			return true;
		}

		bool attributeValue(const char *value) {
			stdoutput.printf(" = \"%s\"\n",value);
			return true;
		}

		bool text(const char *string) {
			stdoutput.printf("  text: %s\n",string);
			return true;
		}

		bool tagEnd(const char *ns, const char *name) {
			stdoutput.printf("end tag: %s\n",name);
			return true;
		}

		bool comment(const char *string) {
			stdoutput.printf("comment: %s\n",string);
			return true;
		}
};

int main(int argc, const char **argv) {

	myxmlhandler	handler;

	// parse an xml string
	stdoutput.write("parsing xml:\n\n");
	handler.parseString(
		"<!-- a list of animals -->"
		"<animals>"
		"  <animal type=\"mammal\">dog</animal>"
		"  <animal type=\"bird\">eagle</animal>"
		"  <animal type=\"reptile\">snake</animal>"
		"</animals>");
}
