// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/domevents.h>
#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include "test.cpp"

const char original[]=
"<p>Hello there <b>Dave</b> check out this photo:\n"
"<img src=\"coolphoto.jpg\"/>  It&apos;s <i>pretty cool</i>.</p>"
"\n";

const char mod1[]=
"<p>Hello there <em>Dave</em> check out this photo:\n"
"<img src=\"coolerphoto.jpg\"/>  It&apos;s pretty cool.</p>"
// FIXME: fails if we include the trailing \n
//"\n"
;

const char mod2[]=
"<p>Hello there <em>Dave</em> check out this photo:\n"
"<img src=\"coolestphoto.jpg\"/>  It&apos;s pretty cool.</p>"
// FIXME: fails if we include the trailing \n
//"\n"
;

const char terse[]=
"<events:events>\n"
"\n"
"  <p>\n"
"    <b event=\"rename\" to=\"em\"/>\n"
"    <i event=\"ignore\"/>\n"
"  </p>\n"
"\n"
"  <img event=\"resrc\" from=\"coolphoto.jpg\" to=\"coolerphoto.jpg\"/>\n"
"\n"
"</events:events>\n";

const char verbose[]=
"<events:events>\n"
"\n"
"  <p>\n"
"    <b>\n"
"      <events:rename to=\"em\"/>\n"
"    </b>\n"
"    <i>\n"
"      <events:ignore/>\n"
"    </i>\n"
"  </p>\n"
"\n"
"  <img>\n"
"    <events:resrc from=\"coolphoto.jpg\" to=\"coolerphoto.jpg\"/>\n"
"  </img>\n"
"\n"
"</events:events>\n";

const char multiple[]=
"<events:events>\n"
"\n"
"  <p>\n"
"    <b event=\"rename\" to=\"em\"/>\n"
"    <i event=\"ignore\"/>\n"
"  </p>\n"
"  <img>\n"
"\n"
"    <events:resrc from=\"coolphoto.jpg\" to=\"coolerphoto.jpg\"/>\n"
"    <events:resrc from=\"coolerphoto.jpg\" to=\"coolestphoto.jpg\"/>\n"
"  </img>\n"
"\n"
"</events:events>\n";


// define some event handlers that we'll implement later
static	domnode	*rename(domnode *xmltreenode, domnode *eventtreenode,
							void *data);
static	domnode	*ignore(domnode *xmltreenode, domnode *eventtreenode,
							void *data);
static	domnode	*resrc(domnode *xmltreenode, domnode *eventtreenode,
							void *data);

int main(int argc, const char **argv) {

	header("domevents");


	domevents	d;

	stdoutput.printf("terse...\n");
	xmldom		xml;
	test("parse xml",xml.parseString(original));
	xmldom		events;
	test("parse events",events.parseString(terse));
	test("set events",d.setEvents(events.getRootNode()));
	test("set event handler - rename",d.setEventHandler("rename",rename));
	test("set event handler - ignore",d.setEventHandler("ignore",ignore));
	test("set event handler - resrc",d.setEventHandler("resrc",resrc));
	d.process(xml.getRootNode());
	stringbuffer	result;
	xml.write(&result,false);
	test("result",!charstring::compare(result.getString(),mod1));
	result.clear();
	stdoutput.printf("\n");

	stdoutput.printf("verbose...\n");
	test("parse xml",xml.parseString(original));
	test("parse events",events.parseString(verbose));
	test("set events",d.setEvents(events.getRootNode()));
	test("set event handler - rename",d.setEventHandler("rename",rename));
	test("set event handler - ignore",d.setEventHandler("ignore",ignore));
	test("set event handler - resrc",d.setEventHandler("resrc",resrc));
	d.process(xml.getRootNode());
	xml.write(&result,false);
	test("result",!charstring::compare(result.getString(),mod1));
	result.clear();
	stdoutput.printf("\n");

	stdoutput.printf("multiple...\n");
	test("parse xml",xml.parseString(original));
	test("parse events",events.parseString(multiple));
	test("set events",d.setEvents(events.getRootNode()));
	test("set event handler - rename",d.setEventHandler("rename",rename));
	test("set event handler - ignore",d.setEventHandler("ignore",ignore));
	test("set event handler - resrc",d.setEventHandler("resrc",resrc));
	d.process(xml.getRootNode());
	xml.write(&result,false);
	test("result",!charstring::compare(result.getString(),mod2));
	result.clear();
	stdoutput.printf("\n");

	return 0;
}

// rename the xml tag to the value of the event tag's "to" attribute
domnode	*rename(domnode *xmltreenode, domnode *eventtreenode, void *data) {
	xmltreenode->setName(eventtreenode->getAttributeValue("to"));
	return xmltreenode;
}

// remove the xml tag, preserving children
domnode	*ignore(domnode *xmltreenode, domnode *eventtreenode, void *data) {
	domnode	*parent=xmltreenode->getParent();
	// when using terse syntax:
	//	get the tag name from the event node
	// when using verbose syntax:
	//	get the tag name from the event node parent
	const char	*tag=
		(!charstring::compare(eventtreenode->getNamespace(),"events"))?
			eventtreenode->getParent()->getName():
			eventtreenode->getName();
	parent->unwrapFirstChild(tag);
	return parent->getFirstTagChild();
}

// if the value of the xml tag's "src" attribute is the same as the value
// of the event tag's "from" attribute, then replace it with the value of
// the event tag's "to" attribute
domnode	*resrc(domnode *xmltreenode, domnode *eventtreenode, void *data) {
	const char	*src=xmltreenode->getAttributeValue("src");
	const char	*from=eventtreenode->getAttributeValue("from");
	const char	*to=eventtreenode->getAttributeValue("to");
	if (!charstring::compare(src,from)) {
		xmltreenode->setAttributeValue("src",to);
	}
	return xmltreenode;
}
