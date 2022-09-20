// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		cronschedule(cronschedule &c) {};
		cronschedule	&operator=(cronschedule &c) { return *this; };

		bool	splitTimePart(
				linkedlist< cronscheduleperiod * > *periods,
				const char *timepartlist);
		bool	splitDayParts(const char *daypartlist);
		bool	inPeriods(linkedlist< cronscheduleperiod * > *periods,
						int32_t timepart);
		bool	inDayParts(int32_t hour, int32_t minute);

		cronscheduleprivate	*pvt;
