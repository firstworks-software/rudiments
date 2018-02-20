// Copyright (c) 1999-2017 David Muse
// See the COPYING file for more information

#ifndef TEMPLATEENGINE_H
#define TEMPLATEENGINE_H

#include <rudiments/private/templateengineincludes.h>


/** File parsers must have this function signature: */
typedef bool (*fileparser_t)(stringbuffer *output,
					const char *filename,
					void *data);

/** Block parsers must have this function signature: */
typedef bool (*blockparser_t)(stringbuffer *output,
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
	stringbuffer		*output;
	const char	 	*blockname;
	blockparser_t		parser;
	void			*data;
};

class templateengineprivate;

/** The templateengine class provides a framework for registering user-defined
 *  methods to parse and perform replacements in marked-up files or strings. */
class RUDIMENTS_DLLSPEC templateengine {
	public:

		templateengine();
		virtual	~templateengine();

		void	setVariableStart(const char *delimiter);
		void	setVariableEnd(const char *delimiter);
		void	setBlockStartStart(const char *delimiter);
		void	setBlockStartEnd(const char *delimiter);
		void	setBlockEndStart(const char *delimiter);
		void	setBlockEndEnd(const char *delimiter);
		void	setIncludeStart(const char *delimiter);
		void	setIncludeEnd(const char *delimiter);

		const char	*getVariableStart();
		const char	*getVariableEnd();
		const char	*getBlockStartStart();
		const char	*getBlockStartEnd();
		const char	*getBlockEndStart();
		const char	*getBlockEndEnd();
		const char	*getIncludeStart();
		const char	*getIncludeEnd();


		/** Looks up "filename" in "fileparsers", and calls the
		 *  appropriate file parser method.
		 *
		 *  For each variable encountered in "filename", values from
		 *  "vars" are substitued.
		 * 
		 *  Returns true on success and false if the file parser
		 *  method returns false or if no fileparser was found. */
		bool	parse(stringbuffer *output,
				const char *filename,
				fileparser *fileparsers,
				dictionary< const char *, const char * > *vars);

		/** Parses the file at "filename" and writes the output to
		 *  "output".  For each variable encountered, values from
		 *  "vars" are substituted.  For each block encountered, the
		 *  block is looked up in "blockparsers" and the
		 *  appropriate block parser method is called.  The block
		 *  parser method is passed "req", "resp", "output", the
		 *  name, data and length of the block and "data" from the
		 *  blockparser entry.
		 * 
		 *  If a block is encountered but no block parser method
		 *  is registered for it, it is parsed using "vars".
		 * 
		 *  Returns false if a block parser method returns false and
		 *  true otherwise. */
		bool	parse(stringbuffer *output,
				const char *filename,
				blockparser *blockparsers,
				dictionary< const char *, const char * > *vars);

		/** Parses "length" bytes of "block" and writes the output to
		 *  "output".  For each variable encountered, values from
		 *  "vars" are substituted.  For each nested block
		 *  encountered, the block is looked up in "blockparsers"
		 *  and the appropriate block parser method is called.  The
		 *  block parser method is passed "req", "resp", the name,
		 *  data and length of the block, "output" and "data" from
		 *  the blockparser entry.
		 *
		 *  If a block is encountered but no block parser method is
		 *  registered for it, it is parsed using "vars".
		 *
		 *  Returns false if a block parser method returns false and
		 *  true otherwise. */
		bool	parse(stringbuffer *output,
				const char *block,
				uint64_t blocklength,
				blockparser *blockparsers,
				dictionary< const char *, const char * > *vars);

	#include <rudiments/private/templateengine.h>
};

#endif
