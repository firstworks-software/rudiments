// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/domevents.h>
#include <rudiments/xmldom.h>

#if (defined(_MSC_VER) && (_MSC_VER <= 1300)) || \
	(defined(__USLC__) && !defined(__GNUC__)) || \
	defined(__VMS) || defined(__DECCXX) || defined(__SGICXX)

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
		domnode		*_eventsnode;
		void		*_data;
		uint8_t		_debuglevel;
};

domevents::domevents() : object() {
	pvt=new domeventsprivate;
	pvt->_eventsnode=NULL;
	pvt->_data=NULL;
	pvt->_debuglevel=0;
}

domevents::~domevents() {
}

bool domevents::setEvents(domnode *events) {

	debugPrintf(1,"setting events... ");

	pvt->_eventsnode=NULL;

	if (!charstring::compare(events->getNamespace(),"events") &&
			!charstring::compare(events->getName(),"events")) {
		pvt->_eventsnode=events;
	} else {
		pvt->_eventsnode=events->getFirstTagDescendent(
						"events","events");
	}

	if (pvt->_eventsnode->isNullNode()) {
		pvt->_eventsnode=NULL;
		debugPrintf(1,"failed\n");
		return false;
	}

	debugPrintf(1,"success\n");
	return true;
}

bool domevents::setEventHandler(const char *event,
					domeventhandler_t handler) {

	debugPrintf(1,"setting event handler for %s... ",event);

	// walk the event tree...
	uint64_t	count=0;
	for (domnode *node=pvt->_eventsnode;
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

	return count;
}

bool domevents::setData(void *data) {
	pvt->_data=data;
	return true;
}

bool domevents::process(domnode *xmltreenode) {

	debugPrintf(1,"process {\n");

	// walk the xml tree...
	while (!xmltreenode->isNullNode()) {

		// find the corresponding node in the event tree
		domnode	*etnode=findEvent(xmltreenode);

		// if there is no corresponding node, then move on
		if (etnode->isNullNode()) {
			xmltreenode=xmltreenode->getNextTag();
			debugPrintf(1,"...\n");
			continue;
		}

		// avoid loops...
		// If this event tree node is already set as the private
		// data of this xml tree node, then the node has already
		// been processed by this event.  Move on.
		if (xmltreenode->getPrivateData()==etnode) {
			xmltreenode=xmltreenode->getNextTag();
			debugPrintf(1,"(already processed)\n");
			continue;
		}

		// attach the event tree node to the xml tree node
		xmltreenode->setPrivateData(etnode);

		domeventhandler_t	handler=NULL;
		domnode			*next=xmltreenode;

		// if the event node names an event, then call that one event
		if (etnode->getAttributeValue("event")) {

			debugPrintf(1,"%s",
				etnode->getAttributeValue("event"));

			// get the event handler
			handler=(domeventhandler_t)etnode->getPrivateData();

			// call the event handler
			if (handler) {
				next=handler(xmltreenode,etnode,pvt->_data);
			} else {
				debugPrintf(1,"(no handler!)");
			}
		}

		// otherwise there might be a set of events defined as child
		// tags, call each them in order
		else {

			// get the first child tag
			etnode=etnode->getFirstTagChild();

			// the set of events must be the first tags inside of
			// the parent node, so only proceed if the first tag
			// has a namespace of "events"
			if (!charstring::compare(
				etnode->getNamespace(),"events")) {

				do {

					debugPrintf(1,"%s ",etnode->getName());

					// get the event handler
					handler=(domeventhandler_t)
						etnode->getPrivateData();
					if (!handler) {
						debugPrintf(1,"(no handler!)");
						break;
					}

					// call the event handler
					next=handler(xmltreenode,etnode,
								pvt->_data);
					if (!next) {
						break;
					}

					// we can only continue to process
					// events if the previous event
					// returned the same xmltreenode, so
					// bail if we got something else back
					if (next!=xmltreenode) {
						break;
					}

					// move on...
					etnode=etnode->getNextTagSibling();

					// bail if we encounter a tag with a
					// namespace other than "events"
					if (charstring::compare(
							etnode->getNamespace(),
							"events")) {
						break;
					}
				
				} while (!etnode->isNullNode());

			} else {
				debugPrintf(1,"...");
			}
		}
		debugPrintf(1,"\n");

		// bail if the next node was set null
		if (!next) {
			debugPrintf(1,"  failed\n}\n");
			return false;
		}

		// continue walking
		xmltreenode=next;
	}

	debugPrintf(1,"  success\n}\n");
	return true;
}

domnode *domevents::findEvent(domnode *xmltreenode) {

	// get the name of the current xml tree node, we'll need them later...
	const char	*xmlnodens=xmltreenode->getNamespace();
	const char	*xmlnodename=xmltreenode->getName();
	const char	*xmlnodevalue=xmltreenode->getAttributeValue("value");

	debugPrintf(1,"  1:<%s%s%s%s%s%s%s>: ",
				(xmlnodens)?xmlnodens:"",
				(xmlnodens)?":":"",
				xmlnodename,
				(xmlnodevalue)?" ":"",
				(xmlnodevalue)?"value=\"":"",
				(xmlnodevalue)?xmlnodevalue:"",
				(xmlnodevalue)?"\"":"");

	// We need to figure out where to start in the event tree.  If the
	// parent of this xmltreenode has an event tree node attached to it,
	// then start there.  If it doesn't, then start at the top of the 
	// event node tree.
	domnode	*p=(domnode *)xmltreenode->getParent()->getPrivateData();
	if (!p || p->isNullNode()) {
		p=pvt->_eventsnode;
	}

	debugPrintf(1,"<%s%s%s> -> ",
			(p->getNamespace())?p->getNamespace():"",
			(p->getNamespace())?":":"",
			p->getName());

	// walk the children of the parent event tree node...
	domnode *c=p->getFirstTagChild(xmlnodens,xmlnodename);
	while (!c->isNullNode()) {

		debugPrintf(1,"<%s%s%s> -> ",
				(c->getNamespace())?c->getNamespace():"",
				(c->getNamespace())?":":"",
				c->getName());

		// test values too, if necessary...
		const char	*v=c->getAttributeValue("value");
		if (charstring::isNullOrEmpty(v) ||
			!charstring::compare(v,xmlnodevalue)) {
			break;
		}

		// move on
 		c=c->getNextTagSibling(xmlnodens,xmlnodename);
	}

	// if we didn't find an event yet, try again from the top of the
	// event node tree (unless that's what we just did)
	if (c->isNullNode() && p!=pvt->_eventsnode) {

		debugPrintf(1,"...\n");
		debugPrintf(1,"  2:<%s%s%s%s%s%s%s>: ",
				(xmlnodens)?xmlnodens:"",
				(xmlnodens)?":":"",
				xmlnodename,
				(xmlnodevalue)?" ":"",
				(xmlnodevalue)?"value=\"":"",
				(xmlnodevalue)?xmlnodevalue:"",
				(xmlnodevalue)?"\"":"");

		p=pvt->_eventsnode;

		debugPrintf(1,"<%s%s%s> -> ",
				(p->getNamespace())?p->getNamespace():"",
				(p->getNamespace())?":":"",
				p->getName());

		// walk the children of the parent event tree node...
		c=p->getFirstTagChild(xmlnodens,xmlnodename);
		while (!c->isNullNode()) {

			debugPrintf(1,"<%s%s%s> -> ",
				(c->getNamespace())?c->getNamespace():"",
				(c->getNamespace())?":":"",
				c->getName());

			// test values too, if necessary...
			const char	*v=c->getAttributeValue("value");
			if (charstring::isNullOrEmpty(v) ||
				!charstring::compare(v,xmlnodevalue)) {
				break;
			}

			// move on
 			c=c->getNextTagSibling(xmlnodens,xmlnodename);
		}
	}

	return c;
}

void domevents::setDebugLevel(uint8_t debuglevel) {
	pvt->_debuglevel=debuglevel;
}
