#include <rudiments/csvdom.h>
#include <rudiments/domnode.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	csvdom	csv;

	// parse a csv string
	csv.parseString(
		"\"name\",\"type\",\"legs\",\"domestic\"\n"
		"\"dog\",\"mammal\",\"4\",\"yes\"\n"
		"\"eagle\",\"bird\",\"2\",\"no\"\n"
		"\"snake\",\"reptile\",\"0\",\"no\"\n");


	// access data using convenience methods
	stdoutput.printf("columns: %lld\n",csv.getColumnCount());
	stdoutput.printf("records: %lld\n\n",csv.getRecordCount());

	stdoutput.write("all animals:\n");
	for (uint64_t i=0; i<csv.getRecordCount(); i++) {

		const char	*name=csv.getField(i,"name");
		const char	*type=csv.getField(i,"type");
		const char	*legs=csv.getField(i,"legs");
		const char	*domestic=csv.getField(i,"domestic");

		stdoutput.printf("  %s (%s) - %s legs - domestic: %s\n",
						name,type,legs,domestic);
	}
	stdoutput.write('\n');


	// write the csv tree to standard output
	stdoutput.write("csv output:\n");
	csv.write();
	stdoutput.write('\n');
}
