// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		ssize_t	writeValue(output *out,
					const char *value) const;
		ssize_t	writeValue(output *out,
					char *value) const;
		ssize_t	writeValue(output *out,
					const wchar_t *value) const;
		ssize_t	writeValue(output *out,
					wchar_t *value) const;
		ssize_t	writeValue(output *out,
					char value) const;
		ssize_t	writeValue(output *out,
					wchar_t value) const;
		ssize_t	writeValue(output *out,
					bool value) const;
		ssize_t	writeValue(output *out,
					int16_t value) const;
		ssize_t	writeValue(output *out,
					int32_t value) const;
		ssize_t	writeValue(output *out,
					int64_t value) const;
		ssize_t	writeValue(output *out,
					const unsigned char *value) const;
		ssize_t	writeValue(output *out,
					unsigned char *value) const;
		ssize_t	writeValue(output *out,
					unsigned char value) const;
		ssize_t	writeValue(output *out,
					uint16_t value) const;
		ssize_t	writeValue(output *out,
					uint32_t value) const;
		ssize_t	writeValue(output *out,
					uint64_t value) const;
		ssize_t	writeValue(output *out,
					float value) const;
		ssize_t	writeValue(output *out,
					double value) const;
		ssize_t	writeValue(output *out,
					long double value) const;
		ssize_t	writeValue(output *out,
					const void *value) const;
		ssize_t	writeValue(output *out,
					void *value) const;
		ssize_t	writeValue(output *out,
					const object *value) const;
		ssize_t	writeValue(output *out,
					object *value) const;
		template <class valuetype>
		ssize_t	writeValue(output *out,
					const valuetype &value) const;

		ssize_t	writeJsonValue(output *out,
					const char *value) const;
		ssize_t	writeJsonValue(output *out,
					char *value) const;
		ssize_t	writeJsonValue(output *out,
					const wchar_t *value) const;
		ssize_t	writeJsonValue(output *out,
					wchar_t *value) const;
		ssize_t	writeJsonValue(output *out,
					char value) const;
		ssize_t	writeJsonValue(output *out,
					wchar_t value) const;
		ssize_t	writeJsonValue(output *out,
					bool value) const;
		ssize_t	writeJsonValue(output *out,
					int16_t value) const;
		ssize_t	writeJsonValue(output *out,
					int32_t value) const;
		ssize_t	writeJsonValue(output *out,
					int64_t value) const;
		ssize_t	writeJsonValue(output *out,
					const unsigned char *value) const;
		ssize_t	writeJsonValue(output *out,
					unsigned char *value) const;
		ssize_t	writeJsonValue(output *out,
					unsigned char value) const;
		ssize_t	writeJsonValue(output *out,
					uint16_t value) const;
		ssize_t	writeJsonValue(output *out,
					uint32_t value) const;
		ssize_t	writeJsonValue(output *out,
					uint64_t value) const;
		ssize_t	writeJsonValue(output *out,
					float value) const;
		ssize_t	writeJsonValue(output *out,
					double value) const;
		ssize_t	writeJsonValue(output *out,
					long double value) const;
		ssize_t	writeJsonValue(output *out,
					const void *value) const;
		ssize_t	writeJsonValue(output *out,
					void *value) const;
		ssize_t	writeJsonValue(output *out,
					const object *value) const;
		ssize_t	writeJsonValue(output *out,
					object *value) const;
		template <class valuetype>
		ssize_t	writeJsonValue(output *out,
					const valuetype &value) const;

	protected:
		bool	managevalues;
		bool	managearrayvalues;
		bool	managekeys;
		bool	managearraykeys;

		comparator	comp;
		comparator	*compptr;
