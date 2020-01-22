// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
		void	*getApiStruct();

		bool	getCharacter(char *ch);

		void		initEnvironmentVariables();
		const char	*getEnvironmentVariable(const char *name);
		uint64_t	getEnvironmentVariableCount();
		const char * const	*getEnvironmentVariables();
		const char * const	*getEnvironmentValues();
		bool		setEnvironmentVariable(const char *name,
							const char *value);
		void		updateEnvironmentVariables();

		httpserverapi	*status(const char *string);
		httpserverapi	*header(const char *variable,
						const char *value);
		httpserverapi	*header(const char *string);

		ssize_t	write(const unsigned char *string, size_t size);
		ssize_t	write(const char *string);
		ssize_t	write(const char *string, size_t size);
		ssize_t	write(char character);
		ssize_t	write(int16_t number);
		ssize_t	write(int32_t number);
		ssize_t	write(int64_t number);
		ssize_t	write(unsigned char character);
		ssize_t	write(uint16_t number);
		ssize_t	write(uint32_t number);
		ssize_t	write(uint64_t number);
		ssize_t	write(float number);
		ssize_t	write(double number);
	private:

		void	*apistruct;

		bool		envdirty;
		uint64_t	envcount;
		char		**envvars;
		char		**envvals;
