// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class logger;
	private:
		filedestination(filedestination &f) {};
		filedestination	&operator=(filedestination &f) {
			return *this;
		};

		void	write(const char *string);
		void	write(const wchar_t *string);

		filedestinationprivate	*pvt;
