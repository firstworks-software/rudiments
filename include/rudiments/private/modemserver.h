// Copyright (c) David Muse
// See the COPYING file for more information.

		modemserver(modemserver &m) {};
		modemserver	&operator=(modemserver &m) { return *this; };

	protected:
		const char	*_listenscript();
		const char	*_acceptscript();
		const char	*_disconnectscript();

	private:
		modemserverprivate	*pvt;
