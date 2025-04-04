// Copyright (c) David Muse
// See the COPYING file for more information.

	friend class logger;
	public:
		virtual	~logdestination();
	protected:
		virtual void	write(const char *string)=0;
		virtual void	write(const wchar_t *string)=0;
