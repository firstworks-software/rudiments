// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		bytebuffer	*getIn();
		bytebuffer	*getOut();
		virtual void	setError(hasherror_t err);
		void		setDirty(bool dirty);
		bool		getDirty();

	private:
		hash(hash &h) {};
		hash	&operator=(hash &h) { return *this; };

		void	construct();
		void	initSalt();

		hashprivate	*pvt;
