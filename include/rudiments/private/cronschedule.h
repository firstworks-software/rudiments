// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	clone(cronschedule &c);
		void	init(const char *years,
				const char *months,
				const char *daysofmonth,
				const char *daysofweek,
				const char *dayparts);
		void	clear();
		void	splitTimePart(
				linkedlist< cronscheduleperiod * > *periods,
				const char *timepartlist);
		void	splitDayParts(const char *daypartlist);
		bool	inPeriods(linkedlist< cronscheduleperiod * > *periods,
						int32_t timepart);
		bool	inDayParts(int32_t hour, int32_t minute);

		cronscheduleprivate	*pvt;
