// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_TEMPLATEENGINE_H
#define RUDIMENTS_TEMPLATEENGINE_H

#include <rudiments/private/templateengineincludes.h>


/** File parsers must have this function signature: */
typedef bool (*fileparser_t)(output *out,
					const char *filename,
					void *data);

/** Block parsers must have this function signature: */
typedef bool (*blockparser_t)(output *out,
					const char *blockname,
					const char *block,
					uint64_t blocklength,
					void *data);


/** This struct associates module/file with parser and is used with
 *  handlePage() below. */
struct fileparser {
	const char	*file;
	fileparser_t	parser;
	void		*data;
};

/** This struct associates module/file with parser and is used with
 *  handlePage() below. */
struct blockparser {
	output		*out;
	const char	 	*blockname;
	blockparser_t		parser;
	void			*data;
};

class templateengineprivate;

/** The templateengine class provides a framework for registering user-defined
 *  methods to parse and perform replacements in marked-up files or strings. */
class RUDIMENTS_DLLSPEC templateengine {
	public:

		/** Creates an instance of the templateengine class. */
		templateengine();

		/** Deletes this instance of the templateengine class. */
		virtual	~templateengine();

		/** Looks up "filename" in "fileparsers", and calls the
		 *  appropriate file parser method.
		 *
		 *  For each variable encountered in "filename", values from
		 *  "vars" are substitued.
		 * 
		 *  Returns true on success and false if the file parser
		 *  method returns false or if no fileparser was found. */
		bool	parse(output *out,
				const char *filename,
				fileparser *fileparsers,
				dictionary< const char *, const char * > *vars);

		/** Parses the file at "filename" and writes the out to
		 *  "out".  For each variable encountered, values from
		 *  "vars" are substituted.  For each block encountered, the
		 *  block is looked up in "blockparsers" and the
		 *  appropriate block parser method is called.  The block
		 *  parser method is passed "req", "resp", "out", the
		 *  name, data and length of the block and "data" from the
		 *  blockparser entry.
		 * 
		 *  If a block is encountered but no block parser method
		 *  is registered for it, it is parsed using "vars".
		 * 
		 *  Returns false if a block parser method returns false and
		 *  true otherwise. */
		bool	parse(output *out,
				const char *filename,
				blockparser *blockparsers,
				dictionary< const char *, const char * > *vars);

		/** Parses "length" bytes of "block" and writes the out to
		 *  "out".  For each variable encountered, values from
		 *  "vars" are substituted.  For each nested block
		 *  encountered, the block is looked up in "blockparsers"
		 *  and the appropriate block parser method is called.  The
		 *  block parser method is passed "req", "resp", the name,
		 *  data and length of the block, "out" and "data" from
		 *  the blockparser entry.
		 *
		 *  If a block is encountered but no block parser method is
		 *  registered for it, it is parsed using "vars".
		 *
		 *  Returns false if a block parser method returns false and
		 *  true otherwise. */
		bool	parse(output *out,
				const char *block,
				uint64_t blocklength,
				blockparser *blockparsers,
				dictionary< const char *, const char * > *vars);

		/** Sets the delimiter that the parse() methods look for at
		 *  the beginning of a variable to "delimiter".
		 *
		 *  Defaults to "$(".
		 *
		 *  For example, by defaults, variables are delimited like:
		 *  $(variable) */
		void	setVariableStart(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the end of a variable to "delimiter".
		 *
		 *  Defaults to ")".
		 *
		 *  For example, by defaults, variables are delimited like:
		 *  $(variable) */
		void	setVariableEnd(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  beginning of a block-start to "delimiter".
		 *
		 *  Defaults to "<!-- start ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		void	setBlockStartStart(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the end of a block-start to "delimiter".
		 *
		 *  Defaults to " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		void	setBlockStartEnd(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the beginning of a block-end to "delimiter".
		 *
		 *  Defaults to "<!-- end ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		void	setBlockEndStart(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the end of a block-end to "delimiter".
		 *
		 *  Defaults to " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		void	setBlockEndEnd(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the beginning of an include to "delimiter".
		 *
		 *  Defaults to "<!-- include ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- include filename -->
		 */
		void	setIncludeStart(const char *delimiter);

		/** Sets the delimiter that the parse() methods look for at
		 *  the end of an include to "delimiter".
		 *
		 *  Defaults to " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- include filename -->
		 */
		void	setIncludeEnd(const char *delimiter);

		/** Returns the delimiter that the parse() methods look for at
		 *  the beginning of a variable.
		 *
		 *  The defaults is "$(".
		 *
		 *  For example, by defaults, variables are delimited like:
		 *  $(variable) */
		const char	*getVariableStart();

		/** Returns the delimiter that the parse() methods look for at
		 *  the end of a variable.
		 *
		 *  The defaults is ")".
		 *
		 *  For example, by defaults, variables are delimited like:
		 *  $(variable) */
		const char	*getVariableEnd();

		/** Returns the delimiter that the parse() methods look for at
		 *  beginning of a block-start.
		 *
		 *  The default is "<!-- start ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		const char	*getBlockStartStart();

		/** Returns the delimiter that the parse() methods look for at
		 *  the end of a block-start.
		 *
		 *  The default is " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		const char	*getBlockStartEnd();

		/** Returns the delimiter that the parse() methods look for at
		 *  the beginning of a block-end.
		 *
		 *  The defaults is "<!-- end ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		const char	*getBlockEndStart();

		/** Returns the delimiter that the parse() methods look for at
		 *  the end of a block-end.
		 *
		 *  The defaults is " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- start block -->
		 *  ... block ...
		 *  <!-- end block -->
		 */
		const char	*getBlockEndEnd();

		/** Returns the delimiter that the parse() methods look for at
		 *  the beginning of an include.
		 *
		 *  The defaults is "<!-- include ".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- include filename -->
		 */
		const char	*getIncludeStart();

		/** Returns the delimiter that the parse() methods look for at
		 *  the end of an include.
		 *
		 *  The default is " -->".
		 *
		 *  For example, by defaults, blocks are delimited like:
		 *
		 *  <!-- include filename -->
		 */
		const char	*getIncludeEnd();

	#include <rudiments/private/templateengine.h>
};

#endif
