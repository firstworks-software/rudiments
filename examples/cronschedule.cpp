#include <rudiments/cronschedule.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	cronschedule	cs;

	// set a schedule using a single expression...
	// format: year month day-of-month day-of-week day-part
	// this schedule matches:
	//   any year, Jan-Jun, any day, weekdays (1-5), 9am-5pm
	cs.setSchedule("* 1-6 * 1-5 9:00-17:00");

	// check some dates against the schedule...
	const char * const dates[]={
		"01/15/2025 10:30:00",
		"01/15/2025 20:00:00",
		"07/15/2025 10:30:00",
		"01/18/2025 10:30:00",
		NULL
	};

	stdoutput.printf("schedule: any year, Jan-Jun, "
			"weekdays, 9am-5pm\n\n");

	for (const char * const *d=dates; *d; d++) {
		stdoutput.printf("  %s : %s\n",*d,
				(cs.isInSchedule(*d))?"in schedule":
							"not in schedule");
	}
	stdoutput.write('\n');


	// set a schedule using individual parts...
	cs.clear();
	cs.setSchedule("2025,2026","*","1,15","*","*");

	stdoutput.printf("schedule: 2025-2026, "
			"1st and 15th of each month, all day\n\n");

	const char * const dates2[]={
		"03/01/2025 12:00:00",
		"03/15/2025 12:00:00",
		"03/10/2025 12:00:00",
		"03/01/2027 12:00:00",
		NULL
	};

	for (const char * const *d=dates2; *d; d++) {
		stdoutput.printf("  %s : %s\n",*d,
				(cs.isInSchedule(*d))?"in schedule":
							"not in schedule");
	}
	stdoutput.write('\n');


	// check the schedule using a datetime instance...
	datetime	dt;
	dt.init("06/15/2025 10:00:00");

	cs.clear();
	cs.setSchedule("* * * * *");

	stdoutput.printf("schedule: wildcard (always matches)\n\n");
	stdoutput.printf("  %s : %s\n",dt.getString(true),
			(cs.isInSchedule(&dt))?"in schedule":
						"not in schedule");
}
