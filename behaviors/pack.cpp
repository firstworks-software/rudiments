#include <stdio.h>

#pragma pack(push)
#pragma pack(1)
struct test_t {
	unsigned short	value;
};
#pragma pack(pop)

int main(int argc, char **argv) {

	test_t	s;
	test_t	a[10];

	printf("pack behavior\n");
	printf("  sizeof(test_t): %d\n",(int)sizeof(test_t));
	printf("  sizeof(s): %d\n",(int)sizeof(s));
	printf("  sizeof(a): %d\n",(int)sizeof(a));
	printf("\n");
}
