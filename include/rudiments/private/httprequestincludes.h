// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/dictionary.h>

#include <rudiments/httpserverapi.h>

class RUDIMENTS_DLLSPEC fileparameter {
	friend class httprequest;
	private:
			fileparameter(const char *name, const char *filename, 
				const char *tempfilename, const char *mimetype);
			~fileparameter();
		char	*name;
		char	*filename;
		char	*tempfilename;
		char	*mimetype;
	public:
		void	print() {}
};
