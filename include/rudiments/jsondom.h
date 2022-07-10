// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_JSONDOM_H
#define RUDIMENTS_JSONDOM_H

#include <rudiments/private/jsondomincludes.h>

/** The jsondom class implements a minimal JSON DOM parser.
 *
 *  It parses a file or string of JSON-formatted data and produces a dom tree
 *  representing the data.  It creates an "r" element for the root, named
 *  elements for each object or array, and "v" elements for each array value.
 *  It adds a "t" attribute to each object, indicating the type: "o" for
 *  standard object, "a" for array, "s" for string, "n" for number, "t" for
 *  true, "f" for false, and "u" for null.  It also adds a "v" attribute to
 *  each string or number object, containing the value.
 *
 *  For example, the following JSON:
 *
 *  {
 *  	string_object: "string value 1",
 *  	number_object: 10,
 *  	true_object: true,
 *  	false_object: false,
 *  	null_object: null,
 *  	composite_object: {
 *  		string_member: "string value",
 *  		number_member: 10,
 *  		true_member: true,
 *  		false_member: false,
 *  		null_member: null
 *  		nested_composite_object: {
 *  			string_member: "string value",
 *  			number_member: 10,
 *  			true_member: true,
 *  			false_member: false,
 *  			null_member: null
 *  		}
 *  	},
 *  	array_object: ["string value", 10, true, false, null]
 *  }
 *
 *  would produce the following dom tree:
 *
 *  <r t="o">
 *    <string_object t="s" v="string value 1"/>
 *    <number_object t="n" v="10"/>
 *    <true_object t="t"/>
 *    <false_object t="f"/>
 *    <null_object t="u"/>
 *    <composite_object t="o">
 *      <string_member t="s" v="string value"/>
 *      <number_member t="n" v="10"/>
 *      <true_member t="t"/>
 *      <false_member t="f"/>
 *      <null_member t="u"/>
 *      <nested_composite_object t="o">
 *        <string_member t="s" v="string value"/>
 *        <number_member t="n" v="10"/>
 *        <true_member t="t"/>
 *        <false_member t="f"/>
 *        <null_member t="u"/>
 *        <array_object t="a">
 *          <v t="s" v="string value"/>
 *          <v t="n" v="10"/>
 *          <v t="t"/>
 *          <v t="f"/>
 *          <v t="u"/>
 *        </array_object>
 *      </nested_composite_object>
 *    </composite_object>
 *  </r>
 *
 */
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
		 *  the cache when no more nodes are using it.  If the data is
		 *  highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each domnode for names and values and freed when the
		 *  domnode is freed.  This is faster but uses much more
		 *  memory. */
		jsondom(bool stringcacheenabled);

		/** Deletes this instance of the jsondom class. */
		virtual	~jsondom();

		/** Returns "jsondom". */
		virtual const char	*getType() const;

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

	protected:

		/** Gets called when an object start is parsed. */
		virtual	bool	objectStart();

		/** Gets called when a member start is parsed. */
		virtual	bool	memberStart();

		/** Gets called when a member name is parsed. */
		virtual	bool	memberName(const char *name);

		/** Gets called when a value start is parsed. */
		virtual	bool	valueStart();

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

		/** Gets called when an value end is parsed. */
		virtual	bool	valueEnd();

		/** Gets called when a member end is parsed. */
		virtual	bool	memberEnd();

		/** Gets called when an object end is parsed. */
		virtual	bool	objectEnd();

	#include <rudiments/private/jsondom.h>
};

#endif
