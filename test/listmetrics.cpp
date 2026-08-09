// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/linkedlist.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/prng.h>
#include <rudiments/datetime.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

//const int32_t	listsize=1024*72;
const int32_t	listsize=1024*1024*4;
const int32_t	iterations=1;

const char *ordertitles[]={
	"forwards",
	"backwards",
	"scaled",
	"unscaled",
	NULL
};

const char *sorttitles[]={
	"quickly",
	"inexpensively",
	NULL
};

int32_t	*scaledvalues=NULL;
int32_t	*unscldvalues=NULL;

int main(int argc, char **argv) {

	datetime	start;
	datetime	finish;

	uint32_t	seed=prng::getSeed();
	scaledvalues=new int32_t[listsize];
	unscldvalues=new int32_t[listsize];
	for (int32_t i=0; i<listsize; i++) {
		seed=prng::generate(seed);
		scaledvalues[i]=prng::scale(seed,0,10);
		unscldvalues[i]=seed;
	}

	linkedlist<int32_t>		uintll;

	stdoutput.printf("linkedlist:\n");

	uint16_t	j;
	for (j=0; j<1; j++) {

		stdoutput.printf("  %s\n",sorttitles[j]);

		for (uint16_t k=0; k<4; k++) {

			stdoutput.printf("    %s\t",ordertitles[k]);
			start.initFromSystemDateTime();
			for (uint16_t l=0; l<iterations; l++) {
				for (int32_t i=0; i<listsize; i++) {
					if (k==0) {
						uintll.append(i);
					} else if (k==1) {
						uintll.prepend(i);
					} else if (k==2) {
						uintll.append(scaledvalues[i]);
					} else if (k==3) {
						uintll.append(unscldvalues[i]);
					}
				}
				if (j==0) {
					uintll.sortQuickly();
				} else {
					uintll.sortInexpensively();
				}
				uintll.clear();
			}
			finish.initFromSystemDateTime();
			stdoutput.printf("%d seconds\n",
					finish.getEpoch()-start.getEpoch());
		}
	}
	

	singlylinkedlist<int32_t>		uintsll;

	stdoutput.printf("singlylinkedlist:\n");

	for (j=0; j<1; j++) {

		stdoutput.printf("  %s\n",sorttitles[j]);

		for (uint16_t k=0; k<4; k++) {

			stdoutput.printf("    %s\t",ordertitles[k]);
			start.initFromSystemDateTime();
			for (uint16_t l=0; l<iterations; l++) {
				for (int32_t i=0; i<listsize; i++) {
					if (k==0) {
						uintsll.append(i);
					} else if (k==1) {
						uintsll.prepend(i);
					} else if (k==2) {
						uintsll.append(scaledvalues[i]);
					} else if (k==3) {
						uintsll.append(unscldvalues[i]);
					}
				}
				if (j==0) {
					uintsll.sortQuickly();
				} else {
					uintsll.sortInexpensively();
				}
				uintsll.clear();
			}
			finish.initFromSystemDateTime();
			stdoutput.printf("%d seconds\n",
					finish.getEpoch()-start.getEpoch());
		}
	}

	return 0;
}
