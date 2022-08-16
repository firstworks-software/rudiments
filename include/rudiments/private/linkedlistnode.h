// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		valuetype	&getValueRef();

		void	setNext(listnode<valuetype> *next);
		void	setPrevious(listnode<valuetype> *previous);

	protected:
		valuetype		value;
		listnode<valuetype>	*next;
		listnode<valuetype>	*previous;
