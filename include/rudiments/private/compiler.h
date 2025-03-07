// Copyright (c) David Muse
// See the COPYING file for more information.

		compiler(compiler &c) {};
		compiler	&operator=(compiler &c) { return *this; };

	private:
		bool	appendModule(
				const char *filename,
				const char *classname,
				const char *id,
				const char *textparams,
				domnode *xmlparams,
				linkedlist< compilermodule * > *modulelist);

		compilerprivate	*pvt;
