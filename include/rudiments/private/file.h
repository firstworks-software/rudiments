// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		virtual	bool	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);

	private:
		static	bool	stat(const char *filename, void *st);

		ssize_t	getContents(unsigned char **buffer,
						size_t *buffersize,
						bool terminate);

		bool	lock(int32_t method, int16_t type, int16_t whence,
					off64_t start, off64_t len);
		bool	checkLock(int16_t type,
					int16_t whence,
					off64_t start,
					off64_t len,
					int16_t *conftype,
					int16_t *confwhence,
					off64_t *confstart,
					off64_t *conflen);
		bool	unlock(int16_t whence,
					off64_t start, off64_t len);

		void	fileClone(file &f);


			bool	posixFadvise(off64_t offset, off64_t len,
							int32_t advice);
		static	int64_t	pathConf(const char *path, int32_t name);
			int64_t	fpathConf(int32_t name);

		fileprivate	*pvt;
