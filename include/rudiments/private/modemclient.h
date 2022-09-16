// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_connectscript();
		const char	*_disconnectscript();
		const char	*_phonenumber();

	private:
		modemclient(modemclient &m) {};
		modemclient	&operator=(modemclient &m) { return *this; };

		modemclientprivate	*pvt;
