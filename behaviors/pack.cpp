#include <rudiments/stdio.h>

#include "../test/test.cpp"

#pragma pack(push)
#pragma pack(1)
struct test_t {
	uint16_t	value;
};
#pragma pack(pop)

int main(int argc, char **argv) {

	test_t	s;
	test_t	a[10];

	header("pack behavior");
	stdoutput.printf("  sizeof(test_t): %d\n",sizeof(test_t));
	stdoutput.printf("  sizeof(s): %d\n",sizeof(s));
	stdoutput.printf("  sizeof(a): %d\n",sizeof(a));
	stdoutput.printf("\n");
}
