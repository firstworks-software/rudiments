// Copyright (c) David Muse
// See the COPYING file for more information.

		tlscontext(tlscontext &t) {};
		tlscontext	&operator=(tlscontext &t) { return *this; };
	
	private:
		void	initContext();
		void	initSubContext();
		void	freeContext();
		void	freeSubContext();
		bool	reInit(bool isclient);
		bool	isPeerCertValid();
		bool	loadPeerCert();
		void	clearError();
		void	setError(int32_t ret);
		void	setError(int32_t err, const char *errstr);
		void	setNativeError();

		// These are the callbacks of a custom BIO that reads
		// and writes through the filedescriptor, rather than
		// through the raw file descriptor.  They must be members
		// of tlscontext.  filedescriptor::lowLevelRead() and
		// lowLevelWrite() are protected, tlscontext is a friend
		// of filedescriptor, and friendship doesn't extend to
		// free functions.
		static int	bioRead(struct bio_st *b, char *buf, int len);
		static int	bioWrite(struct bio_st *b,
						const char *buf, int len);
		static long	bioCtrl(struct bio_st *b, int cmd,
						long num, void *ptr);
		static int	bioCreate(struct bio_st *b);
		static int	bioDestroy(struct bio_st *b);

		tlscontextprivate	*pvt;
