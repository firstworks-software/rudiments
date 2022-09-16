// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	write(const char *logentry);
		void	write(const wchar_t *logentry);

		logger(logger &l) {};
		logger	&operator=(logger &l) { return *this; }

		loggerprivate	*pvt;
