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
collection::collection(const collection &c) :
	object(c),
	managevalues(c.managevalues),
	managearrayvalues(c.managearrayvalues),
	managekeys(c.managekeys),
	managearraykeys(c.managearraykeys),
	comp(c.comp),
	compptr(&comp) {
}

inline
collection &collection::operator=(const collection &c) {
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
comparator *collection::getComparator() const {
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
bool collection::getIsReadOnly() const {
	return false;
}

inline
bool collection::getIsBlockBased() const {
	return false;
}

inline
uint64_t collection::getBlockSize() const {
	return 0;
}

inline
bool collection::getIsSequentialAccess() const {
	return false;
}

inline
bool collection::getManageValues() const {
	return managevalues;
}

inline
void collection::setManageArrayValues(bool manage) {
	managearrayvalues=manage;
	managevalues=false;
}

inline
bool collection::getManageArrayValues() const {
	return managearrayvalues;
}

inline
void collection::setManageKeys(bool manage) {
	managekeys=manage;
	managearraykeys=false;
}

inline
bool collection::getManageKeys() const {
	return managekeys;
}

inline
void collection::setManageArrayKeys(bool manage) {
	managearraykeys=manage;
	managekeys=false;
}

inline
bool collection::getManageArrayKeys() const {
	return managearraykeys;
}

inline
ssize_t collection::writeValue(output *out, const char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, const wchar_t *value) const {
	return out->printf(L"%s",value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t *value) const {
	return out->printf(L"%s",value);
}

inline
ssize_t collection::writeValue(output *out, char value) const {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, wchar_t value) const {
	return out->printf(L"%c",value);
}

inline
ssize_t collection::writeValue(output *out, bool value) const {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeValue(output *out, int16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeValue(output *out, int32_t value) const {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeValue(output *out, int64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out,
					const unsigned char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out,
					unsigned char *value) const {
	return out->printf("%s",value);
}

inline
ssize_t collection::writeValue(output *out, unsigned char value) const {
	return out->printf("%c",value);
}

inline
ssize_t collection::writeValue(output *out, uint16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeValue(output *out, uint32_t value) const {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeValue(output *out, uint64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeValue(output *out, float value) const {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, double value) const {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeValue(output *out, long double value) const {
	return out->printf("%Lf",value);
}

inline
ssize_t collection::writeValue(output *out, const void *value) const {
	return out->printf("%08x",value);
}

inline
ssize_t collection::writeValue(output *out, void *value) const {
	return out->printf("%08x",value);
}

inline
ssize_t collection::writeValue(output *out, const object *value) const {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)value);
}

inline
ssize_t collection::writeValue(output *out, object *value) const {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeValue(output *out, const valuetype &value) const {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("%08x",(const void *)&value);
}

inline
ssize_t collection::writeJsonValue(output *out, const char *value) const {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber(value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, char *value) const {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber(value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const wchar_t *value) const {
	if (!value) {
		return out->write("null");
	} else if (wcharstring::isNumber(value)) {
		return out->printf(L"%s",value);
	}
	return out->printf(L"\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t *value) const {
	if (!value) {
		return out->write("null");
	} else if (wcharstring::isNumber(value)) {
		return out->printf(L"%s",value);
	}
	return out->printf(L"\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, char value) const {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return out->printf("\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, wchar_t value) const {
	if (character::isDigit(value)) {
		return out->printf(L"%c",value);
	}
	return out->printf(L"\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, bool value) const {
	return out->printf("%s",(value)?"true":"false");
}

inline
ssize_t collection::writeJsonValue(output *out, int16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int32_t value) const {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeJsonValue(output *out, int64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out,
					const unsigned char *value) const {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber((const char *)value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out,
					unsigned char *value) const {
	if (!value) {
		return out->write("null");
	} else if (charstring::isNumber((char *)value)) {
		return out->printf("%s",value);
	}
	return out->printf("\"%s\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, unsigned char value) const {
	if (character::isDigit(value)) {
		return out->printf("%c",value);
	}
	return out->printf("\"%c\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint16_t value) const {
	return out->printf("%hd",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint32_t value) const {
	return out->printf("%d",value);
}

inline
ssize_t collection::writeJsonValue(output *out, uint64_t value) const {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		return out->printf("%lld",(long long)value);
	#else
		return out->printf("%ld",(long)value);
	#endif
}

inline
ssize_t collection::writeJsonValue(output *out, float value) const {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeJsonValue(output *out, double value) const {
	return out->printf("%f",value);
}

inline
ssize_t collection::writeJsonValue(output *out, long double value) const {
	return out->printf("%Lf",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const void *value) const {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%08x\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, void *value) const {
	if (!value) {
		return out->write("null");
	}
	return out->printf("\"%08x\"",value);
}

inline
ssize_t collection::writeJsonValue(output *out, const object *value) const {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)value);
}

inline
ssize_t collection::writeJsonValue(output *out, object *value) const {
	if (!value) {
		return out->write("null");
	}
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)value);
}

template <class valuetype>
inline
ssize_t collection::writeJsonValue(output *out, const valuetype &value) const {
	// Why this cast to const void *?  gcc 3.3.6 on openbsd 7.0 for luna88k
	// segfaults without it, and it doesn't hurt other compilers.
	return out->printf("\"%08x\"",(const void *)&value);
}

inline
bool collection::incOrErr(ssize_t *retval, ssize_t val) const {

	// FIXME: this is duplicated in mvcresult, move them somewhere

	// add val to *retval unless:
	// * retval is already negative, indicating that an error condition
	//   occurred previously, in this case leave retval set to the error
	//   condition
	// * val is negative, indicating an error condition just occurred, in
	//   this case set retval to the error condition
	// return true on success or false if an error condition occurred

	if (*retval>-1) {
		if (val>-1) {
			(*retval)+=val;
			return true;
		} else {
			(*retval)=val;
		}
	}
	return false;
}
