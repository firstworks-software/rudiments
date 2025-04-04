// Copyright (c) David Muse
// See the COPYING file for more information.

		userentry(userentry &u) {};
		userentry	&operator=(userentry &u) { return *this; };

	private:
		bool	open(const char *username, uid_t userid);

		userentryprivate	*pvt;
