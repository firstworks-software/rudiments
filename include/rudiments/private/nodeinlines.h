// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_NODEINLINES_H
#define RUDIMENTS_NODEINLINES_H

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/stdio.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.


// print methods...

inline
void node_print(const char *value) {
	stdoutput.printf("%s",value);
}

inline
void node_print(const wchar_t *value) {
	stdoutput.printf(L"%s",value);
}

inline
void node_print(char value) {
	stdoutput.printf("%c",value);
}

inline
void node_print(wchar_t value) {
	stdoutput.printf(L"%c",value);
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
void node_print(const unsigned char *value) {
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
void node_print(const void *value) {
	stdoutput.printf("%08x",value);
}





// delete methods...

// Why do all of the node_delete_value() functions take pointers, just to
// dereference them?
//
// Older compilers (eg. gcc-2.95.2) seem to have trouble determining which
// function to call when passing a reference rather than passing a pointer.
//
// Eg. If there are a bunch of node_delete_value() methods for primitives and
// templated ones for const valuetype & (which does nothing),
// valuetype & (which also does nothing) and valuetype * (which actually
// deletes), then a call like:
//
// someclass	a;
// node_delete_value(a,true,false);
//
// get confused whether to call the const valuetype &, valuetype & or
// valuetype * version.
//
// It's not clear why except just that there's a bug in the compiler.  Newer
// compilers work as expected.
//
// This works around the problem though.  Apparently those old compilers don't
// have any trouble figuring out which function to call if you have functions
// that all take pointers and you call something like:
//
// someclass	a;
// node_delete_value(&a,true,false);
//
// So, for now at least, we'll do it this way.

inline
void node_delete_value(const char **value, bool managed, bool managedarray) {
	if (managedarray) {
		delete[] (char *)*value;
	}
}

inline
void node_delete_value(char **value, bool managed, bool managedarray) {
	if (managedarray) {
		delete[] *value;
	}
}

inline
void node_delete_value(const wchar_t **value, bool managed, bool managedarray) {
	if (managedarray) {
		delete[] (wchar_t *)*value;
	}
}

inline
void node_delete_value(wchar_t **value, bool managed, bool managedarray) {
	if (managedarray) {
		delete[] *value;
	}
}

inline
void node_delete_value(char *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(wchar_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(int16_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(int32_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(int64_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(const unsigned char **value,
				bool managed, bool managedarray) {
	if (managedarray) {
		delete[] (unsigned char *)*value;
	}
}

inline
void node_delete_value(unsigned char **value, bool managed, bool managedarray) {
	if (managedarray) {
		delete[] *value;
	}
}

inline
void node_delete_value(unsigned char *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(uint16_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(uint32_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(uint64_t *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(float *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(double *value, bool managed, bool managedarray) {
}

inline
void node_delete_value(long double *value, bool managed, bool managedarray) {
}

template <class valuetype>
inline
void node_delete_value(valuetype **value, bool managed, bool managedarray) {
	if (managed) {
		delete *value;
	} else if (managedarray) {
		delete[] *value;
	}
}

template <class valuetype>
inline
void node_delete_value(valuetype *value, bool managed, bool managedarray) {
}





// zero methods...

inline
void node_zero_value(const char **value) {
	*((char **)value)=NULL;
}

inline
void node_zero_value(char **value) {
	*value=NULL;
}

inline
void node_zero_value(const wchar_t **value) {
	*((wchar_t **)value)=NULL;
}

inline
void node_zero_value(wchar_t **value) {
	*value=NULL;
}

inline
void node_zero_value(char *value) {
	*value='\0';
}

inline
void node_zero_value(wchar_t *value) {
	*value=L'\0';
}

inline
void node_zero_value(int16_t *value) {
	*value=0;
}

inline
void node_zero_value(int32_t *value) {
	*value=0;
}

inline
void node_zero_value(int64_t *value) {
	*value=0;
}

inline
void node_zero_value(const unsigned char **value) {
	*((unsigned char **)value)=NULL;
}

inline
void node_zero_value(unsigned char **value) {
	*value=NULL;
}

inline
void node_zero_value(unsigned char *value) {
	*value='\0';
}

inline
void node_zero_value(uint16_t *value) {
	*value=0;
}

inline
void node_zero_value(uint32_t *value) {
	*value=0;
}

inline
void node_zero_value(uint64_t *value) {
	*value=0;
}

inline
void node_zero_value(float *value) {
	*value=0.0;
}

inline
void node_zero_value(double *value) {
	*value=0.0;
}

inline
void node_zero_value(long double *value) {
	*value=0.0;
}

template <class valuetype>
inline
void node_zero_value(valuetype **value) {
	*value=NULL;
}

template <class valuetype>
inline
void node_zero_value(valuetype *value) {
}





// duplicate methods...

inline
char *node_duplicate_value(const char *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		return charstring::duplicate(value);
	} else {
		return (char *)value;
	}
}

inline
char *node_duplicate_value(char *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		return charstring::duplicate(value);
	} else {
		return value;
	}
}

inline
const wchar_t *node_duplicate_value(const wchar_t *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		return wcharstring::duplicate(value);
	} else {
		return (wchar_t *)value;
	}
}

inline
wchar_t *node_duplicate_value(wchar_t *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		return wcharstring::duplicate(value);
	} else {
		return value;
	}
}

inline
char node_duplicate_value(char value,
				bool managed, bool managedarray) {
	return value;
}

inline
wchar_t node_duplicate_value(wchar_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
int16_t node_duplicate_value(int16_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
int32_t node_duplicate_value(int32_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
int64_t node_duplicate_value(int64_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
unsigned char *node_duplicate_value(
				const unsigned char *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		// this isn't a true duplicate, but it's
		// the best we can do without a size
		return (unsigned char *)
			charstring::duplicate((const char *)value);
	} else {
		return (unsigned char *)value;
	}
}

inline
unsigned char *node_duplicate_value(unsigned char *value,
				bool managed, bool managedarray) {
	if (managedarray) {
		// this isn't a true duplicate, but it's
		// the best we can do without a size
		return (unsigned char *)charstring::duplicate((char *)value);
	} else {
		return value;
	}
}

inline
unsigned char node_duplicate_value(unsigned char value,
				bool managed, bool managedarray) {
	return value;
}

inline
uint16_t node_duplicate_value(uint16_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
uint32_t node_duplicate_value(uint32_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
uint64_t node_duplicate_value(uint64_t value,
				bool managed, bool managedarray) {
	return value;
}

inline
float node_duplicate_value(float value,
				bool managed, bool managedarray) {
	return value;
}

inline
double node_duplicate_value(double value,
				bool managed, bool managedarray) {
	return value;
}

inline
long double node_duplicate_value(long double value,
				bool managed, bool managedarray) {
	return value;
}

template <class valuetype>
inline
valuetype *node_duplicate_value(valuetype *value,
				bool managed, bool managedarray) {
	if (managed) {
		return new valuetype(*value);
	} else if (managedarray) {
		// no way to do this without a length
		return value;
	} else {
		return value;
	}
}

template <class valuetype>
inline
const valuetype &node_duplicate_value(const valuetype &value,
				bool managed, bool managedarray) {
	if (managed) {
		return *(new valuetype(value));
	} else if (managedarray) {
		// no way to do this without a length
		return value;
	} else {
		return value;
	}
}

#endif
