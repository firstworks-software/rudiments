// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		ssize_t	printfDelegate(const char *format, va_list *argp);
		ssize_t	printfDelegate(const wchar_t *format, va_list *argp);

	protected:
		void		construct(unsigned char *initialcontents,
							size_t initialsize);
		void		clone(bytebuffer &v);
		void		extend(size_t requiredsize);

		size_t		_size();
		void		_size(size_t s);
		size_t		_position();
		void		_position(size_t pos);

	private:
		bytebufferprivate	*pvt;
