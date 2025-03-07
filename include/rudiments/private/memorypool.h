// Copyright (c) David Muse
// See the COPYING file for more information.

		memorypool(memorypool &m) {};
		memorypool	&operator=(memorypool &m) { return *this; };

	private:
		void	construct(size_t initialsize,
					size_t increment,
					size_t resizeinterval);
		bool	clear(bool resetinitialsize,
					size_t initialsize,
					size_t increment,
					size_t resizeinterval);

		memorypoolprivate	*pvt;
