// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARYCOLLECTION_H
#define RUDIMENTS_DICTIONARYCOLLECTION_H

#include <rudiments/collection.h>

/** The dictionarycollection class is the parent class for all rudiments
 *  dictionary collections. */
template<class keytype, class valuetype>
class RUDIMENTS_DLLSPEC dictionarycollection : public collection {
	public:

		/** Creates an instance of the dictionarycollection class. */
		dictionarycollection() : collection() {};

		/** Creates an instance of the dictionarycollection class. */
		dictionarycollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~dictionarycollection() {};

		virtual	void	setValue(keytype key, valuetype value)=0;
		virtual	void	setValues(keytype *key, valuetype *value)=0;
		virtual	void	setValues(keytype const *key,
						valuetype const *value)=0;

		virtual	void	setValues(keytype *key,
						valuetype *value,
						uint64_t count)=0;
		virtual	void	setValues(keytype const *key,
						valuetype const *value,
						uint64_t count)=0;
		virtual	valuetype	getValue(keytype key)=0;
		virtual	bool		remove(keytype key)=0;
		virtual	void		clear()=0;
		virtual	void		print()=0;
};

#endif
