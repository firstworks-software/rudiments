// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSVDOM_H
#define RUDIMENTS_CSVDOM_H

#include <rudiments/private/csvdomincludes.h>

class csvdomprivate;

/** The csvdom class implements a minimal CSV DOM parser. */
class RUDIMENTS_DLLSPEC csvdom : public csvsax, public dom {
	public:

		/** Creates a new instance of the csvdom class. */
		csvdom();

		/** Creates a new instance of the csvdom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each csvdomnode is added to the csvdom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As csv tends to
		 *  be highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each csvdomnode for names and values and freed when the
		 *  csvdomnode is freed.  This is faster but uses much more
		 *  memory. */
		csvdom(bool stringcacheenabled);

		/** Deletes this instance of the csvdom class. */
		virtual	~csvdom();

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

		/** Writes the current DOM tree to "filename" and sets
		 *  permissions of the file to "perms". */
		bool	writeFile(const char *filename, mode_t perms) const;

	protected:

		/** Gets called when the start of the header is encountered. */
		virtual	bool	headerStart();

		/** Gets called when a column name is parsed. */
		virtual	bool	column(const char *name);

		/** Gets called when the end of the header is encountered. */
		virtual	bool	headerEnd();

		/** Gets called when the start of the body is encountered. */
		virtual	bool	bodyStart();

		/** Gets called when the start of a row is encountered. */
		virtual	bool	rowStart();

		/** Gets called when a field is parsed. */
		virtual	bool	field(const char *value);

		/** Gets called when the end of a row is encountered. */
		virtual	bool	rowEnd();

		/** Gets called when the end of the body is encountered. */
		virtual	bool	bodyEnd();

	#include <rudiments/private/csvdom.h>
};

#endif
