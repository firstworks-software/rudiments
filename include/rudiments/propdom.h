// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROPDOM_H
#define RUDIMENTS_PROPDOM_H

#include <rudiments/private/propdomincludes.h>

/** The propdom class implements a minimal properties file DOM parser. */
class RUDIMENTS_DLLSPEC propdom : public propsax, public dom {
	public:

		/** Creates a new instance of the propdom class. */
		propdom();

		/** Creates a new instance of the propdom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each domnode is added to the propdom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As properties
		 *  tend to be highly repetitive this conserves memory at the
		 *  cost of speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		propdom(bool stringcacheenabled);

		/** Deletes this instance of the propdom class. */
		virtual	~propdom();

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

		virtual	bool	whitespaceStart();
		virtual	bool	whitespace(const char *w);
		virtual	bool	whitespaceEnd();

		virtual	bool	exclamationCommentStart();
		virtual	bool	exclamationComment(const char *c);
		virtual	bool	exclamationCommentEnd();

		virtual	bool	poundCommentStart();
		virtual	bool	poundComment(const char *c);
		virtual	bool	poundCommentEnd();

		virtual	bool	keyStart();
		virtual	bool	key(const char *k);
		virtual	bool	keyEnd();

		virtual	bool	equals(const char *e);

		virtual	bool	valueStart();
		virtual	bool	value(const char *v);
		virtual	bool	valueEnd();

	#include <rudiments/private/propdom.h>
};

#endif
