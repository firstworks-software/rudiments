#include <rudiments/table.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	table<const char *>	t;

	// set column names
	t.setColumnName(0,"animal");
	t.setColumnName(1,"type");
	t.setColumnName(2,"legs");

	// set values
	t.setValue(0,0,"dog");
	t.setValue(0,1,"mammal");
	t.setValue(0,2,"4");

	t.setValue(1,0,"eagle");
	t.setValue(1,1,"bird");
	t.setValue(1,2,"2");

	t.setValue(2,0,"snake");
	t.setValue(2,1,"reptile");
	t.setValue(2,2,"0");

	t.setValue(3,0,"frog");
	t.setValue(3,1,"amphibian");
	t.setValue(3,2,"4");

	// print the table dimensions
	stdoutput.printf("%lld columns, %lld rows\n\n",
				t.getColumnCount(),t.getRowCount());

	// print column names
	for (uint64_t col=0; col<t.getColumnCount(); col++) {
		stdoutput.printf("%-12s",t.getColumnName(col));
	}
	stdoutput.write('\n');

	// print values by row and column index
	for (uint64_t row=0; row<t.getRowCount(); row++) {
		for (uint64_t col=0; col<t.getColumnCount(); col++) {
			stdoutput.printf("%-12s",t.getValue(row,col));
		}
		stdoutput.write('\n');
	}
	stdoutput.write('\n');

	// access values by column name
	for (uint64_t row=0; row<t.getRowCount(); row++) {
		stdoutput.printf("%s is a %s with %s legs\n",
					t.getValue(row,"animal"),
					t.getValue(row,"type"),
					t.getValue(row,"legs"));
	}
	stdoutput.write('\n');

	// clear the table
	t.clear();
	stdoutput.printf("after clearing: %lld columns, %lld rows\n",
				t.getColumnCount(),t.getRowCount());
}
