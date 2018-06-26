// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOM_H
#define RUDIMENTS_XMLDOM_H

#include <rudiments/private/xmldomincludes.h>

/** The xmldom class implements a minimal XML DOM parser. */
class RUDIMENTS_DLLSPEC xmldom : public xmlsax, public dom {
	public:

		/** Creates a new instance of the xmldom class. */
		xmldom();

		/** Creates a new instance of the xmldom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each domnode is added to the xmldom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As xml tends to
		 *  be highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		xmldom(bool stringcacheenabled);

		/** Deletes this instance of the xmldom class. */
		virtual	~xmldom();

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

	protected:

		/** Gets called when a start tag is parsed. */
		virtual	bool	tagStart(const char *ns, const char *name);

		/** Gets called when an attribute name is parsed. */
		virtual	bool	attributeName(const char *name);

		/** Gets called when an attribute value is parsed. */
		virtual	bool	attributeValue(const char *value);

		/** Gets called when a block of text is parsed. */
		virtual	bool	text(const char *string);

		/** Gets called when an end tag is parsed. */
		virtual	bool	tagEnd(const char *ns, const char *name);

		/** Gets called when a comment is parsed. */
		virtual	bool	comment(const char *string);

		/** Gets called when cdata is parsed. */
		virtual	bool	cdata(const char *string);

	#include <rudiments/private/xmldom.h>
};

#endif
