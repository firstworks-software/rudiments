// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

			void	reset();
			bool	parse();

	private:
				csvsax(const csvsax &x);
			csvsax	&operator=(const csvsax &x);

			csvsaxprivate	*pvt;
