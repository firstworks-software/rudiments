// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	init();
		void	clear();

		bool	getBrokenDownTimeFromEpoch();
		bool	normalize();
		void	processTZ(void *tms);

		bool	setTZ(const char *zone, char **oldzone, bool ignoredst);
		bool	restoreTZ(char *oldzone);

		const char	*lookupCombinedTimeZone(const char *zone) const;
		bool		daylightZone(const char *zone) const;

		const char	*getTzName(uint8_t index);

		static int16_t	adjustHour(int16_t hour,
						const char *timestring);
		static int32_t	fractionToMicroseconds(const char *fraction);

		static bool	acquireLock();
		static bool	releaseLock();

		datetimeprivate	*pvt;
