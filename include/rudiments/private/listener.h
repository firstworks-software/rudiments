// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		listener(listener &l) {};
		listener	&operator=(listener &l) { return *this; };

	private:
		void	addFileDescriptor(filedescriptor *fd,
						bool read, bool write);
		bool	rebuildMonitorList();
		void	cleanUp();

		listenerprivate	*pvt;
