// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		logger(logger &l) {};
		logger	&operator=(logger &l) { return *this; }

		ssize_t	printfDelegate(const char *format, va_list *argp);
		ssize_t	printfDelegate(const wchar_t *format, va_list *argp);
		ssize_t	printfUcs2Delegate(const ucs2_t *format, va_list *argp);

	private:
		void	writeToLog(const char *logentry);
		void	writeToLog(const wchar_t *logentry);

		loggerprivate	*pvt;
