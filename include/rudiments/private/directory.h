// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		directory(directory &d);
		directory	&operator=(directory &d);

	private:

		static	int64_t	pathConf(const char *pathname, int32_t name);

		int64_t		fpathConf(int32_t name);

		directoryprivate	*pvt;
