// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#if defined(USE_STRING)
	#include <string>
#elif defined(USE_STRINGBUFFER)
	#include <rudiments/stringbuffer.h>
#elif defined(USE_BYTEBUFFER)
	#include <rudiments/bytebuffer.h>
#endif
#include <rudiments/stdio.h>
#include "../test/test.cpp"

#define ITEMS 1024*1024*10
//#define ITERS 10
#define ITERS 160

int main(int argc, char **argv) {

#if defined(USE_STRING)
	std::string	str;
#elif defined(USE_STRINGBUFFER)
	stringbuffer	str;
	stdoutput.printf("initial size: %d\n",str.getInitialSize());
#elif defined(USE_BYTEBUFFER)
	bytebuffer	str;
	stdoutput.printf("initial size: %d\n",str.getInitialSize());
#endif
	datetime	start;
	datetime	end;

	char	block[1024];
	for (uint64_t b=0; b<sizeof(block); b++) {
		block[b]='a'+(b%10);
	}

	for (uint64_t i=1; i<=1024; i=i*2) {

		stdoutput.printf("append %d characters "
				"(%lld at a time, %d iterations)\n",
				ITEMS,i,ITERS);

		start.initFromSystemDateTime();
		for (uint64_t j=0; j<ITERS; j++) {

			for (uint64_t k=0; k<ITEMS/i; k++) {
				str.append(block,i);
			}
			str.clear();
		}
		end.initFromSystemDateTime();

		displayTime(&start,&end);
	}
}
