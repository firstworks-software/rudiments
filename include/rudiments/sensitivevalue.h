// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SENSITIVEVALUE_H
#define RUDIMENTS_SENSITIVEVALUE_H

#include <rudiments/private/sensitivevalueincludes.h>

/** The sensitivevalue class provides methods for reading sensitive values from
 *  a string, which may optionally include the contents of a file. */

enum sensitivevalue_format_t {
	FORMAT_TEXT=0,
	FORMAT_BINARY,
	FORMAT_HEX
};

class RUDIMENTS_DLLSPEC sensitivevalue {
	public:

		/** Creates an instance of the sensitivevalue class. */
		sensitivevalue();

		/** Deletes this instance of the sensitivevalue class. */
		~sensitivevalue();

		/** Sets the delimiter that starts a file include to
		 *  "delimiter".  Defaults to a left square bracket - "[".
		 *  See parse() for details. */
		void	setIncludeStart(const char *delimiter);

		/** Sets the delimiter that starts a file include to
		 *  "delimiter" of length "len".  Defaults to a left square
		 *  bracket - "[".  See parse() for details. */
		void	setIncludeStart(const char *delimiter, uint64_t len);

		/** Sets the delimiter that ends a file include to
		 *  "delimiter".  Defaults to a right square bracket - "]".
		 *  See parse() for details. */
		void	setIncludeEnd(const char *delimiter);

		/** Sets the delimiter that ends a file include to
		 *  "delimiter" of length "len".  Defaults to a right square
		 *  bracket - "]".  See parse() for details. */
		void	setIncludeEnd(const char *delimiter, uint64_t len);

		/** Sets whether values passed directly into parse() will be
		 *  interpreted as text, binary, or hex data.  Defaults to
		 *  text.  See parse() for details. */
		void	setVerbatimFormat(sensitivevalue_format_t format);

		/** Sets whether values retrieved from files will be
		 *  interpreted as text, binary, or hex data.  Defaults to
		 *  text.  See parse() for details. */
		void	setFileFormat(sensitivevalue_format_t format);

		/** Sets whether or not to trim trailing carriage returns
		 *  and line feeds will be trimmed from text files.  Defaults
		 *  to true.  See parse() for details. */
		void	setChompTextFile(bool chomptextfile);

		/** Sets the file search path.  Defaults to NULL.
		 *  See parse() for details. */
		void	setPath(const char *path);

		/** Sets the file search path.  Defaults to NULL, 0.
		 *  See parse() for details. */
		void	setPath(const char *path, uint64_t len);

		/** Sets the extension to use when searching for text files.
		 *  Defaults to NULL.  See parse() for details. */
		void	setTextExtension(const char *ext);

		/** Sets the extension to use when searching for text files.
		 *  Defaults to NULL, 0.  See parse() for details. */
		void	setTextExtension(const char *ext, uint64_t len);

		/** Sets the extension to use when searching for binary files.
		 *  Defaults to NULL.  See parse() for details. */
		void	setBinaryExtension(const char *ext);

		/** Sets the extension to use when searching for binary files.
		 *  Defaults to NULL, 0.  See parse() for details. */
		void	setBinaryExtension(const char *ext, uint64_t len);

		/** Sets the extension to use when searching for hex files.
		 *  Defaults to NULL.  See parse() for details. */
		void	setHexExtension(const char *ext);

		/** Sets the extension to use when searching for hex files.
		 *  Defaults to NULL, 0.  See parse() for details. */
		void	setHexExtension(const char *ext, uint64_t len);

		/** Returns the delimiter that starts a file include,
 		 *  either the delimiter set by setIncludeStart() or the
 		 *  default if no delimiter was set. */
		const char	*getIncludeStart();

		/** Returns the length of the delimiter that starts a file
		 *  include, either the length of the delimiter set by
		 *  setIncludeStart() or the length of the default if no
		 *  delimiter was set. */
		uint64_t	getIncludeStartLength();

		/** Returns the delimiter that ends a file include,
 		 *  either the delimiter set by setIncludeEnd() or the
 		 *  default if no delimiter was set. */
		const char	*getIncludeEnd();

		/** Returns the length of the delimiter that ends a file
		 *  include, either the length of the delimiter set by
		 *  setIncludeEnd() or the length of the default if no
		 *  delimiter was set. */
		uint64_t	getIncludeEndLength();

		/** Returns whether values passed directly into parse() will be
		 *  interpreted as text, binary, or hex data. */
		sensitivevalue_format_t	getVerbatimFormat();

		/** Returns whether values retrieved from files will be
		 *  interpreted as text, binary, or hex data. */
		sensitivevalue_format_t	getFileFormat();

		/** Returns whether or not trailing carriage returns and line
		 *  feeds will be trimmed from values retrieved from text
		 *  files. */
		bool	getChompTextFile();

		/** Returns the file search path. */
		const char	*getPath();

		/** Returns the file search path length. */
		uint64_t	getPathLength();

		/** Returns the extension that will be used when searching for
		 *  text files. */
		const char	*getTextExtension();

		/** Returns the length of the extension that will be used when
		 *  searching for text files. */
		uint64_t	getTextExtensionLength();

		/** Returns the extension that will be used when searching for
		 *  binary files. */
		const char	*getBinaryExtension();

		/** Returns the length of the extension that will be used when
		 *  searching for binary files. */
		uint64_t	getBinaryExtensionLength();

		/** Returns the extension that will be used when searching for
		 *  hex files. */
		const char	*getHexExtension();

		/** Returns the length of the extension that will be used when
		 *  searching for hex files. */
		uint64_t	getHexExtensionLength();

		/** Parses "in".
		 *  
		 *  If "in" does not begin and end with the delimiters set by
		 *  setIncludeStart()/setIncludeEnd() (or square brackets by
		 *  default) then:
		 *
		 *   * Subsequent calls to getValue()/detachValue() or
		 *     getTextValue()/detachTextValue() will return "in".
		 *
		 *   * If setVerbatimFormat() was set to FORMAT_TEXT (the
		 *     default), then subsequent calls to
		 *     getValue()/detachValue() or
		 *     getTextValue()/detachTextValue() will return "in"
		 *     verbatim and subsequent calls to
		 *     getValueSize()/getTextValueLength() will return the size
		 *     (in bytes) or length (in characters) of "in".
		 *
		 *   * If setVerbatimFormat() was set to FORMAT_BINARY then
		 *     subsequent calls to getValue()/detachValue() will return
		 *     "in" verbatim and subsequent calls to getValueSize()
		 *     will return the size (in bytes) of "in".
		 *
		 *     Subsequent calls to getTextValue()/detachTextValue()
		 *     will also return "in" verbatim, and subsequent calls to
		 *     getTextValueLength() will return the length
		 *     (in characters) of "in", but may be unreliable, as "in"
		 *     may contain NULLs which would be interpreted as string
		 *     terminators.
		 *
		 *   * If setVerbatimFormat() was set to FORMAT_HEX, then
		 *     the contents of "in" will be interpreted as a string
		 *     representation of hex values and converted to the
		 *     corresponding binary data.  Subsequent calls to
		 *     getValue()/detachValue() will return this binary data and
		 *     subsequent calls to getValueSize() will return the size
		 *     (in bytes) of this binary data.
		 *
		 *     Subsequent calls to getTextValue()/detachTextValue()
		 *     will also return this binary data, and subsequent calls
		 *     to getTextValueLength() will return the length
		 *     (in characters) of this binary data, but may be
		 *     unreliable, as the data may contain NULLs which would be
		 *     interpreted as string terminators.
		 *
		 *  If "in" begins and ends with the delimiters set by 
		 *  setIncludeStart()/setIncludeEnd() (or square brackets by
		 *  default) then:
		 *
		 *    * The string between the delimiters will be interpreted
		 *      as a file name, and the file will be searched for using
		 *      the name:
		 *
		 *      * as-is
		 *      * with the path defined by setPath() prepended
		 *        (if not NULL)
		 *      * with the extension defined by setTextExtension()
		 *        appended (if not NULL)
		 *      * with the path defined by setPath() prepended
		 *        (if not NULL) and the extension defined by
		 *        setTextExtension() appended (if not NULL)
		 *      * with the extension defined by setBinaryExtension()
		 *        appended (if not NULL)
		 *      * with the path defined by setPath() prepended
		 *        (if not NULL) and the extension defined by
		 *        setBinaryExtension() appended (if not NULL)
		 *      * with the extension defined by setHexExtension()
		 *        appended (if not NULL)
		 *      * with the path defined by setPath() prepended
		 *        (if not NULL) and the extension defined by
		 *        setHexExtension() appended (if not NULL)
		 *
		 *    * If the file is not found then "in" will be interpreted
		 *      as not having been a file after all.  See the previous
		 *      section for details.
		 *
		 *    * If the file is found then the contents will be fetched.
		 *
		 *    * If the file format was FORMAT_TEXT, either:
		 *    	  * by default
		 *    	  * as set by setFormat()
		 *    	  * or because the file was found by text extension
		 *    	then if chomp-text-files is true, either:
		 *    	  * by default
		 *    	  * or as set by setChompTextFile(true)
		 *    	then trailing carriage returns and line feeds will be
		 *    	removed from the contents of the file.
		 *
		 *    * If the file format was FORMAT_HEX, either:
		 *    	  * as set by setFormat()
		 *    	  * or because the file was found by text extension
		 *    	then the contents of the file will be interpreted as a
		 *    	string representation of hex values and converted to
		 *    	the corresponding binary data.
		 *
		 *    * If the file format was FORMAT_TEXT then subsequent
		 *      calls to getValue()/detachValue() or
		 *      getTextValue()/detachTextValue() will return the
		 *      processed contents of the file and subsequent calls to
		 *      getValueSize()/getTextValueLength() will return the size
		 *      (in bytes) or length (in characters) of the processed
		 *      contents of the file.
		 *
		 *    * If the file format was FORMAT_BINARY, then subsequent
		 *      calls to getValue()/detachValue() will return the
		 *      contents of the file and subsequent calls to
		 *      getValueSize() will return the size (in bytes) of the
		 *      contents of the file.
		 *
		 *      Subsequent calls to getTextValue()/detachTextValue()
		 *      will also return the contents of the file, and
		 *      subsequent calls to getTextValueLength() will return
		 *      the length (in characters) of the contents of the file,
		 *      but may be unreliable, as the contents of the file  may
		 *      contain NULLs which would be interpreted as string
		 *      terminators.
		 *
		 *    * If the file format was FORMAT_HEX, then subsequent
		 *      calls to getValue()/detachValue() will return the
		 *      processed contents of the file and subsequent calls to
		 *      getValueSize() will return the size (in bytes) of the
		 *      processed contents of the file.
		 *
		 *      Subsequent calls to getTextValue()/detachTextValue()
		 *      will also return the processed contents of the file,
		 *      and subsequent calls to getTextValueLength() will
		 *      return the length (in characters) of the processed
		 *      contents of the file, but may be unreliable, as the
		 *      data may contain NULLs which would be interpreted as
		 *      string terminators.  */
		void	parse(const char *in);

		/** Parses the first "inlen" bytes of "in".  See parse() for
		 *  details. */
		void	parse(const char *in, uint64_t inlen);
		
		/** Returns the value stored by a previous call to parse(). */
		const unsigned char	*getValue();

		/** Returns the value stored by a previous call to parse().
		 *  
		 *  Subsequent calls to getValue() or detachValue() will return
		 *  NULL until parse() is called again.
		 *
		 *  The calling program must deallocate the string returned
		 *  from this method. */
		unsigned char	*detachValue();

		/** Returns the number of bytes that getValue() or
		 *  detachValue() would return.
		 *
		 *  Note that in most cases, this should be called prior to
		 *  calling detachValue() as calling it after the value has
		 *  been detached will return 0. */
		uint64_t	getValueSize();

		/** Returns the value stored by a previous call to parse(). */
		const char	*getTextValue();

		/** Returns the value stored by a previous call to parse().
		 *  
		 *  Subsequent calls to getValue() or detachValue() will return
		 *  NULL until parse() is called again.
		 *
		 *  The calling program must deallocate the string returned
		 *  from this method. */
		char		*detachTextValue();

		/** Returns the number of characters that getTextValue() or
		 *  detachTextValue() would return.
		 *
		 *  Note that in most cases, this should be called prior to
		 *  calling detachTextValue() as calling it after the value has
		 *  been detached will return 0. */
		uint64_t	getTextValueLength();

		/** Clears the internal buffer containing the value.
		 *
		 *  Subsequent calls to getValue(), detachValue(),
		 *  getTextValue(), and detachTextValue() will return NULL
		 *  until parse() is called again.  Similarly, subsequent calls
		 *  to getValueSize() and getTextValueLength() will return 0
		 *  until parse() is called again.
		 *
		 *  Does not reset properties such as include delimiters, chomp
		 *  behavior, etc. to defaults. */
		void	clear();

		/** Clears the internal buffer containing the value and
		 *  resets properties such as include delimiters, chomp
		 *  behavior, etc. to defaults. */
		void	reset();

	#include <rudiments/private/sensitivevalue.h>
};

#endif
