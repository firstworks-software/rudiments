// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		groupentry(groupentry &g) {};
		groupentry	&operator=(groupentry &g) { return *this; };

		bool	initialize(const char *groupname, gid_t groupid);
		groupentryprivate	*pvt;
