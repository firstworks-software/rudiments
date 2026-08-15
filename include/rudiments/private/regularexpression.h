// Copyright (c) David Muse
// See the COPYING file for more information.

		regularexpression(regularexpression &r) {};
		regularexpression	&operator=(regularexpression &r) {
			return *this;
		};

	private:
		void	construct();

		// runs the engine against the last match() subject at
		// "offset" - the one place the engine is actually called
		bool	runMatch(int32_t offset);
		#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		bool	runRegexec(const char *subject, int32_t offset);
		#endif

		regularexpressionprivate	*pvt;
