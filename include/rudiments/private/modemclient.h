// Copyright (c) David Muse
// See the COPYING file for more information.

		modemclient(modemclient &m) {};
		modemclient	&operator=(modemclient &m) { return *this; };

	protected:
		const char	*_connectscript();
		const char	*_disconnectscript();
		const char	*_phonenumber();

	private:
		modemclientprivate	*pvt;
