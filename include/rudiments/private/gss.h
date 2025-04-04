// Copyright (c) David Muse
// See the COPYING file for more information.

		static const char	*getSspiStatusString(uint32_t status);

		gss(gss &g) {};
		gss	&operator=(gss &g) { return *this; };

	private:
		bool	clear();

		gssprivate	*pvt;
