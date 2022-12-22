// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		aes128(aes128 &a) {};
		aes128	&operator=(aes128 &a) { return *this; };

		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		aes128private	*pvt;
