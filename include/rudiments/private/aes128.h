// Copyright (c) David Muse
// See the COPYING file for more information.

		aes128(aes128 &a) {};
		aes128	&operator=(aes128 &a) { return *this; };

		void	setUseGcm(bool usegcm);
		bool	getUseGcm();

	private:
		const byte_t	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		aes128private	*pvt;
