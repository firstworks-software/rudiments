// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		void	setOptions(termios *tio);
		termios	*getTermios();

	private:
		serialportprofile(serialportprofile &s) {};
		serialportprofile	&operator=(serialportprofile &s) {
			return *this;
		}

		baudrate_t	translateBaudString(const char *baud);
		serialportprofileprivate	*pvt;
