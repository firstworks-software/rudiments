// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		uint32_t		getKeySize();
		const unsigned char	*getDecryptedData();

	private:
		des(des &c) {};
		des	&operator=(des &c) { return *this; };

		desprivate	*pvt;
