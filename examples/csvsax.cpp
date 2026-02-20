#include <rudiments/csvsax.h>
#include <rudiments/stdio.h>

// create a csv sax handler by inheriting from csvsax
class mycsvhandler : public csvsax {

	protected:

		bool headerStart() {
			stdoutput.write("header:\n");
			return true;
		}

		bool column(const char *name, bool quoted) {
			stdoutput.printf("  column: %s",name);
			if (quoted) {
				stdoutput.write(" (quoted)");
			}
			stdoutput.write('\n');
			return true;
		}

		bool headerEnd() {
			stdoutput.write('\n');
			return true;
		}

		bool recordStart() {
			stdoutput.write("record:\n");
			return true;
		}

		bool field(const char *value, bool quoted) {
			stdoutput.printf("  field: %s",value);
			if (quoted) {
				stdoutput.write(" (quoted)");
			}
			stdoutput.write('\n');
			return true;
		}

		bool recordEnd() {
			stdoutput.write('\n');
			return true;
		}
};

int main(int argc, const char **argv) {

	mycsvhandler	handler;

	// parse a csv string
	stdoutput.write("parsing csv:\n\n");
	handler.parseString(
		"\"name\",\"type\",\"legs\",\"domestic\"\n"
		"\"dog\",\"mammal\",\"4\",\"yes\"\n"
		"\"eagle\",\"bird\",\"2\",\"no\"\n"
		"\"snake\",\"reptile\",\"0\",\"no\"\n");
}
