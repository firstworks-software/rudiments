// Copyright (c) David Muse
// See the COPYING file for more information.

		aes256(aes256 &a) {};
		aes256	&operator=(aes256 &a) { return *this; };

	private:
		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		aes256private	*pvt;
