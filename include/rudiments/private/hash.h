// Copyright (c) David Muse
// See the COPYING file for more information.

		hash(hash &h) {};
		hash	&operator=(hash &h) { return *this; };

	protected:
		bytebuffer	*getIn();
		bytebuffer	*getOut();
		virtual void	setError(hasherror_t err);
		void		setDirty(bool dirty);
		bool		getDirty();

	private:
		void	construct();
		void	initSalt();

		hashprivate	*pvt;
