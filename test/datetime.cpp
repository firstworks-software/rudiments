// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/datetime.h>
#include <rudiments/environment.h>
#include <rudiments/stdio.h>
#include <rudiments/sys.h>
#include <rudiments/private/config.h>
#include "test.cpp"

const char	*nondstdatestring="02/08/2016 21:54:30 EST";
const char	*nondstdatestringwithzerousec="02/08/2016 21:54:30:000 EST";
time_t		nondstsecsinceepoch=1454986470;

const char	*datestring="04/08/2016 21:54:30 EDT";
const char	*estdatestring="04/08/2016 21:54:30 EDT";
const char	*cstdatestring="04/08/2016 20:54:30 CDT";
const char	*mstdatestring="04/08/2016 19:54:30 MDT";
const char	*pstdatestring="04/08/2016 18:54:30 PDT";
const char	*datestringwithusec="04/08/2016 21:54:30:500 EDT";
const char	*datestringwithzerousec="04/08/2016 21:54:30:000 EDT";
time_t		secsinceepoch=1460166870;

int main(int argc, const char **argv) {

	// set timezone so epoch to string will work correctly
	environment::setValue("TZ","EST5EDT");

	header("datetime");

	datetime	dt;

	// init from string
	stdoutput.printf("non-dst date from string:\n");
	dt.initialize(nondstdatestring);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==0);
	test("month",dt.getMonth()==2);
	test("month name",
		!charstring::compare(dt.getMonthName(),"February"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Feb"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==2);
	test("day of year",dt.getDayOfYear()==39);
	test("week of year",dt.getWeekOfYear()==6);
	test("year",dt.getYear()==2016);
	test("daylight savings time",!dt.isDaylightSavingsTime());

	// some platforms (haiku) convert EST to EST5EDT,
	// so we have to allow that
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EST") ||
		!charstring::compare(dt.getTimeZoneString(),"EST5EDT"));

	test("offset from GMT",dt.getTimeZoneOffset()==-18000);
	test("seconds since 1970",dt.getEpoch()==nondstsecsinceepoch);

	// some platforms (haiku) convert EST to EST5EDT, so we'll truncate
	// the 5EDT part during these comparisons
	test("date string",
		!charstring::compare(dt.getString(),nondstdatestring,
					charstring::length(nondstdatestring)));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),
			nondstdatestringwithzerousec,
			charstring::length(nondstdatestringwithzerousec)));

	stdoutput.printf("\n");


	// init from string
	stdoutput.printf("date from string:\n");
	dt.initialize(datestringwithusec);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==500);
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("week of year",dt.getWeekOfYear()==14);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),datestringwithusec));
	stdoutput.printf("\n");


	// init from epoch
	stdoutput.printf("date from epoch:\n");
	dt.initialize(secsinceepoch);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",!dt.getMicroseconds());
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("week of year",dt.getWeekOfYear()==14);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),
					datestringwithzerousec));
	stdoutput.printf("\n");


	// init from epoch+usec
	stdoutput.printf("date from epoch+usec:\n");
	dt.initialize(secsinceepoch,500);
	test("hour",dt.getHour()==21);
	test("minutes",dt.getMinutes()==54);
	test("seconds",dt.getSeconds()==30);
	test("useconds",dt.getMicroseconds()==500);
	test("month",dt.getMonth()==4);
	test("month name",
		!charstring::compare(dt.getMonthName(),"April"));
	test("month abbr",
		!charstring::compare(dt.getMonthAbbreviation(),"Apr"));
	test("day of month",dt.getDayOfMonth()==8);
	test("day of week",dt.getDayOfWeek()==6);
	test("day of year",dt.getDayOfYear()==99);
	test("week of year",dt.getWeekOfYear()==14);
	test("year",dt.getYear()==2016);
	test("daylight savings time",dt.isDaylightSavingsTime());
	test("time zone",
		!charstring::compare(dt.getTimeZoneString(),"EDT"));
	test("offset from GMT",dt.getTimeZoneOffset()==-14400);
	test("seconds since 1970",dt.getEpoch()==secsinceepoch);
	test("date string",
		!charstring::compare(dt.getString(),datestring));
	test("date string (with usec)",
		!charstring::compare(dt.getString(true),datestringwithusec));
	stdoutput.printf("\n");


	// add/subtract time
	stdoutput.printf("add/subtract time:\n");
	dt.addMicroseconds(1);
	test("addSeconds(1) hour",dt.getHour()==21);
	test("addSeconds(1) minutes",dt.getMinutes()==54);
	test("addSeconds(1) seconds",dt.getSeconds()==30);
	test("addSeconds(1) useconds",dt.getMicroseconds()==501);
	test("addSeconds(1) month",dt.getMonth()==4);
	test("addSeconds(1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(1) year",dt.getYear()==2016);
	dt.addMicroseconds(-1);
	test("addSeconds(-1) hour",dt.getHour()==21);
	test("addSeconds(-1) minutes",dt.getMinutes()==54);
	test("addSeconds(-1) seconds",dt.getSeconds()==30);
	test("addSeconds(-1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(-1) month",dt.getMonth()==4);
	test("addSeconds(-1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(-1) year",dt.getYear()==2016);
	dt.addSeconds(1);
	test("addSeconds(1) hour",dt.getHour()==21);
	test("addSeconds(1) minutes",dt.getMinutes()==54);
	test("addSeconds(1) seconds",dt.getSeconds()==31);
	test("addSeconds(1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(1) month",dt.getMonth()==4);
	test("addSeconds(1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(1) year",dt.getYear()==2016);
	dt.addSeconds(-1);
	test("addSeconds(-1) hour",dt.getHour()==21);
	test("addSeconds(-1) minutes",dt.getMinutes()==54);
	test("addSeconds(-1) seconds",dt.getSeconds()==30);
	test("addSeconds(-1) useconds",dt.getMicroseconds()==500);
	test("addSeconds(-1) month",dt.getMonth()==4);
	test("addSeconds(-1) day of month",dt.getDayOfMonth()==8);
	test("addSeconds(-1) year",dt.getYear()==2016);
	dt.addMinutes(1);
	test("addMinutes(1) hour",dt.getHour()==21);
	test("addMinutes(1) minutes",dt.getMinutes()==55);
	test("addMinutes(1) seconds",dt.getSeconds()==30);
	test("addMinutes(1) useconds",dt.getMicroseconds()==500);
	test("addMinutes(1) month",dt.getMonth()==4);
	test("addMinutes(1) day of month",dt.getDayOfMonth()==8);
	test("addMinutes(1) year",dt.getYear()==2016);
	dt.addMinutes(-1);
	test("addMinutes(-1) hour",dt.getHour()==21);
	test("addMinutes(-1) minutes",dt.getMinutes()==54);
	test("addMinutes(-1) seconds",dt.getSeconds()==30);
	test("addMinutes(-1) useconds",dt.getMicroseconds()==500);
	test("addMinutes(-1) month",dt.getMonth()==4);
	test("addMinutes(-1) day of month",dt.getDayOfMonth()==8);
	test("addMinutes(-1) year",dt.getYear()==2016);
	dt.addHours(1);
	test("addHours(1) hour",dt.getHour()==22);
	test("addHours(1) minutes",dt.getMinutes()==54);
	test("addHours(1) seconds",dt.getSeconds()==30);
	test("addHours(1) useconds",dt.getMicroseconds()==500);
	test("addHours(1) month",dt.getMonth()==4);
	test("addHours(1) day of month",dt.getDayOfMonth()==8);
	test("addHours(1) year",dt.getYear()==2016);
	dt.addHours(-1);
	test("addHours(-1) hour",dt.getHour()==21);
	test("addHours(-1) minutes",dt.getMinutes()==54);
	test("addHours(-1) seconds",dt.getSeconds()==30);
	test("addHours(-1) useconds",dt.getMicroseconds()==500);
	test("addHours(-1) month",dt.getMonth()==4);
	test("addHours(-1) day of month",dt.getDayOfMonth()==8);
	test("addHours(-1) year",dt.getYear()==2016);
	dt.addDays(1);
	test("addDays(1) hour",dt.getHour()==21);
	test("addDays(1) minutes",dt.getMinutes()==54);
	test("addDays(1) seconds",dt.getSeconds()==30);
	test("addDays(1) useconds",dt.getMicroseconds()==500);
	test("addDays(1) month",dt.getMonth()==4);
	test("addDays(1) day of month",dt.getDayOfMonth()==9);
	test("addDays(1) year",dt.getYear()==2016);
	dt.addDays(-1);
	test("addDays(-1) hour",dt.getHour()==21);
	test("addDays(-1) minutes",dt.getMinutes()==54);
	test("addDays(-1) seconds",dt.getSeconds()==30);
	test("addDays(-1) useconds",dt.getMicroseconds()==500);
	test("addDays(-1) month",dt.getMonth()==4);
	test("addDays(-1) day of month",dt.getDayOfMonth()==8);
	test("addDays(-1) year",dt.getYear()==2016);
	dt.addMonths(1);
	test("addMonths(1) hour",dt.getHour()==21);
	test("addMonths(1) minutes",dt.getMinutes()==54);
	test("addMonths(1) seconds",dt.getSeconds()==30);
	test("addMonths(1) useconds",dt.getMicroseconds()==500);
	test("addMonths(1) month",dt.getMonth()==5);
	//test("addMonths(1) day of month",dt.getDayOfMonth()==9);
	test("addMonths(1) year",dt.getYear()==2016);
	dt.addMonths(-1);
	test("addMonths(-1) hour",dt.getHour()==21);
	test("addMonths(-1) minutes",dt.getMinutes()==54);
	test("addMonths(-1) seconds",dt.getSeconds()==30);
	test("addMonths(-1) useconds",dt.getMicroseconds()==500);
	test("addMonths(-1) month",dt.getMonth()==4);
	test("addMonths(-1) day of month",dt.getDayOfMonth()==8);
	test("addMonths(-1) year",dt.getYear()==2016);
	dt.addYears(1);
	test("addYears(1) hour",dt.getHour()==21);
	test("addYears(1) minutes",dt.getMinutes()==54);
	test("addYears(1) seconds",dt.getSeconds()==30);
	test("addYears(1) useconds",dt.getMicroseconds()==500);
	test("addYears(1) month",dt.getMonth()==4);
	//test("addYears(1) day of month",dt.getDayOfMonth()==9);
	test("addYears(1) year",dt.getYear()==2017);
	dt.addYears(-1);
	test("addYears(-1) hour",dt.getHour()==21);
	test("addYears(-1) minutes",dt.getMinutes()==54);
	test("addYears(-1) seconds",dt.getSeconds()==30);
	test("addYears(-1) useconds",dt.getMicroseconds()==500);
	test("addYears(-1) month",dt.getMonth()==4);
	test("addYears(-1) day of month",dt.getDayOfMonth()==8);
	test("addYears(-1) year",dt.getYear()==2016);
	stdoutput.printf("\n");


	// conversions
	stdoutput.printf("conversions:\n");
	char	*string=datetime::getString(secsinceepoch);
	test("string from epoch",!charstring::compare(string,datestring));
	delete[] string;
	time_t	epoch=datetime::getEpoch(datestring);
	test("epoch from string",epoch==secsinceepoch);
	string=datetime::getString(secsinceepoch,500);
	test("string+usec from epoch",
			!charstring::compare(string,datestringwithusec));
	delete[] string;
	epoch=datetime::getEpoch(datestringwithusec);
	test("epoch from string+usec",epoch==secsinceepoch);
	stdoutput.printf("\n");


	// get time
	stdoutput.printf("dates from various sources:\n");
	test("system clock",dt.getSystemDateAndTime());
	// getHardwareDateAndTime/getAdjustedHardwareDateAndTime
	// aren't reliable on most systems
	stdoutput.printf("\n");


	// time zones
	stdoutput.printf("time zones:\n");
	const char * const *tz=datetime::getTimeZoneAbbreviations();
	uint32_t	i=0;
	test("ACST",!charstring::compare(tz[i++],"ACST"));
	test("ACDT",!charstring::compare(tz[i++],"ACDT"));
	test("ACST-10:30ACDT",!charstring::compare(tz[i++],"ACST-10:30ACDT"));
	test("AST",!charstring::compare(tz[i++],"AST"));
	test("ADT",!charstring::compare(tz[i++],"ADT"));
	test("AST4ADT",!charstring::compare(tz[i++],"AST4ADT"));
	test("AEST",!charstring::compare(tz[i++],"AEST"));
	test("AEDT",!charstring::compare(tz[i++],"AEDT"));
	test("AEST10AEDT",!charstring::compare(tz[i++],"AEST10AEDT"));
	test("AKST",!charstring::compare(tz[i++],"AKST"));
	test("AKDT",!charstring::compare(tz[i++],"AKDT"));
	test("AKST-9ADKT",!charstring::compare(tz[i++],"AKST-9AKDT"));
	test("CST",!charstring::compare(tz[i++],"CST"));
	test("CDT",!charstring::compare(tz[i++],"CDT"));
	test("CST6CDT",!charstring::compare(tz[i++],"CST6CDT"));
	test("CET",!charstring::compare(tz[i++],"CET"));
	test("CEST",!charstring::compare(tz[i++],"CEST"));
	test("CET-1CST",!charstring::compare(tz[i++],"CET-1CST"));
	test("EST",!charstring::compare(tz[i++],"EST"));
	test("EDT",!charstring::compare(tz[i++],"EDT"));
	test("EST5EDT",!charstring::compare(tz[i++],"EST5EDT"));
	test("EET",!charstring::compare(tz[i++],"EET"));
	test("EEST",!charstring::compare(tz[i++],"EEST"));
	test("EET-2EEST",!charstring::compare(tz[i++],"EET-2EEST"));
	test("GMT",!charstring::compare(tz[i++],"GMT"));
	test("BST",!charstring::compare(tz[i++],"BST"));
	test("GMT0BST",!charstring::compare(tz[i++],"GMT0BST"));
	test("HNA",!charstring::compare(tz[i++],"HNA"));
	test("HAA",!charstring::compare(tz[i++],"HAA"));
	test("HNA4HAA",!charstring::compare(tz[i++],"HNA4HAA"));
	test("HNC",!charstring::compare(tz[i++],"HNC"));
	test("HAC",!charstring::compare(tz[i++],"HAC"));
	test("HNC6HAC",!charstring::compare(tz[i++],"HNC6HAC"));
	test("HAST",!charstring::compare(tz[i++],"HAST"));
	test("HADT",!charstring::compare(tz[i++],"HADT"));
	test("HAST10HADT",!charstring::compare(tz[i++],"HAST10HADT"));
	test("HNE",!charstring::compare(tz[i++],"HNE"));
	test("HAE",!charstring::compare(tz[i++],"HAE"));
	test("HNE5HAE",!charstring::compare(tz[i++],"HNE5HAE"));
	test("HNP",!charstring::compare(tz[i++],"HNP"));
	test("HAP",!charstring::compare(tz[i++],"HAP"));
	test("HNP8HAP",!charstring::compare(tz[i++],"HNP8HAP"));
	test("HNR",!charstring::compare(tz[i++],"HNR"));
	test("HAR",!charstring::compare(tz[i++],"HAR"));
	test("HNR7HAR",!charstring::compare(tz[i++],"HNR7HAR"));
	test("HNT",!charstring::compare(tz[i++],"HNT"));
	test("HAT",!charstring::compare(tz[i++],"HAT"));
	test("HNT3:30HAT",!charstring::compare(tz[i++],"HNT3:30HAT"));
	test("HNY",!charstring::compare(tz[i++],"HNY"));
	test("HAY",!charstring::compare(tz[i++],"HAY"));
	test("HNY9HAY",!charstring::compare(tz[i++],"HNY9HAY"));
	test("MST",!charstring::compare(tz[i++],"MST"));
	test("MDT",!charstring::compare(tz[i++],"MDT"));
	test("MST7MDT",!charstring::compare(tz[i++],"MST7MDT"));
	test("MEZ",!charstring::compare(tz[i++],"MEZ"));
	test("MESZ",!charstring::compare(tz[i++],"MESZ"));
	test("MEZ-1MESZ",!charstring::compare(tz[i++],"MEZ-1MESZ"));
	test("NST",!charstring::compare(tz[i++],"NST"));
	test("NDT",!charstring::compare(tz[i++],"NDT"));
	test("NST3:30NDT",!charstring::compare(tz[i++],"NST3:30NDT"));
	test("PST",!charstring::compare(tz[i++],"PST"));
	test("PDT",!charstring::compare(tz[i++],"PDT"));
	test("PST8PDT",!charstring::compare(tz[i++],"PST8PDT"));
	test("WET",!charstring::compare(tz[i++],"WET"));
	test("WEST",!charstring::compare(tz[i++],"WEST"));
	test("WET-1WEST",!charstring::compare(tz[i++],"WET-1WEST"));
	stdoutput.printf("\n");

	stdoutput.printf("time zone offsets:\n");
	const int32_t *tzo=datetime::getTimeZoneOffsets();
	i=0;
	test("34200",tzo[i++]==34200);
	test("37800",tzo[i++]==37800);
	test("34200",tzo[i++]==34200);
	test("-14400",tzo[i++]==-14400);
	test("-10800",tzo[i++]==-10800);
	test("-14400",tzo[i++]==-14400);
	test("36000",tzo[i++]==36000);
	test("36000",tzo[i++]==39600);
	test("39000",tzo[i++]==36000);
	test("-32400",tzo[i++]==-32400);
	test("-28800",tzo[i++]==-28800);
	test("-32400",tzo[i++]==-32400);
	test("-21600",tzo[i++]==-21600);
	test("-18000",tzo[i++]==-18000);
	test("-21600",tzo[i++]==-21600);
	test("3600",tzo[i++]==3600);
	test("7200",tzo[i++]==7200);
	test("3600",tzo[i++]==3600);
	test("-18000",tzo[i++]==-18000);
	test("-14400",tzo[i++]==-14400);
	test("-18000",tzo[i++]==-18000);
	test("7200",tzo[i++]==7200);
	test("10800",tzo[i++]==10800);
	test("7200",tzo[i++]==7200);
	test("0",tzo[i++]==0);
	test("3600",tzo[i++]==3600);
	test("0",tzo[i++]==0);
	test("-14400",tzo[i++]==-14400);
	test("-10800",tzo[i++]==-10800);
	test("-14400",tzo[i++]==-14400);
	test("-21600",tzo[i++]==-21600);
	test("-18000",tzo[i++]==-18000);
	test("-21600",tzo[i++]==-21600);
	test("-36000",tzo[i++]==-36000);
	test("-32400",tzo[i++]==-32400);
	test("-36000",tzo[i++]==-36000);
	test("-18000",tzo[i++]==-18000);
	test("-14400",tzo[i++]==-14400);
	test("-18000",tzo[i++]==-18000);
	test("-28800",tzo[i++]==-28800);
	test("-25200",tzo[i++]==-25200);
	test("-28800",tzo[i++]==-28800);
	test("-25200",tzo[i++]==-25200);
	test("-21600",tzo[i++]==-21600);
	test("-25200",tzo[i++]==-25200);
	test("-12600",tzo[i++]==-12600);
	test("-9000",tzo[i++]==-9000);
	test("-12600",tzo[i++]==-12600);
	test("-32400",tzo[i++]==-32400);
	test("-28800",tzo[i++]==-28800);
	test("-32400",tzo[i++]==-32400);
	test("-25200",tzo[i++]==-25200);
	test("-21600",tzo[i++]==-21600);
	test("-25200",tzo[i++]==-25200);
	test("3600",tzo[i++]==3600);
	test("7200",tzo[i++]==7200);
	test("3600",tzo[i++]==3600);
	test("-12600",tzo[i++]==-12600);
	test("-9000",tzo[i++]==-9000);
	test("-12600",tzo[i++]==-12600);
	test("-28800",tzo[i++]==-28800);
	test("-25200",tzo[i++]==-25200);
	test("-28800",tzo[i++]==-28800);
	test("0",tzo[i++]==0);
	test("3600",tzo[i++]==3600);
	stdoutput.printf("\n");


	// switch time zones
	char	*os=sys::getOperatingSystemName();
	char	*rel=sys::getOperatingSystemRelease();
	double	ver=charstring::toFloat(rel);
	// FIXME: not supported on linux libc, however it's possible that
	// there's a distro with a pre 2.0 kernel that doesn't use libc
	// Also doesn't appear to work on Mac OS X <= 10.1
	bool	notsupported=
			((!charstring::compare(os,"Linux",5) && ver<2.0) ||
			(!charstring::compare(os,"Darwin",6) && ver<6.0));
	delete[] os;
	delete[] rel;
			
	if (!notsupported) {
		stdoutput.printf("switch time zones:\n");
		dt.initialize(datestring);
		dt.adjustTimeZone("CST6CDT");
		test("CST",!charstring::compare(dt.getString(),cstdatestring));
		dt.adjustTimeZone("MST7MDT");
		test("MST",!charstring::compare(dt.getString(),mstdatestring));
		dt.adjustTimeZone("PST8PDT");
		test("PST",!charstring::compare(dt.getString(),pstdatestring));
		dt.adjustTimeZone("EST5EDT");
		test("EST",!charstring::compare(dt.getString(),estdatestring));
		stdoutput.printf("\n");
	}


	// valid/invalid dates
	stdoutput.printf("valid/invalid dates:\n");
	const char	*str="02/20/1974 12:00:00";
	test(str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00";
	test(str,!datetime::validDateTime(str));
	str="02/20/1974 12:00:00 EST5EDT";
	test(str,datetime::validDateTime(str));
	str="02/30/1974 12:00:00 EST5EDT";
	test(str,!datetime::validDateTime(str));
	stdoutput.printf("\n");

	// parse
	stdoutput.printf("parse:\n");
	int16_t	year;
	int16_t	month;
	int16_t	day;
	int16_t	hour;
	int16_t	minute;
	int16_t	second;
	int32_t	microsecond;
	bool	isnegative;
	const char	*dd="/-.:";
	const char	*dates[]={
		"Monday, July 20, 2020",
		"Mon, July 20, 2020",
		"Tuesday, July 20, 2020",
		"Tue, July 20, 2020",
		NULL
	};
	for (const char **d=dates; *d; d++) {
		stdoutput.printf("%s\n",*d);
		datetime::parse(*d,false,false,dd,
				&year,&month,&day,
				&hour,&minute,&second,
				&microsecond,&isnegative);
		test("year",year==2020);
		test("month",month==7);
		test("day",day==20);
		test("hour",hour==-1);
		test("minute",minute==-1);
		test("second",second==-1);
		test("microsecond",microsecond==-1);
		test("isnegative",!isnegative);
	}
}
