// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	write(const char *logentry);
		void	write(const wchar_t *logentry);

		// FIXME: should be possible to copy a logger
			logger(const logger &l);
		logger	&operator=(const logger &l);

		loggerprivate	*pvt;
