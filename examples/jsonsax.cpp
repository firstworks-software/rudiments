#include <rudiments/jsonsax.h>
#include <rudiments/stdio.h>

// create a json sax handler by inheriting from jsonsax
class myjsonhandler : public jsonsax {

	protected:

		bool objectStart() {
			stdoutput.write("object start\n");
			return true;
		}

		bool memberName(const char *name) {
			stdoutput.printf("  member: %s\n",name);
			return true;
		}

		bool stringValue(const char *value) {
			stdoutput.printf("    string: %s\n",value);
			return true;
		}

		bool numberValue(const char *value) {
			stdoutput.printf("    number: %s\n",value);
			return true;
		}

		bool trueValue() {
			stdoutput.write("    true\n");
			return true;
		}

		bool falseValue() {
			stdoutput.write("    false\n");
			return true;
		}

		bool arrayStart() {
			stdoutput.write("  array start\n");
			return true;
		}

		bool arrayEnd() {
			stdoutput.write("  array end\n");
			return true;
		}

		bool objectEnd() {
			stdoutput.write("object end\n");
			return true;
		}
};

int main(int argc, const char **argv) {

	myjsonhandler	handler;

	// parse a json string
	stdoutput.write("parsing json:\n\n");
	handler.parseString(
		"{"
		"  \"animals\": ["
		"    {"
		"      \"name\": \"dog\","
		"      \"type\": \"mammal\","
		"      \"legs\": 4,"
		"      \"domestic\": true"
		"    },"
		"    {"
		"      \"name\": \"eagle\","
		"      \"type\": \"bird\","
		"      \"legs\": 2,"
		"      \"domestic\": false"
		"    },"
		"    {"
		"      \"name\": \"snake\","
		"      \"type\": \"reptile\","
		"      \"legs\": 0,"
		"      \"domestic\": false"
		"    }"
		"  ]"
		"}");
}
