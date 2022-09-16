// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		compiler(compiler &c) {};
		compiler	&operator=(compiler &c) { return *this; };

		bool	appendModule(
				const char *filename,
				const char *classname,
				const char *id,
				const char *textparams,
				domnode *xmlparams,
				linkedlist< compilermodule * > *modulelist);

		compilerprivate	*pvt;
