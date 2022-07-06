// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		ssize_t	writeDelegate(output *out,
					const char *value) const;
		ssize_t	writeDelegate(output *out,
					char *value) const;
		ssize_t	writeDelegate(output *out,
					const wchar_t *value) const;
		ssize_t	writeDelegate(output *out,
					wchar_t *value) const;
		ssize_t	writeDelegate(output *out,
					char value) const;
		ssize_t	writeDelegate(output *out,
					wchar_t value) const;
		ssize_t	writeDelegate(output *out,
					int16_t value) const;
		ssize_t	writeDelegate(output *out,
					int32_t value) const;
		ssize_t	writeDelegate(output *out,
					int64_t value) const;
		ssize_t	writeDelegate(output *out,
					const unsigned char *value) const;
		ssize_t	writeDelegate(output *out,
					unsigned char *value) const;
		ssize_t	writeDelegate(output *out,
					unsigned char value) const;
		ssize_t	writeDelegate(output *out,
					uint16_t value) const;
		ssize_t	writeDelegate(output *out,
					uint32_t value) const;
		ssize_t	writeDelegate(output *out,
					uint64_t value) const;
		ssize_t	writeDelegate(output *out,
					float value) const;
		ssize_t	writeDelegate(output *out,
					double value) const;
		ssize_t	writeDelegate(output *out,
					long double value) const;
		ssize_t	writeDelegate(output *out,
					const void *value) const;
		ssize_t	writeDelegate(output *out,
					void *value) const;
		ssize_t	writeDelegate(output *out,
					const object *value) const;
		ssize_t	writeDelegate(output *out,
					object *value) const;
		ssize_t	writeDelegate(output *out,
					const collection *value) const;
		ssize_t	writeDelegate(output *out,
					collection *value) const;
		ssize_t	writeDelegate(output *out,
					const collection &value) const;
		ssize_t	writeDelegate(output *out,
					collection &value) const;
		template <class valuetype>
		ssize_t	writeDelegate(output *out,
					const valuetype &value) const;

	protected:
		bool	incOrErr(ssize_t *retval, ssize_t val) const;

		bool	managevalues;
		bool	managearrayvalues;
		bool	managekeys;
		bool	managearraykeys;

		comparator	comp;
		comparator	*compptr;

		writer		wr;
		writer		*wrptr;
