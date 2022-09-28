// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		commandline(commandline &c) {};
		commandline	&operator=(commandline &c) { return *this; }

		void	construct();

		commandlineprivate	*pvt;
