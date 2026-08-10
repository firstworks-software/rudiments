// Copyright (c) David Muse
// See the COPYING file for more information.

		regularexpression(regularexpression &r) {};
		regularexpression	&operator=(regularexpression &r) {
			return *this;
		};

	private:
		void	construct();

		// Runs the engine against the subject that the last
		// match() was given, starting at "offset".  match()
		// and matchNext() differ only in that offset, so this
		// is the one place the engine is actually called.
		bool	runMatch(int32_t offset);
		#if !defined(RUDIMENTS_HAS_PCRE2) && !defined(RUDIMENTS_HAS_PCRE)
		bool	runRegexec(const char *subject, int32_t offset);
		#endif

		regularexpressionprivate	*pvt;
