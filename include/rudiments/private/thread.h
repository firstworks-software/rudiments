// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		thread(thread &t) {};
		thread	&operator=(thread &t) { return *this; };

		bool	run(bool detached);

		threadprivate	*pvt;
