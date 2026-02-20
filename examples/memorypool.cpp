#include <rudiments/memorypool.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a memory pool with initial size 1024, growing by
	// 256 bytes, and resizing every 10 allocations
	memorypool	pool(1024,256,10);

	stdoutput.printf("initial size:     %d\n",pool.getInitialSize());
	stdoutput.printf("increment size:   %d\n",pool.getIncrementSize());
	stdoutput.printf("resize interval:  %d\n\n",pool.getResizeInterval());


	// allocate some blocks from the pool
	byte_t	*block1=pool.allocate(100);
	bytestring::set(block1,'A',100);
	stdoutput.write("allocated 100 bytes\n");

	byte_t	*block2=pool.allocate(200);
	bytestring::set(block2,'B',200);
	stdoutput.write("allocated 200 bytes\n");

	byte_t	*block3=pool.allocate(50);
	bytestring::set(block3,'C',50);
	stdoutput.write("allocated 50 bytes\n\n");


	// verify the data
	stdoutput.printf("block1[0]: %c\n",block1[0]);
	stdoutput.printf("block2[0]: %c\n",block2[0]);
	stdoutput.printf("block3[0]: %c\n\n",block3[0]);


	// clear the pool, shrinking it back to its initial size
	pool.clear();
	stdoutput.write("pool cleared\n");
}
