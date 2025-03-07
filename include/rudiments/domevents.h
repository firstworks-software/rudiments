// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DOMEVENTS_H
#define RUDIMENTS_DOMEVENTS_H

#include <rudiments/private/domeventsincludes.h>

typedef domnode *(*domeventhandler_t)(domnode *xmltreenode,
						domnode *eventtreenode,
						void *data);

/** The domevents class implements a framework for processing and modifying dom
 *  trees by attaching events to tags and event handlers to those events.
 *
 *  Lets say we have the following XML:
 *
 *  <p>Hello there <b>Dave</b> check out this photo:
 *  <img src="coolphoto.jpg"/> It&apos;s <i>pretty cool</i>.</p>
 *
 *  ...and, inside paragraphs, we'd like to remove any italics and convert bold
 *  text to emphasized text.  We'd also like to change any refrence to
 *  coolphoto.jpg to coolerphoto.jpg.
 *
 *  We can define the following event tree:
 *
 *  <events:events>
 *
 *    <p>
 *      <b event="rename" to="em"/>
 *      <i event="ignore"/>
 *    </p>
 *
 *    <img event="resrc" from="coolphoto.jpg" to="coolerphoto.jpg"/>
 *
 *  </events:events>
 *
 *  When the XML is processed using this event tree:
 *   * Inside of <p> tags...
 *     * If a <b> tag is encountered then a "rename" event will be triggered.
 *     * If a <i> tag is encountered then an "ignore" event will be triggered.
 *   * If an <img> tag is encountered (inside or outside of any other tag),
 *       then a "resrc" event will be triggered.
 *
 *  Event handlers for "rename", "ignore" and "resrc" events can be written to
 *  perform these actions.
 *
 *  For example:
 *
 *  // define some event handlers that we'll implement later
 *  static	domnode	*rename(domnode *xmltreenode,
 *  						domnode *eventtreenode,
 *  						void *data);
 *  static	domnode	*ignore(domnode *xmltreenode,
 *  						domnode *eventtreenode,
 *  						void *data);
 *  static	domnode	*resrc(domnode *xmltreenode,
 *  						domnode *eventtreenode,
 *  						void *data);
 *
 *  int main(int argc, const char **argv) {
 *
 *      // parse the xml
 *  	xmldom	xml;
 *  	xml.parseString("<p>Hello there...");
 *
 *
 * 	// parse the events
 *  	xmldom	events;
 *  	events.parseString("<events:events>...");
 *
 *
 * 	// create an instance of domevents
 *  	domevents	d;
 *
 *  	// configure it to use the events that we defined
 *  	d.setEvents(events.getRootNode());
 *
 *	// attach events to event handlers
 *  	d.setEventHandler("rename",rename);
 *  	d.setEventHandler("ignore",ignore);
 *  	d.setEventHandler("resrc",resrc);
 *
 * 	// process the xml, using these events
 *  	d.process(xml.getRootNode());
 *
 *
 * 	// write out the processed xml
 *  	xml.write(false);
 *  }
 *
 *  // rename the xml tag to the value of the event tag's "to" attribute
 *  domnode	*rename(domnode *xmltreenode, domnode *eventtreenode,
 *  								void *data) {
 *  	xmltreenode->setName(eventtreenode->getAttributeValue("to"));
 *  	return xmltreenode;
 *  }
 *
 *  // remove the xml tag, preserving children
 *  domnode	*ignore(domnode *xmltreenode, domnode *eventtreenode,
 *  								void *data) {
 *  	domnode	*parent=xmltreenode->getParent();
 *  	// when using terse syntax:
 *  	//	get the tag name from the event node
 *  	// when using verbose syntax:
 *  	//	get the tag name from the event node parent
 *  	const char	*tag=
 *  		(!charstring::compare(eventtreenode->getNamespace(),"events"))?
 *  			eventtreenode->getParent()->getName():
 *  			eventtreenode->getName();
 *  	parent->unwrapFirstChild(tag);
 *	return parent->getFirstTagChild();
 *  }
 *
 *  // if the value of the xml tag's "src" attribute is the same as the value
 *  // of the event tag's "from" attribute, then replace it with the value of
 *  // the event tag's "to" attribute
 *  domnode	*resrc(domnode *xmltreenode, domnode *eventtreenode,
 *  								void *data) {
 *  	const char	*src=xmltreenode->getAttributeValue("src");
 *  	const char	*from=eventtreenode->getAttributeValue("from");
 *  	const char	*to=eventtreenode->getAttributeValue("to");
 *  	if (!charstring::compare(src,from)) {
 *  		xmltreenode->setAttributeValue("src",to);
 *  	}
 *  	return xmltreenode;
 *  }
 *
 *
 *  When run, this program should output:
 *
 *  <p>Hello there <em>Dave</em> check out this photo:
 *  <img src="coolerphoto.jpg"/> It&apos's pretty cool.</p>
 *
 *
 *  Note that each of the event handlers return the node at which processing
 *  will continue.  In most cases, they return the modified node, which will
 *  cause it to be reprocessed, possibly triggering other events.  Care should
 *  be taken to avoid defining events that could loop, eg:
 *
 *  	<b event="rename" to="i"/>
 *  	<i event="rename" to="b"/>
 *
 *  If an event handler returns NULL, process() will immediately return failure.
 *
 *  The rules in the example above are very simple, but complex rules, which
 *  perform much more involved restructuring of the dom tree, can be defined.
 *
 *
 *  Note that the event tree in this example only uses one level of nesting,
 *  but multiple levels can be defined.  For example, if we only wanted to
 *  ignore italics that are also bold (eg. <b><i>hello</i></b>), we could do
 *  something like:
 *
 *  <events:events>
 *
 *    <p>
 *      <b>
 *        <i event="ignore"/>
 *      </b>
 *      <b event="rename" to="em"/>
 *    </p>
 *
 *    <img event="resrc" from="coolphoto.jpg" to="coolerphoto.jpg"/>
 *
 *  </events:events>
 *
 *  Note that events are processed in order.  As such, the event that ignores
 *  italics that are also bold comes prior to the event that renames <b> to
 *  <em>.  If these events were reversed, then italics that are also bold would
 *  never be encountered, as all <b> would be converted to <em> prior to
 *  processing of the second event.
 *
 *  A more verbose syntax for defining events is as follows:
 *
 *  <events:events>
 *
 *    <p>
 *      <b>
 *        <events:rename to="em"/>
 *      </b>
 *      <i>
 *        <events:ignore/>
 *      </i>
 *    </p>
 *
 *    <img>
 *      <events:resrc from="coolphoto.jpg" to="coolerphoto.jpg"/>
 *    </img>
 *
 *  </events:events>
 *
 *  This event tree is equivalent to the one defined initially, but rather than
 *  using the event="..." attribute, it defines separate event tags for each
 *  event.
 *
 *  It is also possible to assign multiple events to a tag.  To do this you
 *  must use the more verbose syntax for those events.  For example:
 *
 *  <events:events>
 *
 *    <p>
 *      <b event="rename" to="em"/>
 *      <i event="ignore"/>
 *    </p>
 *
 *    <img>
 *      <events:resrc from="coolphoto.jpg" to="coolerphoto.jpg"/>
 *      <events:resrc from="coolerphoto.jpg" to="coolestphoto.jpg"/>
 *    </img>
 *
 *  </events:events>
 *
 *  In this (contrived) example, when an <img> tag is encountered, with
 *  src="coolphoto.jpg", it will be replaced with src="coolerphoto.jpg".
 *  Then, if an <img> tag is encountered with src="coolerphoto.jpg", it will
 *  be replaced with src="coolestphoto.jpg".
 */
class RUDIMENTS_DLLSPEC domevents : public object {
	public:

		/** Creates an instance of the domevents class. */
		domevents();

		/** Deletes this instance of the domevents class. */
		~domevents();

		/** Defines the set of events to respond to.  "events" should
		 *  be a domtreenode of an <events:events> tag, or with a
		 *  descendent that is an <events:events> tag, with children
		 *  as defined in the class description.
		 *
		 *  NOTE: calling setEvents() removes any existing event
		 *  handlers set by previous calls to setEventHandler().
		 *
		 *  Returns true on success and false on failure, eg. if an
		 *  <events:events> tag wasn't found. */
		bool	setEvents(domnode *events);

		/** Associates function "handler" with "event".
		 *
		 *  NOTE: calling setEvents() removes any existing event
		 *  handlers set by previous calls to setEventHandler().
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  NOTE: it is not considered a failure if the set of events
		 *  previously set using setEvents() does not contain any
		 *  instances of "event". */
		bool	setEventHandler(const char *event,
					domeventhandler_t handler);

		/** Defines arbitrary data that will be passed into the "data"
		 *  parameter of the event handler.
		 *
		 *  Returns true on success and false on failure. */
		bool	setData(void *data);

		/** Walks "xmltreenode", applying events and event handlers
		 *  previously set by calls to setEvents() and
		 *  setEventHandler().
		 *
		 *  For each domnode of code is encountered that should trigger
		 *  an event, the corresponding event handler is called,
		 *  and passed the domnode of code, the domnode of the event,
		 *  and whatever data was set by a previous call to setData().
 		 *
		 *  Returns true on success and false on failure. */
		bool	process(domnode *xmltreenode);

		/** Sets the debug level to "debuglevel".  Defaults to 0
		 *  (no debug).  Currently, setting to debug level 1 causes all
		 *  debug messages to be printed, but additional debug levels
		 *  may be added in the future. */
		void	setDebugLevel(uint8_t debuglevel);

	#include <rudiments/private/domevents.h>
};

#endif
