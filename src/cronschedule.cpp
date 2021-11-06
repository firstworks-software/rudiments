// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/cronschedule.h>
#include <rudiments/debugprint.h>

class cronscheduleprivate {
	friend class cronschedule;
	private:
		linkedlist< cronscheduleperiod * >	_years;
		linkedlist< cronscheduleperiod * >	_months;
		linkedlist< cronscheduleperiod * >	_daysofmonth;
		linkedlist< cronscheduleperiod * >	_daysofweek;
		linkedlist< cronscheduledaypart * >	_dayparts;
};

cronschedule::cronschedule() : object() {
	pvt=new cronscheduleprivate;
}

cronschedule::~cronschedule() {
	clear();
	delete pvt;
}

void cronschedule::setSchedule(const char *when) {

	clear();

	char		**whenparts;
	uint64_t	whenpartscount;
	charstring::split(when," ",true,&whenparts,&whenpartscount);

	if (whenpartscount==5) {
		init(whenparts[0],whenparts[1],
			whenparts[2],whenparts[3],whenparts[4]);
	}

	for (uint64_t i=0; i<whenpartscount; i++) {
		delete[] whenparts[i];
	}
	delete[] whenparts;
}

void cronschedule::setSchedule(const char *years,
					const char *months,
					const char *daysofmonth,
					const char *daysofweek,
					const char *dayparts) {
	clear();
	init(years,months,daysofmonth,daysofweek,dayparts);
}

void cronschedule::init(const char *years,
				const char *months,
				const char *daysofmonth,
				const char *daysofweek,
				const char *dayparts) {
	splitTimePart(&(pvt->_years),years);
	splitTimePart(&(pvt->_months),months);
	splitTimePart(&(pvt->_daysofmonth),daysofmonth);
	splitTimePart(&(pvt->_daysofweek),daysofweek);
	splitDayParts(dayparts);
}

void cronschedule::clear() {
	listnode< cronscheduleperiod * > *np;
	for (np=pvt->_years.getFirst(); np; np=np->getNext()) {
		delete np->getValue();
	}
	for (np=pvt->_months.getFirst(); np; np=np->getNext()) {
		delete np->getValue();
	}
	for (np=pvt->_daysofmonth.getFirst(); np; np=np->getNext()) {
		delete np->getValue();
	}
	for (np=pvt->_daysofweek.getFirst(); np; np=np->getNext()) {
		delete np->getValue();
	}
	for (listnode< cronscheduledaypart * > *ndp=
						pvt->_dayparts.getFirst();
						ndp; ndp=ndp->getNext()) {
		delete ndp->getValue();
	}
}

void cronschedule::splitTimePart(linkedlist< cronscheduleperiod * > *periods,
						const char *timepartlist) {

	// handle *'s
	if (!charstring::compare(timepartlist,"*")) {
		cronscheduleperiod	*p=new cronscheduleperiod;
		if (periods==&pvt->_years) {
			p->start=0;
			// FIXME: is there a macro for this?
			p->end=65535;
		}
		if (periods==&pvt->_months) {
			p->start=1;
			p->end=12;
		}
		if (periods==&pvt->_daysofmonth) {
			p->start=1;
			p->end=31;
		}
		if (periods==&pvt->_daysofweek) {
			p->start=1;
			p->end=7;
		}
		periods->append(p);
		return;
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

		// create a new period, set the start/end
		// and add it to the list of periods
		cronscheduleperiod	*p=new cronscheduleperiod;
		p->start=charstring::toInteger(timepartparts[0]);
		if (timepartpartscount>1) {
			p->end=charstring::toInteger(timepartparts[1]);
		} else {
			p->end=p->start;
		}
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
}

void cronschedule::splitDayParts(const char *daypartlist) {

	// handle *'s
	if (!charstring::compare(daypartlist,"*")) {
		cronscheduledaypart	*dp=new cronscheduledaypart;
		dp->starthour=0;
		dp->startminute=0;
		dp->endhour=23;
		dp->endminute=59;
		pvt->_dayparts.append(dp);
		return;
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
}

bool cronschedule::inSchedule(datetime *dt) {
	return (inPeriods(&pvt->_years,dt->getYear()) &&
		inPeriods(&pvt->_months,dt->getMonth()) &&
		inPeriods(&pvt->_daysofmonth,dt->getDayOfMonth()) &&
		inPeriods(&pvt->_daysofweek,dt->getDayOfWeek()) &&
		inDayParts(dt->getHour(),dt->getMinutes()));
}

bool cronschedule::inSchedule(const char *dt) {
	datetime	d;
	d.initialize(dt);
	return inSchedule(&d);
}

bool cronschedule::inPeriods(
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

bool cronschedule::inDayParts(int32_t hour, int32_t minute) {

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
