// Copyright (c) David Muse
// See the COPYING file for more information.

		tripledes(tripledes &t) {};
		tripledes	&operator=(tripledes &t) { return *this; };

	private:
		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		tripledesprivate	*pvt;
