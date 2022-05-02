// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROPDOM_H
#define RUDIMENTS_PROPDOM_H

#include <rudiments/private/propdomincludes.h>

/** The propdom class implements a minimal DOM parser for a JAVA-style
 *  properties file.
 *
 *  It parses a file or string of property-file-formatted data and produces a
 *  dom tree representing the data.  It creates "k" elements for each key, "p" elements for each pound-delimited comment, and "c" elements for each semicolon-delimited comments.  It adds a "v" attribute to each element, containing the value.  For each "k" element, it also adds a "k" attribute, containing the name of the key.  Text dom nodes (nodes of type TEXT_DOMNODETYPE) are used to preserve whitespace.
 *
 *  For example, the following INI:
 *
 *  #pound comment
 *  ;semicolon comment
 *  key1=value1
 *  key2=value2
 *
 *  key 3=value 3
 *  key 4=value 4
 *
 *  would produce the following dom tree:
 *
 *    <p v="pound comment"/>
 *    <s v="semicolon comment"/>
 *    <k k="key1" v="value1"/>
 *    <k k="key2" v="value2"/>
 *
 *    <k k="key 3" v="value 3"/>
 *    <k k="key 4" v="value 4"/>
 *
 *  (Note the blank line of text between key2 and key 3.  This is the text
 *  domnode, used to preserve the whitespace.)
 */

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
		 *  the cache when no more nodes are using it.  If the data is
		 *  highly repetitive this conserves memory at the cost of
		 *  speed.
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

	protected:

		/** Gets called when the start of whitespace (outside of a
		 *  comment, section name, key, or value) is encountered. */
		virtual	bool	whitespaceStart();

		/** Gets called when whitespace (outside of a comment, section
		 *  name, key, or value) is parsed. */
		virtual	bool	whitespace(const char *w);

		/** Gets called when the end of whitespace (outside of a
		 *  comment, section name, key, or value) is encountered. */
		virtual	bool	whitespaceEnd();

		/** Gets called when the start of an exclamation-delimited
		 *  comment is encountered. */
		virtual	bool	exclamationCommentStart();

		/** Gets called when an exclamation-delimited comment is
		 *  parsed. */
		virtual	bool	exclamationComment(const char *c);

		/** Gets called when the end of an exclamation-delimited
		 *  comment is encountered. */
		virtual	bool	exclamationCommentEnd();

		/** Gets called when the start of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentStart();

		/** Gets called when a pound-delimited comment is parsed. */
		virtual	bool	poundComment(const char *c);

		/** Gets called when the end of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentEnd();

		/** Gets called when the start of a key is encountered. */
		virtual	bool	keyStart();

		/** Gets called when a key name is parsed. */
		virtual	bool	key(const char *k);

		/** Gets called when the end of a key is encountered. */
		virtual	bool	keyEnd();

		/** Gets called when the equality delimiter between a key and
		 *  value is parsed. */
		virtual	bool	equals(const char *e);

		/** Gets called when the start of a value is encountered. */
		virtual	bool	valueStart();

		/** Gets called when a value name is parsed. */
		virtual	bool	value(const char *v);

		/** Gets called when the end of a value is encountered. */
		virtual	bool	valueEnd();

	#include <rudiments/private/propdom.h>
};

#endif
