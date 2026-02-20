#include <rudiments/inisax.h>
#include <rudiments/stdio.h>

// create an ini sax handler by inheriting from inisax
class myinihandler : public inisax {

	protected:

		bool section(const char *s) {
			stdoutput.printf("section: %s\n",s);
			return true;
		}

		bool key(const char *k) {
			stdoutput.printf("  key: %s",k);
			return true;
		}

		bool value(const char *v) {
			stdoutput.printf(" = %s\n",v);
			return true;
		}

		bool poundComment(const char *c) {
			stdoutput.printf("# %s\n",c);
			return true;
		}

		bool semicolonComment(const char *c) {
			stdoutput.printf("; %s\n",c);
			return true;
		}
};

int main(int argc, const char **argv) {

	myinihandler	handler;

	// parse an ini string
	stdoutput.write("parsing ini:\n\n");
	handler.parseString(
		"# a list of animals\n"
		"[dog]\n"
		"type=mammal\n"
		"legs=4\n"
		"domestic=yes\n"
		"\n"
		"[eagle]\n"
		"type=bird\n"
		"legs=2\n"
		"domestic=no\n"
		"\n"
		"[snake]\n"
		"type=reptile\n"
		"legs=0\n"
		"domestic=no\n");
}
