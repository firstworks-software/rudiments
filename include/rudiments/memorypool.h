// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYPOOL_H
#define RUDIMENTS_MEMORYPOOL_H

#include <rudiments/private/memorypoolincludes.h>

/** The memorypool class provides methods for creating and using a memory pool.
 *
 *  If you have an iterative process that requires variable amounts of memory
 *  for each iteration, using a memory pool can perform better than allocating
 *  memory on-demand and consume less memory than allocating static buffers
 *  that are large enough to accommodate the maximum amount of data you may
 *  have to store. */
class RUDIMENTS_DLLSPEC memorypool : public object {
	public:
		/** Creates a memory pool of initial size 512 bytes.
		 * 
		 *  When the pool needs to grow, it will grow by at least
		 *  128 bytes.  If more than 128 bytes are requested, it will
		 *  grow by that amount instead.
		 * 
		 *  When deallocate() has been called 100 times,
		 *  it will evaluate the average amount of memory
		 *  allocated (since the last time it did this) and
		 *  resize the initial buffer size to this size. */
		memorypool();

		/** Creates a memory pool of initial size "initialsize".
		 * 
		 *  When the pool needs to grow, it will grow by at least
		 *  "incrementsize" bytes.  If more than "incrementsize" bytes
		 *  are requested, it will grow by that amount instead.
		 * 
		 *  When deallocate() has been called "resizeinterval" times,
		 *  it will evaluate the average amount of memory
		 *  allocated (since the last time it did this) and
		 *  resize the initial buffer size to this size. */
		memorypool(size_t initialsize,
				size_t incrementsize,
				size_t resizeinterval);

		/** Deletes this instance of the memorypool class. */
		~memorypool();

		/** Returns the initial size of the pool. */
		size_t	getInitialSize();

		/** Returns the size that the pool will grow by when it needs
		 *  to grow. */
		size_t	getIncrementSize();

		/** Returns the pool's resize interval. */
		size_t	getResizeInterval();

		/** Returns a pointer to a contiguous block of "size"
		 *  bytes in the pool.  The pool will grow as necessary
		 *  to accomodate allocations. */
		unsigned char	*allocate(size_t size);

		/** Shrinks the pool back down to it's initial size
		 *  and frees all previously allocated blocks.
		 *
		 *  When clear() has been called "resizeinterval"
		 *  times (see constructor), it evaluates the average amount
		 *  of memory allocated (since the last time it did this) and
		 *  resizes the initial buffer size to this size. */
		void	clear();

		/** Shrinks the pool back down to it's initial size
		 *  and frees all previously allocated blocks.
		 *
		 *  Also resets the increment size and the resize interval to
		 *  the specified values.
		 *
		 *  When clear() has been called "resizeinterval"
		 *  times (see constructor), it evaluates the average amount
		 *  of memory allocated (since the last time it did this) and
		 *  resizes the initial buffer size to this size. */
		void	clear(size_t incrementsize,
				size_t resizeinterval);

		/** Shrinks the pool back down to it's initial size
		 *  and frees all previously allocated blocks.
		 *
		 *  Also resets the increment size and the resize interval to
		 *  the specified values, and immediately resets the initial
		 *  buffer size to initialsize. */
		void	clear(size_t initialsize,
				size_t incrementsize,
				size_t resizeinterval);

	#include <rudiments/private/memorypool.h>
};

#endif
