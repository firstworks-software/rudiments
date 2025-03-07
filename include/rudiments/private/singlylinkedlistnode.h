// Copyright (c) David Muse
// See the COPYING file for more information

		void	setNext(listnode<valuetype> *next);
		void	setPrevious(listnode<valuetype> *previous);

	protected:
		valuetype		value;
		listnode<valuetype>	*next;
