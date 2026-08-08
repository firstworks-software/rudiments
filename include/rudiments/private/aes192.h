// Copyright (c) David Muse
// See the COPYING file for more information.

		aes192(aes192 &a) {};
		aes192	&operator=(aes192 &a) { return *this; };

	private:
		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		aes192private	*pvt;
