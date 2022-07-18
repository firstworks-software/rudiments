// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RESOURCEPOOL_H
#define RUDIMENTS_RESOURCEPOOL_H

#include <rudiments/private/resourcepoolincludes.h>

/** The resourcepool class allows you to ... */
template <class valuetype>
class resourcepool {
	public:
		/** Creates an empty instance of the resourcepool class. */
		resourcepool();

		/** Creates an instance of the resourcepool class
		 *  that is a copy of "a". */
		resourcepool(const resourcepool<valuetype> &r);

		/** Makes this instance of the resourcepool class
		 *  identical to "a". */
		resourcepool<valuetype>	&operator=(
					const resourcepool<valuetype> &r);

		/** Deletes this instance of the resourcepool class and all
		 *  resources in the pool. */
		virtual ~resourcepool();

		/** Sets the number of resources that will be created when the
		 *  pool is initialized.
		 *
		 *  Defaults to 0. */
		void	setMin(uint64_t min);

		/** Returns the number of resources that will be created when
		 *  the pool is initialized. */
		uint64_t	getMin() const;

		/** Sets the maximum number of resources that the pool can grow
		 *  to on-demand.
		 *
		 *  Defaults to 10. */
		void	setMax(uint64_t max);

		/** Returns the number of resources that the pool can grow to
		 *  on-demand. */
		uint64_t	getMax() const;

		/** Sets the number of resources that will be created when the
		 *  pool grows on-demand.
		 *
		 *  Defaults to 1. */
		void	setGrowBy(uint64_t growby);

		/** Returns the number of resources that will be created when
		 *  the pool grows on-demand. */
		uint64_t	getGrowBy() const;

		/** Initializes the resourcepool, creating the number of
		 *  resources configured by setMin().
		 *
		 *  Returns true on success and false on failure. */
		bool	initialize();

		/** Clears the resourcepool, deleting all resources in the
		 *  pool. */
		void	clear();

		/** Returns a pointer to an instance of "valuetype" from the
		 *  pool, or NULL if none are available or a mutex lock
		 *  failed. */
		valuetype	*borrowResource();

		/** Returns "resource" to the pool.  Returns true on success
		 *  and false if a mutex lock failed. */
		bool	returnResource(valuetype *resource);

		/** Sets a mutex to use when borrowing or returning
		 *  resources.  To not use a mutex set "mtx" to NULL.
		 *  Defaults to NULL. */
		void	setMutex(threadmutex *mtx);

		/** Returns the number of resources that are available in the
		 *  pool without creating any resources on-demand. */
		uint64_t	getAvailableInitialResourceCount();

		/** Returns the number of resources that have been created
		 *  on-demand but have not been loaned out. */
		uint64_t	getAvailableOnDemandResourceCount();

	#include <rudiments/private/resourcepool.h>
};

#include <rudiments/private/resourcepoolinlines.h>

#endif
