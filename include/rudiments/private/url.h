// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		bool	getCurrentProperties();

		char	*getContents();
		ssize_t	getContents(byte_t *buffer,size_t buffersize);

		static	char	*getContents(const char *name);
		static	ssize_t	getContents(const char *name,
						byte_t *buffer,
						size_t buffersize);

		void	enableOpenTimings();
		void	disableOpenTimings();
		void	getOpenTimings(uint64_t *build,
					uint64_t *connect,
					uint64_t *send,
					uint64_t *wait,
					uint64_t *recv);

	protected:
		bool	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);
		int32_t	lowLevelClose();

		ssize_t	lowLevelRead(void *buffer, size_t size);
	private:
		url(url &u) {};
		url	&operator=(url &u) { return *this; };

		void	init();

		bool	httpOpen(const char *urlname, char *userpwd);
		bool	curlOpen(const char *urlname, char *userpwd);
		bool	getChunkSize(bool bof);

		bool	curlPerform();
		static	size_t	curlReadData(void *buffer, size_t size,
						size_t nmemb, void *userp);

		static	bool	initUrl();
		static	void	shutDownUrl();

		uint64_t	interval();

		urlprivate	*pvt;
