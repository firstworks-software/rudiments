// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/output.h>
#include <rudiments/charstring.h>

output::output() {
}

output::~output() {
}

ssize_t output::write(const byte_t *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(const char *string, int32_t sec, int32_t usec) {
	return write(string);
}

ssize_t output::write(const char *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(char character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::write(const wchar_t *string, int32_t sec, int32_t usec) {
	return write(string);
}

ssize_t output::write(const wchar_t *string, size_t size,
						int32_t sec, int32_t usec) {
	return write(string,size);
}

ssize_t output::write(wchar_t character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::writeUcs2(const ucs2_t *string, int32_t sec, int32_t usec) {
	return writeUcs2(string);
}

ssize_t output::writeUcs2(const ucs2_t *string, size_t size,
						int32_t sec, int32_t usec) {
	return writeUcs2(string,size);
}

ssize_t output::writeUcs2(ucs2_t character, int32_t sec, int32_t usec) {
	return writeUcs2(character);
}

ssize_t output::write(int16_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(int32_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(int64_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(byte_t character, int32_t sec, int32_t usec) {
	return write(character);
}

ssize_t output::write(uint16_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(uint32_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(uint64_t number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(float number, int32_t sec, int32_t usec) {
	return write(number);
}

ssize_t output::write(double number, int32_t sec, int32_t usec) {
	return write(number);
}

void output::safePrint(char c) {
	safePrint((byte_t)c);
}

void output::safePrint(const char *string, int32_t length) {
	safePrint((const byte_t *)string,length);
}

void output::safePrint(const char *string) {
	safePrint((const byte_t *)string);
}

static char hex[17]="0123456789ABCDEF";

void output::safePrint(byte_t c) {
	if (c=='\r') {
		printf("\\r");
	} else if (c=='\n') {
		printf("\\n");
	} else if (c=='	') {
		printf("\\t");
	} else if (c>=' ' && c<='~') {
		printf("%c",c);
	} else {
		unsigned int	uintc=(byte_t)c;
		printf("(0x%c%c|%d)",hex[((c>>4)&0x0F)],hex[(c&0x0F)],uintc);
	}
}

void output::safePrint(const byte_t *string, int32_t length) {
	for (int32_t i=0; i<length; i++) {
		safePrint(*string);
		string++;
	}
}

void output::safePrint(const byte_t *string) {
	safePrint(string,charstring::getLength((const char *)string));
}

void output::printBits(byte_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(uint16_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(uint32_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(uint64_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(char value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(int16_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(int32_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(int64_t value) {
	printBits((const byte_t *)&value,sizeof(value));
}

void output::printBits(const byte_t *bits, uint64_t size) {
	for (uint64_t i=0; i<size; i++) {
		byte_t byte=bits[i];
		for (int8_t j=7; j>=0; j--) {
			printf("%d",(byte>>j)&0x01);
		}
	}
}

void output::printHex(const byte_t *data, uint64_t size) {
	printHex(data,size,1);
}

void output::printHex(const byte_t *data, uint64_t size, uint16_t indent) {
	if (!size) {
		return;
	}
	for (uint16_t j=0; j<indent; j++) {
		write('	');
	}
	for (uint64_t i=0; i<size; i++) {
		printf("%02x  ",data[i]);
		if (!((i+1)%8)) {
			write("   ",3);
		}
		if (!((i+1)%16)) {
			write('\n');
			for (uint16_t j=0; j<indent; j++) {
				write('	');
			}
		}
	}
	write('\n');
	for (uint16_t i=0; i<indent; i++) {
		write('	');
	}
	for (uint64_t i=0; i<size; i++) {
		if (data[i]>=' ' && data[i]<='~') {
			printf("%c   ",data[i]);
		} else {
			write(".   ",4);
		}
		if (!((i+1)%8)) {
			write("   ",3);
		}
		if (!((i+1)%16)) {
			write('\n');
			for (uint16_t j=0; j<indent; j++) {
				write('	');
			}
		}
	}
	write('\n');
}
