// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class securitycontext;
	friend class gsscontext;
	friend class tlscontext;
	protected:

		void	construct();

		bool	setStreamWriteBufferSize(size_t size);
		bool	setStorageWriteBufferSize(size_t size);
		bool	unsetStreamWriteBuffer();
		bool	unsetStorageWriteBuffer();

		bool	setStreamReadBufferSize(size_t size);
		bool	setStorageReadBufferSize(size_t size);
		bool	unsetStreamReadBuffer();
		bool	unsetStorageReadBuffer();

		void	allocateWriteBuffer(size_t size);

		off64_t	setPosition(off64_t offset, int32_t whence);
		off64_t	getSize();
		off64_t	lseek(off64_t offset, int32_t whence);

		ssize_t	realignWriteBuffer(int32_t sec, int32_t usec);

		ssize_t	highLevelRead(byte_t *buf, size_t count);
		ssize_t	highLevelRead(byte_t *buf, size_t count,
						int32_t sec, int32_t usec);
		ssize_t	highLevelWrite(const byte_t *buf, size_t count);
		ssize_t	highLevelWrite(const byte_t *buf, size_t count,
						int32_t sec, int32_t usec);
		ssize_t	streamBufferedRead(byte_t *buf, size_t count,
						int32_t sec, int32_t usec);
		ssize_t	streamBufferedWrite(const byte_t *buf,
						size_t count,
						int32_t sec, int32_t usec);
		ssize_t	storageBufferedRead(byte_t *buf, size_t count,
						int32_t sec, int32_t usec);
		ssize_t	storageBufferedWrite(const byte_t *buf,
						size_t count,
						int32_t sec, int32_t usec);

		ssize_t	unBufferedRead(byte_t *buf, size_t count,
						int32_t sec, int32_t usec);
		ssize_t	unBufferedWrite(const byte_t *buf, size_t count,
						int32_t sec, int32_t usec);

		int32_t	getSockOpt(int32_t level, int32_t optname,
					void *optval, socklen_t *optlen);
		int32_t	setSockOpt(int32_t level, int32_t optname,
					const void *optval, socklen_t optlen);

		virtual	ssize_t	lowLevelRead(void *buf,
						size_t count);
		virtual	ssize_t	lowLevelWrite(const void *buf,
						size_t count);
		virtual int32_t	lowLevelClose();

		const char	*type();
		void		type(const char *tp);

		int32_t	fd();
		void	fd(int32_t filedes);

		socketlayer	*socklr();

	private:
		filedescriptor(filedescriptor &f) {};
		filedescriptor	&operator=(filedescriptor &f) { return *this; };

		filedescriptorprivate	*pvt;

	public:
		filedescriptor(int32_t fd);
		static void	*getHandleFromFileDescriptor(int32_t fd);
		size_t	getBytesBuffered();

		ssize_t	printfDelegate(const char *format, va_list *argp);
		ssize_t	printfDelegate(const wchar_t *format, va_list *argp);
		ssize_t	printfUcs2Delegate(const ucs2_t *format, va_list *argp);


