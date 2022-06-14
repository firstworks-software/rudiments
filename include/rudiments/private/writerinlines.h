// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/writer.h>

inline
void writer::write(const char *value) const {
	stdoutput.printf("%s",value);
}

inline
void writer::write(const wchar_t *value) const {
	stdoutput.printf(L"%s",value);
}

inline
void writer::write(char value) const {
	stdoutput.printf("%c",value);
}

inline
void writer::write(wchar_t value) const {
	stdoutput.printf(L"%c",value);
}

inline
void writer::write(int16_t value) const {
	stdoutput.printf("%hd",value);
}

inline
void writer::write(int32_t value) const {
	stdoutput.printf("%d",(int)value);
}

inline
void writer::write(int64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(long long)value);
	#else
		stdoutput.printf("%ld",(long)value);
	#endif
}

inline
void writer::write(const unsigned char *value) const {
	stdoutput.printf("%s",value);
}

inline
void writer::write(unsigned char value) const {
	stdoutput.printf("%c",value);
}

inline
void writer::write(uint16_t value) const {
	stdoutput.printf("%hd",value);
}

inline
void writer::write(uint32_t value) const {
	stdoutput.printf("%d",(unsigned int)value);
}

inline
void writer::write(uint64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(unsigned long long)value);
	#else
		stdoutput.printf("%ld",(unsigned long)value);
	#endif
}

inline
void writer::write(float value) const {
	stdoutput.printf("%f",value);
}

inline
void writer::write(double value) const {
	stdoutput.printf("%f",value);
}

inline
void writer::write(long double value) const {
	stdoutput.printf("%Lf",value);
}

inline
void writer::write(const void *value) const {
	stdoutput.printf("%08x",value);
}
