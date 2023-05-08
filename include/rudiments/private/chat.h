// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		chat(chat &c) {};
		chat	&operator=(chat &c) { return *this; };

	private:
		void	appendAbortString(const char *string);
		void	clearAbortStrings();
		int32_t	send(const char *string,
			dictionary<const char *, const char *> *variables);
		void	flush();
		int32_t	expect(const char *string, char **abort);
		int32_t	substituteVariables(const char **ch,
			dictionary<const char *, const char *> *variables);
		void	charUnescape(const char *str,
					char **newstr, bool second);

		chatprivate	*pvt;
