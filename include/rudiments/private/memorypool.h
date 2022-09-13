// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	init(size_t initialsize,
				size_t increment,
				size_t resizeinterval);
		void	clear(bool resetinitialsize,
				size_t initialsize,
				size_t increment,
				size_t resizeinterval);

		memorypoolprivate	*pvt;
