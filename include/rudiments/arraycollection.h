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
		virtual const char	*getType() const;

		/** Provides read/write access ot the "index"th element of the
		 *  array. */
		virtual	valuetype	&operator[](uint64_t index)=0;

		/** Provides read-only access ot the "index"th element of the
		 *  array. */
		virtual	valuetype	operator[](uint64_t index) const=0;

		/** Returns the number of elements in the array. */
		virtual	uint64_t	getLength() const=0;

		/** Writes a representation of the arraycollection to standard
		 *  output. */
		virtual	ssize_t	write() const;

		/** Writes a representation of the arraycollection to "out". */
		virtual	ssize_t	write(output *out) const;

		/** Writes a JSON representation of the arraycollection to
		 *  standard output. */
		virtual	ssize_t	writeJson() const;

		/** Writes a JSON representation of the arraycollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent) const;

		/** Writes a JSON representation of the arraycollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out) const;

		/** Writes a JSON representation of the arraycollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent) const;

		/** Writes an XML representation of the arraycollection to
		 *  standard output. */
		virtual	ssize_t	writeXml() const;

		/** Writes an XML representation of the arraycollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeXml(bool indent) const;

		/** Writes an XML representation of the arraycollection to
		 *  "out". */
		virtual	ssize_t	writeXml(output *out) const;

		/** Writes an XML representation of the arraycollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeXml(output *out, bool indent) const;
};

#include <rudiments/private/arraycollectioninlines.h>

#endif
