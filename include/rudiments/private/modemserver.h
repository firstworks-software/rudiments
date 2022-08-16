// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_listenscript();
		const char	*_acceptscript();
		const char	*_disconnectscript();

		void	clone(modemserver &m);
	private:
		modemserverprivate	*pvt;
