// Copyright (c) David Muse
// See the COPYING file for more information.

		output(output &o) {};
		output	&operator=(output &o) { return *this; };

		virtual	ssize_t	printfDelegate(const char *format,
							va_list *argp)=0;
		virtual	ssize_t	printfDelegate(const wchar_t *format,
							va_list *argp)=0;
		virtual	ssize_t	printfUcs2Delegate(const ucs2_t *format,
							va_list *argp)=0;

