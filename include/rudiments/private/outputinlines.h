// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

inline
off64_t output::setPositionRelativeToBeginning(off64_t offset) {
	return -1;
}

inline
off64_t output::setPositionRelativeToCurrent(off64_t offset) {
	return -1;
}

inline
off64_t output::setPositionRelativeToEnd(off64_t offset) {
	return -1;
}

inline
off64_t output::getPosition() {
	return -1;
}

inline
ssize_t output::printf(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	ssize_t	retval=printfDelegate(format,&argp);
	va_end(argp);
	return retval;
}

inline
ssize_t output::printf(const char *format, va_list *argp) {
	return printfDelegate(format,argp);
}

inline
ssize_t output::printf(const wchar_t *format, ...) {
	va_list	argp;
	va_start(argp,format);
	ssize_t	retval=printfDelegate(format,&argp);
	va_end(argp);
	return retval;
}

inline
ssize_t output::printf(const wchar_t *format, va_list *argp) {
	return printfDelegate(format,argp);
}

inline
ssize_t output::printfUcs2(const ucs2_t *format, ...) {
	va_list	argp;
	va_start(argp,format);
	ssize_t	retval=printfUcs2Delegate(format,&argp);
	va_end(argp);
	return retval;
}

inline
ssize_t output::printfUcs2(const ucs2_t *format, va_list *argp) {
	return printfUcs2Delegate(format,argp);
}
