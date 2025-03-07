// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CRONSCHEDULE_H
#define RUDIMENTS_CRONSCHEDULE_H

#include <rudiments/private/cronscheduleincludes.h>

/** The cronschedule class provides methods for parsing a cronish-style schedule
 *  string and determining whether a given date/time is in the schedule.  */

class RUDIMENTS_DLLSPEC cronschedule : virtual public object {
	public:
		/** Creates an instance of the cronschedule class. */
		cronschedule();

		/** Deletes this instance of the cronschedule class. */
		~cronschedule();

		/** Parses "when", which should be a cronish-style expression,
		 *  and configures this instance to represent that schedule.
		 *
		 *  The expression should consist of 5 space-delimited parts:
		 *
		 *   * year
		 *    * single - eg. 2022
		 *    * list - eg. 2022,2023,2025
		 *    * range - eg. 2022-2025
		 *    * list of ranges - eg. 2022-2025,2027,2028-2029
		 *    * wildcard - *
		 *       
		 *   * month
		 *    * single - eg. 1
		 *    * list - eg. 1,2,3,4
		 *    * range - eg. 1-4
		 *    * list of ranges - eg. 1-4,7-9,11
		 *    * wildcard - *
		 *
		 *   * day-of-month
		 *    * single - eg. 5
		 *    * list - eg. 1,4,9,31
		 *    * range - eg. 1-5
		 *    * list of ranges - 1-5,7-10,31
		 *    * wildcard - *
		 *
		 *   * day-of-week
		 *    * single - eg. 4
		 *    * list - eg. 1,3,7
		 *    * range - eg. 1-3
		 *    * list of ranges - eg. 1-3,5,7
		 *    * wildcard - *
		 *
		 *   * day-part
		 *    * single - eg. 10:00
		 *    * list - eg. 10:00,10:05,10:10
		 *    * range - eg. 10:00-13:30
		 *    * list of ranges - 10:00-11:05,13:00,14:05-14:40
		 *    * wildcard - *
		 *
		 *  Or, for a complete example:
		 *    2022 1,3,3-5,7 * 1-5 10:00-11:30,13:30-15:30
		 *
		 *  Note that the day-part is different from standard cron or
		 *  java-style cron, in that hours and minutes are specified
		 *  together rather than separately.
		 *
		 *  Returns true on success and false if "when" is malformed. */
		bool	setSchedule(const char *when);

		/** Parses each of "years", "months", "daysofmonth",
		 *  "daysofweek", and "dayparts", which should each be
		 *  cron-style expressions, and configures this instance to
		 *  represent that schedule.
		 *
		 *  "years"
		 *    * single - eg. 2022
		 *    * list - eg. 2022,2023,2025
		 *    * range - eg. 2022-2025
		 *    * list of ranges - eg. 2022-2025,2027,2028-2029
		 *    * wildcard - *
		 *       
		 *  "months"
		 *    * single - eg. 1
		 *    * list - eg. 1,2,3,4
		 *    * range - eg. 1-4
		 *    * list of ranges - eg. 1-4,7-9,11
		 *    * wildcard - *
		 *
		 *  "daysofmonth"
		 *    * single - eg. 5
		 *    * list - eg. 1,4,9,31
		 *    * range - eg. 1-5
		 *    * list of ranges - 1-5,7-10,31
		 *    * wildcard - *
		 *
		 *  "daysofweek"
		 *    * single - eg. 4
		 *    * list - eg. 1,3,7
		 *    * range - eg. 1-3
		 *    * list of ranges - eg. 1-3,5,7
		 *    * wildcard - *
		 *
		 *  "dayparts"
		 *    * single - eg. 10:00
		 *    * list - eg. 10:00,10:05,10:10
		 *    * range - eg. 10:00-13:30
		 *    * list of ranges - 10:00-11:05,13:00,14:05-14:40
		 *    * wildcard - *
		 *
		 *  Returns true on success and false if any of those parts are
		 *  malformed. */
		bool	setSchedule(const char *years,
					const char *months,
					const char *daysofmonth,
					const char *daysofweek,
					const char *dayparts);

		/** Returns true if "dt" falls within the schedule previously
		 *  configured by setSchedule() or false otherwise. */
		bool	isInSchedule(datetime *dt);

		/** Returns true if "dt" falls within the schedule previously
		 *  configured by setSchedule() or false otherwise. */
		bool	isInSchedule(const char *dt);

		/** Clears any schedule previously set by setSchedule() such
		 *  that any call to inSchedule() will return false.
		 *
		 *  Always returns true. */
		bool	clear();

	#include <rudiments/private/cronschedule.h>
};

#endif
