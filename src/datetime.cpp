// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/error.h>
#if defined(RUDIMENTS_HAVE_RTC_SET_TIME) || \
		defined(RUDIMENTS_HAVE_RTC_GET_TIME)
	#include <rudiments/file.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	// SCO OSR 5.0.0 appears to need this extern "C"
	extern "C" {
		#include <sys/time.h>
	}
#endif
#if defined(RUDIMENTS_HAVE_RTC_SET_TIME) || \
		defined(RUDIMENTS_HAVE_RTC_GET_TIME)
	#ifdef RUDIMENTS_HAVE_LINUX_RTC_H
		#include <linux/rtc.h>
	#endif
	#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
		#include <sys/ioctl.h>
	#endif
#endif

#ifdef RUDIMENTS_HAVE_OS_KERNEL_OS_H
	#include <os/kernel/OS.h>
#endif

#ifdef RUDIMENTS_HAS_UNDEFINED_TZSET
extern "C" void tzset();
#endif

class datetimeprivate {
	friend class datetime;
	private:
		int32_t	_usec;

		int32_t	_sec;
		int32_t	_min;
		int32_t	_hour;
		int32_t	_mday;
		int32_t	_mon;
		int32_t	_year;
		int32_t	_wday;
		int32_t	_yday;
		int32_t	_yweek;
		int32_t	_isdst;

		char	*_zone;
		int32_t	_gmtoff;

		char	*_timestring;
		char	*_sqlstring;

		time_t	_epoch;

		#if defined(RUDIMENTS_HAS__GET_TZNAME)
			char	_timezonename[16];
		#endif

		#if !defined(RUDIMENTS_HAS__GET_TZNAME) && \
				!defined(RUDIMENTS_HAS__TZNAME) && \
				!defined(RUDIMENTS_HAS_TZNAME) && \
				defined(RUDIMENTS_HAS_STRFTIME)
			char	_tzname[32];
		#endif

		bool	_dirty;
};

static threadmutex	*_datetimemutex=NULL;

static const char _monthname[][10]={
	"January","February","March",
	"April","May","June",
	"July","August","September",
	"October","November","December"
};

static const char _monthabbr[][4]={
	"Jan","Feb","Mar",
	"Apr","May","Jun",
	"Jul","Aug","Sep",
	"Oct","Nov","Dec"
};

datetime::datetime() : object() {
	pvt=new datetimeprivate;
	construct();
}

datetime::datetime(datetime &d) : object() {
	pvt=new datetimeprivate;
	construct();
	init(d.getString());
}

datetime &datetime::operator=(datetime &d) {
	if (this!=&d) {
		init(d.getString());
	}
	return *this;
}

datetime::~datetime() {
	clear();
	delete pvt;
}

void datetime::construct() {
	pvt->_usec=0;
	pvt->_sec=0;
	pvt->_min=0;
	pvt->_hour=0;
	pvt->_mday=1;
	pvt->_mon=0;
	pvt->_year=0;
	pvt->_wday=0;
	pvt->_yday=0;
	pvt->_yweek=0;
	pvt->_isdst=0;
	pvt->_zone=NULL;
	pvt->_gmtoff=0;
	pvt->_timestring=NULL;
	pvt->_sqlstring=NULL;
	pvt->_epoch=0;
	#if defined(RUDIMENTS_HAS__GET_TZNAME)
	bytestring::zero(pvt->_timezonename,sizeof(pvt->_timezonename));
	#endif
	#if !defined(RUDIMENTS_HAS__GET_TZNAME) && \
			!defined(RUDIMENTS_HAS__TZNAME) && \
			!defined(RUDIMENTS_HAS_TZNAME) && \
			defined(RUDIMENTS_HAS_STRFTIME)
	bytestring::zero(pvt->_tzname,sizeof(pvt->_tzname));
	#endif
	pvt->_dirty=true;
}

bool datetime::clear() {
	delete[] pvt->_zone;
	delete[] pvt->_timestring;
	delete[] pvt->_sqlstring;
	return true;
}

bool datetime::init(const char *tmstring) {

	clear();
	construct();

	// parse out the date
	const char	*ptr=tmstring;
	pvt->_mon=charstring::convertToInteger(ptr)-1;
	ptr=charstring::findFirst(ptr,'/');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_mday=charstring::convertToInteger(ptr);
	ptr=charstring::findFirst(ptr,'/');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_year=charstring::convertToInteger(ptr)-1900;

	// parse out the time
	ptr=charstring::findFirst(ptr,' ');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_hour=charstring::convertToInteger(ptr);
	ptr=charstring::findFirst(ptr,':');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_min=charstring::convertToInteger(ptr);
	ptr=charstring::findFirst(ptr,':');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_sec=charstring::convertToInteger(ptr);

	// parse out microseconds, if provided
	pvt->_usec=0;
	const char *usecptr=charstring::findFirst(ptr,':');
	if (usecptr) {
		pvt->_usec=charstring::convertToInteger(usecptr+1);
	}

	// initialize the daylight savings time flag
	pvt->_isdst=-1;

	// parse out the time zone if it was provided
	ptr=charstring::findFirst(ptr,' ');
	if (ptr) {
		ptr=ptr+sizeof(char);
		if (!charstring::compare(ptr,"Z")) {
			ptr="GMT";
			// FIXME: handle other military time zones
		}
		pvt->_zone=(ptr && ptr[0])?charstring::duplicate(ptr):NULL;
	} else {
		pvt->_zone=NULL;
	}
	return true;
}

bool datetime::init(time_t seconds) {
	return init(seconds,0);
}

bool datetime::init(time_t seconds, time_t microseconds) {

	clear();
	construct();

	pvt->_epoch=seconds;
	pvt->_usec=microseconds;
	if (!acquireLock()) {
		return false;
	}
	bool	retval=getBrokenDownTimeFromEpoch();
	releaseLock();
	return retval;
}

bool datetime::init(const void *tmstruct) {

	clear();
	construct();

	const struct tm	*tms=(const struct tm *)tmstruct;

	pvt->_usec=0;

	pvt->_sec=tms->tm_sec;
	pvt->_min=tms->tm_min;
	pvt->_hour=tms->tm_hour;
	pvt->_mday=tms->tm_mday;
	pvt->_mon=tms->tm_mon;
	pvt->_year=tms->tm_year;
	pvt->_wday=tms->tm_wday;
	pvt->_yday=tms->tm_yday;
	pvt->_yweek=0;
	pvt->_isdst=tms->tm_isdst;

	#if defined(RUDIMENTS_HAS___TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->__tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_NAME)
		pvt->_zone=charstring::duplicate(tms->tm_name);
	#else
		if (!acquireLock()) {
			return false;
		}
		#if defined(RUDIMENTS_HAS__TZSET)
			_tzset();
		#elif defined(RUDIMENTS_HAS_TZSET) || \
			defined(RUDIMENTS_HAS_UNDEFINED_TZSET)
			tzset();
		#else
			#error no tzset or anything like it
		#endif
		const char	*tzn=getTzName(pvt->_isdst,(void *)tms);
		pvt->_zone=charstring::duplicate((tzn && tzn[0])?tzn:"UCT");
		#if defined(RUDIMENTS_HAS__GET_TIMEZONE)
			long	seconds;
			_get_timezone(&seconds);
			pvt->_gmtoff=-seconds;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-timezone;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-_timezone;
		#else
			#error no timezone or anything like it
		#endif
		releaseLock();
	#endif

	#if defined(RUDIMENTS_HAS___TM_GMTOFF)
		pvt->_gmtoff=tms->__tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
		pvt->_gmtoff=tms->tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_TZADJ)
		pvt->_gmtoff=-tms->tm_tzadj;
	#endif

	return true;
}

int32_t datetime::getHour() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_hour;
}

int32_t datetime::getMinute() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_min;
}

int32_t datetime::getSecond() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_sec;
}

int32_t datetime::getMicrosecond() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_usec;
}

int32_t datetime::getMonth() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_mon+1;
}

const char *datetime::getMonthName() {
	if (pvt->_dirty) {
		normalize();
	}
	return _monthname[pvt->_mon];
}

const char *datetime::getMonthAbbreviation() {
	if (pvt->_dirty) {
		normalize();
	}
	return _monthabbr[pvt->_mon];
}

int32_t datetime::getDayOfMonth() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_mday;
}

int32_t datetime::getDayOfWeek() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_wday+1;
}

int32_t datetime::getDayOfYear() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_yday+1;
}

int32_t datetime::getWeekOfYear() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_yweek;
}

int32_t datetime::getYear() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_year+1900;
}

int32_t datetime::getShortYear() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_year%100;
}

int32_t datetime::getCentury() {
	if (pvt->_dirty) {
		normalize();
	}
	return (pvt->_year<100)?20:((pvt->_year/100)+20);
}

bool datetime::isDaylightSavingsTime() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_isdst!=0;
}

const char *datetime::getTimeZoneString() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_zone;
}

int32_t datetime::getTimeZoneOffset() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_gmtoff;
}

time_t datetime::getEpoch() {
	if (pvt->_dirty) {
		normalize();
	}
	return pvt->_epoch;
}

bool datetime::setMicrosecond(int32_t microsecond) {
	pvt->_usec=microsecond;
	pvt->_dirty=true;
	return true;
}

bool datetime::setSecond(int32_t second) {
	pvt->_sec=second;
	pvt->_dirty=true;
	return true;
}

bool datetime::setMinute(int32_t minute) {
	pvt->_min=minute;
	pvt->_dirty=true;
	return true;
}

bool datetime::setHour(int32_t hour) {
	pvt->_hour=hour;
	pvt->_dirty=true;
	return true;
}

bool datetime::setDayOfMonth(int32_t day) {
	pvt->_mday=day;
	pvt->_dirty=true;
	return true;
}

bool datetime::setMonth(int32_t month) {
	pvt->_mon=month-1;
	pvt->_dirty=true;
	return true;
}

bool datetime::setYear(int32_t year) {
	pvt->_year=year-1900;
	pvt->_dirty=true;
	return true;
}

bool datetime::addMicroseconds(int32_t microseconds) {
	pvt->_usec=pvt->_usec+microseconds;
	pvt->_dirty=true;
	return true;
}

bool datetime::addSeconds(int32_t seconds) {
	pvt->_sec=pvt->_sec+seconds;
	pvt->_dirty=true;
	return true;
}

bool datetime::addMinutes(int32_t minutes) {
	pvt->_min=pvt->_min+minutes;
	pvt->_dirty=true;
	return true;
}

bool datetime::addHours(int32_t hours) {
	pvt->_hour=pvt->_hour+hours;
	pvt->_dirty=true;
	return true;
}

bool datetime::addDays(int32_t days) {
	pvt->_mday=pvt->_mday+days;
	pvt->_dirty=true;
	return true;
}

bool datetime::addMonths(int32_t months) {
	pvt->_mon=pvt->_mon+months;
	pvt->_dirty=true;
	return true;
}

bool datetime::addYears(int32_t years) {
	pvt->_year=pvt->_year+years;
	pvt->_dirty=true;
	return true;
}

void datetime::setMutex(threadmutex *mtx) {
	_datetimemutex=mtx;
}

const char *datetime::getString() {
	return getString(false);
}

const char *datetime::getString(bool microseconds) {
	if (pvt->_dirty) {
		normalize();
	}
	delete[] pvt->_timestring;
	stringbuffer	timestr;
	timestr.append(getMonth(),2)->append('/');
	timestr.append(getDayOfMonth(),2)->append('/');
	timestr.append(getYear())->append(' ');
	timestr.append(getHour(),2)->append(':');
	timestr.append(getMinute(),2)->append(':');
	timestr.append(getSecond(),2);
	if (microseconds) {
		timestr.append(':')->append(getMicrosecond(),3);
	}
	timestr.append(' ');
	timestr.append(getTimeZoneString());
	pvt->_timestring=timestr.detachString();
	return pvt->_timestring;
}

const char *datetime::getSqlString() {
	return getSqlString(false);
}

const char *datetime::getSqlString(bool microseconds) {
	if (pvt->_dirty) {
		normalize();
	}
	delete[] pvt->_sqlstring;
	stringbuffer	timestr;
	timestr.append(getYear())->append('-');
	timestr.append(getMonth(),2)->append('-');
	timestr.append(getDayOfMonth(),2)->append(' ');
	timestr.append(getHour(),2)->append(':');
	timestr.append(getMinute(),2)->append(':');
	timestr.append(getSecond(),2);
	if (microseconds) {
		timestr.append('.')->append(getMicrosecond(),3);
	}
	pvt->_sqlstring=timestr.detachString();
	return pvt->_sqlstring;
}

bool datetime::initFromSystemDateTime() {
	#if defined(RUDIMENTS_HAVE_GETTIMEOFDAY)
		struct timeval	tv;
		if (gettimeofday(&tv,NULL)) {
			return false;
		}
		return init(tv.tv_sec,tv.tv_usec);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIMEASFILETIME)

		#ifdef RUDIMENTS_HAVE_LONG_LONG
			// FILETIME contains the number of 100 nanosecond
			// intervals since Jan 1, 1601 UTC.
			FILETIME	ft;
			GetSystemTimeAsFileTime(&ft);

			// convert to a single 64-bit number
			uint64_t	t=ft.dwHighDateTime;
			t<<=32;
			t|=ft.dwLowDateTime;

			// convert to microseconds
			t/=10;

			// subtract microseconds between 1601 and 1970
			t-=11644473600000000ULL;

			return init(t/1000000,t%1000000);
		#else
			SYSTEMTIME	st;
			GetLocalTime(&st);

			stringbuffer	str;
			str.append(st.wMonth)->append('/');
			str.append(st.wDay)->append('/');
			str.append(st.wYear)->append(' ');
			str.append(st.wHour)->append(':');
			str.append(st.wMinute)->append(':');
			str.append(st.wSecond)->append('.');
			str.append(st.wMilliseconds*1000);

			return init(str.getString());
		#endif
	#else
		return init(time(NULL));
	#endif
}

bool datetime::setSystemDateTime() {
	// FIXME: should set /etc/localtime (or /etc/TZ) and TZ env var too...
	#if defined(RUDIMENTS_HAVE_SETSYSTEMTIME)
		SYSTEMTIME	st;
		st.wYear=pvt->_year+1900;
		st.wMonth=pvt->_mon+1;
		st.wDayOfWeek=pvt->_wday;
		st.wDay=pvt->_mday;
		st.wHour=pvt->_hour;
		st.wMinute=pvt->_min;
		st.wSecond=pvt->_sec;
		st.wMilliseconds=pvt->_usec/1000;
		return SetSystemTime(&st)!=0;
	#elif defined(RUDIMENTS_HAVE_SETTIMEOFDAY)
		timeval	tv;
		tv.tv_sec=pvt->_epoch;
		tv.tv_usec=pvt->_usec;
		return !settimeofday(&tv,NULL);
	#elif defined(RUDIMENTS_HAVE_CLOCK_SETTIME)
		timespec	ts;
		ts.tv_sec=pvt->_epoch;
		ts.tv_nsec=pvt->_usec*1000;
		return !clock_settime(CLOCK_REALTIME,&ts);
	#elif defined(RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK)
		set_real_time_clock(pvt->_epoch);
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool datetime::initFromHardwareDateTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC_SET_TIME
		// open the rtc
		file	devrtc;
		if (!devrtc.open("/dev/rtc",O_RDONLY)) {
			return false;
		}

		// get the time from the rtc
		rtc_time rt;
		if (devrtc.ioCtl(RTC_RD_TIME,&rt)==-1) {
			devrtc.close();
			return false;
		}
		devrtc.close();

		// set the local values
		pvt->_mon=rt.tm_mon;
		pvt->_mday=rt.tm_mday;
		pvt->_year=rt.tm_year;
		pvt->_hour=rt.tm_hour;
		pvt->_min=rt.tm_min;
		pvt->_sec=rt.tm_sec;
		pvt->_usec=0;
		pvt->_isdst=rt.tm_isdst;
		pvt->_zone=charstring::duplicate(hwtz);

		return normalize();
	#else
		return false;
	#endif
}

bool datetime::initFromAdjustedHardwareDateTime(const char *hwtz) {
	return (initFromHardwareDateTime(hwtz) && setTimeZone(NULL));
}

bool datetime::setHardwareDateTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC_SET_TIME
		// open the rtc
		file	devrtc;
		if (!devrtc.open("/dev/rtc",O_WRONLY)) {
			return false;
		}

		// adjust the time zone
		if (!setTimeZone(hwtz)) {
			devrtc.close();
			return false;
		}

		// set the values to be stored in the rtc
		rtc_time rt;
		rt.tm_mon=pvt->_mon;
		rt.tm_mday=pvt->_mday;
		rt.tm_year=pvt->_year;
		rt.tm_hour=pvt->_hour;
		rt.tm_min=pvt->_min;
		rt.tm_sec=pvt->_sec;
		rt.tm_wday=pvt->_wday;
		rt.tm_yday=pvt->_yday;
		rt.tm_isdst=pvt->_isdst;

		// set the rtc and clean up
		bool	retval=(devrtc.ioCtl(RTC_SET_TIME,&rt)!=-1);
		devrtc.close();

		return retval;
	#else
		return false;
	#endif
}

bool datetime::setTimeZone(const char *newtz) {
	return setTimeZone(newtz,false);
}

bool datetime::setTimeZone(const char *newtz, bool ignoredst) {

	if (!acquireLock()) {
		return false;
	}

	// If we're dirty and the epoch is 0 then we need to call normalize()
	// to make sure the epoch gets set.  Bail if that fails.
	if (pvt->_dirty && !pvt->_epoch && !normalize()) {
		releaseLock();
		return false;
	}

	// Change the time zone, get the broken down time relative to the
	// current epoch, in the new time zone.
	char	*oldzone=NULL;
	bool	retval=setTZ(newtz,&oldzone,ignoredst) &&
				getBrokenDownTimeFromEpoch() &&
				restoreTZ(oldzone);
	releaseLock();
	return retval;
}

char *datetime::getString(time_t seconds) {
	datetime	dt;
	return ((dt.init(seconds))?
		charstring::duplicate(dt.getString()):NULL);
}

char *datetime::getString(time_t seconds, time_t microseconds) {
	datetime	dt;
	return ((dt.init(seconds,microseconds))?
		charstring::duplicate(dt.getString(true)):NULL);
}

char *datetime::getString(const void *tmstruct) {
	datetime	dt;
	return ((dt.init(tmstruct))?
		charstring::duplicate(dt.getString()):NULL);
}

time_t datetime::getEpoch(const char *datestring) {
	datetime	dt;
	return ((dt.init(datestring))?dt.getEpoch():-1);
}

time_t datetime::getEpoch(const void *tmstruct) {
	datetime	dt;
	return ((dt.init(tmstruct))?dt.getEpoch():-1);
}

bool datetime::setTZ(const char *zone, char **oldzone, bool ignoredst) {

	// If a daylight timezone was passed in, override it with the combined
	// timezone.
	//
	// If a standard timezone was passed in, but we want to account for
	// daylight savings time (eg. EST was passed in, but daylight savings
	// time is currently in effect, so we want to use EDT instead) then
	// use the combined timezone.
	const char	*combinedzone=lookupCombinedTimeZone(zone);
	const char	*realzone=zone;
	if (daylightZone(zone) || !ignoredst) {
		realzone=combinedzone;
	}

	// get the current value of TZ
	const char	*tz=environment::getValue("TZ");
	if (charstring::isNullOrEmpty(tz)) {
		*oldzone=NULL;
	} else {
		*oldzone=charstring::duplicate(tz);
	}

	// If realzone is non-null/non-empty then set TZ to it.  If realzone is
	// null/empty then ignore it and just keep whatever TZ is already set
	// to.
	//
	// It's compilcated why we want to do this...
	//
	// Most platforms have an "internal timezone" set by /etc/localtime or
	// /etc/TIMEZONE, or some other way (eg. on Windows, Solaris 11, etc.)
	// but some (eg. UnixWare 7.0.1) just set the TZ environment variable.
	//
	// On all platforms, the TZ environment variable overrides the
	// "internal timezone".  Setting it null/empty is undefined, but
	// usually results in either it being ignored entirely and falling back
	// to the "internal timezone", or GMT being used.  More often, the
	// former.
	//
	// This method is called when normalizing the time, or when we
	// specifically want to change the time zone.
	//
	// FIXME: not any more...  Also, often,
	// setTZ(pvt->_zone) gets called by construct() when pvt->_zone was
	// just initialized to NULL.
	//
	// In these cases, if we're asked to set TZ to a null/empty value, then
	// ignoring it and not setting TZ at all is really what we want to do -
	// not set it to something with undefined results, and preserve the
	// existing TZ on platforms that depend on it (eg. UnixWare).
	if (!charstring::isNullOrEmpty(realzone)) {
		return environment::setValue("TZ",realzone);
	}
	return true;
}

bool datetime::restoreTZ(char *oldzone) {

	// if oldzone is null or empty then remove the TZ environment variable
	if (charstring::isNullOrEmpty(oldzone)) {
		environment::remove("TZ");
		delete[] oldzone;
		return true;
	}

	// otherwise, set it to whatever oldzone was set to
	bool	retval=environment::setValue("TZ",oldzone);
	delete[] oldzone;
	return retval;
}

bool datetime::getBrokenDownTimeFromEpoch() {

	struct tm	*tms=NULL;
	#ifdef RUDIMENTS_HAVE_LOCALTIME_S
		// localtime_s appears to rely on one or more of:
		// _daylight, _timezone or _tzname rather than just on TZ,
		// so we have to call _tzset() before calling localtime_s.
		//
		// NOTE: We must zero our struct tm because it may have members
		// other than the ones we're setting which mktime() may not set,
		// but which strftime() attempts to use.  Eg. strftime() in
		// glibc 2.0.7 (redhat 5.2) tries to access tm_zone.
		_tzset();
		struct tm	tm;
		bytestring::zero(&tm,sizeof(tm));
		if (localtime_s(&tm,&pvt->_epoch)) {
			return false;
		}
		tms=&tm;
	#else
		// I'm using localtime here instead of localtime_r because
		// localtime_r doesn't appear to handle the timezone properly,
		// at least, not in glibc-2.3
		tms=localtime(&pvt->_epoch);
		if (!tms) {
			return false;
		}
	#endif
	pvt->_epoch=mktime(tms);
	pvt->_sec=tms->tm_sec;
	pvt->_min=tms->tm_min;
	pvt->_hour=tms->tm_hour;
	pvt->_mday=tms->tm_mday;
	pvt->_mon=tms->tm_mon;
	pvt->_year=tms->tm_year;
	pvt->_isdst=tms->tm_isdst;
	pvt->_wday=tms->tm_wday;
	pvt->_yday=tms->tm_yday;
	processTZ((void *)tms);

	setWeekOfYear((void *)tms);

	pvt->_dirty=false;

	return (pvt->_epoch!=-1);
}

void datetime::setWeekOfYear(void *tms) {
	#ifdef RUDIMENTS_HAS_STRFTIME
	char	yweek[3];
	strftime(yweek,sizeof(yweek),"%W",(struct tm *)tms);
	pvt->_yweek=charstring::convertToInteger(yweek);
	#else
		#error no strftime or anything like it
	#endif
}

bool datetime::normalize() {

	if (!acquireLock()) {
		return false;
	}

	// set the TZ environment variable, as mktime() below depends on it
	char	*oldzone=NULL;
	if (!setTZ(pvt->_zone,&oldzone,false)) {
		releaseLock();
		return false;
	}

	// normalize microseconds
	pvt->_sec+=pvt->_usec/1000000;
	pvt->_usec%=1000000;

	// copy relevent values into a struct tm
	//
	// NOTE: We must zero our struct tm because it may have members other
	// than the ones we're setting which mktime() may not set, but which
	// strftime() attempts to use.  Eg. strftime() in glibc 2.0.7
	// (redhat 5.2) tries to access tm_zone.
	struct tm	tms;
	bytestring::zero(&tms,sizeof(tms));
	tms.tm_sec=pvt->_sec;
	tms.tm_min=pvt->_min;
	tms.tm_hour=pvt->_hour;
	tms.tm_mday=pvt->_mday;
	tms.tm_mon=pvt->_mon;
	tms.tm_year=pvt->_year;
	tms.tm_isdst=pvt->_isdst;

	// call mktime() to get the epoch, set wday, yday and isdst
	// and normalize other values
	pvt->_epoch=mktime(&tms);

	// copy values back out of struct tm
	pvt->_sec=tms.tm_sec;
	pvt->_min=tms.tm_min;
	pvt->_hour=tms.tm_hour;
	pvt->_mday=tms.tm_mday;
	pvt->_mon=tms.tm_mon;
	pvt->_year=tms.tm_year;
	pvt->_wday=tms.tm_wday;
	pvt->_yday=tms.tm_yday;
	pvt->_isdst=tms.tm_isdst;

	// set pvt->_zone and pvt->_gmtoff
	processTZ((void *)&tms);

	bool	retval=(pvt->_epoch!=-1);

	// restore the TZ variable to whatever it was, originally
	if (!charstring::isNullOrEmpty(pvt->_zone)) {
		restoreTZ(oldzone);
	}

	releaseLock();

	setWeekOfYear((void *)&tms);

	pvt->_dirty=false;

	return retval;
}

void datetime::processTZ(void *tms) {

	// Use tzset to get the timezone name
	#if defined(RUDIMENTS_HAS__TZSET)
		_tzset();
	#elif defined(RUDIMENTS_HAS_TZSET) || \
		defined(RUDIMENTS_HAS_UNDEFINED_TZSET)
		tzset();
	#else
		#error no tzset or anything like it
	#endif

	delete[] pvt->_zone;
	pvt->_zone=charstring::duplicate(getTzName(pvt->_isdst,tms));

	// Get the offset from the struct tm if we can, otherwise get
	// it from the value set by tzset()
	#if defined(RUDIMENTS_HAS___TM_GMTOFF)
		pvt->_gmtoff=((struct tm *)tms)->__tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
		pvt->_gmtoff=((struct tm *)tms)->tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_TZADJ)
		pvt->_gmtoff=-((struct tm *)tms)->tm_tzadj;
	#elif defined(RUDIMENTS_HAS__GET_TIMEZONE)
		long	seconds;
		_get_timezone(&seconds);
		pvt->_gmtoff=-seconds;
		// apparently _get_timezine doesn't take DST into account
		//
		// isdst will be:
		// * positive if daylight savings time is in effect
		// * zero if not in effect
		// * negative if the information is not available
		//
		// We'll assume that if the info is not available then daylight
		// savings time is not in effect.
		if (pvt->_isdst==1) {
			pvt->_gmtoff+=3600;
		}
	#elif defined(RUDIMENTS_HAS_TIMEZONE)
		pvt->_gmtoff=-timezone;
		// apparently timezone doesn't take DST into account
		//
		// isdst will be:
		// * positive if daylight savings time is in effect
		// * zero if not in effect
		// * negative if the information is not available
		//
		// We'll assume that if the info is not available then daylight
		// savings time is not in effect.
		if (pvt->_isdst==1) {
			pvt->_gmtoff+=3600;
		}
	#elif defined(RUDIMENTS_HAS__TIMEZONE)
		pvt->_gmtoff=-_timezone;
	#else
		#error no timezone or anything like it
	#endif
}

const char *datetime::getTzName(int32_t isdst, void *tms) {

	// isdst will be:
	// * positive if daylight savings time is in effect
	// * zero if not in effect
	// * negative if the information is not available
	//
	// We'll assume that if the info is not available then daylight
	// savings time is not in effect.  Also, we have to do the calculation
	// in each branch to avoid unused variable warnings/errors.

	#if defined(RUDIMENTS_HAS__GET_TZNAME)
		size_t	timezonenamelen;
		_get_tzname(&timezonenamelen,
				pvt->_timezonename,
				sizeof(pvt->_timezonename),
				(isdst==1)?1:0);
		return pvt->_timezonename;
	#elif defined(RUDIMENTS_HAS__TZNAME)
		return _tzname[(isdst==1)?1:0];
	#elif defined(RUDIMENTS_HAS_TZNAME)
		return tzname[(isdst==1)?1:0];
	#elif defined(RUDIMENTS_HAS_TM_ZONE)
		return ((struct tm *)tms)->tm_zone;
	#else
		#error no tzname or anything like it
	#endif
}

bool datetime::acquireLock() {
	return !(_datetimemutex && !_datetimemutex->lock());
}

bool datetime::releaseLock() {
	return !(_datetimemutex && !_datetimemutex->unlock());
}

static const char * const _timezones[]={

	"ACST",	// Australian Central Standard Time	UTC + 9:30 hours
	"ACDT",	// Australian Central Daylight Time	UTC + 10:30 hours
	"ACST-10:30ACDT",

	"AST",	// Atlantic Standard Time		UTC - 4 hours
	"ADT",	// Atlantic Daylight Time		UTC - 3 hours
	"AST4ADT",

	"AEST",	// Australian Eastern Standard Time	UTC + 10 hours
	"AEDT",	// Australian Eastern Daylight Time	UTC + 11 hours
	"AEST10AEDT",

	"AKST",	// Alaska Standard Time	UTC - 9 hours
	"AKDT",	// Alaska Daylight Time	UTC - 8 hours
	"AKST-9AKDT",

	"CST",	// Central Standard Time	UTC - 6 hours
	"CDT",	// Central Daylight Time	UTC - 5 hours
	"CST6CDT",

	"CET",	// Central European Time	UTC + 1 hour
	"CEST",	// Central European Summer Time	UTC + 2 hours
	"CET-1CST",

	"EST",	// Eastern Standard Time	UTC - 5 hours
	"EDT",	// Eastern Daylight Time	UTC - 4 hours
	"EST5EDT",

	"EET",	// Eastern European Time	UTC + 2 hours
	"EEST",	// Eastern European Summer Time	UTC + 3 hours
	"EET-2EEST",

	"GMT",	// Greenwich Mean Time	UTC
	"BST",	// British Summer Time	UTC + 1 hour
	"GMT0BST",

	"HNA",	// Heure Normale de l'Atlantique	UTC - 4 hours
	"HAA",	// Heure Avancée de l'Atlantique	UTC - 3 hours
	"HNA4HAA",

	"HNC",	// Heure Normale du Centre		UTC - 6 hours
	"HAC",	// Heure Avancée du Centre		UTC - 5 hours
	"HNC6HAC",

	"HAST",	// Hawaii-Aleutian Standard Time	UTC - 10 hours
	"HADT",	// Hawaii-Aleutian Daylight Time	UTC - 9 hours
	"HAST10HADT",

	"HNE",	// Heure Normale de l'Est	UTC - 5 hours
	"HAE",	// Heure Avancée de l'Est	UTC - 4 hours
	"HNE5HAE",

	"HNP",	// Heure Normale du Pacifique	UTC - 8 hours
	"HAP",	// Heure Avancée du Pacifique	UTC - 7 hours
	"HNP8HAP",

	"HNR",	// Heure Normale des Rocheuses	UTC - 7 hours
	"HAR",	// Heure Avancée des Rocheuses	UTC - 6 hours
	"HNR7HAR",

	"HNT",	// Heure Normale de Terre-Neuve	UTC - 3:30 hours
	"HAT",	// Heure Avancée de Terre-Neuve	UTC - 2:30 hours
	"HNT3:30HAT",

	"HNY",	// Heure Normale du Yukon	UTC - 9 hours
	"HAY",	// Heure Avancée du Yukon	UTC - 8 hours
	"HNY9HAY",

	"MST",	// Mountain Standard Time	UTC - 7 hours
	"MDT",	// Mountain Daylight Time	UTC - 6 hours
	"MST7MDT",

	"MEZ",	// Mitteleuropäische Zeit	UTC + 1 hour
	"MESZ",	// Mitteleuropäische Sommerzeit	UTC + 2 hours
	"MEZ-1MESZ",

	"NST",	// Newfoundland Standard Time	UTC - 3:30 hours
	"NDT",	// Newfoundland Daylight Time	UTC - 2:30 hours
	"NST3:30NDT",

	"PST",	// Pacific Standard Time	UTC - 8 hours
	"PDT",	// Pacific Daylight Time	UTC - 7 hours
	"PST8PDT",

	"WET",	// Western European Time	UTC
	"WEST",	// Western European Summer Time	UTC + 1 hour
	"WET-1WEST",

	"",
	"",
	""
};

static const int32_t	_timezoneoffsets[]={

	34200,	// Australian Central Standard Time	UTC + 9:30 hours
	37800,	// Australian Central Daylight Time	UTC + 10:30 hours
	34200,

	-14400,	// Atlantic Standard Time		UTC - 4 hours
	-10800,	// Atlantic Daylight Time		UTC - 3 hours
	-14400,

	36000,	// Australian Eastern Standard Time	UTC + 10 hours
	39600,	// Australian Eastern Daylight Time	UTC + 11 hours
	36000,

	-32400,	// Alaska Standard Time	UTC - 9 hours
	-28800,	// Alaska Daylight Time	UTC - 8 hours
	-32400,

	-21600,	// Central Standard Time	UTC - 6 hours
	-18000,	// Central Daylight Time	UTC - 5 hours
	-21600,

	3600,	// Central European Time	UTC + 1 hour
	7200,	// Central European Summer Time	UTC + 2 hours
	3600,

	-18000,	// Eastern Standard Time	UTC - 5 hours
	-14400,	// Eastern Daylight Time	UTC - 4 hours
	-18000,

	7200,	// Eastern European Time	UTC + 2 hours
	10800,	// Eastern European Summer Time	UTC + 3 hours
	7200,

	0,	// Greenwich Mean Time	UTC
	3600,	// British Summer Time	UTC + 1 hour
	0,

	-14400,	// Heure Normale de l'Atlantique	UTC - 4 hours
	-10800,	// Heure Avancée de l'Atlantique	UTC - 3 hours
	-14400,

	-21600,	// Heure Normale du Centre		UTC - 6 hours
	-18000,	// Heure Avancée du Centre		UTC - 5 hours
	-21600,

	-36000,	// Hawaii-Aleutian Standard Time	UTC - 10 hours
	-32400,	// Hawaii-Aleutian Daylight Time	UTC - 9 hours
	-36000,

	-18000,	// Heure Normale de l'Est	UTC - 5 hours
	-14400,	// Heure Avancée de l'Est	UTC - 4 hours
	-18000,

	-28800,	// Heure Normale du Pacifique	UTC - 8 hours
	-25200,	// Heure Avancée du Pacifique	UTC - 7 hours
	-28800,

	-25200,	// Heure Normale des Rocheuses	UTC - 7 hours
	-21600,	// Heure Avancée des Rocheuses	UTC - 6 hours
	-25200,

	-12600,	// Heure Normale de Terre-Neuve	UTC - 3:30 hours
	-9000,	// Heure Avancée de Terre-Neuve	UTC - 2:30 hours
	-12600,

	-32400,	// Heure Normale du Yukon	UTC - 9 hours
	-28800,	// Heure Avancée du Yukon	UTC - 8 hours
	-32400,

	-25200,	// Mountain Standard Time	UTC - 7 hours
	-21600,	// Mountain Daylight Time	UTC - 6 hours
	-25200,

	3600,	// Mitteleuropäische Zeit	UTC + 1 hour
	7200,	// Mitteleuropäische Sommerzeit	UTC + 2 hours
	3600,

	-12600,	// Newfoundland Standard Time	UTC - 3:30 hours
	-9000,	// Newfoundland Daylight Time	UTC - 2:30 hours
	-12600,

	-28800,	// Pacific Standard Time	UTC - 8 hours
	-25200,	// Pacific Daylight Time	UTC - 7 hours
	-28800,

	0,	// Western European Time	UTC
	3600,	// Western European Summer Time	UTC + 1 hour
	0,

	0,
	0,
	0
};

const char * const *datetime::getTimeZoneAbbreviations() {
	return _timezones;
}

const int32_t *datetime::getTimeZoneOffsets() {
	return _timezoneoffsets;
}

// FIXME: this is kind of lame.  There must be a better way to do this than
// looking up values in a table embedded in the class.  I guess I could look
// through every zoneinfo file (on platforms that support them), but I've tried
// that before and I get multiple hits for a given zone.  If anyone reads this
// comment and knows the answer, please let me know.
const char *datetime::lookupCombinedTimeZone(const char *zn) {

	// if the zone name is null or empty then don't bother with the more
	// expsensive stuff below
	if (charstring::isNullOrEmpty(zn)) {
		return zn;
	}

	// if the zone name is longer than 4 chars, then it's a combined zone
	if (charstring::getLength(zn)>4) {
		return zn;
	}

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return the
	// combined zone name, otherwise just return "zn"
	for (int index=0; _timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,_timezones[index]) ||
			!charstring::compare(zn,_timezones[index+1])) {
			return _timezones[index+2];
		}
	}
	return zn;
}

bool datetime::daylightZone(const char *zn) {

	// if the zone name is null or empty then don't bother with the more
	// expsensive stuff below
	if (charstring::isNullOrEmpty(zn)) {
		return zn;
	}

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return true,
	// otherwise return false
	for (int index=0; _timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,_timezones[index+1])) {
			return true;
		}
	}
	return false;
}

bool datetime::isValidDateTime(const char *string) {

	// must at least be 19 chars long (format: "00/00/0000 00:00:00")
	if (charstring::getLength(string)<19) {
		return false;
	}

	// truncate timezone
	char	*newstring=charstring::duplicate(string,19);

	// Initialize a new instance of datetime using the string, then
	// compare it to the string returned by the instance of datetime
	// (ignoring the timezone).
	// If they're the same then it was a valid date.
	datetime	dt;
	bool	result=(dt.init(newstring) &&
			!charstring::compare(newstring,dt.getString(),19));
	delete[] newstring;
	return result;
}

// FIXME: combined these with monthname/abbr above...
static const char *shortmonths[]={
	"JAN",
	"FEB",
	"MAR",
	"APR",
	"MAY",
	"JUN",
	"JUL",
	"AUG",
	"SEP",
	"OCT",
	"NOV",
	"DEC",
	NULL
};

static const char *longmonths[]={
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
	NULL
};

int16_t datetime::adjustHour(int16_t hour, const char *timestring) {
	if (hour<12 && charstring::contains(timestring,"PM")) {
		return hour+12;
	} else if (hour==12 && charstring::contains(timestring,"AM")) {
		return hour-12;
	}
	return hour;
}

int32_t datetime::fractionToMicroseconds(const char *fraction) {

	int32_t	val=charstring::convertToInteger(fraction);
	if (!val) {
		return 0;
	}

	// the fraction may be followed by an am/pm meridian
	// (eg. "123PM"), so only measure the leading digits
	const char	*c=fraction;
	while (character::isDigit(*c)) {
		c++;
	}
	size_t	len=c-fraction;

	while (len<6) {
		val=val*10;
		len++;
	}
	while (len>6) {
		val=val/10;
		len--;
	}
	return val;
}

static const char *_dayofweeknameshort[]={
	"Mon","Tue","Wed","Thu","Fri","Sat","Sun",NULL
};

static const char *_dayofweeknamelong[]={
	"Monday","Tuesday","Wednesday",
	"Thursday","Friday","Saturday","Sunday",NULL
};

static uint16_t _dayofweeknamelonglen[]={
	6,7,9,8,6,8,6
};

bool datetime::parse(const char *datetime, bool ddmm, bool yyyyddmm,
			const char *datedelimiters,
			int16_t *year, int16_t *month, int16_t *day,
			int16_t *hour, int16_t *minute, int16_t *second,
			int32_t *microsecond, bool *isnegative) {

	// FIXME: handle timezone...

	// handle unprovided parameters
	int16_t	junkint16;
	int32_t	junkint32;
	bool	junkbool;
	if (!year) {
		year=&junkint16;
	}
	if (!month) {
		month=&junkint16;
	}
	if (!day) {
		day=&junkint16;
	}
	if (!hour) {
		hour=&junkint16;
	}
	if (!minute) {
		minute=&junkint16;
	}
	if (!second) {
		second=&junkint16;
	}
	if (!microsecond) {
		microsecond=&junkint32;
	}
	if (!isnegative) {
		isnegative=&junkbool;
	}

	// initialize date/time parts
	*year=-1;
	*month=-1;
	*day=-1;
	*hour=-1;
	*minute=-1;
	*second=-1;
	*microsecond=-1;
	*isnegative=false;

	// bail if datetime is NULL
	if (!datetime) {
		return true;
	}

	bool	supportslashdelimiteddate=(!datedelimiters ||
				charstring::contains(datedelimiters,'/'));
	bool	supportdashdelimiteddate=(!datedelimiters ||
				charstring::contains(datedelimiters,'-'));
	bool	supportdotdelimiteddate=(!datedelimiters ||
				charstring::contains(datedelimiters,'.'));
	bool	supportcolondelimiteddate=(!datedelimiters ||
				charstring::contains(datedelimiters,':'));

	// dates can be formatted very differently
	// the number of parts could be:
	// one (eg. date or time only),
	// two (eg. date and time),
	// three (eg. Feb 02 2012),
	// three (eg. 2/2/2012 1:03:04 AM),
	// four (eg. Feb 02 2012 01:03:04:000AM),
	// five (eg. Fri, Feb 02 2012 01:03:04:000AM)

	// in the case of a five-part date (starting with the day-of-week),
	// just skip the day-of-week and handle as 4-part
	bool	found=false;
	for (const char **dow=_dayofweeknameshort; *dow; dow++) {
		if (!charstring::compare(datetime,*dow,3) &&
					*(datetime+3)==',') {
			datetime+=4;
			while (*datetime &&
				character::isWhitespace(*datetime)) {
				datetime++;
			}
			found=true;
			break;
		}
	}
	if (!found) {
		uint16_t	i=0;
		for (const char **dow=_dayofweeknamelong; *dow; dow++) {
			if (!charstring::compare(datetime,*dow,
						_dayofweeknamelonglen[i]) &&
				*(datetime+_dayofweeknamelonglen[i])==',') {
				datetime+=_dayofweeknamelonglen[i]+1;
				while (*datetime &&
					character::isWhitespace(*datetime)) {
					datetime++;
				}
				break;
			}
			i++;
		}
	}

	// split on a space
	char		**parts;
	uint64_t	partcount;
	charstring::split(datetime," ",1,true,&parts,&partcount);
	if (!partcount || partcount>4) {
		for (uint64_t i=0; i<partcount; i++) {
			delete[] parts[i];
		}
		delete[] parts;
		return false;
	}

	// initialize the return value;
	bool	retval=true;

	// handle 3-part dates with AM/PM
	// basically handle them as 2-part, but with a pm-flag
	bool	pmset=false;
	if (partcount==3) {
		if (!charstring::compare(parts[2],"AM")) {
			partcount=2;
		} else if (!charstring::compare(parts[2],"PM")) {
			partcount=2;
			pmset=true;
		}
	}

	// 4-part dates are very different than the rest
	// sybase and ms sql server return these
	if (partcount==3 || partcount==4) {

		// part 1 could be the day or month
		uint16_t	monthindex=0;
		uint16_t	dayindex=1;
		if (charstring::isNumber(parts[0])) {
			dayindex=0;
			monthindex=1;
		}

		// get the month
		*month=0;
		for (int i=0; shortmonths[i]; i++) {
			if (!charstring::compareIgnoringCase(
					parts[monthindex],shortmonths[i]) ||
				!charstring::compareIgnoringCase(
					parts[monthindex],longmonths[i])) {
				*month=i+1;
			}
		}
		if (!*month) {
			retval=false;
		}

		// get the day
		*day=charstring::convertToInteger(parts[dayindex]);

		// part 3 is the year
		*year=charstring::convertToInteger(parts[2]);

		// part 4 could be the time, we'll split it below...
	}

	// parse the parts
	for (uint64_t i=(partcount>2)?3:0; i<partcount && retval; i++) {

		if (charstring::contains(parts[i],':')) {

			// the section with :'s is probably the time...

			// split on :
			char		**timeparts;
			uint64_t	timepartcount;
			charstring::split(parts[i],":",1,true,
						&timeparts,&timepartcount);
	
			// there could be:
			// 2 parts, all numbers,
			//     (02:03)
			// 2 parts with AM/PM in part 2,
			//     (02:03AM)
			// 3 parts, all numbers,
			//     (02:03:04)
			// 3 parts with a decimal fraction and AM/PM in part 3,
			//     (02:03:04.123AM)
			// 3 parts with no decimal fraction and AM/PM in part 3,
			//     (02:03:04AM)
			// 3 parts with a decimal fraction and no AM/PM,
			//     (14:03:04.123)
			// 4 parts with a fractional part 4 and AM/PM,
			//     (02:03:04:123AM)
			// 4 parts with a fractional part 4 and no AM/PM
			//     (14:03:04:123)
			if (timepartcount==2 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1])) {

				*hour=charstring::convertToInteger(timeparts[0]);
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=0;
				*microsecond=0;

			} else if (timepartcount==2 &&
				charstring::isNumber(timeparts[0]) &&
				((charstring::contains(timeparts[1],"AM") &&
				!*(charstring::findFirst(timeparts[1],"AM")+2))
				||
				(charstring::contains(timeparts[1],"PM") &&
				!*(charstring::findFirst(timeparts[1],"PM")+2)))
				) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=0;
				*microsecond=0;
				*hour=adjustHour(*hour,timeparts[1]);

			} else if (timepartcount==3 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				charstring::isNumber(timeparts[2]) &&
				!charstring::contains(timeparts[2],'.')) {

				// well, if the first or last part is 4 digit
				// then it's a date (firebird uses
				// colon-delimited dates) otherwise it's a time
				if (supportcolondelimiteddate &&
					charstring::getLength(timeparts[0])==4) {
					*year=charstring::convertToInteger(
								timeparts[0]);
					if (ddmm) {
						*day=charstring::convertToInteger(
								timeparts[1]);
						*month=charstring::convertToInteger(
								timeparts[2]);
					} else {
						*month=charstring::convertToInteger(
								timeparts[1]);
						*day=charstring::convertToInteger(
								timeparts[2]);
					}
				} else if (supportcolondelimiteddate &&
					charstring::getLength(timeparts[2])==4) {
					if (ddmm) {
						*day=charstring::convertToInteger(
								timeparts[0]);
						*month=charstring::convertToInteger(
								timeparts[1]);
					} else {
						*day=charstring::convertToInteger(
								timeparts[0]);
						*month=charstring::convertToInteger(
								timeparts[1]);
					}
					*year=charstring::convertToInteger(
								timeparts[2]);
				} else {
					if (timeparts[0][0]=='-') {
						*isnegative=true;
						*hour=charstring::convertToInteger(
								timeparts[0]+1);
					} else {
						*hour=charstring::convertToInteger(
								timeparts[0]);
					}
					*minute=charstring::convertToInteger(
								timeparts[1]);
					*second=charstring::convertToInteger(
								timeparts[2]);
					*microsecond=0;
				}

			} else if (timepartcount==3 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				charstring::contains(timeparts[2],'.') &&
				((charstring::contains(timeparts[2],"AM") &&
				!*(charstring::findFirst(timeparts[2],"AM")+2))
				||
				(charstring::contains(timeparts[2],"PM") &&
				!*(charstring::findFirst(timeparts[2],"PM")+2)))
				) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=charstring::convertToInteger(timeparts[2]);
				const char	*dot=
					charstring::findFirst(timeparts[2],'.');
				*microsecond=fractionToMicroseconds(dot+1);
				*hour=adjustHour(*hour,timeparts[2]);

			} else if (timepartcount==3 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				((charstring::contains(timeparts[2],"AM") &&
				!*(charstring::findFirst(timeparts[2],"AM")+2))
				||
				(charstring::contains(timeparts[2],"PM") &&
				!*(charstring::findFirst(timeparts[2],"PM")+2)))
				) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=charstring::convertToInteger(timeparts[2]);
				*microsecond=0;
				*hour=adjustHour(*hour,timeparts[2]);

			} else if (timepartcount==3 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				charstring::contains(timeparts[2],'.')) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=charstring::convertToInteger(timeparts[2]);
				const char	*dot=
					charstring::findFirst(timeparts[2],'.');
				*microsecond=fractionToMicroseconds(dot+1);

			} else if (timepartcount==4 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				charstring::isNumber(timeparts[2]) &&
				((charstring::contains(timeparts[3],"AM") &&
				!*(charstring::findFirst(timeparts[3],"AM")+2))
				||
				(charstring::contains(timeparts[3],"PM") &&
				!*(charstring::findFirst(timeparts[3],"PM")+2)))
				) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=charstring::convertToInteger(timeparts[2]);
				*microsecond=
					fractionToMicroseconds(timeparts[3]);
				*hour=adjustHour(*hour,timeparts[3]);

			} else if (timepartcount==4 &&
				charstring::isNumber(timeparts[0]) &&
				charstring::isNumber(timeparts[1]) &&
				charstring::isNumber(timeparts[2]) &&
				charstring::isNumber(timeparts[3])) {

				if (timeparts[0][0]=='-') {
					*isnegative=true;
					*hour=charstring::convertToInteger(
							timeparts[0]+1);
				} else {
					*hour=charstring::convertToInteger(
							timeparts[0]);
				}
				*minute=charstring::convertToInteger(timeparts[1]);
				*second=charstring::convertToInteger(timeparts[2]);
				*microsecond=
					fractionToMicroseconds(timeparts[3]);

			} else {
				retval=false;
			}

			// handle manually-set am/pm
			if (*hour<12 && pmset) {
				*hour+=12;
			}

			// clean up
			for (uint64_t j=0; j<timepartcount; j++) {
				delete[] timeparts[j];
			}
			delete[] timeparts;

		} else if (supportslashdelimiteddate &&
				charstring::contains(parts[i],'/')) {

			// the section with /'s is the date...

			// split on /
			char		**dateparts;
			uint64_t	datepartcount;
			charstring::split(parts[i],"/",1,true,
						&dateparts,&datepartcount);

			// assume month/day, but in some countries
			// they do it the other way around
			// I'm not sure how to decide...

			// there must be three parts, all numbers
			if (datepartcount==3 &&
				charstring::isNumber(dateparts[0]) &&
				charstring::isNumber(dateparts[1]) &&
				charstring::isNumber(dateparts[2])) {

				// it could be yyyy/xx/xx or xx/xx/yyyy
				if (charstring::getLength(dateparts[0])==4) {
					*year=charstring::convertToInteger(
								dateparts[0]);
					if (ddmm) {
						*day=charstring::convertToInteger(
								dateparts[1]);
						*month=charstring::convertToInteger(
								dateparts[2]);
					} else {
						*month=charstring::convertToInteger(
								dateparts[1]);
						*day=charstring::convertToInteger(
								dateparts[2]);
					}
				} else {
					if (ddmm) {
						*day=charstring::convertToInteger(
								dateparts[0]);
						*month=charstring::convertToInteger(
								dateparts[1]);
					} else {
						*month=charstring::convertToInteger(
								dateparts[0]);
						*day=charstring::convertToInteger(
								dateparts[1]);
					}
					*year=charstring::convertToInteger(
								dateparts[2]);
				}
			} else {
				retval=false;
			}

			// clean up
			for (uint64_t j=0; j<datepartcount; j++) {
				delete[] dateparts[j];
			}
			delete[] dateparts;

		} else if (supportdotdelimiteddate &&
				charstring::contains(parts[i],'.')) {

			// the section with .'s is the date...

			// split on .
			char		**dateparts;
			uint64_t	datepartcount;
			charstring::split(parts[i],".",1,true,
						&dateparts,&datepartcount);

			// there must be three parts, 0 and 2 must be numbers
			if (datepartcount==3 &&
				charstring::isNumber(dateparts[0]) &&
				charstring::isNumber(dateparts[2])) {

				// some dates have a non-numeric month in part 2
				if (!charstring::isNumber(dateparts[1])) {

					*day=charstring::convertToInteger(
								dateparts[0]);
					for (int j=0; shortmonths[j]; j++) {
						if (!charstring::
							compareIgnoringCase(
								dateparts[1],
								shortmonths[j]) 
							||
							!charstring::
							compareIgnoringCase(
								dateparts[1],
								longmonths[j]))
						{
							*month=j+1;
						}
					}
					*year=charstring::convertToInteger(
								dateparts[2]);
				} else {

					// it could be yyyy.xx.xx or xx.xx.yyyy
					if (charstring::getLength(
							dateparts[0])==4) {
						*year=charstring::convertToInteger(
								dateparts[0]);
						if (ddmm) {
							*day=
							charstring::convertToInteger(
								dateparts[1]);
							*month=
							charstring::convertToInteger(
								dateparts[2]);
						} else {
							*month=
							charstring::convertToInteger(
								dateparts[1]);
							*day=
							charstring::convertToInteger(
								dateparts[2]);
						}
					} else {
						if (ddmm) {
							*day=
							charstring::convertToInteger(
								dateparts[0]);
							*month=
							charstring::convertToInteger(
								dateparts[1]);
						} else {
							*month=
							charstring::convertToInteger(
								dateparts[0]);
							*day=
							charstring::convertToInteger(
								dateparts[1]);
						}
						*year=charstring::convertToInteger(
								dateparts[2]);
					}
				}
			} else {
				retval=false;
			}

			// clean up
			for (uint64_t j=0; j<datepartcount; j++) {
				delete[] dateparts[j];
			}
			delete[] dateparts;

		} else if (supportdashdelimiteddate &&
				charstring::contains(parts[i],'-')) {

			// the section with -'s is the date...
			// (a time can also start with a - (indicating that
			// it's a negative interval) but that should should
			// have been caught above because it also contain :'s)

			// split on -
			char		**dateparts;
			uint64_t	datepartcount;
			charstring::split(parts[i],"-",1,true,
						&dateparts,&datepartcount);

			// there must be three parts, 0 and 2 must be numbers
			if (datepartcount==3 &&
				charstring::isNumber(dateparts[0]) &&
				charstring::isNumber(dateparts[2])) {

				// some dates have a non-numeric month in part 2
				if (!charstring::isNumber(dateparts[1])) {

					*day=charstring::convertToInteger(
								dateparts[0]);
					for (int j=0; shortmonths[j]; j++) {
						if (!charstring::
							compareIgnoringCase(
								dateparts[1],
								shortmonths[j]) 
							||
							!charstring::
							compareIgnoringCase(
								dateparts[1],
								longmonths[j]))
						{
							*month=j+1;
						}
					}
					*year=charstring::convertToInteger(
								dateparts[2]);
				} else {

					// it could be yyyy-xx-xx or xx-xx-yyyy
					if (charstring::getLength(
							dateparts[0])==4) {
						*year=charstring::convertToInteger(
								dateparts[0]);
						if (yyyyddmm) {
							*day=
							charstring::convertToInteger(
								dateparts[1]);
							*month=
							charstring::convertToInteger(
								dateparts[2]);
						} else {
							*month=
							charstring::convertToInteger(
								dateparts[1]);
							*day=
							charstring::convertToInteger(
								dateparts[2]);
						}
					} else {
						if (ddmm) {
							*day=
							charstring::convertToInteger(
								dateparts[0]);
							*month=
							charstring::convertToInteger(
								dateparts[1]);
						} else {
							*month=
							charstring::convertToInteger(
								dateparts[0]);
							*day=
							charstring::convertToInteger(
								dateparts[1]);
						}
						*year=charstring::convertToInteger(
								dateparts[2]);
					}
				}
			} else {
				retval=false;
			}

			// clean up
			for (uint64_t j=0; j<datepartcount; j++) {
				delete[] dateparts[j];
			}
			delete[] dateparts;
		} else {
			retval=false;
		}
	}

	// clean up
	for (uint64_t j=0; j<partcount; j++) {
		delete[] parts[j];
	}
	delete[] parts;

	return retval;
}

bool datetime::parse(const char *datetime, bool ddmm, bool yyyyddmm,
			const char *datedelimiters, bool adjustshortyear,
			int16_t *year, int16_t *month, int16_t *day,
			int16_t *hour, int16_t *minute, int16_t *second,
			int32_t *microsecond, bool *isnegative) {
	if (!parse(datetime,ddmm,yyyyddmm,datedelimiters,
			year,month,day,hour,minute,second,
			microsecond,isnegative)) {
		return false;
	}
	if (adjustshortyear && *year<100) {
		if (*year>50) {
			*year+=1900;
		} else {
			*year+=2000;
		}
	}
	return true;
}

char *datetime::formatAs(const char *format,
			int16_t year, int16_t month, int16_t day,
			int16_t hour, int16_t minute, int16_t second,
			int32_t microsecond, bool isnegative) {

	// if no format was passed in
	if (!format) {
		return NULL;
	}

	// normalize times
	day=(day>0)?day:1;
	month=(month>0)?month:1;
	year=(year>0)?year:1;
	hour=(hour>0)?hour:0;
	minute=(minute>0)?minute:0;
	second=(second>0)?second:0;
	microsecond=(microsecond>0)?microsecond:0;

	// output buffer
	stringbuffer	output;

	// work buffer
	char		buf[7];

	// run through the format string
	const char	*ptr=format;
	while (*ptr) {

		if (!charstring::compare(ptr,"DD",2)) {
			charstring::printf(buf,7,"%02d",day);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"D",1)) {
			charstring::printf(buf,7,"%d",day);
			output.append(buf);
			ptr=ptr+1;
		} else if (!charstring::compare(ptr,"MM",2)) {
			charstring::printf(buf,7,"%02d",month);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"MON",3)) {
			output.append(shortmonths[month-1]);
			ptr=ptr+3;
		} else if (!charstring::compare(ptr,"Month",5)) {
			output.append(longmonths[month-1]);
			ptr=ptr+5;
		} else if (!charstring::compare(ptr,"Mon",3)) {
			output.append(_monthabbr[month-1]);
			ptr=ptr+3;
		} else if (!charstring::compare(ptr,"YYYY",4)) {
			charstring::printf(buf,7,"%04d",year);
			output.append(buf);
			ptr=ptr+4;
		} else if (!charstring::compare(ptr,"YY",2)) {
			charstring::printf(buf,7,"%04d",year);
			output.append(buf+2);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"HH24",4)) {
			charstring::printf(buf,7,"%02d",hour);
			output.append(buf);
			ptr=ptr+4;
		} else if (!charstring::compare(ptr,"HH",2)) {
			charstring::printf(buf,7,"%s%02d",
						(isnegative)?"-":"",
						(hour<13)?hour:hour-12);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"MI",2)) {
			charstring::printf(buf,7,"%02d",minute);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"SS",2)) {
			charstring::printf(buf,7,"%02d",second);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"FFFFFF",6)) {
			charstring::printf(buf,7,"%06d",microsecond);
			output.append(buf);
			ptr=ptr+6;
		} else if (!charstring::compare(ptr,"FFFFF",5)) {
			charstring::printf(buf,7,"%05d",microsecond/10);
			output.append(buf);
			ptr=ptr+5;
		} else if (!charstring::compare(ptr,"FFFF",4)) {
			charstring::printf(buf,7,"%04d",microsecond/100);
			output.append(buf);
			ptr=ptr+4;
		} else if (!charstring::compare(ptr,"FFF",3)) {
			charstring::printf(buf,7,"%03d",microsecond/1000);
			output.append(buf);
			ptr=ptr+3;
		} else if (!charstring::compare(ptr,"FF",2)) {
			charstring::printf(buf,7,"%02d",microsecond/10000);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"F",1)) {
			charstring::printf(buf,7,"%01d",microsecond/100000);
			output.append(buf);
			ptr=ptr+2;
		} else if (!charstring::compare(ptr,"AM",2)) {
			output.append((hour<12)?"AM":"PM");
			ptr=ptr+2;
		} else {
			output.append(*ptr);
			ptr=ptr+1;
		}
	}

	return output.detachString();
}
