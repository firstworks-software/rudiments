// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray() {
	init(128,32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(uint64_t initiallength,
					uint64_t incrementlength) {
	init((initiallength)?initiallength:128,
				(incrementlength)?incrementlength:32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(const dynamicarray<valuetype> &v) {
	init(v.initial,v.extlength);
	dynamicarrayClone(v);
}

template< class valuetype >
inline
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					const dynamicarray<valuetype> &v) {
	if (this!=&v) {
		clearExtentList();
		init(v.initial,v.extlength);
		dynamicarrayClone(v);
	}
	return *this;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::init(uint64_t initiallength,
					uint64_t incrementlength) {
	length=0;
	len=0;
	initial=initiallength;
	extlength=incrementlength;
	extend(initiallength);
	curext=extents.getFirst();
	curind=0;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::dynamicarrayClone(
				const dynamicarray<valuetype> &v) {

	// extend storage to fit (do this before setting length)
	extend(v.len);

	// clone lengths and positions
	length=v.length;
	len=v.len;
	initial=v.initial;
	extlength=v.extlength;

	// clone the data
	for (uint64_t i=0; i<v.getLength(); i++) {

		// Why not just:
		//	this[i]=v[i];
		//
		// Some compilers don't allow v[] because the operator[] method
		// isn't const, but v is.
		//
		// Also, some compilers get confused and think that
		//	this[i]=v[i]
		//		means
		//	(this[i])->operator=(v[i])
		// and no carefully placed parentheses help.
		//
		// This silliness sorts both issues out.
		find(i)=((dynamicarray<valuetype> *)&v)->find(i);
	}

	// clone positions
	curind=v.curind;
	curext=extents.getFirst();
	for (uint64_t eind=0; eind<curind; eind++) {
		curext=curext->getNext();
	}
}

template< class valuetype >
inline
dynamicarray<valuetype>::~dynamicarray() {
	clearExtentList();
}

template< class valuetype >
inline
valuetype &dynamicarray<valuetype>::operator[](uint64_t index) {
	extend(index+1);
	if (index>=len) {
		len=index+1;
	}
	return find(index);
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getInitialLength() const {
	return initial;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getIncrementLength() const {
	return extlength;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getLength() const {
	return len;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::extend(uint64_t length) {
	uint64_t	inc=(extents.getLength())?extlength:initial;
	while (this->length<length) {
		valuetype	*newext=new valuetype[inc];
		extents.append(newext);
		this->length=this->length+inc;
		inc=extlength;
	}
}

template< class valuetype >
inline
valuetype &dynamicarray<valuetype>::find(uint64_t index) {

	// move to the extent that contains the specified index
	// (also calculate the index of the first element of the extent)
	size_t	eind;
	if (index<initial) {
		curext=extents.getFirst();
		curind=0;
		eind=0;
	} else {
		uint64_t	targetind=(index-initial+extlength)/extlength;
		while (curind>targetind) {
			curext=curext->getPrevious();
			curind--;
		}
		while (curind<targetind) {
			curext=curext->getNext();
			curind++;
		}
		eind=initial+extlength*(curind-1);
	}

	// return the value
	return curext->getValue()[index-eind];
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clearExtentList() {
	curext=extents.getFirst();
	while (curext) {
		linkedlistnode<valuetype *>	*next=curext->getNext();
		valuetype			*ext=curext->getValue();
		delete[] ext;
		extents.remove(curext);
 		curext=next;
	}
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clear() {
	clear(initial,extlength);
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clear(uint64_t initiallength,
					uint64_t incrementlength) {

	// remove all but the first extent
	curext=extents.getLast();
	while (curext!=extents.getFirst()) {
		linkedlistnode<valuetype *>	*prev=curext->getPrevious();
		valuetype			*ext=curext->getValue();
		delete[] ext;
		extents.remove(curext);
 		curext=prev;
	}

	// reset the initial/incremental lengths
	initial=initiallength;
	extlength=incrementlength;

	// reinit first extent
	valuetype	*ext=curext->getValue();
	for (uint64_t i=0; i<initial; i++) {
		ext[i].~valuetype();
		new(&(ext[i])) valuetype;
	}

	// reset lengths and positions
	length=0;
	len=0;
	curind=0;
}
