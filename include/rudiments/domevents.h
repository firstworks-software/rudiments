// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DOMEVENTS_H
#define RUDIMENTS_DOMEVENTS_H

#include <rudiments/private/domeventsincludes.h>

typedef domnode *(*domeventhandler_t)(domnode *codetreenode,
						domnode *eventtreenode,
						void *data);

class RUDIMENTS_DLLSPEC domevents : public object {
	public:
		domevents();
		~domevents();

		bool	setEvents(const char *events);

		bool	setEventHandler(const char *event,
					domeventhandler_t handler);

		bool	setData(void *data);

		bool	process(domnode *codetreenode);

		void	setDebugLevel(uint8_t debuglevel);


		static	domnode	*ignore(domnode *codetreenode,
						domnode *eventtreenode,
						void *data);

		static	domnode	*rename(domnode *codetreenode,
						domnode *eventtreenode,
						void *data);

		static	domnode	*revalue(domnode *codetreenode,
						domnode *eventtreenode,
						void *data);

	#include <rudiments/private/domevents.h>
};

#endif
