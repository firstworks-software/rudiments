// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	clone(const sparsedynamictable<valuetype> *table);

		sparsedynamicarray<char *>	columnnames;
		sparsedynamicarray<sparsedynamicarray<valuetype> >	values;

		uint64_t	cols;
		uint64_t	rows;
