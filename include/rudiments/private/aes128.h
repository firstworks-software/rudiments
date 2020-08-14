// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		const unsigned char	*getData(bool encrypt);

		void	newContext();
		void	freeContext();

		void	setError(int32_t err);

		aes128private	*pvt;
