// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class csvdomnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			void	writeValue(output *out, domnode *value) const;

				csvdom(const csvdom &x);
			csvdom	&operator=(const csvdom &x);

	private:
			csvdomprivate	*pvt;
