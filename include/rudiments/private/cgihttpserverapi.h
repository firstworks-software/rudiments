// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		ssize_t	read(byte_t *buffer, size_t size);
		ssize_t	read(char *buffer, size_t length);
		ssize_t	read(char *character);
		ssize_t	read(wchar_t *buffer, size_t length);
		ssize_t	read(wchar_t *character);
		ssize_t	readUcs2(ucs2_t *buffer, size_t length);
		ssize_t	readUcs2(ucs2_t *character);
		ssize_t	read(int16_t *number);
		ssize_t	read(int32_t *number);
		ssize_t	read(int64_t *number);
		ssize_t	read(byte_t *character);
		ssize_t	read(uint16_t *number);
		ssize_t	read(uint32_t *number);
		ssize_t	read(uint64_t *number);
		ssize_t	read(float *number);
		ssize_t	read(double *number);

		void		initEnvironmentVariables();
		const char	*getEnvironmentVariable(const char *name);
		uint64_t	getEnvironmentVariableCount();
		const char * const	*getEnvironmentVariables();
		const char * const	*getEnvironmentValues();
		bool		setEnvironmentVariable(const char *name,
							const char *value);
		void		updateEnvironmentVariables();

		void	writeStatusLine(const char *string);
		void	writeStatusHeader(const char *string);
		void	writeHeader(const char *header, const char *value);
		void	writeHeaderTerminator();

		ssize_t	write(const byte_t *string, size_t size);
		ssize_t	write(const char *string);
		ssize_t	write(const char *string, size_t size);
		ssize_t	write(char character);
		ssize_t	write(const wchar_t *string);
		ssize_t	write(const wchar_t *string, size_t size);
		ssize_t	write(wchar_t character);
		ssize_t	writeUcs2(const ucs2_t *string);
		ssize_t	writeUcs2(const ucs2_t *string, size_t size);
		ssize_t	writeUcs2(ucs2_t character);
		ssize_t	write(int16_t number);
		ssize_t	write(int32_t number);
		ssize_t	write(int64_t number);
		ssize_t	write(byte_t character);
		ssize_t	write(uint16_t number);
		ssize_t	write(uint32_t number);
		ssize_t	write(uint64_t number);
		ssize_t	write(float number);
		ssize_t	write(double number);

		ssize_t	printfDelegate(const char *format, va_list *argp);
		ssize_t	printfDelegate(const wchar_t *format, va_list *argp);
		ssize_t	printfUcs2Delegate(const ucs2_t *format, va_list *argp);

		cgihttpserverapi(cgihttpserverapi &c) {};
		cgihttpserverapi	&operator=(cgihttpserverapi &c) {
			return *this;
		}

	private:
		cgihttpserverapiprivate	*pvt;
