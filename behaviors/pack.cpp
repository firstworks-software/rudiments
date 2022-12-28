#include <stdio.h>

#pragma pack(push)
#pragma pack(1)
struct packed_t {
	unsigned short	value;
};
#pragma pack(pop)

struct unpacked_t {
	unsigned short	value;
};

int main(int argc, char **argv) {

	unpacked_t	unpackedscalar;
	unpacked_t	unpackedarray[10];
	packed_t	packedscalar;
	packed_t	packedarray[10];

	printf("pack behavior\n");
	printf("\n");
	printf("  sizeof(unpacked_t): %d\n",(int)sizeof(unpacked_t));
	printf("  sizeof(unpackedscalar): %d\n",(int)sizeof(unpackedscalar));
	printf("  sizeof(unpackedarray[10]): %d\n",(int)sizeof(unpackedarray));
	printf("\n");
	printf("  sizeof(packed_t): %d\n",(int)sizeof(packed_t));
	printf("  sizeof(packedscalar): %d\n",(int)sizeof(packedscalar));
	printf("  sizeof(packedarray[10]): %d\n",(int)sizeof(packedarray));
	printf("\n");
}
