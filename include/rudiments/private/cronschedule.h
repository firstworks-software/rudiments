// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		cronschedule(cronschedule &c) {};
		cronschedule	&operator=(cronschedule &c) { return *this; };

	private:
		bool	splitTimePart(
				linkedlist< cronscheduleperiod * > *periods,
				const char *timepartlist,
				int64_t min, int64_t max);
		bool	splitDayParts(const char *daypartlist);
		bool	isInPeriods(
				linkedlist< cronscheduleperiod * > *periods,
				int32_t timepart);
		bool	isInDayParts(int32_t hour, int32_t minute);

		cronscheduleprivate	*pvt;
