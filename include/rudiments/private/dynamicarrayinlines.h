// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray() : arraycollection<valuetype>() {
	init(128,32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(uint64_t initiallength,
					uint64_t incrementlength) :
						arraycollection<valuetype>() {
	init((initiallength)?initiallength:128,
				(incrementlength)?incrementlength:32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(const dynamicarray<valuetype> &v) :
						arraycollection<valuetype>(v) {
	init(v.initlen,v.inclen);
	clone(&v);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(const arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	init(128,32);
	clone(&v);
}

template< class valuetype >
inline
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					const dynamicarray<valuetype> &v) {
	if (this!=&v) {
		clear(v.initlen,v.inclen);
		arraycollection<valuetype>::operator=(v);
		init(v.initlen,v.inclen);
		clone(&v);
	}
	return *this;
}

template< class valuetype >
inline
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					const arraycollection<valuetype> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		init(initlen,inclen);
		clone(&v);
	}
	return *this;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::init(uint64_t initiallength,
					uint64_t incrementlength) {
	extents.setManageArrayValues(true);
	totallen=0;
	lastlen=0;
	initlen=initiallength;
	inclen=incrementlength;
	extend(initiallength);
	curext=extents.getFirst();
	curind=0;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clone(const dynamicarray<valuetype> *v) {

	// extend storage to fit (do this before setting length)
	extend(v->lastlen);

	// clone lengths and positions
	totallen=v->totallen;
	lastlen=v->lastlen;
	initlen=v->initlen;
	inclen=v->inclen;

	// clone the data
	for (uint64_t i=0; i<v->getLength(); i++) {
		find(i)=v->find(i);
	}

	// clone positions
	curind=v->curind;
	curext=extents.getFirst();
	for (uint64_t eind=0; eind<curind; eind++) {
		curext=curext->getNext();
	}
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clone(const arraycollection<valuetype> *v) {
	lastlen=v->getLength();
	for (uint64_t i=0; i<lastlen; i++) {
		find(i)=(*v)[i];
	}
}

template< class valuetype >
inline
dynamicarray<valuetype>::~dynamicarray() {
}

template< class valuetype >
inline
valuetype &dynamicarray<valuetype>::operator[](uint64_t index) {
	extend(index+1);
	if (index>=lastlen) {
		lastlen=index+1;
	}
	return find(index);
}

template< class valuetype >
inline
valuetype dynamicarray<valuetype>::operator[](uint64_t index) const {
	if (index>=lastlen) {
		return *((valuetype *)NULL);
	}
	return find(index);
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getInitialLength() const {
	return initlen;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getIncrementLength() const {
	return inclen;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getLength() const {
	return lastlen;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::extend(uint64_t length) {
	uint64_t	inc=(extents.getLength())?inclen:initlen;
	while (totallen<length) {
		valuetype	*newext=new valuetype[inc];
		extents.append(newext);
		totallen+=inc;
		inc=inclen;
	}
}

template< class valuetype >
inline
size_t dynamicarray<valuetype>::findExtentStartIndex(uint64_t index) const {

	// move to the extent that contains the specified index
	// (also calculate the index of the first element of the extent)
	size_t	eind;
	if (index<initlen) {
		curext=extents.getFirst();
		curind=0;
		eind=0;
	} else {
		uint64_t	targetind=(index-initlen+inclen)/inclen;
		while (curind>targetind) {
			curext=curext->getPrevious();
			curind--;
		}
		while (curind<targetind) {
			curext=curext->getNext();
			curind++;
		}
		eind=initlen+inclen*(curind-1);
	}
	return eind;
}

template< class valuetype >
inline
valuetype &dynamicarray<valuetype>::find(uint64_t index) {
	// Don't be tempted to do replace [index-eind] with
	// [index-findExtentStartIndex(index)], as that somehow
	// breaks everything, at least with gcc-11.
	// I imagine that findExtendStartIndex() being inline and const, but
	// also modifying mutable variables, somehow confuses the optimizer.
	size_t eind=findExtentStartIndex(index);
	return curext->getValue()[index-eind];
}

template< class valuetype >
inline
valuetype dynamicarray<valuetype>::find(uint64_t index) const {
	// see note above...
	size_t eind=findExtentStartIndex(index);
	return curext->getValue()[index-eind];
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clear() {
	clear(initlen,inclen);
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clear(uint64_t initiallength,
					uint64_t incrementlength) {

	if (initiallength==initlen) {

		// remove all but the first extent
		curext=extents.getLast();
		while (curext!=extents.getFirst()) {
			listnode<valuetype *>	*prev=curext->getPrevious();
			extents.remove(curext);
 			curext=prev;
		}

		// reinit first extent
		valuetype	*ext=curext->getValue();
		for (uint64_t i=0; i<initlen; i++) {
			ext[i].~valuetype();
			new(&(ext[i])) valuetype;
		}

	} else {

		// reset the initial length
		initlen=initiallength;

		// remove all extents
		extents.clear();

		// reinit first extent
		valuetype	*newext=new valuetype[initlen];
		extents.append(newext);
		curext=extents.getFirst();
	}

	// reset the incremental length
	inclen=incrementlength;

	// reset lengths
	totallen=initlen;
	lastlen=0;

	// reset current extent index
	curind=0;
}
