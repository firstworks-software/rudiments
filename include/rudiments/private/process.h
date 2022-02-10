// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		static	void	shutDown(int32_t signum);
		static	void	crash(int32_t signum);
		static	void	exitHandler(int32_t signum);
		static	void	setShutDownFlagHandler(int32_t signum);
		static	void	waitForChildrenToExit(int32_t signum);
		static	char	*fullyQualifiedCommand(const char *command);

		static	signalhandler	_deadchildhandler;
		static	signalhandler	_shutdownhandler;
		static	signalhandler	_crashhandler;
		static	void		(*_shutdownfunc)(int32_t);
		static	void		(*_crashfunc)(int32_t);
		static	bool		_retry;

		static	volatile sig_atomic_t	_shutdownflag;
		static	int32_t			_shutdownsignal;
