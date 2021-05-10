// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTCOLLECTION_H
#define RUDIMENTS_LISTCOLLECTION_H

#include <rudiments/nodecollection.h>

/** The listnode class is the parent class for the node contained by all
 *  rudiments listnode-based collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC listnode : public nodecollectionnode<valuetype> {
	public:

		/** Creates an instance of the listnode class. */
		listnode() : nodecollectionnode<valuetype>() {};

		/** Creates an instance of the listnode class. */
		listnode(const collection &c) :
				nodecollectionnode<valuetype>(c) {};

		/** Deletes this instance of the listnode class. */
		virtual	~listnode() {};

		virtual	void		setValue(valuetype value)=0;
		virtual	valuetype	getValue() const=0;

		virtual	int32_t	compare(valuetype value) const=0;
		virtual	int32_t	compare(listnode<valuetype> *peer) const=0;
		virtual	listnode<valuetype>	*getPrevious()=0;
		virtual	listnode<valuetype>	*getNext()=0;
		virtual	void	setNext(listnode<valuetype> *next)=0;
		virtual	void	setPrevious(listnode<valuetype> *previous)=0;
};

/** The listcollection class is the parent class for all rudiments list
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC listcollection : public nodecollection {
	public:

		/** Creates an instance of the listcollection class. */
		listcollection() : nodecollection() {};

		/** Creates an instance of the listcollection class. */
		listcollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~listcollection() {};

		virtual	uint64_t	getLength() const=0;

		virtual	listnode<valuetype>	*getFirst()=0;

		virtual	listnode<valuetype>	*getNext(
						listnode<valuetype> *node)=0;

		virtual	listnode<valuetype>	*find(valuetype value)=0;
		virtual	listnode<valuetype>	*find(
						listnode<valuetype> *startnode,
						valuetype value)=0;

		virtual	void	clear()=0;

		virtual	void	print() const=0;
		virtual	void	print(uint64_t count) const=0;
};

#endif
