// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/datetime.h>
#include <rudiments/linkedlist.h>

class cronscheduleprivate;

class RUDIMENTS_DLLSPEC cronscheduleperiod {
	public:
		uint16_t	start;
		uint16_t	end;
};

class RUDIMENTS_DLLSPEC cronscheduledaypart {
	public:
		uint16_t	starthour;
		uint16_t	startminute;
		uint16_t	endhour;
		uint16_t	endminute;
};
