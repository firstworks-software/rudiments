// Copyright (c) David Muse
// See the COPYING file for more information.

		gssmechanism(gssmechanism &g) {};
		gssmechanism	&operator=(gssmechanism &g) { return *this; };

	private:
		void	strToOid(const char *str, void *oid);

		gssmechanismprivate	*pvt;
