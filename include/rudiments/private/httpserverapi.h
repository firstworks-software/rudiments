// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		virtual	bool	getCharacter(char *ch)=0;

		virtual	void		initEnvironmentVariables()=0;
		virtual	const char	*getEnvironmentVariable(
							const char *name)=0;
		virtual	uint64_t	getEnvironmentVariableCount()=0;
		virtual	const char * const	*getEnvironmentVariables()=0;
		virtual	const char * const	*getEnvironmentValues()=0;
		virtual	bool		setEnvironmentVariable(
							const char *name,
							const char *value)=0;
		virtual	void		updateEnvironmentVariables()=0;

		virtual	httpserverapi	*status(const char *string)=0;
		virtual	httpserverapi	*header(const char *variable,
							const char *value)=0;
		virtual	httpserverapi	*header(const char *string)=0;

		virtual	uint64_t	getThreadsPerProcess()=0;
