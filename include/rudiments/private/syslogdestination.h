// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		syslogdestination(syslogdestination &f) {};
		syslogdestination	&operator=(syslogdestination &f) {
			return *this;
		}

	friend class logger;
	private:
		void	write(const char *string);
		void	write(const wchar_t *string);

		syslogdestinationprivate	*pvt;
