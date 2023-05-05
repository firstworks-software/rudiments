// Copyright (c) 1999-2018 David Muse
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
		 *  output. */
		virtual	ssize_t	write();

		/** Writes a representation of the arraycollection to "out". */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the arraycollection to
		 *  standard output. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the arraycollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the arraycollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the arraycollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent);
};

#include <rudiments/private/arraycollectioninlines.h>

#endif
