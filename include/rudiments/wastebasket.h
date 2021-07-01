// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WASTEBASKET_H
#define RUDIMENTS_WASTEBASKET_H

#include <rudiments/private/wastebasketincludes.h>

/** The wastebasket class ... */
class RUDIMENTS_DLLSPEC wastebasket : public object {
	public:

		/** Creates an instance of the wastebasket class. */
		wastebasket();

		/** Deletes this instance of the wastebasket class. */
		virtual	~wastebasket();

		void	attachObject(object *o);
		void	attachCollectionWithValues(collection *c);
		void	attachCollectionWithArrayValues(collection *c);
		void	attachCollectionWithKeys(collection *c);
		void	attachCollectionWithArrayKeys(collection *c);
		void	attachCollectionWithArrayKeysAndValues(collection *c);
		void	attachCollectionWithKeysAndArrayValues(collection *c);

		void	empty();

	#include <rudiments/private/wastebasket.h>
};

#endif
