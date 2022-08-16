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
		cronschedule(cronschedule &c);
		cronschedule	&operator=(cronschedule &c);
		virtual	~cronschedule();

		void	setSchedule(const char *when);
		void	setSchedule(const char *years,
					const char *months,
					const char *daysofmonth,
					const char *daysofweek,
					const char *dayparts);

		bool	inSchedule(datetime *dt);
		bool	inSchedule(const char *dt);

	#include <rudiments/private/cronschedule.h>
};

#endif
