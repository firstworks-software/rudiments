// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/cronschedule.h>
#include <rudiments/debugprint.h>

class cronscheduleprivate {
	friend class cronschedule;
	private:
		bool	_validschedule;

		linkedlist< cronscheduleperiod * >	_years;
		linkedlist< cronscheduleperiod * >	_months;
		linkedlist< cronscheduleperiod * >	_daysofmonth;
		linkedlist< cronscheduleperiod * >	_daysofweek;
		linkedlist< cronscheduledaypart * >	_dayparts;
};

cronschedule::cronschedule() : object() {
	pvt=new cronscheduleprivate;
	pvt->_validschedule=false;
	pvt->_years.setManageValues(true);
	pvt->_months.setManageValues(true);
	pvt->_daysofmonth.setManageValues(true);
	pvt->_daysofweek.setManageValues(true);
	pvt->_dayparts.setManageValues(true);
}

cronschedule::~cronschedule() {
	delete pvt;
}

bool cronschedule::setSchedule(const char *when) {

	char		**whenparts;
	uint64_t	whenpartscount;
	charstring::split(when," ",true,&whenparts,&whenpartscount);

	if (whenpartscount==5) {
		setSchedule(whenparts[0],whenparts[1],
				whenparts[2],whenparts[3],whenparts[4]);
	} else {
		pvt->_validschedule=false;
	}

	for (uint64_t i=0; i<whenpartscount; i++) {
		delete[] whenparts[i];
	}
	delete[] whenparts;

	return pvt->_validschedule;
}

bool cronschedule::setSchedule(const char *years,
					const char *months,
					const char *daysofmonth,
					const char *daysofweek,
					const char *dayparts) {
	clear();
	if (splitTimePart(&(pvt->_years),years,0,65535) &&
		splitTimePart(&(pvt->_months),months,1,12) &&
		splitTimePart(&(pvt->_daysofmonth),daysofmonth,1,31) &&
		splitTimePart(&(pvt->_daysofweek),daysofweek,1,7) &&
		splitDayParts(dayparts)) {
		pvt->_validschedule=true;
	} else {
		pvt->_validschedule=false;
	}
	return pvt->_validschedule;
}

bool cronschedule::clear() {
	pvt->_validschedule=false;
	pvt->_years.clear();
	pvt->_months.clear();
	pvt->_daysofmonth.clear();
	pvt->_daysofweek.clear();
	pvt->_dayparts.clear();
	return true;
}

bool cronschedule::splitTimePart(linkedlist< cronscheduleperiod * > *periods,
						const char *timepartlist,
						int64_t	min, int64_t max) {

	// handle *'s
	if (!charstring::compare(timepartlist,"*")) {
		cronscheduleperiod	*p=new cronscheduleperiod;
		p->start=min;
		p->end=max;
		periods->append(p);
		return true;
	}

	// split timepartlist on comma
	char		**timeparts;
	uint64_t	timepartscount;
	charstring::split(timepartlist,",",true,&timeparts,&timepartscount);

	// for each of those...
	for (uint64_t i=0; i<timepartscount; i++) {

		// split them on dash
		char		**timepartparts;
		uint64_t	timepartpartscount;
		charstring::split(timeparts[i],"-",true,
					&timepartparts,
					&timepartpartscount);

		// create a new period
		cronscheduleperiod	*p=new cronscheduleperiod;

		// set start
		int64_t	start=charstring::toInteger(timepartparts[0]);
		if (start<min) {
			return false;
		}
		p->start=start;

		// set end
		if (timepartpartscount>1) {
			int64_t	end=charstring::toInteger(timepartparts[1]);
			if (end>max) {
				return false;
			}
			p->end=end;
		} else {
			p->end=start;
		}

		// add period to the list of periods
		periods->append(p);

		// clean up
		for (uint64_t j=0; j<timepartpartscount; j++) {
			delete[] timepartparts[j];
		}
		delete[] timepartparts;
		delete[] timeparts[i];
	}

	// clean up
	delete[] timeparts;

	return true;
}

bool cronschedule::splitDayParts(const char *daypartlist) {

	// handle *'s
	if (!charstring::compare(daypartlist,"*")) {
		cronscheduledaypart	*dp=new cronscheduledaypart;
		dp->starthour=0;
		dp->startminute=0;
		dp->endhour=23;
		dp->endminute=59;
		pvt->_dayparts.append(dp);
		return true;
	}

	// split daypartlist on comma
	char		**dayparts;
	uint64_t	daypartscount;
	charstring::split(daypartlist,",",true,&dayparts,&daypartscount);

	// for each of those...
	for (uint64_t i=0; i<daypartscount; i++) {

		// split them on dash
		char		**daypartparts;
		uint64_t	daypartpartscount;
		charstring::split(dayparts[i],"-",true,
					&daypartparts,
					&daypartpartscount);

		// FIXME: sanity check range, possibly return false;

		// create a new daypart, set the start/end
		// hour/minute and add it to the list of periods
		cronscheduledaypart	*dp=new cronscheduledaypart;
		dp->starthour=charstring::toInteger(daypartparts[0]);
		const char	*minute=
				charstring::findFirst(daypartparts[0],":");
		if (minute) {
			minute++;
		} else {
			minute="0";
		}
		dp->startminute=charstring::toInteger(minute);

		if (daypartpartscount>1) {
			dp->endhour=charstring::toInteger(daypartparts[1]);
			minute=charstring::findFirst(daypartparts[1],":");
			if (minute) {
				minute++;
			} else {
				minute="0";
			}
			dp->endminute=charstring::toInteger(minute);
		} else {
			dp->endhour=dp->starthour;
			dp->endminute=dp->startminute;
		}
		pvt->_dayparts.append(dp);

		// clean up
		for (uint64_t j=0; j<daypartpartscount; j++) {
			delete[] daypartparts[j];
		}
		delete[] daypartparts;
		delete[] dayparts[i];
	}

	// clean upc
	delete[] dayparts;

	return true;
}

bool cronschedule::getIsInSchedule(datetime *dt) {
	return (pvt->_validschedule &&
		getIsInPeriods(&pvt->_years,dt->getYear()) &&
		getIsInPeriods(&pvt->_months,dt->getMonth()) &&
		getIsInPeriods(&pvt->_daysofmonth,dt->getDayOfMonth()) &&
		getIsInPeriods(&pvt->_daysofweek,dt->getDayOfWeek()) &&
		getIsInDayParts(dt->getHour(),dt->getMinute()));
}

bool cronschedule::getIsInSchedule(const char *dt) {
	datetime	d;
	d.init(dt);
	return getIsInSchedule(&d);
}

bool cronschedule::getIsInPeriods(
			linkedlist< cronscheduleperiod * > *periods,
			int32_t timepart) {

	#ifdef DEBUG_MESSAGES
		if (periods==&pvt->_years) {
			stdoutput.printf("years...\n");
		}
		if (periods==&pvt->_months) {
			stdoutput.printf("months...\n");
		}
		if (periods==&pvt->_daysofmonth) {
			stdoutput.printf("daysofmonth...\n");
		}
		if (periods==&pvt->_daysofweek) {
			stdoutput.printf("daysofweek...\n");
		}
	#endif
	
	for (listnode< cronscheduleperiod * > *pn=periods->getFirst();
							pn; pn=pn->getNext()) {

		cronscheduleperiod	*p=pn->getValue();

		debugPrintf("	%d>=%d && %d<=%d - ",
				timepart,p->start,timepart,p->end);

		if (timepart>=p->start && timepart<=p->end) {
			debugPrintf("yes\n");
			return true;
		}

		debugPrintf("no\n");
	}
	return false;
}

bool cronschedule::getIsInDayParts(int32_t hour, int32_t minute) {

	debugPrintf("dayparts...\n");

	for (listnode< cronscheduledaypart * >
				*dpn=pvt->_dayparts.getFirst();
				dpn; dpn=dpn->getNext()) {

		cronscheduledaypart	*dp=dpn->getValue();

		debugPrintf("	%d:%02d>=%hd:%02hd && ",
				hour,minute,dp->starthour,dp->startminute);
		debugPrintf("%d:%02d<=%hd:%02hd - ",
				hour,minute,dp->endhour,dp->endminute);

		if (hour>=dp->starthour && minute>=dp->startminute &&
			hour<=dp->endhour && minute<=dp->endminute) {
			debugPrintf("yes\n");
			return true;
		}

		debugPrintf("no\n");
	}
	return false;
}
