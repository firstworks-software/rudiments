// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

// NOTE: these methods are defined out-of-order from their declarations because
// some compilers (eg. gcc 2.7.x) prefer for inline methods to be defined before
// they are used.

inline
writer::~writer() {
}

inline
ssize_t writer::write(output *out, const char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t writer::write(output *out, const wchar_t *value) const {
	return out->printf(L"%s",value);
}

inline
ssize_t writer::write(output *out, char value) const {
	return out->printf("%c",value);
}

inline
ssize_t writer::write(output *out, wchar_t value) const {
	return out->printf(L"%c",value);
}

inline
ssize_t writer::write(output *out, int16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t writer::write(output *out, int32_t value) const {
	return out->printf("%d",(int)value);
}

inline
ssize_t writer::write(output *out, int64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t writer::write(output *out, const unsigned char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t writer::write(output *out, unsigned char value) const {
	return out->printf("%c",value);
}

inline
ssize_t writer::write(output *out, uint16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t writer::write(output *out, uint32_t value) const {
	return out->printf("%d",(unsigned int)value);
}

inline
ssize_t writer::write(output *out, uint64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(unsigned long long)value);
	#else
		return out->printf("%ld",(unsigned long)value);
	#endif
}

inline
ssize_t writer::write(output *out, float value) const {
	return out->printf("%f",value);
}

inline
ssize_t writer::write(output *out, double value) const {
	return out->printf("%f",value);
}

inline
ssize_t writer::write(output *out, long double value) const {
	return out->printf("%Lf",value);
}

inline
ssize_t writer::write(output *out, const void *value) const {
	return out->printf("%08x",value);
}

inline
ssize_t writer::write(output *out, const object *value) const {
	return out->printf("%08x",value);
}

inline
ssize_t writer::write(output *out, const object &value) const {
	return out->printf("%08x",&value);
}



inline
ssize_t writer::write(const char *value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(const wchar_t *value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(char value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(wchar_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(int16_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(int32_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(int64_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(const unsigned char *value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(unsigned char value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(uint16_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(uint32_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(uint64_t value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(float value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(double value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(long double value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(const void *value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(const object *value) const {
	return write(&stdoutput,value);
}

inline
ssize_t writer::write(const object &value) const {
	return write(&stdoutput,value);
}
