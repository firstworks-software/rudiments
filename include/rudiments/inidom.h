// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INIDOM_H
#define RUDIMENTS_INIDOM_H

#include <rudiments/private/inidomincludes.h>

/** The inidom class implements a minimal INI DOM parser.
 *
 *  It parses a file or string of INI-formatted data and produces a dom tree
 *  representing the data.  It creates "s" elements for each section, "k"
 *  elements for each key, "p" elements for each pound-delimited comment, and
 *  "c" elements for each semicolon-delimited comments.  It adds a "v"
 *  attribute to each element, containing the value.  For each "k" element, it
 *  also adds a "k" attribute, containing the name of the key.  Text dom nodes
 *  (nodes of type TEXT_DOMNODETYPE) are used to preserve whitespace.
 *
 *  For example, the following INI:
 *
 *  [section 1]
 *  #pound comment
 *  ;semicolon comment
 *  key1=value1
 *  key2=value2
 *
 *  [section 2]
 *  key 1=value 1
 *  key 2=value 2
 *  #pound comment
 *  ;semicolon comment
 *
 *  would produce the following dom tree:
 *
 *  <s v="section 1">
 *    <p v="pound comment"/>
 *    <s v="semicolon comment"/>
 *    <k k="key1" v="value1"/>
 *    <k k="key2" v="value2"/>
 *  </s>
 *
 *  <s v="section 2">
 *    <k k="key 1" v="value 1"/>
 *    <k k="key 2" v="value 2"/>
 *    <p v="pound comment"/>
 *    <s v="semicolon comment"/>
 *  </s>
 *
 *  (Note the blank line of text between the sections.  This is the text
 *  domnode, used to preserve the whitespace.)
 */
class RUDIMENTS_DLLSPEC inidom : public inisax, public dom {
	public:

		/** Creates a new instance of the inidom class. */
		inidom();

		/** Creates a new instance of the inidom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each domnode is added to the inidom tree,
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
		inidom(bool stringcacheenabled);

		/** Deletes this instance of the inidom class. */
		virtual	~inidom();

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

		/** Gets called when the start of a semicolon-delimited comment
		 *  is encountered. */
		virtual	bool	semicolonCommentStart();

		/** Gets called when a semicolon-delimited comment is parsed. */
		virtual	bool	semicolonComment(const char *c);

		/** Gets called when the end of a semicolon-delimited comment
		 *  is encountered. */
		virtual	bool	semicolonCommentEnd();

		/** Gets called when the start of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentStart();

		/** Gets called when a pound-delimited comment is parsed. */
		virtual	bool	poundComment(const char *c);

		/** Gets called when the end of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentEnd();

		/** Gets called when the start of a section is encountered. */
		virtual	bool	sectionStart();

		/** Gets called when the name of a section is parsed. */
		virtual	bool	section(const char *s);

		/** Gets called when the end of a section is encountered. */
		virtual	bool	sectionEnd();

		/** Gets called when the start of a key is encountered. */
		virtual	bool	keyStart();

		/** Gets called when a key name is parsed. */
		virtual	bool	key(const char *k);

		/** Gets called when the end of a key is encountered. */
		virtual	bool	keyEnd();

		/** Gets called when the start of a value is encountered. */
		virtual	bool	valueStart();

		/** Gets called when a value name is parsed. */
		virtual	bool	value(const char *v);

		/** Gets called when the end of a value is encountered. */
		virtual	bool	valueEnd();

	#include <rudiments/private/inidom.h>
};

#endif
