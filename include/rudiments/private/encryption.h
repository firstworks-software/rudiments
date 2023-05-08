// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		encryption(encryption &e) {};
		encryption	&operator=(encryption &e) { return *this; };

	protected:
		void		setRandomBuffer(byte_t *buffer,
							size_t buffersize);
		bytebuffer	*getIn();
		bytebuffer	*getOut();
		virtual void	setError(encryptionerror_t err);
		void		setDirty(bool dirty);
		bool		getDirty();
		void		setEncrypted(bool encrypted);
		bool		getEncrypted();

	private:
		void	construct();
		void	initKey();
		void	initIv();

		encryptionprivate	*pvt;
