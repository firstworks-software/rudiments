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
bool collection::write() const {
	return write(&stdoutput);
}
