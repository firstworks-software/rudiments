// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		dynamicarray<char *>			columnnames;
		dynamicarray<dynamicarray<valuetype> >	values;

		void		clone(tablecollection<valuetype> *t);
		void		deleteManagedValues();
		valuetype	&find(uint64_t row, uint64_t col);

		uint64_t	cols;
		uint64_t	rows;
