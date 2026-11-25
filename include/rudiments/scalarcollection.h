// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SCALARCOLLECTION_H
#define RUDIMENTS_SCALARCOLLECTION_H

#include <rudiments/collection.h>
#include <rudiments/output.h>

/** The scalarcollection class is the parent class for all rudiments scalar
 *  collections. */
template <class valuetype>
class scalarcollection : public collection {
	public:
		/** Returns "scalar". */
		virtual const char	*getType();

		/** Returns 1. */
		virtual uint64_t	getCount();

		/** Sets the value stored in this instance to "v".
		 *
		 *  If setManageValues(true) or setManageArrayValues(true) has
		 *  been called, the previous value is deleted first, unless it
		 *  is the same pointer as "v". */
		virtual	void		setValue(valuetype v)=0;

		/** Returns the value stored in this instance.  Returns NULL or
		 *  0 if no value has been stored. */
		virtual	valuetype	getValue()=0;

		/** Returns a reference to the value stored in this instance.
		 *
		 *  Note that a value assigned through the reference doesn't
		 *  delete the previous value, even if setManageValues(true) or
		 *  setManageArrayValues(true) has been called.  The caller must
		 *  delete it, if necessary. */
		virtual	valuetype	&getReference()=0;

		/** Writes a representation of the scalarcollection to
		 *  standard output.
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

		/** Writes a representation of the scalarcollection to "out".
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

		/** Writes a JSON representation of the scalarcollection to
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

		/** Writes a JSON representation of the scalarcollection to
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

		/** Writes a JSON representation of the scalarcollection to
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

		/** Writes a JSON representation of the scalarcollection to
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

#include <rudiments/private/scalarcollectioninlines.h>

#endif
