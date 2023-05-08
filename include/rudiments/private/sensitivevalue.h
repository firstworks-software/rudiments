// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		sensitivevalue(sensitivevalue &s) {};
		sensitivevalue	&operator=(sensitivevalue &s) { return *this; };

	private:
		bool	getValueFromFile(const char *filename,
						bool hexdecode,
						bool chomp);
		void	construct();

		sensitivevalueprivate	*pvt;
