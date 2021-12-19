// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVC_H
#define RUDIMENTS_MVC_H 1

#include <rudiments/private/mvcincludes.h>

class RUDIMENTS_DLLSPEC mvcsecurity : public object {
	public:
		mvcsecurity() {};
		virtual	~mvcsecurity() {};
		virtual const char	*getUserName()=0;
};

class RUDIMENTS_DLLSPEC mvcproperties : public object {
	public:
		mvcproperties();
		virtual	~mvcproperties();

		bool		parseFile(const char *filename);
		bool		parseString(const char *string);

		const char	*getValue(const char *parameter);

	#include <rudiments/private/mvcproperties.h>
};

class RUDIMENTS_DLLSPEC mvctier : public object {
	public:
		mvctier();
		virtual ~mvctier();

		void		setProperties(mvcproperties *prop);
		mvcproperties	*getProperties();

		void		setSecurity(mvcsecurity *sec);
		mvcsecurity	*getSecurity();

	#include <rudiments/private/mvctier.h>
};

class RUDIMENTS_DLLSPEC mvccontroller : public mvctier {
};

class RUDIMENTS_DLLSPEC mvcview : public mvctier {
	public:
		mvcview();
		virtual ~mvcview();

		void	setRequest(httprequest *req);
		httprequest	*getRequest();

		void	setResponse(httpresponse *resp);
		httpresponse	*getResponse();

	#include <rudiments/private/mvcview.h>
};

class RUDIMENTS_DLLSPEC mvcservice : public mvctier {
};

class RUDIMENTS_DLLSPEC mvcdao : public mvctier {
};

class RUDIMENTS_DLLSPEC mvcresult : public object {
	public:
		mvcresult();
		virtual ~mvcresult();

		void	setResult(bool success);
		void	setResult(bool success, uint32_t code);
		void	setResult(bool success, uint32_t code,
						const char *message);

		void	setSuccess();
		void	setSuccess(uint32_t code);
		void	setSuccess(uint32_t code, const char *message);

		void	setFailed();
		void	setFailed(uint32_t code);
		void	setFailed(uint32_t code, const char *message);

		void	setSuccess(bool success);
		bool	getSuccess();

		void		setCode(uint32_t code);
		uint32_t	getCode();

		void		setMessage(const char *message);
		const char	*getMessage();

		void	attachData(const char *key,
					const char *type,
					collection *data);

		linkedlist<char *>	*getKeys();
		const char		*getType(const char *key);
		collection		*getData(const char *key);

		wastebasket	*getWastebasket();

	#include <rudiments/private/mvcresult.h>
};

#endif
