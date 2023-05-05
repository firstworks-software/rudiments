// Copyright (c) 1999-2018 David Muse
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

		/** Sets the value stored in this instance to "v". */
		virtual	void		setValue(valuetype v)=0;

		/** Returns the value stored in this instance.  Returns NULL or
		 *  0 if no value has been stored. */
		virtual	valuetype	getValue()=0;

		/** Returns a reference to the value stored in this instance. */
		virtual	valuetype	&getReference()=0;

		/** Writes a representation of the scalarcollection to
		 *  standard output. */
		virtual	ssize_t	write();

		/** Writes a representation of the scalarcollection to "out". */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the scalarcollection to
		 *  standard output. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the scalarcollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the scalarcollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the scalarcollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent);
};

#include <rudiments/private/scalarcollectioninlines.h>

#endif
