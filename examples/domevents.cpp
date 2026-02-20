#include <rudiments/domevents.h>
#include <rudiments/xmldom.h>
#include <rudiments/domnode.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

// event handler: rename a tag
static domnode *renametag(domnode *xmltreenode,
				domnode *eventtreenode,
				void *data) {
	xmltreenode->setName(eventtreenode->getAttributeValue("to"));
	return xmltreenode;
}

// event handler: remove a tag, preserving its children
static domnode *ignoretag(domnode *xmltreenode,
				domnode *eventtreenode,
				void *data) {
	domnode	*parent=xmltreenode->getParent();
	const char	*tag=
		(!charstring::compare(
				eventtreenode->getNamespace(),"events"))?
			eventtreenode->getParent()->getName():
			eventtreenode->getName();
	parent->unwrapFirstChild(tag);
	return parent->getFirstTagChild();
}

int main(int argc, const char **argv) {

	// parse some xml to process
	xmldom	xml;
	xml.parseString(
		"<p>Hello there <b>Dave</b>, "
		"how are <i>you</i> today?</p>");

	stdoutput.write("before:\n");
	xml.writeXml();
	stdoutput.write("\n\n");


	// parse an event tree
	xmldom	events;
	events.parseString(
		"<events:events>"
		"  <p>"
		"    <b event=\"rename\" to=\"em\"/>"
		"    <i event=\"ignore\"/>"
		"  </p>"
		"</events:events>");


	// create domevents and configure it
	domevents	d;
	d.setEvents(events.getRootNode());
	d.setEventHandler("rename",renametag);
	d.setEventHandler("ignore",ignoretag);


	// process the xml using the events
	d.process(xml.getRootNode());

	stdoutput.write("after:\n");
	xml.writeXml();
	stdoutput.write('\n');
}
