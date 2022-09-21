// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		char		**_connecterror();
		int32_t		_timeoutsec();
		int32_t		_timeoutusec();
		uint32_t	_retrywait();
		uint32_t	_tries();

		void	setParameters(char *connecterror,
					int32_t timeoutsec,
					int32_t timeoutusec,
					uint32_t retrywait,
					uint32_t tries);

	private:
		client(client &c) {};
		client	&operator=(client &c) { return *this; };

		clientprivate	*pvt;
