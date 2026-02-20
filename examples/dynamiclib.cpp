#include <rudiments/dynamiclib.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	dynamiclib	dl;

	// open the math library
	if (!dl.open("libm.so",true,true)) {
		stdoutput.write("failed to open libm.so\n");
		return 1;
	}
	stdoutput.write("opened libm.so\n\n");


	// look up the ceil() function
	double (*ceilfn)(double)=
			(double (*)(double))dl.getSymbol("ceil");
	if (!ceilfn) {
		stdoutput.write("failed to find ceil()\n");
		dl.close();
		return 1;
	}

	// call ceil()
	stdoutput.printf("ceil(1.1) = %g\n",ceilfn(1.1));
	stdoutput.printf("ceil(1.9) = %g\n\n",ceilfn(1.9));


	// look up the floor() function
	double (*floorfn)(double)=
			(double (*)(double))dl.getSymbol("floor");
	if (!floorfn) {
		stdoutput.write("failed to find floor()\n");
		dl.close();
		return 1;
	}

	// call floor()
	stdoutput.printf("floor(1.1) = %g\n",floorfn(1.1));
	stdoutput.printf("floor(1.9) = %g\n\n",floorfn(1.9));


	// close the library
	dl.close();
	stdoutput.write("closed libm.so\n");
}
