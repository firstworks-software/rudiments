// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COLLECTION_H
#define RUDIMENTS_COLLECTION_H

#include <rudiments/object.h>

/** The collection class is the parent class for all rudiments collections. */
class RUDIMENTS_DLLSPEC collection : public object {
	public:

		/** Creates an instance of the collection class. */
		collection();

		/** Creates an instance of the collection class that is a copy
		 *  of "c". */
		collection(const collection &c);

		/** Deletes this instance of the collection class. */
		virtual	~collection();

		virtual	void	setManageValues(bool manage);
		virtual	bool	getManageValues();

		virtual	void	setManageArrayValues(bool manage);
		virtual	bool	getManageArrayValues();

		virtual	void	setManageKeys(bool manage);
		virtual	bool	getManageKeys();

		virtual	void	setManageArrayKeys(bool manage);
		virtual	bool	getManageArrayKeys();

	#include <rudiments/private/collection.h>
};

#include <rudiments/private/collectioninlines.h>

#endif
