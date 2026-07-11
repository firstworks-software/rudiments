// Copyright (c) David Muse
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

		/** Creates an instance of the datetime class
		 *  that is a copy of "d". */
		datetime(datetime &d);

		/** Makes this instance of the datetime class
		 *  identical to "d". */
		datetime	&operator=(datetime &d);

		/** Destroys this instance of the datetime class. */
		~datetime();

		/** Initializes this instance to the date/time represented
		 *  by "tmstring" where "tmstring" is of the format:
		 *  	"mm/dd/yyyy hh:mm:ss TZN"
		 *  or of the format
		 *  	"mm/dd/yyyy hh:mm:ss:uuu TZN"
		 * 
		 *  Note that TZN must be a valid timezone.  Otherwise
		 *  GMT is assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	init(const char *tmstring);

		/** Initializes this intance to the date/time represented
 		 *  by "seconds" where "seconds" is the number of seconds since
 		 *  1970 (the epoch).
		 * 
		 *  Returns true on success and false on failure. */
		bool	init(time_t seconds);

		/** Initializes this instance to the date/time represented
		 *  by "seconds" and "microseconds", where "seconds" is the
		 *  number of seconds since 1970 (the epoch) and "microseconds"
		 *  is the number of microseconds after "seconds".
		 * 
		 *  Returns true on success and false on failure. */
		bool	init(time_t seconds, time_t microseconds);

		/** Initializes this instance to the date/time represented
		 *  by "tmstruct" where "tmstruct" is a pointer to a
		 *  platform-specific time structure (eg. struct tm * on unix)
		 * 
		 *  Note that in "tmstruct", the timezone and GMT offset
		 *  must be set to valid values.  Otherwise GMT is
		 *  assumed.
		 * 
		 *  Returns true on success and false on failure. */
		bool	init(const void *tmstruct);

		/** Initialies this instance to the date/time stored in the
		 *  system clock.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initFromSystemDateTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Initialies this instance to the date/time stored in the
		 *  hardware clock.
		 * 
		 *  "hwtz" must be set to the timezone that the hardware
		 *  clock is using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initFromHardwareDateTime(const char *hwtz);

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 *
		 *  Initialies this instance to the date/time stored in the
		 *  hardware clock then adjusts it to the timezone used by the
		 *  system.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initFromAdjustedHardwareDateTime(const char *hwtz);


		/** Sets the system clock's date/time to the date/time
		 *  currently represented in this instance.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setSystemDateTime();

		/** This method only works if your system has a working
		 *  real-time clock at /dev/rtc.
		 * 
		 *  Sets the hardware clock's date/time to the date/time
		 *  currently represented in this instance.
		 * 
		 *  "hwtz" must be set to the timezone that the system
		 *  clock using.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setHardwareDateTime(const char *hwtz);


		/** Sets the microsecond component (0-999999) of the date/time
		 *  represented in this instance to "microsecond" and
		 *  recalculates the current date/time (eg. if
		 *  "microsecondsecond" is greater than 999999 or less than
		 *  0). */
		bool	setMicrosecond(int32_t microsecond);

		/** Sets the second component (0-59) of the date/time
		 *  represented in this instance  "second" and recalculates the
		 *  current date/time (eg. if "second" is greater than 59 or
		 *  less than 0). */
		bool	setSecond(int32_t second);

		/** Sets the minute component (0-59) of the date/time
		 *  represented in this instance to "minute" and recalculates
		 *  the current date/time (eg. if "minute" is greater than 60
		 *  or less than 0). */
		bool	setMinute(int32_t minute);

		/** Sets the hour component (0-24) of the date/time represented
		 *  in this instance to "hour" and recalculates the current
		 *  date/time (eg. if "hour" is greater than 24 or less than
		 *  0). */
		bool	setHour(int32_t hour);

		/** Sets the day-of-the-month component (1-31) of the date/time
 		 *  represented in this instance to "day" and recalculates
 		 *  the current date/time (eg. if "day" is greater than 31 or
 		 *  less than 0). */
		bool	setDayOfMonth(int32_t day);

		/** Sets the month component (1-12) of the date/time
 		 *  represented in this instance to "month" and recalculates
 		 *  the current date/time (eg. if "month" is greater than 12 or
 		 *  less than 1). */
		bool	setMonth(int32_t month);

		/** Sets the year component of the date/time represented in this
		 *  instance to "year". */
		bool	setYear(int32_t year);

		/** Sets the timezone of the date/time represented in this
		 *  instance to "newtz" and recalculates the current date/time.
		 *
		 *  Eg. if the current timezone is EST, and "newtz" is CST then
		 *  the hour will be decremented by 1.
		 * 
		 *  If, in the new timezone, daylight savings time is
		 *  currently in effect, the time zone will be adjusted
		 *  accordingly.  Ie.  if "newtz" = EST and daylight
		 *  savings time is in effect in EST5EDT, then EDT
		 *  will be used instead.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setTimeZone(const char *newtz);

		/** Sets the timezone of the date/time represented in this
		 *  instance to "newtz" and recalculates the current date/time.
		 *
		 *  Eg. if the current timezone is EST, and "newtz" is CST then
		 *  the hour will be decremented by 1.
		 * 
		 *  If ignoredst is false, this method works like
		 *  setTimezone(const char *newtz).  However if
		 *  ignoredst is true, this method will not adjust
		 *  EST to EDT if daylight savings time is in
		 *  effect in EST5EDT.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setTimeZone(const char *newtz, bool ignoredst);


		/** Adds "seconds" to the date/time represented in this
		 *  instance.  "seconds" may be negative. */
		bool	addSeconds(int32_t seconds);

		/** Adds "microseconds" to the date/time represented in this
		 *  instance.  "microseconds" may be negative. */
		bool	addMicroseconds(int32_t microseconds);

		/** Adds "minutes" to the date/time represented in this
		 *  instance.  "minutes" may be negative. */
		bool	addMinutes(int32_t minutes);

		/** Adds "hours" to the date/time represented in this
		 *  instance.  "hours" may be negative. */
		bool	addHours(int32_t hours);

		/** Adds "days" to the date/time represented in this
		 *  instance.  "days" may be negative. */
		bool	addDays(int32_t days);

		/** Adds "months" to the date/time represented in this
		 *  instance.  "months" may be negative. */
		bool	addMonths(int32_t months);

		/** Adds "years" to the date/time represented in this
		 *  instance.  "years" may be negative. */
		bool	addYears(int32_t years);


		/** Returns the hour component of the date/time currently
		 *  represented in this instance. */
		int32_t	getHour();

		/** Returns the minute component of the date/time currently
		 *  represented in this instance. */
		int32_t	getMinute();

		/** Returns the second component of the date/time currently
		 *  represented in this instance. */
		int32_t	getSecond();

		/** Returns the microsecond component of the date/time
		 *  currently represented in this instance. */
		int32_t	getMicrosecond();

		/** Returns the month component (1-12) of the date/time
		 *  currently represented in this instance. */
		int32_t	getMonth();

		/** Returns the full name of the month component of the
		 *  date/time currently represented in this instance. */
		const char	*getMonthName();

		/** Returns the abbreviated name of the month component of the
		 *  date/time currently represented in this instance. */
		const char	*getMonthAbbreviation();

		/** Returns the day-of-the-month component (1-31) of the
		 *  date/time currently represented in this instance. */
		int32_t	getDayOfMonth();

		/** Returns the day-of-the-week component (1-7) of the
		 *  date/time currently represented in this instance. */
		int32_t	getDayOfWeek();

		/** Returns the day-of-the-year component (1-365) of the
		 *  date/time currently represented in this instance. */
		int32_t	getDayOfYear();

		/** Returns the week-of-the-year component (0-53) of the
		 *  date/time currently represented in this instance.
		 *
		 *  Note that week 1 begins with the first Monday of the year,
		 *  meaning that a year will have no week 0 if it starts on
		 *  a Monday. */
		int32_t	getWeekOfYear();

		/** Returns the year component, including the century, of
		 *  the date/time currently represented in this instance. */
		int32_t	getYear();

		/** Returns the year component, not including the century, of
		 *  the date/time currently represented in this instance. */
		int32_t	getShortYear();

		/** Returns the (one-based) century component of the date/time
 		 *  currently represented in this instance. */
		int32_t	getCentury();

		/** Returns true if daylight savings time is currently
		 *  in effect and false if it isn't */
		bool	isDaylightSavingsTime();

		/** Returns a 3 character string representing the time zone */
		const char	*getTimeZoneString();

		/** Returns the offset from GMT in seconds */
		int32_t	getTimeZoneOffset();


		/** Returns a string of the format: "mm/dd/yyyy hh:mm:ss TZN"
		 *  for the date/time represented in this instance.
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if this instance is deleted.)
		 */
		const char	*getString();

		/** Returns a string representing the date/time in this
		 *  instance.
		 *
		 *  If microseconds is false then the string is of the format:
		 *  "mm/dd/yyyy hh:mm:ss TZN"
		 *
		 *  If microseconds is true then the string is of the format:
		 *  "mm/dd/yyyy hh:mm:ss:uuu TZN"
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if this instance is deleted.)
		 */
		const char	*getString(bool microseconds);


		/** Returns a string of the format: "yyyy-mm-dd hh:mm:ss"
		 *  for the date/time represented in this instance.
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if this instance is
		 *  deleted.) */
		const char	*getSqlString();

		/** Returns a string representing the date/time in this
		 *  instance.
		 *
		 *  If microseconds is false then the string is of the format:
		 *  "yyyy-mm-dd hh:mm:ss"
		 *
		 *  If microseconds is true then the string is of the format:
		 *  "yyyy-mm-dd hh:mm:ss.mmm"
		 * 
		 *  (Note that this method returns a pointer to an internal
		 *  string which will be deleted if this instance is deleted.)
		 */
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
		static bool	isValidDateTime(const char *string);

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

		/** Attempts to parse "datetime" into its components setting
		 *  "year", "month", "day", "hour", "minute", "second",
		 *  "microsecond", and "isnegative" to the different parts, as
		 *  appropriate.
		 *
		 *  Handles a wide variety of date/time formats.
		 *
		 *  If "ddmm" is set true then the date format is assumed to
		 *  be dd/mm/yyyy rather than mm/dd/yyyy when a date with a
		 *  trailing year is encountered.
		 *
		 *  If "yyyyddmm" is set true then the date format is assumed
		 *  to be yyyy/dd/mm rather than yyyy/mm/dd when a date with
		 *  a leading year is encountered.
		 *
		 *  "datedelimiters" may be set to a set of valid date
		 *  delimiters and may contain any combination of '/', '-', '.',
		 *  and ':'.  Eg. "/-" would mean that only '/' and '-' are
		 *  valid date delimiters.  If left NULL then it defaults to
		 *  "/-.:"
		 *
		 *  If a component isn't found, then it is set to -1.  Eg. if a
		 *  date is given with no time, then "hour", "minute", "second",
		 *  and "microsecond" are set to -1.
		 *
		 *  Any of "year", "month", "day", "hour", "minute", "second",
		 *  "microsecond", and "isnegative" may be NULL.  In that case,
		 *  that component is just not set.
		 *
		 *  "isnegative" is set true if the hour component is negative.
		 *  Eg. if "-10:00" was encountered.  Note that if a negative
		 *  hour component is encountered, then the "hour" will be set
		 *  to a positive number, and "isnegative" will be set true.
		 *  In the example above, "hour" would bet set to 10 and
		 *  "isnegative" would be set true.
		 *
		 *  Returns true if the date/time was successfully parsed and
		 *  false if it failed to parse the date/time. */
		static bool	parse(const char *datetime,
					bool ddmm, bool yyyyddmm,
					const char *datedelimiters,
					int16_t *year, int16_t *month,
					int16_t *day, int16_t *hour,
					int16_t *minute, int16_t *second,
					int32_t *microsecond, bool *isnegative);

		/** Attempts to parse "datetime" into its components setting
		 *  "year", "month", "day", "hour", "minute", "second",
		 *  "microsecond", and "isnegative" to the different parts, as
		 *  appropriate.
		 *
		 *  Handles a wide variety of date/time formats.
		 *
		 *  If "ddmm" is set true then the date format is assumed to
		 *  be dd/mm/yyyy rather than mm/dd/yyyy when a date with a
		 *  trailing year is encountered.
		 *
		 *  If "yyyyddmm" is set true then the date format is assumed
		 *  to be yyyy/dd/mm rather than yyyy/mm/dd when a date with
		 *  a leading year is encountered.
		 *
		 *  "datedelimiters" may be set to a set of valid date
		 *  delimiters and may contain any combination of '/', '-', '.',
		 *  and ':'.  Eg. "/-" would mean that only '/' and '-' are
		 *  valid date delimiters.  If left NULL then it defaults to
		 *  "/-.:"
		 *
		 *  If "adjustshortyear" is true then years less than 100, and
		 *  greater than 50 will be presumed to be 1950+ and years less
		 *  than 100, and less than 50 will be presumed to be 2000+.
		 *  If "adjustshortyear" is false then years will not be
		 *  adjusted.
		 *
		 *  If a component isn't found, then it is set to -1.  Eg. if a
		 *  date is given with no time, then "hour", "minute", "second",
		 *  and "microsecond" are set to -1.
		 *
		 *  Any of "year", "month", "day", "hour", "minute", "second",
		 *  "microsecond", and "isnegative" may be NULL.  In that case,
		 *  that component is just not set.
		 *
		 *  "isnegative" is set true if the hour component is negative.
		 *  Eg. if "-10:00" was encountered.  Note that if a negative
		 *  hour component is encountered, then the "hour" will be set
		 *  to a positive number, and "isnegative" will be set true.
		 *  In the example above, "hour" would bet set to 10 and
		 *  "isnegative" would be set true.
		 *
		 *  Returns true if the date/time was successfully parsed and
		 *  false if it failed to parse the date/time. */
		static bool	parse(const char *datetime,
					bool ddmm, bool yyyyddmm,
					const char *datedelimiters,
					bool adjustshortyear,
					int16_t *year, int16_t *month,
					int16_t *day, int16_t *hour,
					int16_t *minute, int16_t *second,
					int32_t *microsecond, bool *isnegative);

		/** Allocates and returns a date/time string, composed of the
		 *  components "year", "month", "day", "hour", "minute",
		 *  "second", and "microsecond" as specified by the format
		 *  string "format".  If "isnegative" is set true then the hour
		 *  component will be negative.
		 *
		 *  "format" may contain any of the following format strings:
		 *
		 *  DD - 2 digit day, left padded with 0
		 *  D - 1 or 2 digit day, not left padded with 0
		 *  MM - 2 digit month, left padded with 0
		 *  MON - uppercase, 3-character month abbreviation
		 *  Mon - mixed-case, 3-character month abbreviation
		 *  Month - mixed-case full name of the month
		 *  YYYY - 4 digit year, left padded with 0
		 *  YY - 2 digit year, left padded with 0
		 *  HH24 - 2 digit hour, in 24-hour format, left padded with 0
		 *  HH - 2 digit hour, in 12-hour format, left padded with 0
		 *  MI - 2 digit minute, left padded with 0
		 *  SS - 2 digit second, left padded with 0
		 *  FFFFFF -  6 digit fractional second, left padded with 0
		 *  FFFFF -  5 digit fractional second, left padded with 0
		 *  FFFF -  4 digit fractional second, left padded with 0
		 *  FFF -  3 digit fractional second, left padded with 0
		 *  FF -  2 digit fractional second, left padded with 0
		 *  F -  1 digit fractional second
		 *  AM - AM or PM
		 *
		 *  Any other characters in "format" will be reprouduced
		 *  verbatim.
		 *
		 *  Returns the formatted string or NULL if "format" was
		 *  NULL. */
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
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/datetime.h>
};

#endif
