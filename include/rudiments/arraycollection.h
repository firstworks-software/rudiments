// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ARRAYCOLLECTION_H
#define RUDIMENTS_ARRAYCOLLECTION_H

#include <rudiments/collection.h>
#include <rudiments/output.h>

/** The arraycollection class is the parent class for all rudiments array
 *  collections. */
template <class valuetype>
class arraycollection : public collection {
	public:
		/** Returns "array". */
		virtual const char	*getType();

		/** Provides read/write access ot the "index"th element of the
		 *  array. */
		virtual	valuetype	&operator[](uint64_t index)=0;

		/** Writes a representation of the arraycollection to standard
		 *  output.
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	write();

		/** Writes a representation of the arraycollection to "out".
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the arraycollection to
		 *  standard output.
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  A wide string key or value is written in the character
		 *  set of the current locale.  An error is returned if a
		 *  character cannot be converted.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the arraycollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the collection
		 *  is written without indentation.
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  A wide string key or value is written in the character
		 *  set of the current locale.  An error is returned if a
		 *  character cannot be converted.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the arraycollection to
		 *  "out".
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  A wide string key or value is written in the character
		 *  set of the current locale.  An error is returned if a
		 *  character cannot be converted.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the arraycollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the collection
		 *  is written without indentation.
		 *
		 *  An int8_t value is written as its number where int8_t is
		 *  signed char.  Where int8_t is char, such as on Solaris
		 *  with _CHAR_IS_SIGNED, the char writer handles it, because
		 *  the writers take signed char.  It is then written as the
		 *  character with that byte value.  A digit is written bare,
		 *  and any other character as a JSON string.
		 *
		 *  A wide string key or value is written in the character
		 *  set of the current locale.  An error is returned if a
		 *  character cannot be converted.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(output *out, bool indent);
};

#include <rudiments/private/arraycollectioninlines.h>

#endif
