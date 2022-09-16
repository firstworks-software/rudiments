// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		void		setRandomBuffer(unsigned char *buffer,
							size_t buffersize);
		bytebuffer	*getIn();
		bytebuffer	*getOut();
		virtual void	setError(encryptionerror_t err);
		void		setDirty(bool dirty);
		bool		getDirty();
		void		setEncrypted(bool encrypted);
		bool		getEncrypted();

	private:
		encryption(encryption &e) {};
		encryption	&operator=(encryption &e) { return *this; };

		void	init();
		void	clone(encryption &e);
		void	initKey();
		void	initIv();

		encryptionprivate	*pvt;
