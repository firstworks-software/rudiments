// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DOM_H
#define RUDIMENTS_DOM_H

#include <rudiments/private/domincludes.h>

/** The dom class implements a minimal DOM-style parser. */
class RUDIMENTS_DLLSPEC dom : public collection {
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
		 *  the cache when no more nodes are using it.  If the data is
		 *  highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		dom(bool stringcacheenabled);

		/** Deletes this instance of the dom class. */
		virtual	~dom();

		/** Returns "dom". */
		virtual const char	*getType() const;

		/** Parses generic input "in" and generates a DOM tree. */
		virtual bool	parse(input *in)=0;

		/** Parses file "filename" and generates a DOM tree. */
		virtual bool	parseFile(const char *filename)=0;

		/** Parses string "string" and generates a DOM tree. */
		virtual bool	parseString(const char *string)=0;

		/** Returns the root node of the DOM tree. */
		virtual domnode	*getRootNode() const;

		/** Returns the null node of the DOM tree. */
		virtual domnode	*getNullNode() const;

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		virtual void	createRootNode();

		/** Writes the DOM tree to "filename" and sets permissions of
		 *  the file to "perms". */
		virtual ssize_t	writeFile(const char *filename,
						mode_t perms) const;

		/** Writes the DOM tree to "filename" and sets permissions of
		 *  the file to "perms".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is output
		 *  as-is. */
		virtual ssize_t	writeFile(const char *filename,
						mode_t perms,
						bool indent) const;

		/** Writes a representation of the DOM tree to standard
		 *  output. */
		virtual ssize_t	write() const;

		/** Writes a representation of the DOM tree to standard
		 *  output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is output
		 *  as-is. */
		virtual ssize_t	write(bool indent) const;

		/** Writes a representation of the DOM tree to "out". */
		virtual ssize_t	write(output *out) const;

		/** Writes a representation of the DOM tree to "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is output
		 *  as-is. */
		virtual ssize_t	write(output *out, bool indent) const;

		/** Writes an XML representation of the DOM tree to
		 *  standard output. */
		virtual ssize_t	writeXml() const;

		/** Writes an XML representation of the DOM tree to standard
		 *  output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual ssize_t	writeXml(bool indent) const;

		/** Writes an XML representation of the DOM tree to "out". */
		virtual ssize_t	writeXml(output *out) const;

		/** Writes an XML representation of the DOM tree to "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual ssize_t	writeXml(output *out, bool indent) const;

		/** Returns true if the string cache is enabled and false
		 *  if it is disabled. */
		bool	stringCacheEnabled();

	#include <rudiments/private/dom.h>
};

#endif
