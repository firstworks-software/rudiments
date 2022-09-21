// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		const char	*getDevice();
		const char	*getBaud();

		bool	configureSerialPort(int32_t fd, const char *baud);

	private:
		modemutilprivate	*pvt;
