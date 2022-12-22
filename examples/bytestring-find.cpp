#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// initialize buffer
	byte_t	buffer[]={0,2,0,2,0,2,0};

	stdoutput.write("buffer : ");
	stdoutput.safePrint(buffer,sizeof(buffer));
	stdoutput.write("\n\n");


	// find first and last 0
	byte_t	*firstzero=(byte_t *)
			bytestring::findFirst(buffer,0,sizeof(buffer));
	byte_t	*lastzero=(byte_t *)
			bytestring::findLast(buffer,0,sizeof(buffer));

	stdoutput.write("first 0 : ");
	stdoutput.safePrint(firstzero,sizeof(buffer)-(firstzero-buffer));
	stdoutput.write('\n');

	stdoutput.write(" last 0 : ");
	stdoutput.safePrint(lastzero,sizeof(buffer)-(lastzero-buffer));
	stdoutput.write("\n\n");


	// find first and last 0,2,0 patterns
	byte_t	pattern[]={0,2,0};

	byte_t	*firstzerotwozero=(byte_t *)
			bytestring::findFirst(buffer,sizeof(buffer),
						pattern,sizeof(pattern));
	byte_t	*lastzerotwozero=(byte_t *)
			bytestring::findLast(buffer,sizeof(buffer),
						pattern,sizeof(pattern));

	stdoutput.write("first 0,2,0 : ");
	stdoutput.safePrint(firstzerotwozero,
				sizeof(buffer)-(firstzerotwozero-buffer));
	stdoutput.write('\n');

	stdoutput.write(" last 0,2,0 : ");
	stdoutput.safePrint(lastzerotwozero,
				sizeof(buffer)-(lastzerotwozero-buffer));
	stdoutput.write("\n\n");
}
