// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		userentry(userentry &u) {};
		userentry	&operator=(userentry &u) { return *this; };

		bool	open(const char *username, uid_t userid);

		userentryprivate	*pvt;
