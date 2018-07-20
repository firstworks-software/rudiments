// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#if defined(USE_MAP)
	#include <map>
#elif defined(USE_DICTIONARY)
	#include <rudiments/dictionary.h>
#endif
#include <rudiments/stdio.h>
#include "../test/test.cpp"

#define ITEMS 1024*100
#define ITERS 80

int main(int argc, char **argv) {

#if defined(USE_MAP)
	std::map<uint64_t,uint64_t>	d;
#elif defined(USE_DICTIONARY)
	dictionary<uint64_t,uint64_t>	d;
	d.setTrackInsertionOrder(false);
#endif
	datetime	start;
	datetime	end;

	stdoutput.printf("appending %d items (%d times)\n",ITEMS,ITERS);

	start.getSystemDateAndTime();
	for (uint64_t j=0; j<ITERS; j++) {

		for (uint64_t k=0; k<ITEMS; k++) {
			#if defined(USE_MAP)
				d.insert(std::pair<uint64_t,uint64_t>(k,k));
			#elif defined(USE_DICTIONARY)
				d.setValue(k,k);
			#endif
		}

		d.clear();

		stdoutput.printf(".");
	}
	end.getSystemDateAndTime();

	displayTime(&start,&end);
}
