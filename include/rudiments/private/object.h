// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		bool	incOrErr(ssize_t *retval,
					ssize_t val) const;
		bool	incOrErr(ssize_t *retval,
					ssize_t val, ssize_t expected) const;
