// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/character.h>

inline
collection::collection() :
	object(),
	managevalues(false),
	managearrayvalues(false),
	managekeys(false),
	managearraykeys(false),
	compptr(&comp) {
}

inline
collection::collection(collection &c) :
	object(c),
	managevalues(c.managevalues),
	managearrayvalues(c.managearrayvalues),
	managekeys(c.managekeys),
	managearraykeys(c.managearraykeys),
	comp(c.comp),
	compptr(&comp) {
}

inline
collection &collection::operator=(collection &c) {
	if (this!=&c) {
		object::operator=(c);
		managevalues=c.managevalues;
		managearrayvalues=c.managearrayvalues;
		managekeys=c.managekeys;
		managearraykeys=c.managearraykeys;
		comp=c.comp;
		compptr=&comp;
	}
	return *this;
}

inline
collection::~collection() {
}

inline
comparator *collection::getComparator() {
	return compptr;
}

inline
void collection::setComparator(comparator *newcomp) {
	if (newcomp) {
		compptr=newcomp;
	} else {
		compptr=&comp;
	}
}

inline
void collection::setManageValues(bool manage) {
	managevalues=manage;
	managearrayvalues=false;
}

inline
bool collection::isReadOnly() {
	return false;
}

inline
bool collection::isBlockBased() {
	return false;
}

inline
uint64_t collection::getBlockSize() {
	return 0;
}

inline
bool collection::isSequentialAccess() {
	return false;
}

inline
bool collection::getManageValues() {
	return managevalues;
}

inline
void collection::setManageArrayValues(bool manage) {
	managearrayvalues=manage;
	managevalues=false;
}

inline
bool collection::getManageArrayValues() {
	return managearrayvalues;
}

inline
void collection::setManageKeys(bool manage) {
	managekeys=manage;
	managearraykeys=false;
}

inline
bool collection::getManageKeys() {
	return managekeys;
}

inline
void collection::setManageArrayKeys(bool manage) {
	managearraykeys=manage;
	managekeys=false;
}

inline
bool collection::getManageArrayKeys() {
	return managearraykeys;
}

inline
ssize_t collection::writeValue(output *out, const char *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, char *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, const wchar_t *value) {
	return out->printf(L"%s",value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t *value) {
	return out->printf(L"%s",value);
}

inline
ssize_t collection::writeValue(output *out, char value) {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t value) {
	return out->printf(L"%c",value);
}

inline
ssize_t collection::writeValue(output *out, bool value) {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeValue(output *out, int16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeValue(output *out, int32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeValue(output *out, int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out, const byte_t *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, byte_t *value) {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, byte_t value) {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, uint16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeValue(output *out, uint32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeValue(output *out, uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out, float value) {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, double value) {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, long double value) {
	return out->printf("%Lf",value);
}

inline
ssize_t collection::writeValue(output *out, const void *value) {
	return out->printf("%08x",value);
}

inline
ssize_t collection::writeValue(output *out, void *value) {
	return out->printf("%08x",value);
}

inline
ssize_t collection::writeValue(output *out, const object *value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)value);
}

inline
ssize_t collection::writeValue(output *out, object *value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeValue(output *out, const valuetype &value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)&value);
}

inline
ssize_t collection::writeJsonValue(output *out, const char *value) {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber(value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, char *value) {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber(value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const wchar_t *value) {
	if (!value) {
		return out->write("null");
	} else if (wcharstring::isNumber(value)) {
		return out->printf(L"%s",value);
	}
	return out->printf(L"\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t *value) {
	if (!value) {
		return out->write("null");
	} else if (wcharstring::isNumber(value)) {
		return out->printf(L"%s",value);
	}
	return out->printf(L"\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, char value) {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return out->printf("\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t value) {
	if (character::isDigit(value)) {
		return out->printf(L"%c",value);
	}
	return out->printf(L"\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, bool value) {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeJsonValue(output *out, int16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out, const byte_t *value) {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber((const char *)value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, byte_t *value) {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber((char *)value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, byte_t value) {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return out->printf("\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint16_t value) {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint32_t value) {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out, float value) {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeJsonValue(output *out, double value) {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeJsonValue(output *out, long double value) {
	return out->printf("%Lf",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const void *value) {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%08x\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, void *value) {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%08x\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const object *value) {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)value);
}

inline
ssize_t collection::writeJsonValue(output *out, object *value) {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeJsonValue(output *out, const valuetype &value) {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)&value);
}
