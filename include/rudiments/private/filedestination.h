// Copyright (c) David Muse
// See the COPYING file for more information.

		filedestination(filedestination &f) {};
		filedestination	&operator=(filedestination &f) {
			return *this;
		};

	friend class logger;
	private:
		void	write(const char *string);
		void	write(const wchar_t *string);

		filedestinationprivate	*pvt;
