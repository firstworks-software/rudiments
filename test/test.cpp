#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/datetime.h>
#include <rudiments/error.h>

void header(const char *title) {
	stdoutput.printf("\n===============================================================================\n");
	stdoutput.printf("\n%s:\n\n",title);
}

void test(const char *printstring, bool result) {
	stdoutput.write("\t");
	stdoutput.write(printstring);
	stdoutput.write((result)?": success":": failed");
	stdoutput.write("\n");
	if (!result) {
		stdoutput.printf("%d: %s\n",
			error::getErrorNumber(),
			error::getErrorString());
		process::exit(1);
	}
}

void test(const wchar_t *printstring, bool result) {
	stdoutput.write("\t");
	stdoutput.write(printstring);
	stdoutput.write((result)?": success":": failed");
	stdoutput.write("\n");
	if (!result) {
		stdoutput.printf("%d: %s\n",
			error::getErrorNumber(),
			error::getErrorString());
		process::exit(1);
	}
}

void displayTime(datetime *start, datetime *end) {
	uint32_t	sec=end->getEpoch()-start->getEpoch();
	int32_t		usec=end->getMicroseconds()-start->getMicroseconds();
	if (usec<0) {
		sec--;
		usec=usec+100000;
	}
	long double	totalsec=(long double)sec+
					(((long double)usec)/1000000.0);
	stdoutput.printf("time: %Lf\n",totalsec);
}
