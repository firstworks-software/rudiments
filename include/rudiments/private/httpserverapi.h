// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

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

		virtual	httpserverapi	*sendStatusHeader(const char *string)=0;
		virtual	httpserverapi	*sendHeader(const char *variable,
							const char *value)=0;
		virtual	httpserverapi	*sendHeader(const char *string)=0;
