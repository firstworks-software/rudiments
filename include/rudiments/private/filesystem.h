// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		filesystem(filesystem &f) {};
		filesystem &operator=(filesystem &f) { return *this; };

	private:
		filesystemprivate	*pvt;
