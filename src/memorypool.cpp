// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/memorypool.h>
#include <rudiments/linkedlist.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

//#define DEBUG_ALLOCATE 1
//#define DEBUG_DEALLOCATE 1
//#define DEBUG_RESIZE 1

#if defined(DEBUG_ALLOCATE) || \
		defined(DEBUG_DEALLOCATE) || \
		defined(DEBUG_RESIZE)
	#include <rudiments/stdio.h>
#endif

#define DEFAULT_INITIALSIZE 512
#define DEFAULT_INCREMENTSIZE 128
#define DEFAULT_RESIZEINTERVAL 100

class memorypoolbuffer {
	friend class memorypool;
	private:
			memorypoolbuffer(size_t size);
	public:
			~memorypoolbuffer();
	private:

		unsigned char	*_buffer;
		size_t		_size;
		size_t		_remaining;
		size_t		_position;
};

memorypoolbuffer::memorypoolbuffer(size_t size) {
	_buffer=new unsigned char[size];
	_size=size;
	_remaining=size;
	_position=0;
}

memorypoolbuffer::~memorypoolbuffer() {
	delete[] _buffer;
}

typedef	listnode<memorypoolbuffer *>	memorypoollistnode;
typedef	linkedlist<memorypoolbuffer *>	memorypoollist;

class memorypoolprivate {
	friend class memorypool;
	private:
		memorypoollist		_bufferlist;
		memorypoollistnode	*_first;

		size_t	_initialsize;
		size_t	_incrementsize;
		size_t	_resizeinterval;

		size_t	_clears;
		size_t	_total;
		size_t	_average;
};

// Pad to 8-byte boundary.  It's faster to access data aligned on a word
// boundary.  Some architectures (eg. sparc) require it and will throw a bus
// error if you don't.  Arguably we should make this optional for architectures
// that don't require it though, to conserve memory.
#define MEMORYPOOLPAD(a) ((8-(a%8))%8)

memorypool::memorypool() : object() {
	init(DEFAULT_INITIALSIZE,DEFAULT_INCREMENTSIZE,DEFAULT_RESIZEINTERVAL);
}

memorypool::memorypool(size_t initialsize,
			size_t incrementsize,
			size_t resizeinterval) : object() {
	init(initialsize,incrementsize,resizeinterval);
}

memorypool::memorypool(memorypool &m) : object() {
	init(m.pvt->_initialsize,m.pvt->_incrementsize,m.pvt->_resizeinterval);
}

memorypool &memorypool::operator=(memorypool &m) {
	if (this!=&m) {
		object::operator=(m);
		clear(true,m.pvt->_initialsize,
				m.pvt->_incrementsize,
				m.pvt->_resizeinterval);
	}
	return *this;
}

memorypool::~memorypool() {
	pvt->_bufferlist.setManageValues(true);
	delete pvt;
}

void memorypool::init(size_t initialsize,
			size_t incrementsize,
			size_t resizeinterval) {

	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (!incrementsize) {
		incrementsize=DEFAULT_INCREMENTSIZE;
	}
	if (!resizeinterval) {
		resizeinterval=DEFAULT_RESIZEINTERVAL;
	}

	pvt=new memorypoolprivate;
	pvt->_initialsize=initialsize+MEMORYPOOLPAD(initialsize);
	pvt->_incrementsize=incrementsize+MEMORYPOOLPAD(incrementsize);
	pvt->_resizeinterval=resizeinterval;

	pvt->_clears=0;
	pvt->_total=0;
	pvt->_average=0;

	pvt->_bufferlist.append(new memorypoolbuffer(pvt->_initialsize));
	pvt->_first=pvt->_bufferlist.getFirst();
}

size_t memorypool::getInitialSize() {
	return pvt->_initialsize;
}

size_t memorypool::getIncrementSize() {
	return pvt->_incrementsize;
}

size_t memorypool::getResizeInterval() {
	return pvt->_resizeinterval;
}

unsigned char *memorypool::allocate(size_t length) { 

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("allocate {\n");
	stdoutput.printf("	len  : %d",length);
	#endif

	length+=MEMORYPOOLPAD(length);

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf(" (%d)\n",length);
	uint32_t	counter=0;
	#endif

	// look for a node with enough memory remaining
	memorypoollistnode	*node=pvt->_first;
	memorypoolbuffer	*membuf;
	while (node) {

		membuf=node->getValue();
		if (membuf->_remaining>=length) {
			break;
		}
		node=node->getNext();

		#ifdef DEBUG_ALLOCATE
		counter++;
		#endif
	}

	#ifdef DEBUG_ALLOCATE
	if (node) {
		stdoutput.printf("	node : %d (of %d)\n",
					counter,pvt->_bufferlist.getLength());
	}
	#endif

	// if we didn't find a node with enough memory remaining
	// then create a new one at the end of the list
	if (!node) {

		// increase size by increments of at least 10% for better
		// performance and 10% max memory usage penalty - Claudio Freire
		size_t	incr=length;
		if (incr<pvt->_incrementsize) {
			incr=pvt->_incrementsize;
		}
		if (incr<(pvt->_total/10)) {
			incr=(pvt->_total/10);
			incr+=MEMORYPOOLPAD(incr);
		}
		membuf=new memorypoolbuffer(incr);
		pvt->_bufferlist.append(membuf);

		#ifdef DEBUG_ALLOCATE
		stdoutput.printf("\n	allocating new node "
						"(%d bytes)\n\n",incr);
		#endif
	}

	// add the length to the total
	pvt->_total+=length;

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("	total: %d\n",pvt->_total);
	#endif

	// get the buffer to return
	unsigned char	*buffer=membuf->_buffer+membuf->_position;

	// adjust position and bytes remaining in
	// the node that the buffer came from
	membuf->_position+=length;
	membuf->_remaining-=length;

	// balance the list...
	if (node!=pvt->_first) {

		memorypoolbuffer	*firstmembuf=pvt->_first->getValue();

		#ifdef DEBUG_ALLOCATE
		stdoutput.printf("\n	remain in this node : %d\n",
						membuf->_remaining);
		stdoutput.printf("	remain in first node: %d\n",
						firstmembuf->_remaining);
		#endif

		// If this node has 128 bytes more space remaining than the
		// first node has, and that is at least 10% more space
		// remaining than the first node has, then move it to the
		// front.  This tends to reorganize the list such that nodes
		// with more space remaining tend to be are nearer to the front
		// of the list and the next allocate has a better chance of
		// finding space without having to search too hard.  The
		// 128/10% hopes to prevent flip-flopping of nodes.
		if (membuf->_remaining>firstmembuf->_remaining) {
			size_t	difference=membuf->_remaining-
						firstmembuf->_remaining;
			if (!firstmembuf->_remaining ||
				(difference>128 &&
				difference>firstmembuf->_remaining/10)) {

				pvt->_bufferlist.moveBefore(pvt->_first,node);
				pvt->_first=pvt->_bufferlist.getFirst();

				#ifdef DEBUG_ALLOCATE
				stdoutput.printf("\n	moving node "
							"to front\n");
				#endif
			}
		}
	}

	#ifdef DEBUG_ALLOCATE
	stdoutput.printf("}\n");
	#endif

	// return the buffer
	return buffer;
}

void memorypool::clear() {
	clear(false,0,pvt->_incrementsize,pvt->_resizeinterval);
}

void memorypool::clear(size_t incrementsize,
			size_t resizeinterval) {
	clear(false,0,incrementsize,resizeinterval);
}

void memorypool::clear(size_t initialsize,
			size_t incrementsize,
			size_t resizeinterval) {
	clear(true,initialsize,incrementsize,resizeinterval);
}

void memorypool::clear(bool resetinitialsize,
				size_t initialsize,
				size_t incrementsize,
				size_t resizeinterval) {

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("clear {\n");
	#endif

	if (!initialsize) {
		initialsize=DEFAULT_INITIALSIZE;
	}
	if (!incrementsize) {
		incrementsize=DEFAULT_INCREMENTSIZE;
	}
	if (!resizeinterval) {
		resizeinterval=DEFAULT_RESIZEINTERVAL;
	}

	// reset sizes/interval
	if (resetinitialsize) {
		pvt->_initialsize=initialsize+MEMORYPOOLPAD(initialsize);
	}
	pvt->_incrementsize=incrementsize+MEMORYPOOLPAD(incrementsize);
	pvt->_resizeinterval=resizeinterval;

	// if the pool was unused during this iteration...
	// (a surprisingly common case)
	if (pvt->_bufferlist.getLength()==1 &&
			!pvt->_first->getValue()->_position) {

		#ifdef DEBUG_DEALLOCATE
		stdoutput.printf("	pool was unused\n");
		stdoutput.printf("}\n");
		#endif

		return;
	}

	// get the first buffer
	// (we'll need it later and pvt->_first is about to become invalid)
	memorypoolbuffer	*firstmembuf=pvt->_first->getValue();

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	clearing %d nodes\n",
					pvt->_bufferlist.getLength());
	#endif

	// delete all buffers except for the first one
	for (memorypoollistnode	*node=pvt->_first->getNext();
					node; node=node->getNext()) {
		delete node->getValue();
	}
	pvt->_bufferlist.clear();

	// bump clears counter
	pvt->_clears++;

	// update the running average
	pvt->_average-=pvt->_average/pvt->_clears;
	pvt->_average+=pvt->_total/pvt->_clears;

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	num: %d\n",pvt->_clears);
	stdoutput.printf("	avg: %d\n",pvt->_average);
	stdoutput.printf("	tot: %d\n",pvt->_total);
	#endif

	// if we force-resized the first buffer...
	if (resetinitialsize) {

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	force-resize {\n");
		stdoutput.printf("		current    : %d\n",
							firstmembuf->_size);
		stdoutput.printf("		new        : %d\n",
							pvt->_initialsize);
		#endif

		delete[] firstmembuf->_buffer;
		firstmembuf->_buffer=new unsigned char[pvt->_initialsize];
		firstmembuf->_size=pvt->_initialsize;

		// reset counters
		pvt->_clears=0;
		pvt->_average=0;

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	}\n");
		#endif

	} else
	// if it's time to re-evaluate and re-size the first buffer...
	// (_clears could be > _resizeinterval if _resizeinterval was reset)
	if (pvt->_clears>=pvt->_resizeinterval) {

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	resize {\n");
		stdoutput.printf("		current    : %d\n",
							firstmembuf->_size);
		stdoutput.printf("		average    : %d\n",
							pvt->_average);
		#endif

		// calculate the difference between the current size
		// of the first buffer and average total allocation
		// since the last resize
		size_t	difference=
			(pvt->_average>firstmembuf->_size)?
				pvt->_average-firstmembuf->_size:
				firstmembuf->_size-pvt->_average;

		// don't grow or shrink the buffer if the
		// average is within 10% of its current size
		if (difference>firstmembuf->_size-firstmembuf->_size/10) {

			// resize the first buffer to the average
			// total allocation since the last resize
			delete[] firstmembuf->_buffer;
			firstmembuf->_buffer=new unsigned char[pvt->_average];
			firstmembuf->_size=pvt->_average;

			#ifdef DEBUG_RESIZE
			stdoutput.printf("		resizing\n");
			#endif
		}
		#ifdef DEBUG_RESIZE
		else {
			stdoutput.printf("		not resizing\n");
		}
		#endif

		// reset counters
		pvt->_clears=0;
		pvt->_average=0;

		#ifdef DEBUG_RESIZE
		stdoutput.printf("	}\n");
		#endif
	}

	// reset position and remaining on the first node
	firstmembuf->_position=0;
	firstmembuf->_remaining=firstmembuf->_size;

	// reset total
	pvt->_total=0;

	// add back the first buffer
	pvt->_bufferlist.append(firstmembuf);
	pvt->_first=pvt->_bufferlist.getFirst();

	#ifdef DEBUG_DEALLOCATE
	stdoutput.printf("	node count: %d\n",pvt->_bufferlist.getLength());
	stdoutput.printf("}\n");
	#endif
}
