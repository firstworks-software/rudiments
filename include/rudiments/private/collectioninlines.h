// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

inline
collection::collection() :
	object(),
	managevalues(false),
	managearrayvalues(false),
	managekeys(false),
	managearraykeys(false),
	compptr(&comp),
	wrptr(&wr) {
}

inline
collection::collection(const collection &c) :
	object(c),
	managevalues(c.managevalues),
	managearrayvalues(c.managearrayvalues),
	managekeys(c.managekeys),
	managearraykeys(c.managearraykeys),
	comp(c.comp),
	compptr(&comp),
	wr(c.wr),
	wrptr(&wr) {
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
		wr=c.wr;
		wrptr=&wr;
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
writer *collection::getWriter() const {
	return wrptr;
}

inline
void collection::setWriter(writer *newwr) {
	if (newwr) {
		wrptr=newwr;
	} else {
		wrptr=&wr;
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
ssize_t collection::writeDelegate(output *out, const char *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, char *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, const wchar_t *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, wchar_t *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, char value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, wchar_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, int16_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, int32_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, int64_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out,
					const unsigned char *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out,
					unsigned char *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, unsigned char value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, uint16_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, uint32_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, uint64_t value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, float value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, double value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, long double value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, const void *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, void *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, const collection *value) const {
	return value->write(out);
}

inline
ssize_t collection::writeDelegate(output *out, collection *value) const {
	return value->write(out);
}

inline
ssize_t collection::writeDelegate(output *out, const collection &value) const {
	return value.write(out);
}

inline
ssize_t collection::writeDelegate(output *out, collection &value) const {
	return value.write(out);
}

inline
ssize_t collection::writeDelegate(output *out, const object *value) const {
	return getWriter()->write(out,value);
}

inline
ssize_t collection::writeDelegate(output *out, object *value) const {
	return getWriter()->write(out,value);
}

template <class valuetype>
inline
ssize_t collection::writeDelegate(output *out, const valuetype &value) const {
	return getWriter()->write(out,(const void *)&value);
}

inline
bool collection::incOrErr(ssize_t *retval, ssize_t val) const {

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
