// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/writer.h>

// NOTE: these methods are defined out-of-order from their declarations because
// some compilers (eg. gcc 2.7.x) prefer for inline methods to be defined before
// they are used.

inline
writer::~writer() {
}

inline
void writer::write(output *out, const char *value) const {
	out->printf("%s",value);
}

inline
void writer::write(output *out, const wchar_t *value) const {
	out->printf(L"%s",value);
}

inline
void writer::write(output *out, char value) const {
	out->printf("%c",value);
}

inline
void writer::write(output *out, wchar_t value) const {
	out->printf(L"%c",value);
}

inline
void writer::write(output *out, int16_t value) const {
	out->printf("%hd",value);
}

inline
void writer::write(output *out, int32_t value) const {
	out->printf("%d",(int)value);
}

inline
void writer::write(output *out, int64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		out->printf("%lld",(long long)value);
	#else
		out->printf("%ld",(long)value);
	#endif
}

inline
void writer::write(output *out, const unsigned char *value) const {
	out->printf("%s",value);
}

inline
void writer::write(output *out, unsigned char value) const {
	out->printf("%c",value);
}

inline
void writer::write(output *out, uint16_t value) const {
	out->printf("%hd",value);
}

inline
void writer::write(output *out, uint32_t value) const {
	out->printf("%d",(unsigned int)value);
}

inline
void writer::write(output *out, uint64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		out->printf("%lld",(unsigned long long)value);
	#else
		out->printf("%ld",(unsigned long)value);
	#endif
}

inline
void writer::write(output *out, float value) const {
	out->printf("%f",value);
}

inline
void writer::write(output *out, double value) const {
	out->printf("%f",value);
}

inline
void writer::write(output *out, long double value) const {
	out->printf("%Lf",value);
}

inline
void writer::write(output *out, const void *value) const {
	out->printf("%08x",value);
}

/*template <class valuetype>
inline
void writer::write(output *out, valuetype &value) const {
	out->printf("%08x",&value);
}*/

template <class valuetype>
inline
void writer::write(output *out, const valuetype &value) const {
	out->printf("%08x",&value);
}





inline
void writer::write(const char *value) const {
	write(&stdoutput,value);
}

inline
void writer::write(const wchar_t *value) const {
	write(&stdoutput,value);
}

inline
void writer::write(char value) const {
	write(&stdoutput,value);
}

inline
void writer::write(wchar_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(int16_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(int32_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(int64_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(const unsigned char *value) const {
	write(&stdoutput,value);
}

inline
void writer::write(unsigned char value) const {
	write(&stdoutput,value);
}

inline
void writer::write(uint16_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(uint32_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(uint64_t value) const {
	write(&stdoutput,value);
}

inline
void writer::write(float value) const {
	write(&stdoutput,value);
}

inline
void writer::write(double value) const {
	write(&stdoutput,value);
}

inline
void writer::write(long double value) const {
	write(&stdoutput,value);
}

inline
void writer::write(const void *value) const {
	write(&stdoutput,value);
}

/*template <class valuetype>
inline
void writer::write(valuetype &value) const {
	write(&stdoutput,value);
}*/

template <class valuetype>
inline
void writer::write(const valuetype &value) const {
	write(&stdoutput,value);
}
