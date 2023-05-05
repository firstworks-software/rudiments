// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray() : arraycollection<valuetype>() {
	construct(128,32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(uint64_t initialcount,
					uint64_t incrementcount) :
						arraycollection<valuetype>() {
	construct((initialcount)?initialcount:128,
				(incrementcount)?incrementcount:32);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(dynamicarray<valuetype> &v) :
						arraycollection<valuetype>(v) {
	construct(v.initcount,v.inccount);
	clone(v);
}

template< class valuetype >
inline
dynamicarray<valuetype>::dynamicarray(arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	construct(128,32);
	clone(v);
}

template< class valuetype >
inline
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					dynamicarray<valuetype> &v) {
	if (this!=&v) {
		clear(v.initcount,v.inccount);
		arraycollection<valuetype>::operator=(v);
		construct(v.initcount,v.inccount);
		clone(v);
	}
	return *this;
}

template< class valuetype >
inline
dynamicarray<valuetype> &dynamicarray<valuetype>::operator=(
					arraycollection<valuetype> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		construct(initcount,inccount);
		clone(v);
	}
	return *this;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::construct(uint64_t initialcount,
					uint64_t incrementcount) {
	extents.setManageArrayValues(true);
	totalcount=0;
	lastcount=0;
	initcount=initialcount;
	inccount=incrementcount;
	extend(initialcount);
	curext=extents.getFirst();
	curind=0;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::clone(dynamicarray<valuetype> &v) {

	// extend storage to fit (do this before setting count)
	extend(v.lastcount);

	// clone counts and positions
	totalcount=v.totalcount;
	lastcount=v.lastcount;
	initcount=v.initcount;
	inccount=v.inccount;

	// clone the data
	for (uint64_t i=0; i<v.getCount(); i++) {
		find(i)=node_duplicate_value(&(v.find(i)),
					this->getManageValues(),
					this->getManageArrayValues());
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
void dynamicarray<valuetype>::clone(arraycollection<valuetype> &v) {
	lastcount=v.getCount();
	for (uint64_t i=0; i<lastcount; i++) {
		find(i)=node_duplicate_value(&(v[i]),
					this->getManageValues(),
					this->getManageArrayValues());
	}
}

template< class valuetype >
inline
dynamicarray<valuetype>::~dynamicarray() {
	deleteManagedValues();
}

template< class valuetype >
inline
valuetype &dynamicarray<valuetype>::operator[](uint64_t index) {
	extend(index+1);
	if (index>=lastcount) {
		lastcount=index+1;
	}
	// I once had (semi-clever) bounds-checking code here like:
	//
	// if (index>=lastcount) {
	//	return *((valuetype *)NULL);
	// }
	//
	// which would successfully return a NULL/0 if valuetype was a pointer,
	// but would throw warnings on some platforms if valuetype isn't a
	// pointer, and would probably crash on those platforms if it managed
	// to run.
	//
	// It's (apparently) conventional for the operator[] not to do any
	// bounds checking, not even throw any exceptions, and and just attempt
	// to access and return the requested index, possibly crashing in the
	// process.  When a program accesses via the [] opertator it's telling
	// the compiler "trust me, I know what I'm doing".
	//
	// So, for now, I removed that code above and we'll just let whatever
	// happens, happen.
	return find(index);
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getInitialCount() {
	return initcount;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getIncrementCount() {
	return inccount;
}

template< class valuetype >
inline
uint64_t dynamicarray<valuetype>::getCount() {
	return lastcount;
}

template< class valuetype >
inline
void dynamicarray<valuetype>::extend(uint64_t count) {
	uint64_t	inc=(extents.getCount())?inccount:initcount;
	while (totalcount<count) {
		valuetype	*newext=new valuetype[inc];
		extents.append(newext);
		totalcount+=inc;
		inc=inccount;
	}
}

template< class valuetype >
inline
size_t dynamicarray<valuetype>::findExtentStartIndex(uint64_t index) {

	// move to the extent that contains the specified index
	// (also calculate the index of the first element of the extent)
	size_t	eind;
	if (index<initcount) {
		curext=extents.getFirst();
		curind=0;
		eind=0;
	} else {
		uint64_t	targetind=(index-initcount+inccount)/inccount;
		while (curind>targetind) {
			curext=curext->getPrevious();
			curind--;
		}
		while (curind<targetind) {
			curext=curext->getNext();
			curind++;
		}
		eind=initcount+inccount*(curind-1);
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
	// FIXME: now that the methods aren't const and modifying mutables,
	// it might work
	size_t eind=findExtentStartIndex(index);
	return curext->getValue()[index-eind];
}

template< class valuetype >
inline
bool dynamicarray<valuetype>::clear() {
	return clear(initcount,inccount);
}

template< class valuetype >
inline
void dynamicarray<valuetype>::deleteManagedValues() {

	// delete managed values in all extents
	if (this->getManageValues() || this->getManageArrayValues()) {
		uint64_t	i=0;
		uint64_t	count=initcount;
		for (curext=extents.getFirst(); i<lastcount && curext;
						curext=curext->getNext()) {
			valuetype	*data=curext->getValue();
			for (uint64_t j=0; i<lastcount && j<count; j++) {
				node_delete_value(&(data[j]),
						this->getManageValues(),
						this->getManageArrayValues());
				node_zero_value(&(data[j]));
				i++;
			}
			count=inccount;
		}
	}
}

template< class valuetype >
inline
bool dynamicarray<valuetype>::clear(uint64_t initialcount,
					uint64_t incrementcount) {

	deleteManagedValues();

	if (initialcount==initcount) {

		// remove all but the first extent
		curext=extents.getLast();
		while (curext!=extents.getFirst()) {
			listnode<valuetype *>	*prev=curext->getPrevious();
			extents.remove(curext);
 			curext=prev;
		}

		// reinit first extent
		valuetype	*ext=curext->getValue();
		for (uint64_t i=0; i<initcount; i++) {
			// gcc 2.91.66 on redhat 6.2 throws an internal
			// compiler error unless we use a pointer to call
			// the destructor.  No other compilers appear to have
			// this problem.
			#if __GNUC__ == 2 && __GNUC_MINOR__ == 91
				valuetype	*v=&(ext[i]);
				v->~valuetype();
			#else
				ext[i].~valuetype();
			#endif
			new(&(ext[i])) valuetype;
		}

	} else {

		// reset the initial count
		initcount=initialcount;

		// remove all extents
		extents.clear();

		// reinit first extent
		valuetype	*newext=new valuetype[initcount];
		extents.append(newext);
		curext=extents.getFirst();
	}

	// reset the incremental count
	inccount=incrementcount;

	// reset counts
	totalcount=initcount;
	lastcount=0;

	// reset current extent index
	curind=0;

	return true;
}
