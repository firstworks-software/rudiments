// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>

inline
bool mvccrud::doCreate(const char * const *columns,
					const char * const *values) {
	return doCreate(columns,values,NULL);
}

inline
bool mvccrud::doUpdate(const char * const * columns,
					const char * const *values,
					const char *criteria) {
	return doUpdate(columns,values,NULL,criteria);
}

inline
const char *mvccrud::deriveDataType(const char *value) {
	if (!value) {
		return "u";
	}
	if (charstring::isInteger(value)) {
		return "n";
	}
	return "s";
}
