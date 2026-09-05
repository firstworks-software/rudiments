// Copyright (c) David Muse
// See the COPYING file for more information.

		singledes(singledes &s) {};
		singledes	&operator=(singledes &s) { return *this; };

	private:
		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		singledesprivate	*pvt;
