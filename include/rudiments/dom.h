// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DOM_H
#define RUDIMENTS_DOM_H

#include <rudiments/private/domincludes.h>

class domprivate;

/** The dom class implements a minimal DOM-style parser. */
class RUDIMENTS_DLLSPEC dom {
	public:

		/** Creates a new instance of the dom class. */
		dom();

		/** Creates a new instance of the dom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each domnode is added to the dom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As some formats
		 *  tend to be highly repetitive this conserves memory at the
		 *  cost of speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		dom(bool stringcacheenabled);

		/** Deletes this instance of the dom class. */
		virtual	~dom();

		/** Parses file "filename" and generates a DOM tree. */
		virtual bool	parseFile(const char *filename)=0;

		/** Parses string "string" and generates a DOM tree. */
		virtual bool	parseString(const char *string)=0;

		/** Writes the current DOM tree to "filename" and sets
		 *  permissions of the file to "perms". */
		virtual bool	writeFile(const char *filename,
						mode_t perms) const=0;

		/** Returns true if the string cache is enabled and false
		 *  if it is disabled. */
		bool	stringCacheEnabled();

	#include <rudiments/private/dom.h>
};

#endif
