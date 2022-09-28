// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		static const char	*getSspiStatusString(uint32_t status);

	private:
		gss(gss &g) {};
		gss	&operator=(gss &g) { return *this; };

		bool	clear();

		gssprivate	*pvt;
