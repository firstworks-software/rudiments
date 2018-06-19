// Copyright (c) 2015  David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOMEVENTS_H
#define RUDIMENTS_XMLDOMEVENTS_H

#include <rudiments/dom.h>
#include <rudiments/private/xmldomeventsincludes.h>

class xmldomeventsprivate;

typedef domnode *(*xmldomeventhandler_t)(domnode *codetreenode,
						domnode *eventtreenode,
						void *data);

class RUDIMENTS_DLLSPEC xmldomevents {
	public:
		xmldomevents();
		~xmldomevents();

		bool	setEvents(const char *events);

		bool	setEventHandler(const char *event,
					xmldomeventhandler_t handler);

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

	#include <rudiments/private/xmldomevents.h>
};

#endif
