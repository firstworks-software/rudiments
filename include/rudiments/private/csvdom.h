// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	friend class csvdomnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

				csvdom(const csvdom &x);
			csvdom	&operator=(const csvdom &x);

	private:
			csvdomprivate	*pvt;
