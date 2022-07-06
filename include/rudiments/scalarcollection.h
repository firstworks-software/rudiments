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
		/** Sets the value stored in this instance to "v". */
		virtual	void		setValue(valuetype v)=0;

		/** Returns the value stored in this intance.  Returns NULL or
		 *  0 if no value has been stored. */
		virtual	valuetype	getValue() const=0;

		/** Writes a representation of the scalarcollection to
		 *  stdout. */
		virtual	ssize_t	write() const;

		/** Writes a representation of the scalarcollection to "out". */
		virtual	ssize_t	write(output *out) const;

		/** Writes a JSON representation of the scalarcollection to
		 *  stdout. */
		virtual	ssize_t	writeJson() const;

		/** Writes a JSON representation of the scalarcollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out) const;

		/** Writes an XML representation of the scalarcollection to
		 *  stdout. */
		virtual	ssize_t	writeXml() const;

		/** Writes an XML representation of the scalarcollection to
		 *  "out". */
		virtual	ssize_t	writeXml(output *out) const;
};

#include <rudiments/private/scalarcollectioninlines.h>

#endif
