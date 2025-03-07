// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		dynamicarray<char *>			columnnames;
		dynamicarray<dynamicarray<valuetype> >	values;

		void	clone(tablecollection<valuetype> *t);

		uint64_t	cols;
		uint64_t	rows;
