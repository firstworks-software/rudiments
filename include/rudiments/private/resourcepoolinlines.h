// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
resourcepool<valuetype>::resourcepool() : minval(0), maxval(10),
						growby(1), total(0),
						initialized(false),
						mtx(NULL) {
}

template <class valuetype>
inline
resourcepool<valuetype>::~resourcepool() {
	clear();
}

template <class valuetype>
inline
void resourcepool<valuetype>::setMin(uint64_t min) {
	minval=min;
	if (maxval<min) {
		maxval=min;
	}
}

template <class valuetype>
inline
uint64_t resourcepool<valuetype>::getMin() {
	return minval;
}

template <class valuetype>
inline
void resourcepool<valuetype>::setMax(uint64_t max) {
	maxval=max;
}

template <class valuetype>
inline
uint64_t resourcepool<valuetype>::getMax() {
	return maxval;
}

template <class valuetype>
inline
void resourcepool<valuetype>::setGrowBy(uint64_t growby) {
	this->growby=(!growby)?1:growby;
}

template <class valuetype>
inline
uint64_t resourcepool<valuetype>::getGrowBy() {
	return growby;
}

template <class valuetype>
inline
bool resourcepool<valuetype>::create() {

	// lock mutex
	if (mtx && !mtx->lock()) {
		return false;
	}

	for (uint64_t i=0; i<minval; i++) {
		valuetype	*v=createResource();
		if (!v) {
			clearDelegate();

			// unlock mutex
			if (mtx) {
				mtx->unlock();
			}

			return false;
		}
		initiallist.append(v);
	}
	total=minval;
	initialized=true;

	// unlock mutex
	if (mtx) {
		mtx->unlock();
	}

	return true;
}

template <class valuetype>
inline
bool resourcepool<valuetype>::clear() {

	// lock mutex
	if (mtx && !mtx->lock()) {
		return false;
	}

	clearDelegate();

	// unlock mutex
	if (mtx) {
		mtx->unlock();
	}
	return true;
}

template <class valuetype>
inline
bool resourcepool<valuetype>::reset() {

	// lock mutex
	if (mtx && !mtx->lock()) {
		return false;
	}

	clearDelegate();
	minval=0;
	maxval=10;
	growby=1;

	// unlock mutex
	if (mtx) {
		mtx->unlock();
	}
	return true;
}

template <class valuetype>
inline
void resourcepool<valuetype>::clearDelegate() {

	initiallist.setManageValues(true);
	initiallist.clear();
	initiallist.setManageValues(false);

	ondemandlist.setManageValues(true);
	ondemandlist.clear();
	ondemandlist.setManageValues(false);

	initialized=false;
}

template <class valuetype>
inline
valuetype *resourcepool<valuetype>::borrowResource() {

	// lock mutex
	if (mtx && !mtx->lock()) {
		return NULL;
	}

	// if we have initial resources available to loan out,
	// then loan one out
	if (initiallist.getCount()) {

		// the list contains resources available to be loaned out and
		// the tree contains resources that have been loaned out,
		// move the resource from the list to the tree and return it
		listnode<valuetype *>	*node=initiallist.getFirst();
		valuetype		*v=node->getValue();
		initialtree.insert(v);
		initiallist.remove(node);

		// unlock mutex
		if (mtx) {
			mtx->unlock();
		}

		// return the resource
		return v;

	}

	// if we don't have any on-demand resources available to loan out,
	// then grow, if we can
	if (!ondemandlist.getCount()) {
		for (uint64_t i=0; i<growby && total<maxval; i++) {
			valuetype	*v=createResource();
			ondemandlist.append(v);
			total++;
		}
	}

	// if we have on-demand resources available to loan out,
	// then loan one out
	if (ondemandlist.getCount()) {

		// the list contains resources available to be loaned out and
		// the tree contains resources that have been loaned out,
		// move the resource from the list to the tree and return it
		listnode<valuetype *>	*node=ondemandlist.getFirst();
		valuetype		*v=node->getValue();
		ondemandtree.insert(v);
		ondemandlist.remove(node);

		// unlock mutex
		if (mtx) {
			mtx->unlock();
		}

		// return the resource
		return v;
	}

	// unlock mutex
	if (mtx) {
		mtx->unlock();
	}

	// if no resources are available then return NULL
	return NULL;
}

template <class valuetype>
inline
bool resourcepool<valuetype>::returnResource(valuetype *resource) {

	if (mtx) {
		if (!mtx->lock()) {
			return false;
		}
	}

	// if this is one of the initial resources...
	treenode<valuetype *>	*node=initialtree.find(resource);
	if (node) {

		// the tree contains resources that have been loaned out and
		// the list contains resources available to be loaned out,
		// move the resource from the tree to the list
		initiallist.append(node->getValue());
		initialtree.remove(node);

	} else {

		// if this is one of the on-demand resources...
		treenode<valuetype *>	*node=ondemandtree.find(resource);
		if (node) {

			// the tree contains resources that have been loaned
			// out and the list contains resources available to be
			// loaned out, delete the resource and remove its tree
			// node
			delete node->getValue();
			ondemandtree.remove(node);
		}
	}

	if (mtx) {
		if (!mtx->unlock()) {
			return false;
		}
	}
	return true;
}

template <class valuetype>
inline
valuetype *resourcepool<valuetype>::createResource() {
	return new valuetype;
}

template <class valuetype>
inline
void resourcepool<valuetype>::setMutex(threadmutex *mtx) {
	this->mtx=mtx;
}

template <class valuetype>
inline
uint64_t resourcepool<valuetype>::getAvailableInitialResourceCount() {
	return initiallist.getCount();
}


template <class valuetype>
inline
uint64_t resourcepool<valuetype>::getAvailableOnDemandResourceCount() {
	return ondemandlist.getCount();
}
