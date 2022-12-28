#include <stdio.h>

#pragma pack(push)
#pragma pack(1)
struct p8_t {
	unsigned char	value;
};
#pragma pack(pop)

struct up8_t {
	unsigned char	value;
};


#pragma pack(push)
#pragma pack(1)
struct p16_t {
	unsigned short	value;
};
#pragma pack(pop)

struct up16_t {
	unsigned short	value;
};


#pragma pack(push)
#pragma pack(1)
struct p32_t {
	unsigned int	value;
};
#pragma pack(pop)

struct up32_t {
	unsigned int	value;
};


int main(int argc, char **argv) {

	printf("pack behavior\n");
	printf("\n");

	up8_t	up8scalar;
	up8_t	up8array[10];

	printf("  sizeof(up8_t): %d\n",(int)sizeof(up8_t));
	printf("  sizeof(up8scalar): %d\n",(int)sizeof(up8scalar));
	printf("  sizeof(up8array[10]): %d\n",(int)sizeof(up8array));
	printf("\n");

	p8_t	p8scalar;
	p8_t	p8array[10];
	printf("  sizeof(p8_t): %d\n",(int)sizeof(p8_t));
	printf("  sizeof(p8scalar): %d\n",(int)sizeof(p8scalar));
	printf("  sizeof(p8array[10]): %d\n",(int)sizeof(p8array));
	printf("\n");

	up16_t	up16scalar;
	up16_t	up16array[10];

	printf("  sizeof(up16_t): %d\n",(int)sizeof(up16_t));
	printf("  sizeof(up16scalar): %d\n",(int)sizeof(up16scalar));
	printf("  sizeof(up16array[10]): %d\n",(int)sizeof(up16array));
	printf("\n");

	p16_t	p16scalar;
	p16_t	p16array[10];
	printf("  sizeof(p16_t): %d\n",(int)sizeof(p16_t));
	printf("  sizeof(p16scalar): %d\n",(int)sizeof(p16scalar));
	printf("  sizeof(p16array[10]): %d\n",(int)sizeof(p16array));
	printf("\n");

	up32_t	up32scalar;
	up32_t	up32array[10];

	printf("  sizeof(up32_t): %d\n",(int)sizeof(up32_t));
	printf("  sizeof(up32scalar): %d\n",(int)sizeof(up32scalar));
	printf("  sizeof(up32array[10]): %d\n",(int)sizeof(up32array));
	printf("\n");

	p32_t	p32scalar;
	p32_t	p32array[10];
	printf("  sizeof(p32_t): %d\n",(int)sizeof(p32_t));
	printf("  sizeof(p32scalar): %d\n",(int)sizeof(p32scalar));
	printf("  sizeof(p32array[10]): %d\n",(int)sizeof(p32array));
	printf("\n");
}
