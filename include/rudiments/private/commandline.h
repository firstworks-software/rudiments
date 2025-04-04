// Copyright (c) David Muse
// See the COPYING file for more information.

		commandline(commandline &c) {};
		commandline	&operator=(commandline &c) { return *this; }

	private:
		void	construct();

		commandlineprivate	*pvt;
