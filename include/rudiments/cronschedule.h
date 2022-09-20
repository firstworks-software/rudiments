// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CRONSCHEDULE_H
#define RUDIMENTS_CRONSCHEDULE_H

#include <rudiments/private/cronscheduleincludes.h>

/** The cronschedule class provides methods for parsing a cron-style schedule
 *  string and determining whether a given date/time is in the schedule.  */

class RUDIMENTS_DLLSPEC cronschedule : virtual public object {
	public:
		cronschedule();
		virtual	~cronschedule();

		/** Parses "when", which should be a cron-style expression,
		 *  and configures this instance to represent that schedule.
		 *
		 *  Returns true on success and false if "when" is malformed. */
		bool	setSchedule(const char *when);

		/** Parses each of "years", "months", "daysofmonth",
		 *  "daysofweek", and "dayparts", which should each be
		 *  cron-style expressions, and configures this instance to
		 *  represent that schedule.
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
		bool	inSchedule(datetime *dt);

		/** Returns true if "dt" falls within the schedule previously
		 *  configured by setSchedule() or false otherwise. */
		bool	inSchedule(const char *dt);

		/** Clears any schedule previously set by setSchedule() such
		 *  that any call to inSchedule() will return false. */
		void	clear();

	#include <rudiments/private/cronschedule.h>
};

#endif
