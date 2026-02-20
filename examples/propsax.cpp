#include <rudiments/propsax.h>
#include <rudiments/stdio.h>

// create a properties sax handler by inheriting from propsax
class myprophandler : public propsax {

	protected:

		bool key(const char *k) {
			stdoutput.printf("key: %s",k);
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

		bool exclamationComment(const char *c) {
			stdoutput.printf("! %s\n",c);
			return true;
		}
};

int main(int argc, const char **argv) {

	myprophandler	handler;

	// parse a properties string
	stdoutput.write("parsing properties:\n\n");
	handler.parseString(
		"# animal types\n"
		"! a simple list\n"
		"dog=mammal\n"
		"eagle=bird\n"
		"snake=reptile\n");
}
