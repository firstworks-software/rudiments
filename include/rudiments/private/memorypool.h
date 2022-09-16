// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		memorypool(memorypool &m) {};
		memorypool	&operator=(memorypool &m) { return *this; };

		void	construct(size_t initialsize,
					size_t increment,
					size_t resizeinterval);
		void	clear(bool resetinitialsize,
					size_t initialsize,
					size_t increment,
					size_t resizeinterval);

		memorypoolprivate	*pvt;
