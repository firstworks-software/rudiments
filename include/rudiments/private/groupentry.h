// Copyright (c) David Muse
// See the COPYING file for more information.

		groupentry(groupentry &g) {};
		groupentry	&operator=(groupentry &g) { return *this; };

	private:
		bool	open(const char *groupname, gid_t groupid);

		groupentryprivate	*pvt;
