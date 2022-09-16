// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		virtual	ssize_t	printfDelegate(const char *format,
							va_list *argp)=0;
		virtual	ssize_t	printfDelegate(const wchar_t *format,
							va_list *argp)=0;

	private:
		output(output &o) {};
		output	&operator=(output &o) { return *this; };

