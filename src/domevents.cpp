// Copyright (c) 2015  David Muse
// See the COPYING file for more information.

#include <rudiments/domevents.h>

#if (defined(_MSC_VER) && (_MSC_VER <= 1300)) || \
	(defined(__USLC__) && !defined(__GNUC__)) || \
	defined(__VMS)

	// degenerate debug macros for really incapable compilers
	// (old microsoft compilers, unixware native compiler, openvms compiler)
	static void debugPrintIndent(uint8_t level) {}
	static void debugPrintf(uint8_t level, const char *format, ...) {}
	static void debugSafePrint(uint8_t level, const char *string) {}
	static void debugSafePrintLength(uint8_t level, const char *string,
							uint32_t length) {}
#else

	// working debug macros for more modern compilers
	#define debugPrintIndent(level) if (pvt->_debuglevel>=level) { for (uint32_t i=0; i<pvt->_depth; i++) { stdoutput.printf(" "); } }
	#ifdef _MSC_VER
		#define debugPrintf(level,ARGS,...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS,__VA_ARGS__); stdoutput.flush(); }
	#else
		#define debugPrintf(level,ARGS...) if (pvt->_debuglevel>=level) { stdoutput.printf(ARGS); stdoutput.flush(); }
	#endif
	#define debugSafePrint(level,string) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string); }
	#define debugSafePrintLength(level,string,length) if (pvt->_debuglevel>=level) { stdoutput.safePrint(string,length); }

#endif

class domeventsprivate {
	friend class domevents;
	private:
		dom		_etree;
		domnode	*_eventsnode;
		void		*_data;
		uint8_t		_debuglevel;
};

domevents::domevents() {
	pvt=new domeventsprivate;
	pvt->_eventsnode=NULL;
	pvt->_data=NULL;
	pvt->_debuglevel=0;
}

domevents::~domevents() {
}

bool domevents::setEvents(const char *events) {

	debugPrintf(1,"setting events... ");

	pvt->_eventsnode=NULL;

	if (!pvt->_etree.parseString(events)) {
		debugPrintf(1,"failed\n");
		return false;
	}

	pvt->_eventsnode=pvt->_etree.getRootNode()->
					getFirstTagChild("events","events");

	debugPrintf(1,"success\n");
	return true;
}

bool domevents::setEventHandler(const char *event,
					domeventhandler_t handler) {

	debugPrintf(1,"setting event handler for %s... ",event);

	// walk the event tree...
	uint64_t	count=0;
	for (domnode *node=pvt->_etree.getRootNode();
				!node->isNullNode();
				node=node->getNextTag()) {

		// for each <events:"event"> tag, and for each tag with an
		// event="event" attribute...
		if ((!charstring::compare(node->getNamespace(),"events") &&
				!charstring::compare(node->getName(),event)) ||
			!charstring::compare(
				node->getAttributeValue("event"),event)) {

			// attach the event handler to the node
			node->setPrivateData((void *)handler);
			count++;
		}
	}

	debugPrintf(1,"%lld instances\n",count);

	return true;
}

bool domevents::setData(void *data) {
	pvt->_data=data;
	return true;
}

bool domevents::process(domnode *codetreenode) {

	debugPrintf(1,"process {\n");

	// walk the code tree...
	while (!codetreenode->isNullNode()) {

		// find the corresponding node in the event tree
		domnode	*etnode=findEvent(codetreenode);

		// avoid loops...
		// If this event tree node is already set as the private
		// data of this code tree node, then the node has already
		// been processed by this event.  Move on.
		if (codetreenode->getPrivateData()==etnode) {
			codetreenode=codetreenode->getNextTag();
			continue;
		}

		// attach the event tree node to the code tree node
		codetreenode->setPrivateData(etnode);

		// if there is no corresponding node, then move on
		if (etnode->isNullNode()) {
			codetreenode=codetreenode->getNextTag();
			continue;
		}

		// get the event handler
		domeventhandler_t	handler=
				(domeventhandler_t)etnode->getPrivateData();

		// call the event handler
		domnode	*next=(handler)?
					handler(codetreenode,etnode,pvt->_data):
					codetreenode;

		// if there are a set of event handlers then call those too,
		// but skip the check if we're at the top of the event node
		// tree...
		// FIXME: this works but it's slow...  instead, build a list
		// of handlers and make that the private data of the eventnode
		// instead of just the one handler, then iterate over the list
		if (etnode!=pvt->_eventsnode) {
			for (etnode=etnode->getFirstTagChild();
					!etnode->isNullNode();
					etnode=etnode->getNextTagSibling()) {

				// we can only continue to process events if
				// the previous event returned the same
				// codetreenode, so bail if we got something
				// else back
				if (next!=codetreenode) {
					break;
				}

				// the set of events must be the first tags
				// inside of the parent node, so bail if we
				// find something with a namespace other than
				// "events"
				if (charstring::compare(
					etnode->getNamespace(),"events")) {
					break;
				}

				// get the event handler
				handler=(domeventhandler_t)
						etnode->getPrivateData();
				if (!handler) {
					continue;
				}

				// call the event handler
				next=handler(codetreenode,etnode,pvt->_data);
				if (!next) {
					break;
				}
			}
		}

		// bail if the next node was set null
		if (!next) {
			debugPrintf(1,"} failed\n");
			return false;
		}

		// continue walking
		codetreenode=next;
	}

	debugPrintf(1,"} success\n");
	return true;
}

domnode *domevents::findEvent(domnode *codetreenode) {

	// get the name of the current code tree node, we'll need them later...
	const char	*ctnodens=codetreenode->getNamespace();
	const char	*ctnodename=codetreenode->getName();
	const char	*ctnodevalue=codetreenode->getAttributeValue("value");

	debugPrintf(1,"  <%s%s%s%s%s%s%s> - ",
				(ctnodens)?ctnodens:"",
				(ctnodens)?":":"",
				ctnodename,
				(ctnodevalue)?" ":"",
				(ctnodevalue)?"value=\"":"",
				(ctnodevalue)?ctnodevalue:"",
				(ctnodevalue)?"\"":"");

	// We need to figure out where to start in the event tree.  If the
	// parent of this codetreenode has an event tree node attached to it,
	// then start there.  If it doesn't, then start at the top of the 
	// event node tree.
	domnode	*p=(domnode *)codetreenode->
					getParent()->getPrivateData();
	if (!p || p->isNullNode()) {
		p=pvt->_eventsnode;
	}

	debugPrintf(1,"<%s%s%s> - ",
			(p->getNamespace())?p->getNamespace():"",
			(p->getNamespace())?":":"",
			p->getName());

	// walk the children of the parent event tree node...
	domnode *c=p->getFirstTagChild(ctnodens,ctnodename);
	while (!c->isNullNode()) {

		// test values too, if necessary...
		const char	*v=c->getAttributeValue("value");
		if (charstring::isNullOrEmpty(v) ||
			!charstring::compare(v,ctnodevalue)) {
			break;
		}

		// move on
 		c=c->getNextTagSibling(ctnodens,ctnodename);
	}

	// if we didn't find an event yet, try again from the top of the
	// event node tree (unless that's what we just did)
	if (c->isNullNode() && p!=pvt->_eventsnode) {

		p=pvt->_eventsnode;

		debugPrintf(1,"<%s%s%s> - ",
				(p->getNamespace())?p->getNamespace():"",
				(p->getNamespace())?":":"",
				p->getName());

		// walk the children of the parent event tree node...
		c=p->getFirstTagChild(ctnodens,ctnodename);
		while (!c->isNullNode()) {

			// test values too, if necessary...
			const char	*v=c->getAttributeValue("value");
			if (charstring::isNullOrEmpty(v) ||
				!charstring::compare(v,ctnodevalue)) {
				break;
			}

			// move on
 			c=c->getNextTagSibling(ctnodens,ctnodename);
		}
	}

	if (pvt->_debuglevel) {
		if (!c->isNullNode() && c->getAttributeValue("event")) {
			debugPrintf(1,"%s\n",c->getAttributeValue("event"));
		} else {
			debugPrintf(1,"\n");
		}

		if (!c->getPrivateData() && !charstring::isNullOrEmpty(
					c->getAttributeValue("event"))) {
			debugPrintf(1,"    WARNING: "
					"handler for event \"%s\" not found\n",
					c->getAttributeValue("event"));
		}
	}

	return c;
}

void domevents::setDebugLevel(uint8_t debuglevel) {
	pvt->_debuglevel=debuglevel;
}
