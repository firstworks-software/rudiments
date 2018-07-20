// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		void		init(unsigned char *initialcontents,
					size_t initialsize, size_t increment);
		void		clearExtentList();
		void		clear(bool resetpositions,
					bool resetveryinitialsize,
					unsigned char *initialcontents,
					size_t initialsize, size_t increment);
		void		copyIn(unsigned char *data,
						size_t size);
		void		copyOut(unsigned char *data,
						size_t size,
						size_t *bytescopied);
		void		extend(size_t size);
		unsigned char	*coalesce(bool replaceinitial);
		void		bytebufferClone(const bytebuffer &v);
		void		goToEnd();
		void		findExtent();

		size_t		_end();
		void		_end(size_t e);
		size_t		_position();
		void		_position(size_t pos);

	private:
		bytebufferprivate	*pvt;
