// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#ifndef RUDIMENTS_MVCRESULT_H
#define RUDIMENTS_MVCRESULT_H 1

#include <rudiments/private/mvcresultincludes.h>

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

	#include <rudiments/private/mvcresult.h>
};

#endif
