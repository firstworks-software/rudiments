// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/memorypool.h>
#include <rudiments/stdio.h>
#include "../test/test.cpp"

const uint32_t	stresscycles=1024*50;
const uint32_t	stresssize=4096;

int main(int argc, const char **argv) {

	uint32_t	i;
	uint32_t	j;

	datetime	start;
	datetime	end;

	// stress
	stdoutput.printf("stress...\n");

	// allocate from the pool, clearng after each cycle...
	stdoutput.printf("	memory pool... ");
	memorypool	*mp=new memorypool(32,16,10);
	start.initFromSystemDateTime();
	for (i=0; i<stresscycles; i++) {
		for (j=1; j<=stresssize; j++) {
			mp->allocate(j);
		}
		mp->clear();
	}
	end.initFromSystemDateTime();
	delete mp;
	displayTime(&start,&end);


	// ...as opposed to allocating and clearng on-demand
	stdoutput.printf("	on-demand...   ");
	start.initFromSystemDateTime();
	for (i=0; i<stresscycles; i++) {
		for (j=0; j<stresssize; j++) {
			char	*data=new char[j];
			delete[] data;
		}
	}
	end.initFromSystemDateTime();

	// should be about 3 times slower on modern Linux
	// and about 10 times slower on modern Windows
	displayTime(&start,&end);

#ifdef _WIN32
	stdoutput.printf("(this next one might take a while)\n");
#endif

	// ...and as opposed to allocating on-demand, keeping a chunk-list,
	// and clearng all chunks after each cycle
	stdoutput.printf("	chunk-list...  ");
	char	*chunklist[stresssize];
	start.initFromSystemDateTime();
	for (i=0; i<stresscycles; i++) {
		for (j=0; j<stresssize; j++) {
			chunklist[j]=new char[j];
		}
		for (j=0; j<stresssize; j++) {
			delete[] chunklist[j];
		}
	}
	end.initFromSystemDateTime();

	// should be about 3 times slower on modern Linux
	// and about 200 times slower on modern Windows
	displayTime(&start,&end);

	stdoutput.printf("\n");
}
