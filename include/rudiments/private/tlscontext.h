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

		// callbacks of a custom BIO that reads and writes through
		// a filedescriptor.  They're members because friendship
		// with filedescriptor doesn't extend to free functions.
		static int	bioRead(struct bio_st *b, char *buf, int len);
		static int	bioWrite(struct bio_st *b,
						const char *buf, int len);
		static long	bioCtrl(struct bio_st *b, int cmd,
						long num, void *ptr);
		static int	bioCreate(struct bio_st *b);
		static int	bioDestroy(struct bio_st *b);

		tlscontextprivate	*pvt;
