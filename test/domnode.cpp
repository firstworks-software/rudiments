// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/domnode.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include "test.cpp"

// Run the full set of embedded-null assertions against a fresh dom tree.
// The dom's string cache is either enabled or disabled, per "d", but
// length-aware values must bypass the cache either way, so the results
// should be identical regardless.
void runTests(dom *d, const char *label) {

	stdoutput.printf("\t%s...\n",label);

	const char	embedded[]="x\0y";
	const size_t	embeddedlength=3;

	d->createRootNode();
	domnode	*root=d->getRootNode();
	domnode	*node=root->appendTag("test");

	// value set with an embedded null

	node->setValue(embedded,embeddedlength);

	test("value length",node->getValueLength()==embeddedlength);
	test("value bytes",
		!bytestring::compare(node->getValue(),embedded,embeddedlength));

	// legacy null-terminated callers should still see a truncated,
	// but non-crashing, view of the value - not the full length
	test("value still null-terminated for legacy callers",
		charstring::getLength(node->getValue())==1);

	// attribute value set with an embedded null

	node->setAttributeValue("attr",embedded,embeddedlength);

	test("attribute value length (by name)",
		node->getAttributeValueLength("attr")==embeddedlength);
	test("attribute value bytes (by name)",
		!bytestring::compare(node->getAttributeValue("attr"),
						embedded,embeddedlength));
	test("attribute value length (by position)",
		node->getAttributeValueLength((uint64_t)0)==embeddedlength);
	test("attribute value bytes (by position)",
		!bytestring::compare(node->getAttributeValue((uint64_t)0),
						embedded,embeddedlength));
	test("attribute value still null-terminated for legacy callers",
		charstring::getLength(node->getAttributeValue("attr"))==1);

	// clone() should preserve the embedded-null value and attribute

	domnode	*clone=node->clone();

	test("clone value length",clone->getValueLength()==embeddedlength);
	test("clone value bytes",
		!bytestring::compare(clone->getValue(),
						embedded,embeddedlength));
	test("clone attribute value length",
		clone->getAttributeValueLength("attr")==embeddedlength);
	test("clone attribute value bytes",
		!bytestring::compare(clone->getAttributeValue("attr"),
						embedded,embeddedlength));

	delete clone;
}

int main(int argc, const char **argv) {

	header("domnode");

	xmldom	cacheenabled(true);
	runTests(&cacheenabled,"string cache enabled");

	xmldom	cachedisabled(false);
	runTests(&cachedisabled,"string cache disabled");

	stdoutput.printf("\n");

	return 0;
}
