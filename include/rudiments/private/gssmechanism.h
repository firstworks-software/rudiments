// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		gssmechanism(gssmechanism &g) {};
		gssmechanism	&operator=(gssmechanism &g) { return *this; };

		void	strToOid(const char *str, void *oid);

		gssmechanismprivate	*pvt;
