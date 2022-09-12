// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATETIME_H
#define RUDIMENTS_DATETIME_H

#include <rudiments/private/datetimeincludes.h>

/** The datetime class represents a date/time combination and provides methods
 *  for getting and setting dates and times, converting date/time formats and
 *  accessing various date/time values. */
class RUDIMENTS_DLLSPEC datetime : public object {
	public:

		/** Creates an instance of the datetime class. */
		datetime();

		/** Destroys this instance of the datetime class. */
		~datetime();

		/** Parses "tmstring" and sets the date and time
		 *  represented in the class to that time.
		 *  "tmstring" must be of the format: "mm/dd/yyyy hh:mm:ss TZN".
		 * 
		 *  Note that TZN must be a valid timezone.  Otherwise
		 *  GMT is assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *tmstring);

		/** Processes "seconds" and sets the date and time
		 *  represented in the class to that time.
		 *  "seconds" is the number of seconds since 1970 (the epoch).
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(time_t seconds);

		/** Processes "seconds" and "microseconds" and sets the date
		 *  and time represented in the class to that time.
		 *  "seconds" is the number of seconds since 1970 (the epoch).
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(time_t seconds, time_t microseconds);

		/** Processes "tmstruct" and sets the date and time
		 *  represented in the class to that time.
		 * 
		 *  "tmstruct" should be a pointer to a platform-specific
		 *  time structure (struct tm * on unix)
		 * 
		 *  Note that in "tmstruct", the timezone and GMT offset
		 *  must be set to valid values.  Otherwise GMT is
		 *  assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const void *tmstruct);


		/** Sets the date and time represented in the class to
		 *  the date and time stored in the system clock.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getSystemDateAndTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Sets the date and time represented in the class to
		 *  the date and time stored in the hardware clock.
		 * 
		 *  "hwtz" must be set to the timezone that the hardware
		 *  clock is using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getHardwareDateAndTime(const char *hwtz);

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 *
		 *  Gets the date and time from the hardware clock,
		 *  then adjusts it to the timezone used by the system.
		 * 
		 *  Returns true on success and false on failure. */
		bool	getAdjustedHardwareDateAndTime(const char *hwtz);

		/** Sets the system clock's date and time to the date
		 *  and time currently represented in the class.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setSystemDateAndTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Sets the hardware clock's date and time to the date
		 *  and time currently represented in the class.
		 * 
		 *  "hwtz" must be set to the timezone that the system
		 *  clock using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setHardwareDateAndTime(const char *hwtz);


		/** Returns the hour component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getHour();

		/** Returns the minute component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getMinute();

		/** Returns the second component of the date/time currently
		 *  represented in the instance of the class. */
		int32_t	getSecond();

		/** Returns the microsecond component of the date/time
		 *  currently represented in the instance of the class. */
		int32_t	getMicrosecond();

		/** Returns the month component (1-12) of the date/time
		 *  currently represented in the instance of the class. */
		int32_t	getMonth();

		/** Returns the full name of the month component of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		const char	*getMonthName();

		/** Returns the abbreviated name of the month component of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		const char	*getMonthAbbreviation();

		/** Returns the day-of-the-month component (1-31) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfMonth();

		/** Returns the day-of-the-week component (1-7) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfWeek();

		/** Returns the day-of-the-year component (1-365) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getDayOfYear();

		/** Returns the week-of-the-year component (1-52) of the
		 *  date/time currently represented in the instance of the
		 *  class. */
		int32_t	getWeekOfYear();

		/** Returns the year component, including the century, of
		 *  the date/time currently represented in the instance of the
		 *  class. */
		int32_t	getYear();

		/** Returns the year component, not including the century, of
		 *  the date/time currently represented in the instance of the
		 *  class. */
		int32_t	getShortYear();

		/** Returns the (one-based) century component of the date/time
 		 *  currently represented in the instance of the class. */
		int32_t	getCentury();

		/** Returns true if daylight savings time is currently
		 *  in effect and false if it isn't */
		bool	isDaylightSavingsTime();

		/** Returns a 3 character string representing the time zone */
		const char	*getTimeZoneString();

		/** Returns the offset from GMT in seconds */
		int32_t	getTimeZoneOffset();
			
		/** Recalculates the time currently represented in the
		 *  class to correspond to the time zone "newtz".
		 * 
		 *  If, in the new timezone, daylight savings time is
		 *  currently in effect, the time zone will be adjusted
		 *  accordingly.  Ie.  if "newtz" = EST and daylight
		 *  savings time is in effect in EST5EDT, then EDT
		 *  will be used instead.
		 * 
		 *  Returns true on success and false on failure. */
		bool	adjustTimeZone(const char *newtz);

		/** Recalculates the time currently represented in the
		 *  class to correspond to the time zone "newtz".
		 * 
		 *  If ignoredst is false, this method works like
		 *  adjustTimezone(const char *newtz).  However if
		 *  ignoredst is true, this method will not adjust
		 *  EST to EDT if daylight savings time is in
		 *  effect in EST5EDT.
		 * 
		 *  Returns true on success and false on failure. */
		bool	adjustTimeZone(const char *newtz, bool ignoredst);


		/** Sets the second component of the date/time represented in
		 *  the instance of the class to "seconds". */
		bool	setSecond(int32_t seconds);

		/** Sets the microsecond component of the date/time
		 *  represented in the instance of the class to
		 *  "microseconds". */
		bool	setMicrosecond(int32_t microseconds);

		/** Sets the minute component of the date/time represented in
		 *  the instance of the class to "minutes". */
		bool	setMinute(int32_t minute);

		/** Sets the hour component of the date/time represented in
		 *  the instance of the class to "hour". */
		bool	setHour(int32_t hour);

		/** Sets the day-of-the-month component (1-31) of the date/time
 		 *  represented in the instance of the class to "day". */
		bool	setDayOfMonth(int32_t day);

		/** Sets the month component (1-12) of the date/time
 		 *  represented in the instance of the class to "month". */
		bool	setMonth(int32_t month);

		/** Sets the year component of the date/time represented in the
		 *  instance of the class to "year". */
		bool	setYear(int32_t year);

		/** Adds "seconds" to the date/time represented in the
		 *  instance of the class. */
		bool	addSeconds(int32_t second);

		/** Adds "microseconds" to the date/time represented in the
		 *  instance of the class. */
		bool	addMicroseconds(int32_t microsecond);

		/** Adds "minutes" to the date/time represented in the
		 *  instance of the class. */
		bool	addMinutes(int32_t minutes);

		/** Adds "hours" to the date/time represented in the
		 *  instance of the class. */
		bool	addHours(int32_t hours);

		/** Adds "days" to the date/time represented in the
		 *  instance of the class. */
		bool	addDays(int32_t days);

		/** Adds "months" to the date/time represented in the
		 *  instance of the class. */
		bool	addMonths(int32_t months);

		/** Adds "years" to the date/time represented in the
		 *  instance of the class. */
		bool	addYears(int32_t years);


		/** Returns a string of the format: "mm/dd/yyyy hh:mm:ss TZN"
		 *  for the date/time represented in the instance of the class.
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if the class instance is
		 *  deleted.) */
		const char	*getString();

		/** Returns a string representing the date/time in the instance
		 *  of the class.
		 *
		 *  If microseconds is false then the string is of the format:
		 *  "mm/dd/yyyy hh:mm:ss TZN"
		 *
		 *  If microseconds is true then the string is of the format:
		 *  "mm/dd/yyyy hh:mm:ss:uuu TZN"
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if the class instance is
		 *  deleted.) */
		const char	*getString(bool microseconds);


		/** Returns a string of the format: "yyyy-mm-dd hh:mm:ss"
		 *  for the date/time represented in the instance of the class.
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if the class instance is
		 *  deleted.) */
		const char	*getSqlString();

		/** Returns a string representing the date/time in the instance
		 *  of the class.
		 *
		 *  If microseconds is false then the string is of the format:
		 *  "yyyy-mm-dd hh:mm:ss"
		 *
		 *  If microseconds is true then the string is of the format:
		 *  "yyyy-mm-dd hh:mm:ss.mmm"
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if the class instance is
		 *  deleted.) */
		const char	*getSqlString(bool microseconds);


		/** Returns the number of seconds since 1970 (the epoch). */
		time_t		getEpoch();


		/** Returns a NULL terminated array of timezone
		 *  abbreviations. */
		static const char * const	*getTimeZoneAbbreviations();

		/** Returns an array of timezone offsets from
		 *  GMT (in seconds).  Each element of this
		 *  array corresponds to an element of the
		 *  array returned b getTimeZoneAbbreviations(). */
		static const int32_t		*getTimeZoneOffsets();

		/** Returns true if string represents a valid date/time and
		 *  false otherwise.
		 * 
		 *  For this methods to return true, "string" must be
		 *  formatted like: "00/00/0000 00:00:00" optionally
		 *  followed by a space and a timezone. */
		static bool	validDateTime(const char *string);

		/** Takes "seconds" since the epoch (the number of seconds
		 *  since 1970) and returns a string in
		 *  "mm/dd/yyyy hh:mm:ss TZN" format.
		 *	
		 *  Note that this method allocates a buffer to return the
		 *  string in which must be deleted by the calling program. */
		static char	*getString(time_t seconds);

		/** Takes "seconds" since the epoch (the number of seconds
		 *  since 1970) and "microseconds" and returns a string in
		 *  "mm/dd/yyyy hh:mm:ss:uuu TZN" format.
		 *	
		 *  Note that this method allocates a buffer to return the
		 *  string in which must be deleted by the calling program. */
		static char	*getString(time_t seconds, time_t microseconds);

		/** Takes "tmstruct", a pointer to a platform-specific time
		 *  structure (struct tm * on unix) and returns a string in
		 *  "mm/dd/yyyy hh:mm:ss TZN" format.
		 *	
		 *  Note that this method allocates a buffer to return the
		 *  string in which must be deleted by the calling program. */
		static char	*getString(const void *tmstruct);

		/** Parses "datestring" which must be of the format:
		 *  "mm/dd/yyyy hh:mm:ss TZN" and returns the number of seconds
		 *  since 1970 (the epoch).
		 *	
		 * Note that TZN must be a valid timezone.  Otherwise
		 * GMT is assumed. */
		static time_t	getEpoch(const char *datestring);

		/** Takes "tmstruct", a pointer to a platform-specific 
		 *  time structure (struct tm * on unix) and returns the number
		 *  of seconds since 1970 (the eopch).
		 *	
		 *  Note that in "tmstruct", the timezone and GMT offset must
		 *  be set to valid values.  Otherwise GMT is assumed. */
		static time_t	getEpoch(const void *tmstruct);

		/** FIXME: document... */
		static bool	parse(const char *datetime,
					bool ddmm, bool yyyyddmm,
					const char *datedelimiters,
					int16_t *year, int16_t *month,
					int16_t *day, int16_t *hour,
					int16_t *minute, int16_t *second,
					int32_t *microsecond, bool *isnegative);

		/** FIXME: document... */
		static char	*formatAs(const char *format,
					int16_t year, int16_t month,
					int16_t day, int16_t hour,
					int16_t minute, int16_t second,
					int32_t microsecond, bool isnegative);

		/** Many of the functions that the datetime class uses
		 *  internally are not reentrant and thus not thread-safe.
		 *  Indeed, for some functions, there is no thread-safe version
		 *  available on any platform.
		 *
		 *  If your application is multi-threaded, you must use this
		 *  method to supply a mutex and ensure thread safety.
		 * 
		 *  If you don't supply a mutex, the methods in the class
		 *  will still work, but will not be thread-safe. */
		static	void	setTimeMutex(threadmutex *mtx);

	#include <rudiments/private/datetime.h>
};

#endif
