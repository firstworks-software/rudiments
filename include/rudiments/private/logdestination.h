// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class logger;
	public:
		virtual	~logdestination();
	protected:
		virtual void	write(const char *string)=0;
