// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		void		init(unsigned char *initialcontents,
						size_t initialsize);
		void		bytebufferClone(const bytebuffer &v);
		void		extend(size_t requiredsize);

		size_t		_size();
		void		_size(size_t s);
		size_t		_position();
		void		_position(size_t pos);

	private:
		bytebufferprivate	*pvt;
