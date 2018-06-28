// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_JSONDOM_H
#define RUDIMENTS_JSONDOM_H

#include <rudiments/private/jsondomincludes.h>

/** The jsondom class implements a minimal XML DOM parser. */
class RUDIMENTS_DLLSPEC jsondom : public jsonsax, public dom {
	public:

		/** Creates a new instance of the jsondom class. */
		jsondom();

		/** Creates a new instance of the jsondom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each domnode is added to the jsondom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As json tends to
		 *  be highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		jsondom(bool stringcacheenabled);

		/** Deletes this instance of the jsondom class. */
		virtual	~jsondom();

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

		/** Writes the current DOM tree to "out". */
		virtual bool	write(output *out) const;

	protected:

		/** Gets called when an object start is parsed. */
		virtual	bool	objectStart();

		/** Gets called when a member start is parsed. */
		virtual	bool	memberStart();

		/** Gets called when a member name is parsed. */
		virtual	bool	memberName(const char *name);

		/** Gets called when a string value is parsed. */
		virtual	bool	stringValue(const char *value);

		/** Gets called when a number value is parsed. */
		virtual	bool	numberValue(const char *value);

		/** Gets called when a true value is parsed. */
		virtual	bool	trueValue();

		/** Gets called when a false value is parsed. */
		virtual	bool	falseValue();

		/** Gets called when a null value is parsed. */
		virtual	bool	nullValue();

		/** Gets called when an array start is parsed. */
		virtual	bool	arrayStart();

		/** Gets called when an array end is parsed. */
		virtual	bool	arrayEnd();

		/** Gets called when a member end is parsed. */
		virtual	bool	memberEnd();

		/** Gets called when an object end is parsed. */
		virtual	bool	objectEnd();

	#include <rudiments/private/jsondom.h>
};

#endif
