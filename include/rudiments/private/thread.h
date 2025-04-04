// Copyright (c) David Muse
// See the COPYING file for more information.

		thread(thread &t) {};
		thread	&operator=(thread &t) { return *this; };

	private:
		bool	run(bool detached);

		threadprivate	*pvt;
