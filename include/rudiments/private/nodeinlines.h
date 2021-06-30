// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_NODE_H
#define RUDIMENTS_NODE_H

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.

inline
int32_t node_compare(char *value1, char *value2) {
	return charstring::compare(value1,value2);
}

inline
int32_t node_compare(const char *value1, const char *value2) {
	return charstring::compare(value1,value2);
}

inline
int32_t node_compare(const unsigned char *value1,
				const unsigned char *value2) {
	return charstring::compare((const char *)value1,(const char *)value2);
}

inline
int32_t node_compare(unsigned char *value1, unsigned char *value2) {
	return charstring::compare((const char *)value1,(const char *)value2);
}

inline
int32_t node_compare(char value1, char value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(int16_t value1, int16_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(int32_t value1, int32_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(int64_t value1, int64_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(unsigned char value1, unsigned char value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(uint16_t value1, uint16_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(uint32_t value1, uint32_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(uint64_t value1, uint64_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(float value1, float value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(double value1, double value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(long double value1, long double value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
int32_t node_compare(void *value1, void *value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

inline
void node_print(const char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(char value) {
	stdoutput.printf("%c",value);
}

inline
void node_print(int16_t value) {
	stdoutput.printf("%hd",value);
}

inline
void node_print(int32_t value) {
	stdoutput.printf("%d",(int)value);
}

inline
void node_print(int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(long long)value);
	#else
		stdoutput.printf("%ld",(long)value);
	#endif
}

inline
void node_print(unsigned const char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(unsigned char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(unsigned char value) {
	stdoutput.printf("%c",value);
}

inline
void node_print(uint16_t value) {
	stdoutput.printf("%hd",value);
}

inline
void node_print(uint32_t value) {
	stdoutput.printf("%d",(unsigned int)value);
}

inline
void node_print(uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(unsigned long long)value);
	#else
		stdoutput.printf("%ld",(unsigned long)value);
	#endif
}

inline
void node_print(float value) {
	stdoutput.printf("%f",value);
}

inline
void node_print(double value) {
	stdoutput.printf("%f",value);
}

inline
void node_print(long double value) {
	stdoutput.printf("%Lf",value);
}

inline
void node_print(void *value) {
	stdoutput.printf("%08x",value);
}

#ifdef NODE_DELETE

inline
void node_delete(const char *value) {
}

inline
void node_delete(char *value) {
}

inline
void node_delete(char value) {
}

inline
void node_delete(int16_t value) {
}

inline
void node_delete(int32_t value) {
}

inline
void node_delete(int64_t value) {
}

inline
void node_delete(unsigned const char *value) {
}

inline
void node_delete(unsigned char *value) {
}

inline
void node_delete(unsigned char value) {
}

inline
void node_delete(uint16_t value) {
}

inline
void node_delete(uint32_t value) {
}

inline
void node_delete(uint64_t value) {
}

inline
void node_delete(float value) {
}

inline
void node_delete(double value) {
}

inline
void node_delete(long double value) {
}

inline
void node_delete(void *value) {
	delete value;
}

inline
void node_delete(object *value) {
	delete value;
}

inline
void node_array_delete(const char *value) {
	delete[] (char *)value;
}

inline
void node_array_delete(char *value) {
	delete[] value;
}

inline
void node_array_delete(char value) {
}

inline
void node_array_delete(int16_t value) {
}

inline
void node_array_delete(int32_t value) {
}

inline
void node_array_delete(int64_t value) {
}

inline
void node_array_delete(unsigned const char *value) {
	delete[] (unsigned char *)value;
}

inline
void node_array_delete(unsigned char *value) {
	delete[] value;
}

inline
void node_array_delete(unsigned char value) {
}

inline
void node_array_delete(uint16_t value) {
}

inline
void node_array_delete(uint32_t value) {
}

inline
void node_array_delete(uint64_t value) {
}

inline
void node_array_delete(float value) {
}

inline
void node_array_delete(double value) {
}

inline
void node_array_delete(long double value) {
}

inline
void node_array_delete(void *value) {
	delete[] value;
}

inline
void node_array_delete(object *value) {
	delete[] value;
}

#endif

#endif
